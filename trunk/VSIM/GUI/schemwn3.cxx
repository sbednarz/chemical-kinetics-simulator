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
//  SCHEMWN3.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This modules defines the ThreeDArraySchemeWindow class, an MDI window 
//  containing a view of a reaction scheme that is a 1,2 or 3 -dimensional array
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
#include "schemwn3.hxx"
#include "mainapp.hxx"
#include "statwin.hxx"



#endif


#include "strings.hrc"


// ----------------------------------------------------------------------------
//           ThreeDArraySchemeWindow::ThreeDArraySchemeWindow()
// ............................................................................
//
//
//   constructor 
//
// ----------------------------------------------------------------------------

ThreeDArraySchemeWindow::ThreeDArraySchemeWindow( Window* pParent ) :
SchemeWindow( pParent ), 
num_compts_x(0),
num_compts_y(0),
num_compts_z(0)
{ 
     GetScheme().SetSimulatorName( String( ResId( vID_STR_3D_ENGINE_FILENAME ) ) );
}




// ----------------------------------------------------------------------------
//                   ThreeDArraySchemeWindow::Add3DArray()
// ............................................................................
//
//  function initializes the scheme with an array of compartments and 
//  transfer paths 
//
//
//        0      1      2       ....                 num_compts_x-1
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
//    |      <->     <->     <->     <->    <->     <->      |  num_compts_y-1
//    |       |       |       |       |      |       |       |
//    +------------------------------------------------------+
//
//
// ----------------------------------------------------------------------------

