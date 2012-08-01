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
//  SCALWIN.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This module defines the ScalableWindow  class, which contains a
//  scaling mechanism for creating drawings on screen 
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "scalwin.hxx"
#include "assert.hxx"

#endif


#include <stdlib.h>


const INT16 NUMERATOR = 4;
const INT16 DENOMINATOR = 5;

const Fraction ScalableWindow::FULL_SCALE( 1, 1);
const Fraction ScalableWindow::DECREMENT_SCALE( NUMERATOR, DENOMINATOR );
const Fraction ScalableWindow::INCREMENT_SCALE( DENOMINATOR, NUMERATOR );

const Fraction ScalableWindow::MINIMUM_SCALE( NUMERATOR * NUMERATOR * NUMERATOR * NUMERATOR * NUMERATOR,
                                              DENOMINATOR * DENOMINATOR * DENOMINATOR * DENOMINATOR * DENOMINATOR);
const Fraction ScalableWindow::MAXIMUM_SCALE( DENOMINATOR * DENOMINATOR * DENOMINATOR, 
                                              NUMERATOR * NUMERATOR * NUMERATOR);


ScalableWindow::ScalableWindow( Window* pParent ) : 
Window( pParent ),
win_offset_pixels(),
virtual_win_size_logic(),
output_size_pixels(),
output_area_origin_pixels(),
current_window_scale( FULL_SCALE )
{
    ChangeScale( current_window_scale );
}



// ----------------------------------------------------------------------------
//                  ScalableWindow::ChangeScale( )
// ............................................................................
//
//  called when the user changes the scale of the ScalableWindow 
//
// ----------------------------------------------------------------------------

Fraction ScalableWindow::ChangeScale( const Fraction& rNewScale )
{
    MapMode map_mode = GetMapMode();

    Fraction old_scale( map_mode.GetScaleX() );

    ASSERT( rNewScale.IsValid() && ( rNewScale.GetNumerator() > 0 ) );

    map_mode.ChangeScaleX( rNewScale );
    map_mode.ChangeScaleY( rNewScale );
    current_window_scale = rNewScale;

    long x = rNewScale * Fraction( (long) win_offset_pixels.X() );
    long y = rNewScale * Fraction( (long) win_offset_pixels.Y() );

    win_offset_pixels = Point( x,y );

    ChangeMapMode( map_mode );

    Invalidate();

    return old_scale;
}







void ScalableWindow::SetPosSizePixel( const Point& rNewPos, const Size& rNewSize )
{
     Window::SetPosSizePixel( rNewPos, rNewSize );

    // store the old offset

    const Point prev_win_offset_pixels( win_offset_pixels );

    // for axis without scrollbar active, restore the origin to 0 

    if ( !(ConcealedWidth() > 0) )
         win_offset_pixels.X() = 0;

    if ( !(ConcealedHeight() > 0) )
         win_offset_pixels.Y() = 0;


    // select the shifted map-mode

    if ( win_offset_pixels != prev_win_offset_pixels )
    {
         MapMode old_mapmode = ChangeMapMode( MapMode(MAP_PIXEL) );

         Window::Scroll(
          win_offset_pixels.X() - prev_win_offset_pixels.X(),
          win_offset_pixels.Y() - prev_win_offset_pixels.Y() );

        ChangeMapMode( old_mapmode );
    }

    MapMode map_mode = GetMapMode();

    map_mode.ChangeOrigin( Point( 0,0 ) );

    map_mode.ChangeOrigin( PixelToLogic( win_offset_pixels, map_mode ) );

    ChangeMapMode( map_mode );
}





