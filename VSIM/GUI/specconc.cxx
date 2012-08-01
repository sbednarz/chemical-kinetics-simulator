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
//  SPECCONC.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the EditSpeciesConcDialog class
//  the VSIM user interface. 
//
//  CREATED : 5.01.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "specconc.hxx"
#include "mainapp.hxx"

#endif

#include "specconc.hrc"
#include "strings.hrc"




// ----------------------------------------------------------------------------
//                     class SpeciesConcDialog 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class SpeciesConcDialog : public ModalDialog
{
protected:
    GroupBox conc_label_groupbox;
    ListBox species_name_listbox;
    FixedText species_label_text;
    Edit conc_entryfield;
    FixedText units_text;
    OKButton close_btn;
    PushButton defaults_btn;
    PushButton undo_btn;
    HelpButton help_btn;
    FixedText aFixedText2;
    FixedText species_name_text;
    CheckBox show_reactive_species_checkbox;

    CompartmentDialog* pparent_window;
    CompartmentObject* pparent_object;

     BOOL8               data_altered;
     BOOL8               current_data_altered;

     void                InitializeDialog( );
     void                InitializeListBox( );
     void                UpdateEntryField( );
     BOOL8               ReadEntryField( );

     CompartmentDialog*  GetpParent( ) { return pparent_window; } 
     CompartmentObject*  GetpParentObject( ) { return pparent_object; } 

public:
    SpeciesConcDialog( CompartmentDialog* pParent);

     void CheckBoxHandler( CheckBox* );
     void TextModifiedHandler( Edit* );
     void ListboxSelectHandler( ListBox* );
     void CloseHandler( OKButton * );
     void UndoHandler( PushButton * );
     void DefaultsHandler( PushButton * );
};







// ----------------------------------------------------------------------------
//                   SpeciesConcDialog::SpeciesConcDialog()
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

