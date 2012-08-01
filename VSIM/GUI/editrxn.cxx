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
//  EDITRXN.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This modules defines the Reaction Edit dialog used in the 
//  the VSIM user interface. 
//
//  CREATED : 1.30.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "mainapp.hxx"
#include "reaction.hxx"
#include "editrxn.hxx"
#include "comptdlg.hxx"
#include "ratelaw.hxx"

#endif


#include "strings.hrc"
#include "editrxn.hrc"


const String SPACE_STR( " ");



// ----------------------------------------------------------------------------
//                         class RxnDataEntryDialog 
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

class RxnDataEntryDialog : public ModalDialog
{

protected :
     CompartmentDialog* const pparent_dialog;     
     CompartmentObject* const pcompartment;

     FixedText           rxn_step_label;
     Edit                eqn_entry_field;
     RadioButton         arrh_form_r_btn;
     RadioButton         single_rate_r_btn;
     RadioButton         progr_rate_r_btn;
     PushButton          edit_prog_btn;
     GroupBox            group_box1;
     RadioButton         stoich_ratelaw_r_btn;
     RadioButton         special_ratelaw_r_btn;
     PushButton          edit_ratelaw_btn;
     GroupBox            group_box2;
     FixedText           fwd_label_text;
     FixedText           rev_label_text;
     Edit                fwd_a_factor_entry_field;
     Edit                fwd_T_exp_entry_field;
     Edit                fwd_Ea_entry_field;
     Edit                rev_a_factor_entry_field;
     Edit                rev_T_exp_entry_field;
     Edit                rev_Ea_entry_field;
     GroupBox            group_box_3;
     OKButton            close_btn;
     PushButton          add_rxn_btn;
     PushButton          delete_rxn_btn;
     PushButton          undo_btn;
     HelpButton          help_btn;
     FixedText           a_factor_label;
     FixedText           t_exp_label;
     FixedText           Ea_label;
     FixedText           a_factor_units_label;
     FixedText           Ea_units_label;
     SymbolButton        prev_btn;
     SymbolButton        next_btn;

     Reaction            current_rxn;

     // these two variables track whether any data has been altered and saved 
     // during this invocation of dlg  

     BOOL8               data_altered;         
     BOOL8               eqn_altered;

     // these two variables track only whether current rxn data has been 
     // altered and saved   

     BOOL8               current_data_altered; 
     BOOL8               current_eqn_altered;  

     // protected functions 

     void InitializeEntries( );
     void UpdatePanel( );
     void SetUnitsText();

     void CheckEqnForReversibility( );
     void CheckRateConstantFormat( );

     BOOL ValidateData( );
     void StoreData( );

     void SetCurrentDataAlteredFlag();
     void ClearCurrentDataAlteredFlag();

     void NumericInputErrorMsg( const String& FieldName, const String& UserEntry ) const;
     void DisplayEqnErrorMessage(enum EQN_ANALYSIS_RESULT ErrorCode ) const;
     void StartEditRateLawDialog();

     CompartmentObject* const GetpCompartment( ) { return pcompartment; }
     CompartmentDialog* const GetpParentDialog( ) { return pparent_dialog; }

public :

     RxnDataEntryDialog( CompartmentDialog* pParent );

     void PrevHandler( SymbolButton* aSymbolButton );
     void NextHandler( SymbolButton* aSymbolButton );

     void SetModifiedFlagHandler( Edit* );

     void ReactionStepModifiedHandler( Edit* pEdit );

     void RateConstantHandler( RadioButton* );

     void RateLawHandler( RadioButton* pRButton );
     void EditRateLawHandler( PushButton* ); 
     void EditProgRateHandler( PushButton* ); 

     void CloseHandler( PushButton* pButton );
     void AddRxnHandler( PushButton* pButton );
     void DeleteRxnHandler( PushButton* pButton );
     void UndoHandler( PushButton* pButton );
};



// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::SetUnitsText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::SetUnitsText()
{
     ReactionScheme& rscheme = GetpCompartment()->GetpParent()->GetScheme();

     a_factor_units_label.SetText( String( ResId( vID_STR_LEFT_BRACKET ) ) +
                                   rscheme.GetVolumeUnits() +
                                   String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                   rscheme.GetAmountUnits() +
                                   String( ResId( vID_STR_SYMBOL_DASH ) ) +
                                   rscheme.GetTimeUnits() +
                                   String( ResId( vID_STR_RIGHT_BRACKET ) ) + 
                                   String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
                                   String( ResId( vID_STR_UNITS ) )  );

     Ea_units_label.SetText( rscheme.GetLargeEnergyUnits() + 
                             String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                             rscheme.GetAmountUnits() );
}



// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::RxnDataEntryDialog( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

RxnDataEntryDialog::RxnDataEntryDialog( CompartmentDialog* pParent ) :
pparent_dialog ( pParent ),
ModalDialog ( pParent, ResId ( vID_DLG_EDIT_RXN ) ),
rxn_step_label ( this, ResId ( vID_STEXT_REACTION_STEP ) ),
eqn_entry_field ( this, ResId ( vID_EF_EQUATION ) ),
arrh_form_r_btn ( this, ResId ( vID_RB_ARRH_FORM ) ),
single_rate_r_btn ( this, ResId ( vID_RB_SINGLE_RATE ) ),
progr_rate_r_btn ( this, ResId ( vID_RB_PROG_RATE ) ), 
edit_prog_btn ( this, ResId ( vID_PB_EDIT_PROG ) ), 
group_box1 ( this, ResId ( 1 ) ),
group_box2 ( this, ResId ( 2 ) ),
stoich_ratelaw_r_btn ( this, ResId ( vID_RB_STOICH_RATELAW ) ),
special_ratelaw_r_btn ( this, ResId ( vID_RB_SPEC_RATELAW ) ),
edit_ratelaw_btn ( this, ResId ( vID_PB_EDIT_RATELAW ) ),
fwd_label_text ( this, ResId ( vID_STEXT_FORWARD ) ),
rev_label_text ( this, ResId ( vID_STEXT_REVERSE ) ),
fwd_a_factor_entry_field ( this, ResId ( vID_EF_FWD_AFACTOR ) ),
fwd_T_exp_entry_field ( this, ResId ( vID_EF_FWD_TEXP ) ),
fwd_Ea_entry_field ( this, ResId ( vID_EF_FWD_EA ) ),
rev_a_factor_entry_field ( this, ResId ( vID_EF_REV_A_FACTOR ) ),
rev_T_exp_entry_field ( this, ResId ( vID_EF_REV_TEXP ) ),
rev_Ea_entry_field ( this, ResId ( vID_EF_REV_EA ) ),
group_box_3 ( this, ResId ( 3 ) ),
close_btn ( this, ResId ( vID_PB_CLOSE ) ),
add_rxn_btn ( this, ResId ( vID_PB_ADD_REACTION ) ),
delete_rxn_btn ( this, ResId ( vID_PB_DELETE_REACTION ) ),
undo_btn ( this, ResId ( vID_PB_UNDO ) ),
help_btn ( this, ResId ( vID_HBTN_HELP) ),
a_factor_label ( this, ResId ( vID_STEXT_AFACTOR ) ),
t_exp_label ( this, ResId ( vID_STEXT_TEMP_EXPONENT ) ),
Ea_label ( this, ResId ( vID_STEXT_EA ) ),
a_factor_units_label ( this, ResId ( vID_STEXT_AFACTOR_UNITS ) ),
Ea_units_label ( this, ResId ( vID_STEXT_EA_UNITS ) ),
prev_btn ( this, ResId ( vID_SBTN_PREV_STEP_BTN ) ),
next_btn ( this, ResId ( vID_SBTN_NEXT_STEP_BTN ) ),
pcompartment( GetpParentDialog()->GetpCompartment()  ),
current_rxn( GetpCompartment()->GetRxnStep( GetpCompartment()->GetRxnListboxSelection() ) ),
data_altered( FALSE ),
eqn_altered( FALSE ),
current_data_altered( FALSE ),
current_eqn_altered( FALSE )
{
     FreeResource( );

     SetText( GetText( ) + GetpCompartment()->GetName() );

     InitializeEntries( );
     SetUnitsText();

     // make links to button handlers

     prev_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, PrevHandler ) );
     next_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, NextHandler ) );
     eqn_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, ReactionStepModifiedHandler ) );
     arrh_form_r_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateConstantHandler ) );
     single_rate_r_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateConstantHandler ) );
     progr_rate_r_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateConstantHandler ) );

     stoich_ratelaw_r_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateLawHandler ) );
     special_ratelaw_r_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateLawHandler ) );
     edit_ratelaw_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, EditRateLawHandler ) );
     edit_prog_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, EditProgRateHandler ) );

     // entry field modified handlers 

     fwd_a_factor_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     fwd_T_exp_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     fwd_Ea_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     rev_a_factor_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     rev_T_exp_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     rev_Ea_entry_field.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );

     // main button handlers 

     close_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, CloseHandler ) );
     add_rxn_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, AddRxnHandler ) );
     delete_rxn_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, DeleteRxnHandler ) );
     undo_btn.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, UndoHandler ) );

    GetpMainApp()->CascadeWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::InitializeEntries( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::InitializeEntries( )
{
// initialize the AFactor and Activation Energy units

// wdh - no units fcn yet 
//     a_factor_units_label.SetText( String( "(" ) +
//          String( msimAFactorUnits( 0.0, pSimInstance->conc_units,
//                    pSimInstance->volume_units, pSimInstance->time_units ) ) +
//          String( ") units" ) );
//     Ea_units_label.SetText( String( "k" ) +
//          String( msimEaUnits( pSimInstance-> energy_units ) ) );


     UpdatePanel( );

     ClearCurrentDataAlteredFlag();

     // following clause added 8.14.98 to deal with initialization of dialog with empty added reaction

     if ( current_rxn.IsNullEquation() )     
     {                                       
          data_altered = TRUE;
          eqn_altered = TRUE; 
          current_data_altered = TRUE; 
          current_eqn_altered = TRUE;
     }
     else
     {
          current_eqn_altered = FALSE;
     }

    //current_eqn_altered = FALSE;  8.14.98 commented out
}







// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::UpdatePanel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::UpdatePanel( )
{
     const USHORT current_selection = GetpCompartment()->GetRxnListboxSelection( );
     const USHORT num_steps = GetpCompartment()->GetNumberOfRxnSteps( );

     // build label that appears above the equation entry field 

     rxn_step_label.SetText( String( ResId( vID_STR_REACTION_STEP ) ) +
          String( current_selection + 1 ) +
          String( ResId( vID_STR_OF) ) + String( num_steps ) );

     // update all entry fields with new data 

     eqn_entry_field.SetText( current_rxn.GetEquation() );
     fwd_a_factor_entry_field.SetText( current_rxn.GetFwdA() );
     fwd_T_exp_entry_field.SetText( current_rxn.GetFwdM( ) );
     fwd_Ea_entry_field.SetText( current_rxn.GetFwdEa( ) );

     CheckEqnForReversibility( );

     arrh_form_r_btn.Check( TEMP_DEPENDENT == current_rxn.GetRateConstantFormat() );
     single_rate_r_btn.Check( TEMP_INDEPENDENT == current_rxn.GetRateConstantFormat() );
     progr_rate_r_btn.Check( PROGRAMMED == current_rxn.GetRateConstantFormat() ); 

     if ( PROGRAMMED == current_rxn.GetRateConstantFormat() )
     {
          if ( current_rxn.GetProgRateProfileName().Len() > 0)
          {
               progr_rate_r_btn.SetText( String( ResId( vID_STR_PROGRATE_RB_LABEL ) ) +
                DirEntry( current_rxn.GetProgRateProfileName() ).GetName() );
          }
          else
          {
               progr_rate_r_btn.SetText( String( ResId( vID_STR_PROGRATE_RB_LABEL ) )  + 
                String( ResId( vID_STR_FILENAME_NONE_SELECTED ) ) );
          }
    }

     CheckRateConstantFormat( );

     stoich_ratelaw_r_btn.Check( !current_rxn.HasNonStandardRateLaw( ) );
     special_ratelaw_r_btn.Check( current_rxn.HasNonStandardRateLaw( ) );

     if (  0 == current_selection )
          prev_btn.Disable( );
     else
          prev_btn.Enable( );

     if ( (current_selection + 1) != num_steps )
          next_btn.Enable( );
     else
          next_btn.Disable( );

     // If using a special rate law, then enable the RateLaw Button,
     // otherwise, disable the button

     if ( special_ratelaw_r_btn.IsChecked( ) )
          edit_ratelaw_btn.Enable( );
     else
          edit_ratelaw_btn.Disable( );
}






// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::PrevHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::PrevHandler( SymbolButton* )
{

     USHORT current_selection = GetpCompartment( )->GetRxnListboxSelection( );

     // first test to be sure that a previous rxn step exists

     if ( current_selection == 0 )
          return;

     if ( current_data_altered )
     {
          if ( ! ValidateData( ) )
               return;
          else
               StoreData();
     }


     // replace text in main rxn list box if altered
     // ( note -- this does not work under MAC )

     if ( current_eqn_altered )
          GetpParentDialog()->ReplaceListboxEntry( current_selection, current_rxn.GetEquation() );


     //  Display the previous reaction step

     GetpCompartment()->SetRxnListboxSelection( --current_selection );
     current_rxn = GetpCompartment()->GetRxnStep( current_selection );

     UpdatePanel( );

     //  Select line in main rxn list -- wdh added
     //  ( note - doesn't fcn on MAC )

     GetpParentDialog()->SelectListboxEntry( current_selection );
}







// ----------------------------------------------------------------------------
//                      RxnDataEntryDialog::NextHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::NextHandler( SymbolButton* )
{
     // first test to be sure that the next rxn step exists
     // i.e, we are not currently editing the last step 

     USHORT current_selection = GetpCompartment()->GetRxnListboxSelection();
     const USHORT num_steps = GetpCompartment()->GetNumberOfRxnSteps( );

     if ( current_selection + 1  == num_steps )
          return;

     if ( current_data_altered )
     {
          // If the information regarding the previous reaction step is 
          // changed, then store all the entries in the data structure. 

          if ( ! ValidateData( ) )
               return;
          else
               StoreData();
     }

     // replace text in main rxn list box if altered
     // ( note - this does not work under MAC )

     if ( current_eqn_altered )
          GetpParentDialog()->ReplaceListboxEntry( current_selection, current_rxn.GetEquation() );

     // Display the next reaction step

     GetpCompartment()->SetRxnListboxSelection( ++current_selection );
     
     current_rxn = GetpCompartment()->GetRxnStep( current_selection );

     UpdatePanel( );

     //  Select line in main rxn list
     //  (this does not work under MAC)

     GetpParentDialog()->SelectListboxEntry( current_selection );
}




// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::ReactionStepModifiedHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::ReactionStepModifiedHandler( Edit* )
{
     CheckEqnForReversibility( );
     SetCurrentDataAlteredFlag( );
     current_eqn_altered = TRUE;
}







// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::RateConstantHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::RateConstantHandler( RadioButton* )
{
     // Update the appropriate information

     CheckRateConstantFormat( );
     SetCurrentDataAlteredFlag( );
}






// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::RateLawHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::RateLawHandler( RadioButton* )
{
     // If using a special rate law, enable the RateLaw Button and start
     // the Edit Rate Law dialog box, otherwise, disable the button 

     SetCurrentDataAlteredFlag( );

     if ( special_ratelaw_r_btn.IsChecked( ) )
     {
          edit_ratelaw_btn.Enable( );

          StartEditRateLawDialog();

     }
     else
          edit_ratelaw_btn.Disable( );

     return;
}



// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::StartEditRateLawDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::StartEditRateLawDialog()
{
     current_rxn.SetEquation( eqn_entry_field.GetText( ));

     enum EQN_ANALYSIS_RESULT rc;

     if ( current_rxn.SyntaxIsValid( rc ) )
     {
         if ( RL_RC_ACCEPT == EditRateLaw( this, current_rxn ) )
               SetCurrentDataAlteredFlag();
     }
     else
         DisplayEqnErrorMessage( rc );
}





// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::EditRateLawHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::EditRateLawHandler( PushButton* )
{
     StartEditRateLawDialog();
}




// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::EditProgRateHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::EditProgRateHandler( PushButton* )
{
     String tempstr( current_rxn.GetProgRateProfileName() );

     if ( TRUE == EditProgRateConstant( this, GetpCompartment()->GetpParent()->GetScheme(), 
          tempstr ) 
        )
     {
          current_rxn.SetProgRateProfileName( tempstr );

          progr_rate_r_btn.SetText( String( ResId( vID_STR_PROGRATE_RB_LABEL ) ) +
           DirEntry( current_rxn.GetProgRateProfileName() ).GetName() );
          SetCurrentDataAlteredFlag();
     }
}




// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::CloseHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::CloseHandler( PushButton* )
{
     // test to see if there is only one rxn step defined. If so and it is blank
     // then let the user exit the dialog, delete the rxn step and clean things up 

     if ( GetpCompartment()->GetNumberOfRxnSteps() == 1 )
     {
          current_rxn.SetEquation( eqn_entry_field.GetText( ));

          if (current_rxn.IsNullEquation() )
          {
               // if string is blank or null,  then clean up and leave


               GetpCompartment()->DeleteRxnStep( 0 );

               GetpParentDialog()->RemoveListboxEntry( 0 );

               EndDialog( ER_RC_ACCEPT );

               return;
          }
     }

     // validate current data if modified 

     if ( current_data_altered )
     {
          // Update everything

          if ( ValidateData( ) )
          {
               StoreData();
               GetpCompartment( )->UpdateSpeciesList( );
          }
          else
               return;
     }

     // we get to here if no errors are found during rxn scheme analysis

     // if an equation has been altered then the species list may be 
     // invliad - set appropriate falg in SchemeWindow parent 

     if ( eqn_altered)
          GetpCompartment()->GetpParent()->GetScheme().SetSpeciesDataModifiedFlag( TRUE );

     EndDialog( data_altered ? ER_RC_ACCEPT : ER_RC_CANCEL );
}







// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::AddRxnHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::AddRxnHandler( PushButton* )
{
     USHORT current_selection = GetpCompartment()->GetRxnListboxSelection();

     // If the information regarding the previous reaction step is
     // changed, then store all the entries in the data structure.

     if ( current_data_altered )
     {
          if ( ! ValidateData( ) )
               return;
          else
          {
               StoreData();

               // replace eqn text in parent dialog listbox 
               // ( note - this does not work under MAC )
          
               if ( current_eqn_altered )
                    GetpParentDialog()->ReplaceListboxEntry( current_selection, 
                     current_rxn.GetEquation() );
          }
          
     }

     // try to create new reaction object 
     // note we will always have at least one reaction already in existence 
     // when we are in this dialog so will not have to worry about
     // creating reaction number 0 as we did in comptdlg.cxx

     current_selection++;

     if ( GetpCompartment()->AddNewRxnStep( current_selection) )
     {
          current_rxn = GetpCompartment()->GetRxnStep( current_selection );

          GetpParentDialog()->AddListboxEntry( current_selection, current_rxn.GetEquation() );

          GetpParentDialog()->SelectListboxEntry( current_selection );

          GetpCompartment()->SetRxnListboxSelection( current_selection );

          InitializeEntries();

          current_data_altered = current_eqn_altered = TRUE;
     }
}






// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::DeleteRxnHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::DeleteRxnHandler( PushButton* )
{
     USHORT current_selection = GetpCompartment()->GetRxnListboxSelection();

     const USHORT num_steps = GetpCompartment()->GetNumberOfRxnSteps( );

     ASSERT( num_steps != 0 );


     // Delete the current reaction step 

     GetpCompartment()->DeleteRxnStep( current_selection );

     // deletion is irreversible - data is irretrievably altered 

     data_altered = TRUE;
     eqn_altered = TRUE;

     // update parent dialog
     // ( note - this does not work under MAC )

     GetpParentDialog()->RemoveListboxEntry( current_selection );


     if ( num_steps == 1 )
     {
          // If we deleted the only reaction, then end the dialog box 

          GetpCompartment()->GetpParent()->GetScheme().SetSpeciesDataModifiedFlag( TRUE );
          GetpCompartment()->SetRxnListboxSelection( 0 );

          EndDialog( ER_RC_ACCEPT );
     }
     else
     {
          // if we are deleting the bottom step in the reaction step scheme listbox 
          // we need to select the equation item preceding the one just deleted

          if ( current_selection + 1  == num_steps )
               current_selection--;


          GetpCompartment()->SetRxnListboxSelection( current_selection );
          GetpParentDialog()->SelectListboxEntry( current_selection );

          // otherwise Update the dialog panel with the next reaction step 

          current_rxn = GetpCompartment()->GetRxnStep( current_selection );

          InitializeEntries();
     }
}





// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::UndoHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::UndoHandler( PushButton* )
{
     // Undo all the changes made 

     current_rxn = GetpCompartment()->GetRxnStep( GetpCompartment()->GetRxnListboxSelection() );

     InitializeEntries();
}







// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::CheckEqnForReversibility()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::CheckEqnForReversibility( )
{

     // Read the reaction step and determine if the reaction is reversible //
     // or not.  If it is then need to enable the reverse information      //
     // entries.  Otherwise, make suure they are disabled.                 //
     // Also set text in the appropriate dialog entries.                   //

     current_rxn.SetEquation( eqn_entry_field.GetText( ) );

     if ( current_rxn.IsReversible() )
     {
          rev_label_text.Enable( );
          rev_a_factor_entry_field.Enable( );
          if ( ! single_rate_r_btn.IsChecked( ) )
          {
               rev_T_exp_entry_field.Enable( );
               rev_Ea_entry_field.Enable( );
          }
     }
     else
     {
          rev_label_text.Disable( );
          rev_a_factor_entry_field.Disable( );
          rev_T_exp_entry_field.Disable( );
          rev_Ea_entry_field.Disable( );
     }

     rev_a_factor_entry_field.SetText( current_rxn.GetRevA( ) );
     rev_T_exp_entry_field.SetText( current_rxn.GetRevM( ) );
     rev_Ea_entry_field.SetText( current_rxn.GetRevEa( ) );

     return;
}






// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::CheckRateConstantFormat( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::CheckRateConstantFormat( )
{
     // if the rate is programmed then disable all rate constant
     // entry stuff and enable the pushbutton so user can get
     // to programming dialog.
     // otherwise enable rate constant entry items and disbale pushbutton

     if (progr_rate_r_btn.IsChecked() )
     {
          edit_prog_btn.Enable();

          fwd_label_text.Disable();
          rev_label_text.Disable();
          fwd_a_factor_entry_field.Disable();
          fwd_T_exp_entry_field.Disable();
          fwd_Ea_entry_field.Disable();
          rev_a_factor_entry_field.Disable();
          rev_T_exp_entry_field.Disable();
          rev_Ea_entry_field.Disable();

          a_factor_label.Disable();
          t_exp_label.Disable();
          Ea_label.Disable();
          a_factor_units_label.Disable();
          Ea_units_label.Disable();
     }
     else
     {
          edit_prog_btn.Disable();

          fwd_label_text.Enable();
          rev_label_text.Enable();
          fwd_a_factor_entry_field.Enable();
          fwd_T_exp_entry_field.Enable();
          fwd_Ea_entry_field.Enable();
          rev_a_factor_entry_field.Enable();
          rev_T_exp_entry_field.Enable();
          rev_Ea_entry_field.Enable();

          a_factor_label.Enable();
          t_exp_label.Enable();
          Ea_label.Enable();
          a_factor_units_label.Enable();
          Ea_units_label.Enable();

          // If Single rate is requested then hide the information regarding 
          // with Temperature exponents and Activation Energies.  Otherwise 
          // make sure these values are visible and accessible to users.    

          if ( single_rate_r_btn.IsChecked( ) )
          {
               t_exp_label.Hide( );
               Ea_label.Hide( );
               Ea_units_label.Hide( );
               a_factor_label.SetText( String( ResId( vID_STR_RATE_CONSTANT ) ) );
     
               fwd_T_exp_entry_field.Hide( );
               fwd_Ea_entry_field.Hide( );
               rev_T_exp_entry_field.Hide( );
               rev_Ea_entry_field.Hide( );
          }
          else
          {
               t_exp_label.Show( );
               Ea_label.Show( );
               Ea_units_label.Show( );
               a_factor_label.SetText( String( ResId( vID_STR_A_FACTOR ) ) );
     
               fwd_T_exp_entry_field.Show( );
               fwd_Ea_entry_field.Show( );
               rev_T_exp_entry_field.Show( );
               rev_Ea_entry_field.Show( );
     
               if ( current_rxn.IsReversible()  )
               {
                    rev_T_exp_entry_field.Enable( );
                    rev_Ea_entry_field.Enable( );
               }
          }
     }
}



// ----------------------------------------------------------------------------
//                     RxnDataEntryDialog::StoreData( ) 
// ............................................................................
// 
//   this fcn stores the current rxn data back in the parent Compartment's 
//   reaction list.
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::StoreData( )
{
     UINT16 current_selection = GetpCompartment()->GetRxnListboxSelection();
     GetpCompartment()->SetRxnStep( current_selection, current_rxn );
     GetpParentDialog()->ReplaceListboxEntry( current_selection, current_rxn.GetEquation() );
  
     data_altered = TRUE;

     if ( current_eqn_altered)
          eqn_altered = TRUE;
}





// ----------------------------------------------------------------------------
//                     RxnDataEntryDialog::ValidateData( ) 
// ............................................................................
// 
//   this fcn retrieves the data from the entry fields and checks for its 
//   validity. If the data has a problem then a diagnostic message is 
//   displayed. The function returns TRUE is data is valid, FALSE if not 
//
// ----------------------------------------------------------------------------

BOOL RxnDataEntryDialog::ValidateData( )
{
     // Check for the presence of a valid reaction equation and for valid
     // numeric values in the rate parameter entry fields.

     // first get the string from the equation entry field 

     enum EQN_ANALYSIS_RESULT error_code;

     current_rxn.SetEquation( eqn_entry_field.GetText( ) );

     if ( !current_rxn.SyntaxIsValid( error_code ) )
     {
          DisplayEqnErrorMessage( error_code ) ;
          return FALSE;
     }
     else
     {
          // now retrieve and test the data in all the numeric entry fields 

          current_rxn.SetFwdA( fwd_a_factor_entry_field.GetText( ) );
          current_rxn.SetFwdM( fwd_T_exp_entry_field.GetText( ) );
          current_rxn.SetFwdEa( fwd_Ea_entry_field.GetText( ) );

          current_rxn.SetRevA( rev_a_factor_entry_field.GetText( ) );
          current_rxn.SetRevM( rev_T_exp_entry_field.GetText( ) );
          current_rxn.SetRevEa( rev_Ea_entry_field.GetText( ) );

          // get the user's selection for the rate constant format

          enum RATE_CONST_FORMAT rcf = TEMP_DEPENDENT;

          if ( single_rate_r_btn.IsChecked() ) 
               rcf = TEMP_INDEPENDENT;

          if ( progr_rate_r_btn.IsChecked() ) 
               rcf = PROGRAMMED;

          current_rxn.SetRateConstantFormat( rcf );


          current_rxn.SetNonStandardRateLaw( special_ratelaw_r_btn.IsChecked() );

          // now validate input data if rate constant is not programmed 

          if ( PROGRAMMED != rcf )
          {
               NumericString test_string = current_rxn.GetFwdA();
     
               if ( !test_string.IsValidPositiveFLOAT64() )
               {
                    NumericInputErrorMsg( fwd_label_text.GetText() + SPACE_STR  +
                     a_factor_label.GetText(), current_rxn.GetFwdA() );
     
                    return FALSE;
               }
     
               if ( TEMP_DEPENDENT == current_rxn.GetRateConstantFormat( ) )
               {
                    test_string = current_rxn.GetFwdM();
                    if ( !test_string.IsValidFLOAT64( ) )
                    {
                         NumericInputErrorMsg( fwd_label_text.GetText() + SPACE_STR  +
                          t_exp_label.GetText(), current_rxn.GetFwdM() );
     
                         return FALSE;
                    }
     
                    test_string = current_rxn.GetFwdEa();
                    if ( !test_string.IsValidFLOAT64( ) )
                    {
                         NumericInputErrorMsg( fwd_label_text.GetText() + SPACE_STR  +
                          Ea_label.GetText(), current_rxn.GetFwdEa( ) );
     
                         return FALSE;
                    }
     
              }
     
              // if the equation is reversible repeat the above process for the 
              // numerical values for the reversible information 
     
              if ( current_rxn.IsReversible( ) )
              {
                    test_string = current_rxn.GetRevA();
                    if ( !test_string.IsValidPositiveFLOAT64() )
                    {
                         NumericInputErrorMsg( rev_label_text.GetText() + SPACE_STR  +
                          a_factor_label.GetText(), current_rxn.GetRevA() );
          
                         return FALSE;
                    }
          
                    if ( TEMP_DEPENDENT == current_rxn.GetRateConstantFormat( ) )
                    {
                         test_string = current_rxn.GetRevM();
     
                         if ( !test_string.IsValidFLOAT64( ) )
                         {
                              NumericInputErrorMsg( rev_label_text.GetText() + SPACE_STR  +
                               t_exp_label.GetText(), current_rxn.GetRevM() );
          
                              return FALSE;
                         }
          
                         test_string = current_rxn.GetRevEa();
     
                         if ( !test_string.IsValidFLOAT64( ) )
                         {
                              NumericInputErrorMsg( rev_label_text.GetText() + SPACE_STR  +
                               Ea_label.GetText(), current_rxn.GetRevEa( ) );
          
                              return FALSE;
                         }
          
                    }
              }
          }
     }

     return TRUE;
}





// ----------------------------------------------------------------------------
//                  RxnDataEntryDialog::SetCurrentDataAlteredFlag()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::SetCurrentDataAlteredFlag()
{
          undo_btn.Enable();
          current_data_altered = TRUE;
}



// ----------------------------------------------------------------------------
//                RxnDataEntryDialog::ClearCurrentDataAlteredFlag()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::ClearCurrentDataAlteredFlag()
{
          undo_btn.Disable();
          current_data_altered = FALSE;
}



// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::SetModifiedFlagHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::SetModifiedFlagHandler( Edit* )
{
     if ( !data_altered )
          SetCurrentDataAlteredFlag( );
}






// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::NumericInputErrorMsg( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::NumericInputErrorMsg( const String& FieldName, const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_RDE_INPUT_ERR1 ) ) + UserEntry + 
      String( ResId( vID_STR_RDE_INPUT_ERR2 ) ) + FieldName +
      String( ResId( vID_STR_RDE_INPUT_ERR3 ) );

     WarningBox( GetParent(), WB_OK | WB_DEF_OK, message ).Execute( );
}






// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::DisplayEqnErrorMessage()
// ............................................................................
// 
// Function for informing the user that an error was detected during       
//  analysis of the rxn scheme                                             
//
// ----------------------------------------------------------------------------

void RxnDataEntryDialog::DisplayEqnErrorMessage( enum EQN_ANALYSIS_RESULT ErrorCode ) const
{
     String err_msg;

     switch ( ErrorCode )
     {

     case EQN_NO_ERROR :

          err_msg = String( ResId( vID_STR_EQN_NO_ERROR ) );
          break;

     case EQN_OTHER :
     default :

          err_msg = String( ResId( vID_STR_EQN_UNKNOWN_ERROR ) );
          break;

     case EQN_SYNTAX :

          err_msg = String( ResId( vID_STR_EQN_SYNTAX_ERROR ) );
          break;

     case EQN_SPECIES_ERROR :

          err_msg = String( ResId( vID_STR_EQN_SPECIES_ERROR ) );
          break;

     case EQN_STOICH_ERROR :

          err_msg = String( ResId( vID_STR_EQN_STOICH_ERROR ) );
          break;

     case EQN_TOO_MANY_COMPS :

          err_msg = String( ResId( vID_STR_EQN_NUMCOMP_ERROR ) );
          break;

     case EQN_NAME_APPEARS_TWICE :

          err_msg = String( ResId( vID_STR_EQN_DBLNAME_ERROR ) );
          break;

     case EQN_EMPTY_EQUATION :

          err_msg = String( ResId( vID_STR_EQN_EMPTY_ERROR ) );
          break;

     case EQN_SPECIES_NAME_TOO_LONG :

          err_msg = String( ResId( vID_STR_EQN_NAME_LEN_ERROR ) );
          break;

     }

     String msg( ResId( vID_STR_EQN_ANALYSIS_ERROR1 ) );
     msg += err_msg;
     msg += String( ResId( vID_STR_EQN_ANALYSIS_ERROR2 ) ); 

     WarningBox( GetParent(), WB_OK | WB_DEF_OK, msg ).Execute( );
}






// ----------------------------------------------------------------------------
//                              EditReactionDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum EDIT_RXN_RC EditReactionDialog( CompartmentDialog* pParent )
{
     enum EDIT_RXN_RC rc;

     RxnDataEntryDialog* pdlg = new RxnDataEntryDialog( pParent );

     if ( pdlg )
     {
          rc = (EDIT_RXN_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return ER_RC_FAIL;
     }
}


