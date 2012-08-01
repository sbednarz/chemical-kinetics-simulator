// ----------------------------------------------------------------------------
//
// Copyright (C) 1996, 1998, 2012 International Business Machines Corporation
//   
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ----------------------------------------------------------------------------

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimsmop.cxx                                                */
/*                                                                          */
/*  This module implements a dialog box for setting the simulation options  */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for multiple platforms                                                  */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*  Version 1.0  started Aug 1993                                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_ENTRYFIELDS_IN_SIM_OPT_DLG  7
#define MAX_SEEDVAL                             30000

class SimOptionsDlg : public ModalDialog
{

protected :
     Edit NumMolsSLE;
     Edit RecordIntSLE;
     Edit RndmNoSeedSLE;
     FixedText aFixedText1;
     FixedText aFixedText2;
     FixedText aFixedText3;
     FixedText aFixedText5;
     FixedText aFixedText6;
     FixedText aFixedText7;
     Edit NumEventsSLE;
     Edit ElapsedTimeLimitSLE;
     RadioButton EquilDetectEnabledRB;
     RadioButton EquilDetDisabledRB;
     FixedText aFixedText8;
     FixedText aFixedText9;
     Edit CycleCountSLE;
     Edit MinEffSLE;
     FixedText ElapsedTimeUnitsLabel;
     FixedText aFixedText12;
     FixedText aFixedText13;
     GroupBox aGroupBox1;
     GroupBox aGroupBox2;
     GroupBox aGroupBox3;
     OKButton ClosePB;
     PushButton DefaultsPB;
     PushButton UndoPB;
     HelpButton HelpPB;

     msimPINSTANCE pSimInstance;
     msimWID owner;
     msimBOOL data_altered;

     void Initialize_Entries( msimPINSTANCE PSimInstance );
     void SetEnableDisableGroup( );
     msimBOOL ValidEntry( SHORT, PCHAR );

public :
     SimOptionsDlg( Window PTR pParent, msimPINSTANCE PSimInstance );

     void EnableDisableHandler( RadioButton PTR pRButton );

     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );

     void SetModifiedFlagHandler( Edit PTR pEdit );
};


// Constructor of SimOptionsDlg