BOOL8  ThreeDArraySchemeWindow::Add3DArray( UINT16 NumComptsX, UINT16 NumComptsY, 
        UINT16 NumComptsZ, FLOAT64 CellXDimension, FLOAT64 CellYDimension, 
        FLOAT64 CellZDimension, BOOL8 XWrap, BOOL8 YWrap, BOOL8 ZWrap  )
{
     const UINT32    PRINT_INTERVAL = 16; 

     StatusWindow * pstatwin = new StatusWindow( this );

     ASSERT ( ( 0 == GetScheme().GetNumCompartments() )  && 
              ( 0 == GetScheme().GetNumTransferObjects() )
            );

     BOOL8 retval = TRUE;
     String name = String( "X" );

     num_compts_x = NumComptsX;
     num_compts_y = NumComptsY; 
     num_compts_z = NumComptsZ; 

     UINT32 num_compartments   = num_compts_y * num_compts_x *  num_compts_z;

     UINT32 num_xfer_paths_per_z_layer = ( num_compts_x * (num_compts_y-1)) + ( num_compts_y * (num_compts_x-1) );

     UINT32 num_transfer_paths = ( num_compts_z * num_xfer_paths_per_z_layer ) + 
      ( (num_compts_z-1) * (num_compts_x * num_compts_y ) );


     UINT32 num_wrapped_xfer_paths = (XWrap ? (num_compts_y * num_compts_z ) : 0 ) +
                                     (YWrap ? (num_compts_x * num_compts_z ) : 0 ) +
                                     (ZWrap ? (num_compts_x * num_compts_y ) : 0 ) ;

     if ( pstatwin )
     {
           pstatwin->SetStatusText( 
            String( ResId( vID_STR_CREATING ) ) +
            String( num_compartments ) +
            String( ResId( vID_STR_COMPARTMENTS ) ) 
            );
           
           pstatwin->Show();
           pstatwin->Update();
     }

      GetpMainApp()->Reschedule();


     for ( UINT32 i = 0; i < num_compartments; i++ )
     {
          // make a temporary name based on index 


          CompartmentObject* ptr = new CompartmentObject( name, this );

          if ( NULL == ptr )
          {
               retval = FALSE;
               break;
          }
          else
          {
               ptr->SetXDimension( CellXDimension );
               ptr->SetYDimension( CellYDimension );
               ptr->SetZDimension( CellZDimension );
               ptr->SetVolumeOption( CONSTANT_VOLUME );

               GetScheme().AddCompartmentObject( ptr );

               // periodically print out progress on status window 

               if ( ( i  > 0 ) && 
                    ( 0 == ( i % PRINT_INTERVAL ) )
                  )
               {
                    if ( pstatwin )
                    {
                         pstatwin->SetStatusText( 
                          String( ResId( vID_STR_CREATED ) ) +
                          String( i ) +
                          String( ResId( vID_STR_OF ) ) +
                          String( num_compartments ) +
                          String( ResId( vID_STR_COMPARTMENTS ) ) 
                          );
                    }

                     GetpMainApp()->Reschedule();
               }
          }
     }

     // if everything went OK then continue with TransferPath creations

     if ( TRUE == retval )
     {

          if ( pstatwin )
          {
               pstatwin->SetStatusText( 
                String( ResId( vID_STR_CREATING ) ) +
                String( num_transfer_paths ) +
                String( ResId( vID_STR_XFERPATHS ) ) 
                );
          }

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
                    GetScheme().AddTransferObject( ptr );

                    if ( ( i  > 0 ) && 
                         ( 0 == ( i % PRINT_INTERVAL ) ) 
                       )
                    {
                         // periodically print out progress on status window 

                         if ( pstatwin )
                         {
                              pstatwin->SetStatusText( 
                               String( ResId( vID_STR_CREATED ) ) +
                               String( i ) +
                               String( ResId( vID_STR_OF ) ) +
                               String( num_transfer_paths ) +
                               String( ResId( vID_STR_XFERPATHS ) ) 
                               );
                         }

                          GetpMainApp()->Reschedule();
                    }
               }
          }

     }


     // if everything went OK then continue with WrappedTransferPath creations

     if ( TRUE == retval )
     {

          if ( pstatwin )
          {
               pstatwin->SetStatusText( 
                String( ResId( vID_STR_CREATING ) ) +
                String( num_wrapped_xfer_paths ) +
                String( ResId( vID_STR_WRAPPED_XFERPATHS ) ) 
                );
          }

          GetpMainApp()->Reschedule();

          for ( UINT32 i = 0; i < num_wrapped_xfer_paths; i++ )
          {
               // create name based on index 

               name = String( ResId( vID_STR_ARRAY_XFERPATH_ROOT ) ) + String( num_transfer_paths + i + 1 );

               WrappedTransferObject* ptr = new WrappedTransferObject( name, this );
     
               if ( NULL == ptr )
               {
                    retval = FALSE;
                    break;
               }
               else
               {
                    GetScheme().AddTransferObject( ptr );

                    if ( ( i  > 0 ) && 
                         ( 0 == ( i % PRINT_INTERVAL ) ) 
                       )
                    {
                         // periodically print out progress on status window 

                         if ( pstatwin )
                         {
                              pstatwin->SetStatusText( 
                               String( ResId( vID_STR_CREATED ) ) +
                               String( i ) +
                               String( ResId( vID_STR_OF ) ) +
                               String( num_wrapped_xfer_paths ) +
                               String( ResId( vID_STR_WRAPPED_XFERPATHS ) ) 
                               );
                         }

                          GetpMainApp()->Reschedule();
                    }
               }
          }

     }




     if ( TRUE == retval )
     {
          if ( pstatwin )
               pstatwin->SetStatusText( String( ResId( vID_STR_ALIGN_COMPTS ) ) );

          GetpMainApp()->Reschedule();

          // If everything went OK then we now have completed creation of all objects
          // Now make all the connections and position the objects
     
          // we will work though the array methodically, from left to right 
          // ( e.g from column 0 to column (num_compts_x-1), starting in row 0;
          // then we repeat for row 1 etc.
     
          // At each step we do the following :
          //
          //   a. if not last compartment in row,  link to compartment to immediate right
          //   b. if not last compartment in column, link to compartment immediately below
          //   
     
          UINT32 current_compartment = 0;
          UINT32 current_xferpath    = 0;

          Rectangle    compt_rect = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment )->GetActiveArea();

          const INT16  init_compt_x_offset = compt_rect.GetWidth();
          const INT16  init_compt_y_offset = compt_rect.GetHeight();

          if (num_compartments > 1 )
          {
               const Rectangle    xfer_rect  = GetScheme().GetTransferObjectPtrFromIndex( current_compartment )->GetActiveArea();
     
               const INT16  x_increment = compt_rect.GetWidth() + xfer_rect.GetWidth() + ( 2 * GetDefaultCharWidth() );
               const INT16  y_increment = compt_rect.GetHeight() + xfer_rect.GetHeight() + ( 2 * GetDefaultCharHeight() ); 
     
               String row_name;
               String column_name;
     
               for ( UINT16 row_num = 0; row_num < num_compts_y; row_num++ )
               {
                    row_name = String( ResId( vID_STR_ARRAY_COMPT_ROW_PREFIX ) ) + String( row_num+1 );
     
                    for ( UINT16 column_num = 0; column_num < num_compts_x; column_num++ )
                    {
     
                         column_name = String( ResId( vID_STR_ARRAY_COMPT_COL_PREFIX ) ) + String( column_num+1 );
     
                         compt_rect.ChangePos( 
                          Point( init_compt_x_offset + (column_num * x_increment ), 
                                 init_compt_y_offset + (row_num * y_increment ) ) );
     
     
                         CompartmentObject* pcompt = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
     
                         pcompt->SetActiveArea( compt_rect );
                         pcompt->SetName( row_name+ column_name );
     
                         if ( column_num < (num_compts_x-1) )
                         {
                              // first add transfer object between compartments in a  single row 
     
                              // get some pointers 
     
                              CompartmentObject* pc1 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                              CompartmentObject* pc2 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment+1 );
                              TransferObject*    pt1 = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );
     
                              pc1->AddTransferObject( pt1 );
                              pc2->AddTransferObject( pt1 );
                              pt1->SetpSourceCompartment( pc1 );
                              pt1->SetpTargetCompartment( pc2 );
     
                              pt1->SetDirection( X_AXIS );
     
                              current_xferpath++;
                         }
     
                         if (row_num < (num_compts_y-1) )
                         {
                              // first add transfer object between compartments in current row  and the one below 
     
                              // get some pointers 
     
                              CompartmentObject* pc1 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                              CompartmentObject* pc2 = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment + num_compts_x );
                              TransferObject*    pt1 = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );
     
                              pc1->AddTransferObject( pt1 );
                              pc2->AddTransferObject( pt1 );
                              pt1->SetpSourceCompartment( pc1 );
                              pt1->SetpTargetCompartment( pc2 );
     
                              pt1->SetDirection( Y_AXIS );
     
                              current_xferpath++;
                         }
     
                         current_compartment++;
                    }
     
               }

               // now deal with wrapped transfer objects here 
               // the wrapped transfer objects come after the regular trasnfer objects
               // in the transfer object list


               if (TRUE == XWrap )
               {
                    current_compartment = 0;

                    while (current_compartment < num_compartments )
                    {
                         CompartmentObject* pct = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                         CompartmentObject* pcs = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment + num_compts_x -1 );
     
                         TransferObject*    pwt = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );
     
                         pcs->AddTransferObject( pwt );
                         pct->AddTransferObject( pwt );
                         pwt->SetpSourceCompartment( pcs );
                         pwt->SetpTargetCompartment( pct );
     
                         pwt->SetDirection( X_AXIS );

                         current_compartment += num_compts_x;
                         current_xferpath++;
                    }

               }
     
               if (TRUE == YWrap )
               {
                    current_compartment = 0;

                    while (current_compartment < num_compts_x )
                    {
                         CompartmentObject* pct = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment );
                         CompartmentObject* pcs = GetScheme().GetCompartmentObjectPtrFromIndex( current_compartment + (num_compts_x *(num_compts_y-1)) );
     
                         TransferObject*    pwt = GetScheme().GetTransferObjectPtrFromIndex( current_xferpath );
     
                         pcs->AddTransferObject( pwt );
                         pct->AddTransferObject( pwt );
                         pwt->SetpSourceCompartment( pcs );
                         pwt->SetpTargetCompartment( pct );
     
                         pwt->SetDirection( Y_AXIS );

                         current_compartment++;
                         current_xferpath++;
                    }
               }


     
          }
          else
          {
               String row_name = String( ResId( vID_STR_ARRAY_COMPT_ROW_PREFIX ) ) + String( 1 );
     
               String column_name = String( ResId( vID_STR_ARRAY_COMPT_COL_PREFIX ) ) + String( 1 );
     
               compt_rect.ChangePos( Point( init_compt_x_offset, init_compt_y_offset ) );
     
               CompartmentObject* pcompt = GetScheme().GetCompartmentObjectPtrFromIndex( 0 );
     
               pcompt->SetActiveArea( compt_rect );
               pcompt->SetName( row_name+column_name );
     
          }


          if ( pstatwin )
               pstatwin->SetStatusText( String( ResId( vID_STR_ALIGN_XFERPATHS ) ) );

          GetpMainApp()->Reschedule();

          for ( UINT32 xferpath = 0; xferpath < (num_transfer_paths+num_wrapped_xfer_paths); xferpath++ )
          {
              GetScheme().GetTransferObjectPtrFromIndex( xferpath )->Reposition();
          }
               
          RecalcTotalSize();

          GetScheme().SetStatus( SIM_DATA_MODIFIED );
          GetpFrame()->UpdateStatusBar();
     }

     if ( pstatwin )
          delete pstatwin;

     return retval;
}




