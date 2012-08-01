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
//  EDITXFER.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the EditTransferProcessDialog class
//  the VSIM user interface. 
//
//  CREATED : 4.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "editxfer.hxx"
#include "schemwin.hxx"
#include "mainapp.hxx"

#endif


#include "editxfer.hrc"
#include "strings.hrc"









// ----------------------------------------------------------------------------
//                class EditTransferProcessDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class EditTransferProcessDialog : public ModalDialog
{
protected:

     // stuff up at the top of the dialog ( no group box )

     FixedText           status_text;
     SymbolButton        prev_btn;
     SymbolButton        next_btn;

     RadioButton         src_to_trgt_rb;
     RadioButton         reversible_rb;
     RadioButton         trgt_to_src_rb;

     SchematicWindow     scheme_window;
  
     // first group box 
  
     RadioButton         arrh_form_rb;
     RadioButton         single_rate_rb;
     GroupBox            group_box1;
  
     // second group box 
  
     FixedText           fwd_text;
     FixedText           rev_text;
     FixedText           a_factor_text;
     FixedText           temp_exp_text;
     FixedText           act_energy_text;
     FixedText           a_factor_units_text;
     FixedText           act_energy_units_text;
     Edit                fwd_a_factor_entry_field;
     Edit                fwd_temp_exp_entry_field;
     Edit                fwd_ea_entry_field;
     Edit                rev_a_factor_entry_field; 
     Edit                rev_temp_exp_entry_field; 
     Edit                rev_ea_entry_field;       
     GroupBox            group_box2;
  
     // third group box 

     FixedText           fwd_rate_law_text;
     FixedText           rev_rate_law_text;
     Edit                fwd_conc_exp_entry_field;
     Edit                rev_conc_exp_entry_field;
     GroupBox            group_box3;
  
     // pushbuttons at bottom of dialog 

     OKButton            ok_btn;
//     PushButton          add_step_btn;
//     PushButton          delete_step_btn;
     PushButton          undo_btn;
     HelpButton          help_btn;

     TransferStep        current_step;
     TransferObject*     p_object;
    
     BOOL8               data_altered;
     String              status_text_prefix;
     enum SCHEMWIN_TYPE  scheme_type;
     enum XFER_DIRECTION xfer_direction;

     TransferObject*     GetpObject() const { return p_object; }

     void                UpdateEntryFieldStatus( );
     void                UpdateEntryFieldText( );
     void                DrawSchematic( );
     void                SetRadioButtons( );
     void                InitializeDialog( );
     void                UpdatePanel( );
     void                SetUnitsText();

     BOOL8               ValidateAndStoreData( );
     void                SendErrorMessage( const String& FieldName, const String& UserEntry ) const;

     enum XFER_DIRECTION GetSelectedDirection();

public:

     EditTransferProcessDialog( TransferObject* pTransferObj, Window* pParent );

     void PrevHandler( SymbolButton* aSymbolButton );
     void NextHandler( SymbolButton* aSymbolButton );
     void SetModifiedFlagHandler( Edit* );
     void RadioButtonHandler( RadioButton* pRButton );
     void DirectionRadioButtonHandler( RadioButton* pRButton );
     void CheckBoxHandler( CheckBox* );

     void CloseHandler( PushButton* );
     void UndoHandler( PushButton* );
};






// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::EditTransferProcessDialog()
// ............................................................................
// 
//   constructor
//
// ----------------------------------------------------------------------------

EditTransferProcessDialog::EditTransferProcessDialog( TransferObject* pTransferObj, 
Window* pParent ) :
ModalDialog( pParent, ResId( vID_DLG_EDIT_XFER ) ),
status_text( this, ResId( vID_STEXT_TRANSFER_STEP ) ),
prev_btn( this, ResId( vID_SBTN_PREV_STEP_BTN ) ),
next_btn( this, ResId( vID_SBTN_NEXT_STEP_BTN ) ),
src_to_trgt_rb( this, ResId( vID_RB_SRC_TO_TARGET ) ), 
reversible_rb( this, ResId( vID_RB_REVERSIBLE ) ),                        
trgt_to_src_rb( this, ResId( vID_RB_TARGET_TO_SRC ) ),                        
scheme_window( this, ResId( vID_WIN_XFER_SCHEMATIC ) ),

