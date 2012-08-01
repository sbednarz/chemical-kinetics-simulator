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

// ----------------------------------------------------------------------------
//  RXNCONDN.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module declares the ReactionConditionDialog invokation function
//  and the subordinate dialogs used by ReactionConditionDialog
//
//  CREATED : 5.11.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "rxncondn.hxx"
#include "comptobj.hxx"
#include "comptdlg.hxx"
#include "mainapp.hxx"

#endif


#include "rxncondn.hrc"
#include "strings.hrc"
#include "popup.hrc"




// ----------------------------------------------------------------------------
//                       class ReactionOptionsDialog 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ReactionOptionsDialog : public ModalDialog
{
protected:
     GroupBox    group_box_1;
     RadioButton const_temp_rb;
     RadioButton var_temp_rb;
     RadioButton prog_temp_rb;
     RadioButton ext_profile_rb;
     PushButton  temp_settings_btn;
     GroupBox    group_box_2;
     RadioButton const_volume_rb;
     RadioButton var_volume_rb;
     RadioButton dont_care_volume_rb;
     PushButton  volume_settings_btn;
     GroupBox    group_box_3;
     RadioButton const_press_rb;
     RadioButton var_press_rb;
	GroupBox    group_box_4;
	RadioButton solid_phase_rb; 
	RadioButton liquid_phase_rb; 
	RadioButton gas_phase_rb; 
     OKButton    close_btn;
     PushButton  defaults_btn;
     PushButton  undo_btn;
     HelpButton  help_btn;

     CompartmentObject* pcompartment;
     CompartmentObject  current_compt;

     BOOL8              data_altered;

     String             temperature_units;
     String             time_units;
     String             volume_units;

     BOOL8 IsDataAltered() const { return data_altered; }
     void  SetDataAltered( );

     void AssignValues( CompartmentObject& rTarget, const CompartmentObject& rSource );

     void          InitializeDialog( );
     void          InitializeData( );

     enum TEMPERATURE_OPTION GetTempOption() const;
     enum VOLUME_OPTION     GetVolumeOption() const;
     enum PRESSURE_OPTION   GetPressureOption() const;
     enum COMPARTMENT_PHASE_TYPE GetPhaseOption() const;

     BOOL8      InvokeConstantTempDlg( );
     BOOL8      InvokeVariableTempDlg( );
     BOOL8      InvokeProgrTempDlg( );
     BOOL8      InvokeExternalTProfileDlg( );

     BOOL8      InvokeConstantVolumeDlg( );
     BOOL8      InvokeVariableVolumeDlg( );

     void       SetConstTempRBLabel();
     void       SetVariableTempRBLabel();
     void       SetProgrTempRBLabel();
     void       SetExtTempProfileRBLabel();
     void       SetConstantVolumeRBLabel();
     void       SetVariableVolumeRBLabel();

     CompartmentObject* GetpCompartment()  { return &current_compt; }

public:

     ReactionOptionsDialog( CompartmentDialog* pParent );
  
     void          ChangeTempSettingsHandler( PushButton * );
     void          ChangeVolumeSettingsHandler( PushButton * );
     void          CloseHandler( OKButton * );
     void          RadioBtnHandler( RadioButton* );
     void          VolumeRadioBtnHandler( RadioButton* pRB );
     void          DefaultsHandler( PushButton * );
     void          UndoHandler( PushButton * );
};






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::ReactionOptionsDialog()
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

