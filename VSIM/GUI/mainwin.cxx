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
//  MAINWIN.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the main Window class for
//  the VSIM user interface
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

#include "mainwin.hxx"
#include "mainapp.hxx"
#include "schemwin.hxx"

#endif


#include "strings.hrc"
#include "mainmenu.hrc"
#include "icons.hrc"




// ----------------------------------------------------------------------------
//                         MainWindow::MainWindow( )
// ............................................................................
//
//  constructor - creates  StatusBar also
// ----------------------------------------------------------------------------

MainWindow::MainWindow( Window* pParent, WinBits aWinStyle ) :
WorkWindow( pParent, aWinStyle ),
aHelpStatus( this, WB_BORDER | WB_SVLOOK ),
ToolBar( this, ResId( vID_TOOLBOX_APP ) ),
active_area(),
pclip_object( NULL )
{
     SetText( ResId( vID_STR_MAIN_WIN_TITLE ) );
     SetHelpText( String( ResId( vID_STR_MAIN_WIN_HELPTEXT) ) );

     // initialize status bar

     aHelpStatus.SetText( GetHelpText( ) );
     aHelpStatus.Show();

     // initialize tool bar

    ToolBar.ChangeHighlightHdl(  LINK( this, MainWindow, ToolBarHighlight ) );
    ToolBar.ChangeSelectHdl( LINK( this, MainWindow, ToolBarSelect ) );

    ActivateEditTools( FALSE );
    ActivateZoomTools( FALSE );
    ActivatePaste( FALSE );
    ActivateAddCompartment( FALSE );
    ActivateAddTransferObject( FALSE );
    ActivateSelectAllTools( FALSE );
    Activate3DSpecialEditTools( FALSE );                                     
}




// ----------------------------------------------------------------------------
//                         MainWindow::~MainWindow( )
// ............................................................................
//
//  destructor 
// ----------------------------------------------------------------------------

MainWindow::~MainWindow( )
{
     if ( NULL != pclip_object )
          delete pclip_object;
}



// ----------------------------------------------------------------------------
//                         MainWindow::AddCompartment( )
// ............................................................................
//
//  determines active MDIWindow and adds a new compartment to it
//
// ----------------------------------------------------------------------------

void MainWindow::AddCompartment( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          pcurrent_win->GetSchemeWindow().StartAddCompartment( );
     }
}





// ----------------------------------------------------------------------------
//                         MainWindow::ActivateZoomTools( )
// ............................................................................
//
//  changes the activity state of the zoom  buttons
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateZoomTools( const BOOL NewState )
{
     Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_VIEW );

    if ( FALSE == NewState )
    {
         ToolBar.EnableItem( vID_TI_ZOOMIN, FALSE );
         ToolBar.EnableItem( vID_TI_ZOOMOUT, FALSE );
         ToolBar.EnableItem( vID_TI_ZOOM100, FALSE );

         pmenu->EnableItem( vID_MI_ZOOMIN, FALSE );
         pmenu->EnableItem( vID_MI_ZOOMOUT, FALSE );
         pmenu->EnableItem( vID_MI_ZOOM100, FALSE );
    }
    else
    {
         // we only activate some of the buttons depending on
         // the active windows current scale

         ScrollingMDIWindow* pcurrent_win = (ScrollingMDIWindow*) GetpMainApp()->GetActiveWindow();

         Fraction scale( ScalableWindow::FULL_SCALE );

         if ( pcurrent_win )
               scale = pcurrent_win->GetDrawingScale();

         ToolBar.EnableItem( vID_TI_ZOOMIN, scale < ScalableWindow::MAXIMUM_SCALE );
         ToolBar.EnableItem( vID_TI_ZOOMOUT, scale > ScalableWindow::MINIMUM_SCALE );
         ToolBar.EnableItem( vID_TI_ZOOM100, scale != ScalableWindow::FULL_SCALE );

         pmenu->EnableItem( vID_MI_ZOOMIN, scale < ScalableWindow::MAXIMUM_SCALE );
         pmenu->EnableItem( vID_MI_ZOOMOUT, scale > ScalableWindow::MINIMUM_SCALE );
         pmenu->EnableItem( vID_MI_ZOOM100, scale != ScalableWindow::FULL_SCALE );
    }
}



