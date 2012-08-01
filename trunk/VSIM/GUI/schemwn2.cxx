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
//  SCHEMWN2.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This modules defines the TwoDArraySchemeWindow class, an MDI window 
//  containing a view of a reaction scheme that is a two-dimensional array
//  of Compartments connected by TransferPaths
//
//  CREATED : 6.24.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>
#include "schemwn2.hxx"
#include "mainapp.hxx"
#include "statwin.hxx"



#endif


#include "strings.hrc"


// ----------------------------------------------------------------------------
//           TwoDArraySchemeWindow::TwoDArraySchemeWindow()
// ............................................................................
//
//
//   constructor 
//
// ----------------------------------------------------------------------------

TwoDArraySchemeWindow::TwoDArraySchemeWindow( Window* pParent ) :
SchemeWindow( pParent ), 
num_rows(0), 
num_columns(0) 
{ 
}




// ----------------------------------------------------------------------------
//                   TwoDArraySchemeWindow::Add2DArray()
// ............................................................................
//
//  function initializes the scheme with an array of compartments and 
//  transfer paths 
//
//
//        0      1      2       ....                 NumColumns-1
//    +------------------------------------------------------+
//    |       |       |       |       |      |       |       |
//    |      <->     <->     <->     <->    <->     <->      |  0
//    |   +   |   +   |   +   |   +   |  +   |   +   |   +   |
//    |---|-------|-------|-------|------|-------|-------|---|
//    |   v   |   v   |   v   |   v   |  v   |   v   |   v   |
//    |      <->     <->     <->     <->    <->     <->      |  1
//    |   +   |   +   |   +   |   +   |  +   |   +   |   +   |
//    |---|-------|-------|-------|------|-------|-------|---|
//    |   v   |   v   |   v   |   v   |  v   |   v   |   v   |
//    |      <->     <->     <->     <->    <->     <->      |  2
//    |   +   |   +   |   +   |   +   |  +   |   +   |   +   |
//    |---|-------|-------|-------|------|-------|-------|---|
//    |   v   |   v   |   v   |   v   |  v   |   v   |   v   |
//    |      <->     <->     <->     <->    <->     <->      |  .
//    |   +   |   +   |   +   |   +   |  +   |   +   |   +   |
//    |---|-------|-------|-------|------|-------|-------|---|
//    |   v   |   v   |   v   |   v   |  v   |   v   |   v   |
//    |      <->     <->     <->     <->    <->     <->      |
//    |   +   |   +   |   +   |   +   |  +   |   +   |   +   |
//    |---|-------|-------|-------|------|-------|-------|---|
//    |   v   |   v   |   v   |   v   |  v   |   v   |   v   |
//    |      <->     <->     <->     <->    <->     <->      |  NumRows-1
//    |       |       |       |       |      |       |       |
//    +------------------------------------------------------+
//
//
// ----------------------------------------------------------------------------