SpeciesConcDialog::SpeciesConcDialog( CompartmentDialog* pParent ) :
ModalDialog( pParent, ResId( vID_DLG_INIT_CONC ) ),
conc_label_groupbox( this, ResId( vID_GB_CONC_LABEL ) ),
species_name_listbox( this, ResId( vID_LB_SPECIES_LIST ) ),
species_label_text( this, ResId( vID_STEXT_SPECIES_LABEL ) ),
conc_entryfield( this, ResId( vID_EDIT_CONC ) ),
units_text( this, ResId( vID_STEXT_CONC_UNITS ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
aFixedText2( this, ResId( 2 ) ),
species_name_text( this, ResId( vID_STEXT_SPECIES_NAME ) ),
show_reactive_species_checkbox( this, ResId( vID_CB_SHOW_REACTIVE_SPECIES ) ),
pparent_window( pParent ),
pparent_object( GetpParent()->GetpCompartment() ),
data_altered( FALSE ),
current_data_altered( FALSE )
{
     FreeResource( );

     InitializeDialog( );

     // set up links

     // for checkbox

     show_reactive_species_checkbox.ChangeClickHdl(
          LINK( this, SpeciesConcDialog, CheckBoxHandler ) );

     // for entry field modification 

     conc_entryfield.ChangeModifyHdl( 
      LINK( this, SpeciesConcDialog, TextModifiedHandler ) ); 

     // for listbox

     species_name_listbox.ChangeSelectHdl( LINK( this, SpeciesConcDialog, ListboxSelectHandler ) );

     // for pushbuttons 

    close_btn.ChangeClickHdl( LINK( this, SpeciesConcDialog, CloseHandler ) );
    undo_btn.ChangeClickHdl( LINK( this, SpeciesConcDialog, UndoHandler ) );
    defaults_btn.ChangeClickHdl( LINK( this, SpeciesConcDialog, DefaultsHandler ) );
    
    GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                   SpeciesConcDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::CloseHandler( OKButton * )
{
     ReadEntryField( );
     GetpParentObject()->species_list.RemoveNonreactiveSpeciesWithZeroConc( );

     EndDialog( data_altered ? EC_RC_ACCEPT : EC_RC_CANCEL );
}





// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::ListboxSelectHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::ListboxSelectHandler( ListBox* )
{
     ReadEntryField( );
     UpdateEntryField();
}


// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::UndoHandler( PushButton* )
{
     UpdateEntryField();
}


// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::DefaultsHandler( PushButton* )
{
     conc_entryfield.SetText( ResId( vID_STR_REAL_ZERO) );
     conc_entryfield.Enable();

     undo_btn.Enable();

     current_data_altered = TRUE;
}






// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::ReadEntryField( )
// ............................................................................
// 
//  returns FALSE if a user input error is detected, TRUE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 SpeciesConcDialog::ReadEntryField( ) 
{
     // don't bother with this if the data has not been altered

     if ( !current_data_altered )
          return TRUE;

     // get the user entry 

     NumericString conc_entry = conc_entryfield.GetText();

     // is the entry in valid numeric format with correct range ?

     if ( conc_entry.IsValidNonNegativeFLOAT64() )
     {
          // if so, then decide what to do with this value

          // first check to see if current species name is in the 
          // species list 

          String current_species = species_name_text.GetText();

          if ( !GetpParentObject()->species_list.NameIsInList( current_species ) )
          {
               // the name is not in the list - add it if the current conc_entry is
               // non-zero

               if ( 0.0 != (FLOAT64) conc_entry )
               {
                    GetpParentObject()->species_list.AddNonreactiveSpecies( current_species );
               }
               else
               {
                    // the entry is 0.0 - no need to save it explicitly

                    return TRUE;
               }
          }

          // now get the existing data for this species in the list, 
          // update it with the newly entered concentration , and replace it 

          SpeciesListEntry list_entry = 
           GetpParentObject()->species_list.GetSpeciesDataByName( current_species );

          list_entry.SetInitialConcentration( conc_entry );

          GetpParentObject()->species_list.SetSpeciesDataByName( current_species, list_entry );

          data_altered = TRUE;
          return TRUE;
     }
     else
     {
          // entry field text is not a valid numeric value -- tell user 
          String message = String( ResId( vID_STR_CONC_INPUT_ERR1 ) ) + 
           conc_entry + 
           String( ResId( vID_STR_CONC_INPUT_ERR2 ) );

          WarningBox box( this, WB_OK | WB_DEF_OK, message );
          box.Execute( );

          return FALSE;
      }
}




// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::CheckBoxHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::CheckBoxHandler( CheckBox* )
{
     if ( ReadEntryField( ) )
     {
          InitializeListBox( );
          UpdateEntryField( );
     }
}




// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::TextModifiedHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::TextModifiedHandler( Edit* )
{
     if ( !current_data_altered )
     {
          current_data_altered = TRUE;
          undo_btn.Enable();
     }
}




// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::InitializeDialog( )
{
     units_text.SetText( GetpParentObject()->GetpParent()->GetScheme().GetConcentrationUnits() );

     InitializeListBox( );
     UpdateEntryField( );
}




// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::UpdateEntryField()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::UpdateEntryField( )
{

     if ( species_name_listbox.GetSelectEntryCount () > 0 )
     {
          String selected_entry = species_name_listbox.GetSelectEntry();

          species_name_text.SetText( selected_entry);

          // is the currently selected species in the compartment species
          // list? if so then it has a non-zero concentration or it is
          // used in the reaction steps within this compartment 

          if ( GetpParentObject()->species_list.NameIsInList( selected_entry ) )
          {
               const SpeciesListEntry& r_entry = GetpParentObject()->species_list.GetSpeciesDataByName( selected_entry );

               conc_entryfield.SetText( r_entry.GetInitialConcentration() );
          }
          else
          {
               // we come here if species is not already in species list. It 
               // must therefore be a non-reactive species in the SpeciesDatabase
               // with an implicit initial concentration = 0.0
 
               conc_entryfield.SetText( ResId( vID_STR_REAL_ZERO) );
          }

          conc_entryfield.Enable();
          defaults_btn.Enable();

     }
     else
     {
          species_name_text.SetText( ResId( vID_STR_NO_SELECTION ) );
          conc_entryfield.SetText( ResId( vID_STR_REAL_ZERO) );
          conc_entryfield.Disable();
          defaults_btn.Disable();
     }

     undo_btn.Disable();

     current_data_altered = FALSE;
}




// ----------------------------------------------------------------------------
//                 SpeciesConcDialog::InitializeListBox()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpeciesConcDialog::InitializeListBox( )
{
     UINT16  number_added = 0;

     species_name_listbox.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     species_name_listbox.Clear( );


     if ( show_reactive_species_checkbox.IsChecked() ) 
     {
          // initialize from CompartmentObject SpeciesList

          UINT16 num_species = GetpParentObject()->species_list.GetNumberOfSpecies( );

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               const SpeciesListEntry& r_entry = GetpParentObject()->species_list.GetSpeciesDataByPosition( i );

               if ( r_entry.IsReactive() )
               {
                    species_name_listbox.InsertEntry( r_entry.GetName( ) );
                    number_added++;
               }
          }
     }
     else
     {
          // initialize from Reaction Scheme SpeciesDatabase

          // first get the Speceis database so we can retrieve the species names 

          SchemeWindow* p_scheme_win = GetpParentObject()->GetpParent( );

          SpeciesDatabase* p_db = p_scheme_win->GetScheme().GetpSpeciesDatabase();

          UINT16 num_species = p_db->GetNumberOfSpecies( );

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               const SpeciesDatabaseEntry& r_entry = p_db->GetSpeciesDataByPosition( i );
               species_name_listbox.InsertEntry( r_entry.GetName( ) );
               number_added++;
          }
     }

     if (number_added > 0 )
          species_name_listbox.SelectEntryPos( 0 );

     species_name_listbox.ChangeUpdateMode( TRUE );
     species_name_listbox.Update( );
}




// ----------------------------------------------------------------------------
//                 EditSpeciesConcDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum EDIT_CONC_RC EditSpeciesConcDialog( CompartmentDialog* pParent)
{
     enum EDIT_CONC_RC rc;

     SpeciesConcDialog* pdlg = new SpeciesConcDialog( pParent);

     if ( pdlg )
     {
          rc = (EDIT_CONC_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return EC_RC_FAIL;
     }
}