// ----------------------------------------------------------------------------
//                    ThreeDArraySchemeWindow::PasteObject()
// ............................................................................
//
//  we don't actually paste an object into the reaction scheme in this instance.
//  Instead we do a "flood fill" where each selected object is set to contain
//  most of the values in the object on the clipboard
//  returns TRUE if successful, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8  ThreeDArraySchemeWindow::PasteObject( const ProcessObject* const pClipObject )
{
     GetpMainApp()->Wait( TRUE );

     switch ( pClipObject->GetType() )
     {
          case PO_TYPE_COMPT:
          {
               ProcessObjectList& list = GetSelectObjectList();
           
               UINT16 num_selected_objs = list.Count();

               String old_name;
               Rectangle old_area;
           
               for ( UINT16 i = 0; i < num_selected_objs; i++ )
               {
                    CompartmentObject* pcompt = (CompartmentObject*) list.GetObject( i);

                    // save some of the old values 

                    old_name = pcompt->GetName();
                    old_area = pcompt->GetActiveArea();


                    FLOAT64  x = pcompt->GetXDimension();
                    FLOAT64  y = pcompt->GetYDimension();
                    FLOAT64  z = pcompt->GetZDimension();

                    *pcompt = *( (CompartmentObject*) pClipObject ); 

                    pcompt->SetName( old_name );
                    pcompt->SetActiveArea( old_area );

                    pcompt->SetXDimension(x);
                    pcompt->SetYDimension(y);
                    pcompt->SetZDimension(z);

                    // are these necessary ?

                    pcompt->SetpParent( this ); 
                    pcompt->SetpOutputDevice( this );

               }

               // first reconstruct species database so that new species are included

               ReconstructSpeciesDatabase( );

               // we are now going to set the species data in the species database
               // to have values that were in effect for the compartment now being pasted
               // that were in effect when it was cut/copied  from the original  CompartmentSchemeWindow

               // a typecast for readability

               ClippedCompartmentObject* p_clipped_compt = ( ClippedCompartmentObject* ) pClipObject;

               UINT16 num_species_to_paste = p_clipped_compt->GetNumberOfSpeciesToPaste();


               for ( i = 0; i < num_species_to_paste ; i++ )
               {
                    // now go through the  list of species database entries in the
                    // clipped object and update the species databse in the new parent
                    // scheme window to the original values

                    const SpeciesDatabaseEntry& r_entry = p_clipped_compt->GetSpeciesData( i );
                    GetScheme().GetpSpeciesDatabase()->SetSpeciesDataByName( r_entry.GetName(), r_entry );
               }
           }
           break;


           case PO_TYPE_XFER :
           {
               ProcessObjectList& list = GetSelectObjectList();
           
               UINT16 num_selected_objs = list.Count();

               String old_name;
               Rectangle old_area;
               enum DIRECTION old_direction;
           
               for ( UINT16 i = 0; i < num_selected_objs; i++ )
               {
                    TransferObject* pxfer = (TransferObject*) list.GetObject( i);

                    // save some of the old values 

                    old_name      = pxfer->GetName();
                    old_area      = pxfer->GetActiveArea();
                    old_direction = pxfer->GetDirection();

                    *pxfer = *( (TransferObject*) pClipObject ); 

                    pxfer->SetName( old_name );
                    pxfer->SetActiveArea( old_area );
                    pxfer->SetDirection( old_direction );

                    pxfer->Reposition( );

                    // are these necessary ?

                    pxfer->SetpParent( this ); // change parent to this CompartmentSchemeWindow
                    pxfer->SetpOutputDevice( this );

                    // now delete any transfer step for species which do not
                    // appear in the species database of the new parent CompartmentSchemeWindow
                    // we work from the end of the list toward the front since
                    // the position index changes for all remaining steps
                    // if we delete a step
       
                    UINT16 xfer_step_num = pxfer->GetNumberOfTransferSteps();
       
                    // delete any TransferSteps for species which are not now in the
                    // CompartmentSchemeWindow's Species Database
       
                    while ( xfer_step_num-- )
                    {
                       const String& rname = pxfer->GetTransferStep( xfer_step_num ).GetSpeciesName( );
       
                       if ( !GetScheme().GetpSpeciesDatabase()->ContainsEntryFor( rname ) )
                            pxfer->DeleteTransferStep( xfer_step_num );
                    }

               }
  
          }
          break;

          default :

               // abort right here if parameter is unknown

               ASSERT( 0 );
               break;
    }

    DeselectAllObjects();
    GetpMainApp()->GetMainWindow()->ActivateEditTools( FALSE );
    GetScheme().SetStatus( SIM_DATA_MODIFIED );
    GetpFrame()->UpdateStatusBar();

    GetpMainApp()->Wait( FALSE );

    return TRUE;
}








