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
//  COMPTDLG.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This modules defines the CompartmentObject Edit dialog used in the 
//  the VSIM user interface. 
//
//  CREATED : 1.23.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "procsobj.hxx"
#include "comptdlg.hxx"
#include "editrxn.hxx"
#include "mainapp.hxx"
#include "specconc.hxx"
#include "rxncondn.hxx"
#include "assert.hxx"

#endif


#include "strings.hrc"
#include "comptdlg.hrc"



// ----------------------------------------------------------------------------
//                  CompartmentDialog::CompartmentDialog( )
// ............................................................................
// 
//  constructor for  CompartmentDialog
//
// ----------------------------------------------------------------------------

CompartmentDialog::CompartmentDialog( Window * pParent, CompartmentObject* pCompartment )
: ModalDialog( pParent, ResId( vID_DLG_COMPT_EDIT ) ),
name_stext( this, ResId( vID_STEXT_LABEL ) ),
name( this, ResId( vID_EF_NAME ) ), 
dimension_stext( this, ResId( 2 ) ),
x_dimension( this, ResId( vID_STEXT_X_DIMENSION ) ),
y_dimension( this, ResId( vID_STEXT_Y_DIMENSION ) ),
z_dimension( this, ResId( vID_STEXT_Z_DIMENSION ) ),
attr_gb( this, ResId( vID_GB_ATTR ) ),  
compt_color_pb( this, ResId( vID_PB_COMPT_COLOR ) ),  
color_sample( this, ResId( vID_WIN_COLOR_SAMPLE ) ),
rxn_condn_pb( this, ResId( vID_PB_RXN_CONDNS ) ),  
edit_conc_pb( this, ResId( vID_PB_EDIT_CONCS ) ),  
add_step_pb( this, ResId( vID_PB_ADD_STEP ) ),  
edit_step_pb( this, ResId( vID_PB_EDIT_STEP ) ),  
delete_step_pb( this, ResId( vID_PB_DELETE_STEP ) ),  
rxn_step_lb( this, ResId( vID_LB_RXNSTEPS ) ),  
steps_groupbox( this, ResId( vID_GB_RXNSTEPS ) ),  
close_button( this, ResId( vID_BTN_CLOSE ) ),  
help_button( this, ResId( vID_BTN_HELP ) ),
pcompartment ( pCompartment ),
data_altered( FALSE ),
species_data_needs_refresh( FALSE ),
color( GetpCompartment()->GetObjectColor() )
{
     FreeResource( );
     name.SetText( GetpCompartment()->GetName( ) );
     color_sample.ChangeBackgroundBrush( Brush( color ) );
     color_sample.Invalidate();
     color_sample.Update();
 
     InitializeDimensions();
     InitializeListBox();
     UpdateActivation();
 
     // create links to pushbutton handlers
 
     close_button.ChangeClickHdl( LINK( this, CompartmentDialog, CloseHandler ) );
     add_step_pb.ChangeClickHdl( LINK( this, CompartmentDialog, AddRxnHandler) );
     edit_step_pb.ChangeClickHdl( LINK( this, CompartmentDialog, EditRxnHandler) );
     delete_step_pb.ChangeClickHdl( LINK( this, CompartmentDialog, DeleteRxnHandler) ); 
     edit_conc_pb.ChangeClickHdl( LINK( this, CompartmentDialog, EditSpeciesHandler ) ); 
     rxn_condn_pb.ChangeClickHdl( LINK( this, CompartmentDialog, EditRxnCondnsHandler ) );  

     compt_color_pb.ChangeClickHdl( LINK( this, CompartmentDialog, EditColorHandler ) );  
 
     name.ChangeModifyHdl( LINK( this, CompartmentDialog, SetModifiedFlagHandler ) );

     // create links to listbox handlers 

     rxn_step_lb.ChangeSelectHdl( LINK( this, CompartmentDialog, ListboxSelectHandler ) );
     rxn_step_lb.ChangeDoubleClickHdl( LINK( this, CompartmentDialog, ListboxDoubleClickHandler ) );

     // position window 

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}






