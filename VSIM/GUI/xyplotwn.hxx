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
//  XYPLOTWN.HXX - a header file for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a Window specialized for presenting XY plots 
//
//  CREATED : 8.15.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__XYPLOTWN_HXX__)
     #define __XYPLOTWN_HXX__


#include <sv.hxx>
#pragma hdrstop

#include "multplot.hxx"
#include "rxnschem.hxx"
#include "xydatfil.hxx"
#include "displywn.hxx"



class XyPlotWindow : public DisplayWindow
{
protected:
     MultiXyPlot                plot;
     XyDataFile                 ext_data;
     ReactionScheme&            r_scheme;

     void                       MouseButtonDown( const MouseEvent& rMEvt );
     void                       Initialize();

     virtual void               PrintPlot( Printer* pPrinter );

     ReactionScheme&            GetScheme() const { return r_scheme; }


public:
                                XyPlotWindow( Window* pParent, ReactionScheme& rScheme );
                               ~XyPlotWindow( );

     XyPlot*                    AddPlot( ) { return plot.AddPlot(); }

     virtual void               Paint( const Rectangle& );
     virtual void               Resize( );

     XyDataFile&                GetExternalDataFile() { return ext_data; }

     void                       EditAttributes( );

     BOOL8                      SetMaxNumPlotPts( UINT32 NumPts ) { return plot.SetMaxNumPlotPts( NumPts ); }

     virtual COLOR_SCHEME_TYPE  GetColorScheme() const;
     virtual void               SetColorScheme( enum COLOR_SCHEME_TYPE NewScheme );
  
};


#endif
