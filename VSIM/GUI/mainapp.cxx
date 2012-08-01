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
//  MAINAPP.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the main Application behavior for
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

#include "mainapp.hxx"
#include "fileutil.hxx"
#include "logo.hxx"
#include "assert.hxx"
#include "events.hxx"
#include "rxnschem.hxx"
#include "create2d.hxx"

#endif


#include "mainmenu.hrc"
#include "strings.hrc"
#include "popup.hrc"
#include "fonts.hrc"


const UINT32 RESERVED_MEMORY_SIZE         = 8092;
const UINT16 MAX_NUM_RELOADABLE_FILENAMES = 9;
const UINT16 MAX_NUM_LOADED_FILENAMES     = 20;

// ----------------------------------------------------------------------------
//                             MainApp::MainApp()
// ............................................................................
//
//  constructor for the MainApp object
//
// ----------------------------------------------------------------------------

MainApp::MainApp() :
MDIApplication( ),
window_list(),
over_window( NULL ),
init_file_menu_item_count( 0 ),
init_window_menu_item_count( 0 ),
file_menu_sep_present( FALSE ),
window_menu_sep_present( FALSE ),
confirm_before_overwrite( TRUE ),
save_win_data( TRUE ),
preserved_mem_block( NULL )
{
}






// ----------------------------------------------------------------------------
//                      MainApp::MenuActivate()
// ............................................................................
//
//  activation handler for main menu - called whenever it is activated
//
// ----------------------------------------------------------------------------

long MainApp::MenuActivate( const Menu* pMenu )
{
     if ( GetHelp()->IsBalloonModeEnabled() )
          pMenu->SetItemText( vID_MI_HELPBALLOON, String( ResId( vID_STR_HIDE_BALLOON ) ) );
     else
          pMenu->SetItemText( vID_MI_HELPBALLOON, String( ResId( vID_STR_SHOW_BALLOON ) ) );

     // return zero to continue execution of activation hdl chain

     return 0;
}




// ----------------------------------------------------------------------------
//                 MainApp::RxnSchemeIsAlreadyLoaded()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 MainApp::RxnSchemeIsAlreadyLoaded( const String& rFileName )
{
     UINT16 count = window_list.Count();
    

     for ( UINT16 i = 0; i < count; i++ )
    {
         ScrollingMDIWindow* ptr = window_list.GetObject(i);

         if ( MDI_SCHEMEWINDOW == ptr->WindowType() )
         {
              if ( FilenamesMatch( rFileName, ( (SchemeFrameWindow*) ptr)->GetName() ) )
                   return TRUE;
         }
    }

    return FALSE;
}





// ----------------------------------------------------------------------------
//                 MainApp::GetWindowPtrFromId()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

ScrollingMDIWindow* MainApp::GetWindowPtrFromId( UINT16 Id )
{
     UINT16 count = window_list.Count();
    

     for ( UINT16 i = 0; i < count; i++ )
    {
         ScrollingMDIWindow* ptr =  window_list.GetObject(i);

         if ( Id  == ptr->GetMenuId() )
              return ptr;
    }

    ASSERT( 0 );
}




// ----------------------------------------------------------------------------
//                             MainApp::UserEvent()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  MainApp::UserEvent( ULONG nEvent, void* pEventData )
{
     switch ( nEvent )
     {
     case  vEVT_UPDATE_AFTER_ABORT : 

          break;


     case  vEVT_UPDATE_AFTER_SIMULATION :

          ((ReactionScheme*) pEventData)->UpdateSchemeAfterSimulation( );
          break;

     case vEVT_UPDATE_WINDOW_ASYNC      :

          ((Window*) pEventData)->Invalidate();
          ((Window*) pEventData)->Update();

          break;

     default :

          ASSERT( 0 );
          break;
     }
}





// ----------------------------------------------------------------------------
//                         MainApp::MenuHighlight(  )
// ............................................................................
//
//  This function updates the help text in the status area whenever the mouse
//  points to a menu item
//
// ----------------------------------------------------------------------------

long MainApp::MenuHighlight( const Menu* pMenu )
{
     String aText = pMenu->GetHelpText( pMenu->GetCurItemId( ) );

     if ( ! aText )
          GetMainWindow( )->GetStatusBar( ).SetText( GetAppWindow()->GetHelpText( ) );
     else
          GetMainWindow( )->GetStatusBar( ).SetText( String( aText ) );

     SetWindowUnderPointer( NULL );

     return TRUE;
}




// ----------------------------------------------------------------------------
//                         MainApp::LoadReactionScheme()
// ............................................................................
//
//  This function gets a file name form the user, creats a SchemeWindow
//  to contain the scheme to be loaded, then initializes that SchemeWindow
//  using data from the rxn file
//
// ----------------------------------------------------------------------------

void MainApp::LoadReactionScheme()
{
     String name;

     if ( OpenFileDialog( name, RXNSCHEME_FILETYPE ) )
          LoadReactionScheme( name );
}






// ----------------------------------------------------------------------------
//                         MainApp::LoadReactionScheme()
// ............................................................................
//
//  This function takes a filename as a passed parameter, creates a SchemeWindow
//  to contain the scheme to be loaded, then initializes that SchemeWindow
//  using data from the rxn file
//
// ----------------------------------------------------------------------------

