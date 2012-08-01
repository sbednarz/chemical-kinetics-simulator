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
//  MAINAPP.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file defines the MainApp class for use in
//  the VSIM user interface
//
//  CREATED : 1.10.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__MAINAPP_HXX__)
#define __MAINAPP_HXX__

#include "mainwin.hxx"
#include "schemfrm.hxx"


const UINT16 VERSION_MAJOR = 0;
const UINT16 VERSION_MINOR = 0;
const UINT32 BUILD_LEVEL   = 30295;  // encoded date  [m]mddyy



DECLARE_LIST( WindowList, ScrollingMDIWindow* )

// ----------------------------------------------------------------------------
//                                class MainApp
// ............................................................................
//
//  This class is derived from MDIApplication
//
// ----------------------------------------------------------------------------

class MainApp : public MDIApplication
{
protected:
     WindowList     window_list;

     Window*        over_window;
     String*        p_rxn_file_path;
     String*        p_txt_file_path;
     String*        p_any_file_path;
     String*        p_prf_file_path;
     String*        p_rat_file_path;
     UINT16         init_file_menu_item_count;
     UINT16         init_window_menu_item_count;
     BOOL8          file_menu_sep_present;
     BOOL8          window_menu_sep_present;
     BOOL8          confirm_before_overwrite;
     BOOL8          save_win_data;
     PUCHAR8        preserved_mem_block;

     void           EnableAppMenu( BOOL State = TRUE );

     void           LoadReactionScheme();
     void           LoadReactionScheme( const String& rFilename );
     void           SaveSchemeUnderNewName();
            
     void           NewCompartmentScheme();
     void           New3DScheme();

     void           InitializeFromConfigFile();
     void           SaveStateToConfigFile();
     void           ValidateDefaultDirString( String* pStr, UINT16 InitStrID );

     virtual void   UserEvent( ULONG nEvent, void* pEventData );

     BOOL8          RxnSchemeIsAlreadyLoaded( const String& rFileName );
                    
     void           FileAlreadyLoaded ( const String& rFilename );

     ScrollingMDIWindow* GetWindowPtrFromId( UINT16 Id );

    UINT16          AddItemToWindowMenu( const String& rText );
    void            RemoveItemFromWindowMenu( UINT16 ID );

    void            RemoveNameFromReloadMenu( const String& rFilename );

public:
                    MainApp();

    virtual void    Main( int, char*[] );

    virtual BOOL    QueryExit() { return CloseAll(); }

    void            SetWindowUnderPointer( Window* pWin ) { over_window = pWin; }
    const Window*   GetWindowUnderPointer( ) { return over_window; }

    long            MenuSelect( Menu* pMenu );
    long            MenuHighlight( const Menu* pMenu );
    long            MenuActivate( const Menu* pMenu );
    MainWindow*     GetMainWindow( ) {return (MainWindow *) GetAppWindow( ); };

     // menu manipulation functions 



    void            UpdateMenuActivation( const ScrollingMDIWindow* pActiveWindow );

    // calls to popup message boxes

    void            OutOfMemory(  );
    void            ForeignInputFile( const String& rFilename );
    void            InvalidInputFile( const String& rFilename );
    void            InputFileCorrupt( const String& rFilename );
    void            FileOpenError( const String& rFilename );
    void            FileSeekError( const String& rFilename );
    void            FileReadError( const String& rFilename );
    void            FileWriteError( const String& rFilename );

    BOOL8           OKtoOverwriteFile( const String& rFilename );

    void            CascadeWindowOnParent( Window* pChild, Window* pParent );
    void            CenterWindowOnParent( Window* pChild, Window* pParent );

    ScrollingMDIWindow*   GetActiveWindow( )
    {
         return ( ( ScrollingMDIWindow* ) MDIApplication::GetActiveWindow( ) );
    }

    UINT16          GetMajorVersion( ) const { return VERSION_MAJOR; }
    UINT16          GetMinorVersion( ) const { return VERSION_MINOR; }
    UINT32          GetBuildLevel( ) const { return BUILD_LEVEL; }

    const String&   GetDefaultRxnFilePath()  const { return *p_rxn_file_path; }
    const String&   GetDefaultTxtFilePath() const { return *p_txt_file_path; }
    const String&   GetDefaultAnyFilePath()  const { return *p_any_file_path; }
    const String&   GetDefaultPrfFilePath()  const { return *p_prf_file_path; }
    const String&   GetDefaultRatFilePath()  const { return *p_rat_file_path; }

    void            SetDefaultRxnFilePath( const String& rNewStr ){ *p_rxn_file_path = rNewStr; }
    void            SetDefaultTxtFilePath( const String& rNewStr ){ *p_txt_file_path = rNewStr; }
    void            SetDefaultAnyFilePath( const String& rNewStr ){ *p_any_file_path = rNewStr; }
    void            SetDefaultPrfFilePath( const String& rNewStr ){ *p_prf_file_path = rNewStr; }
    void            SetDefaultRatFilePath( const String& rNewStr ){ *p_rat_file_path = rNewStr; }

    void            AddToWindowList( ScrollingMDIWindow* pNewWin );
    void            RemoveFromWindowList( ScrollingMDIWindow* pWin );

    void            AddNameToReloadMenu( const String& rFilename);

    BOOL8           ConfirmBeforeOverwrite() const { return confirm_before_overwrite;}
    BOOL8           SaveWindowPositionData( ) const { return save_win_data;}
    void            SetConfirmBeforeOverwrite( BOOL8 NewState) { confirm_before_overwrite = NewState;}
    void            SetSaveWindowPositionData( BOOL8 NewState) { save_win_data = NewState;}
};


extern MainApp* GetpMainApp( );

#endif