// ----------------------------------------------------------------------------
//              CompartmentDialog::InitializeDimensions()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::InitializeDimensions()
{
     enum SCHEMWIN_TYPE scheme_type = GetpCompartment( )->GetpParent()->GetSchemeType();

     switch ( scheme_type )
     {
     case SW_TYPE_COMPARTMENT:

          dimension_stext.Hide();
          x_dimension.Hide(); 
          y_dimension.Hide(); 
          z_dimension.Hide(); 

          break;


     default:
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;


     case SW_TYPE_3D :
          {
               String length_units = String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
                                     GetpCompartment()->GetpParent()->GetScheme().GetDistanceUnits(); 
     
               x_dimension.SetText( 
                String( ResId( vID_STR_3D_STATX ) ) +
                NumericString( GetpCompartment()->GetXDimension()  ) +
                length_units );
          
               y_dimension.SetText( 
                String( ResId( vID_STR_3D_STATY ) ) +
                NumericString( GetpCompartment()->GetYDimension() ) +
                length_units );
          
          
               z_dimension.SetText( 
                String( ResId( vID_STR_3D_STATZ ) ) +
                NumericString( GetpCompartment()->GetZDimension() ) +
                length_units );
     
               break;
          }
     }

}






// ----------------------------------------------------------------------------
//              CompartmentDialog::SetModifiedFlagHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::SetModifiedFlagHandler( Edit* )
{
     data_altered = TRUE;
}




// ----------------------------------------------------------------------------
//                  CompartmentDialog::EditRxnCondnsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::UpdateActivation( )
{
     // ensure that both the compartment species lists and global species
     // databases are up-to-date before we continue


     if ( TRUE == species_data_needs_refresh )
     {
          StatusWindow statwin( this );

          statwin.SetStatusText( String( ResId( vID_STR_UPDATING_SPEC_DB ) ) );
          statwin.Show();
          statwin.Update();

          GetpCompartment()->UpdateSpeciesList();
          GetpCompartment()->GetpParent()->ReconstructSpeciesDatabase( );
          species_data_needs_refresh  = FALSE;
     }

     if ( 0 == GetpCompartment()->GetNumberOfRxnSteps() )
     {
          edit_step_pb.Disable();
          delete_step_pb.Disable(); 
     }
     else
     {
          edit_step_pb.Enable();
          delete_step_pb.Enable(); 
     }

     if ( GetpCompartment()->GetpParent()->GetScheme().GetpSpeciesDatabase()->IsEmpty() )
          edit_conc_pb.Disable();
      else
          edit_conc_pb.Enable( );
}






// ----------------------------------------------------------------------------
//                  CompartmentDialog::EditRxnCondnsHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::EditRxnCondnsHandler( PushButton * ) 
{

     enum SCHEMWIN_TYPE scheme_type = GetpCompartment( )->GetpParent()->GetSchemeType();

     switch ( scheme_type )
     {
     case SW_TYPE_COMPARTMENT:


     if ( RC_RC_ACCEPT == EditReactionConditions( this ) )
          data_altered = TRUE;

          break;


     default:
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;


     case SW_TYPE_3D :

     if ( RC_RC_ACCEPT == EditReactionConditions3D( this ) )
          data_altered = TRUE;

          break;
     }
}






// ----------------------------------------------------------------------------
//                  CompartmentDialog::EditColorHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::EditColorHandler( PushButton * ) 
{
     ColorDialog      dlg( this, WB_STDMODAL | WB_SVLOOK );

     dlg.ChangeColor( color );

     if ( TRUE == dlg.Execute() )
     {
          color = dlg.GetColor();

          color_sample.ChangeBackgroundBrush( Brush( color ) );
          color_sample.Invalidate();
          color_sample.Update();

          data_altered = TRUE;
     }
}




// ----------------------------------------------------------------------------
//                  CompartmentDialog::ListboxSelectHandler( )
// ............................................................................
// 
//   called whenever a line in the listbox is clicked, Simply updates the 
//   Compartment data to keep it up-to-date on user selection
//
// ----------------------------------------------------------------------------

