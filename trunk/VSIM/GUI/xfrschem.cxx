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
//  XFRSCHEM.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SchematicWindow class for
//  the VSIM user interface. 
//
//  CREATED : 8.07.98 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "xfrschem.hxx"

#endif



const Color ARROW_FILL_COLOR( COL_BLACK );



// ----------------------------------------------------------------------------
//                SchematicWindow::SchematicWindow()
// ............................................................................
// 
//   constructor
//
// ----------------------------------------------------------------------------

SchematicWindow::SchematicWindow( Window* pParent, const ResId& rResId ) :
Window ( pParent, rResId ),
source(),
target(),
species(),
xfer_direction( SOURCE_TO_TARGET ),
arrow_fill( ARROW_FILL_COLOR )
{
     ChangeMapMode( MapMode( MAP_POINT ) );
     ChangeBackgroundBrush( pParent->GetBackgroundBrush( ) );

     window_size = GetOutputSize( );

     box_size = Size( ( 3 * window_size.Width() ) / 10 , 
                      ( 8 * window_size.Height() ) / 10 );

     source_box_pos = Point( window_size.Width() / 20, window_size.Height() / 10 );
     target_box_pos = Point( (65 * window_size.Width() ) / 100, window_size.Height() / 10 );

     fwd_arrow_left_end = Point( (4 * window_size.Width() ) / 10, window_size.Height() / 2 ); 
     fwd_arrow_right_end = Point( (6 * window_size.Width() ) / 10, window_size.Height() / 2 ); 
     rev_arrow_left_end = Point( (4 * window_size.Width() ) / 10, (7 * window_size.Height() ) / 10 ); 
     rev_arrow_right_end = Point( (6 * window_size.Width() ) / 10, (7 * window_size.Height() ) / 10 ); 

     Size str_size = GetTextSize( "XXXXXXXXXX" );

     char_width = str_size.Width()/10;
     char_height = str_size.Height();

     Font afont( GetFont() );
     afont.ChangeTransparent( TRUE );
     ChangeFont( afont);
}





// ----------------------------------------------------------------------------
//                SchematicWindow::Paint()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SchematicWindow::Paint( const Rectangle& )
{
     const short ARROW_LENGTH = 1;      // ARROW_LENGTH in character height  
     const short ARROW_ASPECT_RATIO = 2; // ARROW_ASPECT_RATIO is ratio of length to width 

     // define point array in preparation of drawing a polygon

     Point point_array[3];


     // define dimensions of arrowhead 

     short arrow_length = char_height * ARROW_LENGTH;
     short arrow_halfwidth = arrow_length / ( 2 * ARROW_ASPECT_RATIO ); 

     // note factor of 2 since  width is bisected by line

     // do some drawing 

     DrawRect( Rectangle( source_box_pos, box_size ) );
     DrawRect( Rectangle( target_box_pos, box_size ) );

     Brush old_brush = ChangeFillInBrush( arrow_fill );

     if (TARGET_TO_SOURCE != xfer_direction )
     {
          DrawLine( fwd_arrow_left_end, fwd_arrow_right_end );

          point_array[0] = fwd_arrow_right_end;
          point_array[1] =  fwd_arrow_right_end - Point( arrow_length, arrow_halfwidth );
          point_array[2] =  fwd_arrow_right_end - Point( arrow_length, -arrow_halfwidth );
     
          DrawPolygon( Polygon( 3, point_array ) );
     
     }

     if (SOURCE_TO_TARGET != xfer_direction )
     {
          DrawLine( rev_arrow_left_end, rev_arrow_right_end );

          point_array[0] = rev_arrow_left_end;
          point_array[1] =  rev_arrow_left_end + Point( arrow_length, arrow_halfwidth ); 
          point_array[2] =  rev_arrow_left_end + Point( arrow_length, -arrow_halfwidth ); 

          DrawPolygon( Polygon( 3, point_array ) );
     }

     ChangeFillInBrush( old_brush );
     Size text_size = GetTextSize( source );

     Point location = Point( 
     source_box_pos.X() + (box_size.Width()/2) - (text_size.Width()/2), 
      window_size.Height()/2 - (char_height/2) );

     DrawText( location, source );

     text_size = GetTextSize( target );

     location.X() = target_box_pos.X() + (box_size.Width()/2) - (text_size.Width()/2);

     DrawText( location, target );

     text_size = GetTextSize( species );
     DrawText( 
      Point( window_size.Width()/2 - text_size.Width()/2, 
      target_box_pos.Y() ), 
      species );
}





