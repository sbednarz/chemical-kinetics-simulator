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
//  PLOTLINE.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module  defines the PlotLine class for
//  the VSIM user interface. This class provides for the 
//  capability for drawing connecting lines and  discrete data points 
//
//  CREATED : 1.19.96 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "plotline.hxx"
#include "linestyl.hxx"

#endif



const enum ColorName DEFAULT_COLOR        = COL_BLACK;
const UINT16         DEFAULT_LINEWIDTH    = 0;
const Size           DEFAULT_MARKER_SIZE( 10,10 );
const FLOAT64        DEFAULT_MARKER_SCALING_FACTOR  = 0.60;



// ----------------------------------------------------------------------------
//                             PlotLine::PlotLine( )
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

PlotLine::PlotLine( OutputDevice* pParent ) :
PlotObject( pParent),
show_points( TRUE ),
show_lines( TRUE ),
marker_style( DEFAULT_MARKER_STYLE ),
pen( Color( DEFAULT_COLOR), DEFAULT_LINEWIDTH, DEFAULT_LINE_STYLE ),
marker_size(  DEFAULT_MARKER_SIZE ),
marker_scaling_factor( DEFAULT_MARKER_SCALING_FACTOR )
{
}





// ----------------------------------------------------------------------------
//                             PlotLine::~PlotLine( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

PlotLine::~PlotLine()
{
}












// ----------------------------------------------------------------------------
//                     PlotLine::DrawMarker( )
// ............................................................................
// 
//  Draws a single marker in the current color and style at Point Pt
//
// ----------------------------------------------------------------------------

void PlotLine::DrawMarker( const Point Pt )
{

     Brush old_brush = GetpParent()->GetFillInBrush( );

     switch ( marker_style) 
     {
     case MARKER_SOLIDDIAMOND :
     case MARKER_SOLIDSQUARE :
     case MARKER_SOLIDCIRCLE :

          GetpParent()->ChangeFillInBrush( Brush( pen.GetColor( ), BRUSH_SOLID ) );
          break;

     default:

          GetpParent()->ChangeFillInBrush( Brush(BRUSH_NULL) );
          break;

     } 


     DrawTheMarker( Pt );

     GetpParent()->ChangeFillInBrush( old_brush );
}





// ----------------------------------------------------------------------------
//                     PlotLine::DrawTheMarker()
// ............................................................................
// 
//  Draws a single marker in the current color and style at Point Pt. Do not
// call this one directly, call DrawMarker instead
//
// ----------------------------------------------------------------------------

void PlotLine::DrawTheMarker( const Point Pt )
{
     // draw the figure here

     const INT16 MARKER_WIDTH = marker_size.Width();
     const INT16 MARKER_HT    = marker_size.Height();

     switch ( marker_style )
     {

     case MARKER_CROSS :
          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Pt + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) );
          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, MARKER_HT / 2 ), Pt + Point( MARKER_WIDTH / 2, - MARKER_HT / 2 ) );

          break;

     case MARKER_CIRCLE :
     case MARKER_SOLIDCIRCLE :

          GetpParent()->DrawEllipse( Rectangle( Pt + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Pt + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) ) );

          break;

     case MARKER_STAR6 :

          //vertical

          GetpParent()->DrawLine( Pt - Point( 0, MARKER_HT / 2 ),
               Pt + Point( 0, MARKER_HT / 2 )
          );

          // ll to ur

          GetpParent()->DrawLine( Pt - Point( MARKER_WIDTH / 2, MARKER_HT / 2 ),
               Pt + Point( MARKER_WIDTH / 2, MARKER_HT / 2 )
          );

          // lr to ul

          GetpParent()->DrawLine( Pt - Point( -( MARKER_WIDTH / 2 ), MARKER_HT / 2 ),
               Pt + Point( -( MARKER_WIDTH / 2 ), MARKER_HT / 2 )
          );

          break;

     case MARKER_STAR8 :

          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Pt + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) );
          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, MARKER_HT / 2 ), Pt + Point( MARKER_WIDTH / 2, - MARKER_HT / 2 ) );

          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, 0 ), Pt + Point( MARKER_WIDTH / 2, 0 ) );
          GetpParent()->DrawLine( Pt + Point( 0, - MARKER_HT / 2 ), Pt + Point( 0, MARKER_HT / 2 ) );

          break;

     case MARKER_SQUARE :
     case MARKER_SOLIDSQUARE :

          GetpParent()->DrawRect( Rectangle( Pt + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Size( MARKER_WIDTH, MARKER_HT ) ) );

          break;

     case MARKER_DIAMOND :
     case MARKER_SOLIDDIAMOND :
     {
          Point pt_array[4];

          pt_array[0] = Pt + Point( - MARKER_WIDTH / 2, 0 );
          pt_array[1] = Pt + Point( 0, - MARKER_HT / 2 );
          pt_array[2] = Pt + Point( MARKER_WIDTH / 2, 0 );
          pt_array[3] = Pt + Point( 0, MARKER_HT / 2 );

          GetpParent()->DrawPolygon( Polygon( 4, pt_array ) );

          break;
     }


     case MARKER_PLUS :
     default :
          GetpParent()->DrawLine( Pt + Point( - MARKER_WIDTH / 2, 0 ), Pt + Point( MARKER_WIDTH / 2, 0 ) );
          GetpParent()->DrawLine( Pt + Point( 0, - MARKER_HT / 2 ), Pt + Point( 0, MARKER_HT / 2 ) );

          break;
     }    

}





// ----------------------------------------------------------------------------
//                     PlotLine::DrawMarkers( )
// ............................................................................
// 
//  Draws a series of markers in the current color and style at each Point in 
//  the Polygon rPoly
//
// ----------------------------------------------------------------------------

void PlotLine::DrawMarkers( const Polygon& rPoly )
{

     Brush old_brush = GetpParent()->GetFillInBrush( );

     switch ( marker_style) 
     {
     case MARKER_SOLIDDIAMOND :
     case MARKER_SOLIDSQUARE :
     case MARKER_SOLIDCIRCLE :

          GetpParent()->ChangeFillInBrush( Brush( pen.GetColor( ), BRUSH_SOLID ) );
          break;

     default:

          GetpParent()->ChangeFillInBrush( Brush(BRUSH_NULL) );
          break;

     } 

     UINT16 i = 0;
     const UINT16 num_points = rPoly.GetSize();

     while ( i < num_points )
          DrawTheMarker( rPoly[i++] );

     GetpParent()->ChangeFillInBrush( old_brush );
}