BOOL8  TwoDArraySchemeWindow::Add2DArray( UINT16 NumRows, UINT16 NumColumns )
{
     const UINT32    PRINT_INTERVAL = 16; 

     StatusWindow * pstatwin = new StatusWindow( this );

     ASSERT ( ( 0 == GetScheme().GetNumCompartments() )  && 
              ( 0 == GetScheme().GetNumTransferObjects() )
            );

     BOOL8 retval = TRUE;
     String name;

     num_rows = NumRows;
     num_columns = NumColumns;

     UINT32 num_compartments   = NumRows * NumColumns;
     UINT32 num_transfer_paths = ( NumRows * (NumColumns-1)) + ( NumColumns * (NumRows-1) );

     pstatwin->SetStatusText( 
      String( ResId( vID_STR_CREATING ) ) +
      String( num_compartments ) +
      String( ResId( vID_STR_COMPARTMENTS ) ) 
      );

      pstatwin->Show();

      GetpMainApp()->Reschedule();


     for ( UINT32 i = 0; i < num_compartments; i++ )
     {
          // make a name based on index 

          name = String( ResId( vID_STR_ARRAY_COMPT_ROOT ) ) + String( i+1 );

          CompartmentObject* ptr = new CompartmentObject( name, this );

          if ( NULL == ptr )
          {
               retval = FALSE;
               break;
          }
          else
          {
               GetScheme().AddCompartmentObject( ptr );

               // periodically print out progress on status window 

               if ( ( i  > 0 ) && 
                    ( 0 == ( i % PRINT_INTERVAL ) )
                  )
               {
                    pstatwin->SetStatusText( 
                     String( ResId( vID_STR_CREATED ) ) +
                     String( i ) +
                     String( ResId( vID_STR_OF ) ) +
                     String( num_compartments ) +
                     String( ResId( vID_STR_COMPARTMENTS ) ) 
                     );

                     GetpMainApp()->Reschedule();
               }
          }
     }

     // if everything went OK then continue with TransferPath creations

     if ( TRUE == retval )
     {

          pstatwin->SetStatusText( 
           String( ResId( vID_STR_CREATING ) ) +
           String( num_transfer_paths ) +
           String( ResId( vID_STR_XFERPATHS ) ) 
           );

          GetpMainApp()->Reschedule();

          for ( UINT32 i = 0; i < num_transfer_paths; i++ )
          {
               // create name based on index 

               name = String( ResId( vID_STR_ARRAY_XFERPATH_ROOT ) ) + String( i+1 );

               TransferObject* ptr = new TransferObject( name, this );
     
               if ( NULL == ptr )
               {
                    retval = FALSE;
                    break;
               }
               else
               {
                    ptr->ShowArrowHead( FALSE );

                    GetScheme().AddTransferObject( ptr );

                    if ( ( i  > 0 ) && 
                         ( 0 == ( i % PRINT_INTERVAL ) ) 
                       )
                    {
                         // periodically print out progress on status window 

                         pstatwin->SetStatusText( 
                          String( ResId( vID_STR_CREATED ) ) +
                          String( i ) +
                          String( ResId( vID_STR_OF ) ) +
                          String( num_transfer_paths ) +
                          String( ResId( vID_STR_XFERPATHS ) ) 
                          );

                          GetpMainApp()->Reschedule();
                    }
               }
          }

     }

     if ( TRUE == retval )
     {
          pstatwin->SetStatusText( String( ResId( vID_STR_ALIGN_COMPTS ) ) );

          GetpMainApp()->Reschedule();

          // if everything went OK then we now have completed creation of 
          // now make all the connections and position the objects
     
          // we will work though the array methodically, from left to right 
          // ( e.g from column 0 to column (NumColumns-1), starting in row 0;
          // then we repeat for row 1 etc.
     
          // At each step we do the following :
          //
          //   a. if not last compartment in row,  link to compartment to immediate right
          //   b. if not last compartment in column, link to compartment immediately below
          //   
     
          UINT32 current_compartment = 0;
          UINT32 current_xferpath    = 0;

          Rectangle    compt_rect = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment )->GetActiveArea();
          const Rectangle    xfer_rect  = GetScheme().GetTransferObjectPtrFromIndex( current_compartment )->GetActiveArea();

          const INT16  init_compt_x_offset = GetDefaultCharWidth();
          const INT16  init_compt_y_offset = GetDefaultCharHeight();

          const INT16  x_increment = compt_rect.GetWidth() + xfer_rect.GetWidth() + ( 2 * GetDefaultCharWidth() );
          const INT16  y_increment = compt_rect.GetHeight() + xfer_rect.GetHeight() + ( 2 * GetDefaultCharHeight() ); 

          for ( UINT16 row_num = 0; row_num < NumRows; row_num++ )
          {
               for ( UINT16 column_num = 0; column_num < NumColumns; column_num++ )
               {

                    compt_rect.ChangePos( 
                     Point( init_compt_x_offset + (column_num * x_increment ), 
                            init_compt_y_offset + (row_num * y_increment ) ) );

                    GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment )->SetActiveArea( compt_rect );

                    if ( column_num < (NumColumns-1) )
                    {
                         // first add transfer object between compartments in row 

                         // get some pointers 

                         CompartmentObject* pc1 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                         CompartmentObject* pc2 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment+1 );
                         TransferObject*    pt1 = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );

                         pc1->AddTransferObject( pt1 );
                         pt1->SetpSourceCompartment( pc1 );
                         pt1->SetpTargetCompartment( pc2 );

                         current_xferpath++;
                    }

                    if (row_num < (NumRows-1) )
                    {
                         // first add transfer object between compartments in row 

                         // get some pointers 

                         CompartmentObject* pc1 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                         CompartmentObject* pc2 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment + NumColumns );
                         TransferObject*    pt1 = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );

                         pc1->AddTransferObject( pt1 );
                         pt1->SetpSourceCompartment( pc1 );
                         pt1->SetpTargetCompartment( pc2 );

                         current_xferpath++;
                    }

                    current_compartment++;
               }

          }

          pstatwin->SetStatusText( String( ResId( vID_STR_ALIGN_XFERPATHS ) ) );

          GetpMainApp()->Reschedule();

          for ( UINT32 xferpath = 0; xferpath < num_transfer_paths; xferpath++ )
          {
              GetScheme().GetTransferObjectPtrFromIndex( xferpath )->Reposition();
          }
               
          RecalcTotalSize();

