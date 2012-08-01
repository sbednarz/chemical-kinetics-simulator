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
/*  FILE NAME : msimrxop.cxx                                                */
/*                                                                          */
/*  This module implements a dialog for viewing and editing the reaction    */
/*  options such as temperature, pressure, concentrations etc.              */
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

enum TEMP_OPT
{
     CONST_T, VAR_T, PROG_T_FCN, PROG_T_FILE
};

class RxnOptionsDlg : public ModalDialog
{
protected :
     FixedText aFixedText1;
     FixedText aFixedText2;
     FixedText ConcUnitsLabel;
     FixedText SpeciesNameText;
     FixedText SpeciesLabel;
     ListBox ConcLB;
     Edit ConcSLE;
     GroupBox RxnOptConcGB;

     RadioButton ConstTempRB;
     RadioButton VarTempRB;
     RadioButton LinearTProgRB;
     RadioButton ExtTProgRB;
     PushButton TempSettingsPB;
     GroupBox aGroupBox1;

     RadioButton ConstVolRB;
     RadioButton VarVolRB;
     RadioButton DontCareVolRB;
     GroupBox aGroupBox2;

     RadioButton ConstPressRB;
     RadioButton VarPressRB;
     GroupBox aGroupBox3;

     OKButton ClosePB; PushButton DefaultsPB;
     PushButton UndoPB;
     HelpButton HelpPB;

     msimPINSTANCE pSimInstance;
     msimREAL_STRING PTR saved_conc_data;
     msimBOOL data_altered;
     msimBOOL species_data_altered;
     msimPSPECIES speciesptr;
     USHORT curposition;
     msimWID owner;
     msimBOOL initialized;

     void InitializeEntries( );
     void UpdatePanel( );
     void UpdateTVPOptions( msimPINSTANCE PSimInstance );
     void SetVolPanel( USHORT VolOption );
     msimBOOL ReadCurrentInitConc( );
     void SetLinProgTempBtnText( msimPINSTANCE pInstance );
     void SetFileProgTempBtnText( msimPINSTANCE pInstance );
     void SetVarTempBtnText( msimPINSTANCE pInstance );
     void SetConstTempBtnText( msimPINSTANCE pInstance );

     enum TEMP_OPT GetSelectedTempOption( );

public :

     RxnOptionsDlg( Window PTR pParent, msimPINSTANCE PSimInstance );
     ~RxnOptionsDlg( );

     void ListboxSelectHandler( ListBox PTR pListBox );
     void SetModifiedFlagHandler( Edit PTR pEdit );
     void TempSettingsHandler( PushButton PTR pButton );
     void VolumeHandler( RadioButton PTR pRButton );
     void TempRBHandler( RadioButton PTR pRButton );
     void DataAlteredHandler( Button PTR );
     void CloseHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );

     msimBOOL Initialized( )
     {
          return initialized;
     };

};



static PCHAR ConcStr = "Initial concentration";
static PCHAR AmountStr = "Initial amount";

RxnOptionsDlg::~RxnOptionsDlg( )
{
     if ( saved_conc_data )
          delete[]saved_conc_data;
}


