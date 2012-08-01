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
//  PLOTLINE.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This module  declares the PlotLine class for
//  the VSIM user interface. This class provides the
//  capability for drawing  lines and discrete data points
//  on an OutputDevice
//
//  CREATED : 1.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__PLOTLINE_HXX__)
     #define __PLOTLINE_HXX__

#include "plotobj.hxx"
#include "markers.hxx"

class PlotLine : public PlotObject 
{
private:

     void           DrawTheMarker( const Point Pt );

protected :

     BOOL8          show_points;
     BOOL8          show_lines;
     MARKER_STYLE   marker_style;
     Pen            pen;
     Size           marker_size;
     FLOAT64        marker_scaling_factor;

     void           DrawMarker( const Point Pt );
     void           DrawMarkers( const Polygon& rPoly );


public :
                    PlotLine( OutputDevice* pParent );

     virtual       ~PlotLine();

     void           ShowPoints( const BOOL8 NewState ) { show_points = NewState; }
     void           ShowLines( const BOOL8 NewState ) { show_lines = NewState; }

     void           SetMarkerStyle( enum MARKER_STYLE NewStyle ) { marker_style = NewStyle; }
     void           SetMarkerSize( const Size NewSize ) { marker_size = NewSize; }
     void           SetMarkerScaleFactor( const FLOAT64 NewScale ) { marker_scaling_factor = NewScale; }

     void           SetColor ( enum ColorName NewColor ) { pen.ChangeColor( NewColor ); }
     void           SetLineStyle ( enum PenStyle NewStyle ) { pen.ChangeStyle( NewStyle) ; }


};



#endif