arrh_form_rb( this, ResId( vID_RB_ARRH_FORM ) ), 
single_rate_rb( this, ResId( vID_RB_SINGLE_RATE ) ), 
group_box1( this, ResId( 1 ) ),

fwd_text( this, ResId( vID_STEXT_FORWARD ) ),
rev_text( this, ResId( vID_STEXT_REVERSE ) ),
a_factor_text( this, ResId( vID_STEXT_AFACTOR ) ),
temp_exp_text( this, ResId( vID_STEXT_TEMP_EXPONENT ) ),
act_energy_text( this, ResId( vID_STEXT_EA ) ),
a_factor_units_text( this, ResId( vID_STEXT_AFACTOR_UNITS ) ),
act_energy_units_text( this, ResId( vID_STEXT_EA_UNITS ) ),
fwd_a_factor_entry_field( this, ResId( vID_EF_FWD_AFACTOR ) ),
fwd_temp_exp_entry_field( this, ResId( vID_EF_FWD_TEXP ) ),
fwd_ea_entry_field( this, ResId( vID_EF_FWD_EA ) ),
rev_a_factor_entry_field( this, ResId( vID_EF_REV_A_FACTOR ) ),
rev_temp_exp_entry_field( this, ResId( vID_EF_REV_TEXP ) ),
rev_ea_entry_field( this, ResId( vID_EF_REV_EA ) ),
group_box2( this, ResId( 2 ) ),

fwd_rate_law_text( this, ResId( vID_STEXT_FWD_RATE_LAW ) ),
rev_rate_law_text( this, ResId( vID_STEXT_REV_RATE_LAW ) ),
fwd_conc_exp_entry_field( this, ResId( vID_EF_FWD_CONC_EXPONENT ) ),
rev_conc_exp_entry_field( this, ResId( vID_EF_REV_CONC_EXPONENT ) ),
group_box3( this, ResId( 3 ) ),

ok_btn( this, ResId( vID_PB_CLOSE ) ),
//add_step_btn( this, ResId( vID_PB_ADD_STEP ) ),
//delete_step_btn( this, ResId( vID_PB_DELETE_STEP ) ),
undo_btn( this, ResId( vID_PB_UNDO ) ),
help_btn( this, ResId( vID_HBTN_HELP ) ),
current_step( ),
p_object( pTransferObj ),
data_altered( FALSE ),
status_text_prefix(),
scheme_type( GetpObject()->GetpParent()->GetSchemeType() ),
xfer_direction( SOURCE_TO_TARGET )
{
     FreeResource( );

     current_step = GetpObject()->GetTransferStep( 
      GetpObject()->GetTransferListboxSelection() );


     UpdatePanel( );

     prev_btn.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, PrevHandler ) );
     next_btn.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, NextHandler ) );

     arrh_form_rb.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, RadioButtonHandler ) );
     single_rate_rb.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, RadioButtonHandler ) );

     reversible_rb.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, DirectionRadioButtonHandler ) );
     src_to_trgt_rb.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, DirectionRadioButtonHandler ) );
     trgt_to_src_rb.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, DirectionRadioButtonHandler ) );

     fwd_a_factor_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     fwd_temp_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     fwd_ea_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     rev_a_factor_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     rev_temp_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     rev_ea_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     fwd_conc_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 
     rev_conc_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditTransferProcessDialog, SetModifiedFlagHandler ) ); 

     ok_btn.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl(
          LINK( this, EditTransferProcessDialog, UndoHandler ) );

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );

     // some conditional stuff 

     switch ( scheme_type )
     {
     case SW_TYPE_3D :

          // set text strings as required 

          SetText( String( ResId( vID_STR_DIFFN_STEP_DLG_TITLE ) ) );

          status_text_prefix = String( ResId( vID_STR_DIFFN_STEP_STATUS_1 ) );
          group_box1.SetText( String( ResId( vID_STR_DIFFN_COEFF_FORM ) ) );
          group_box3.SetText( String( ResId( vID_STR_DIFFN_COEFF_VALUE ) ) );
          fwd_text.SetText( String( ResId( vID_STR_DIFFN_COEFF_FWD ) ) ); 
          rev_text.SetText( String( ResId( vID_STR_DIFFN_COEFF_REV ) ) ); 

          fwd_rate_law_text.Disable();
          rev_rate_law_text.Disable(); 
          fwd_conc_exp_entry_field.Disable(); 
          rev_conc_exp_entry_field.Disable(); 
          group_box2.Disable(); 

          break;


     default : 
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;


     case SW_TYPE_COMPARTMENT :

          // we do little here as the default dialog strings are for this case 

          status_text_prefix = String( ResId( vID_STR_XFER_STEP_STATUS_1 ) );

          break;

     }


     SetUnitsText();

}