void MainApp::LoadReactionScheme( const String& rFilename )
{
     if ( RxnSchemeIsAlreadyLoaded( rFilename ) )
     {
          FileAlreadyLoaded( rFilename );

          return;
     }

     enum SCHEMWIN_TYPE file_type = SchemeWindow::QueryRxnFileSchemeType( rFilename );

     if ( SW_TYPE_UNKNOWN == file_type )
     {
          InvalidInputFile( rFilename );
          return;
     }

     Wait( TRUE );

     SchemeFrameWindow* ptr = new SchemeFrameWindow( file_type );

     if ( ptr && ptr->IsInitialized() )
     {
          ChangeActiveWindow( ptr );

          if ( ptr->LoadScheme( rFilename ) )
          {
               ptr->Show();
               UpdateMenuActivation( ptr );
               AddToWindowList( ptr );
               RemoveNameFromReloadMenu( rFilename );

               GetMainWindow()->ActivateZoomTools( TRUE );
     
               if ( ( file_type == SW_TYPE_COMPARTMENT ) && 
                    ( ptr->GetReactionScheme().GetNumCompartments() > 1 ) 
                  )
                    GetMainWindow()->ActivateAddTransferObject( TRUE );
          }
          else
          {
               if ( ptr )
                    delete ptr;
          }

     }
     else
     {
          Wait( FALSE );
          OutOfMemory();

          if (ptr )
               delete ptr;
     }

     Wait( FALSE );
}




// ----------------------------------------------------------------------------
//                           MainApp::NewCompartmentScheme()
// ............................................................................
//
//  This function creates a new, unnamed CompartmentSchemeWindow
//
// ----------------------------------------------------------------------------

void MainApp::NewCompartmentScheme( )
{
     SchemeFrameWindow* ptr = new SchemeFrameWindow( SW_TYPE_COMPARTMENT );

     if ( ptr && ptr->IsInitialized() )
     {
          ChangeActiveWindow( ptr );
          ptr->Show();
          UpdateMenuActivation( ptr );

          AddToWindowList( ptr );

          GetMainWindow()->ActivateAddCompartment( TRUE );
          GetMainWindow()->ActivateZoomTools( TRUE );
     }
     else
     {
          OutOfMemory();

          if (ptr )
               delete ptr;
     }
}






// ----------------------------------------------------------------------------
//                           MainApp::New3DScheme()
// ............................................................................
//
//  This function creates a new, unnamed ThreeSchemeWindow
//
// ----------------------------------------------------------------------------

void MainApp::New3DScheme( )
{
     SchemeFrameWindow* ptr = new SchemeFrameWindow( SW_TYPE_3D );

     if ( ptr && ptr->IsInitialized() )
     {
          switch ( Create3DComptScheme( GetMainWindow(), ptr ) )
          {
          case C3D_RC_ACCEPT :

               ChangeActiveWindow( ptr );
               ptr->Show();
               UpdateMenuActivation( ptr );
               AddToWindowList( ptr );

               GetMainWindow()->ActivateZoomTools( TRUE );

               break;

          case C3D_RC_FAIL :

               OutOfMemory();
               delete ptr;

               break;

          case C3D_RC_CANCEL :
          
               delete ptr;

               break;

          default: 

               ASSERT( 0 );
               break;
          }
     }
     else
     {
          OutOfMemory();

          if (ptr)
               delete ptr;
     }
}







// ----------------------------------------------------------------------------
//                           MainApp::SaveSchemeUnderNewName()
// ............................................................................
//
//  This function saves the Active Scheme under a new name selected by the user
//
// ----------------------------------------------------------------------------

void MainApp::SaveSchemeUnderNewName()
{
     String name;

     if ( SaveAsFileDialog( name, RXNSCHEME_FILETYPE ) )
     {
          RemoveItemFromWindowMenu( ( (ScrollingMDIWindow*) GetActiveWindow() )->GetMenuId() );

          ( (SchemeFrameWindow*) GetActiveWindow() )->SetName( name );
          ( (SchemeFrameWindow*) GetActiveWindow() )->SaveScheme( );

          AddItemToWindowMenu( name + String( ResId( vID_STR_SCHEME_TITLE_SUFFIX) ) ); 
     }
}





// ----------------------------------------------------------------------------
//                         MainApp::MenuSelect(  )
// ............................................................................
//
//  This function handles all main menu selection events
//
// ----------------------------------------------------------------------------