void CompartmentDialog::ListboxSelectHandler( ListBox* )
{
     USHORT selected_position = rxn_step_lb.GetSelectEntryPos( );

     GetpCompartment()->SetRxnListboxSelection( selected_position );
}






// ----------------------------------------------------------------------------
//                  CompartmentDialog::ListboxDoubleClickHandler()
// ............................................................................
// 
//   called whenever a line in the listbox is double-clicked, which means 
//   to edit the selected reaction step.
//
// ----------------------------------------------------------------------------

void CompartmentDialog::ListboxDoubleClickHandler( ListBox* )
{
     if ( ER_RC_ACCEPT == EditReactionDialog( this ) )
     {
          data_altered = TRUE;
          species_data_needs_refresh = TRUE;
     }

     UpdateActivation();
}



// ----------------------------------------------------------------------------
//                  CompartmentDialog::InitializeListBox( )
// ............................................................................
// 
//  This function initializes the listbox, using the list of reactions in the 
//  Compartment. The compartment has recorded which line was previously
//  selected, and that line is the selected line upon initialization
//
// ----------------------------------------------------------------------------

void CompartmentDialog::InitializeListBox( )
{
     rxn_step_lb.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     rxn_step_lb.Clear( );

     const UINT16 num_steps = GetpCompartment( )->GetNumberOfRxnSteps();

     if ( num_steps > 0 )
     {
          // fill the listbox

          for ( USHORT i = 0; i < num_steps; i++ )
               rxn_step_lb.InsertEntry( GetpCompartment()->GetRxnStep( i ).GetEquation( ) );

          // select the line number as previously saved 

          rxn_step_lb.SelectEntryPos( GetpCompartment()->GetRxnListboxSelection( ) );
     }

     rxn_step_lb.ChangeUpdateMode( TRUE );
     rxn_step_lb.Update( );

}




// ----------------------------------------------------------------------------
//                  CompartmentDialog::ReplaceListboxEntry( )
// ............................................................................
// 
//  This function replaces the text of a specific litbox entry. the first entry 
//  is number 0. If the line in the listbox was previously selected then it 
//  is again selected upon replacing the text
//
// ----------------------------------------------------------------------------

void CompartmentDialog::ReplaceListboxEntry( UINT16 EntryNum,  const String rNewStr )
{
          BOOL position_selected = rxn_step_lb.IsEntryPosSelected( EntryNum );

          rxn_step_lb.ChangeUpdateMode( FALSE );

          rxn_step_lb.RemoveEntry( EntryNum );
          rxn_step_lb.InsertEntry( rNewStr, EntryNum );

          if ( position_selected )
               rxn_step_lb.SelectEntryPos( EntryNum );

          rxn_step_lb.ChangeUpdateMode( TRUE );
}






// ----------------------------------------------------------------------------
//                  CompartmentDialog::EditRxnHandler( )
// ............................................................................
// 
//   here we simply figure call the Edit Reaction Dialog. The Compartment
//   object already has the index of the selected reaction
//
// ----------------------------------------------------------------------------

void CompartmentDialog::EditRxnHandler( PushButton * )
{
     ASSERT ( 0 < rxn_step_lb.GetSelectEntryCount() );

     if ( ER_RC_ACCEPT == EditReactionDialog( this ) )
     {
          data_altered = TRUE;
          species_data_needs_refresh = TRUE;
     }

     UpdateActivation();
}