RxnOptionsDlg::
RxnOptionsDlg( Window PTR pParent, msimPINSTANCE PSimInstance )
: ModalDialog ( pParent, ResId ( msimRXN_OPTIONS_PANEL ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
ConcUnitsLabel ( this, ResId ( msimRXNOPT_CONC_UNITS ) ),
SpeciesNameText ( this, ResId ( msimRXNOPT_SPECIES_NAME ) ),
SpeciesLabel ( this, ResId ( msimRXNOPT_SPECIES_LABEL ) ),
ConcLB ( this, ResId ( msimRXNOPT_LISTBOX ) ),
ConcSLE ( this, ResId ( msimRXNOPT_CONC ) ),
RxnOptConcGB ( this, ResId ( msimRXNOPT_CONC_LABEL ) ),

ConstTempRB ( this, ResId ( msimRXNOPT_CONSTTEMP ) ),
VarTempRB ( this, ResId ( msimRXNOPT_VAR_TEMP ) ),
LinearTProgRB ( this, ResId ( msimRXNOPT_PROGTEMP ) ),
ExtTProgRB ( this, ResId ( msimRXNOPT_EXT_T_PROFILE ) ),
TempSettingsPB ( this, ResId ( msimRXNOPT_LOAD_PROFILE ) ),
aGroupBox1 ( this, ResId ( 1 ) ),

ConstVolRB ( this, ResId ( msimRXNOPT_CONSTVOLUME ) ),
VarVolRB ( this, ResId ( msimRXNOPT_VARVOLUME ) ),
DontCareVolRB ( this, ResId ( msimRXNOPT_DONTCARE_VOL ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
ConstPressRB ( this, ResId ( msimRXNOPT_CONSTPRESS ) ),
VarPressRB ( this, ResId ( msimRXNOPT_VARPRESS ) ),
aGroupBox3 ( this, ResId ( 3 ) ),

ClosePB ( this, ResId ( msimRXNOPT_CLOSE ) ),
DefaultsPB ( this, ResId ( msimRXNOPT_DEFAULTS ) ),
UndoPB ( this, ResId ( msimRXNOPT_UNDO ) ),
HelpPB ( this, ResId ( msimRXNOPT_HELP ) )
{
     FreeResource( );

     pSimInstance = PSimInstance;

     saved_conc_data = new msimREAL_STRING[pSimInstance->speciescount];

     if ( saved_conc_data == NULL )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, this );

          initialized = FALSE;
          return;
     }
     else
          initialized = TRUE;

     owner = pParent;

     // Set the title of the dialog

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize entries in the dialog box

     InitializeEntries( );

     ConcLB.ChangeSelectHdl(
          LINK( this, RxnOptionsDlg, ListboxSelectHandler ) );
     ConcSLE.ChangeModifyHdl(
          LINK( this, RxnOptionsDlg, SetModifiedFlagHandler ) );

     ConstTempRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, TempRBHandler ) );
     VarTempRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, TempRBHandler ) );
     LinearTProgRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, TempRBHandler ) );
     ExtTProgRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, TempRBHandler ) );

     TempSettingsPB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, TempSettingsHandler ) );

     ConstVolRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, VolumeHandler ) );
     VarVolRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, VolumeHandler ) );
     DontCareVolRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, VolumeHandler ) );

     ConstPressRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, DataAlteredHandler ) );
     VarPressRB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, DataAlteredHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, CloseHandler ) );
     DefaultsPB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, DefaultsHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, RxnOptionsDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


void RxnOptionsDlg::InitializeEntries( )
{

     // save initial conc data in previously allocated saved_conc_data[]

     msimPSPECIES tempptr = pSimInstance->ptr_to_species_list;
     USHORT i = 0;

     // fill in the concentration information for each species into
     // temporary array

     while ( tempptr )
     {
          msimStringCopy( saved_conc_data[i++], tempptr->initialconc,
               sizeof saved_conc_data[0] );
          tempptr = tempptr->next;
     }

     // now initialize dialog
     // turn off update to prevent flicker

     ConcLB.ChangeUpdateMode( FALSE );

     // fill the listbox

     for ( speciesptr = pSimInstance->ptr_to_species_list;
               speciesptr != NULL; speciesptr = speciesptr->next )
          ConcLB.InsertEntry( String( speciesptr->name ) );

     ConcLB.SelectEntryPos( 0 );
     ConcLB.ChangeUpdateMode( TRUE );
     ConcLB.Update( );

     // reset the species pointer to the top of the list and highlight
     // first entry.  also initialize the current and old listbox position

     speciesptr = pSimInstance->ptr_to_species_list;
     curposition = 0;

     // now fill in the rest of the option panel (temperature, volume and pressure)

     UpdatePanel( );

     data_altered = FALSE;
     species_data_altered = FALSE;

     return;
}


