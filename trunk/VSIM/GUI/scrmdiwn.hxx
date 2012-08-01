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
//  SCRMDIWN.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file describes the ScrollingMDIWindow  class, which contains a
//  status bar, toolbar, horizontal and vertical scroll bars and a scaling
//  mechanism
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__SCRMDIWN_HXX__)
#define __SCRMDIWN_HXX__


#include <sv.hxx>

#include "vsimtype.hxx"
#include "events.hxx"
#include "scalwin.hxx"


enum  MDI_WIN_TYPE { MDIWIN_UNKNOWN, MDI_SCHEMEWINDOW, MDI_XYPLOTWINDOW, 
                     MDI_WATERFALLPLOTWINDOW, MDI_TEXTWINDOW };



const UINT16        INVALID_MENU_ITEM = 0xFFFE;

class ScrollingMDIWindow : public MDIWindow
{
  
private:
    UINT16              menu_id;
  
    INT16               scrollbar_width_pixels;
    INT16               scrollbar_height_pixels;

    INT16               line_height_pixels;        // size of a line/column (pixel)
    INT16               column_width_pixels;

    INT16               statusbar_height_pixels;
    INT16               toolbar_height_pixels;

    ScrollBar           vert_scrollbar;
    ScrollBar           horiz_scrollbar;
    Window              corner_win;
    BOOL8               ctrl_sizes_initialized;
    StatusBar           status_bar;
    ToolBox             tool_bar;
    ScalableWindow*     pdrawing_area;

  
  
protected:
  
    void                ScrollHdl( ScrollBar* pScroll );
    void                InitializeControlSizes();

    void                PositionWindowElements( );

    virtual void        SetScrollbarPageAndLineSizes();
    virtual void        SetScrollbarActivation( );
    virtual void        SetScrollbarRangeAndThumbPosition();
  
    void                InvalidateDrawingArea( ){ GetpDrawingArea()->Invalidate( ); }

    void                SetDrawingAreaSizeLogic( const Size& rNewSize );

  
    ScrollBar&          GetVerticalScrollBar( ) { return vert_scrollbar; }
    ScrollBar&          GetHorizontalScrollBar( ) { return horiz_scrollbar; }
    ToolBox&            GetToolBar( ) { return tool_bar; }
    StatusBar&          GetStatusBar( ) { return status_bar; }

public:
                        ScrollingMDIWindow( WorkWindow* pParent, ScalableWindow* pDrawingArea, const ResId& rToolBoxResId );

    virtual            ~ScrollingMDIWindow( );


    void                SetTotalDrawingSize( const Size& rNewSize ){ pdrawing_area->SetTotalSize( rNewSize ); }
    Fraction            GetDrawingScale() const { return pdrawing_area->GetScale(); }
    virtual Fraction    ChangeDrawingScale( const Fraction& rNewScale );
    ScalableWindow*     GetpDrawingArea() { return pdrawing_area; }
    void                SetpDrawingArea( ScalableWindow* pWin );
  
    void                RecalcWindowLayout( );

    virtual MDI_WIN_TYPE WindowType ( ) const = 0;

    virtual const String& GetName( ) const = 0;
  
    void                UpdateWindow() { GetpApp()->PostUserEvent( vEVT_UPDATE_WINDOW_ASYNC, this );  }

#if defined(OS2)
    virtual void        Activate() { Invalidate(); Update(); }
#endif

     UINT16              GetMenuId( ) const { return menu_id; }
     void                SetMenuId( UINT16 NewId ) { menu_id = NewId; }
};


#endif
