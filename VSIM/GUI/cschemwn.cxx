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
//  CSCHEMWN.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This module defines the CompartmentSchemeWindow class, an MDI window 
//  containing a view of a reaction scheme. This is the base class for the 
//  various varieties of SchemeWindows
//
//  CREATED : 4.29.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "cschemwn.hxx"

#endif



#include "strings.hrc"
#include "popup.hrc"


CompartmentSchemeWindow::CompartmentSchemeWindow( Window* pParent ) :
SchemeWindow( pParent ) 
{ 
  GetScheme().SetSimulatorName( String( ResId( vID_STR_ENGINE_FILENAME ) ) );
}



// ----------------------------------------------------------------------------
//                 CompartmentSchemeWindow::GetMoveObject()
// ............................................................................
//
//  
// ----------------------------------------------------------------------------

ProcessObject*  CompartmentSchemeWindow::GetMoveObject () 
{ 
     ProcessObjectList& list = GetMoveObjectList();

     return ( 0 == list.Count() ) ? NULL : list.GetObject( 0 );
}





// ----------------------------------------------------------------------------
//                CompartmentSchemeWindow::SetMoveObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::SetMoveObject ( ProcessObject* pPO )
{
     // if there is a curren move object, remove from list

     ProcessObjectList& list = GetMoveObjectList();

     if ( 0 < list.Count() )
     {
          ASSERT ( list.Count() == 1 );

          list.Clear();
     }

     // if new selection  != NULL then save ptr 

     if ( pPO )
          list.Insert( pPO, LIST_APPEND );
}











// ----------------------------------------------------------------------------
//                     CompartmentSchemeWindow::PasteObject()
// ............................................................................
//
//  pastes a copy of the object currently on the clipboard into the CompartmentSchemeWindow
//  returns TRUE if successful, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 CompartmentSchemeWindow::PasteObject( const ProcessObject* const pClipObject )
{
     BOOL8    success = TRUE;

     switch ( pClipObject->GetType() )
     {
          case PO_TYPE_COMPT:
          {
               // construct a copy of object on clipboard

               CompartmentObject* pco = new CompartmentObject( *( (CompartmentObject*) pClipObject) );

               if ( pco )
               {

                    pco->SetName( GetScheme().GetUniqueCompartmentName() );
                    pco->SetpParent( this ); // change parent to this CompartmentSchemeWindow
                    pco->SetpOutputDevice( this );

                    // add to list of compartments in scheme window

                    GetScheme().AddCompartmentObject( pco );

                    // now update species database to include new species data

                    // first reconstruct species database so that new species are included

                    ReconstructSpeciesDatabase( );

                    // we are now going to set the species data in the species database
                    // to have values that were in effect for the compartment now being pasted
                    // that were in effect when it was cut/copied  from the original  CompartmentSchemeWindow

                    // a typecast for readability

                    ClippedCompartmentObject* p_clipped_compt = ( ClippedCompartmentObject* ) pClipObject;

                    // now go through the  list of species database entries in the
                    // clipped object and update the species databse in the new parent
                    // scheme window to the original values

                    UINT16 num_species_to_paste = p_clipped_compt->GetNumberOfSpeciesToPaste();

                    for ( UINT16 i = 0; i < num_species_to_paste ; i++ )
                    {
                         const SpeciesDatabaseEntry& r_entry = p_clipped_compt->GetSpeciesData( i );
                         GetScheme().GetpSpeciesDatabase()->SetSpeciesDataByName( r_entry.GetName(), r_entry );
                    }


                    GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( ResId( vID_STR_SCHEMWIN_ADD_COMPT ) );
                    SetOperation( SW_CMPTOBJ_ADD );

                    // set previous_rect to empty rectangle as indicator that there is no valid previous rect

                    previous_rect = Rectangle();

                    current_rect  = pco->GetActiveArea( );
                    SetMoveObject( pco );
                    ActivateDragAttributes( );
               }
               else
               {
                    GetpMainApp()->OutOfMemory();
                    success = FALSE;
               }
               break;
          }

          case PO_TYPE_XFER:
          {
               TransferObject* pto = new TransferObject( *((TransferObject*) pClipObject) );

               if ( pto )
               {
                    // change parent to this CompartmentSchemeWindow

                    pto->SetName( GetScheme().GetUniqueTransferObjectName( ) );
                    pto->SetpParent( this );
                    pto->SetpOutputDevice( this );

                    // now delete any transfer step for species which do not
                    // appear in the species database of the new parent CompartmentSchemeWindow
                    // we work from the end of the list toward the front since
                    // the position index changes for all remaining steps
                    // if we delete a step

                    UINT16 xfer_step_num = pto->GetNumberOfTransferSteps();

                    // delete any TransferSteps for species which are not now in the
                    // CompartmentSchemeWindow's Species Database

                    while ( xfer_step_num-- )
                    {
                       const String& rname = pto->GetTransferStep( xfer_step_num ).GetSpeciesName( );

                       if ( !GetScheme().GetpSpeciesDatabase()->ContainsEntryFor( rname ) )
                            pto->DeleteTransferStep( xfer_step_num );
                    }

                                        GetScheme().AddTransferObject( pto );
                    GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( ResId( vID_STR_SCHEMWIN_ADD_XFEROBJ1 ) );
                    SetOperation( SW_XFEROBJ_ADD );
                    SetMoveObject( pto );

                    ActivateDragAttributes( );
               }
               else
               {
                    GetpMainApp()->OutOfMemory();
                    success = FALSE;
               }
               break;
          }

          default :

               // abort right here if parameter is unknown

               ASSERT( 0 );
    }

    return success;
}












