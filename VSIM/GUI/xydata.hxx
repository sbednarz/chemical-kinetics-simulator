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
//  XYDATA.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the XyData class, to be used contain an array of 
//  X-Y data points in a form suiltable for plotting ( this include conversion 
//  from floating point to integer form, clipping, etc.
//
//  CREATED : 6.26.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if !defined(__XYDATA_HXX__)
#define __XYDATA_HXX__

#include <sv.hxx>

#include "flotarry.hxx"
#include "limits.hxx"

class XyData
{
private:
     Polygon                  line_polygon;
     Polygon                  point_polygon;

     UINT16                   max_num_plot_pts;
     UINT16                   interval;

     FLOAT64                  x_scaling_factor;
     FLOAT64                  y_scaling_factor;

     const FloatArray&        rx;   // contains ref to X data in floating pt format 
     const FloatArray&        ry;   // contains ref to Y data in floating pt format 

     Limits                   x_limits;
     Limits                   y_limits;

     Limits                   init_x_limits;
     Limits                   init_y_limits;

     Rectangle                bounding_rect;

     BOOL8                    line_polygon_invalid;
     BOOL8                    point_polygon_invalid;

     void                     ScaleDataForLinePlot( );
     void                     ScaleDataForPointPlot( );

     UINT32                   GetStepInterval( );
     UINT32                   GetNumXYDataPoints() const { return rx.GetArraySize(); }

     BOOL8                    VisibleLineSegment( FLOAT64& rX1, FLOAT64& rY1, 
                               FLOAT64& rX2, FLOAT64& rY2, UINT16 AreaCode1, 
                               UINT16 AreaCode2 );

     UINT16                   AreaCode( const UINT32 Index ) const;
     UINT16                   AreaCode( const FLOAT64 X, const FLOAT64 Y ) const;

     void                     CalcScalingFactors();

     Point                    ScaleData( UINT32 Index ) const;
     Point                    ScaleData( FLOAT64 X, FLOAT64 Y ) const;

public:
                  XyData( const FloatArray& rX, const FloatArray& rY );

     Rectangle    ChangeBoundingRectangle( const Rectangle& rRect );

     void         SetXDataLimits( const Limits& NewLimits ) 
                  { 
                       x_limits = NewLimits;
                       Invalidate();
                  }

     void         SetYDataLimits( const Limits& NewLimits )  
                  { 
                       y_limits = NewLimits;
                       Invalidate();
                  } 

     void         ResetDataLimits();

	void         Invalidate( ) { point_polygon_invalid = line_polygon_invalid = TRUE; }

     const Limits& GetXDataLimits() const { return x_limits; }
     const Limits& GetYDataLimits() const { return y_limits; }

     BOOL8        SetMaxNumPlotPts( UINT32 NumPts );
     UINT32       GetMaxNumPlotPts() { return max_num_plot_pts; }
     UINT32       GetNumberOfDataPairs() { return rx.GetArraySize(); }

     const Polygon& AsPolyLine();
     const Polygon& AsDiscretePoints();

};

#endif

