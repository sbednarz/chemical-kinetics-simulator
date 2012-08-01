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
//  XOBJDLG.HXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the EditTransferObjectDialog class
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

#include "xobjdlg.hxx"
#include "schemwin.hxx"
#include "mainapp.hxx"
#include "comptobj.hxx"
#include "editmxfr.hxx"

#endif


#include "xobjdlg.hrc"
#include "strings.hrc"




class TransferObjectEditDialog : public ModalDialog
{
protected:

     FixedText            name_label;
     FixedText            source_label;
     FixedText            target_label;
     FixedText            source_name;
     FixedText            target_name;
 
     Edit                 name_entry_field;
     GroupBox             group_box1;
 
     ListBox              xfer_step_listbox;
     PushButton           add_vdiffn_pb;
     PushButton           add_sdiffn_pb;
     PushButton           add_mass_xfer_pb;
     PushButton           edit_step_pb;
     PushButton           delete_step_pb;
 
     GroupBox             group_box2;
 
     OKButton             ok_btn;
     HelpButton           help_btn;
 
     TransferObject*      object;
     BOOL8                data_altered;
 
     void                 InitializeDialog( );
     TransferObject*      GetpObject( ) { return object; }
 
     void                 InitializeListBox( );
     void                 UpdatePushButtonActivations();
 
     void                 EditSelectedStep( );
     void                 ConstructStepDescription( const TransferStep& rStep, String& rTarget );

public:

     TransferObjectEditDialog( TransferObject* pObject );

     void CloseHandler( OKButton * );
     void ListboxSelectHandler( ListBox* );
     void ListboxDoubleClickHandler( ListBox* );

     void AddMassTransferStepHandler( PushButton* );
     void AddStdDiffusionStepHandler( PushButton* );
     void AddVirtualDiffusionStepHandler( PushButton* );
     void EditStepHandler( PushButton* );
     void DeleteStepHandler( PushButton* );
     void UndoHandler( PushButton* );
};