void RxnOptionsDlg::UpdatePanel( )
{
     // display the initial concentration and name of the selected species

     ConcSLE.SetText( speciesptr->initialconc );
     SpeciesNameText.SetText( String( speciesptr->name ) );

     // now fill in the temperature, volume and pressure option panel

     UpdateTVPOptions( pSimInstance );

     return;
}

void RxnOptionsDlg::SetLinProgTempBtnText( msimPINSTANCE PSimInstance )
{
     // construct a string to show the existing linear
     // temperature program settings. If the slope == progr_coeffA has
     // a leading positive or negative sign character then we do not
     // add a sing character in constructing the string to be displayed

     String sign;

     CHAR first_char =  *( msimSubWord( PSimInstance->temp_data.progr_coeffA, 1) );

     if ( ( first_char == '-' ) || ( first_char == '+' ) )
          sign = String(" ");
     else
          sign = String( " + " );

     LinearTProgRB.SetText( String( "Follow ~linear program T(t) = " ) +
          String( PSimInstance->temp_data.prog_initial_temp ) +
          sign +
          String( PSimInstance->temp_data.progr_coeffA ) +
          String( " t (" ) +
          String( msimTimeUnits( PSimInstance->time_units ) )
          + String( ")" ) );
}

void RxnOptionsDlg::SetFileProgTempBtnText( msimPINSTANCE pInstance )
{
     String text( "Follow ~external profile in file: " );

#if defined(__MAC__)
     if ( msimWords( pInstance->temp_profile_data ) > 0 )
#else
     if ( msimWords( pInstance->temp_profile_data ) == 1 )
#endif
     {
          text += String( msimBaseFilename( pInstance->temp_profile_data ) );
     }
     else
     {
          text += String( NO_FILENAME );
          * ( pInstance->temp_profile_data ) = '\0';// invalid data, set to null string
     }

     ExtTProgRB.SetText( text );
}

void RxnOptionsDlg::SetVarTempBtnText( msimPINSTANCE pInstance )
{
     VarTempRB.SetText( String( "V~ariable starting from init. T = " )
          + String( pInstance->temp_data.initial_temp ) + String( " deg K" ) );
}


void RxnOptionsDlg::SetConstTempBtnText( msimPINSTANCE pInstance )
{
     ConstTempRB.SetText( String( "~Constant at T = " )
          + String( pInstance->temp_data.const_temp ) + String( " deg K" ) );
}


void RxnOptionsDlg::UpdateTVPOptions( msimPINSTANCE pInstance )
{
     // fill in the temperature option panel

     SetLinProgTempBtnText( pInstance );
     SetFileProgTempBtnText( pInstance );
     SetVarTempBtnText( pInstance );
     SetConstTempBtnText( pInstance );

     switch ( pInstance->temp_option )
     {
     case msimCONST_TEMP :

          ConstTempRB.Check( );
          break;

     case msimVAR_TEMP :

          VarTempRB.Check( );
          break;

     case msimPROGR_TEMP :

          if ( pInstance->temp_prog_data_format )
               ExtTProgRB.Check( );
          else
               LinearTProgRB.Check( );
          break;

     default :
          break;
     }

     // fill in the volume option panel

     switch ( pInstance->volume_option )
     {
     case msimCONST_VOL :
          ConstVolRB.Check( );
          break;

     case msimVAR_VOL :
          VarVolRB.Check( );
          break;

     case msimDONT_CARE_VOL :
          DontCareVolRB.Check( );
          break;

     default :
          break;
     }

     SetVolPanel( pInstance->volume_option );


     // fill in the presure option panel
     if ( pInstance->variablepress )
          VarPressRB.Check( );
     else
          ConstPressRB.Check( );

     return;

}