// ----------------------------------------------------------------------------
//                         MainWindow::ActivateEditTools( )
// ............................................................................
//
//  changes the activity state of the Cut, Copy and Delete Toolbar buttons
//
// ----------------------------------------------------------------------------
void MainWindow::ActivateEditTools( const BOOL NewState )                                            
{                                                                                                    
     Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT );                         
                                                                                                     
     enum SCHEMWIN_TYPE  scheme_type = SW_TYPE_UNKNOWN;                                              
                                                                                                     
      // if we are activating tools, see if cut/delete are allowed.                                  
     // they are ot allowed in 3D array scheme                                                       
                                                                                                     
     if ( TRUE == NewState)                                                                          
     {                                                                                               
          SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
                                                                                                     
          if ( NULL != pcurrent_win)                                                                 
               scheme_type = pcurrent_win->GetSchemeWindow().GetSchemeType();                        
     }                                                                                               
                                                                                                     
     ToolBar.EnableItem( vID_TI_CUT, NewState && ( SW_TYPE_3D != scheme_type) );                     
     ToolBar.EnableItem( vID_TI_COPY, NewState );                                                    
     ToolBar.EnableItem( vID_TI_DELETE, NewState && ( SW_TYPE_3D != scheme_type) );                  
                                                                                                     
     pmenu->EnableItem( vID_MI_CUT, NewState && ( SW_TYPE_3D != scheme_type) );                      
     pmenu->EnableItem( vID_MI_COPY, NewState );                                                     
     pmenu->EnableItem( vID_MI_DELETE, NewState && ( SW_TYPE_3D != scheme_type) );                   
                                                                                                     
     ActivateSelectAllTools( NewState && ( SW_TYPE_3D == scheme_type ) );                            
}                                                                                                    









// ----------------------------------------------------------------------------
//                         MainWindow::Activate3DSpecialEditTools()
// ............................................................................
//
//  changes the activity state of the menu items specially for editing
//  3D schemes
//
// ----------------------------------------------------------------------------

void MainWindow::Activate3DSpecialEditTools( const BOOL NewState )
{
     Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT );                         

     pmenu->EnableItem( vID_MI_CHANGE_COMPT_DIMENSIONS, NewState );
     pmenu->EnableItem( vID_MI_CHANGE_SCHEME_TEMPERATURE, NewState );
     pmenu->EnableItem( vID_MI_GLOBAL_CONC_CHANGE, NewState );
}





// ----------------------------------------------------------------------------
//                         MainWindow::ActivateSelectAllTools( )
// ............................................................................
//
//  changes the activity state of the Select All Xompts/XferPath toolbar and menu items
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateSelectAllTools( const BOOL NewState )
{
     ActivateSelectAllXfersTools( NewState && ( PO_TYPE_XFER == GetClipObjectType() ) );
     ActivateSelectAllComptsTools( NewState && ( PO_TYPE_COMPT == GetClipObjectType() ));
}



// ----------------------------------------------------------------------------
//                         MainWindow::ActivateSelectAllXfersTools( )
// ............................................................................
//
//  changes the activity state of the Select All XferPaths toolbar and menu items
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateSelectAllXfersTools( const BOOL NewState )
{
     Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT ); 

     enum SCHEMWIN_TYPE  scheme_type = SW_TYPE_UNKNOWN;

      // if we are activating tools, see if cut/delete are allowed. 
     // they are ot allowed in 3D array scheme 

     if ( TRUE == NewState)
     {
          SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
      
          if ( NULL != pcurrent_win)
               scheme_type = pcurrent_win->GetSchemeWindow().GetSchemeType();
     }

     ToolBar.EnableItem( vID_TI_SELECT_ALL_XFERPATHS, NewState && ( SW_TYPE_3D == scheme_type) );
     pmenu->EnableItem( vID_MI_SELECT_ALL_XFERPATHS, NewState && ( SW_TYPE_3D == scheme_type) );
}





