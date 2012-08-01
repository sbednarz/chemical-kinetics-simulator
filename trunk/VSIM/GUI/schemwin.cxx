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
//  SCHEMWIN.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the behavior for an MDI window containing a view
//  of a reaction scheme.
//
//  CREATED : 1.10.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "schemwin.hxx"
#include "mainapp.hxx"
#include "schemfrm.hxx"

#endif


#include "strings.hrc"
#include "popup.hrc"




const Color DRAG_PEN_COLOR( COL_WHITE );
const enum BrushStyle DRAG_BRUSH = BRUSH_NULL;






// ----------------------------------------------------------------------------
//                         SchemeWindow::SchemeWindow( )
// ............................................................................
//
//  default constructor for SchemeWindow
//
// ----------------------------------------------------------------------------

SchemeWindow::SchemeWindow( Window* pParent ) :
ScalableWindow( pParent ),
scheme( this ),
char_width( 0 ),
char_height( 0 ),
results_window_list(),
highlight_object_list(),
select_object_list(),
move_object_list(),
previous_rect(),
current_rect(),
drag_pen( DRAG_PEN_COLOR , 0  ),
drag_brush( DRAG_BRUSH ),
std_pen( GetPen( ) ),
std_brush( GetFillInBrush( ) ),
prev_position(),
drag_offset(),
original_ptr(),
current_operation( SW_NO_OP ),
in_edit_mode( FALSE )
{
    RecalcTotalSize();

     // change font so that fill color mathecs that for compartments

    Font font = GetFont();
    font.ChangeFillColor( Color( PO_NORMAL_BG_FILL_COLOR ) );
    ChangeFont( font );

    // establish layout units before any window scaling occurs...
    // used by Process Objects for their own sizing

    Size str_size = GetTextSize( "XXXXXXXXXX" );
    char_width = str_size.Width()/10;
    char_height = str_size.Height();
}






// ----------------------------------------------------------------------------
//                         SchemeWindow::~SchemeWindow( )
// ............................................................................
//
//  destructor for SchemeWindow
//  we walk through the lists of ProcessObjects and delete them
//
// ----------------------------------------------------------------------------

SchemeWindow::~SchemeWindow()
{
     // delete any ResultsWindows we may have created
     // these should be deleted before deleting p_results

     if ( results_window_list.First() )
     {
		// we have at least one window 

          delete results_window_list.GetCurObject();
     
          while ( results_window_list.Next() )
               delete results_window_list.GetCurObject();
     }
}






// ----------------------------------------------------------------------------
//                         SchemeWindow::Paint( )
// ............................................................................
//
//  defines the drawing procedure for SchemeWindow
//
// ----------------------------------------------------------------------------

void SchemeWindow::Paint( const Rectangle& rRect )
{
     // do we have any compartment defined?  if so draw all in the list

     GetScheme().DrawAllObjects( rRect );
}







// ----------------------------------------------------------------------------
//                   SchemeWindow::Resize( )
// ............................................................................
//
//  called whenever the SchemeWindow is resized - we recalc the window layout
//  and then invalidate the whole window
//
// ----------------------------------------------------------------------------

void SchemeWindow::Resize( )
{
     GetpFrame()->RecalcWindowLayout();
     GetpFrame()->UpdateToolbarActivations( );     
     Invalidate();
}






// ----------------------------------------------------------------------------
//                         SchemeWindow::MouseButtonDown( )
// ............................................................................
//
//  this fcn is called whenever we double-click on an object
//
// ----------------------------------------------------------------------------

void SchemeWindow::EditObject( Point& rPos )
{
    ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( rPos );

     if (  NULL != current_object )
     {
          enum ProcObjState prev_state = current_object->ChangeState( PO_OPEN );

          in_edit_mode = TRUE;

          if ( PO_RC_ACCEPT == current_object->EditContents( ) )
               GetScheme().SetStatus( SIM_DATA_MODIFIED );

          in_edit_mode = FALSE;

          current_object->ChangeState( prev_state );

//        ReconstructSpeciesDatabase( );
     }

     GetpFrame()->UpdateStatusBar();
}








// ----------------------------------------------------------------------------
//                     SchemeWindow::Activate()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeWindow::Activate()
{
     if (!in_edit_mode)
     {
          UINT16 num_selected_objs = select_object_list.Count();
      
          for ( UINT16 i = 0; i < num_selected_objs; i++ )
          {
               select_object_list.GetObject( i)->ChangeState( PO_SELECT );
          }
     }
}





// ----------------------------------------------------------------------------
//                     SchemeWindow::Deactivate()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeWindow::Deactivate()
{
     if (!in_edit_mode)
     {
          UINT16 num_selected_objs = select_object_list.Count();
      
          for ( UINT16 i = 0; i < num_selected_objs; i++ )
          {
               select_object_list.GetObject( i)->ChangeState( PO_NORMAL );
          }
     }
}