ReactionOptionsDialog::ReactionOptionsDialog( CompartmentDialog* pParent ):
ModalDialog( pParent, ResId( vID_DLG_RXN_OPTIONS ) ),
group_box_1( this, ResId( 1 ) ), 
const_temp_rb( this, ResId( vID_RB_CONST_TEMP ) ), 
var_temp_rb( this, ResId( vID_RB_VAR_TEMP ) ), 
prog_temp_rb( this, ResId( vID_RB_PROG_TEMP ) ), 
ext_profile_rb( this, ResId( vID_RB_EXT_T_PROFILE ) ), 
temp_settings_btn( this, ResId( vID_BTN_CHANGE_TEMP_SETTINGS ) ), 
group_box_2( this, ResId( 2 ) ), 
const_volume_rb( this, ResId( vID_RB_CONST_VOLUME ) ), 
var_volume_rb( this, ResId( vID_RB_VAR_VOLUME ) ), 
dont_care_volume_rb( this, ResId( vID_RB_DONT_CARE_VOL ) ), 
volume_settings_btn( this, ResId( vID_BTN_CHANGE_VOLUME_SETTINGS ) ),
group_box_3( this, ResId( 3 ) ), 
const_press_rb( this, ResId( vID_RB_CONST_PRESS ) ), 
var_press_rb( this, ResId( vID_RB_VAR_PRESS ) ), 
group_box_4( this, ResId( 4 ) ),  
solid_phase_rb( this, ResId( vID_RB_SOLID_PHASE ) ),   
liquid_phase_rb( this, ResId( vID_RB_LIQUID_PHASE ) ),  
gas_phase_rb( this, ResId( vID_RB_GAS_PHASE ) ),  
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ),
pcompartment( pParent->GetpCompartment() ),
current_compt( pcompartment->GetName(), pcompartment->GetpParent() ),   
data_altered( FALSE ),
temperature_units( pcompartment->GetpParent()->GetScheme().GetTemperatureUnits() ),
time_units( pcompartment->GetpParent()->GetScheme().GetTimeUnits() ),
volume_units( pcompartment->GetpParent()->GetScheme().GetVolumeUnits() )
{
     FreeResource( );
     AssignValues ( current_compt, *pcompartment );

     InitializeDialog( );

     // create links to radio buttons 

     const_temp_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) ); 
     var_temp_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );  
     prog_temp_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );  
     ext_profile_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );  

     temp_settings_btn.ChangeClickHdl( LINK( this, ReactionOptionsDialog, ChangeTempSettingsHandler ) );  
     volume_settings_btn.ChangeClickHdl( LINK( this, ReactionOptionsDialog, ChangeVolumeSettingsHandler ) );  

     const_volume_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, VolumeRadioBtnHandler ) );
     var_volume_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, VolumeRadioBtnHandler ) ); 
     dont_care_volume_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, VolumeRadioBtnHandler ) ); 

     const_press_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) ); 
     var_press_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );  

     solid_phase_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );   
     liquid_phase_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );   
     gas_phase_rb.ChangeClickHdl( LINK( this, ReactionOptionsDialog, RadioBtnHandler ) );   

     // create links to pushbuttons 

     close_btn.ChangeClickHdl( LINK( this, ReactionOptionsDialog, CloseHandler ) );  
     defaults_btn.ChangeClickHdl( LINK( this, ReactionOptionsDialog, DefaultsHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, ReactionOptionsDialog, UndoHandler ) );  

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::AssignValues( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::AssignValues( CompartmentObject& rTarget, 
const CompartmentObject& rSource )
{
     rTarget.SetTemperatureOption(     rSource.GetTemperatureOption()     );
     rTarget.SetVolumeOption(          rSource.GetVolumeOption()          );
     rTarget.SetPressureOption(        rSource.GetPressureOption()        );
     rTarget.SetVarVolumeInitialValue( rSource.GetVarVolumeInitialValue() );
     rTarget.SetConstVolumeValue(      rSource.GetConstVolumeValue()      );
     rTarget.SetConstTempValue(        rSource.GetConstTempValue()        );
     rTarget.SetVarTempInitialValue(   rSource.GetVarTempInitialValue()   );
     rTarget.SetVarTempConvergenceStd( rSource.GetVarTempConvergenceStd() );
     rTarget.SetProgTempInitialValue(  rSource.GetProgTempInitialValue()  );
     rTarget.SetProgTempRate(          rSource.GetProgTempRate()          );
     rTarget.SetProgTempMaxValue(      rSource.GetProgTempMaxValue()      );
     rTarget.SetProgTempMaxStepSize(   rSource.GetProgTempMaxStepSize()   );
     rTarget.SetProgTempProfileName(   rSource.GetProgTempProfileName( )  );
     rTarget.SetPhase(                 rSource.GetPhase()                 );
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::SetDataAltered( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  ReactionOptionsDialog::SetDataAltered( ) 
{ 
     if ( !data_altered )
     {
          data_altered = TRUE;
          undo_btn.Enable();
     }
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::DefaultsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::DefaultsHandler( PushButton * )
{
     // create a temporary Compartment for setting defaults only 

     CompartmentObject temp_compt( pcompartment->GetName(), pcompartment->GetpParent() );

     AssignValues( current_compt, temp_compt );

     InitializeData( );

     SetDataAltered( );
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::UndoHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::UndoHandler( PushButton * )
{
     AssignValues( current_compt, *pcompartment );

     InitializeData( );

     data_altered = FALSE;
     undo_btn.Disable();
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::GetTempOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum TEMPERATURE_OPTION ReactionOptionsDialog::GetTempOption() const
{
     if ( const_temp_rb.IsChecked() )
          return CONSTANT_TEMP;

     if ( var_temp_rb.IsChecked() )
          return VARIABLE_TEMP;

     if ( prog_temp_rb.IsChecked() )
          return ANALYTIC_PROGR_TEMP;

     if ( ext_profile_rb.IsChecked() )
          return EXT_PROFILE_PROGR_TEMP;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::GetVolumeOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum VOLUME_OPTION ReactionOptionsDialog::GetVolumeOption() const
{
     if ( const_volume_rb.IsChecked() )
          return CONSTANT_VOLUME;

     if ( var_volume_rb.IsChecked() )
          return VARIABLE_VOLUME;

     if ( dont_care_volume_rb.IsChecked() )
          return NOT_TRACKED_VOLUME;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}


// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::GetPhaseOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum COMPARTMENT_PHASE_TYPE ReactionOptionsDialog::GetPhaseOption() const
{
     if ( solid_phase_rb.IsChecked() )
          return SOLID;

     if ( liquid_phase_rb.IsChecked() )
          return LIQUID;

     if ( gas_phase_rb.IsChecked() )
          return GAS;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::GetPressureOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum PRESSURE_OPTION    ReactionOptionsDialog::GetPressureOption() const
{
     if ( const_press_rb.IsChecked() )
          return CONSTANT_PRESSURE;
     
     if ( var_press_rb.IsChecked() )
          return VARIABLE_PRESSURE;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}




// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::SetConstTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetConstTempRBLabel()
{
     const_temp_rb.SetText( 
      String( ResId( vID_STR_CONST_TEMP_RB_STR1 ) ) +
      current_compt.GetConstTempValue() + 
      String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
      temperature_units
     );
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::SetVariableTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetVariableTempRBLabel()
{
     var_temp_rb.SetText( 
      String( ResId( vID_STR_VAR_TEMP_RB_STR1 ) ) +
      current_compt.GetVarTempInitialValue( ) +  
      String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
      temperature_units
     );
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::SetProgrTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetProgrTempRBLabel()
{
     prog_temp_rb.SetText( 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR1 ) ) +
      current_compt.GetProgTempInitialValue( ) + 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR2 ) ) +
      current_compt.GetProgTempRate( ) + 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR3 ) ) +
      time_units +
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR4 ) )
     );
}




// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::SetExtTempProfileRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetExtTempProfileRBLabel()
{
     ext_profile_rb.SetText(
      String( ResId( vID_STR_EXT_PROG_TEMP_RB_STR1 ) ) +
      current_compt.GetProgTempProfileName( )  
     );
}






// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::SetConstantVolumeRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetConstantVolumeRBLabel()
{
     const_volume_rb.SetText(
      String( ResId( vID_STR_CONST_VOL_RB_STR1 ) ) +
      current_compt.GetConstVolumeValue() + 
      String( ResId( vID_STR_CONST_VOL_RB_STR2 ) ) +
      volume_units
     );
}




// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::SetVariableVolumeRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::SetVariableVolumeRBLabel()
{
     var_volume_rb.SetText(
      String( ResId( vID_STR_VAR_VOL_RB_STR1 ) ) +
      current_compt.GetVarVolumeInitialValue( ) + 
      String( ResId( vID_STR_VAR_VOL_RB_STR2) ) + 
      volume_units
     );
}




// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::InitializeDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::InitializeDialog()
{
     // initialize radiobutton text which depends on the current temperature
     // and volume parameter values 

     SetConstTempRBLabel();
     SetVariableTempRBLabel();
     SetProgrTempRBLabel();
     SetExtTempProfileRBLabel();
     SetConstantVolumeRBLabel();
     SetVariableVolumeRBLabel();

     InitializeData( );
}






// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::InitializeData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::InitializeData( )
{
     // initialize temperature option radiobuttons 

     enum TEMPERATURE_OPTION t_opt = current_compt.GetTemperatureOption();

     const_temp_rb.Check( CONSTANT_TEMP == t_opt );
     var_temp_rb.Check( VARIABLE_TEMP == t_opt ); 
     prog_temp_rb.Check( ANALYTIC_PROGR_TEMP == t_opt );  
     ext_profile_rb.Check( EXT_PROFILE_PROGR_TEMP == t_opt );  

     // initialize volume option radiobuttons 

     enum VOLUME_OPTION v_opt = current_compt.GetVolumeOption();

     const_volume_rb.Check( CONSTANT_VOLUME == v_opt );
     var_volume_rb.Check( VARIABLE_VOLUME == v_opt ); 
     dont_care_volume_rb.Check( NOT_TRACKED_VOLUME == v_opt ); 

     if (NOT_TRACKED_VOLUME == v_opt )
          volume_settings_btn.Disable();
     else
          volume_settings_btn.Enable();

     // initialize pressure option radiobuttons 

     enum PRESSURE_OPTION p_opt = current_compt.GetPressureOption();

     const_press_rb.Check( CONSTANT_PRESSURE == p_opt );
     var_press_rb.Check( VARIABLE_PRESSURE == p_opt ); 

     // initialize phase radiobuttons 

     enum COMPARTMENT_PHASE_TYPE phase = current_compt.GetPhase();

     solid_phase_rb.Check( SOLID == phase );
     liquid_phase_rb.Check( LIQUID == phase );
     gas_phase_rb.Check( GAS == phase );
     
}







// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::ChangeTempSettingsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::ChangeTempSettingsHandler( PushButton * )
{
     switch( GetTempOption() )
     {
     case CONSTANT_TEMP :

          if ( InvokeConstantTempDlg( ) )
          {
               SetConstTempRBLabel();
               SetDataAltered();
          }
          break;


     case VARIABLE_TEMP :

          if ( InvokeVariableTempDlg( ) )
          {
               SetVariableTempRBLabel();
               SetDataAltered();
          }
          break;


     case ANALYTIC_PROGR_TEMP :

          if ( InvokeProgrTempDlg( ) )
          {
               SetProgrTempRBLabel();
               SetDataAltered();
          }
          break;

     case EXT_PROFILE_PROGR_TEMP :

          UnderConstruction();
#if 0
          if ( InvokeExternalTProfileDlg( ) )
          {
               SetExtTempProfileRBLabel();
               SetDataAltered();
          }
#endif
          break;
     }
}






// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::ChangeVolumeSettingsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::ChangeVolumeSettingsHandler( PushButton * )
{
     switch( GetVolumeOption() )
     {
     case CONSTANT_VOLUME :

          if ( InvokeConstantVolumeDlg( ) )
          {
               SetConstantVolumeRBLabel();
               SetDataAltered();
          }
          break;


     case VARIABLE_VOLUME :

          if ( InvokeVariableVolumeDlg( ) )
          {
               SetVariableVolumeRBLabel();
               SetDataAltered();
          }
          break;
     }
}






// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::CloseHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::CloseHandler( OKButton * )
{
     if ( IsDataAltered() )
     {
          current_compt.SetTemperatureOption( GetTempOption() );
          current_compt.SetVolumeOption( GetVolumeOption() );
          current_compt.SetPressureOption( GetPressureOption() );
          current_compt.SetPhase( GetPhaseOption() );

          AssignValues(  *pcompartment, current_compt );

          EndDialog( RC_RC_ACCEPT );
     }
     else
          EndDialog( RC_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::RadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::RadioBtnHandler( RadioButton* )
{
     SetDataAltered( );
}


// ----------------------------------------------------------------------------
//               ReactionOptionsDialog::VolumeRadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ReactionOptionsDialog::VolumeRadioBtnHandler( RadioButton* pRB )
{
     SetDataAltered( );

     if (pRB == &dont_care_volume_rb )
          volume_settings_btn.Disable();
     else
          volume_settings_btn.Enable();
}







// ----------------------------------------------------------------------------
//         class SimpleEntryDialog
// ............................................................................
// 
// base class for all the simple dialogs for modifying rxn condns
//
// ----------------------------------------------------------------------------

class SimpleEntryDialog
{
protected:

     CompartmentObject*      pcompartment;
     BOOL8                   data_altered;
     Window*                 p_parent_dlg;
     PushButton*             p_undo_btn;

     CompartmentObject*      GetpCompartment() const { return pcompartment; }


     void SetDataAlteredFlag();
     void ClearDataAlteredFlag();
     virtual void Initialize()=0;
     virtual BOOL8 ReadAndValidateUserData( )=0;
     virtual void  CloseDialog(RxnCondnRC ReturnValue )=0;

     void EntryError( const NumericString& UserEntry, const String& rLabel );

public :

     SimpleEntryDialog( Window* pParent, CompartmentObject* pCompt );  

     void CloseBtnHandler( OKButton* );
     void UndoBtnHandler( PushButton* );
     void TextModifiedHandler( Edit* );
};




// ----------------------------------------------------------------------------
//         SimpleEntryDialog::SimpleEntryDialog
// ............................................................................
// 
// constructor
//
// ----------------------------------------------------------------------------

SimpleEntryDialog::SimpleEntryDialog( Window* pParent, CompartmentObject* pCompt ) :
pcompartment( pCompt ),
data_altered( FALSE ),
p_parent_dlg( pParent ),
p_undo_btn( NULL )
{
}


void SimpleEntryDialog::SetDataAlteredFlag()
{
     if ( !data_altered )
     {
          data_altered = TRUE;
          p_undo_btn->Enable();
     }
}





void SimpleEntryDialog::ClearDataAlteredFlag()
{
     data_altered = FALSE;
     p_undo_btn->Disable();
}


// ----------------------------------------------------------------------------
//         SimpleEntryDialog::EntryError( )
// ............................................................................
// 
// popup dialog to inform user of input data error 
//
// ----------------------------------------------------------------------------

void SimpleEntryDialog::EntryError( const NumericString& UserEntry, 
 const String& rLabel )
{
     String message = String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR1 ) ) + 
      UserEntry + 
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR2 ) ) +
      rLabel +
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR3 ) );

      WarningBox( p_parent_dlg, WB_OK | WB_DEF_OK, message ).Execute( );
}




