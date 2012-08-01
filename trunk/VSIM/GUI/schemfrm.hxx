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
//  SCHEMFRM.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the SchemeFrameWindow class, an MDI window containing a view 
//  of a reaction scheme.
//
//  CREATED : 1.24.96 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__SCHEMFRM_HXX__)
#define __SCHEMFRM_HXX__


#include "scrmdiwn.hxx"
#include "schemwin.hxx"


// ----------------------------------------------------------------------------
//                                class SchemeFrameWindow
// ............................................................................
//
//  This class is derived from ScrollingMDIWindow. The intent if for this 
//  class to contain and allow user modification of the view shown in a 
//  SchemeWindow
//
// ----------------------------------------------------------------------------

class SchemeFrameWindow : public ScrollingMDIWindow 
{
protected:

     static const USHORT       NUM_COMPTS_STR_INDEX;
     static const USHORT       NUM_XFERPATHS_STR_INDEX;
     
     const enum SCHEMWIN_TYPE scheme_type;
     NotebookDialog*    p_notebook_dlg;
  
     SchemeWindow*      pscheme_win;

     virtual void       Resize(){ RecalcWindowLayout( ); }

     void               ViewResults( );

     virtual void       KeyInput( const KeyEvent& rKeyEvt );
     virtual void       Activate( );
     virtual void       Deactivate();

     void               SetWindowTitle( );
     void               SaveWindowGeometry( );

     void               ReconstructSpeciesDatabase( );

     BOOL8              OKToAbortSimulation();
     void               SaySimulationIsRunning();

     enum SCHEMWIN_TYPE GetSchemeType() const { return scheme_type; }

public:
                        SchemeFrameWindow( enum SCHEMWIN_TYPE SchemeType );
     virtual           ~SchemeFrameWindow( );

     virtual MDI_WIN_TYPE WindowType( ) const { return MDI_SCHEMEWINDOW;}

     void               ToolBarHighlight( ToolBox* pBox );
     void               ToolBarSelect( ToolBox* pBox );

     virtual BOOL       Close();

     void               UpdateStatusBar();
     void               UpdateToolbarActivations();

     void               SetName( const String& rName );
     virtual const String&  GetName( ) const { return pscheme_win->GetScheme().GetName(); }

     SchemeWindow&      GetSchemeWindow( ) { return *pscheme_win; }
     SchemeWindow*      GetpSchemeWindow( ) { return pscheme_win; }

     void               SetRunningIcon( );
     void               SetStandardIcon( );

     ReactionScheme&    GetReactionScheme( ) { return pscheme_win->GetScheme(); }

     BOOL8              SaveScheme( );
     BOOL8              LoadScheme( const String& rFilename );
     BOOL8              StartSimulation( );
     BOOL8              IsInitialized( ) const { return ( NULL != pscheme_win ); }

     void               SetpNotebookDlg( NotebookDialog* pDlg ) { p_notebook_dlg = pDlg; }
     NotebookDialog*    GetpNotebookDlg( ) const { return p_notebook_dlg; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, SchemeFrameWindow& rObject );
     friend BinaryInputStream&  operator >> ( BinaryInputStream&  rBIS, SchemeFrameWindow& rObject );

};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, SchemeFrameWindow& rObject );
BinaryInputStream&  operator >> ( BinaryInputStream&  rBIS, SchemeFrameWindow& rObject );

#endif

