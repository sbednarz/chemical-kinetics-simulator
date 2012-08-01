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
//  EDITVDIF.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the EditVirtualDiffusionStepDialog class
//  the VSIM user interface.
//
//  CREATED : 8.13.98
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "editvdif.hxx"
#include "schemwin.hxx"
#include "mainapp.hxx"

#endif

#include "editvdif.hrc"
#include "strings.hrc"
#include "popup.hrc"







// ----------------------------------------------------------------------------
//                class EditVirtualDiffusionStepDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class EditVirtualDiffusionStepDialog : public ModalDialog
{
protected:

     // stuff up at the top of the dialog ( no group box )

	ListBox             species_lb;
	ListBox             species_lb2;
	FixedText           species_lb_label; 
	FixedText           species_lb_label2; 

     RadioButton         src_to_trgt_rb;
     RadioButton         reversible_rb;
     RadioButton         trgt_to_src_rb;
     GroupBox            group_box2;
  
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
     GroupBox            group_box3;
  
     // pushbuttons at bottom of dialog 

     OKButton            ok_btn;
     HelpButton          help_btn;
     PushButton          undo_btn;


     TransferStep        current_step;
     TransferStep*       p_original_step;
     TransferObject*     p_object;
    
     BOOL8               data_altered;
     String              species_name;
     String              species_name2;
     enum XFER_DIRECTION xfer_direction;

     TransferObject*     GetpObject() const { return p_object; }

     void                UpdateEntryFieldStatus( );
     void                UpdateEntryFieldText( );
     void                DrawSchematic( );
     void                SetRadioButtons( );
     void                InitializeDialog( );
     void                UpdatePanel( );
     void                SetUnitsText();
     void                InitializeListBox();

     BOOL8               ValidateAndStoreData( );
     void                SendErrorMessage( const String& FieldName, const String& UserEntry ) const;
     void                MakeSpeciesLine( const String& rName1, const String& rName2, String& rTarget );

     enum XFER_DIRECTION GetSelectedDirection();

public:

     EditVirtualDiffusionStepDialog( TransferObject* pTransferObj, TransferStep* pStep );

     void SetModifiedFlagHandler( Edit* );
     void RadioButtonHandler( RadioButton* pRButton );
     void DirectionRadioButtonHandler( RadioButton* pRButton );
     void ListboxSelectHandler( ListBox* );

     void CloseHandler( PushButton* );
     void UndoHandler( PushButton* );
};





// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::EditVirtualDiffusionStepDialog()
// ............................................................................
// 
//   constructor
//
// ----------------------------------------------------------------------------

EditVirtualDiffusionStepDialog::EditVirtualDiffusionStepDialog( TransferObject* pTransferObj, 
TransferStep* pStep ) :
ModalDialog( pTransferObj->GetpParent(), ResId( vID_DLG_EDIT_VIRTUAL_DIFFUSION ) ),
species_lb( this, ResId( vID_LB_SPECIES ) ),
species_lb2( this, ResId( vID_LB_SPECIES2 ) ),
species_lb_label( this, ResId( vID_STEXT_SPECIES_LABEL ) ), 
species_lb_label2( this, ResId( vID_STEXT_SPECIES2_LABEL ) ), 
scheme_window( this, ResId( vID_WIN_XFER_SCHEMATIC ) ),
src_to_trgt_rb( this, ResId( vID_RB_SRC_TO_TARGET ) ), 
reversible_rb( this, ResId( vID_RB_REVERSIBLE ) ),                        
trgt_to_src_rb( this, ResId( vID_RB_TARGET_TO_SRC ) ),                        
group_box2( this, ResId( 2 ) ),
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
group_box3( this, ResId( 3 ) ),
ok_btn( this, ResId( vID_PB_CLOSE ) ),
undo_btn( this, ResId( vID_PB_UNDO ) ), 
help_btn( this, ResId( vID_HBTN_HELP ) ),
current_step( *pStep ),
p_original_step( pStep ),
p_object( pTransferObj ),
data_altered( FALSE ),
species_name(),
species_name2(),
xfer_direction( SOURCE_TO_TARGET )
{
     FreeResource( );

     species_name = current_step.GetSpeciesName();

     species_name2 = current_step.GetSpeciesName2();

     InitializeListBox();

     UpdatePanel( );

     arrh_form_rb.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, RadioButtonHandler ) );
     single_rate_rb.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, RadioButtonHandler ) );

     reversible_rb.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, DirectionRadioButtonHandler ) );
     src_to_trgt_rb.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, DirectionRadioButtonHandler ) );
     trgt_to_src_rb.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, DirectionRadioButtonHandler ) );

     fwd_a_factor_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     fwd_temp_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     fwd_ea_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     rev_a_factor_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     rev_temp_exp_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     rev_ea_entry_field.ChangeModifyHdl( 
      LINK( this, EditVirtualDiffusionStepDialog, SetModifiedFlagHandler ) ); 
     species_lb.ChangeSelectHdl( LINK( this, EditVirtualDiffusionStepDialog, ListboxSelectHandler ) );   
     species_lb2.ChangeSelectHdl( LINK( this, EditVirtualDiffusionStepDialog, ListboxSelectHandler ) );   

     ok_btn.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl(
          LINK( this, EditVirtualDiffusionStepDialog, UndoHandler ) );

    GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );

     SetUnitsText();
}