// ----------------------------------------------------------------------------
//                     SchemeWindow::ReconstructSpeciesDatabase( )
// ............................................................................
//
//  this function rebuilds the species database after the reaction scheme
//  has been modified
//
// ----------------------------------------------------------------------------

void SchemeWindow::ReconstructSpeciesDatabase( )
{
     GetScheme().ReconstructSpeciesDatabase();
     
     // if there are no species in the database then prevent access to species
     // database dialog

     GetpFrame()->UpdateToolbarActivations();
}






// ----------------------------------------------------------------------------
//                         SchemeWindow::CalcTotalSize( )
// ............................................................................
//
//  calculates the smallest Size necessary to enclose all the ProcessObjects
//  currently in the scheme
//
// ----------------------------------------------------------------------------

Size SchemeWindow::CalcTotalSize( ) const
{
     Rectangle rect( 0,0,0,0 );

     UINT16 num_compartments = GetScheme().GetNumCompartments( );
     for ( UINT16 i = 0; i < num_compartments; i++ )
          rect.Union( GetScheme().GetCompartmentObjectPtrFromIndex( i )->GetActiveArea() );

     // check for wrapped xfers and adjust size as needed 

     UINT16 num_xfers = GetScheme().GetNumTransferObjects();

     for ( UINT16 j = 0; j < num_xfers; j++ )
     {
          TransferObject* ptr = GetScheme().GetTransferObjectPtrFromIndex( j );

          if ( PO_TYPE_WRAPPED_XFER == ptr->GetType() )
          {
               rect.Union( ((WrappedTransferObject*) ptr)->GetPhantomRectangle() );
          }
     }

     return Size( rect.GetWidth() + GetDefaultCharWidth(),
                  rect.GetHeight() + GetDefaultCharHeight() );
}





// ----------------------------------------------------------------------------
//                     SchemeWindow::DeleteResultsWindows( )
// ............................................................................
//
// if there are any results windows, ask user if it is ok to delete these windows 
// if the answer is yes, then delete them and return TRUE; return false if the answer
// is no.  If there are no windows existent then immediately return TRUE
//  
// ----------------------------------------------------------------------------

BOOL8 SchemeWindow::DeleteResultsWindows()
{
     if ( results_window_list.First() )
     {
		// we have at least one window 

         String msg( ResId( vID_STR_RESULTS_WIN_CLOSE1 ) );

         msg =  msg + String( results_window_list.Count() ) + 
                String( ResId( vID_STR_RESULTS_WIN_CLOSE2 ) ) +
                GetScheme().GetNickName( ) + String( ResId( vID_STR_RESULTS_WIN_CLOSE3 ) );

         QueryBox query_box( NULL, WB_OK_CANCEL, msg );

          if ( RET_OK == query_box.Execute() )
          {
               delete results_window_list.GetCurObject();
     
               while ( results_window_list.Next() )
                    delete results_window_list.GetCurObject();
     
               results_window_list.Clear();

			return TRUE;
          }
          else 
          	return FALSE;
     }
     else
          return TRUE;
}




// ----------------------------------------------------------------------------
//                     SchemeWindow::QueryRxnFileSchemeType()
// ............................................................................
//
// this static function reade part of the header of the file named rFileName
// and attempts to determined what the file type is. If is can't or there
// was some error reading the file then file type UNKNOWN is returned 
//  
// ----------------------------------------------------------------------------

enum SCHEMWIN_TYPE SchemeWindow::QueryRxnFileSchemeType( const String& rFileName )
{
     BinaryInputStream infile( rFileName );

     if ( !infile.IsOpen() )
          return SW_TYPE_UNKNOWN;

     // is this a VSIM file for this OS?

     IDType first_type;
     IDType os_type;

     infile >> first_type;
     infile >> os_type;

     if ( ( first_type != IDType( SCHEME_FRAME_WINDOW )  ) || 
          ( os_type    != IDType( CURRENT_OS )  )
        )
          return SW_TYPE_UNKNOWN;


     infile.MoveTo( SIMSCHEME_TYPE_POSITION );

     UINT16 temp;

     infile >> temp;

     // cast to enum 

     enum SCHEMWIN_TYPE temp_type = (enum SCHEMWIN_TYPE) temp;

     switch ( temp_type )
     {
          case SW_TYPE_COMPARTMENT :
          case SW_TYPE_3D :

               return temp_type;

          default :
               return SW_TYPE_UNKNOWN;
     }

}







// ----------------------------------------------------------------------------
//                 SchemeWindow::GetSelectedObject()
// ............................................................................
//
//  
// ----------------------------------------------------------------------------

ProcessObject*  SchemeWindow::GetSelectObject () 
{ 
     ProcessObjectList& list = GetSelectObjectList();

     return ( 0 == list.Count() ) ? NULL : list.GetObject( 0 );
}







