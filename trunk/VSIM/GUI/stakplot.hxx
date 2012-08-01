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
//  STAKPLOT.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This files declares the StackedXyPlot class for displaying a
//  waterfall-style x-y plot of simulation results for the VSIM user interface.
//
//  CREATED : 7.20.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#include "xyplot.hxx"
#include "plotobj.hxx"
#include "zaxis.hxx"


#if !defined(__STAKPLOT_HXX__)
     #define __STAKPLOT_HXX__

DECLARE_LIST( PlotNameList, String* )

class StackedXyPlot : public XyPlot
{
private:
     ZAxis          z_axis;
     BOOL8          labels_need_refresh;
     PlotNameList   name_list;

     void           RefreshLabels();

public:

                    StackedXyPlot( OutputDevice* pParent );
            virtual ~StackedXyPlot( );

     virtual void   DrawObject( );
     virtual void   ResizeObject( );
     virtual BOOL8  EditObject( const Point Position );
     virtual void   SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme );

     BOOL8          AddDataSet( const FloatArray& rX, const FloatArray& rY, const String& rName );

     virtual PLOT_OBJECT_TYPE    Type() const { return STACKED_XY_PLOT_OBJECT; }

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent );

};


#endif

