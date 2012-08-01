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
//  ARROW.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the Arrow  class for
//  the VSIM user interface. 
//
//  CREATED : 10.18.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "arrow.hxx"
#include "math.hxx"

#endif


#include <stdlib.h>
#include <math.h>



const Color ARROW_FILL_COLOR( COL_BLACK );

const INT16 DEFAULT_ARROW_LENGTH = 2;
const INT16 DEFAULT_ARROW_WIDTH  = 2;




// ----------------------------------------------------------------------------
//                    Arrow::Arrow( )
// ............................................................................
// 
// constructor
//
// ----------------------------------------------------------------------------

Arrow::Arrow( OutputDevice* pOutputDevice ):
poutput_device( pOutputDevice ),  
source_point( ),
target_point( ),
arrow_fill( ARROW_FILL_COLOR ),
arrow_length( DEFAULT_ARROW_LENGTH ),
arrow_halfwidth( DEFAULT_ARROW_WIDTH/2 ),
show_arrowhead ( TRUE ),
polygon( 3 )
{
     // set arrow size based on current font size
  
     SizeArrow( GetpOutputDevice()->GetTextSize( "X" ).Height());                
}  



// ----------------------------------------------------------------------------
//                    Arrow::operator = 
// ............................................................................
// 
//   assignment operator
//
// ----------------------------------------------------------------------------
Arrow& Arrow::operator = ( const Arrow& rObject )
{
     poutput_device  = rObject.poutput_device;
     source_point    = rObject.source_point;
     target_point    = rObject.target_point; 
     arrow_fill      = rObject.arrow_fill;
     arrow_length    = rObject.arrow_length;  
     arrow_halfwidth = rObject.arrow_halfwidth;
     show_arrowhead  = rObject.show_arrowhead;
     polygon         = rObject.polygon;

}






// ----------------------------------------------------------------------------
//                    Arrow::SetpOutputDevice()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Arrow::SetpOutputDevice( OutputDevice* pNewDevice ) 
{ 
     poutput_device = pNewDevice; 
     SizeArrow( GetpOutputDevice()->GetTextSize( "X" ).Height());                
}





// ----------------------------------------------------------------------------
//                    Arrow::DrawObject( )
// ............................................................................
// 
// 
//
// ----------------------------------------------------------------------------

void Arrow::DrawObject()
{
     GetpOutputDevice()->DrawLine( source_point, target_point );

     if ( show_arrowhead )
          DrawArrowHead( );
}





// ----------------------------------------------------------------------------
//                    Arrow::Reposition( )
// ............................................................................
// 
//  this function reposition the arrow so that it points from the source
//  rectangle to the target rectang;e without goiing inside the rectangles
//
// ----------------------------------------------------------------------------

void Arrow::Reposition( const Rectangle& rSourceRect, const Rectangle& rTargetRect )
{
     source_point = MovePoint( rSourceRect,  rTargetRect.Center()); 
     target_point = MovePoint( rTargetRect,  rSourceRect.Center() );

     CalcPolygon();
}  





// ----------------------------------------------------------------------------
//                    Arrow::SizeArrow( )
// ............................................................................
// 
//  this function sets the arrow dimensions
//
// ----------------------------------------------------------------------------

void Arrow::SizeArrow( INT16 Size )
{

     const INT16 ARROW_LENGTH = 1;      // ARROW_LENGTH in character height  
     const INT16 ARROW_ASPECT_RATIO = 2; // ARROW_ASPECT_RATIO is ratio of length to width 

     // define dimensions of arrowhead 
     // note factor of 2 in arrow_halfwidthsince  width is bisected by line

     arrow_length = Size * ARROW_LENGTH;
     arrow_halfwidth = arrow_length / ( 2 * ARROW_ASPECT_RATIO );
     
}     






// ----------------------------------------------------------------------------
//                    Arrow::CalcPolygon( )
// ............................................................................
// 
//  procedure for calculating arrowhead vertices 
//
// ----------------------------------------------------------------------------

void Arrow::CalcPolygon( )
{     
     // the initial point will be the point of the arrowhead

     polygon[0] = target_point;

     short delta_y = source_point.Y() - target_point.Y();
     short delta_x = source_point.X() - target_point.X();

     if ( delta_y == 0 )
     {
          // line is horizontal - a special case 

          if (delta_x < 0 )
          {
               polygon[1] =  target_point - Point( arrow_length, arrow_halfwidth );
               polygon[2] =  target_point - Point( arrow_length, -arrow_halfwidth );
          }
          else
          {
               polygon[1] =  target_point + Point( arrow_length, arrow_halfwidth );
               polygon[2] =  target_point + Point( arrow_length, -arrow_halfwidth );
          }
     }
     else
     {
          if (delta_x == 0 )
          {
               // line is vertical - a special case 

               if (delta_y < 0 )
               {
                    polygon[1] =  target_point - Point( arrow_halfwidth, arrow_length );
                    polygon[2] =  target_point - Point( -arrow_halfwidth, arrow_length );
               }
               else
               {
                    polygon[1] =  target_point + Point( arrow_halfwidth, arrow_length);
                    polygon[2] =  target_point + Point( -arrow_halfwidth, arrow_length );
               }

          }
          else
          {
               // the general case - line is neither horizontal nor vertical

               FLOAT64 alpha = atan2( (FLOAT64) delta_y , (FLOAT64) delta_x );
               FLOAT64 sin_alpha  = sin( alpha );
               FLOAT64 cosin_alpha  = cos( alpha );

               FLOAT64 temp_x = ( cosin_alpha *  arrow_length ) + ( sin_alpha * arrow_halfwidth ) ;
               FLOAT64 temp_y = ( sin_alpha *  arrow_length ) - ( cosin_alpha * arrow_halfwidth );

               polygon[1] = target_point + Point( Round( temp_x), Round( temp_y ) );

               temp_x = ( cosin_alpha *  arrow_length ) - ( sin_alpha * arrow_halfwidth );
               temp_y = ( sin_alpha *  arrow_length ) + ( cosin_alpha * arrow_halfwidth );

               polygon[2] = target_point + Point( Round( temp_x), Round( temp_y ) );

          }
     }
}