TransferObjectEditDialog::TransferObjectEditDialog( TransferObject* pObject) :
ModalDialog( pObject->GetpParent(), ResId( vID_DLG_XFERPATH_EDIT ) ),
name_label( this, ResId( vID_STEXT_NAME_LABEL ) ), 
source_label( this, ResId( vID_STEXT_SOURCE_LABEL ) ), 
target_label( this, ResId( vID_STEXT_TARGET_LABEL ) ), 
source_name( this, ResId( vID_STEXT_SOURCE_NAME ) ), 
target_name( this, ResId( vID_STEXT_TARGET_NAME ) ), 
name_entry_field( this, ResId( vID_EF_NAME ) ), 
group_box1( this, ResId( vID_GB_1 ) ), 
xfer_step_listbox( this, ResId( vID_LB_XFER_STEPS ) ), 
add_vdiffn_pb( this, ResId( vID_PB_ADD_VDIFFN_STEP ) ),
add_sdiffn_pb( this, ResId( vID_PB_ADD_SDIFFN_STEP ) ),
add_mass_xfer_pb( this, ResId( vID_PB_ADD_MASS_XFER_STEP ) ),
edit_step_pb( this, ResId( vID_PB_EDIT_XFER_STEP ) ),
delete_step_pb( this, ResId( vID_PB_DELETE_XFER_STEP ) ),
group_box2( this, ResId( vID_GB_PATH_CONTENTS ) ), 
ok_btn( this, ResId( vID_BTN_CLOSE ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ),
object( pObject ),
data_altered( FALSE )
{
    FreeResource( );

    InitializeDialog( );

    ok_btn.ChangeClickHdl( LINK( this, TransferObjectEditDialog, CloseHandler ) );

    add_vdiffn_pb.ChangeClickHdl( LINK( this, TransferObjectEditDialog, AddVirtualDiffusionStepHandler ) );
    add_sdiffn_pb.ChangeClickHdl( LINK( this, TransferObjectEditDialog, AddStdDiffusionStepHandler ) );
    add_mass_xfer_pb.ChangeClickHdl( LINK( this, TransferObjectEditDialog, AddMassTransferStepHandler ) );

    edit_step_pb.ChangeClickHdl( LINK( this, TransferObjectEditDialog, EditStepHandler ) );
    delete_step_pb.ChangeClickHdl( LINK( this, TransferObjectEditDialog, DeleteStepHandler ) );

    xfer_step_listbox.ChangeSelectHdl( LINK( this, TransferObjectEditDialog, ListboxSelectHandler ) );
    xfer_step_listbox.ChangeDoubleClickHdl( LINK( this, TransferObjectEditDialog, ListboxDoubleClickHandler ) );

    GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}







// ----------------------------------------------------------------------------
//                  TransferObjectEditDialog::CloseHandler( )
// ............................................................................
// 
//   this function is called when the Close button is pressed. Close means 
//   that the data in the dialog is to be accepted, so we save the appropriate
//   user entries.
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::CloseHandler( OKButton * )
{
     GetpObject()->SetName( name_entry_field.GetText( ) );

     GetpObject()->SetTransferListboxSelection( 
      xfer_step_listbox.GetSelectEntryPos() );
          
     if ( data_altered )
          EndDialog( EXO_RC_CLOSE );
     else
          EndDialog( EXO_RC_CANCEL );
}








// ----------------------------------------------------------------------------
//                  TransferObjectEditDialog::ListboxSelectHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::ListboxSelectHandler( ListBox* )
{
     UpdatePushButtonActivations();
}






// ----------------------------------------------------------------------------
//             TransferObjectEditDialog::UpdatePushButtonActivations()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::UpdatePushButtonActivations()
{
     if ( xfer_step_listbox.GetSelectEntryCount() > 0 )
     {
         edit_step_pb.Enable();
         delete_step_pb.Enable();
     }
     else
     {
         edit_step_pb.Disable();
         delete_step_pb.Disable();
     }

     
}







// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::ListboxDoubleClickHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::ListboxDoubleClickHandler( ListBox* )
{
     EditSelectedStep();
}






// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::AddVirtualDiffusionStepHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::AddVirtualDiffusionStepHandler( PushButton* )
{
     TransferStep* p_step = new TransferStep( XS_TYPE_VIRTUAL_DIFFUSION );

     if ( p_step )
     {
         enum EDIT_VDIF_RC rc = EditVirtualDiffusionStep( object, p_step );

         // if data was entered/altered then add to step list 

         if ( EVD_RC_CLOSE  == rc )
         {
              if ( FALSE == GetpObject()->AddNewTransferStep( *p_step ) )
              {
                   return;
              }

              // now add it to the "transfer step listbox ", and select it
              String workstr;

              ConstructStepDescription( *p_step, workstr );
         
              xfer_step_listbox.InsertEntry( workstr, xfer_step_listbox.GetSelectEntryPos() );
              xfer_step_listbox.SelectEntry( workstr );
              UpdatePushButtonActivations();
              data_altered = TRUE;
         }
     }
     else
     {
          GetpMainApp()->OutOfMemory();
     }
}




// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::AddStdDiffusionStepHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::AddStdDiffusionStepHandler( PushButton* )
{
     TransferStep* p_step = new TransferStep( XS_TYPE_STD_DIFFUSION );

     if ( p_step )
     {
         enum EDIT_SDIF_RC rc = EditStandardDiffusionStep( object, p_step );

         // if data was entered/altered then add to step list 

         if ( ESD_RC_CLOSE  == rc )
         {
              if ( FALSE == GetpObject()->AddNewTransferStep( *p_step ) )
              {
                   return;
              }

              // now add it to the "transfer step listbox ", and select it
              String workstr;

              ConstructStepDescription( *p_step, workstr );
         
              xfer_step_listbox.InsertEntry( workstr, xfer_step_listbox.GetSelectEntryPos() );
              xfer_step_listbox.SelectEntry( workstr );
              UpdatePushButtonActivations();
              data_altered = TRUE;
         }
     }
     else
     {
          GetpMainApp()->OutOfMemory();
     }
}





// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::AddMassTransferStepHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::AddMassTransferStepHandler( PushButton* )
{
     TransferStep* p_step = new TransferStep( XS_TYPE_MASS_XFER );

     if ( p_step )
     {
         enum EDIT_MXFR_RC rc = EditMassTransferStep( object, p_step );

         // if data was entered/altered then add to step list 

         if ( EMX_RC_CLOSE  == rc )
         {
              if ( FALSE == GetpObject()->AddNewTransferStep( *p_step ) )
              {
                   return;
              }

              // now add it to the "transfer step listbox ", and select it
              String workstr;

              ConstructStepDescription( *p_step, workstr );
         
              xfer_step_listbox.InsertEntry( workstr, xfer_step_listbox.GetSelectEntryPos() );
              xfer_step_listbox.SelectEntry( workstr );
              UpdatePushButtonActivations();
              data_altered = TRUE;
         }
     }
     else
     {
          GetpMainApp()->OutOfMemory();
     }
}










// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::EditSelectedStep( )
// ............................................................................
// 
//  simply figures out the position of the selection in the transfer step 
//  listbox, sotres it in the parent TransferObject, and invokes the 
//  EditTransferStepsDialog
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::EditSelectedStep( )
{
     // make a copy of the selected step 
     BOOL8 changed = FALSE;

     UINT16 lb_position = xfer_step_listbox.GetSelectEntryPos();

     GetpObject()->SetTransferListboxSelection( lb_position );

     TransferStep step( GetpObject()->GetTransferStep( lb_position ) );

     switch ( step.GetType() )
     {
          case XS_TYPE_MASS_XFER :
     
              changed = (EMX_RC_CLOSE == EditMassTransferStep( object, &step ) );
              break;
          


          case XS_TYPE_STD_DIFFUSION :

              changed = (ESD_RC_CLOSE == EditStandardDiffusionStep( object, &step ) );
              break;



          case XS_TYPE_VIRTUAL_DIFFUSION :

              changed = (EVD_RC_CLOSE == EditVirtualDiffusionStep( object, &step ) );
              break;


          case XS_TYPE_NONE : 
          default : 

               ASSERT( 0 );
               break;
     }

     if ( TRUE == changed )
     {
          // now replace the existing step in the "transfer step listbox ", and select it

          String workstr;

          ConstructStepDescription( step, workstr );
     
          BOOL position_selected = xfer_step_listbox.IsEntryPosSelected( lb_position );

          xfer_step_listbox.ChangeUpdateMode( FALSE ); 
          xfer_step_listbox.RemoveEntry( lb_position );

          xfer_step_listbox.InsertEntry( workstr, lb_position );

          if ( position_selected )
               xfer_step_listbox.SelectEntryPos( lb_position );

          xfer_step_listbox.ChangeUpdateMode( TRUE );

          UpdatePushButtonActivations();

          GetpObject()->SetTransferStep( lb_position, step );

          data_altered = TRUE;
     }

}







// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::EditStepHandler()
// ............................................................................
// 
//  
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::EditStepHandler( PushButton* )
{
     EditSelectedStep();
}







// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::DeleteStepHandler( )
// ............................................................................
// 
//  this fcn is called whenever the delete step button is pressed. We figure out
//  which item is selected in the list using its position; since both the listbox
//  and the transfer_step_list are maintained in alphabetical order the positions 
//  correspond. We delete the name for the xfer_step_listbox, 
//  and delete the correspnonding TransferStep from the transfer_step_list
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::DeleteStepHandler( PushButton* )
{
     // get info on selected species 

     UINT16 position = xfer_step_listbox.GetSelectEntryPos( );
     String selected_name = xfer_step_listbox.GetSelectEntry();

     // delete it from listbox and internal transfer step list 

     xfer_step_listbox.RemoveEntry( position );

     if ( position > 0)
     {
          xfer_step_listbox.SelectEntryPos( position - 1 );
     }
     else
     {
          if (xfer_step_listbox.GetEntryCount() > 0)
               xfer_step_listbox.SelectEntryPos( 0 );
     }

     GetpObject()->DeleteTransferStep( position );

     UpdatePushButtonActivations();
     data_altered = TRUE;

}








// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::ConstructStepDescription( )
// ............................................................................
// 
//  This function initializes the listbox, using the list of species in the 
//  transfer step list in the transfer object. The previously saved line position
//  is selected line upon initialization
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::ConstructStepDescription( const TransferStep& rStep, String& rTarget )
{
               
               rTarget = String( ResId( vID_STR_XFER_DESCRIBE_HEAD ) ) + rStep.GetSpeciesName();

               switch ( rStep.GetType() )
               {
                    case XS_TYPE_NONE :
                    default: 
               
                         ASSERT( 0 );
                         break;
               
               
                    case XS_TYPE_MASS_XFER :
               
                         rTarget += String( ResId( vID_STR_MASS_XFER_DESCRIBE ) );
                         break;
               
               
                    case XS_TYPE_STD_DIFFUSION :
               
                         rTarget += String( ResId( vID_STR_STD_DIFFN_DESCRIBE ) );
                         break;
               
               
                    case XS_TYPE_VIRTUAL_DIFFUSION :

                         rTarget = rTarget + String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                   rStep.GetSpeciesName2() + String( ResId( vID_STR_VIRT_DIFFN_DESCRIBE ) );
                         break;
               }


               switch ( rStep.GetTransferDirection() )
               {
                    case SOURCE_TO_TARGET :
               
                         rTarget += String( ResId( vID_STR_DIR_SRC_TO_TRGT ) );
                         break;

                    case REVERSIBLE :
               
                         rTarget += String( ResId( vID_STR_DIR_REVERSIBLE ) );
                         break;

               
                    case TARGET_TO_SOURCE :
               
                         rTarget += String( ResId( vID_STR_DIR_TRGT_TO_SRC ) );
                         break;
                }

}







// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::InitializeListBox( )
// ............................................................................
// 
//  This function initializes the listbox, using the list of species in the 
//  transfer step list in the transfer object. The previously saved line position
//  is selected line upon initialization
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::InitializeListBox( )
{
     String workstr;

     xfer_step_listbox.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     xfer_step_listbox.Clear( );

     const UINT16 num_steps = GetpObject( )->GetNumberOfTransferSteps( );

     if ( num_steps > 0 )
     {
          // fill the listbox

          for ( USHORT i = 0; i < num_steps; i++ )
          {
               ConstructStepDescription( GetpObject()->GetTransferStep( i ), workstr );
               xfer_step_listbox.InsertEntry( workstr );
          }

          // select the line number as previously saved 

          xfer_step_listbox.SelectEntryPos( GetpObject()->GetTransferListboxSelection( ) );
     }

     xfer_step_listbox.ChangeUpdateMode( TRUE );
     xfer_step_listbox.Update( );

     UpdatePushButtonActivations();
}








// ----------------------------------------------------------------------------
//        TransferObjectEditDialog::InitializeDialog( )
// ............................................................................
// 
//  general initialization of dialog items 
//
// ----------------------------------------------------------------------------

void TransferObjectEditDialog::InitializeDialog( )
{
     // set some text/entry fields 

     name_entry_field.SetText( GetpObject()->GetName() );
     source_name.SetText( GetpObject()->GetpSourceCompartment()->GetName() );
     target_name.SetText( GetpObject()->GetpTargetCompartment()->GetName() );


     // disable all add buttons if no species are defined 

     if ( 0 == GetpObject()->GetpParent()->GetScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() )
     {
          add_vdiffn_pb.Disable(); 
          add_sdiffn_pb.Disable(); 
          add_mass_xfer_pb.Disable(); 
     }
     else
     {
          switch ( GetpObject()->GetpParent()->GetSchemeType() )
          {
               case SW_TYPE_UNKNOWN :  
               default :
                    ASSERT( 0 );
                    break;
     
     
               case SW_TYPE_COMPARTMENT :
     
                    add_vdiffn_pb.Disable(); 
                    add_sdiffn_pb.Disable(); 
     
                    break;
     
     
               case SW_TYPE_3D  :
     
                   add_mass_xfer_pb.Disable(); 
     
                    break;
          }
     }

     InitializeListBox();
}



// ----------------------------------------------------------------------------
//                                  EditTransferObject( )
// ............................................................................
// 
//  global function invoked to edit the contents of a TransferObject 
//
// ----------------------------------------------------------------------------

enum EDIT_XOBJ_RC EditTransferObject( TransferObject* pObject)
{
     enum EDIT_XOBJ_RC rc;

     TransferObjectEditDialog* pdlg = new TransferObjectEditDialog( pObject);

     if ( pdlg )
     {
          rc = (EDIT_XOBJ_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return EXO_RC_FAIL;
     }
}