// ----------------------------------------------------------------------------
//                         MainWindow::ActivateSelectAllComptsTools( )
// ............................................................................
//
//  changes the activity state of the Select All Compts toolbar and menu items
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateSelectAllComptsTools( const BOOL NewState )
{
     Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT ); 

     enum SCHEMWIN_TYPE  scheme_type = SW_TYPE_UNKNOWN;

      // if we are activating tools, see if cut/delete are allowed. 
     // they are ot allowed in 3D array scheme 

     if ( TRUE == NewState)
     {
          SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
      
          if ( NULL != pcurrent_win)
               scheme_type = pcurrent_win->GetSchemeWindow().GetSchemeType();
     }

     ToolBar.EnableItem( vID_TI_SELECT_ALL_COMPTS, NewState && ( SW_TYPE_3D == scheme_type) );
     pmenu->EnableItem( vID_MI_SELECT_ALL_COMPTS, NewState && ( SW_TYPE_3D == scheme_type) );
}







// ----------------------------------------------------------------------------
//                         MainWindow::SelectAllCompartments()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void MainWindow::SelectAllCompartments()
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
      
     if ( ( NULL != pcurrent_win ) &&
          (SW_TYPE_3D == pcurrent_win->GetSchemeWindow().GetSchemeType() )
        )

          ( (ThreeDArraySchemeWindow*) (pcurrent_win->GetpSchemeWindow()))->SelectAllCompartmentObjects();
}





// ----------------------------------------------------------------------------
//                         MainWindow::SelectAllTransferPaths()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void MainWindow::SelectAllTransferPaths()
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
      
     if ( ( NULL != pcurrent_win ) &&
          (SW_TYPE_3D == pcurrent_win->GetSchemeWindow().GetSchemeType() )
        )
          ( (ThreeDArraySchemeWindow*) (pcurrent_win->GetpSchemeWindow()))->SelectAllTransferObjects();
}







// ----------------------------------------------------------------------------
//                         MainWindow::ActivateAddCompartment( )
// ............................................................................
//
//  changes the activity state of the Add Compartment button
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateAddCompartment( const BOOL NewState )
{
    Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT );

    ToolBar.EnableItem( vID_TI_ADD_COMPARTMENT, NewState );
    pmenu->EnableItem( vID_MI_ADD_COMPARTMENT, NewState );
}




// ----------------------------------------------------------------------------
//                         MainWindow::ActivatePaste( )
// ............................................................................
//
//  changes the activity state of the Add Compartment button
//
// ----------------------------------------------------------------------------

