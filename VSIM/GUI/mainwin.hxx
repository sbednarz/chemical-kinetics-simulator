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
//  MAINWIN.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header declares the main Window class for
//  the VSIM user interface 
//
//  CREATED : 1.10.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__MAINWIN_HXX__)
#define __MAINWIN_HXX__

#include "procsobj.hxx"

// ----------------------------------------------------------------------------
//  class MainWindow                                   
// ............................................................................
// 
//  This class is derived from WorkWindow                       
//
// ----------------------------------------------------------------------------

class MainWindow : public WorkWindow
{
protected:
     StatusBar       aHelpStatus;
     ToolBox         ToolBar;
     Rectangle       active_area;

     ProcessObject*  pclip_object;

     virtual void    MouseMove( const MouseEvent & );
     void            AddCompartment( );
     void            AddTransferPath( );
     void            DeleteObject( );
     void            CopyObject( );
     void            PasteObject( );
     void            Zoom( UINT16 ZoomMode );
     void            SelectAllCompartments();
     void            SelectAllTransferPaths();

     virtual void    ChangeCompartmentDimensions();
     virtual void    ChangeSchemeTemperature();
     virtual void    GlobalSpeciesConcChange();

public:
                     MainWindow( Window* pParent, WinBits aWinStyle );
                     ~MainWindow( );
     StatusBar&      GetStatusBar() { return aHelpStatus; }
     const Rectangle& GetActiveArea() { return active_area; }

     void            ToolBarHighlight( ToolBox* pBox );
     void            ToolBarSelect( ToolBox* pBox );

     virtual void    Resize();
     virtual BOOL    Close();

     void            ActivateEditTools( const BOOL NewState );
     void            ActivatePaste( BOOL NewState );
     void            ActivateAddCompartment( const BOOL NewState );
     void            ActivateAddTransferObject( const BOOL NewState );
     void            ActivateZoomTools( const BOOL NewState );
     void            ActivateSelectAllTools( const BOOL NewState );
     void            ActivateSelectAllXfersTools( const BOOL NewState );
     void            ActivateSelectAllComptsTools( const BOOL NewState );
     void            Activate3DSpecialEditTools( const BOOL NewState );                                           

     void            ExecuteAction( UINT16 ID );

    enum ProcObjType GetClipObjectType() const { return  ( NULL == pclip_object ? PO_TYPE_NONE : pclip_object->GetType() );}
};


#endif