// ----------------------------------------------------------------------------
//                         ThreeDArraySchemeWindow::MouseMove( )
// ............................................................................
//
//  handler - called whenever the mouse is moved
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::MouseMove( const MouseEvent& rMEvt )
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
//                         ThreeDArraySchemeWindow::MouseButtonUp( )
// ............................................................................
//
//  handler - called whenever a MouseButton is released
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::MouseButtonUp( const MouseEvent& rMEvt )
{
     DrawRect( current_rect );
     DeactivateDragAttributes( );
     SetOperation( SW_NO_OP );

     // we only deal here with release of MB1 to set the new location of
     // a Compartment object being moved

     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( position );

     // if we are over an object and the rectangle we have drawn is completely within 
     // the current object then select it 

     if ( NULL != current_object && current_object->GetActiveArea().IsInside( current_rect ) )
     {
          SelectObject( position );
          GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() &&
           current_object->GetType() == GetpMainApp()->GetMainWindow()->GetClipObjectType()
          );
     }
     else
     {
          current_rect.Justify();

          switch ( GetpMainApp()->GetMainWindow()->GetClipObjectType() )
          {
          case PO_TYPE_NONE:

               DeselectAllObjects();
               GetpMainApp()->GetMainWindow()->ActivateEditTools( current_object != NULL );

               break;


          case PO_TYPE_COMPT :

               SelectCompartmentObjects( );

               break;


          case PO_TYPE_XFER :

               SelectTransferObjects( );

               break;
          }
          GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() );
     }


