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
//  XYDATPLT.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This module  declares the XyDataPlot class for
//  the VSIM user interface. This class provides for the
//  drawing of a set of X, Y data points on an OutputDevice
//  It includes capability for drawing connecting lines and
//  discrete data points
//
//  CREATED : 7.13.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__XYDATPLT_HXX__)
     #define __XYDATPLT_HXX__

#include "plotline.hxx"
#include "xydata.hxx"
#include "markers.hxx"

class XyDataPlot : public PlotLine
{
protected :
     XyData         data;

     void           DrawPolyMarker();
     void           DrawPolyLine();

public :
                    XyDataPlot( OutputDevice* pParent, const FloatArray& rX, 
                     const FloatArray& rY );

     virtual       ~XyDataPlot();

     virtual void   DrawObject( );
     virtual void   ResizeObject( );
     virtual BOOL8  EditObject( const Point Position );

     BOOL8           SetMaxNumPlotPts( UINT32 NumPts ) { return data.SetMaxNumPlotPts( NumPts ); }
     UINT32          GetMaxNumPlotPts() { return data.GetMaxNumPlotPts( ); }
     UINT32          GetNumberOfDataPairs( ) { return data.GetNumberOfDataPairs(); }

     virtual PLOT_OBJECT_TYPE    Type() const { return XY_DATA_OBJECT; }

     void           SetXDataLimits( const Limits& NewLimits ) { data.SetXDataLimits( NewLimits ); }
     void           SetYDataLimits( const Limits& NewLimits ) { data.SetYDataLimits( NewLimits ); }

     void           ResetLimits() { data.ResetDataLimits(); }
     const Limits&  GetXDataLimits( ) { return data.GetXDataLimits( ); }
     const Limits&  GetYDataLimits( ) { return data.GetYDataLimits( ); }
};



#endif

