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
/*  FILE NAME : msimtpgp.cxx                                                */
/*                                                                          */
/*  This module implementa dialog for viewing and editing the linear        */
/*  temperature program                                                     */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for mutliple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1993                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"
#include "msimtprg.hxx"

#include <string.h>
#include <stdlib.h>


class LinearTempProgDlg : public ModalDialog
{
protected :
     FixedText EditLabel1;
     FixedText EditLabel2;
     FixedText InitTempLabel;
     Edit InitTempSLE;
     FixedText aTempUnitsLabel;
     FixedText CoeffALabel;
     Edit CoeffASLE;
     FixedText CoeffAUnitsLabel;
     FixedText aFixedText5;
     Edit MaxTempStepSLE;
     FixedText aFixedText7;
     FixedText aFixedText6;
     Edit MaxTempSLE;
     FixedText aFixedText8;
     OKButton ClosePB;
     PushButton UndoPB;
     PushButton DefaultsPB;
     HelpButton HelpPB;
     GroupBox aGroupBox1;
     GroupBox aGroupBox2;

     msimPINSTANCE pSimInstance;
     msimBOOL data_altered;
     msimWID owner;

     void InitializeEntries( msimPINSTANCE PSimInstance );
     void SetModifiedFlagHandler( Edit PTR pEdit );
     void SendErrorMesg( const String& rField, PCHAR Value );

public :

     LinearTempProgDlg( Window PTR pParent, msimPINSTANCE PSimInstance );

     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );
};