long MainApp::MenuSelect( Menu* pMenu )
{
     USHORT id = pMenu->GetCurItemId();

     switch ( id )
     {
     case vID_MI_HELPBALLOON :

          if ( pMenu->GetItemText( id ) == String( ResId( vID_STR_HIDE_BALLOON ) ) )
          {
               GetHelp()->DisableBalloonMode();
               pMenu->SetItemText( vID_MI_HELPBALLOON, String( ResId( vID_STR_SHOW_BALLOON ) ) );
          }
          else
          {
               GetHelp()->EnableBalloonMode();
               pMenu->SetItemText( vID_MI_HELPBALLOON, String( ResId( vID_STR_HIDE_BALLOON ) ) );
          }

          break;


     case vID_MI_NEW_COMPARTMENT_SCHEME :

          NewCompartmentScheme();
          break;


     case vID_MI_NEW_3D_SCHEME :

          New3DScheme( );
          break;


     case vID_MI_LOAD_SCHEME:

          LoadReactionScheme();

          break;


     case vID_MI_CLOSE_SCHEME :

          ASSERT ( NULL != GetActiveWindow() );

          GetActiveWindow()->Close();

          break;


     case vID_MI_SAVE_SCHEME :

          ASSERT ( NULL != GetActiveWindow() );

          // is it named yet - if not get a name

          if ( !( (SchemeFrameWindow*) GetActiveWindow() )->GetReactionScheme().GetName() )
               SaveSchemeUnderNewName();
          else
               ( (SchemeFrameWindow*) GetActiveWindow() )->SaveScheme( );

          break;


     case vID_MI_SAVE_SCHEME_AS :

          ASSERT ( NULL != GetActiveWindow() );

          SaveSchemeUnderNewName();

          break;



     case vID_MI_EXIT :

            if ( CloseAll( ) )
                 Quit();

            break;


        case vID_MI_CASCADE :

            Cascade();
            break;

        case vID_MI_TILE :

            Tile();
            break;

        case vID_MI_HORIZONTAL :

            Horizontal();
            break;

        case vID_MI_VERTICAL :

            Vertical();
            break;

        case vID_MI_ARRANGE :
            Arrange();
            break;

        case vID_MI_CLOSEALL :

            CloseAll();

            GetMainWindow()->ActivateEditTools( FALSE );
            GetMainWindow()->ActivateZoomTools( FALSE );
            GetMainWindow()->ActivatePaste( FALSE );
            GetMainWindow()->ActivateAddCompartment( FALSE );
            GetMainWindow()->ActivateAddTransferObject( FALSE );
            GetMainWindow()->Activate3DSpecialEditTools( FALSE );                                     

            break;

        case vID_MI_ABOUT :

            ShowAboutDialog( GetMainWindow() );

            break;

       case vID_MI_USER_PREFERENCES :

            EditPreferencesDialog( GetMainWindow() );

            break;


        case MI_OVERVIEW  :
        case MI_TUTORIAL  :
        case MI_WINHELP   :
        case MI_MENUHELP  :
        case MI_INDEX     :

            GetHelp()->Start( ( ULONG ) id );
      
            break;


        case vID_MI_HELPHELP :

            GetHelp()->Start( HELP_HELPONHELP );

            break;


        case  vID_MI_CUT              :
        case  vID_MI_COPY             :
        case  vID_MI_PASTE            :
        case  vID_MI_DELETE           :
        case  vID_MI_ADD_COMPARTMENT  :
        case  vID_MI_ADD_PATH         :
        case  vID_MI_ZOOM100          :
        case  vID_MI_ZOOMOUT          :
        case  vID_MI_ZOOMIN           :
        case  vID_MI_SELECT_ALL_COMPTS    :
        case  vID_MI_SELECT_ALL_XFERPATHS :
        case  vID_MI_CHANGE_COMPT_DIMENSIONS :
        case  vID_MI_CHANGE_SCHEME_TEMPERATURE :
        case  vID_MI_GLOBAL_CONC_CHANGE :

            GetMainWindow()->ExecuteAction( id );

            break;

       default :

            // did user select a filename to reload?

            if ( (id >= vID_MI_PREV_FILE_1) &&
                 (id < (vID_MI_PREV_FILE_1 + MAX_NUM_RELOADABLE_FILENAMES ) )
               )
            {
                 LoadReactionScheme( pMenu->GetItemText( id ) );
                 pMenu->RemoveItem( pMenu->GetItemPos( id ) );
                 break;
            }

            if ( (id >= vID_MI_LOADED_FILE_1) &&
                 (id < (vID_MI_LOADED_FILE_1 + MAX_NUM_LOADED_FILENAMES ) )
               )
            {
                 ScrollingMDIWindow* p_win = GetWindowPtrFromId( id );  

                 ChangeActiveWindow( p_win ); 

                 if ( p_win->IsMinimized() )
                      p_win->Restore();

            }
            else
                 return FALSE;
    }

    return TRUE;
}




// ----------------------------------------------------------------------------
//                         MainApp::EnableAppMenu(  )
// ............................................................................
//
//  This function serves to activate/deactivate the entire application menu;
//  useful on the Macintosh mainly to prevent menu selection during
//  initialization.
//
// ----------------------------------------------------------------------------

void    MainApp::EnableAppMenu( BOOL State )
{
     GetAppMenu( )->EnableItem( vID_MI_FILE, State );
     GetAppMenu( )->EnableItem( vID_MI_EDIT, State );
     GetAppMenu( )->EnableItem( vID_MI_WINDOW, State );
     GetAppMenu( )->EnableItem( vID_MI_HELP, State );
}


// this is for reserving a block of memory  at startup time
// that we can free up if necessary to create the out-of-memory dialog





// ----------------------------------------------------------------------------
//                         MainApp::Main(  )
// ............................................................................
//
//  The main application function. We load the application menu, create the
//  application windowntosh mainly to prevent menu selection during
//  initialization.
//
// ----------------------------------------------------------------------------

void MainApp::Main( int, char*[] )
{
    preserved_mem_block = new UCHAR8[ RESERVED_MEMORY_SIZE ];

    ChangeAppFont( Font( ResId(vID_FNT_APPLICATION) ) );
    EnableSVLook();

     // load the main menu

     MenuBar app_menu( ResId( vID_MENU_MAIN ) );
     ChangeAppMenu( &app_menu );

     init_file_menu_item_count = app_menu.GetPopupMenu( vID_MI_FILE )->GetItemCount();
     init_window_menu_item_count = app_menu.GetPopupMenu( vID_MI_WINDOW )->GetItemCount();

     // create and initialize the main window

     MainWindow main_window( NULL, WB_APP | WB_STDWORK | WB_SVLOOK );

     EnableAppMenu( FALSE );

     // tell aAppMenu to send messages to MAinApp::MenuHandler and MenuSelect

     app_menu.PushHighlightHdl( LINK( this, MainApp, MenuHighlight ) );
     app_menu.PushSelectHdl( LINK( this, MainApp, MenuSelect) );
     app_menu.PushActivateHdl( LINK( this, MainApp, MenuActivate ) );

     ShowLogo( GetMainWindow() );

     Help help;

     ChangeHelp( &help );
     EnableHelp( HELPMODE_ALL );

     InitializeFromConfigFile();

     UpdateMenuActivation( NULL );

     // display the main window

     main_window.Show();

     // ready to go - enable menu and execute application

     EnableAppMenu( TRUE );

     Execute();

     SaveStateToConfigFile();
}

// instantiate MainApp

MainApp aMainApp;


MainApp* GetpMainApp( )
{
     return &aMainApp;
}




