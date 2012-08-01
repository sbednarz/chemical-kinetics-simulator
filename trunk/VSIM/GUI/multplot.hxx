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
//  MULTPLOT.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This files declares the MultiXyPlot class for displaying 
//  multiple x-y plots of simulation results for the VSIM user interface.
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#include "xyplot.hxx"
#include "plotobj.hxx"


#if !defined(__MULTPLOT_HXX__)
     #define __MULTPLOT_HXX__


DECLARE_LIST( XyPlotList, XyPlot* )


class MultiXyPlot : public PlotObject
{
protected:
     XyPlotList              plot_list;
     enum COLOR_SCHEME_TYPE  color_scheme;

public:

                    MultiXyPlot( OutputDevice* pParent );
            virtual ~MultiXyPlot( );

     XyPlot*        AddPlot( );

     virtual void   DrawObject( );
     virtual void   ResizeObject( );
     virtual BOOL8  EditObject( const Point Position );

     enum  COLOR_SCHEME_TYPE GetColorScheme() const { return color_scheme; }
     void  SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme );

     virtual PLOT_OBJECT_TYPE    Type() const { return MULT_XY_PLOT_OBJECT; }

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent );

     BOOL8        SetMaxNumPlotPts( UINT32 NumPts );
};

#endif
 
 