// ----------------------------------------------------------------------------
//                   CompartmentSchemeWindow::DeleteSelectedObject( )
// ............................................................................
//
//  if an object is currently highlighted in the scheme window then it is deleted
//   and the window is redrawn
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::DeleteSelectedObject( )
{
     ProcessObject* selection = GetSelectObject();
     if ( selection != NULL )
     {
          ChangeUpdateMode( FALSE );
          SetSelectObject( );

          switch ( selection->GetType() )
          {

          case PO_TYPE_COMPT:
          {
                // if we are deleting a compartment object then delete any
                // TransferObject links it has with other compartments also

                TransferObject* pto;
                CompartmentObject * pco = (CompartmentObject*) selection;
                pto = pco->GetFirstTransferObject( );

                while ( pto )
                {
                     // if there are TransferObjects attached to the compartment to
                     // be deleted then remove from Compartment's internal list and
                     // from Scheme Window

                     GetScheme().RemoveTransferObject( pto );

                     // DeleteTransferObject makes the next TransferObject in
                     // the list the first so we repetitively call
                     // GetFirstTransferObject until NULL is returned

                     pco->DeleteTransferObject( pto );

				 delete pto;					

                     pto = pco->GetFirstTransferObject( );
                }

               GetScheme().RemoveCompartmentObject( pco );

               ReconstructSpeciesDatabase( );
          }
               break;

          case PO_TYPE_XFER :

               GetScheme().RemoveTransferObject( (TransferObject*) selection );
               break;
          }


          delete selection;
          RecalcTotalSize();

          GetpFrame()->RecalcWindowLayout();
          GetpFrame()->UpdateToolbarActivations( );
          Invalidate( );

          ChangeUpdateMode( TRUE );
          Update();
          GetScheme().SetStatus( SIM_DATA_MODIFIED );
     }

     GetpFrame()->UpdateStatusBar();
}

















// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::StartAddCompartment( )
// ............................................................................
//
//  called by ToolBar - starts the process of adding a new Compartment object
//  both to the drawing on screen and to the reaction scheme. The addition
//  process is ended by function  CompartmentSchemeWindow::EndMoveCompartment( );
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::StartAddCompartment( )
{
     CompartmentObject* pco = new CompartmentObject( GetScheme().GetUniqueCompartmentName(), this );

     if ( pco )
     {
          GetScheme().AddCompartmentObject( pco );
          GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( ResId( vID_STR_SCHEMWIN_ADD_COMPT ) );
          SetOperation( SW_CMPTOBJ_ADD );

          // set to empty rectangle as indicator that there is no valid previous rect

          previous_rect = Rectangle();

          current_rect  = pco->GetActiveArea( );
          SetMoveObject( pco );
          ActivateDragAttributes( );
     }

}




// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::StartAddTransferObject( )
// ............................................................................
//
//  called by ToolBar - starts the process of adding a new TransferObject
//  both to the drawing on screen and to the reaction scheme. The addition
//  process is ended by function  CompartmentSchemeWindow::EndAddTransferObject()
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::StartAddTransferObject( )
{
     TransferObject* pto = new TransferObject( GetScheme().GetUniqueTransferObjectName(), this );

     if ( pto )
     {
          GetScheme().AddTransferObject( pto );
          GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( ResId( vID_STR_SCHEMWIN_ADD_XFEROBJ1 ) );
          SetOperation( SW_XFEROBJ_ADD );
          SetMoveObject( pto );

          ActivateDragAttributes( );
     }
}
















// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::MouseMove( )
// ............................................................................
//
//  handler - called whenever the mouse is moved
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::MouseMove( const MouseEvent& rMEvt )
{
     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( position );

     // get the current operation so we know how to handle the current event

     switch ( GetOperation( ) )
     {

     case SW_NO_OP :

          if ( ( ( rMEvt.IsLeft( ) && rMEvt.IsShift() ) || rMEvt.IsRight() ) &&
               ( current_object != NULL ) &&
               ( current_object->GetType()  == PO_TYPE_COMPT )
          )
          {
               // if the left button (MB1) is down and we are over a
               // Compartment object then we are starting a
               // Compartment Move operation

               current_rect = current_object->GetActiveArea();
               SetMoveObject( current_object );
               drag_offset = position - current_rect.TopLeft();
               SetOperation( SW_COMPOBJ_MOVE );
               ActivateDragAttributes( );

               previous_rect = Rectangle();   // create empty rectangle  as signal
          }
          else
          {

              // we come here if it is just a plain mouse move /w no  buttons pressed

              // test to see if we above an object
              // if so then change highlight if necessary

              ProcessObject* highlight_object = GetHighlightObject();

              if ( current_object != NULL )
              {
                   if ( current_object == highlight_object )  // still on the same object - do nothing
                        return;
                   else
                   {
                        // a new object - change its state
                        // first remove highlight if there already is one highlighted

                        if ( highlight_object )
                        {
                             SetHighlightObject( );
                             highlight_object->Highlight( FALSE );
                        }

                        SetHighlightObject( current_object );
                        current_object->Highlight( TRUE );

                   }
              }
              else
              {
                   // we are not above any object - if there is a highlighted
                   // object then we have just moved off it and should reset it
                   // to normal

                   if ( highlight_object )
                   {
                        highlight_object->Highlight( FALSE );
                        SetHighlightObject( );
                   }
              }
          }
          break;


     case SW_CMPTOBJ_ADD :

            // this is the first mouse move then previous_rect will be empty
            // and is handled differently than the rest of the moves

            if ( !previous_rect.IsEmpty( ) )
                 DrawRect( previous_rect );

            current_rect.ChangePos( position );
            DrawRect( current_rect );
            previous_rect = current_rect;

          break;


     case SW_XFEROBJ_ADD :

          {
               // in this case the move_object is of type TransfeObject
               // If the source compartment has been selected then
               // we draw a rubber band line between it and mouse pointer

               CompartmentObject* psource = ( (TransferObject*) GetMoveObject() )->GetpSourceCompartment();
               
               if ( psource )
               {
                    Point source_pt = ((TransferObject*) GetMoveObject() )->GetpSourceCompartment()->
                      GetActiveArea().Center();
                    DrawLine( source_pt, prev_position );
                    DrawLine( source_pt, position );
                    prev_position = position;
               }
          }
          break;


     case SW_COMPOBJ_MOVE :

            current_rect.ChangePos( position - drag_offset );

            if ( current_rect != previous_rect )
            {
                 if ( !previous_rect.IsEmpty( ) )
                      DrawRect( previous_rect );

                 DrawRect( current_rect );
                 previous_rect = current_rect;
            }

          break;
     }
}






// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::MouseButtonUp( )
// ............................................................................
//
//  handler - called whenever a MouseButton is released
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::MouseButtonUp( const MouseEvent& rMEvt )
{
     // we only deal here with release of MB1 to set the new location of
     // a Compartment object being moved

     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     if ( ( GetOperation() == SW_COMPOBJ_MOVE ) && ( rMEvt.IsLeft() || rMEvt.IsRight() ) )
     {

          Point tmp = position - drag_offset;  // EndMove Compartment needs lvalue .. make it explicit
          EndMoveCompartment( tmp );

          if ( !previous_rect.IsEmpty( ) )
          {
               // cause all linked transfer objects to adjust themselves

               ( (CompartmentObject*) GetMoveObject() )->RepositionTransferObjects( );

               GetpFrame()->RecalcWindowLayout();
               GetpFrame()->UpdateToolbarActivations( );
               Invalidate();
          }
     }
}




// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::MouseButtonDown( )
// ............................................................................
//
//  handler - called whenever a MousButton is pressed
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::MouseButtonDown( const MouseEvent& rMEvt )
{
     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     switch ( GetOperation( ) )
     {

     case SW_NO_OP :

           if (rMEvt.GetClicks() == 1)
                SelectObject( position );
           else
                EditObject( position );

           break;


      case SW_CMPTOBJ_ADD :

           EndMoveCompartment( position );

           ReconstructSpeciesDatabase( );

           break;


     case SW_XFEROBJ_ADD :

           EndAddTransferObject( position );

           break;


      default :

           ASSERT( 0 );   // this should not happen -- make it crash if it does !

           break;
     }
}





// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::EndMoveCompartment( )
// ............................................................................
//
//  this function is called when we are in the process of adding a Compartment
//  to the reaction scheme. This places the compartment where the mouse was
//  clicked.
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::EndMoveCompartment( Point& rPos)
{
     // remove the drag image

     if ( !previous_rect.IsEmpty( ) )
          DrawRect( previous_rect );

     // place the new object at the selected position

     current_rect.ChangePos( rPos );
     GetMoveObject()->SetActiveArea( current_rect);

     // recalculate the total size of the scheme, taking into account
     // the newly selected position

     RecalcTotalSize();
     GetpFrame()->RecalcWindowLayout();
     GetpFrame()->UpdateToolbarActivations( );     
     Invalidate();

     DrawRect( current_rect );

     // done - clean up and leave

     DeactivateDragAttributes( );

     GetMoveObject()->ChangeState( PO_NORMAL );
     GetMoveObject()->DrawObject( );
     SetOperation( SW_NO_OP );

     GetScheme().SetStatus( SIM_DATA_MODIFIED );
     GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( GetpMainApp()->GetMainWindow()->GetHelpText() );
     GetpFrame()->UpdateStatusBar();
}




// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::EndAddTransferObject( )
// ............................................................................
//
//  this function is called when we are in the process of adding a TransferObject
//  to the reaction scheme. Two compartments must be selected to complete the
//  addition. the action taken in this fcn depends on whether there is a
//  compartment under the mouse at the instant of the mouse click, whether
//  it is a first or second selection, and whether the second selected compartment
//  is identical to the first
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::EndAddTransferObject( Point& rPos )
{
     // In this case the move_object is of type TransferObject, so
     // first we do a typecast just to make things read more clearly below

     TransferObject* pto = (TransferObject*) GetMoveObject();

     // did we just click on a compartment ?

     ProcessObject* ppo = GetScheme().GetProcessObjectPtrFromLocation( rPos );

     if ( ( NULL != ppo ) && ( PO_TYPE_COMPT == ppo->GetType() ) )
     {
          // if so, then if we hav no source then make the current
          // compartment the source

          CompartmentObject* pco = (CompartmentObject*) ppo;

          if ( pto->GetpSourceCompartment( ) == NULL )
          {
               // we do not yet have a source so make current compartment
               // the Source

               pto->SetpSourceCompartment( pco );
               
               //              pto->SetSourcePoint( pco->GetActiveArea().Center() );
               DrawLine( pco->GetActiveArea().Center(), rPos );
               prev_position = rPos;

               // change the help message at the bottom
               GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( ResId( vID_STR_SCHEMWIN_ADD_XFEROBJ2 ) );
          }
          else
          {
               // we already have a source compartment so this one will be
               // the Target

               // if it is the same as the Source compartment then
               // do not accept

               if ( pto->GetpSourceCompartment() != pco )
               {
                     pto->SetpTargetCompartment( pco );
                     //                     pto->SetTargetPoint( pco->GetActiveArea().Center() );

                     // notify the source and target compartments that they
                     // are now connected by this TransferObject

                     pto->GetpSourceCompartment()->AddTransferObject( pto );
                     pto->GetpTargetCompartment()->AddTransferObject( pto );

                     // now get rid of rubber band line between source and here

                     DrawLine( pto->GetpSourceCompartment()->GetActiveArea().Center(), prev_position );

                     DeactivateDragAttributes( );

                     pto->Reposition( );
                     pto->ChangeState( PO_NORMAL );
                     pto->DrawObject( );
                     SetOperation( SW_NO_OP );
                     GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( GetpMainApp()->GetMainWindow()->GetHelpText() );

                     GetScheme().SetStatus( SIM_DATA_MODIFIED );
                     GetpFrame()->UpdateStatusBar();
                     GetpFrame()->RecalcWindowLayout();
                     GetpFrame()->UpdateToolbarActivations( );                     
                     Invalidate();
               }
               else
                    InfoBox( this, ResId( vID_POPUP_SOURCE_TARGET_COLLISION ) ).Execute( );

          }

     }

}