//     GetScheme().SetStatus( SIM_DATA_MODIFIED );
     GetpMainApp()->GetMainWindow()->GetStatusBar().SetText( GetpMainApp()->GetMainWindow()->GetHelpText() );
     GetpFrame()->UpdateStatusBar();


     GetpMainApp()->GetMainWindow()->ActivateSelectAllTools( TRUE );
}





// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::DeselectAllObjects()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::DeselectAllObjects( )
{
    ProcessObjectList& list = GetSelectObjectList();


    UINT16 num_selected_objs = list.Count();

    for ( UINT16 i = 0; i < num_selected_objs; i++ )
    {
         list.GetObject( i)->ChangeState( PO_NORMAL );
    }

    list.Clear();
}





// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::SelectCompartmentObjects()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::SelectCompartmentObjects( )
{
    ProcessObjectList&  list        = GetSelectObjectList();
    ReactionScheme&     scheme      = GetScheme();
    UINT16              num_compts  = scheme.GetNumCompartments();

    for ( UINT16 i = 0; i < num_compts; i++ )
    {
         CompartmentObject* pcompt = scheme.GetCompartmentObjectPtrFromIndex( i );

         if ( current_rect.IsInside( pcompt->GetActiveArea() ) )
         {
              list.Insert( pcompt, LIST_APPEND );
              pcompt->ChangeState( PO_SELECT );
         }
    }
}






// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::SelectAllCompartmentObjects()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::SelectAllCompartmentObjects( )
{
    DeselectAllObjects(); 

    ProcessObjectList&  list        = GetSelectObjectList();
    ReactionScheme&     scheme      = GetScheme();
    UINT16              num_compts  = scheme.GetNumCompartments();

    for ( UINT16 i = 0; i < num_compts; i++ )
    {
         CompartmentObject* pcompt = scheme.GetCompartmentObjectPtrFromIndex( i );

         list.Insert( pcompt, LIST_APPEND );
         pcompt->ChangeState( PO_SELECT );
    }

    GetpMainApp()->GetMainWindow()->ActivateEditTools( FALSE );
    GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() );
}







// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::SelectTransferObjects()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::SelectTransferObjects( )
{
    ProcessObjectList&  list            = GetSelectObjectList();
    ReactionScheme&     scheme          = GetScheme();
    UINT16              num_xfer_paths  = scheme.GetNumTransferObjects();

    for ( UINT16 i = 0; i < num_xfer_paths; i++ )
    {
         TransferObject* pxfer = scheme.GetTransferObjectPtrFromIndex( i );

         if (current_rect.IsInside( pxfer->GetActiveArea() ) )
         {
              list.Insert( pxfer, LIST_APPEND );
              pxfer->ChangeState( PO_SELECT );
         }
    }

    GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() );
}






// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::SelectAllTransferObjects()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::SelectAllTransferObjects( )
{
    DeselectAllObjects(); 

    ProcessObjectList&  list            = GetSelectObjectList();
    ReactionScheme&     scheme          = GetScheme();
    UINT16              num_xfer_paths  = scheme.GetNumTransferObjects();

    for ( UINT16 i = 0; i < num_xfer_paths; i++ )
    {
         TransferObject* pxfer = scheme.GetTransferObjectPtrFromIndex( i );

         list.Insert( pxfer, LIST_APPEND );
         pxfer->ChangeState( PO_SELECT );
    }

    GetpMainApp()->GetMainWindow()->ActivateEditTools( FALSE );
    GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() );
}