// ----------------------------------------------------------------------------
//           EditTransferProcessDialog::SetUnitsText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::SetUnitsText()
{
     ReactionScheme& rscheme = p_object->GetpParent()->GetScheme();

     switch (scheme_type)
     {

     case SW_TYPE_3D :

          a_factor_units_text.SetText( rscheme.GetDistanceUnits() +
                                       String( ResId( vID_STR_SYMBOL_EXPONENT_2 ) ) + 
                                  String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                        rscheme.GetTimeUnits() );

          act_energy_units_text.SetText( rscheme.GetLargeEnergyUnits() + 
                                  String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                  rscheme.GetAmountUnits() );

          break;


     case SW_TYPE_COMPARTMENT :

          a_factor_units_text.SetText( String( ResId( vID_STR_LEFT_BRACKET ) ) +
                                        rscheme.GetVolumeUnits() +
                                        String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                        rscheme.GetAmountUnits() +
                                        String( ResId( vID_STR_SYMBOL_DASH ) ) +
                                        rscheme.GetTimeUnits() +
                                        String( ResId( vID_STR_RIGHT_BRACKET ) ) +
                                        String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
                                        String( ResId( vID_STR_UNITS ) )  );
     
          act_energy_units_text.SetText( rscheme.GetLargeEnergyUnits() + 
                                  String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                  rscheme.GetAmountUnits() );
     
          break;


     default : 
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;

     }
}





// ----------------------------------------------------------------------------
//           EditTransferProcessDialog::GetSelectedDirection()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum XFER_DIRECTION EditTransferProcessDialog::GetSelectedDirection()
{
     if ( src_to_trgt_rb.IsChecked() )
     {
          return SOURCE_TO_TARGET;
     }
     else
     {
          if ( trgt_to_src_rb.IsChecked() )
          {
               return TARGET_TO_SOURCE;
          }
          else
          {
               if ( reversible_rb.IsChecked() )
               {
                    return REVERSIBLE;
               }
               else
                    ASSERT( 0 );
           }
     }
}





