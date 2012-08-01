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
//  XYDATPLT.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module  defines the XyDataPlot class for
//  the VSIM user interface. This class provides for the 
//  drawing of a set of X, Y data points on an OutputDevice
//  It includes capability for drawing connecting lines and 
//  discrete data points 
//
//  CREATED : 7.13.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "xydatplt.hxx"
#include "math.hxx"

#endif


const UINT16         POLYLINE_LIMIT       = 8000;

// ----------------------------------------------------------------------------
//                     XyDataPlot::XyDataPlot( )
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

XyDataPlot::XyDataPlot( OutputDevice* pParent, const FloatArray& rX, 
const FloatArray& rY ) :
PlotLine( pParent),
data( rX, rY )
{
}





// ----------------------------------------------------------------------------
//                     XyDataPlot::~XyDataPlot( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

XyDataPlot::~XyDataPlot()
{
}





// ----------------------------------------------------------------------------
//                     XyDataPlot::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 XyDataPlot::EditObject( const Point )
{
     return FALSE;
}





// ----------------------------------------------------------------------------
//                     XyDataPlot::DrawPolyLine( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyDataPlot::DrawPolyLine()
{
     Pen old_pen = GetpParent()->ChangePen( pen );

     const Polygon& rpoly = data.AsPolyLine();

     UINT16 num_points = rpoly.GetSize();

     if ( num_points <= POLYLINE_LIMIT )
          GetpParent()->DrawPolyLine( rpoly );
     else
     {
          UINT16 i;

          Point src( rpoly[0] );
          Point dest;

          for ( i = 1; i <  num_points; i++ )
          {
               dest = rpoly[i];

               GetpParent()->DrawLine( src, dest );

               src = dest;        // get ready for next segment 
          }
     }

      GetpParent()->ChangePen( old_pen );
}



// ----------------------------------------------------------------------------
//                     XyDataPlot::DrawPolyMarker()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void XyDataPlot::DrawPolyMarker()
{
     // we need to use solid lines for markers 

     Pen saved_pen = pen; // save pen state as we will restore when done

     pen.ChangeStyle( PEN_SOLID );

     Pen old_pen = GetpParent()->ChangePen( pen );

     const Polygon& rpoly = data.AsDiscretePoints();

     UINT16 num_points = rpoly.GetSize();

     if ( num_points <= POLYLINE_LIMIT )
          DrawMarkers( rpoly );
     else
     {
          UINT16 i;

          Point src( rpoly[0] );
          Point dest;

          for ( i = 1; i <  num_points; i++ )
          {
               dest = rpoly[i];

               GetpParent()->DrawLine( src, dest );

               src = dest;        // get ready for next segment 
          }
     }

      GetpParent()->ChangePen( old_pen );

      pen = saved_pen;
}




// ----------------------------------------------------------------------------
//                     XyDataPlot::DrawObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyDataPlot::DrawObject( )
{
     if ( show_points)
          DrawPolyMarker( );

     if ( show_lines )
          DrawPolyLine( );

     if ( highlight )
          HighlightArea();

}




// ----------------------------------------------------------------------------
//                     XyDataPlot::ResizeObject( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void XyDataPlot::ResizeObject( )
{
     data.ChangeBoundingRectangle( active_area );

     Size font_size = GetpParent()->GetFont().GetSize();

     INT16 val =  Round( marker_scaling_factor * font_size.Height() );
     marker_size = Size( val, val );
}