// ----------------------------------------------------------------------------
//              EditVirtualDiffusionStepDialog::MakeSpeciesLine()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::MakeSpeciesLine( const String& rName1, const String& rName2, String& rTarget )
{
     rTarget = rName1 + String( ResId( vID_STR_SYMBOL_SLASH) ) + rName2;
}






// ----------------------------------------------------------------------------
//                  EditVirtualDiffusionStepDialog::ListboxSelectHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::ListboxSelectHandler( ListBox* )
{
     species_name = species_lb.GetSelectEntry();
     species_name2 = species_lb2.GetSelectEntry();

     String str;

	MakeSpeciesLine( species_name, species_name2, str );
     scheme_window.SetSpeciesName( str );

     DrawSchematic( );
     data_altered = TRUE;
}




// ----------------------------------------------------------------------------
//           EditVirtualDiffusionStepDialog::InitializeListBox()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::InitializeListBox()
{
     species_lb.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     species_lb.Clear( );


     species_lb2.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     species_lb2.Clear( );


    SpeciesDatabase* p_db = GetpObject()->GetpParent()->GetScheme().GetpSpeciesDatabase();
                                                                                          
     const UINT16 num_species = p_db->GetNumberOfSpecies( );                              

     if ( num_species > 0 )
     {
          // fill the listbox

          for ( USHORT i = 0; i < num_species; i++ )
          {
               species_lb.InsertEntry( p_db->GetSpeciesDataByPosition(i).GetName() );
               species_lb2.InsertEntry( p_db->GetSpeciesDataByPosition(i).GetName() );
          }

          // select the line number as previously saved 

//          species_lb.SelectEntryPos( GetpObject()->GetTransferListboxSelection( ) );
     }

     if ( 0 == species_name.Len()  )
     {
          species_name = species_lb.GetEntry( 0 );
          data_altered = TRUE;
     }

     species_lb.SelectEntry( species_name );


     species_lb.ChangeUpdateMode( TRUE );
     species_lb.Update( );


     if ( 0 == species_name2.Len()  )
     {
          species_name2 = species_lb2.GetEntry( 0 );
          data_altered  = TRUE;
     }

     species_lb2.SelectEntry( species_name2);

     species_lb2.ChangeUpdateMode( TRUE );
     species_lb2.Update( );
}

                                                                                          




// ----------------------------------------------------------------------------
//           EditVirtualDiffusionStepDialog::SetUnitsText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::SetUnitsText()
{
     ReactionScheme& rscheme = p_object->GetpParent()->GetScheme();

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

}





// ----------------------------------------------------------------------------
//           EditVirtualDiffusionStepDialog::GetSelectedDirection()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum XFER_DIRECTION EditVirtualDiffusionStepDialog::GetSelectedDirection()
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
//           EditVirtualDiffusionStepDialog::DirectionRadioButtonHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::DirectionRadioButtonHandler( RadioButton* )
{
     data_altered = TRUE;

     scheme_window.SetTransferDirection( GetSelectedDirection() );
     DrawSchematic( );
     UpdateEntryFieldStatus();
}







// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::UpdatePanel()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::UpdatePanel( )
{
     xfer_direction = current_step.GetTransferDirection();

     InitializeDialog( );
     DrawSchematic( );
     SetRadioButtons( );
     UpdateEntryFieldStatus( );
     UpdateEntryFieldText( );
     InitializeListBox();
}








// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::RadioButtonHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

#pragma argsused