SimOptionsDlg::SimOptionsDlg( Window PTR pParent, msimPINSTANCE PSimInstance ) :
ModalDialog ( pParent, ResId ( msimSIMULATION_OPTIONS_PANEL ) ),
NumMolsSLE ( this, ResId ( msimSIMOPT_NUM_MOLS ) ),
RecordIntSLE ( this, ResId ( msimSIMOPT_RECORD_INT ) ),
RndmNoSeedSLE ( this, ResId ( msimSIMOPT_RNDNOSEED ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aFixedText5 ( this, ResId ( 5 ) ),
aFixedText6 ( this, ResId ( 6 ) ),
aFixedText7 ( this, ResId ( 7 ) ),
NumEventsSLE ( this, ResId ( msimSIMOPT_NUMEVENTS ) ),
ElapsedTimeLimitSLE ( this, ResId ( msimSIMOPT_ET_LIMIT_VALUE ) ),
EquilDetectEnabledRB ( this, ResId ( msimSIMOPT_EQUILDETENABLED ) ),
EquilDetDisabledRB ( this, ResId ( msimSIMOPT_EQUILDET_DISABLED ) ),
aFixedText8 ( this, ResId ( 8 ) ),
aFixedText9 ( this, ResId ( 9 ) ),
CycleCountSLE ( this, ResId ( msimSIMOPT_CYCLECOUNT ) ),
MinEffSLE ( this, ResId ( msimSIMOPT_MINEFF ) ),
ElapsedTimeUnitsLabel ( this, ResId ( msimSIMOPT_ET_LIMITS_UNITS ) ),
aFixedText12 ( this, ResId ( 12 ) ),
aFixedText13 ( this, ResId ( 13 ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aGroupBox3 ( this, ResId ( 3 ) ),
ClosePB ( this, ResId ( msimSIMOPT_CLOSE ) ),
DefaultsPB ( this, ResId ( msimSIMOPT_DEFAULTS ) ),
UndoPB ( this, ResId ( msimSIMOPT_UNDO ) ),
HelpPB ( this, ResId ( msimSIMOPT_HELP ) )
{
     FreeResource( );
     pSimInstance = PSimInstance;
     owner = pParent;

     // Set the title of the dialog //

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize entries in the dialog box

     Initialize_Entries( pSimInstance );

     EquilDetectEnabledRB.ChangeClickHdl( LINK( this,
               SimOptionsDlg, EnableDisableHandler ) );
     EquilDetDisabledRB.ChangeClickHdl( LINK( this,
               SimOptionsDlg, EnableDisableHandler ) );

     ClosePB.ChangeClickHdl( LINK( this,
               SimOptionsDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl( LINK( this,
               SimOptionsDlg, UndoHandler ) );
     DefaultsPB.ChangeClickHdl( LINK( this,
               SimOptionsDlg, DefaultsHandler ) );

     NumEventsSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     NumMolsSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     RecordIntSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     RndmNoSeedSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     ElapsedTimeLimitSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     CycleCountSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );
     MinEffSLE.ChangeModifyHdl( LINK( this,
               SimOptionsDlg, SetModifiedFlagHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}

void SimOptionsDlg::Initialize_Entries(
          msimPINSTANCE
          PSimInstance )
{

     // Initialize entries in the dialog box

     NumEventsSLE.SetText( String( PSimInstance->optionvalue[1] ) );
     NumMolsSLE.SetText( String( PSimInstance->optionvalue[0] ) );
     RecordIntSLE.SetText( String( PSimInstance->optionvalue[2] ) );
     RndmNoSeedSLE.SetText( String( PSimInstance->optionvalue[3] ) );
     ElapsedTimeLimitSLE.SetText( String( PSimInstance->elapsed_time_limit ) );
     ElapsedTimeUnitsLabel.SetText( String( msimTimeUnits( PSimInstance->time_units ) ) );
     CycleCountSLE.SetText( String( PSimInstance->equil_data.cycle_length ) );
     MinEffSLE.SetText( String( PSimInstance->equil_data.min_eff ) );

     if ( PSimInstance->enable_equil_detect )
     {
          EquilDetectEnabledRB.Check( TRUE );
          EquilDetDisabledRB.Check( FALSE );
     }
     else
     {
          EquilDetectEnabledRB.Check( FALSE );
          EquilDetDisabledRB.Check( TRUE );
     }

     SetEnableDisableGroup( );

     data_altered = FALSE;

     return;
}


void SimOptionsDlg::SetEnableDisableGroup( )
{
     if ( EquilDetectEnabledRB.IsChecked( ) )
     {
          MinEffSLE.Enable( );
          CycleCountSLE.Enable( );
          aFixedText9.Enable( );
          aFixedText6.Enable( );
          aFixedText12.Enable( );
          aFixedText13.Enable( );

#if defined(__MAC__)

          SetActive( aFixedText9);
          SetActive( aFixedText6);
          SetActive( aFixedText12);
          SetActive( aFixedText13);
          SetActive( MinEffSLE);
          SetActive( CycleCountSLE);

#endif
     }
     else
     {
          MinEffSLE.Disable( );
          CycleCountSLE.Disable( );
          aFixedText9.Disable( );
          aFixedText6.Disable( );
          aFixedText12.Disable( );
          aFixedText13.Disable( );

#if defined(__MAC__)

          SetInactive( aFixedText9);
          SetInactive( aFixedText6);
          SetInactive( aFixedText12);
          SetInactive( aFixedText13);
          SetInactive( MinEffSLE);
          SetInactive( CycleCountSLE);

#endif

     }

     return;
}

void SimOptionsDlg::EnableDisableHandler( RadioButton PTR pRButton )
{

#if defined(__MAC__)

     // this deals with SV bug in implementing aut radiobuttons

     EquilDetectEnabledRB.Check( pRButton == &EquilDetectEnabledRB );
     EquilDetDisabledRB.Check( pRButton == &EquilDetDisabledRB );

#endif

     // Enable/disable Equilibrium Test Cycle Length and Selection Frequency entries

     data_altered = TRUE;
     SetEnableDisableGroup( );

     return;
}


void SimOptionsDlg::CloseHandler( PushButton PTR )
{
     //  Check for the validity of the user inputs
     USHORT j, i = 5;

     SHORT objectid[NUM_ENTRYFIELDS_IN_SIM_OPT_DLG];
     msimREAL_STRING tmp[NUM_ENTRYFIELDS_IN_SIM_OPT_DLG];
     msimBOOL equil_detect_on;

     // If nothing has been changed, then we don't need to
     // re-store the same data in the data structure
     //  so simply close the dialog box and leave.                                                      //
     // if the data has been altered since last save then,
     // set the appropriate flag, validate the data and end the dlg

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          // first test whether equil_detect is on

          equil_detect_on = EquilDetectEnabledRB.IsChecked( );

          // read the entry fields (i.e., Field ID and the User Entry )
          // into arrays (tmp and objectid)

          objectid[0] = msimSIMOPT_NUM_MOLS;
          msimStringCopy( tmp[0], NumMolsSLE.GetText( ), sizeof tmp[0] );

          objectid[1] = msimSIMOPT_NUMEVENTS;
          msimStringCopy( tmp[1], NumEventsSLE.GetText( ), sizeof tmp[1] );

          objectid[2] = msimSIMOPT_RECORD_INT;
          msimStringCopy( tmp[2], RecordIntSLE.GetText( ), sizeof tmp[2] );

          objectid[3] = msimSIMOPT_RNDNOSEED;
          msimStringCopy( tmp[3], RndmNoSeedSLE.GetText( ), sizeof tmp[3] );

          objectid[4] = msimSIMOPT_ET_LIMIT_VALUE;
          msimStringCopy( tmp[4], ElapsedTimeLimitSLE.GetText( ), sizeof tmp[4] );


          if ( equil_detect_on )
          {
               i += 2;
               objectid[5] = msimSIMOPT_CYCLECOUNT;
               msimStringCopy( tmp[5], CycleCountSLE.GetText( ), sizeof tmp[5] );

               objectid[6] = msimSIMOPT_MINEFF;
               msimStringCopy( tmp[6], MinEffSLE.GetText( ), sizeof tmp[6] );
          }

          // test for valid input in entry fields //

          for ( j = 0; j < i; j++ )
               if ( ! ValidEntry( objectid[j], tmp[j] ) )
                    return;

          // if everything is valid, then copy the
          // information to the pSimInstance data structure

          msimStringCopy( pSimInstance->optionvalue[0], tmp[0],
               sizeof pSimInstance->optionvalue[0] );
          msimStringCopy( pSimInstance->optionvalue[1], tmp[1],
               sizeof pSimInstance->optionvalue[1] );
          msimStringCopy( pSimInstance->optionvalue[2], tmp[2],
               sizeof pSimInstance->optionvalue[2] );
          msimStringCopy( pSimInstance->optionvalue[3], tmp[3],
               sizeof pSimInstance->optionvalue[3] );
          msimStringCopy( pSimInstance->elapsed_time_limit, tmp[4],
               sizeof pSimInstance->optionvalue[4] );

          if ( equil_detect_on )
          {
               msimStringCopy( pSimInstance->equil_data.cycle_length, tmp[5],
                    sizeof pSimInstance->equil_data.cycle_length );
               msimStringCopy( pSimInstance->equil_data.min_eff, tmp[6],
                    sizeof pSimInstance->equil_data.min_eff );
          }

          pSimInstance->enable_equil_detect = equil_detect_on;

          pSimInstance->data_altered_since_lastsave = TRUE;

     }

     // End the dialog box.

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}

void SimOptionsDlg::UndoHandler( PushButton PTR )
{
     //   Undo the changes made by the user to the original state.

     Initialize_Entries( pSimInstance );

     data_altered = FALSE;

     return;

}

void SimOptionsDlg::DefaultsHandler( PushButton PTR )
{

//    Obtain the default state.

     Initialize_Entries( &msimC_INSTANCE );

     data_altered = TRUE;

     return;

}


msimBOOL SimOptionsDlg::ValidEntry( SHORT PFieldID, PCHAR UserEntry )
{
     // Checks user input to simulation options dialog for errors.
     // Creates a dialog box  notitying user of an error in user
     // input in an entry field.  The message is tailored depending on
     // the identity of the entry field to assist the user.
     // returns TRUE if all entries are valid, FALSE otherwise.

     LONG testint;

     msimFLOAT testfloat;
     PCHAR ptr;
     String field;
     String message;

     msimBOOL error_found = FALSE;

     // use different criteria for judging validity of user input
     // depending on the entry field ID

     switch ( PFieldID )
     {
     case msimSIMOPT_NUM_MOLS :
          if ( msimValidPositiveLongInteger( UserEntry ) )
          {
               if ( strtoul( UserEntry, &ptr, 10 ) > LONG_MAX )
               {
                    message = String( ResId( msimSIMOPT_DLG_STR_1 ) ) +
                    String( ( long ) ( LONG_MAX ) ) +
                    String( ResId( msimSIMOPT_DLG_STR_2 ) ) +
                    String( UserEntry ) +  String( ResId( msimSIMOPT_DLG_STR_3 ) );


                    WarningBox box( this, WB_OK | WB_DEF_OK, message );

                    box.Execute( );
                    return FALSE;
               }
          }
          else
          {
               error_found = TRUE;
               field = String( ResId( msimSIMOPT_DLG_STR_4 ) );
          }
          break;

     case msimSIMOPT_NUMEVENTS :
          if ( ! msimValidPositiveULongInteger( UserEntry ) )
          {
               error_found = TRUE;
               field = String( ResId( msimSIMOPT_DLG_STR_5 ) );
          }
          break;

     case msimSIMOPT_RECORD_INT :
          if ( ! msimValidPositiveLongInteger( UserEntry ) )
          {
               error_found = TRUE;
               field = String( ResId( msimSIMOPT_DLG_STR_6 ) );
          }
          break;

     case msimSIMOPT_RNDNOSEED :
          if ( msimValidNonNegativeShortInteger( UserEntry ) )
          {
               testint = atol( UserEntry );
               if ( testint <= 0 || testint > MAX_SEEDVAL )
               {
                    message = String( ResId( msimSIMOPT_DLG_STR_7 ) ) +
                     String( msimMACRO_AS_STR( MAX_SEEDVAL ) ) +
                     String( ResId( msimSIMOPT_DLG_STR_8 ) ) +
                    String( UserEntry ) +
                    String( ResId( msimSIMOPT_DLG_STR_9 ) ) ;
                    WarningBox box( this, WB_OK | WB_DEF_OK, message );

                    box.Execute( );
                    return FALSE;
               }
          }
          else
          {
               error_found = TRUE;
               field = String( ResId( msimSIMOPT_DLG_STR_10 ) );
          }
          break;

     case msimSIMOPT_CYCLECOUNT :
          if ( ! msimValidPositiveLongInteger( UserEntry ) )
          {
               message =  String( ResId( msimSIMOPT_DLG_STR_11 ) ) +
                String(( long ) ( LONG_MAX ) ) +
                String( ResId( msimSIMOPT_DLG_STR_12 ) ) +
                String( UserEntry) +
                String( ResId( msimSIMOPT_DLG_STR_13 ) );

               WarningBox box( this, WB_OK | WB_DEF_OK, message );

               box.Execute( );
               return FALSE;

          }
          break;

     case msimSIMOPT_MINEFF :
          if ( msimValidFloat( UserEntry ) )
          {
               testfloat = atof( UserEntry );
               if ( testfloat < 0.0 || testfloat > 100.0 )
               {
                    message = String( ResId( msimSIMOPT_DLG_STR_14 ) ) +
                    String( UserEntry ) +
                     String( ResId( msimSIMOPT_DLG_STR_15 ) );
                    WarningBox box( this, WB_OK | WB_DEF_OK, message );

                    box.Execute( );
                    return FALSE;

               }
          }
          else
          {
               error_found = TRUE;
               field =  String( ResId( msimSIMOPT_DLG_STR_16 ) );
          }
          break;

     case msimSIMOPT_TCONVSTD :
          if ( ! msimValidFloat( UserEntry ) )
          {
               error_found = TRUE;
               field =  String( ResId( msimSIMOPT_DLG_STR_17 ) );
          }
          break;

     case msimSIMOPT_ET_LIMIT_VALUE :
          if ( ! msimValidNonNegativeFloat( UserEntry ) )
          {
               error_found = TRUE;
               field = String( ResId( msimSIMOPT_DLG_STR_18 ) );
          }
          break;

     default :
          break;

     }                                 // End of switch //

     if ( error_found )
     {
          message = String( ResId( msimSIMOPT_DLG_STR_19 ) ) +
          String( UserEntry ) +
          String( ResId( msimSIMOPT_DLG_STR_20 ) ) +
          String( field) +
          String( ResId( msimSIMOPT_DLG_STR_21 ) );

          WarningBox box( this, WB_OK | WB_DEF_OK, message );

          box.Execute( );
          return FALSE;

     }
     else
          return TRUE;
}

void SimOptionsDlg::SetModifiedFlagHandler( Edit PTR
     )
{
     data_altered = TRUE;
     return;
}

VOID msimSimulationOptionDialog( msimWID Owner, msimPINSTANCE pInstance )
{
     SimOptionsDlg PTR pdlg =
                       new SimOptionsDlg( Owner, pInstance );

     if ( pdlg )
     {
          pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}