void RxnOptionsDlg::ListboxSelectHandler( ListBox PTR )
{
     // check the validity of the initial concentration of previously selected
     // species and obtain the current listbox position
     // if the current entry is invalid return

     if ( ! ReadCurrentInitConc( ) )
     {
          ConcLB.SelectEntryPos( curposition );

          return;
     }

     // current entry is valid - go to the new listbox position and
     // adjust speciesptr accordingly

     curposition = ConcLB.GetSelectEntryPos( );

     speciesptr = pSimInstance->ptr_to_species_list;

     for ( USHORT i = 0; i < curposition; i++ )
          speciesptr = speciesptr->next;

     // now update the appropriate information for the new selected species

     ConcSLE.SetText( String( speciesptr->initialconc ) );

     SpeciesNameText.SetText( String( speciesptr->name ) );

     return;
}


enum TEMP_OPT RxnOptionsDlg::GetSelectedTempOption( )
{
     if ( ConstTempRB.IsChecked( ) )
          return CONST_T;
     else
          if ( VarTempRB.IsChecked( ) )
               return VAR_T;
          else
               if ( LinearTProgRB.IsChecked( ) )
                    return PROG_T_FCN;
               else
                    return PROG_T_FILE;
}


void RxnOptionsDlg::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}


void RxnOptionsDlg::TempSettingsHandler( PushButton PTR )
{

     switch ( GetSelectedTempOption( ) )
     {
     case CONST_T :

          msimConstTempDlg( this, pSimInstance );
          SetConstTempBtnText( pSimInstance );

          break;

     case VAR_T :

          msimVarTempDlg( this, pSimInstance );
          SetVarTempBtnText( pSimInstance );

          break;

     case PROG_T_FCN :

          // analytical temp prog here ;

          msimLinearTempProgDlg( this, pSimInstance );
          SetLinProgTempBtnText( pSimInstance );

          break;

     case PROG_T_FILE :

          msimExternalTempProgDlg( this, pSimInstance );
          SetFileProgTempBtnText( pSimInstance );

          break;
     }
}


void RxnOptionsDlg::VolumeHandler( RadioButton PTR )
{
     // update the appropriate information
     if ( VarVolRB.IsChecked( ) )
     {
          SetVolPanel( msimVAR_VOL );

          species_data_altered = TRUE;/* wdh added 11.22.93 - we'll need to check species data*/
     }
     else
          SetVolPanel( msimDONT_CARE_VOL );

     data_altered = TRUE;

     return;
}

void RxnOptionsDlg::TempRBHandler( RadioButton PTR pRButton )
{

#if defined(__MAC__)

     // to deal with SV bug in implemenmting auto rb's

     ConstTempRB.Check( pRButton == &ConstTempRB );
     VarTempRB.Check( pRButton == &VarTempRB );
     LinearTProgRB.Check( pRButton == &LinearTProgRB );
     ExtTProgRB.Check( pRButton == &ExtTProgRB );

#endif

     data_altered = TRUE;
}


void RxnOptionsDlg::DataAlteredHandler( Button PTR )
{
     data_altered = TRUE;
}