void EditVirtualDiffusionStepDialog::RadioButtonHandler( RadioButton* pRButton  )
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
//            EditVirtualDiffusionStepDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::CloseHandler( PushButton* )
{
     if ( ( 0 == species_name.Len() ) ||  ( 0 == species_name.Len()  ) )
     {
           WarningBox( this, ResId( vID_POPUP_INVALID_SPECIES_SELECTION ) ).Execute();
           return;
     }

     if ( data_altered )
     {
          if ( ! ValidateAndStoreData( ) )
               return;

          EndDialog( EMX_RC_CLOSE );
     }
     else
          EndDialog( EMX_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::UndoHandler( PushButton* )
{
     current_step = *p_original_step;
     data_altered = FALSE;

     UpdatePanel( );
}






// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::DrawSchematic()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::DrawSchematic(  )
{
     scheme_window.Invalidate();
}







// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::SetModifiedFlagHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::SetModifiedFlagHandler( Edit* )
{
     data_altered = TRUE;
     return;
}






// ----------------------------------------------------------------------------
//               EditVirtualDiffusionStepDialog::ValidateAndStoreData( )
// ............................................................................
// 
//   this fcn retrieves the data from the entry fields, checks for its validity,
//   and stores it back in the parent Object's transfer list if valid. If
//   the data has a problem then a diagnostic message is displayed. The function 
//   returns TRUE is data is valid, FALSE if not 
//
// ----------------------------------------------------------------------------

BOOL8 EditVirtualDiffusionStepDialog::ValidateAndStoreData( )
{
     const String SPACE_STR( " ");

     // retrieve and test the data in all the numeric entry fields 
     
     current_step.SetFwdA( fwd_a_factor_entry_field.GetText( ) );
     current_step.SetFwdM( fwd_temp_exp_entry_field.GetText( ) );
     current_step.SetFwdEa( fwd_ea_entry_field.GetText( ) );

     current_step.SetRevA( rev_a_factor_entry_field.GetText( ) );
     current_step.SetRevM( rev_temp_exp_entry_field.GetText( ) );
     current_step.SetRevEa( rev_ea_entry_field.GetText( ) );

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

     // deal with species_name 

    SpeciesDatabase* p_db = GetpObject()->GetpParent()->GetScheme().GetpSpeciesDatabase();

    SpeciesDatabaseEntry entry = p_db->GetSpeciesDataByName( species_name );
                                                                                          
    current_step.SetSpecies( entry.GetName(), entry.GetID() );

    SpeciesDatabaseEntry entry2 = p_db->GetSpeciesDataByName( species_name2 );
                                                                                          
    current_step.SetSpecies2( entry2.GetName(), entry2.GetID() );

        // If all the entry fields are valid, then store the data.    

     current_step.SetModified( TRUE );

     *p_original_step = current_step;

     return TRUE;
}











// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::UpdateEntryFieldText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::UpdateEntryFieldText( )
{
     // rate constant parameter entry fields 

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
//            EditVirtualDiffusionStepDialog::SetRadioButtons()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::SetRadioButtons( )
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
//            EditVirtualDiffusionStepDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::InitializeDialog( )
{

     UINT16 current_selection = GetpObject()->GetTransferListboxSelection( );
     UINT16 total_num_steps = GetpObject()->GetNumberOfTransferSteps( );

     scheme_window.SetSourceName( GetpObject()->GetpSourceCompartment()->GetName() );
     scheme_window.SetTargetName( GetpObject()->GetpTargetCompartment()->GetName() );

     String str;
	MakeSpeciesLine( species_name, species_name2, str );

     scheme_window.SetSpeciesName( str );
     scheme_window.SetTransferDirection( current_step.GetTransferDirection() );
}







// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStepDialog::UpdateEntryFieldStatus()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::UpdateEntryFieldStatus(  )
{
     enum XFER_DIRECTION direction   =  GetSelectedDirection();
     BOOL8               single_rate =  single_rate_rb.IsChecked();

     // deal with static text first 

     if ( single_rate)
     {
          temp_exp_text.Hide( );
          act_energy_text.Hide( );
          act_energy_units_text.Hide( );
          a_factor_text.SetText( String( ResId( vID_STR_DIFFUSION_COEFFICIENT ) ) );
     }
     else
     {
          temp_exp_text.Show( );
          act_energy_text.Show( );
          act_energy_units_text.Show( );
          a_factor_text.SetText( String( ResId( vID_STR_A_FACTOR ) ) );
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
//            EditVirtualDiffusionStepDialog::SendErrorMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditVirtualDiffusionStepDialog::SendErrorMessage( const String& FieldName, const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_RDE_INPUT_ERR1 ) ) + UserEntry + 
      String( ResId( vID_STR_RDE_INPUT_ERR2 ) ) + FieldName +
      String( ResId( vID_STR_RDE_INPUT_ERR3 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}






// ----------------------------------------------------------------------------
//            EditVirtualDiffusionStep()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------


enum EDIT_VDIF_RC EditVirtualDiffusionStep( TransferObject* pParentObject, TransferStep* pStep)
{
     enum EDIT_VDIF_RC rc;

     EditVirtualDiffusionStepDialog* pdlg = new EditVirtualDiffusionStepDialog( pParentObject, pStep );

     if ( pdlg )
     {
          rc = (EDIT_VDIF_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return EVD_RC_FAIL;
     }
}