void MainWindow::ActivatePaste( BOOL NewState )
{
     USHORT text_id     = vID_STR_PASTE_TEXT; 
     USHORT menutext_id = vID_STR_PASTE_TEXT_MENU; 
     USHORT bitmap_id   = vID_BITMAP_PASTE; 
     USHORT helptext_id = vID_STR_PASTE_HELPTEXT; 

    if ( TRUE == NewState && pclip_object != NULL )
    {
         SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

         switch ( pcurrent_win->GetSchemeWindow().GetSchemeType() )
         {

         case SW_TYPE_COMPARTMENT :

              {
                   switch ( pclip_object->GetType() )
                   {
                   case PO_TYPE_COMPT :
          
                         bitmap_id    = vID_BITMAP_PASTE_COMPT;
                         text_id      = vID_STR_PASTE_COMPT_TEXT;
                         menutext_id  = vID_STR_PASTE_COMPT_TEXT_MENU;
                         helptext_id  = vID_STR_PASTE_COMPT_HELPTEXT;
          
                         break;
          
          
                   case PO_TYPE_XFER :
          
                         bitmap_id    =  vID_BITMAP_PASTE_XFER;
                         text_id      =  vID_STR_PASTE_XFER_TEXT;
                         menutext_id  =  vID_STR_PASTE_XFER_TEXT_MENU;
                         helptext_id  =  vID_STR_PASTE_XFER_HELPTEXT; 
          
                         break;


                   default: 

                        ASSERT( 0 );
                        break;
          
                   }
               }
               break;


         case SW_TYPE_3D :

              {
                   switch ( pclip_object->GetType() )
                   {
                   case PO_TYPE_COMPT :
          
                         bitmap_id    = vID_BITMAP_FILL_COMPT;
                         text_id      = vID_STR_FILL_COMPT_TEXT;
                         menutext_id  = vID_STR_FILL_COMPT_TEXT_MENU;
                         helptext_id  = vID_STR_FILL_COMPT_HELPTEXT;
          
                         break;
          
          
                   case PO_TYPE_XFER :
          
                         bitmap_id    =  vID_BITMAP_FILL_XFER;
                         text_id      =  vID_STR_FILL_XFER_TEXT;
                         menutext_id  =  vID_STR_FILL_XFER_TEXT_MENU;
                         helptext_id  =  vID_STR_FILL_XFER_HELPTEXT; 
          
                         break;

                   default: 

                        ASSERT( 0 );
                        break;
          
                   }

                   ActivateSelectAllXfersTools( PO_TYPE_XFER == pclip_object->GetType() );
                   ActivateSelectAllComptsTools( PO_TYPE_COMPT == pclip_object->GetType() );


               }
               break;


         default :

              ASSERT( 0 );
              break;
         }

    }

    ToolBar.SetItemBitmap( vID_TI_PASTE, Bitmap(ResId( bitmap_id ) ) );
    ToolBar.SetItemText( vID_TI_PASTE, String( ResId( text_id ) ) );
    ToolBar.SetHelpText( vID_TI_PASTE, String( ResId( helptext_id ) ) ); 
    ToolBar.EnableItem( vID_TI_PASTE, NewState && (pclip_object != NULL ) );

    Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT );

    pmenu->SetItemText( vID_MI_PASTE, String( ResId( menutext_id ) ) );
    pmenu->SetHelpText( vID_MI_PASTE, String( ResId( helptext_id ) ) ); 
    pmenu->EnableItem( vID_MI_PASTE, NewState && (pclip_object != NULL ) );
}






// ----------------------------------------------------------------------------
//                  MainWindow::ActivateAddTransferObject( )
// ............................................................................
//
//  changes the activity state of the Add Transfer Object button
//
// ----------------------------------------------------------------------------

void MainWindow::ActivateAddTransferObject( const BOOL NewState )
{
    Menu* pmenu = GetpMainApp()->GetAppMenu()->GetPopupMenu( vID_MI_EDIT );

    ToolBar.EnableItem( vID_TI_ADD_PATH, NewState );
    pmenu->EnableItem( vID_MI_ADD_PATH, NewState );
}



// ----------------------------------------------------------------------------
//                         MainWindow::AddTransferPath( )
// ............................................................................
//
//  determines active MDIWindow and adds a new transfer path to it 
//
// ----------------------------------------------------------------------------

void MainWindow::AddTransferPath( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          pcurrent_win->GetSchemeWindow().StartAddTransferObject( );
     }
}





// ----------------------------------------------------------------------------
//                         MainWindow::DeleteObject( )
// ............................................................................
//
//  determines active MDIWindow and deletes currently selected object in it
//
// ----------------------------------------------------------------------------

void MainWindow::DeleteObject( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          pcurrent_win->GetSchemeWindow().DeleteSelectedObject( );
     }
}





// ----------------------------------------------------------------------------
//                         MainWindow::ChangeCompartmentDimensions( )
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void MainWindow::ChangeCompartmentDimensions( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          ASSERT ( pcurrent_win->GetSchemeWindow().GetSchemeType() == SW_TYPE_3D );

          pcurrent_win->GetSchemeWindow().ChangeCompartmentDimensions();
     }
}