// ----------------------------------------------------------------------------
//                             MainApp::OutOfMemory()
// ............................................................................
//
//  called when new operator fails
//
// ----------------------------------------------------------------------------
void MainApp::OutOfMemory(  )
{
     // free up some memory that we set aside at startup

     delete preserved_mem_block;

     // try to create a dialog

     WarningBox* pbox = new WarningBox( NULL, ResId( vID_POPUP_OUT_OF_MEMORY ) );

     if ( pbox )
          pbox->Execute( );
     else
          Sound::Beep( SOUND_WARNING );
}





// ----------------------------------------------------------------------------
//                             MainApp::ForeignInputFile( )
// ............................................................................
//
//  called when user attempts to load a rxn file from another operating system
//
// ----------------------------------------------------------------------------

void MainApp::ForeignInputFile( const String& rFilename )
{
     String text = String( ResId( vID_STR_FOREIGN_INPUT_FILE_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FOREIGN_INPUT_FILE_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}




// ----------------------------------------------------------------------------
//                             MainApp::InputFileCorrupt( )
// ............................................................................
//
//  called when an unexpected object idtype  is found upon reading an input file
//
// ----------------------------------------------------------------------------

void MainApp::InputFileCorrupt( const String& rFilename )
{
     String text = String( ResId( vID_STR_INPUT_FILE_CORRUPT_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_INPUT_FILE_CORRUPT_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}



// ----------------------------------------------------------------------------
//                             MainApp::InvalidInputFile( )
// ............................................................................
//
//  called when user attempts to load a file that is not a VSIM rxn file
//
// ----------------------------------------------------------------------------

void MainApp::InvalidInputFile( const String& rFilename )
{
     String text = String( ResId( vID_STR_INVALID_INPUT_FILE_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_INVALID_INPUT_FILE_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}



// ----------------------------------------------------------------------------
//                             MainApp::InvalidInputFile( )
// ............................................................................
//
//  called when user attempts to load a file that is already in memory
//
// ----------------------------------------------------------------------------

void MainApp::FileAlreadyLoaded ( const String& rFilename )
{
     String text = String( ResId( vID_STR_FILE_IN_MEMORY_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FILE_IN_MEMORY_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}






// ----------------------------------------------------------------------------
//                             MainApp::FileOpenError( )
// ............................................................................
//
//  called when an error occurs trying to open a file
//
// ----------------------------------------------------------------------------

void MainApp::FileOpenError( const String& rFilename )
{
     String text = String( ResId( vID_STR_FILE_OPEN_ERROR_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FILE_OPEN_ERROR_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}






// ----------------------------------------------------------------------------
//                             MainApp::FileSeekError( )
// ............................................................................
//
//  called when an error occurs trying to move to a new position within a file
//
// ----------------------------------------------------------------------------

void MainApp::FileSeekError( const String& rFilename )
{
     String text = String( ResId( vID_STR_FILE_SEEK_ERROR_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FILE_SEEK_ERROR_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}





// ----------------------------------------------------------------------------
//                             MainApp::FileReadError( )
// ............................................................................
//
//  called when an error occurs trying to read a file
//
// ----------------------------------------------------------------------------

void MainApp::FileReadError( const String& rFilename )
{
     String text = String( ResId( vID_STR_FILE_READ_ERROR_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FILE_READ_ERROR_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}







// ----------------------------------------------------------------------------
//                             MainApp::FileWriteError( )
// ............................................................................
//
//  called when an error occurs trying to write to a file
//
// ----------------------------------------------------------------------------


void MainApp::FileWriteError( const String& rFilename )
{
     String text = String( ResId( vID_STR_FILE_WRITE_ERROR_1 ) );
     text += rFilename;
     text += String( ResId( vID_STR_FILE_WRITE_ERROR_2 ) );

     WarningBox( GetAppWindow( ), WB_OK | WB_DEF_OK, text ).Execute( );
}




// ----------------------------------------------------------------------------
//                          MainApp::OKtoOverwriteFile(( )
// ............................................................................
// 
//  asks user if it OK to overwrite the named file
//  returns TRUE if user say OK, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 MainApp::OKtoOverwriteFile( const String& rFilename )
{
     // return immediately with TRUE if user does not want to check 
     // for file over-writes            

     if ( ! GetpMainApp()->ConfirmBeforeOverwrite() )
          return TRUE;

     // build string for display

     String message = String( ResId( vID_STR_OVERWRITE_1 ) ) +
                      DirEntry( rFilename ).GetName() +
                      String( ResId( vID_STR_OVERWRITE_2 ) );

     return ( RET_YES == QueryBox( GetpMainApp()->GetAppWindow(), WB_YES_NO | WB_DEF_NO, message ) .Execute( ) );
}





//--------------------------------------------------------------------------
//                 MainApp::CascadeWindowOnParent( )
//..........................................................................
//
// function to reposition a child window on its owner in a "cascade" sort
// of positioning. We first try to offset  left left,
// then right from right, then upper from upper, then lower from lower
// The offsets are given in percent of the owner window dimensions
//--------------------------------------------------------------------------

void MainApp::CascadeWindowOnParent( Window* pChild, Window* pParent )
{
     const USHORT VERTICAL_OFFSET  = 10;
     const USHORT HORIZ_OFFSET     = 10;
     const USHORT MIN_SPACE_PIXELS = 20;

     short desired_h_offset, desired_v_offset;

     Size dlg_size = pChild->GetSizePixel( );
     Size owner_size = pParent->GetSizePixel( );
     Point owner_pos = pParent->GetPosPixel( );

     short dlg_width = dlg_size.Width ( );
     short dlg_height = dlg_size.Height ( );
     short owner_width = owner_size.Width ( );
     short owner_height = owner_size.Height ( );
     short owner_x = owner_pos.X ( );
     short owner_y = owner_pos.Y ( );

     Size screen = System::GetScreenSizePixel( );
     short screen_width = screen.Width ( );
     short screen_height = screen.Height ( );

     short new_x, new_y;

     desired_h_offset = ( HORIZ_OFFSET * owner_width ) / 100;
     desired_v_offset = ( VERTICAL_OFFSET * owner_height ) / 100;

     // calculate a trial position for new location of dlg

     new_x = owner_x + desired_h_offset;

     // off the left side of screen ?? move to right a bit

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = MIN_SPACE_PIXELS;

     // if we are off the right end of the screen try offset from right side
     // of owner

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = owner_x + owner_width - desired_h_offset - dlg_width;

     // if we are still off the right side then move over a bit from right edge

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = screen_width - MIN_SPACE_PIXELS - dlg_width;

     /* if we are back off the left edge of screen then center on screen */

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = ( screen_width - dlg_width ) / 2;

     /* now try the vertical offsets : + because + moves downward           */

     new_y = owner_y + desired_v_offset;
     // are we off the bottom of the screen ?? if so try to offset up from
     // bottom of owner

     if ( ( new_y + dlg_height ) > ( screen_height - MIN_SPACE_PIXELS ) )
          new_y = owner_y + owner_height - dlg_height - desired_v_offset;
     // if we are still off the bottom of screen then move up a bit from bottom

     if ( ( new_y + dlg_height ) > ( screen_height - MIN_SPACE_PIXELS ) )
          new_y = screen_height - MIN_SPACE_PIXELS - dlg_height;

     /* are we now off the top of the screen ?                              */

     // if so then move down a bit from top

     if ( new_y < MIN_SPACE_PIXELS )
          new_y = MIN_SPACE_PIXELS;

     pChild->ChangePosPixel( Point( new_x, new_y ) );

     return;
}




//--------------------------------------------------------------------------
//                 MainApp::CenterWindowOnParent( )
//..........................................................................
//
// function to reposition a child window on its owner by centering
// of positioning. We first try to offset  left left,
// then right from right, then upper from upper, then lower from lower
// The offsets are given in percent of the owner window dimensions
//
//--------------------------------------------------------------------------

void MainApp::CenterWindowOnParent( Window* pChild, Window* pParent )
{
     const USHORT MIN_SPACE_PIXELS = 20;

     Size  screen = System::GetScreenSizePixel( );
     short screen_width = screen.Width ( );
     short screen_height = screen.Height ( );

     Size  owner_size;
     Point owner_pos;

     ASSERT( pChild != NULL );    // a logic error - should know that it's not NULL before getting here

     Size dlg_size = pChild->GetSizePixel( );

     // deal with NULL Parent - here a NULL value means that the Screen is the parent so we
     // center on that

     if ( ! pParent )
     {
          owner_size = screen;   // center on screen owner_pos = Point( 0, 0);
     }
     else
     {
          owner_size = pParent->GetSizePixel( );
          owner_pos = pParent->GetPosPixel( );
     }


     short dlg_width = dlg_size.Width ( );
     short dlg_height = dlg_size.Height ( );
     short owner_width = owner_size.Width ( );
     short owner_height = owner_size.Height ( );
     short owner_x = owner_pos.X ( );
     short owner_y = owner_pos.Y ( );

     short new_x, new_y;

     //  recalc coords for Dialog
     // center horizontally first

     new_x = ( owner_width - dlg_width ) / 2 + owner_x;

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = MIN_SPACE_PIXELS;

     // are we off right side of screen ??
     // if so then move back such that we are MIN_SPACE_PIXELS from right edge
     // of screen

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = screen_width - MIN_SPACE_PIXELS - dlg_width;

     // now center vertically

     new_y = ( owner_height - dlg_height ) / 2 + owner_y;
     if ( new_y < MIN_SPACE_PIXELS )
          new_y = MIN_SPACE_PIXELS;

     // are we off bottom ?

     if ( new_y + dlg_height > screen_height - MIN_SPACE_PIXELS )
          new_y = screen_height - MIN_SPACE_PIXELS - dlg_height;

     // done -  move window

     pChild->ChangePosPixel( Point( new_x, new_y ) );

     return;
}






//--------------------------------------------------------------------------
//                 MainApp::InitializeFromConfigFile()
//..........................................................................
//
// function to initialize the application from a configuration file on
// disk that contains settings saved from the previous session of the
// application.
//
//--------------------------------------------------------------------------

void MainApp::InitializeFromConfigFile()
{
     Config config;

     // deal with settings in the 'App' group

     config.SetGroup( String( ResId( vID_STR_APPGROUP ) ) );

     // get the saved window position and size

     Point pos;
     Size  size;

     pos.X()       = config.ReadKey( String( ResId( vID_STR_APPWIN_X ) ) );
     pos.Y()       = config.ReadKey( String( ResId( vID_STR_APPWIN_Y ) ) );
     size.Width()  = config.ReadKey( String( ResId( vID_STR_APPWIN_WIDTH ) ) );
     size.Height() = config.ReadKey( String( ResId( vID_STR_APPWIN_HEIGHT ) ) );


     // only reset position and size if the values are not equal to 0
     // and the origin is on screen 
     // otherwise set in center of screen and set to default size

     Size screen = System::GetScreenSizePixel( );

     if ( (String( ResId( vID_STR_ON ) ) == config.ReadKey( String( ResId( vID_STR_KEEP_WIN_DATA ) ) )) &&
          ( size.Width() != 0 ) && 
          ( size.Height() != 0 ) &&
          ( Rectangle( Point(), screen ).IsInside( pos ) )
        )
          GetAppWindow()->SetPosSizePixel( pos, size );
      else
      {

          size.Width()  = ( 3 * screen.Width() )/4;
          size.Height() = ( 3 * screen.Height() )/4;

          GetAppWindow()->ChangeSizePixel( size );
          CenterWindowOnParent( GetAppWindow(), NULL );
      }

     // read in previous file names if any

     USHORT num_saved_filenames = 0;

     num_saved_filenames = config.ReadKey( String( ResId( vID_STR_NUM_PREV_FILENAMES )));

     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_FILE );

     for ( USHORT i = 1; i <= num_saved_filenames; i++ )
     {
          String name = config.ReadKey( String( ResId( vID_STR_PREV_FILENAME_STUB )) +
                          String( i ) );

           UINT16 id = vID_MI_PREV_FILE_1 - 1 + i;

           if ( name.Len() )
           {
                if ( !file_menu_sep_present )
                {
                     pmenu->InsertSeparator();
                     file_menu_sep_present = TRUE;
                     init_file_menu_item_count++;
                }

                pmenu->InsertItem( id, name );
                pmenu->SetHelpText( id, String( ResId( vID_STR_RELOAD_HELP )) );
           }
     }

     p_rxn_file_path  = new String;
     p_txt_file_path  = new String;
     p_any_file_path  = new String;
     p_prf_file_path  = new String;
     p_rat_file_path  = new String;

     *p_rxn_file_path  = config.ReadKey( String( ResId( vID_STR_RXN_FILE_PATH ) ) ); 
     *p_txt_file_path  = config.ReadKey( String( ResId( vID_STR_TXT_FILE_PATH ) ) ); 
     *p_any_file_path  = config.ReadKey( String( ResId( vID_STR_ANY_FILE_PATH ) ) ); 
     *p_prf_file_path  = config.ReadKey( String( ResId( vID_STR_TXT_TPROFILE_PATH ) ) );
     *p_rat_file_path  = config.ReadKey( String( ResId( vID_STR_TXT_RPROFILE_PATH ) ) ); 


     ValidateDefaultDirString( p_rxn_file_path, vID_STR_DEFAULT_RXN_SUBDIR ); 
     ValidateDefaultDirString( p_txt_file_path, vID_STR_DEFAULT_TXT_SUBDIR ); 
     ValidateDefaultDirString( p_any_file_path, vID_STR_DEFAULT_ANY_SUBDIR ); 
     ValidateDefaultDirString( p_prf_file_path, vID_STR_DEFAULT_PRF_SUBDIR ); 
     ValidateDefaultDirString( p_rat_file_path, vID_STR_DEFAULT_RAT_SUBDIR ); 

     // set balloon mode to saved state 

     if ( String( ResId( vID_STR_ON ) ) == config.ReadKey( String( ResId( vID_STR_BALLOON_MODE ) ) ) )
          GetHelp()->EnableBalloonMode();
     else
          GetHelp()->DisableBalloonMode();


     if (String( ResId( vID_STR_OFF ) ) == config.ReadKey( String( ResId( vID_STR_CONFIRM_OVERWRITE ) )) )       
          confirm_before_overwrite = FALSE;
     else
          confirm_before_overwrite = TRUE;
}





//--------------------------------------------------------------------------
//                 MainApp::ValidateDefaultDirString()
//..........................................................................
//
//   validates the setting of the default directory strings, naking sure they
//   exist; if not then an existing default directory is used 
//
//--------------------------------------------------------------------------

void MainApp::ValidateDefaultDirString( String* pStr, UINT16 InitStrID )
{

     // test for a valid, existing directory 
     // if not there o then build one 

     if ( ( 0 == pStr->Len() ) || ( !DirEntry( *pStr ).Exists() ) 
         )
     {
          DirEntry base_app( DirEntry( GetpMainApp()->GetAppFileName() ).GetPath() );
     
          DirEntry base_path( base_app.GetPath() );
     
          base_path += DirEntry( String( ResId( InitStrID ) ) );
     
          if ( base_path.Exists() )
               *pStr = base_path.GetFull();
          else
               *pStr = base_app.GetPath().GetFull();
     }
}




//--------------------------------------------------------------------------
//                 MainApp::SaveStateToConfigFile()
//..........................................................................
//
// function to save to a configuration file on disk
// various settings to be used in the next session of the
// application.
//
//--------------------------------------------------------------------------

void MainApp::SaveStateToConfigFile()
{
     Config config;

     // save the window position and size

     config.SetGroup( String( ResId( vID_STR_APPGROUP ) ) );

     Point pos;
     Size  size;

     if (! GetAppWindow()->IsMaximized() )
     {
          pos = GetAppWindow()->GetPosPixel();
          size = GetAppWindow()->GetSizePixel();
     }

     config.WriteKey( String( ResId( vID_STR_APPWIN_X ) ), pos.X() );
     config.WriteKey( String( ResId( vID_STR_APPWIN_Y ) ), pos.Y() );
     config.WriteKey( String( ResId( vID_STR_APPWIN_WIDTH ) ), size.Width() );
     config.WriteKey( String( ResId( vID_STR_APPWIN_HEIGHT ) ), size.Height() );

     // save the previous rxn file names if any

     // first remove any existing filenames in config file

     USHORT num_saved_filenames = 0;

     num_saved_filenames = config.ReadKey( String( ResId( vID_STR_NUM_PREV_FILENAMES )));

     for ( USHORT i = 1; i <= num_saved_filenames; i++ )
          config.DeleteKey( String( ResId( vID_STR_PREV_FILENAME_STUB )) +
                          String( i ) );

     // now write the new ones

     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_FILE );

     // calculate number of items to save. If greater than zero then save to config file

     USHORT current_item_count = pmenu->GetItemCount();

     USHORT num_items_to_save = current_item_count - init_file_menu_item_count;

     config.WriteKey( String( ResId( vID_STR_NUM_PREV_FILENAMES )), num_items_to_save );

     for ( i = 1; i <= num_items_to_save; i++ )
     {
         config.WriteKey( String( ResId( vID_STR_PREV_FILENAME_STUB )) +
                          String( i ),
                          pmenu->GetItemText( pmenu->GetItemId( current_item_count - num_items_to_save - 1 + i ))) ;
     }

     config.WriteKey( String( ResId( vID_STR_RXN_FILE_PATH ) ), *p_rxn_file_path );
     config.WriteKey( String( ResId( vID_STR_TXT_FILE_PATH ) ), *p_txt_file_path );
     config.WriteKey( String( ResId( vID_STR_ANY_FILE_PATH ) ), *p_any_file_path );
     config.WriteKey( String( ResId( vID_STR_TXT_TPROFILE_PATH ) ), *p_prf_file_path );
     config.WriteKey( String( ResId( vID_STR_TXT_RPROFILE_PATH ) ), *p_rat_file_path ); 

     delete p_rxn_file_path;
     delete p_txt_file_path;
     delete p_any_file_path;
     delete p_prf_file_path;
     delete p_rat_file_path;


     if ( GetHelp()->IsBalloonModeEnabled() )
          config.WriteKey( String( ResId( vID_STR_BALLOON_MODE ) ),  String( ResId( vID_STR_ON ) )  );
     else
          config.WriteKey( String( ResId( vID_STR_BALLOON_MODE ) ),  String( ResId( vID_STR_OFF ) )  );


     if ( ConfirmBeforeOverwrite() )
          config.WriteKey( String( ResId( vID_STR_CONFIRM_OVERWRITE ) ),  String( ResId( vID_STR_ON ) )  );
     else
          config.WriteKey( String( ResId( vID_STR_CONFIRM_OVERWRITE ) ),  String( ResId( vID_STR_OFF ) )  );
}







//--------------------------------------------------------------------------
//                 MainApp::RemoveNameFromWindowMenu()
//..........................................................................
//
// function to remove a filename from the 'Window' menu reloadable file list
//
//--------------------------------------------------------------------------

void  MainApp::RemoveItemFromWindowMenu( UINT16 ID )
{
     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_WINDOW );

     pmenu->RemoveItem( pmenu->GetItemPos( ID ) );

     // do we need to remove the separator ?

     if( init_window_menu_item_count == pmenu->GetItemCount() )
     {
          if( MENUITEM_SEPARATOR == pmenu->GetItemType( init_window_menu_item_count-1 ) )
          {
               pmenu->RemoveItem( init_window_menu_item_count-1 );
               window_menu_sep_present = FALSE;
               init_window_menu_item_count--;
           }
      }
}





//--------------------------------------------------------------------------
//                 MainApp::RemoveNameFromReloadMenu()
//..........................................................................
//
// function to remove a filename from the 'File' menu reloadable file list
//
//--------------------------------------------------------------------------

void  MainApp::RemoveNameFromReloadMenu( const String& rFilename )
{
     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_FILE );

     USHORT current_item_count = pmenu->GetItemCount();

     // check to see if the name is already there as a menu item -
     // if so delete it

     for ( USHORT i = init_file_menu_item_count; i <= current_item_count ; i++)
     {
          if ( FilenamesMatch( rFilename, pmenu->GetItemText( pmenu->GetItemId( i ) ) ) )
          {
               pmenu->RemoveItem( i );
               break;
          }
     }

     // do we need to remove the separator ?

     if( init_file_menu_item_count == pmenu->GetItemCount() )
     {
          if( MENUITEM_SEPARATOR == pmenu->GetItemType( init_file_menu_item_count-1 ) )
          {
               pmenu->RemoveItem( init_file_menu_item_count-1 );
               file_menu_sep_present = FALSE;
               init_file_menu_item_count--;
           }
      }
}






//--------------------------------------------------------------------------
//                 MainApp::AddNameToReloadMenu()
//..........................................................................
//
// function to add a filename to the bottom of the 'File' menu
// so that it can be selected by the user to reload
//
//--------------------------------------------------------------------------

void  MainApp::AddNameToReloadMenu( const String& rFilename )
{
     // don't bother doing anything if rFilename is an empty string

     if ( !rFilename)
        return;


     // get the 'File' popup menu

     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_FILE );

     if ( !file_menu_sep_present )
     {
          pmenu->InsertSeparator();
          file_menu_sep_present = TRUE;
          init_file_menu_item_count++;
     }
     else
          RemoveNameFromReloadMenu( rFilename );

     USHORT current_item_count = pmenu->GetItemCount();

     USHORT new_item_id = 0;

     // if we already have the maximum number of filenames then remove the one at the top
     // and save its id as the one we will re-use

     if ( current_item_count >= ( init_file_menu_item_count + MAX_NUM_RELOADABLE_FILENAMES ) )
     {
          USHORT pos = current_item_count - MAX_NUM_RELOADABLE_FILENAMES;

          new_item_id = pmenu->GetItemId( pos );

          pmenu->RemoveItem( pos );
     }
     else
     {
          // otherwise find an unused id

         for ( USHORT i = vID_MI_PREV_FILE_1;
               ( i < (vID_MI_PREV_FILE_1 + MAX_NUM_RELOADABLE_FILENAMES) && ( new_item_id == 0 ) );
               i++ )
         {
              if ( pmenu->GetItemPos( i ) == MENU_ITEM_NOTFOUND )
                   new_item_id = i;
         }
     }

     ASSERT( new_item_id != 0 );  // a logic error - abort app

     // now add the name at the bottom of the menu

     pmenu->InsertItem( new_item_id, rFilename );

     pmenu->SetHelpText( new_item_id, String( ResId( vID_STR_RELOAD_HELP )) );
}










//--------------------------------------------------------------------------
//                 MainApp::AddItemToWindowMenu()
//..........................................................................
//
// function to add a menu item to the bottom of the 'Window' menu
// so that it can be selected by the user to show a window.
// The function returns the menu item ID if successful or 
// returns INVALID_MENU_ITEM if it fails
//
//--------------------------------------------------------------------------

UINT16 MainApp::AddItemToWindowMenu( const String& rText )
{
     // don't bother doing anything if rFilename is an empty string

     if ( !rText)
        return INVALID_MENU_ITEM;

     // get the 'File' popup menu

     Menu* pmenu = GetAppMenu()->GetPopupMenu( vID_MI_WINDOW );

     if ( !window_menu_sep_present )
     {
          pmenu->InsertSeparator();
          window_menu_sep_present = TRUE;
          init_window_menu_item_count++;
     }

     UINT16 new_item_id = 0;

     // find an unused id
 
     for ( UINT16 i = vID_MI_LOADED_FILE_1;
           ( i < (vID_MI_LOADED_FILE_1 + MAX_NUM_LOADED_FILENAMES) && ( new_item_id == 0 ) );
           i++ )
     {
          if ( pmenu->GetItemPos( i ) == MENU_ITEM_NOTFOUND )
               new_item_id = i;
     }


     if( new_item_id != 0 )
     {
          // now add the name at the bottom of the menu

          pmenu->InsertItem( new_item_id, rText );
 
          pmenu->SetHelpText( new_item_id, String( ResId( vID_STR_LOADED_FILE_HELP )) );

          return new_item_id;
     }
     else
          return INVALID_MENU_ITEM;
}







//--------------------------------------------------------------------------
//                 MainApp::UpdateMenuActivation()
//..........................................................................
//
// function to enable/disable various main menu items depending
// on the general state of things
//
//--------------------------------------------------------------------------

void  MainApp::UpdateMenuActivation( const ScrollingMDIWindow* pActiveWindow )
{
     // get pointer to the menus

     MenuBar* p_app_menu = GetAppMenu();
     Menu*    p_file_menu = p_app_menu->GetPopupMenu( vID_MI_FILE );
     Menu*    p_window_menu = p_app_menu->GetPopupMenu( vID_MI_WINDOW );

     // if we have no active window then do default actions 

     if ( NULL == pActiveWindow )
     {
          p_file_menu->EnableItem( vID_MI_CLOSE_SCHEME,   FALSE );
          p_file_menu->EnableItem( vID_MI_SAVE_SCHEME,    FALSE );
          p_file_menu->EnableItem( vID_MI_SAVE_SCHEME_AS, FALSE );
     }
     else
     {
          if ( pActiveWindow->WindowType() == MDI_SCHEMEWINDOW )
          {
     
               ReactionScheme& rscheme = ( ( SchemeFrameWindow* ) pActiveWindow )->GetReactionScheme();
          
               // enable/disable items as required
          
               p_file_menu->EnableItem( vID_MI_CLOSE_SCHEME, TRUE );
          
               p_file_menu->EnableItem( vID_MI_SAVE_SCHEME, 
                                        rscheme.SchemeDataIsModified() &&
                                        rscheme.GetName().Len() 
                                      );
          
               p_file_menu->EnableItem( vID_MI_SAVE_SCHEME_AS, TRUE );
     
          }
     }

     // deal with the MDI window menu items 

     BOOL8 activate_state = ( NULL != pActiveWindow ) && ( GetVisibleWindowCount() > 0 ); 

     p_window_menu->EnableItem( vID_MI_CASCADE, activate_state );
     p_window_menu->EnableItem( vID_MI_TILE, activate_state );
     p_window_menu->EnableItem( vID_MI_HORIZONTAL, activate_state );
     p_window_menu->EnableItem( vID_MI_VERTICAL, activate_state );
     p_window_menu->EnableItem( vID_MI_ARRANGE, activate_state );

     p_window_menu->EnableItem( vID_MI_CLOSEALL, ( NULL != pActiveWindow ) && ( GetWindowCount() > 0 ) );
}




//--------------------------------------------------------------------------
//                 MainApp::AddToWindowList()
//..........................................................................
//
//
//--------------------------------------------------------------------------

void MainApp::AddToWindowList( ScrollingMDIWindow* pNewWin ) 
{ 
     // add to the internal List 

     window_list.Insert( pNewWin, LIST_APPEND ); 

     switch ( pNewWin->WindowType() )
     {
     case MDI_SCHEMEWINDOW :

          // deal with untitled scheme 
          if ( !pNewWin->GetName() )
               pNewWin->SetMenuId( AddItemToWindowMenu( String( ResId( vID_STR_SCHEME_DEFAULT_NAME) )  + String( ResId( vID_STR_SCHEME_TITLE_SUFFIX) ) ) );
          else
               pNewWin->SetMenuId( AddItemToWindowMenu( pNewWin->GetName() + String( ResId( vID_STR_SCHEME_TITLE_SUFFIX) ) ) );

          break;


     case MDI_WATERFALLPLOTWINDOW :

          pNewWin->SetMenuId( AddItemToWindowMenu( pNewWin->GetName() + String( ResId( vID_STR_WATERFALLPLOTWIN_TITLE) ) ) );

          break;

     case MDI_XYPLOTWINDOW:

          pNewWin->SetMenuId( AddItemToWindowMenu( pNewWin->GetName() + String( ResId( vID_STR_XYPLOTWIN_TITLE ) ) ) );

          break;

     case MDI_TEXTWINDOW :

          pNewWin->SetMenuId( AddItemToWindowMenu( pNewWin->GetName() + String( ResId( vID_STR_TEXTWIN_TITLE ) ) ) );

          break;

     default:
          break;
     }
}









//--------------------------------------------------------------------------
//                 MainApp::RemoveFromWindowList()
//..........................................................................
//
//
//--------------------------------------------------------------------------


void MainApp::RemoveFromWindowList( ScrollingMDIWindow* pWin ) 
{ 
     // remove from internal list 

     window_list.Remove( pWin ); 

     // remove from menu 
     RemoveItemFromWindowMenu( pWin->GetMenuId() );
}