void RxnOptionsDlg::CloseHandler( PushButton PTR )
{
     USHORT t_opt, v_opt;
     enum TEMP_OPT temp_option;
     msimBOOL p_opt;

     // first check current conc entry for validity

     if ( ! ReadCurrentInitConc( ) )
          return;

     // get the temperature, volume and pressure options

     temp_option = GetSelectedTempOption( );

     switch ( temp_option )
     {
     case CONST_T :

          t_opt = msimCONST_TEMP;

          break;

     case VAR_T :

          t_opt = msimVAR_TEMP;

          break;

     case PROG_T_FCN :
     case PROG_T_FILE :
          t_opt = msimPROGR_TEMP;

          break;
     }

     if ( ConstVolRB.IsChecked( ) )
          v_opt = msimCONST_VOL;
     else
     {
          if ( VarVolRB.IsChecked( ) )
               v_opt = msimVAR_VOL;
          else
               v_opt = msimDONT_CARE_VOL;
     }

     p_opt = VarPressRB.IsChecked( );

     // before storing the initial concentration of currently selected
     // species, check the validity of the value.  also obtain the the
     // current listbox position and species pointer value


     if ( ! msimValidTPVCombo( p_opt, t_opt, v_opt ) )
     {
          WarningBox( this, ResId( msimRXOP ) ) .Execute( );
          return;
     }

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          pSimInstance->data_altered_since_lastsave = TRUE;

          // store the information regaring temperature option

          switch ( temp_option )
          {
          case CONST_T :

               pSimInstance->temp_option = msimCONST_TEMP;
               break;

          case VAR_T :

               pSimInstance->temp_option = msimVAR_TEMP;
               break;

          case PROG_T_FCN :

               pSimInstance->temp_option = msimPROGR_TEMP;
               pSimInstance->temp_prog_data_format = msimANALYTIC_TPROG;
               break;

          case PROG_T_FILE :

               pSimInstance->temp_option = msimPROGR_TEMP;
               pSimInstance->temp_prog_data_format = msimFILE_TPROG;
               break;

          }

          // store the information regarding volume option

          pSimInstance->volume_option = v_opt;

          pSimInstance->variablepress = p_opt;
     }

     // if species_data_altered set flag to true otherwise clear it

     pSimInstance->specieslist_altered = species_data_altered;

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     pSimInstance -> nonzeroconcs = msimNumberNonZero( pSimInstance -> ptr_to_species_list );

     return;
}


void RxnOptionsDlg::DefaultsHandler( PushButton PTR )
{
     // obtain the default settings //
     UpdateTVPOptions( &msimC_INSTANCE );

     data_altered = TRUE;

     return;
}


void RxnOptionsDlg::UndoHandler( PushButton PTR )
{
     // restore initial conc data from saved_conc_data[]

     USHORT i = 0;
     msimPSPECIES tempptr = pSimInstance->ptr_to_species_list;

     // fill in the concentration information for each species into
     // temporary array

     while ( tempptr )
     {
          msimStringCopy( tempptr->initialconc, saved_conc_data[i++],
               sizeof tempptr->initialconc );
          tempptr = tempptr->next;
     }

     UpdatePanel( );

     data_altered = FALSE;

     return;
}



void RxnOptionsDlg::SetVolPanel( USHORT VolOption )
{
     SpeciesLabel.SetText(
          String( VolOption == msimVAR_VOL ? AmountStr : ConcStr )
          + String( " of species" ) );


     RxnOptConcGB.SetText( String(
               VolOption == msimVAR_VOL ? AmountStr : ConcStr ) + String( "s" ) );


     ConcUnitsLabel.SetText(
          String( msimConcUnits( pSimInstance->conc_units, VolOption ) ) );


     return;
}


msimBOOL RxnOptionsDlg::ReadCurrentInitConc( )
{
     msimREAL_STRING tempstr;

     msimStringCopy( tempstr, ConcSLE.GetText( ), sizeof tempstr );

     if ( ! msimValidNonNegativeFloat( tempstr ) )
     {
          String message;

          message = "The value [ " + String( tempstr ) +
          "] entered for the " +
          SpeciesLabel.GetText( ) +
          " " + String( speciesptr->name ) +
          " is out-of-range or does not represent a numeric value.";
          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }
     else
     {
          msimStringCopy( speciesptr->initialconc, tempstr,
               sizeof speciesptr->initialconc );
          return TRUE;
     }
}


VOID msimSetRxnConditions( msimWID Owner, msimPINSTANCE pInstance )
{
     RxnOptionsDlg PTR pdlg =
                       new RxnOptionsDlg( Owner, pInstance );

     if ( pdlg )
     {
          if ( pdlg->Initialized( ) )
               pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}