// ----------------------------------------------------------------------------
//                         SchemeWindow::SetSelectObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeWindow::SetSelectObject ( ProcessObject* pPO )
{
     // if there is a currently selected object, de-select it

     ProcessObjectList& list = GetSelectObjectList();

     if ( 0 < list.Count() )
     {
          ASSERT ( list.Count() == 1 );

          ProcessObject* select_object = list.GetObject( 0 );

          select_object->ChangeState( PO_NORMAL );

          list.Clear();
     }

     // if new selection  != NULL then change its state to selected
     // and save ptr 

     if ( pPO )
     {
          pPO->ChangeState( PO_SELECT );
          list.Insert( pPO, LIST_APPEND );
     }

     // is nothing is selected then de-activate edit btns on ToolBar

     GetpMainApp()->GetMainWindow()->ActivateEditTools( pPO != NULL );
}








// ----------------------------------------------------------------------------
//                     SchemeWindow::CopySelectedObject()
// ............................................................................
//
//  creates a copy of the objected currently selected. Returns TRUE if
//  succesful, FALSE if faliure
//
// ----------------------------------------------------------------------------

BOOL8 SchemeWindow::CopySelectedObject( ProcessObject*& rpClipObject )
{
     ProcessObject* selection = GetSelectObject();

     if ( selection != NULL )
     {

          ProcessObject* pcopy;

          switch ( selection->GetType() )
          {
                // create a copy of the selected object depending on its type

          case PO_TYPE_COMPT:

                pcopy = new ClippedCompartmentObject( *((CompartmentObject*) selection) );
                break;


           case PO_TYPE_XFER:
           case PO_TYPE_WRAPPED_XFER:

                pcopy = new TransferObject( *((TransferObject*) selection) );
                break;


           default :
                ASSERT( 0 );
                break;
          }

          if ( pcopy )
          {
               // if clipboard already contains a copy of something then delete it

               if (NULL != rpClipObject )
                    delete rpClipObject;

               rpClipObject = pcopy;
               return TRUE;
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               return FALSE;
          }

     }
     else
          return FALSE;
}





// ----------------------------------------------------------------------------
//                 SchemeWindow::GetHighlightObject()
// ............................................................................
//
//  
// ----------------------------------------------------------------------------

ProcessObject*  SchemeWindow::GetHighlightObject () 
{ 
     ProcessObjectList& list = GetHighlightObjectList();

     return ( 0 == list.Count() ) ? NULL : list.GetObject( 0 );
}





// ----------------------------------------------------------------------------
//                SchemeWindow::SetHighlightObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeWindow::SetHighlightObject ( ProcessObject* pPO )
{
     // if there is a curren move object, remove from list

     ProcessObjectList& list = GetHighlightObjectList();

     if ( 0 < list.Count() )
     {
          ASSERT ( list.Count() == 1 );

          list.Clear();
     }

     // if new selection  != NULL then save ptr 

     if ( pPO )
     {
          list.Insert( pPO, LIST_APPEND );
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( String( ResId( vID_STR_SCHEME_HELP ) ) );
     }
     else
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( GetpMainApp()->GetAppWindow()->GetHelpText( ) );
}







// ----------------------------------------------------------------------------
//                         SchemeWindow::SelectObject( )
// ............................................................................
//
//  this function is called whenever the Mouse button is clicked and there is no
//  addition operation in progress. Here we figure out whether we are over an
//  object. If so it is selcted and its appearance is updated to reflect that.
//  If we are not over an object then any selected object is deselected.
//
// ----------------------------------------------------------------------------

void SchemeWindow::SelectObject( Point& rPos )
{
    // test to see if we above an object

    ProcessObject* po = GetScheme().GetProcessObjectPtrFromLocation( rPos );

    // if so then change selection if necessary

    if ( po )
    {
         if ( po == GetSelectObject() )  // still on the same object - do nothing
              return;
         else
              // a previously unselected object so change its state

              SetSelectObject( po );
    }
    else
         // we are not above any object - if there is a selected
         // object then we have just clicked on the window - deselect
         // the selected object

         SetSelectObject( );  // deselect selected object if any
}



// ----------------------------------------------------------------------------
//                   SchemeWindow::ActivateDragAttributes( )
// ............................................................................
//
//  sets drawing parameters to enable dragging of objects around on the screen
//
// ----------------------------------------------------------------------------

void SchemeWindow::ActivateDragAttributes( )
{
     // use XOR raster op to facilitate erase of previous figure

     ChangeRasterOp( ROP_XOR );

     // set window brush transparent and pen so it will be visible upon XOR draw

     ChangePen( drag_pen );
     ChangeFillInBrush( drag_brush );

     original_ptr = ChangePointer( Pointer( POINTER_CROSS) );

     CaptureMouse();
}


// ----------------------------------------------------------------------------
//                     SchemeWindow::DeactivateDragAttributes( )
// ............................................................................
//
//  resets drawing parameters to their normal drawing state
//
// ----------------------------------------------------------------------------

void SchemeWindow::DeactivateDragAttributes( )
{
     ChangeRasterOp( ROP_OVERPAINT );
     ChangePen( std_pen );
     ChangeFillInBrush( std_brush );
     ChangePointer( original_ptr );

     ReleaseMouse();
}