// ----------------------------------------------------------------------------
//           EditTransferProcessDialog::DirectionRadioButtonHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::DirectionRadioButtonHandler( RadioButton* )
{
     data_altered = TRUE;

     scheme_window.SetTransferDirection( GetSelectedDirection() );
     DrawSchematic( );
     UpdateEntryFieldStatus();
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::UpdatePanel()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::UpdatePanel( )
{
     xfer_direction = current_step.GetTransferDirection();

     InitializeDialog( );
     DrawSchematic( );
     SetRadioButtons( );
     UpdateEntryFieldStatus( );
     UpdateEntryFieldText( );
}








// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::RadioButtonHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

#pragma argsused

void EditTransferProcessDialog::RadioButtonHandler( RadioButton* pRButton  )
{
     // Update the appropriate information

#if defined(_MAC__)

    // under MAC these RB's are not automatic it seems - probably a SV bug

    arrh_form_rb.Check( pRButton == &arrh_form_rb );
    single_rate_rb.Check( pRButton == single_rate_rb );

#endif

     UpdateEntryFieldStatus( );
     data_altered = TRUE;
     return;
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::CloseHandler( PushButton* )
{
     if ( data_altered )
     {
          if ( ! ValidateAndStoreData( ) )
               return;

          EndDialog( EX_RC_CLOSE );
     }
     else
          EndDialog( EX_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::UndoHandler( PushButton* )
{
     current_step = GetpObject()->GetTransferStep( 
      GetpObject()->GetTransferListboxSelection() );
     data_altered = FALSE;

     UpdatePanel( );
}






// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::DrawSchematic()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::DrawSchematic(  )
{
     scheme_window.Invalidate();
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::SetModifiedFlagHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::SetModifiedFlagHandler( Edit* )
{
     data_altered = TRUE;
     return;
}






// ----------------------------------------------------------------------------
//               EditTransferProcessDialog::ValidateAndStoreData( )
// ............................................................................
// 
//   this fcn retrieves the data from the entry fields, checks for its validity,
//   and stores it back in the parent Object's transfer list if valid. If
//   the data has a problem then a diagnostic message is displayed. The function 
//   returns TRUE is data is valid, FALSE if not 
//
// ----------------------------------------------------------------------------

BOOL8 EditTransferProcessDialog::ValidateAndStoreData( )
{
     const String SPACE_STR( " ");

     // retrieve and test the data in all the numeric entry fields 
     
     current_step.SetFwdA( fwd_a_factor_entry_field.GetText( ) );
     current_step.SetFwdM( fwd_temp_exp_entry_field.GetText( ) );
     current_step.SetFwdEa( fwd_ea_entry_field.GetText( ) );

     current_step.SetRevA( rev_a_factor_entry_field.GetText( ) );
     current_step.SetRevM( rev_temp_exp_entry_field.GetText( ) );
     current_step.SetRevEa( rev_ea_entry_field.GetText( ) );

     current_step.SetFwdExponent( fwd_conc_exp_entry_field.GetText( ) );
     current_step.SetRevExponent( rev_conc_exp_entry_field.GetText( ) );

     current_step.SetRateConstantFormat( 
      single_rate_rb.IsChecked( ) ? TEMP_INDEPENDENT : TEMP_DEPENDENT );

     current_step.SetTransferDirection( GetSelectedDirection() );

     // validate entries 

     NumericString test_string = current_step.GetFwdA();

     if ( !test_string.IsValidPositiveFLOAT64() )
     {
          SendErrorMessage( fwd_text.GetText() + SPACE_STR  +
           a_factor_text.GetText(), current_step.GetFwdA( ) );

          return FALSE;
     }

     if ( TEMP_DEPENDENT == current_step.GetRateConstantFormat( ) )
     {
          test_string = current_step.GetFwdM();

          if ( !test_string.IsValidFLOAT64( ) )
          {
               SendErrorMessage( fwd_text.GetText() + SPACE_STR  +
                temp_exp_text.GetText(), current_step.GetFwdM() );

               return FALSE;
          }


          test_string = current_step.GetFwdEa();

          if ( !test_string.IsValidFLOAT64( ) )
          {
               SendErrorMessage( fwd_text.GetText() + SPACE_STR  +
                act_energy_text.GetText(), current_step.GetFwdEa( ) );

               return FALSE;
          }

          // if the equation is reversible repeat the above process for the 
          // numerical values for the reversible information 

          if ( SOURCE_TO_TARGET != GetSelectedDirection() )
          {
               test_string = current_step.GetRevA();

               if ( !test_string.IsValidPositiveFLOAT64() )
               {
                    SendErrorMessage( rev_text.GetText() + SPACE_STR  +
                     a_factor_text.GetText(), current_step.GetRevA() );
     
                    return FALSE;
               }
     
               if ( TEMP_DEPENDENT == current_step.GetRateConstantFormat( ) )
               {
                    test_string = current_step.GetRevM();

                    if ( !test_string.IsValidFLOAT64( ) )
                    {
                         SendErrorMessage( rev_text.GetText() + SPACE_STR  +
                          temp_exp_text.GetText(), current_step.GetRevM() );
     
                         return FALSE;
                    }
     
                    test_string = current_step.GetRevEa();

                    if ( !test_string.IsValidFLOAT64( ) )
                    {
                         SendErrorMessage( rev_text.GetText() + SPACE_STR  +
                          act_energy_text.GetText(), current_step.GetRevEa( ) );
     
                         return FALSE;
                    }
     
              }
         }
     }

        // If all the entry fields are valid, then store the data.    

     current_step.SetModified( TRUE );

     GetpObject()->SetTransferStep( GetpObject()->GetTransferListboxSelection(), current_step);

     return TRUE;
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::PrevHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::PrevHandler( SymbolButton* )
{

     UINT16 current_selection = GetpObject()->GetTransferListboxSelection( );

     // first test to be sure that a previous rxn step exists

     if ( current_selection == 0 )
          return;

     if ( data_altered )
     {
          // If the information regarding the previous transfer step is
          // changed, then store all the entries in the data structure.

          if ( ! ValidateAndStoreData( ) )
               return;
     }

     //  Display the previous reaction step

     GetpObject()->SetTransferListboxSelection( --current_selection );

     current_step = GetpObject()->GetTransferStep( current_selection );
     data_altered = FALSE;

     UpdatePanel( );

     //  Select line in main rxn list -- wdh added

#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // doesn't fcn on MAC

  //   GetpParentDialog()->SelectListboxEntry( current_selection );

#endif

     return;
}





// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::NextHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::NextHandler( SymbolButton* )
{
     // first test to be sure that the next rxn step exists
     // i.e, we are not currently editing the last step 

     UINT16 current_selection = GetpObject()->GetTransferListboxSelection( );

     UINT16 total_num_steps = GetpObject()->GetNumberOfTransferSteps( );

     if ( current_selection + 1  == total_num_steps )
          return;

     if ( data_altered )
     {
          if ( ! ValidateAndStoreData( ) )
               return;
     }

     // Display the next reaction step

     GetpObject()->SetTransferListboxSelection( ++current_selection );

     current_step = GetpObject()->GetTransferStep( current_selection );
     data_altered = FALSE;

     UpdatePanel( );


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     //  Select line in main rxn list

//   GetpParentDialog()->SelectListboxEntry( current_selection );

#endif

     return;
}






// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::UpdateEntryFieldText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::UpdateEntryFieldText( )
{
     // exponent entry fields 

     fwd_conc_exp_entry_field.SetText( current_step.GetFwdExponent( ) );

//   if ( SOURCE_TO_TARGET != GetSelectedDirection() )
          rev_conc_exp_entry_field.SetText( current_step.GetRevExponent( ) );

     // and rate constant parameter entry fields 

     fwd_a_factor_entry_field.SetText( current_step.GetFwdA( ) );

//     if ( arrh_form_rb.IsChecked( ) ) 
//     {
          fwd_temp_exp_entry_field.SetText( current_step.GetFwdM() );
          fwd_ea_entry_field.SetText( current_step.GetFwdEa() );
//     }

//     if ( reversible_chkbox.IsChecked( ) )
//     {
          rev_a_factor_entry_field.SetText( current_step.GetRevA( ) );

//          if ( arrh_form_rb.IsChecked( ) ) 
//          {
               rev_temp_exp_entry_field.SetText( current_step.GetRevM() );
               rev_ea_entry_field.SetText( current_step.GetRevEa() );
//          }
//     }


}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::SetRadioButtons()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::SetRadioButtons( )
{
     // set the rate constant format radio buttons 

     enum RATE_CONST_FORMAT rc_fmt = current_step.GetRateConstantFormat( );

     arrh_form_rb.Check( TEMP_INDEPENDENT != rc_fmt );
     single_rate_rb.Check( TEMP_INDEPENDENT == rc_fmt );

     // and the reversible check box 

     reversible_rb.Check( xfer_direction == REVERSIBLE );
     src_to_trgt_rb.Check( xfer_direction == SOURCE_TO_TARGET );
     trgt_to_src_rb.Check( xfer_direction == TARGET_TO_SOURCE );
}








// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::InitializeDialog( )
{

     UINT16 current_selection = GetpObject()->GetTransferListboxSelection( );
     UINT16 total_num_steps = GetpObject()->GetNumberOfTransferSteps( );

     // enable/disable scrolling buttons 

     if ( current_selection > 0 )
          prev_btn.Enable();
     else
          prev_btn.Disable();

     if ( current_selection < total_num_steps-1 )
          next_btn.Enable();
     else
          next_btn.Disable();

     // set the status text 

     status_text.SetText( status_text_prefix +
      String( current_selection+1 ) +
      String( ResId( vID_STR_XFER_STEP_STATUS_2 ) ) +
      String( total_num_steps) );

     if ( scheme_type != SW_TYPE_3D )
     {
          fwd_rate_law_text.SetText( 
           String( ResId( vID_STR_FWD_XFER_RATE_TEXT_1) ) +
           current_step.GetSpeciesName() +
           String( ResId( vID_STR_FWD_XFER_RATE_TEXT_2) ) );
     
          rev_rate_law_text.SetText( 
           String( ResId( vID_STR_REV_XFER_RATE_TEXT_1) ) +
           current_step.GetSpeciesName() +
           String( ResId( vID_STR_REV_XFER_RATE_TEXT_2) ) );
     }


     scheme_window.SetSourceName( GetpObject()->GetpSourceCompartment()->GetName() );
     scheme_window.SetTargetName( GetpObject()->GetpTargetCompartment()->GetName() );
     scheme_window.SetSpeciesName( current_step.GetSpeciesName() );
     scheme_window.SetTransferDirection( current_step.GetTransferDirection() );
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::UpdateEntryFieldStatus()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::UpdateEntryFieldStatus(  )
{
     enum XFER_DIRECTION direction   =  GetSelectedDirection();
     BOOL8               single_rate =  single_rate_rb.IsChecked();

     // deal with static text first 

     if ( single_rate)
     {
          temp_exp_text.Hide( );
          act_energy_text.Hide( );
          act_energy_units_text.Hide( );
          a_factor_text.SetText( String( ResId( vID_STR_RATE_CONSTANT ) ) );
     }
     else
     {
          temp_exp_text.Show( );
          act_energy_text.Show( );
          act_energy_units_text.Show( );
          a_factor_text.SetText( String( ResId( vID_STR_A_FACTOR ) ) );
     }


     if ( scheme_type != SW_TYPE_3D )
     {
          if ( SOURCE_TO_TARGET != direction )
          {
               // we need to show the info regarding reverse step 
     
                    rev_rate_law_text.Show( );
          
                    rev_conc_exp_entry_field.Show( );
                    rev_conc_exp_entry_field.SetText( current_step.GetRevExponent( ) );
          }
          else
          {
               rev_rate_law_text.Hide( );
               rev_conc_exp_entry_field.Hide( );
          }



          if ( TARGET_TO_SOURCE != direction )
          {
               fwd_rate_law_text.Show( );
     
               fwd_conc_exp_entry_field.Show( );
               fwd_conc_exp_entry_field.SetText( current_step.GetFwdExponent( ) );
          }
          else
          {
               fwd_rate_law_text.Hide( );
               fwd_conc_exp_entry_field.Hide( );
          }
     }



     // now set up the rate constant entry fields 

     // what is active or inactvie depends on (1) whether temp-dependent 
     // rate constants are in use, and (2) whether the step is reversible 

     // If Single rate is requested then hide the information regarding 
     // Temperature expponents and Activation Energies.  Otherwise 
     // make sure these values are visible and accessible to users.


     if ( SOURCE_TO_TARGET != direction )
     {
          // we need to show  reverse step info  
          rev_a_factor_entry_field.Show();

          if ( single_rate )
          {
               rev_temp_exp_entry_field.Hide( );
               rev_ea_entry_field.Hide( );
          }
          else
          {
               rev_temp_exp_entry_field.Show( );
               rev_ea_entry_field.Show( );
          }
     }
     else
     {
          rev_a_factor_entry_field.Hide();
          rev_temp_exp_entry_field.Hide( );
          rev_ea_entry_field.Hide( );
     }


     if ( TARGET_TO_SOURCE != direction )
     {
          // we need to show  forward step info  

          fwd_a_factor_entry_field.Show();

          if ( single_rate )
          {
               fwd_temp_exp_entry_field.Hide( );
               fwd_ea_entry_field.Hide( );
          }
          else
          {
               fwd_temp_exp_entry_field.Show( );
               fwd_ea_entry_field.Show( );
          }
     }
     else
     {
          fwd_a_factor_entry_field.Hide();
          fwd_temp_exp_entry_field.Hide( );
          fwd_ea_entry_field.Hide( );
     }



     return;
}







// ----------------------------------------------------------------------------
//            EditTransferProcessDialog::SendErrorMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditTransferProcessDialog::SendErrorMessage( const String& FieldName, const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_RDE_INPUT_ERR1 ) ) + UserEntry + 
      String( ResId( vID_STR_RDE_INPUT_ERR2 ) ) + FieldName +
      String( ResId( vID_STR_RDE_INPUT_ERR3 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}






// ----------------------------------------------------------------------------
//            EditTransferStepsDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum EDIT_XFER_RC EditTransferStepsDialog( TransferObject* pObject, Window* pParent)
{
     enum EDIT_XFER_RC rc;

     EditTransferProcessDialog* pdlg = new EditTransferProcessDialog( pObject, pParent);

     if ( pdlg )
     {
          rc = (EDIT_XFER_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return EX_RC_FAIL;
     }
}