// ----------------------------------------------------------------------------
//                    Arrow::DrawArrowHead( )
// ............................................................................
// 
//  procedure for drawing arrowheads 
//
// ----------------------------------------------------------------------------

void Arrow::DrawArrowHead( )
{     

     Brush old_brush = GetpOutputDevice()->ChangeFillInBrush( arrow_fill );
     GetpOutputDevice()->DrawPolygon( polygon );
     GetpOutputDevice()->ChangeFillInBrush( old_brush );
}





// ----------------------------------------------------------------------------
//                    Arrow::MovePoint( )
// ............................................................................
// 
//  this function clips a line that extends from from outside the Rectangle
//  rRect to the center of rRect. The function returns the point at which the
//  line should be clipped to keep it from extendign into rRect.
//
//  eg. this                           gets clipped to this
//
//                 /                                  /
//                /                                  /     
//               /                                  /
//     +--------------+                   +--------X-----+   X = the Point returned
//     |       /      |                   |              | 
//     |      /       |                   |              | 
//     |              |                   |              | 
//     |              |                   |              | 
//     +--------------+                   +--------------+ 
// ----------------------------------------------------------------------------

Point Arrow::MovePoint( const Rectangle& rRect, const Point& rPoint )
{
     const UINT16 LEFT_OF_WINDOW   = (1<<1);
     const UINT16 RIGHT_OF_WINDOW  = (1<<2);
     const UINT16 BELOW_WINDOW     = (1<<3);
     const UINT16 ABOVE_WINDOW     = (1<<4);

     UINT16 area_code;

     // get the relevant values as doubles 

     FLOAT64 x1 = (FLOAT64) rRect.Center().X();
     FLOAT64 y1 = (FLOAT64) rRect.Center().Y();
     FLOAT64 x2 = (FLOAT64) rPoint.X();
     FLOAT64 y2 = (FLOAT64) rPoint.Y();
     FLOAT64 min_x = (FLOAT64) rRect.Left();
     FLOAT64 max_x = (FLOAT64) rRect.Right();
     FLOAT64 max_y = (FLOAT64) rRect.Bottom();   // remember that upper left is (0,0) 
     FLOAT64 min_y = (FLOAT64) rRect.Top();      // remember that upper left is (0,0) 


     // calculate the initial "area code" for the point external to rRect

     area_code = 0;

     if ( x2 < min_x)
          area_code = LEFT_OF_WINDOW;
     else
          if ( x2  > max_x )

               area_code = RIGHT_OF_WINDOW;

     if ( y2 < min_y )
          area_code |= BELOW_WINDOW;
     else
          if ( y2 > max_y )

               area_code |= ABOVE_WINDOW;


     while ( area_code )
     {

          if ( area_code & LEFT_OF_WINDOW )
          {
               // the line crosses the left border of the viewport;
               // calculate the intersection with the left border

               y2 = y1 + ( ( ( y2 - y1 ) / ( x2 - x1 ) ) * (min_x - x1) );
               x2 = min_x;

               goto RecalcCode;
          }

          if (  area_code & RIGHT_OF_WINDOW )
          {
               // the line crosses the right border of the viewport;
               // calculate the intersection with the right border

               y2 = y1 + ( ( ( y2 - y1 ) / ( x2 - x1 ) ) * (max_x - x1) );
               x2 = max_x;

               goto RecalcCode;
          }

          if ( area_code & ABOVE_WINDOW )
          {
               // the line crosses the top border of the viewport;
               // calculate the intersection with the top border

               x2 = x1 + ( ( ( x2 - x1 ) / ( y2 - y1 ) ) * (max_y - y1) );
               y2 = max_y;

               goto RecalcCode;
          }

          if (  area_code & BELOW_WINDOW )
          {
               // the line crosses the bottom border of the viewport;
               // calculate the intersection with the bottom border

               x2 = x1 + ( ( ( x2 - x1 ) / ( y2 - y1 ) ) * (min_y - y1) );
               y2 = min_y;

          }

RecalcCode:

     area_code = 0;

     if ( x2 < min_x )
          area_code = LEFT_OF_WINDOW;
     else
          if ( x2 > max_x )

               area_code = RIGHT_OF_WINDOW;

     if ( y2 < min_y )
          area_code |= BELOW_WINDOW;
     else
          if ( y2 > max_y )

               area_code |= ABOVE_WINDOW;

     }

     // we have successfully pushed the points  and a visible line segment remains
     // return the pushed point after rounding the values 

     return Point( (short) Round( x2 ), (short) Round( y2 ) );
}