// ----------------------------------------------------------------------------
//                         CompartmentSchemeWindow::CancelAddTransferObject( )
// ............................................................................
//
//  this function is called when we are in the process of adding a TransferObject
//  to the reaction scheme and the user has pressed Escape to cancel the operation.
//  The action taken in this fcn depends on whether the Source compartment has been
//  selected yet
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::CancelAddTransferObject(  )
{
     // In this case the move_object is of type TransferObject, so
     // first we do a typecast just to make things read more clearly below

     TransferObject* pto = (TransferObject*) GetMoveObject();

     // if we have already selected a source compartment so there will
     // be a rubber band line  on the schematic that we need to delete

     if ( pto->GetpSourceCompartment( ) != NULL )
          DrawLine( pto->GetpSourceCompartment()->GetActiveArea().Center(), prev_position );

     GetScheme().RemoveTransferObject( pto );  // remove object from list

     delete pto;                   // get back the memory

     DeactivateDragAttributes( );
     SetOperation( SW_NO_OP );
     GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( GetpMainApp()->GetMainWindow()->GetHelpText() );
}





// ----------------------------------------------------------------------------
//                        CompartmentSchemeWindow::CancelAddCompartmentObject( )
// ............................................................................
//
//  this function is called when we are in the process of adding a compartment object
//  to the reaction scheme and the user has pressed Escape to cancel the operation.
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::CancelAddCompartmentObject( )
{
     // typecast to clarify things

     CompartmentObject* pto = (CompartmentObject*) GetMoveObject();

     // draw current drag image - XOR mode erase image

     DrawRect( current_rect );

     GetScheme().RemoveCompartmentObject( pto );

     delete pto;

     // done - clean up and leave

     DeactivateDragAttributes( );
     SetOperation( SW_NO_OP );
     GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( GetpMainApp()->GetMainWindow()->GetHelpText() );

     GetpMainApp()->GetMainWindow()->ActivateAddTransferObject( GetScheme().GetNumCompartments() > 1 );
}


// ----------------------------------------------------------------------------
//                       ThreeDArraySchemeWindow::Activate()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void CompartmentSchemeWindow::Activate()
{
     SchemeWindow::Activate();
     GetpMainApp()->GetMainWindow()->ActivateSelectAllTools( FALSE );
}




#if 0
void ReportMouseMode( String Locn, const MouseEvent& rEvt )
{
     String str ( Locn );
     str = str +  "\nPosPixel = ";
     str = str + String( rEvt.GetPosPixel().X() ) + "," + String(rEvt.GetPosPixel().Y() )
     + "\nMode = " + String( rEvt.GetMode() )
     + "\nClicks = " + String( rEvt.GetClicks() )
     + "\nButtons = " + String( rEvt.GetButtons() );

     InfoBox( NULL, str).Execute();
}
#endif