// ----------------------------------------------------------------------------
//                         ThreeDArraySchemeWindow::MouseButtonDown( )
// ............................................................................
//
//  handler - called whenever a MouseButton is pressed
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::MouseButtonDown( const MouseEvent& rMEvt )
{
     Point position = PixelToLogic( rMEvt.GetPosPixel() );

     ProcessObject* current_object = GetScheme().GetProcessObjectPtrFromLocation( position );

     if ( NULL != current_object && (rMEvt.GetClicks() > 1) )
     {
          SetOperation( SW_NO_OP );
          EditObject( position );
          GetpMainApp()->GetMainWindow()->ActivatePaste( 
           current_object->GetType() == GetpMainApp()->GetMainWindow()->GetClipObjectType() );
      
     }
     else
     {
          // start rubber band to select multiple objects
          // first deselect any selected object 

          DeselectAllObjects( );
          GetpMainApp()->GetMainWindow()->ActivateEditTools( current_object != NULL );
          GetpMainApp()->GetMainWindow()->ActivateSelectAllTools( TRUE );

          SetOperation( SW_RUBBER_BAND_SELECT );

          previous_rect = Rectangle();

          current_rect  = Rectangle( position, position );
          ActivateDragAttributes( );
     }

}






// ----------------------------------------------------------------------------
//                       ThreeDArraySchemeWindow::Activate()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::Activate()
{
     SchemeWindow::Activate();
     GetpMainApp()->GetMainWindow()->Activate3DSpecialEditTools( TRUE );                                     
     GetpMainApp()->GetMainWindow()->ActivateSelectAllTools( TRUE );

     GetpMainApp()->GetMainWindow()->ActivatePaste( IsObjectSelected() &&
           GetSelectObjectList().GetObject(0)->GetType() == GetpMainApp()->GetMainWindow()->GetClipObjectType() );
}



// ----------------------------------------------------------------------------
//                       ThreeDArraySchemeWindow::Deactivate()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::Deactivate() 
{
     SchemeWindow::Deactivate();
     GetpMainApp()->GetMainWindow()->Activate3DSpecialEditTools( FALSE );                                     
     GetpMainApp()->GetMainWindow()->ActivateSelectAllTools( FALSE );
     GetpMainApp()->GetMainWindow()->ActivatePaste( FALSE );
}






// ----------------------------------------------------------------------------
//                     ThreeDArraySchemeWindow::CopySelectedObject()
// ............................................................................
//
//  creates a copy of the objected currently selected. Returns TRUE if
//  succesful, FALSE if faliure
//
// ----------------------------------------------------------------------------

BOOL8 ThreeDArraySchemeWindow::CopySelectedObject( ProcessObject*& rpClipObject )
{
     BOOL8 result = SchemeWindow::CopySelectedObject( rpClipObject );
     SetSelectObject( NULL );
     return result;
}





// ----------------------------------------------------------------------------
//             ThreeDArraySchemeWindow::ChangeCompartmentDimensions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::ChangeCompartmentDimensions() 
{ 
     GetScheme().ChangeAllCompartmentDimensions(); 
     GetpFrame()->UpdateStatusBar();
}





// ----------------------------------------------------------------------------
//               ThreeDArraySchemeWindow::ChangeSchemeTemperature()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::ChangeSchemeTemperature() 
{ 
     GetScheme().ChangeSchemeTemperature();
     GetpFrame()->UpdateStatusBar();
}



// ----------------------------------------------------------------------------
//               ThreeDArraySchemeWindow::ChangeSpeciesConcentration()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ThreeDArraySchemeWindow::ChangeSpeciesConcentration()
{ 
     GetScheme().ChangeSpeciesConcentration();
     GetpFrame()->UpdateStatusBar();
}


