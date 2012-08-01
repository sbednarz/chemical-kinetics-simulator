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
//  XYPLOT.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This module defines the XyPlot class for use in displaying simulation
//  results in VSIM
//
//  CREATED : 7.14.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__XYPLOT_HXX__)
#define __XYPLOT_HXX__

#include "flotarry.hxx"
#include "xydatplt.hxx"
#include "xyaxis.hxx"
#include "plottitl.hxx"
#include "colors.hxx"
#include "legend.hxx"



DECLARE_LIST( DataPlotList, XyDataPlot* )

class XyPlot : public PlotObject
{
protected:
     enum  COLOR_SCHEME_TYPE color_scheme;

     BOOL8             x_limits_data_changed;
     BOOL8             y_limits_data_changed;
     BOOL8             vary_line_style;
     BOOL8             vary_color;
     BOOL8             show_markers;
     BOOL8             show_line;
     BOOL8             vary_marker_style;
     BOOL8             show_rect;

     Rectangle         bounding_rect;

     DataPlotList      xydata_list;
     XAxis             x_axis;
     YAxis             y_axis;

     PlotTitle         title;
     Legend            legend;

     Limits            x_data_limits;  // always contains limits that enclose all the data sets
     Limits            y_data_limits;

     void              RecalcLimits();
     void              RecalcXAxisLimits();
     void              RecalcYAxisLimits();
     void              SynchXAxisAndDataPlotLimits();
     void              SynchYAxisAndDataPlotLimits();
     void              ValidateLimits( Limits& rLimits );


public:
                         XyPlot( OutputDevice* pParent );

    virtual             ~XyPlot( );

     BOOL8               AddDataSet( const FloatArray& rX, const FloatArray& rY, const String& rName );

     virtual void        DrawObject( );
     virtual void        ResizeObject( );
     virtual BOOL8       EditObject( const Point Position );

     enum AXIS_RC        SetXAxisParameters( const Limits& rNewLimits, 
                            const FLOAT64 NewTicInterval, const UINT16 Precision );

     enum AXIS_RC        SetYAxisParameters( const Limits& rNewLimits,
                            const FLOAT64 NewTicInterval, const UINT16 Precision );

     void                VaryLineStyle( const BOOL8 State );
     void                VaryColor( const BOOL8 State );
     void                ShowMarkers( const BOOL8 State );
     void                ShowLines( const BOOL8 State );
     void                VaryMarkerStyle( const BOOL8 State );
     void                ShowBoundingRectangle( const BOOL8 State ) { show_rect = State; }

     // general inline functions 

     virtual PLOT_OBJECT_TYPE    Type() const { return XY_PLOT_OBJECT; }

     void                SetPlotTitle( const String& rStr ) { title.SetTitle( rStr ); }

     // x-axis inline functions 

     void                SetXAxisTitle( const String& rStr ) { x_axis.SetTitle( rStr ); }
     void                SetXAxisUnits( const String& rStr ) {x_axis.SetUnits( rStr ); }

     const Limits        GetXAxisLimits() const { return x_axis.GetAxisLimits( ); }

     FLOAT64             GetXAxisTicInterval()  const { return x_axis.GetTicInterval(); }
     UINT16              GetXAxisTicLabelPrecision()    const { return x_axis.GetTicLabelPrecision(); }
     UINT16              GetXAxisTicLabelTotalNumDigits() const { return x_axis.GetTicLabelTotalNumDigits(); }


     void                ResetXAxisParameters( ) { RecalcXAxisLimits(); }

     void                ShowXAxisGrid( BOOL8 State ) { x_axis.ShowGrid( State ); }

     // y-axis inline functions 

     void                SetYAxisTitle( const String& rStr ) { y_axis.SetTitle( rStr ); }
     void                SetYAxisUnits( const String& rStr ) {y_axis.SetUnits( rStr ); }

     const Limits        GetYAxisLimits() const { return y_axis.GetAxisLimits( ); }

     FLOAT64             GetYAxisTicInterval()  const { return y_axis.GetTicInterval(); }
     UINT16              GetYAxisTicLabelPrecision()    const { return y_axis.GetTicLabelPrecision(); }
     UINT16              GetYAxisTicLabelTotalNumDigits() const { return y_axis.GetTicLabelTotalNumDigits(); }

     void                ResetYAxisParameters( ) { RecalcYAxisLimits(); }

     void                ShowYAxisGrid( BOOL8 State ) { y_axis.ShowGrid( State ); }

     void                ResetLimits();

     BOOL8               VaryLineStyle( )   const { return vary_line_style;   }
     BOOL8               VaryColor( )       const { return vary_color;        }
     BOOL8               ShowMarkers( )     const { return show_markers;      }
     BOOL8               ShowLines( )       const { return show_line;         }
     BOOL8               VaryMarkerStyle( ) const { return vary_marker_style; }
     BOOL8               ShowBoundingRectangle( ) const { return show_rect;   }

     enum  COLOR_SCHEME_TYPE GetColorScheme() const { return color_scheme; }

     virtual void        SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme );

     BOOL8               SetMaxNumPlotPts( UINT32 NumPts );
     UINT32              GetMaxNumPlotPts();
     UINT32              GetNumberOfDataPairs();

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent );
};

#endif

