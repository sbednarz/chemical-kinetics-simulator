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
//  SCRMDIWN.HXX - a module of the VSIM User interface 
// ............................................................................
//
//  This module defines the ScrollingMDIWindow  class, which contains a
//  status bar, toolbar, horizontal and vertical scroll bars and a scaling
//  mechanism
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "scrmdiwn.hxx"
#include "math.hxx"
#include "assert.hxx"

#endif


#include <stdlib.h>





// ----------------------------------------------------------------------------
//             ScrollingMDIWindow::ScrollingMDIWindow()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

ScrollingMDIWindow::ScrollingMDIWindow( WorkWindow* pParent, ScalableWindow* pDrawingArea, const ResId& rToolBoxResId ):
MDIWindow( pParent, WB_CLOSEABLE | WB_SVLOOK ),
menu_id( INVALID_MENU_ITEM ),
scrollbar_width_pixels(0),
scrollbar_height_pixels(0),
line_height_pixels(0),
column_width_pixels(0),
statusbar_height_pixels(0),
toolbar_height_pixels(0),
vert_scrollbar( this, WinBits(WB_VSCROLL | WB_DRAG) ),
horiz_scrollbar( this, WinBits(WB_HSCROLL | WB_DRAG) ),
corner_win( this ),
ctrl_sizes_initialized( FALSE ),
status_bar( this, WB_BORDER | WB_SVLOOK ),
tool_bar( this, rToolBoxResId ),
pdrawing_area( pDrawingArea )
{
    corner_win.ChangeBackgroundBrush( Brush( Color(COL_SCROLLBAR) ) );

    status_bar.Show();
    tool_bar.Show();
    vert_scrollbar.Show();
    horiz_scrollbar.Show();
    corner_win.Show();
    
    vert_scrollbar.ChangeScrollHdl( LINK(this, ScrollingMDIWindow, ScrollHdl) );
    horiz_scrollbar.ChangeScrollHdl( LINK(this, ScrollingMDIWindow, ScrollHdl) );
}





// ----------------------------------------------------------------------------
//             ScrollingMDIWindow::~ScrollingMDIWindow()
// ............................................................................
//
//   destructor
//
// ----------------------------------------------------------------------------

ScrollingMDIWindow::~ScrollingMDIWindow( )
{
}





// ----------------------------------------------------------------------------
//             ScrollingMDIWindow::SetpDrawingArea()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  ScrollingMDIWindow::SetpDrawingArea( ScalableWindow* pWin ) 
{ 
     pdrawing_area = pWin; 
     pWin->ChangeParent( this ); 
     pWin->Show();
}






Fraction ScrollingMDIWindow::ChangeDrawingScale( const Fraction& rNewScale ) 
{ 
     Fraction f = GetpDrawingArea()->ChangeScale( rNewScale );

     SetScrollbarPageAndLineSizes();
     RecalcWindowLayout();

     return f;
}





// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::InitializeControlSizes( )
// ............................................................................
//
//  called only once during life of an object - this is required since
//  some of the controls are not correctly sized at the time of object
//  construction
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::InitializeControlSizes()
{
     // get default widths and put them into effect

     scrollbar_width_pixels    = vert_scrollbar.GetSizePixel().Width();
     column_width_pixels       = scrollbar_width_pixels;
     scrollbar_height_pixels   = horiz_scrollbar.GetSizePixel().Height();
     line_height_pixels        = scrollbar_height_pixels;
     statusbar_height_pixels   = status_bar.GetOutputSizePixel().Height();
     toolbar_height_pixels     = tool_bar.CalcWindowSizePixel().Height( );

     SetScrollbarPageAndLineSizes();

     ctrl_sizes_initialized = TRUE;
}




// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::SetScrollbarPageAndLineSizes()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::SetScrollbarPageAndLineSizes()
{
     const INT16 LINES_PER_PAGE = 3;

     // set up scroll bar page and line sizes in logical coordinates

     INT32 value = GetpDrawingArea()->GetScale() * Fraction( (long) column_width_pixels );
     horiz_scrollbar.ChangePageSize( LINES_PER_PAGE * value );
     horiz_scrollbar.ChangeLineSize( value );

     value = GetpDrawingArea()->GetScale() * Fraction( (long) line_height_pixels );
     vert_scrollbar.ChangePageSize( LINES_PER_PAGE * value  );
     vert_scrollbar.ChangeLineSize( value );
}






// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::PositionWindowElements()
// ............................................................................
//
//  called whenever the SchemeWindow is resized 
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::PositionWindowElements( )
{
    // get the current window and control output sizes in pixel units

    Size output_size_pixels = MDIWindow::GetOutputSizePixel();

     // position the status bar

    status_bar.SetPosSizePixel( Point( 0, output_size_pixels.Height() - statusbar_height_pixels ),
                             Size( output_size_pixels.Width(), statusbar_height_pixels ) );

     // and position the tool bar

    tool_bar.SetPosSizePixel( Point( 0, 0 ),
                          Size( output_size_pixels.Width( ), toolbar_height_pixels ) );


    // correct current output size to account for scroll, status and tool bars

    output_size_pixels.Height() -= (toolbar_height_pixels +
     statusbar_height_pixels + scrollbar_height_pixels );

    output_size_pixels.Width() -= scrollbar_width_pixels;

     // resize drawing_area and other controls in window using changed value of output_size_pixels

    GetpDrawingArea()->SetPosSizePixel( Point( 0, toolbar_height_pixels ), output_size_pixels );

    horiz_scrollbar.SetPosSizePixel(
     Point( 0, output_size_pixels.Height() + toolbar_height_pixels ),
     Size( output_size_pixels.Width(), scrollbar_height_pixels ) );

    vert_scrollbar.SetPosSizePixel(
     Point( output_size_pixels.Width(), toolbar_height_pixels ),
     Size( scrollbar_width_pixels, output_size_pixels.Height() ) );

    corner_win.SetPosSizePixel(
     Point( output_size_pixels.Width(), output_size_pixels.Height() + toolbar_height_pixels ),
     Size( scrollbar_width_pixels, scrollbar_height_pixels ) );
}







// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::SetScrollbarActivation()
// ............................................................................
//
//  called whenever the SchemeWindow is resized 
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::SetScrollbarActivation( )
{
     // do we need to activate scrollbars ? 
    BOOL8 horiz_scrollbar_active = ( GetpDrawingArea()->ConcealedWidth() > 0 );
    BOOL8 vert_scrollbar_active  = ( GetpDrawingArea()->ConcealedHeight() > 0 );


    if ( horiz_scrollbar_active )
         horiz_scrollbar.Enable();
     else
         horiz_scrollbar.Disable();

    if ( vert_scrollbar_active )
         vert_scrollbar.Enable();
     else
         vert_scrollbar.Disable();
}






// ----------------------------------------------------------------------------
//              ScrollingMDIWindow::SetScrollbarRangeAndThumbPosition()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::SetScrollbarRangeAndThumbPosition()
{
     // reset ranges on scroll bars 
     // first the horizontal 

    Range range(0, GetpDrawingArea()->ConcealedWidth() );
    
    horiz_scrollbar.ChangeRange( range );
    horiz_scrollbar.ChangeThumbPos(
     ( -GetpDrawingArea()->HorizontalOffsetPixels() > range.Max()) ? range.Max() : -GetpDrawingArea()->HorizontalOffsetPixels() );

     // then the vertical 

    range = Range( 0, GetpDrawingArea()->ConcealedHeight() );
    
    vert_scrollbar.ChangeRange( range );
    vert_scrollbar.ChangeThumbPos(
     ( -GetpDrawingArea()->VerticalOffsetPixels() > range.Max()) ? range.Max() : -GetpDrawingArea()->VerticalOffsetPixels() );
                                  
}




// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::RecalcWindowLayout( )
// ............................................................................
//
//  called whenever the SchemeWindow is resized - we recalc the window layout
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::RecalcWindowLayout( )
{
    if ( !ctrl_sizes_initialized )
         InitializeControlSizes();   // only called on very first invocation 

     PositionWindowElements();
     SetScrollbarActivation();
     SetScrollbarRangeAndThumbPosition();
}





// ----------------------------------------------------------------------------
//                   ScrollingMDIWindow::ScrollHdl( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::ScrollHdl( ScrollBar* pScroll )
{
    if ( pScroll == &horiz_scrollbar )
        GetpDrawingArea()->Scroll( horiz_scrollbar.GetDelta(), 0 );
    else
        GetpDrawingArea()->Scroll( 0, vert_scrollbar.GetDelta() );
}











// ----------------------------------------------------------------------------
//                  ScrollingMDIWindow::SetOutputAreaSizeLogic()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ScrollingMDIWindow::SetDrawingAreaSizeLogic( const Size& rNewSize ) 
{ 
     Size req_size_pixels = GetpDrawingArea()->LogicToPixel( rNewSize );

     ChangeOutputSizePixel( 
      Size( req_size_pixels.Width() + scrollbar_width_pixels, 
            req_size_pixels.Height() + scrollbar_height_pixels + 
             statusbar_height_pixels + toolbar_height_pixels 
          )
     );

     RecalcWindowLayout();
}