LinearTempProgDlg::
LinearTempProgDlg( Window PTR pParent, msimPINSTANCE PSimInstance )
: ModalDialog ( pParent, ResId ( msimTEMP_PROGR_PANEL ) ),
EditLabel1 ( this, ResId ( msimTPROG_EDIT_LABEL_1 ) ),
EditLabel2 ( this, ResId ( msimTPROG_EDIT_LABEL_2 ) ),
InitTempLabel ( this, ResId ( msimTPE_INIT_TEMP_LABEL ) ),
InitTempSLE ( this, ResId ( msimTPE_INIT_TEMP ) ),
aTempUnitsLabel ( this, ResId ( msimTPE_TEMP_UNIT_LABEL ) ),
CoeffALabel ( this, ResId ( msimTPE_A_LABEL ) ),
CoeffASLE ( this, ResId ( msimTPE_A ) ),
CoeffAUnitsLabel ( this, ResId ( msimTPE_A_UNIT_LABEL ) ),
aFixedText5 ( this, ResId ( 5 ) ),
MaxTempStepSLE ( this, ResId ( msimTPE_MAX_TEMP_STEP ) ),
aFixedText7 ( this, ResId ( 7 ) ),
aFixedText6 ( this, ResId ( 6 ) ),
MaxTempSLE ( this, ResId ( msimTPE_MAX_TEMP ) ),
aFixedText8 ( this, ResId ( 8 ) ),
ClosePB ( this, ResId ( msimTPE_CLOSE ) ),
UndoPB ( this, ResId ( msimTPE_UNDO ) ),
DefaultsPB ( this, ResId ( msimTPE_DEFAULTS ) ),
HelpPB ( this, ResId ( msimTPE_HELP ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aGroupBox2 ( this, ResId ( 2 ) )

{
     FreeResource( );

     pSimInstance = PSimInstance;
     owner = pParent;

     // Set the title of the dialog //
     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize entries in the dialog box //
     InitializeEntries( pSimInstance );

     InitTempSLE.ChangeModifyHdl(
          LINK( this, LinearTempProgDlg, SetModifiedFlagHandler ) );
     CoeffASLE.ChangeModifyHdl(
          LINK( this, LinearTempProgDlg, SetModifiedFlagHandler ) );
     MaxTempStepSLE.ChangeModifyHdl(
          LINK( this, LinearTempProgDlg, SetModifiedFlagHandler ) );
     MaxTempSLE.ChangeModifyHdl(
          LINK( this, LinearTempProgDlg, SetModifiedFlagHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, LinearTempProgDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, LinearTempProgDlg, UndoHandler ) );
     DefaultsPB.ChangeClickHdl(
          LINK( this, LinearTempProgDlg, DefaultsHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


void LinearTempProgDlg::InitializeEntries( msimPINSTANCE PSimInstance )
{
     // initialize the unit labels

     EditLabel2.SetText( String( ResId( msimLINEAR_TP_DLG_STR1 ) ) +
          String( msimTimeUnits( PSimInstance->time_units ) )
          + String( ResId( msimLINEAR_TP_DLG_STR2 ) ) );

     CoeffAUnitsLabel.SetText( String( ResId( msimLINEAR_TP_DLG_STR3 ) ) +
          String( msimTimeUnits( PSimInstance->time_units ) ) );

     InitTempSLE.SetText( PSimInstance->temp_data.prog_initial_temp );
     CoeffASLE.SetText( PSimInstance->temp_data.progr_coeffA );
     MaxTempStepSLE.SetText( PSimInstance->temp_data.max_step_size );
     MaxTempSLE.SetText( PSimInstance->temp_data.max_temp );

     data_altered = FALSE;

     return;
}


void LinearTempProgDlg::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}


void LinearTempProgDlg::CloseHandler( PushButton PTR )
{
     msimREAL_STRING proginit_temp, slope, maxstepsize, finaltemp;
     String message;

     msimFLOAT num_init_temp;
     msimFLOAT num_final_temp;
     msimFLOAT num_slope;

     msimStringCopy( proginit_temp, InitTempSLE.GetText( ), sizeof proginit_temp );
     msimStringCopy( slope, CoeffASLE.GetText( ), sizeof slope );
     msimStringCopy( maxstepsize, MaxTempStepSLE.GetText( ), sizeof maxstepsize );
     msimStringCopy( finaltemp, MaxTempSLE.GetText( ), sizeof finaltemp );

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          // check the validity of the entry fields

          if ( ! msimValidPositiveFloat( proginit_temp, &num_init_temp ) )
          {
               SendErrorMesg( String( ResId( msimLINEAR_TP_DLG_STR4 ) ) , proginit_temp );
               return;
          }

          if ( ! msimValidNonZeroFloat( slope, &num_slope ) )
          {
               message = String( ResId( msimLINEAR_TP_DLG_STR5 ) ) + String( slope ) +
               String( ResId( msimLINEAR_TP_DLG_STR6 ) );

               WarningBox box( this, WB_OK | WB_DEF_OK, message );
               box.Execute( );
               return;
          }

          if ( ! msimValidPositiveFloat( maxstepsize ) )
          {
               SendErrorMesg( String( ResId( msimLINEAR_TP_DLG_STR7 ) ), maxstepsize );
               return;
          }

          if ( ! msimValidPositiveFloat( finaltemp, &num_final_temp ) )
          {
               SendErrorMesg( String( ResId( msimLINEAR_TP_DLG_STR8 ) ), finaltemp );
               return;
          }

          // now test whether initial and final temperatures are consistent
          // if the slope is greater than zero then final temp must be greater
          // than initial. If slope is less than zero than the final temp must
          // be less than initial. Tell user is this is not true;

          if (num_slope > 0.0)
          {
               // the slope is positive; test for consistency 

               if (  (num_init_temp >= num_final_temp) )
               {
                    WarningBox( this, ResId( msimLINEAR_TP_DLG_ERR1) ).Execute( );
                    return;
               }
          }
          else
          {
               // num_slope is negative since we have checked for zero value
               // earlier. 

               if (  (num_final_temp >= num_init_temp) )
               {
                    WarningBox( this, ResId( msimLINEAR_TP_DLG_ERR2) ).Execute( );
                    return;
               }
          }


          // update the data_altered_since-lastsave flag

          pSimInstance->data_altered_since_lastsave = TRUE;

          // now store all the data into the appropriate data structre 

          msimStringCopy( pSimInstance->temp_data.prog_initial_temp, proginit_temp,
               sizeof pSimInstance->temp_data.prog_initial_temp );
          msimStringCopy( pSimInstance->temp_data.progr_coeffA, slope,
               sizeof pSimInstance->temp_data.progr_coeffA );
          msimStringCopy( pSimInstance->temp_data.max_step_size, maxstepsize,
               sizeof pSimInstance->temp_data.max_step_size );
          msimStringCopy( pSimInstance->temp_data.max_temp, finaltemp,
               sizeof pSimInstance->temp_data.max_temp );
     }

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


void LinearTempProgDlg::UndoHandler( PushButton PTR )
{
     InitializeEntries( pSimInstance );
     return;
}


void LinearTempProgDlg::DefaultsHandler( PushButton PTR )
{
     InitializeEntries( &msimC_INSTANCE );

     data_altered = TRUE;

     return;
}


void LinearTempProgDlg::SendErrorMesg( const String& rField, PCHAR Value )
{
     String message;

     message = String( ResId( msimLINEAR_TP_DLG_STR9 ) ) +
               String( Value ) +
               String( ResId( msimLINEAR_TP_DLG_STR10 ) ) +
               rField +
               String( ResId( msimLINEAR_TP_DLG_STR11 ) ) ;
     
     WarningBox box( this, WB_OK | WB_DEF_OK, message );
     box.Execute( );
     return;
}

void msimLinearTempProgDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     LinearTempProgDlg PTR pdlg =
                           new LinearTempProgDlg( Owner, pInstance );

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