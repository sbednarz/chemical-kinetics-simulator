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
//  WFPLOTWN.HXX - a header file for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a Window specialized for presenting waterfall plots 
//
//  CREATED : 6.09.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__WFPLOTWN_HXX__)
     #define __WFPLOTWN_HXX__


#include <sv.hxx>
#pragma hdrstop

#include "stakplot.hxx"
#include "rxnschem.hxx"
#include "xydatfil.hxx"
#include "displywn.hxx"



class WaterfallPlotWindow : public DisplayWindow
{
protected:
     StackedXyPlot              plot;
     XyDataFile                 ext_data;
     ReactionScheme&            r_scheme;

     void                       MouseButtonDown( const MouseEvent& rMEvt );
     void                       Initialize();

     virtual void               PrintPlot( Printer* pPrinter );

     ReactionScheme&            GetScheme() const { return r_scheme; }


public:
                                WaterfallPlotWindow( Window* pParent, ReactionScheme& rScheme );
                               ~WaterfallPlotWindow( );

     virtual void               Paint( const Rectangle& );
     virtual void               Resize( );

     XyDataFile&                GetExternalDataFile() { return ext_data; }

     void                       EditAttributes( );

     BOOL8                      SetMaxNumPlotPts( UINT32 NumPts ) { return plot.SetMaxNumPlotPts( NumPts ); }

     virtual COLOR_SCHEME_TYPE  GetColorScheme() const;
     virtual void               SetColorScheme( enum COLOR_SCHEME_TYPE NewScheme );
  
     StackedXyPlot&             GetPlot() { return plot;}
};


#endif