// ----------------------------------------------------------------------------
//         SimpleEntryDialog::UndoBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SimpleEntryDialog::UndoBtnHandler( PushButton* )
{
     Initialize();
     ClearDataAlteredFlag( );
}




// ----------------------------------------------------------------------------
//         SimpleEntryDialog::TextModifiedHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SimpleEntryDialog::TextModifiedHandler( Edit* )
{
     SetDataAlteredFlag();
}






// ----------------------------------------------------------------------------
//         SimpleEntryDialog::CloseBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SimpleEntryDialog::CloseBtnHandler( OKButton* )
{
     if ( data_altered )
     {
          if ( ReadAndValidateUserData() )
               CloseDialog( RC_RC_ACCEPT );
          else
               return;

     }
     else
          CloseDialog( RC_RC_CANCEL );
}













// ----------------------------------------------------------------------------
//         class ExternalTempProfileDlg 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ExternalTempProfileDlg : public SimpleEntryDialog, public ModalDialog
{

protected:
     FixedText      filename_text;
     PushButton     select_profile_btn;
     PushButton     create_profile_btn;
     FixedText      profile_edit_label_text;
     FixedText      aFixedText3;
     FixedText      aFixedText4;
     MultiLineEdit  profile_multiline_entryfield;
     PushButton     refresh_btn;
     Edit           step_size_entryfield;
     OKButton       close_btn;
     PushButton     undo_btn;
     PushButton     save_as_btn;
     HelpButton     help_btn;
     GroupBox       group_box_2;
     GroupBox       group_box_1;
     Window    plot_window;

     virtual void Initialize();

     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }


public:
     ExternalTempProfileDlg( Window* pParent, CompartmentObject* pCompt );   
};






// ----------------------------------------------------------------------------
//         ExternalTempProfileDlg::ReadAndValidateUserData()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ExternalTempProfileDlg::ReadAndValidateUserData( )
{
     // stub 
     return TRUE;
}





// ----------------------------------------------------------------------------
//         ExternalTempProfileDlg::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ExternalTempProfileDlg::Initialize()
{
    filename_text.SetText( GetpCompartment()->GetProgTempProfileName( ) );
}





// ----------------------------------------------------------------------------
//         ExternalTempProfileDlg::ExternalTempProfileDlg()
// ............................................................................
// 
// constructor 
//
// ----------------------------------------------------------------------------