// ----------------------------------------------------------------------------
//                  ScalableWindow::Scroll( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ScalableWindow::Scroll( INT16 PixelDeltaX, INT16 PixelDeltaY )
{
    // get the delta in pixel units and make local copies of some class data members

    // get the delta in logic coordinates

    Size    log_delta( PixelToLogic( Size( PixelDeltaX, PixelDeltaY ) ) );
    INT16 LogicalDeltaX = log_delta.Width();
    INT16 LogicalDeltaY = log_delta.Height(); 

    Size    delta_in_pixels( PixelDeltaX, PixelDeltaY );

    Size    out_pix_size( output_size_pixels );

    MapMode map_mode( GetMapMode() );
    Point   new_win_offset_pixels( win_offset_pixels );

    // calculate stuff on fundamental  ( i.e, pixel ) units

    // do this block if we are scrolling horizontally

    if ( LogicalDeltaX != 0 )
    {
        // recalc window offset

        new_win_offset_pixels.X() -= delta_in_pixels.Width();

        // width of visible window + new offset cannot exceed the width of the virtual window

        if ( ( out_pix_size.Width() - new_win_offset_pixels.X() ) > GetTotalSizePixels().Width() )
        {
             new_win_offset_pixels.X() = - ( GetTotalSizePixels().Width() - out_pix_size.Width() );
        }
        else
        {
             // and offset cannot be greater than zero

             if ( new_win_offset_pixels.X() > 0 )
                  new_win_offset_pixels.X() = 0;
        }
        
    }


    // do this block if we are scrolling vertically

    if ( LogicalDeltaY != 0 )
    {
        // recalc window offset

        new_win_offset_pixels.Y() -= delta_in_pixels.Height();

        // height of visible window + new offset cannot exceed the height of the virtual window

        if ( ( out_pix_size.Height() - new_win_offset_pixels.Y() ) > GetTotalSizePixels().Height() )
        {
             new_win_offset_pixels.Y() = - ( GetTotalSizePixels().Height() - out_pix_size.Height() );
        }
        else
        {
             // and offset cannot be greater than zero

             if ( new_win_offset_pixels.Y() > 0 )
                  new_win_offset_pixels.Y() = 0;
        }

    }

    // now recompute the delta to be scrolled in logical units

    delta_in_pixels.Width() = win_offset_pixels.X() - new_win_offset_pixels.X();
    delta_in_pixels.Height() = win_offset_pixels.Y() - new_win_offset_pixels.Y();
    Size delta_in_logic( PixelToLogic(delta_in_pixels) );
    LogicalDeltaX = delta_in_logic.Width();
    LogicalDeltaY = delta_in_logic.Height(); 

    win_offset_pixels = new_win_offset_pixels;

    // is there any physical scrolling called for ?

    if ( LogicalDeltaX != 0 || LogicalDeltaY != 0 )
    {
        Update();

        map_mode.ChangeOrigin( Point() );

        map_mode.ChangeOrigin( PixelToLogic( win_offset_pixels +  output_area_origin_pixels, map_mode ) );
        ChangeMapMode(map_mode);

        // does the new area overlap the old one?

        if ( ( ( LogicalDeltaY != 0) && ( abs( delta_in_pixels.Height() ) < out_pix_size.Height() ) ) ||
             ( ( LogicalDeltaX != 0) && ( abs( delta_in_pixels.Width() ) < out_pix_size.Width() ) ) )
        {
            // scroll the area of overlap without scrolling the scrollbars or the
            // toolbar or status bar

            Point scroll_area_origin = output_area_origin_pixels;

            if ( LogicalDeltaX < 0 )
                 out_pix_size.Width()  = out_pix_size.Width()  + delta_in_pixels.Width();

            if ( LogicalDeltaY < 0 )
                 out_pix_size.Height()  = out_pix_size.Height() +  delta_in_pixels.Height();
            else
            {
                 scroll_area_origin += Point( 0, delta_in_pixels.Height() );
                 out_pix_size.Height()  = out_pix_size.Height() - delta_in_pixels.Height();
             }

#if defined(__AIX__)
            
            // calculate the rectangle to scroll, in logical units
            // PixelToLogic( Size ) give the wrong answer on AIX platform for some reason
            // so the scaling of Size is done explicitly here
            
            FLOAT64 factor = current_window_scale;

            ASSERT ( factor != 0.0 );
              
            Size size( Round(  out_pix_size.Width() / factor ), out_pix_size.Width() / factor );
#else
            Size size =  PixelToLogic( out_pix_size, map_mode );
#endif
            
            Point origin =  PixelToLogic( scroll_area_origin, map_mode );
            
              
            Rectangle  scroll_rect = Rectangle( origin, size );
            
            Window::Scroll( -LogicalDeltaX, -LogicalDeltaY, scroll_rect );
        }
        else
        {
            // repaint the whole output area

            ChangeMapMode(map_mode);
            Invalidate();
        }

        Update();
    }

}