// ----------------------------------------------------------------------------
//                         MainWindow::GlobalSpeciesConcChange()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void MainWindow::GlobalSpeciesConcChange()
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          ASSERT ( pcurrent_win->GetSchemeWindow().GetSchemeType() == SW_TYPE_3D );

          pcurrent_win->GetSchemeWindow().ChangeSpeciesConcentration();
     }
}






// ----------------------------------------------------------------------------
//                         MainWindow::ChangeSchemeTemperature( )
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void MainWindow::ChangeSchemeTemperature()
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          ASSERT ( pcurrent_win->GetSchemeWindow().GetSchemeType() == SW_TYPE_3D );

          pcurrent_win->GetSchemeWindow().ChangeSchemeTemperature();
     }
}








// ----------------------------------------------------------------------------
//                         MainWindow::CopyObject( )
// ............................................................................
//
//  determines active MDIWindow and copies currently selected object into it
//
// ----------------------------------------------------------------------------

void MainWindow::CopyObject( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );

     if ( pcurrent_win )
     {
          if ( pcurrent_win->GetSchemeWindow().CopySelectedObject( pclip_object ) )
          {

               switch ( pcurrent_win->GetSchemeWindow().GetSchemeType() )
               {

               case SW_TYPE_3D:

                    ActivatePaste( pcurrent_win->GetSchemeWindow().IsObjectSelected() );
                    ActivateSelectAllTools( TRUE );
                    break;


               case SW_TYPE_COMPARTMENT:

                    ActivatePaste( TRUE );
                    break;

               default:
                    break;

               }
          }
     }
}





// ----------------------------------------------------------------------------
//                         MainWindow::PasteObject( )
// ............................................................................
//
//  determines active MDIWindow and pastes currently selected object into it
//
// ----------------------------------------------------------------------------

void MainWindow::PasteObject( )
{
     SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) (GetpMainApp()->GetActiveWindow() );
     

     if ( pcurrent_win )
     {
          pcurrent_win->GetSchemeWindow().PasteObject( pclip_object );
     }
}




// ----------------------------------------------------------------------------
//                         MainWindow::Zoom( )
// ............................................................................
//
//  determines active MDIWindow and changes the drawing scale within it
//
// ----------------------------------------------------------------------------

void MainWindow::Zoom( UINT16 ZoomMode )
{
     // set change in scale base on selected mode

    Fraction delta;

    switch ( ZoomMode )
    {

    case vID_TI_ZOOMOUT :
    case vID_MI_ZOOMOUT :

         delta = ScalableWindow::DECREMENT_SCALE;
         break;


    case vID_TI_ZOOMIN :
    case vID_MI_ZOOMIN  :

         delta = ScalableWindow::INCREMENT_SCALE;
         break;


    default :
    case vID_TI_ZOOM100 :
    case vID_MI_ZOOM100 :

         delta = ScalableWindow::FULL_SCALE;
         break;
    }

    SchemeFrameWindow* pcurrent_win = (SchemeFrameWindow*) GetpMainApp()->GetActiveWindow();

    Fraction scale = pcurrent_win->GetDrawingScale();

    if ( vID_TI_ZOOM100 == ZoomMode || vID_MI_ZOOM100 == ZoomMode  )
         scale = delta;
    else
         scale *= delta;

    pcurrent_win->ChangeDrawingScale( scale );
    aHelpStatus.SetText( GetHelpText( ) );
    ActivateZoomTools( TRUE );
}




// ----------------------------------------------------------------------------
//                         MainWindow::ToolBarSelect( )
// ............................................................................
//
//  handler for ToolBar select events
//
// ----------------------------------------------------------------------------

void MainWindow::ToolBarSelect( ToolBox* pBox )
{
     ExecuteAction( pBox->GetCurItemId() );
}







// ----------------------------------------------------------------------------
//                         MainWindow::ExecuteAction( )
// ............................................................................
//
//  called by ToolBar handlerand by main menu handler to initiate selected 
//  events
//
// ----------------------------------------------------------------------------