ExternalTempProfileDlg::ExternalTempProfileDlg( Window* pParent, CompartmentObject* pCompt ) :
SimpleEntryDialog( pParent, pCompt ),
ModalDialog( pParent, ResId( vID_DLG_EXT_T_PROFILE ) ),
filename_text( this, ResId( vID_STEXT_FILENAME ) ), 
select_profile_btn( this, ResId( vID_BTN_SELECT_FILE ) ), 
create_profile_btn( this, ResId( vID_BTN_PROFILE_CREATE ) ), 
profile_edit_label_text( this, ResId( vID_STEXT_EDIT_PROF_LABEL ) ), 
aFixedText3( this, ResId( 3 ) ), 
aFixedText4( this, ResId( 4 ) ), 
profile_multiline_entryfield( this, ResId( vID_MLE_EDIT_PROFILE ) ), 
refresh_btn( this, ResId( vID_BTN_REFRESH ) ), 
step_size_entryfield( this, ResId( vID_EF_STEPSIZE ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
save_as_btn( this, ResId( vID_BTN_SAVEAS ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
group_box_2( this, ResId( 2 ) ), 
group_box_1( this, ResId( 1 ) ),
plot_window( this, ResId( vID_WINDOW_PLOT_WINDOW ) )
{
     FreeResource( );
     Initialize();

     p_undo_btn = &undo_btn;
     
     GetpMainApp()->CenterWindowOnParent( this, pParent );
}












// ----------------------------------------------------------------------------
//                       class ConstantTempDialog 
// ............................................................................
// 
//  class declaration for simple dialog to modify constant temp value 
//
// ----------------------------------------------------------------------------

class ConstantTempDialog : public SimpleEntryDialog, public ModalDialog
{
protected:
     Edit       temp_entryfield;
     OKButton   close_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText  label_text;
     FixedText  units_text;
     GroupBox   group_box_1;

     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }


public:
     ConstantTempDialog( Window* pParent, CompartmentObject* pCompt );
};



// ----------------------------------------------------------------------------
//                ConstantTempDialog::ReadAndValidateUserData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ConstantTempDialog::ReadAndValidateUserData( )
{
     NumericString str = temp_entryfield.GetText( );

     if (str.IsValidPositiveFLOAT64() )
     {
          GetpCompartment()->SetConstTempValue( str );

          return TRUE;
     }
     else
     {
          EntryError( str, label_text.GetText() );
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                ConstantTempDialog::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ConstantTempDialog::Initialize( )
{
     temp_entryfield.SetText( GetpCompartment()->GetConstTempValue() );
}







// ----------------------------------------------------------------------------
//                ConstantTempDialog::ConstantTempDialog()
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

ConstantTempDialog::ConstantTempDialog( Window* pParent, CompartmentObject* pCompt ) :
SimpleEntryDialog( pParent, pCompt ),
ModalDialog( pParent, ResId( vID_DLG_CONST_TEMP ) ),
temp_entryfield( this, ResId( vID_EF_CONST_TEMP_VALUE ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
label_text( this, ResId( vID_STEXT_ENTRYFIELD_LABEL ) ), 
units_text( this, ResId( vID_STEXT_UNITS_LABEL ) ), 
group_box_1( this, ResId( 1 ) )
{
     FreeResource();
     Initialize();
     units_text.SetText( pCompt->GetpParent()->GetScheme().GetTemperatureUnits() );
     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, ConstantTempDialog, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, ConstantTempDialog, UndoBtnHandler ) );         
     temp_entryfield.ChangeModifyHdl( LINK( this, ConstantTempDialog, TextModifiedHandler ) );          

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}










// ----------------------------------------------------------------------------
//                class ConstantVolumeDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ConstantVolumeDialog : public SimpleEntryDialog, public ModalDialog 
{
protected:
     Edit volume_entryfield;
     OKButton close_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText  label_text;
     FixedText volume_units_text;
     GroupBox group_box_1;

     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }


public:
     ConstantVolumeDialog( Window* pParent, CompartmentObject* pCompt );
};






// ----------------------------------------------------------------------------
//                ConstantVolumeDialog::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ConstantVolumeDialog::Initialize( )
{
     volume_entryfield.SetText( GetpCompartment()->GetConstVolumeValue() );
}


// ----------------------------------------------------------------------------
//                ConstantVolumeDialog::ReadAndValidateUserData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ConstantVolumeDialog::ReadAndValidateUserData( )
{
     NumericString str = volume_entryfield.GetText( );

     if (str.IsValidPositiveFLOAT64() )
     {
          GetpCompartment()->SetConstVolumeValue(( str ) );

          return TRUE;
     }
     else
     {
          EntryError( str, label_text.GetText() );
          return FALSE;
     }
}


// ----------------------------------------------------------------------------
//                ConstantVolumeDialog::ConstantVolumeDialog()
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

ConstantVolumeDialog::ConstantVolumeDialog( Window* pParent, CompartmentObject* pCompt ) :
SimpleEntryDialog( pParent, pCompt ),
ModalDialog( pParent, ResId( vID_DLG_CONST_VOLUME ) ),
volume_entryfield( this, ResId( vID_EF_CONST_VOLUME_VALUE ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
label_text( this, ResId( vID_STEXT_VOLUME_LABEL ) ), 
volume_units_text( this, ResId( vID_STEXT_VOLUME_UNITS ) ), 
group_box_1( this, ResId( 1 ) )
{
     FreeResource();
     Initialize();
     volume_units_text.SetText( pCompt->GetpParent()->GetScheme().GetVolumeUnits() );
     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, ConstantVolumeDialog, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, ConstantVolumeDialog, UndoBtnHandler ) );         
     volume_entryfield.ChangeModifyHdl( LINK( this, ConstantVolumeDialog, TextModifiedHandler ) );          

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}








// ----------------------------------------------------------------------------
//                class VariableVolumeDialog 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class VariableVolumeDialog : public SimpleEntryDialog, public ModalDialog
{
protected:
     Edit volume_entryfield;
     OKButton close_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText label_text;
     FixedText volume_units_text;
     GroupBox group_box_1;

     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }

public:
     VariableVolumeDialog( Window* pParent, CompartmentObject* pCompt );
};





// ----------------------------------------------------------------------------
//                VariableVolumeDialog::Initialize( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void VariableVolumeDialog::Initialize( )
{
     volume_entryfield.SetText( GetpCompartment()->GetVarVolumeInitialValue() );
}




// ----------------------------------------------------------------------------
//                VariableVolumeDialog::ReadAndValidateUserData( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 VariableVolumeDialog::ReadAndValidateUserData( )
{
     NumericString str = volume_entryfield.GetText( );

     if (str.IsValidPositiveFLOAT64() )
     {
          GetpCompartment()->SetVarVolumeInitialValue(( str ) );

          return TRUE;
     }
     else
     {
          EntryError( str, label_text.GetText() );
          return FALSE;
     }
}




// ----------------------------------------------------------------------------
//                VariableVolumeDialog::VariableVolumeDialog(  ) 
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

VariableVolumeDialog::VariableVolumeDialog( Window* pParent, CompartmentObject* pCompt ) :
SimpleEntryDialog( pParent, pCompt ),
ModalDialog( pParent, ResId( vID_DLG_VARIABLE_VOLUME ) ),
volume_entryfield( this, ResId( vID_EF_INITIAL_VOLUME_VALUE ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
label_text( this, ResId( vID_STEXT_VOLUME_LABEL ) ), 
volume_units_text( this, ResId( vID_STEXT_VOLUME_UNITS ) ), 
group_box_1( this, ResId( 1 ) )
{
     FreeResource();
     Initialize();
     volume_units_text.SetText( pCompt->GetpParent()->GetScheme().GetVolumeUnits() );
     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, VariableVolumeDialog, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, VariableVolumeDialog, UndoBtnHandler ) );         
     volume_entryfield.ChangeModifyHdl( LINK( this, VariableVolumeDialog, TextModifiedHandler ) );          

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}









// ----------------------------------------------------------------------------
//                class VariableTempDialog 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class VariableTempDialog : public SimpleEntryDialog, public ModalDialog
{
protected:
     Edit         init_temp_entryfield;
     Edit         t_conv_std_entryfield;
     OKButton     close_btn;
     PushButton   undo_btn;
     HelpButton   help_btn;
     FixedText    init_temp_label_text;
     FixedText    convg_std_label_text;
     FixedText    init_temp_units;
     FixedText    convg_std_units;
     GroupBox group_box_1;

     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }

public:
     VariableTempDialog( Window* pParent, CompartmentObject* pCompt );
};




// ----------------------------------------------------------------------------
//                VariableTempDialog::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void VariableTempDialog::Initialize( )
{
     init_temp_entryfield.SetText( GetpCompartment()->GetVarTempInitialValue() );  
     t_conv_std_entryfield.SetText( GetpCompartment()->GetVarTempConvergenceStd() );
}



// ----------------------------------------------------------------------------
//                VariableTempDialog::ReadAndValidateUserData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 VariableTempDialog::ReadAndValidateUserData( )
{
     // get both entries 

     NumericString str1 = init_temp_entryfield.GetText( );
     NumericString str2 = t_conv_std_entryfield.GetText( );

     // test each in turn for erroneous content 

     if ( !str1.IsValidPositiveFLOAT64() )
     {
          EntryError( str1, init_temp_label_text.GetText() );
               return FALSE;
     }

     if ( !str2.IsValidPositiveFLOAT64() )
     {
          EntryError( str2, convg_std_label_text.GetText() );
               return FALSE;
     }

     // if we get to here, then both entries are valid 
     // so we accept them and return 

     GetpCompartment()->SetVarTempInitialValue( ( str1 ) );
     GetpCompartment()->SetVarTempConvergenceStd( ( str2 ) );

     return TRUE;
}




// ----------------------------------------------------------------------------
//                VariableTempDialog::VariableTempDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

VariableTempDialog::VariableTempDialog( Window* pParent, CompartmentObject* pCompt ) :
SimpleEntryDialog( pParent, pCompt ), 
ModalDialog( pParent, ResId( vID_DLG_VAR_TEMP ) ),
init_temp_entryfield( this, ResId( vID_EF_INITTEMP ) ), 
t_conv_std_entryfield( this, ResId( vID_EF_TEMP_CONVERGE_STD ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
init_temp_label_text( this, ResId( vID_STEXT_INIT_TEMP_LABEL ) ), 
convg_std_label_text( this, ResId( vID_STEXT_CONVG_STD_LABEL ) ), 
init_temp_units( this, ResId( vID_STEXT_INIT_TEMP_UNITS ) ),  
convg_std_units( this, ResId( vID_STEXT_CONVG_STD_UNITS ) ),   
group_box_1( this, ResId( 1 ) )
{
     FreeResource();
     Initialize();

     init_temp_units.SetText( pCompt->GetpParent()->GetScheme().GetTemperatureUnits() ); 
     convg_std_units.SetText( pCompt->GetpParent()->GetScheme().GetTemperatureUnits() );  

     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, VariableTempDialog, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, VariableTempDialog, UndoBtnHandler ) );         
     init_temp_entryfield.ChangeModifyHdl( LINK( this, VariableTempDialog, TextModifiedHandler ) );          
     t_conv_std_entryfield.ChangeModifyHdl( LINK( this, VariableTempDialog, TextModifiedHandler ) );           

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}






// ----------------------------------------------------------------------------
//                VariableTempDialog::VariableTempDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ProgrammedTempDlg : public SimpleEntryDialog, public ModalDialog
{
protected:
     FixedText          eqn_label_text;
     FixedText          init_temp_label_text;
     Edit               init_temp_entryfield;
     FixedText          temp_units_text;
     FixedText          slope_label_text;
     Edit               slope_entryfield;
     FixedText          slope_units_text;
     FixedText          step_size_label_text;
     Edit               max_temp_step_entryfield;
     FixedText          max_step_units_text;
     FixedText          max_temp_label_text;
     Edit               max_temp_entryfield;
     FixedText          max_temp_units_text;
     OKButton           close_btn;
     PushButton         undo_btn;
     PushButton         defaults_btn;
     HelpButton         help_btn;
     GroupBox           group_box_1;
     GroupBox           group_box_2;

     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }

public:
     ProgrammedTempDlg( Window* pParent, CompartmentObject* pCompt );

     void DefaultsBtnHandler( PushButton* );
};





// ----------------------------------------------------------------------------
//                ProgrammedTempDlg::ProgrammedTempDlg( )
// ............................................................................
// 
//   constructor
//
// ----------------------------------------------------------------------------

ProgrammedTempDlg::ProgrammedTempDlg( Window* pParent, CompartmentObject* pCompt ):
SimpleEntryDialog( pParent, pCompt ), 
ModalDialog( pParent, ResId( vID_DLG_TEMP_PROGR ) ),
eqn_label_text( this, ResId( vID_STEXT_EQN_LABEL ) ),
init_temp_label_text( this, ResId( vID_STEXT_INIT_TEMP_LABEL ) ),
init_temp_entryfield( this, ResId( vID_EF_INIT_TEMP ) ),
temp_units_text( this, ResId( vID_STEXT_TEMP_UNIT_LABEL ) ),
slope_label_text( this, ResId( vID_STEXT_SLOPE_LABEL ) ),
slope_entryfield( this, ResId( vID_EF_SLOPE ) ),
slope_units_text( this, ResId( vID_STEXT_SLOPE_UNITS_LABEL ) ),
step_size_label_text( this, ResId( vID_STEXT_STEP_SIZE_LABEL ) ),
max_temp_step_entryfield( this, ResId( vID_EF_MAX_TEMP_STEP ) ),
max_step_units_text( this, ResId( vID_STEXT_MAX_STEP_UNITS ) ),
max_temp_label_text( this, ResId( vID_STEXT_MAX_TEMP_LABEL ) ),
max_temp_entryfield( this, ResId( vID_EF_MAX_TEMP ) ),
max_temp_units_text( this, ResId( vID_STEXT_MAX_TEMP_UNITS ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
group_box_1( this, ResId( 1 ) ),
group_box_2( this, ResId( 2 ) )
{
     FreeResource();
     Initialize();

     max_temp_units_text.SetText( pCompt->GetpParent()->GetScheme().GetTemperatureUnits() );
     max_step_units_text.SetText( max_temp_units_text.GetText() );
     temp_units_text.SetText( max_temp_units_text.GetText() ); 
     slope_units_text.SetText( max_temp_units_text.GetText() + 
                               String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                               pCompt->GetpParent()->GetScheme().GetTimeUnits() ); 

     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, ProgrammedTempDlg, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, ProgrammedTempDlg, UndoBtnHandler ) );         
     defaults_btn.ChangeClickHdl( LINK( this, ProgrammedTempDlg, DefaultsBtnHandler ) );         

     init_temp_entryfield.ChangeModifyHdl( LINK( this, ProgrammedTempDlg, TextModifiedHandler ) );          
     slope_entryfield.ChangeModifyHdl( LINK( this, ProgrammedTempDlg, TextModifiedHandler ) );          
     max_temp_entryfield.ChangeModifyHdl( LINK( this, ProgrammedTempDlg, TextModifiedHandler ) );          
     max_temp_step_entryfield.ChangeModifyHdl( LINK( this, ProgrammedTempDlg, TextModifiedHandler ) );          

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}


void ProgrammedTempDlg::DefaultsBtnHandler( PushButton* )
{
     init_temp_entryfield.SetText( ResId( vID_STR_DEFAULT_TEMP) );  
     slope_entryfield.SetText( ResId( vID_STR_DEFAULT_PROG_T_RATE) ); 
     max_temp_step_entryfield.SetText( ResId( vID_STR_DEFAULT_PROG_T_MAX_STEP ) );
     max_temp_entryfield.SetText( ResId( vID_STR_DEFAULT_PROG_T_MAX_VAL ) );

     SetDataAlteredFlag( );
}




// ----------------------------------------------------------------------------
//                ProgrammedTempDlg::Initialize()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgrammedTempDlg::Initialize()
{
     init_temp_entryfield.SetText( GetpCompartment()->GetProgTempInitialValue() );
     slope_entryfield.SetText( GetpCompartment()->GetProgTempRate() ); 
     max_temp_step_entryfield.SetText( GetpCompartment()->GetProgTempMaxStepSize() );                         
     max_temp_entryfield.SetText( GetpCompartment()->GetProgTempMaxValue() );                         
}






// ----------------------------------------------------------------------------
//                     ProgrammedTempDlg::ReadAndValidateUserData()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ProgrammedTempDlg::ReadAndValidateUserData( )
{
     NumericString str1 = init_temp_entryfield.GetText( );
     NumericString str2 = slope_entryfield.GetText( );
     NumericString str3 = max_temp_step_entryfield.GetText( );
     NumericString str4 = max_temp_entryfield.GetText( );

     if ( !str1.IsValidPositiveFLOAT64() )
     {
          EntryError( str1, init_temp_label_text.GetText() );
               return FALSE;
     }

     if ( !str2.IsValidNonZeroFLOAT64() )
     {
          EntryError( str2, slope_label_text.GetText() );
               return FALSE;
     }


     if ( !str3.IsValidNonZeroFLOAT64() )
     {
          EntryError( str3, step_size_label_text.GetText() );
               return FALSE;
     }

     if ( !str4.IsValidNonZeroFLOAT64() )
     {
          EntryError( str4, max_temp_label_text.GetText() );
               return FALSE;
     }

     // if we made it here the entries are all valid - save and exit

     GetpCompartment()->SetProgTempInitialValue( str1 );
     GetpCompartment()->SetProgTempRate( str2 ); 
     GetpCompartment()->SetProgTempMaxStepSize( str3 );                         
     GetpCompartment()->SetProgTempMaxValue( str4 );                         

     return TRUE;
}










// ----------------------------------------------------------------------------
//                     ReactionOptionsDialog::InvokeConstantTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeConstantTempDlg( )
{
     ConstantTempDialog* pdlg = new ConstantTempDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                     ReactionOptionsDialog::InvokeVariableTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeVariableTempDlg( )
{
     VariableTempDialog* pdlg = new VariableTempDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::InvokeProgrTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeProgrTempDlg( )
{
     ProgrammedTempDlg* pdlg = new ProgrammedTempDlg( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT ==  (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::InvokeExternalTProfileDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeExternalTProfileDlg( )
{
     ExternalTempProfileDlg* pdlg = new ExternalTempProfileDlg( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}






// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::InvokeConstantVolumeDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeConstantVolumeDlg( )
{
     ConstantVolumeDialog* pdlg = new ConstantVolumeDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT ==  (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                 ReactionOptionsDialog::InvokeVariableVolumeDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 ReactionOptionsDialog::InvokeVariableVolumeDlg( )
{
     VariableVolumeDialog* pdlg = new VariableVolumeDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );

     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}


// ----------------------------------------------------------------------------
//                       class RxnOptionsDlg3D 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class RxnOptionsDlg3D : public ModalDialog
{
protected:
     GroupBox    group_box_1;
     RadioButton const_temp_rb;
     RadioButton var_temp_rb;
     RadioButton prog_temp_rb;
     RadioButton ext_profile_rb;
     PushButton  temp_settings_btn;
     GroupBox    group_box_2;
     RadioButton const_volume_rb;
     RadioButton var_volume_rb;

     FixedText   select_dim_text;
     CheckBox    x_cb;
     CheckBox    y_cb;
     CheckBox    z_cb;

	GroupBox    group_box_3;
	RadioButton solid_phase_rb; 
	RadioButton liquid_phase_rb; 
	RadioButton gas_phase_rb; 

     OKButton    close_btn;
     PushButton  defaults_btn;
     PushButton  undo_btn;
     HelpButton  help_btn;

     CompartmentObject* pcompartment;
     CompartmentObject  current_compt;

     BOOL8              data_altered;

     String             temperature_units;
     String             time_units;
     String             volume_units;

     BOOL8 IsDataAltered() const { return data_altered; }
     void  SetDataAltered( );

     void AssignValues( CompartmentObject& rTarget, const CompartmentObject& rSource );

     void          InitializeDialog( );
     void          InitializeData( );

     enum TEMPERATURE_OPTION GetTempOption() const;
     enum VOLUME_OPTION      GetVolumeOption() const;
     enum DIRECTION          GetUnconstrainedDirection() const;
     enum COMPARTMENT_PHASE_TYPE GetPhaseOption() const;

     BOOL8      InvokeConstantTempDlg( );
     BOOL8      InvokeVariableTempDlg( );
     BOOL8      InvokeProgrTempDlg( );
     BOOL8      InvokeExternalTProfileDlg( );

     void       SetConstTempRBLabel();
     void       SetVariableTempRBLabel();
     void       SetProgrTempRBLabel();
     void       SetExtTempProfileRBLabel();
     void       EnableDirectionInput( BOOL8 NewState );

     CompartmentObject* GetpCompartment()  { return &current_compt; }

public:

     RxnOptionsDlg3D( CompartmentDialog* pParent );
  
     void          ChangeTempSettingsHandler( PushButton * );
     void          CloseHandler( OKButton * );
     void          RadioBtnHandler( RadioButton* );
     void          VolumeRadioBtnHandler( RadioButton* pRB );
     void          DefaultsHandler( PushButton * );
     void          UndoHandler( PushButton * );
     void          CheckBoxHandler( CheckBox* );
};






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::RxnOptionsDlg3D()
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

RxnOptionsDlg3D::RxnOptionsDlg3D( CompartmentDialog* pParent ):
ModalDialog( pParent, ResId( vID_DLG_RXN_OPTIONS_3D ) ),
group_box_1( this, ResId( 1 ) ), 
const_temp_rb( this, ResId( vID_RB_CONST_TEMP ) ), 
var_temp_rb( this, ResId( vID_RB_VAR_TEMP ) ), 
prog_temp_rb( this, ResId( vID_RB_PROG_TEMP ) ), 
ext_profile_rb( this, ResId( vID_RB_EXT_T_PROFILE ) ), 
temp_settings_btn( this, ResId( vID_BTN_CHANGE_TEMP_SETTINGS ) ), 
group_box_2( this, ResId( 2 ) ), 
const_volume_rb( this, ResId( vID_RB_CONST_VOLUME ) ), 
var_volume_rb( this, ResId( vID_RB_VAR_VOLUME ) ), 
select_dim_text( this, ResId( vID_STEXT_VARIABLE_DIMENSIONS ) ),  
x_cb( this, ResId( vID_CB_X ) ),  
y_cb( this, ResId( vID_CB_Y ) ),  
z_cb( this, ResId( vID_CB_Z ) ),  
group_box_3( this, ResId( 3 ) ),  
solid_phase_rb( this, ResId( vID_RB_SOLID_PHASE ) ),   
liquid_phase_rb( this, ResId( vID_RB_LIQUID_PHASE ) ),  
gas_phase_rb( this, ResId( vID_RB_GAS_PHASE ) ),  
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ),
pcompartment( pParent->GetpCompartment() ),
current_compt( pcompartment->GetName(), pcompartment->GetpParent() ),   
data_altered( FALSE ),
temperature_units( pcompartment->GetpParent()->GetScheme().GetTemperatureUnits() ),
time_units( pcompartment->GetpParent()->GetScheme().GetTimeUnits() ),
volume_units( pcompartment->GetpParent()->GetScheme().GetVolumeUnits() )
{
     FreeResource( );
     AssignValues ( current_compt, *pcompartment );

     InitializeDialog( );

     // create links to radio buttons 

     const_temp_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) ); 
     var_temp_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  
     prog_temp_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  
     ext_profile_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  

     solid_phase_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  
     liquid_phase_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  
     gas_phase_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, RadioBtnHandler ) );  

     x_cb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, CheckBoxHandler ) );  
     y_cb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, CheckBoxHandler ) );  
     z_cb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, CheckBoxHandler ) );  

     temp_settings_btn.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, ChangeTempSettingsHandler ) );  

     const_volume_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, VolumeRadioBtnHandler ) );
     var_volume_rb.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, VolumeRadioBtnHandler ) ); 

     // create links to pushbuttons 

     close_btn.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, CloseHandler ) );  
     defaults_btn.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, DefaultsHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, RxnOptionsDlg3D, UndoHandler ) );  

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::AssignValues( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::AssignValues( CompartmentObject& rTarget, 
const CompartmentObject& rSource )
{
     rTarget.SetTemperatureOption(     rSource.GetTemperatureOption()     );
     rTarget.SetVolumeOption(          rSource.GetVolumeOption()          );
//     rTarget.SetPressureOption(        rSource.GetPressureOption()        );
     rTarget.SetVarVolumeInitialValue( rSource.GetVarVolumeInitialValue() );
     rTarget.SetConstVolumeValue(      rSource.GetConstVolumeValue()      );
     rTarget.SetConstTempValue(        rSource.GetConstTempValue()        );
     rTarget.SetVarTempInitialValue(   rSource.GetVarTempInitialValue()   );
     rTarget.SetVarTempConvergenceStd( rSource.GetVarTempConvergenceStd() );
     rTarget.SetProgTempInitialValue(  rSource.GetProgTempInitialValue()  );
     rTarget.SetProgTempRate(          rSource.GetProgTempRate()          );
     rTarget.SetProgTempMaxValue(      rSource.GetProgTempMaxValue()      );
     rTarget.SetProgTempMaxStepSize(   rSource.GetProgTempMaxStepSize()   );
     rTarget.SetProgTempProfileName(   rSource.GetProgTempProfileName( )  );

     rTarget.SetUnconstrainedDirection( rSource.GetUnconstrainedDirection() );
     rTarget.SetPhase(                  rSource.GetPhase() );
}









// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::SetDataAltered( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  RxnOptionsDlg3D::SetDataAltered( ) 
{ 
     if ( !data_altered )
     {
          data_altered = TRUE;
          undo_btn.Enable();
     }
}





// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::DefaultsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::DefaultsHandler( PushButton * )
{
     // create a temporary Compartment for setting defaults only 

     CompartmentObject temp_compt( pcompartment->GetName(), pcompartment->GetpParent() );

     temp_compt.SetVolumeOption( CONSTANT_VOLUME ); 

     AssignValues( current_compt, temp_compt );

     InitializeData( );

     SetDataAltered( );
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::UndoHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::UndoHandler( PushButton * )
{
     AssignValues( current_compt, *pcompartment );

     InitializeData( );

     data_altered = FALSE;
     undo_btn.Disable();
}





// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::GetTempOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum TEMPERATURE_OPTION RxnOptionsDlg3D::GetTempOption() const
{
     if ( const_temp_rb.IsChecked() )
          return CONSTANT_TEMP;

     if ( var_temp_rb.IsChecked() )
          return VARIABLE_TEMP;

     if ( prog_temp_rb.IsChecked() )
          return ANALYTIC_PROGR_TEMP;

     if ( ext_profile_rb.IsChecked() )
          return EXT_PROFILE_PROGR_TEMP;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::GetVolumeOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum VOLUME_OPTION RxnOptionsDlg3D::GetVolumeOption() const
{
     if ( const_volume_rb.IsChecked() )
          return CONSTANT_VOLUME;

     if ( var_volume_rb.IsChecked() )
          return VARIABLE_VOLUME;

     // should not make it to here - abort if we do

     ASSERT(0);
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::GetPhaseOption( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum COMPARTMENT_PHASE_TYPE RxnOptionsDlg3D::GetPhaseOption() const
{
     if ( solid_phase_rb.IsChecked() )
          return SOLID;

     if ( liquid_phase_rb.IsChecked() )
          return LIQUID;

     if ( gas_phase_rb.IsChecked() )
          return GAS;
     
     // should not make it to here - abort if we do

     ASSERT(0);
}







// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::SetConstTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::SetConstTempRBLabel()
{
     const_temp_rb.SetText( 
      String( ResId( vID_STR_CONST_TEMP_RB_STR1 ) ) +
      current_compt.GetConstTempValue() + 
      String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
      temperature_units
     );
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::SetVariableTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::SetVariableTempRBLabel()
{
     var_temp_rb.SetText( 
      String( ResId( vID_STR_VAR_TEMP_RB_STR1 ) ) +
      current_compt.GetVarTempInitialValue( ) + 
      String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
      temperature_units
     );
}





// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::SetProgrTempRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::SetProgrTempRBLabel()
{
     prog_temp_rb.SetText( 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR1 ) ) +
      current_compt.GetProgTempInitialValue( ) + 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR2 ) ) +
      current_compt.GetProgTempRate( ) + 
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR3 ) ) +
      time_units +
      String( ResId( vID_STR_ANAL_PROG_TEMP_RB_STR4 ) )
     );
}




// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::SetExtTempProfileRBLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::SetExtTempProfileRBLabel()
{
     ext_profile_rb.SetText(
      String( ResId( vID_STR_EXT_PROG_TEMP_RB_STR1 ) ) +
      current_compt.GetProgTempProfileName( )  
     );
}








// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::InitializeDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::InitializeDialog()
{
     // initialize radiobutton text which depends on the current temperature
     // and volume parameter values 

     SetConstTempRBLabel();
     SetVariableTempRBLabel();
     SetProgrTempRBLabel();
     SetExtTempProfileRBLabel();

     InitializeData( );
}






// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::InitializeData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::InitializeData( )
{
     // initialize temperature option radiobuttons 

     enum TEMPERATURE_OPTION t_opt = current_compt.GetTemperatureOption();

     const_temp_rb.Check( CONSTANT_TEMP == t_opt );
     var_temp_rb.Check( VARIABLE_TEMP == t_opt ); 
     prog_temp_rb.Check( ANALYTIC_PROGR_TEMP == t_opt );  
     ext_profile_rb.Check( EXT_PROFILE_PROGR_TEMP == t_opt );  

     // initialize volume option radiobuttons 

     enum VOLUME_OPTION v_opt = current_compt.GetVolumeOption();

     const_volume_rb.Check( CONSTANT_VOLUME == v_opt );
     var_volume_rb.Check( VARIABLE_VOLUME == v_opt ); 

     // initialize phase radiobuttons 

     enum COMPARTMENT_PHASE_TYPE phase = current_compt.GetPhase();

     solid_phase_rb.Check( SOLID == phase );
     liquid_phase_rb.Check( LIQUID == phase );
     gas_phase_rb.Check( GAS == phase );
     

     // set checkboxes 

     BOOL8 x_is_variable;
     BOOL8 y_is_variable;
     BOOL8 z_is_variable;

     switch (current_compt.GetUnconstrainedDirection() )
     {

     case NONE :
           x_is_variable = FALSE;
           y_is_variable = FALSE;
           z_is_variable = FALSE;

          break;

     case X_AXIS :
           x_is_variable = TRUE;
           y_is_variable = FALSE;
           z_is_variable = FALSE;

          break;

     case Y_AXIS :
           x_is_variable = FALSE;
           y_is_variable = TRUE;
           z_is_variable = FALSE;

          break;

     case Z_AXIS :
           x_is_variable = FALSE;
           y_is_variable = FALSE;
           z_is_variable = TRUE;

          break;

     case XY_PLANE :
           x_is_variable = TRUE;
           y_is_variable = TRUE;
           z_is_variable = FALSE;

          break;

     case XZ_PLANE :
           x_is_variable = TRUE;
           y_is_variable = FALSE;
           z_is_variable = TRUE;

          break;

     case YZ_PLANE :
           x_is_variable = FALSE;
           y_is_variable = TRUE;
           z_is_variable = TRUE;

          break;

     case ALL_DIRECTIONS :
           x_is_variable = TRUE;
           y_is_variable = TRUE; 
           z_is_variable = TRUE; 

          break;


     default:
          ASSERT( 0 );
          break;

     }

     x_cb.Check( x_is_variable );
     y_cb.Check( y_is_variable );
     z_cb.Check( z_is_variable );

     EnableDirectionInput( VARIABLE_VOLUME == v_opt );
}







// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::ChangeTempSettingsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::ChangeTempSettingsHandler( PushButton * )
{
     switch( GetTempOption() )
     {
     case CONSTANT_TEMP :

          if ( InvokeConstantTempDlg( ) )
          {
               SetConstTempRBLabel();
               SetDataAltered();
          }
          break;


     case VARIABLE_TEMP :

          if ( InvokeVariableTempDlg( ) )
          {
               SetVariableTempRBLabel();
               SetDataAltered();
          }
          break;


     case ANALYTIC_PROGR_TEMP :

          if ( InvokeProgrTempDlg( ) )
          {
               SetProgrTempRBLabel();
               SetDataAltered();
          }
          break;

     case EXT_PROFILE_PROGR_TEMP :

          UnderConstruction();
#if 0 
          if ( InvokeExternalTProfileDlg( ) )
          {
               SetExtTempProfileRBLabel();
               SetDataAltered();
          }
#endif
          break;
     }
}





// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::GetUnconstrainedDirection()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum DIRECTION RxnOptionsDlg3D::GetUnconstrainedDirection() const
{
     const UINT32 X_UNCONSTRAINED = 0x0001;
     const UINT32 Y_UNCONSTRAINED = 0x0002;
     const UINT32 Z_UNCONSTRAINED = 0x0004;

     enum DIRECTION retval;
     UINT32 settings = 0;

     if (x_cb.IsChecked( ) )
          settings |= X_UNCONSTRAINED;

     if (y_cb.IsChecked( ) )
          settings |= Y_UNCONSTRAINED;

     if (z_cb.IsChecked( ) )
          settings |= Z_UNCONSTRAINED;

     switch (settings)
     {
     
     case 0 :
          retval = NONE;
          break;


     case X_UNCONSTRAINED : 

          retval = X_AXIS;
          break;


     case Y_UNCONSTRAINED : 

          retval = Y_AXIS;
          break;


     case Z_UNCONSTRAINED : 

          retval = Z_AXIS;
          break;

     case (X_UNCONSTRAINED | Y_UNCONSTRAINED) :

          retval = XY_PLANE;
          break;

     case (X_UNCONSTRAINED | Z_UNCONSTRAINED) :

          retval = XZ_PLANE;
          break;

     case (Y_UNCONSTRAINED | Z_UNCONSTRAINED) :

          retval = YZ_PLANE;
          break;

     case (X_UNCONSTRAINED | Y_UNCONSTRAINED | Z_UNCONSTRAINED ) :

          retval = ALL_DIRECTIONS;
          break;


     }

     return retval;
}




// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::CloseHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::CloseHandler( OKButton * )
{
     if ( IsDataAltered() )
     {
          // test whether variable volume is checked but no unconstrained 
          // directions have been selected 

          enum VOLUME_OPTION v_opt = GetVolumeOption();

          if ( (VARIABLE_VOLUME == v_opt) &&
               ( !x_cb.IsChecked() && !y_cb.IsChecked() && !z_cb.IsChecked() )
          )
          {
               InfoBox( this , ResId( vID_POPUP_NO_UNCONSTRAINED_DIR ) ).Execute();
               return;

          }

          current_compt.SetTemperatureOption( GetTempOption() );
          current_compt.SetVolumeOption( v_opt );
          current_compt.SetUnconstrainedDirection( GetUnconstrainedDirection() );
          current_compt.SetPhase( GetPhaseOption() );

          AssignValues(  *pcompartment, current_compt );

          EndDialog( RC_RC_ACCEPT );
     }
     else
          EndDialog( RC_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::RadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::RadioBtnHandler( RadioButton* )
{
     SetDataAltered( );
}




// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::CheckBoxHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::CheckBoxHandler( CheckBox* )
{
     SetDataAltered( );
}





// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::EnableDirectionInput()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::EnableDirectionInput( BOOL8 NewState )
{
     if ( NewState )
     {
          select_dim_text.Enable();
          x_cb.Enable(); 
          y_cb.Enable(); 
          z_cb.Enable(); 
     }
     else
     {
          select_dim_text.Disable();
          x_cb.Disable(); 
          y_cb.Disable(); 
          z_cb.Disable(); 
     }
}





// ----------------------------------------------------------------------------
//               RxnOptionsDlg3D::VolumeRadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnOptionsDlg3D::VolumeRadioBtnHandler( RadioButton* pRB )
{
     SetDataAltered( );

     EnableDirectionInput( pRB == &var_volume_rb );
}













// ----------------------------------------------------------------------------
//                     RxnOptionsDlg3D::InvokeConstantTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 RxnOptionsDlg3D::InvokeConstantTempDlg( )
{
     ConstantTempDialog* pdlg = new ConstantTempDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                     RxnOptionsDlg3D::InvokeVariableTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 RxnOptionsDlg3D::InvokeVariableTempDlg( )
{
     VariableTempDialog* pdlg = new VariableTempDialog( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}






// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::InvokeProgrTempDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 RxnOptionsDlg3D::InvokeProgrTempDlg( )
{
     ProgrammedTempDlg* pdlg = new ProgrammedTempDlg( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT ==  (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                 RxnOptionsDlg3D::InvokeExternalTProfileDlg()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 RxnOptionsDlg3D::InvokeExternalTProfileDlg( )
{
     ExternalTempProfileDlg* pdlg = new ExternalTempProfileDlg( this, GetpCompartment() );

     if ( pdlg )
     {
          return ( RC_RC_ACCEPT == (RxnCondnRC ) pdlg->Execute() );
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}






// ----------------------------------------------------------------------------
//                       class GlobalTempDialog 
// ............................................................................
// 
//  class declaration for simple dialog to modify constant temp value across 
// the whole reaction scheme 
//
// ----------------------------------------------------------------------------

class GlobalTempDialog : public SimpleEntryDialog, public ModalDialog
{
protected:
     Edit       temp_entryfield;
     OKButton   close_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText  label_text;
     FixedText  units_text;
     GroupBox   group_box_1;

     ReactionScheme* p_scheme;

     ReactionScheme*      GetpScheme() const { return p_scheme; }
     virtual void Initialize();
     virtual BOOL8 ReadAndValidateUserData( );

     virtual void  CloseDialog(RxnCondnRC ReturnValue ) 
     { EndDialog( (short) ReturnValue ); }


public:
     GlobalTempDialog( Window* pParent, ReactionScheme* pScheme );
};



// ----------------------------------------------------------------------------
//                GlobalTempDialog::ReadAndValidateUserData( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 GlobalTempDialog::ReadAndValidateUserData( )
{
     NumericString str = temp_entryfield.GetText( );

     if (str.IsValidPositiveFLOAT64() )
     {
          GetpScheme()->SetSchemeToConstantTemperature( str );

          return TRUE;
     }
     else
     {
          EntryError( str, label_text.GetText() );
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                GlobalTempDialog::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void GlobalTempDialog::Initialize( )
{
     // get temperature from first compartment as an initial value

     temp_entryfield.SetText( GetpCompartment()->GetConstTempValue() );
}







// ----------------------------------------------------------------------------
//                GlobalTempDialog::GlobalTempDialog()
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

GlobalTempDialog::GlobalTempDialog( Window* pParent, ReactionScheme* pScheme ) :
SimpleEntryDialog( pParent, pScheme->GetCompartmentObjectPtrFromIndex(0) ),
ModalDialog( pParent, ResId( vID_DLG_SET_COMPT_TEMP ) ),
temp_entryfield( this, ResId( vID_EF_CONST_TEMP_VALUE ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
label_text( this, ResId( vID_STEXT_ENTRYFIELD_LABEL ) ), 
units_text( this, ResId( vID_STEXT_UNITS_LABEL ) ), 
group_box_1( this, ResId( 1 ) ),
p_scheme( pScheme )
{
     FreeResource();
     Initialize();
     units_text.SetText( GetpScheme()->GetTemperatureUnits() );
     p_undo_btn = &undo_btn;

     close_btn.ChangeClickHdl( LINK( this, GlobalTempDialog, CloseBtnHandler ) );  
     undo_btn.ChangeClickHdl( LINK( this, GlobalTempDialog, UndoBtnHandler ) );         
     temp_entryfield.ChangeModifyHdl( LINK( this, GlobalTempDialog, TextModifiedHandler ) );          

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}












// ----------------------------------------------------------------------------
//                 EditReactionConditions()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

RxnCondnRC EditReactionConditions3D( CompartmentDialog* pParent )
{
     RxnOptionsDlg3D* pdlg = new RxnOptionsDlg3D( pParent );

     if ( pdlg )
     {
           return (RxnCondnRC ) pdlg->Execute();
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return RC_RC_FAIL;
     }
}



// ----------------------------------------------------------------------------
//                 EditGlobalSchemeTemperature()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

RxnCondnRC EditGlobalSchemeTemperature( ReactionScheme* pScheme, Window* pParent )
{
     GlobalTempDialog* pdlg = new GlobalTempDialog( pParent, pScheme );

     if ( pdlg )
     {
           return (RxnCondnRC ) pdlg->Execute();
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return RC_RC_FAIL;
     }
}



// ----------------------------------------------------------------------------
//                 EditReactionConditions()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

RxnCondnRC EditReactionConditions( CompartmentDialog* pParent )
{
     ReactionOptionsDialog* pdlg = new ReactionOptionsDialog( pParent);

     if ( pdlg )
     {
           return (RxnCondnRC ) pdlg->Execute();
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return RC_RC_FAIL;
     }
}