//        GetpFrame()->UpdateToolbarActivations( );
          GetpFrame()->UpdateStatusBar();
     }

     delete pstatwin;

     return retval;
}




// ----------------------------------------------------------------------------
//                    TwoDArraySchemeWindow::PasteObject()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

BOOL8  TwoDArraySchemeWindow::PasteObject( const ProcessObject* const pClipObject )
{
     return TRUE;
}







// ----------------------------------------------------------------------------
//                         TwoDArraySchemeWindow::MouseMove( )
// ............................................................................
//
//  handler - called whenever the mouse is moved
//
// ----------------------------------------------------------------------------

void TwoDArraySchemeWindow::MouseMove( const MouseEvent& rMEvt )
{
     Point position = PixelToLogic( rMEvt.GetPosPixel() );


     // get the current operation so we know how to handle the current event

     if ( SW_RUBBER_BAND_SELECT == GetOperation( ) )
     {
            current_rect.Bottom() = position.Y();
            current_rect.Right() = position.X();

            if ( current_rect != previous_rect )
            {
                 if ( !previous_rect.IsEmpty( ) )
                      DrawRect( previous_rect );

                 DrawRect( current_rect );
                 previous_rect = current_rect;
            }
     }
     else
     {
          ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( position );

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
}






// ----------------------------------------------------------------------------
//                         TwoDArraySchemeWindow::MouseButtonUp( )
// ............................................................................
//
//  handler - called whenever a MouseButton is released
//
// ----------------------------------------------------------------------------

void TwoDArraySchemeWindow::MouseButtonUp( const MouseEvent& rMEvt )
{
     // we only deal here with release of MB1 to set the new location of
     // a Compartment object being moved

     Point position = PixelToLogic( rMEvt.GetPosPixel() );

#if 0
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
#endif
}




// ----------------------------------------------------------------------------
//                         TwoDArraySchemeWindow::MouseButtonDown( )
// ............................................................................
//
//  handler - called whenever a MouseButton is pressed
//
// ----------------------------------------------------------------------------

void TwoDArraySchemeWindow::MouseButtonDown( const MouseEvent& rMEvt )
{
     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( position );

     if ( NULL != current_object )
     {
          // we are over an object 

          if (rMEvt.GetClicks() == 1)
               SelectObject( position );
          else
               EditObject( position );
      
     }
     else
     {
          // start rubber band to select multiple objects
          // first deselect any selected object 

          SetSelectObject ( );

          SetOperation( SW_RUBBER_BAND_SELECT );

          previous_rect = Rectangle();

          current_rect  = Rectangle( position, position );
          ActivateDragAttributes( );
     }

}




// ----------------------------------------------------------------------------
//                     TwoDArraySchemeWindow::CopySelectedObject()
// ............................................................................
//
//  creates a copy of the objected currently selected. Returns TRUE if
//  succesful, FALSE if faliure
//
// ----------------------------------------------------------------------------

BOOL8 TwoDArraySchemeWindow::CopySelectedObject( ProcessObject*& rpClipObject )
{
     BOOL8 result = SchemeWindow::CopySelectedObject( rpClipObject );
     SetSelectObject( NULL );
     return result;
}