void MainWindow::ExecuteAction( UINT16 ID )
{
     switch ( ID )
     {
          case vID_TI_ADD_COMPARTMENT :
          case vID_MI_ADD_COMPARTMENT :

               AddCompartment( );
               break;


          case vID_TI_ADD_PATH :
          case vID_MI_ADD_PATH :

               AddTransferPath( );
               break;


          case vID_TI_DELETE :
          case vID_MI_DELETE :

               DeleteObject( );
               break;


          case vID_TI_CUT :
          case vID_MI_CUT :

               CopyObject( );
               DeleteObject( );
               break;


          case vID_TI_COPY :
          case vID_MI_COPY :

               CopyObject( );
               break;


          case vID_TI_PASTE :
          case vID_MI_PASTE :

               PasteObject( );
               break;

          case vID_TI_ZOOMOUT :
          case vID_TI_ZOOMIN  :
          case vID_TI_ZOOM100 :
          case vID_MI_ZOOM100 :
          case vID_MI_ZOOMOUT :
          case vID_MI_ZOOMIN  :

               Zoom( ID );
               break;


          case vID_TI_SELECT_ALL_COMPTS    :
          case vID_MI_SELECT_ALL_COMPTS    :

               SelectAllCompartments();
               break;

          case vID_MI_SELECT_ALL_XFERPATHS :
          case vID_TI_SELECT_ALL_XFERPATHS :

               SelectAllTransferPaths();
               break;

          case vID_MI_CHANGE_COMPT_DIMENSIONS :

               ChangeCompartmentDimensions();
               break;

          case vID_MI_CHANGE_SCHEME_TEMPERATURE :

               ChangeSchemeTemperature();
               break;

          case vID_MI_GLOBAL_CONC_CHANGE :

               GlobalSpeciesConcChange();
               break;
     }
}




// ----------------------------------------------------------------------------
//                         MainWindow::ToolBarHighlight( )
// ............................................................................
//
//  Set help text appropriately
//
// ----------------------------------------------------------------------------

void MainWindow::ToolBarHighlight( ToolBox* pBox )
{
     String text = pBox->GetHelpText( pBox->GetCurItemId( ) );

     if ( ! text )
          GetStatusBar( ).SetText( GetHelpText( ) );
     else
          GetStatusBar( ).SetText( text );

    GetpMainApp()->SetWindowUnderPointer( NULL );
}



// ----------------------------------------------------------------------------
//                             MainWindow::Resize( )
// ............................................................................
//
//  recalc size of aHelpStatus
//
// ----------------------------------------------------------------------------

void MainWindow::Resize( )
{
    Size   aSize = GetOutputSizePixel();

    USHORT height = aHelpStatus.GetOutputSizePixel().Height();

    aHelpStatus.SetPosSizePixel( Point( 0, aSize.Height() - height ),
                             Size( aSize.Width(), height ) );

    USHORT width = ToolBar.CalcWindowSizePixel().Width();

    ToolBar.SetPosSizePixel( Point( 0, 0 ),
                          Size( width, GetOutputSizePixel().Height( ) ) );

    active_area = Rectangle( width, 0, aSize.Width(), aSize.Height() - height );
}




// ----------------------------------------------------------------------------
//                             MainWindow::Close()
// ............................................................................
//
// ----------------------------------------------------------------------------

BOOL MainWindow::Close()
{
    if ( GetpMainApp()->CloseAll( ) )
         return WorkWindow::Close();
    else
         return FALSE;
}



// ----------------------------------------------------------------------------
//                             MainWindow::MouseMove( )
// ............................................................................
//
//   Mouse move handler - receives and deals with all Mouse Move events in 
//   main window areas
// ----------------------------------------------------------------------------

void MainWindow::MouseMove( const MouseEvent & )
{
     // do nothing if we have not changed the window we are over

     if ( GetpMainApp()->GetWindowUnderPointer( ) == this )
          return;

     aHelpStatus.SetText( GetHelpText( ) );

     GetpMainApp()->SetWindowUnderPointer( this );
}