// ----------------------------------------------------------------------------
//                  CompartmentDialog::DeleteRxnHandler( )
// ............................................................................
// 
//   here we simply figure out which entry is selected utton is pressed
//   A new blank reaction is added to the listbox immediately after the 
//   currently selected line. The "Edit Reaction" dialog is then called 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::DeleteRxnHandler( PushButton * )
{
     ASSERT ( 0 < rxn_step_lb.GetSelectEntryCount() );

     USHORT selected_position = GetpCompartment()->GetRxnListboxSelection( );

     // get total number of entries

     USHORT num_entries = rxn_step_lb.GetEntryCount();

     ASSERT ( 0 != num_entries );

     GetpCompartment()->DeleteRxnStep( selected_position );
     rxn_step_lb.RemoveEntry( selected_position );

     if ( num_entries == 1 )
     {
          // the selected entry was the only one 

          rxn_step_lb.SetNoSelection( );
          GetpCompartment()->SetRxnListboxSelection( 0 ); 
     }
     else
     {
          // Was the selected entry the last entry in list? if so then delete it 
          // and highlight the line preceding the deleted entry. Otherwise 
          // highlight the line after the deleted entry

          if ( selected_position == ( num_entries-1 ) )
               selected_position--;

          rxn_step_lb.SelectEntryPos( selected_position );
          GetpCompartment()->SetRxnListboxSelection( selected_position ); 
     }

    data_altered = TRUE;
    species_data_needs_refresh = TRUE;
    UpdateActivation( );
}




// ----------------------------------------------------------------------------
//                  CompartmentDialog::AddRxnHandler( )
// ............................................................................
// 
//   this function is called when the Add Reaction pusshbutton is pressed
//   A new blank reaction is added to the listbox immediately after the 
//   currently selected line. The "Edit Reaction" dialog is then called 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::AddRxnHandler( PushButton * )
{
     // we need to figure out the position to add the new reaction step in the 
     // list box. It is entered in a corresponding place in the list of reactions
     // the following cases must be dealt with : 
     //
     //   1. there are not yet any reaction steps in the list - we add the first one
     //   2. is there are entries in the list then we insert the new entry 
     //      immediately after the selected entry in the list 

     // if there are no steps now in the listbox then add the first 

     USHORT selected_position;

     USHORT num_entries = rxn_step_lb.GetEntryCount();

     if ( num_entries > 0 )
     {
           selected_position = rxn_step_lb.GetSelectEntryPos( );
           selected_position++;
     }
      else
           selected_position = 0;

       

      if ( TRUE == GetpCompartment()->AddNewRxnStep( selected_position ) )
      {
           rxn_step_lb.InsertEntry( 
            GetpCompartment()->GetRxnStep( selected_position ).GetEquation(),
            selected_position );

           rxn_step_lb.SelectEntryPos( selected_position );
           GetpCompartment()->SetRxnListboxSelection( selected_position );

           EditReactionDialog( this );

           data_altered = TRUE;
           species_data_needs_refresh = TRUE;
      }

    UpdateActivation( );


}



// ----------------------------------------------------------------------------
//                  CompartmentDialog::CloseHandler( )
// ............................................................................
// 
//   this function is called when the Close button is pressed. Close means 
//   that the data in the dialog is to be accepted, so we save the appropriate
//   user entries.
//
// ----------------------------------------------------------------------------

void CompartmentDialog::CloseHandler( OKButton * )
{
     if ( data_altered )
     {
          pcompartment->SetName( name.GetText( ) );
          GetpCompartment()->ChangeObjectColor( color ); 
          EndDialog( PO_RC_ACCEPT );
     }
     else
          EndDialog(  PO_RC_CANCEL );
}





// ----------------------------------------------------------------------------
//                  CompartmentDialog::EditSpeciesHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDialog::EditSpeciesHandler( PushButton* )
{
     // ensure that both the compartment species lists and global species
     // databases are up-to-date before we continue

     if ( EC_RC_ACCEPT == EditSpeciesConcDialog( this ) )
          data_altered = TRUE;

}






// ----------------------------------------------------------------------------
//                  EditCompartment( )
// ............................................................................
// 
//   this global function creates a CompartmentDialog
//
// ----------------------------------------------------------------------------

ProcObjRC   EditCompartment( Window* pParent, CompartmentObject* pCompartment )
{
     CompartmentDialog* pdlg = new CompartmentDialog( pParent, pCompartment);

     if ( pdlg )
     {
           return (ProcObjRC) pdlg->Execute();
     }

     return PO_RC_FAIL;
}

