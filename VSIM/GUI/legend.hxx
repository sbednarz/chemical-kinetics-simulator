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
//  LEGEND.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Legendtclass for displaying legends on plots 
//  created in VSIM
//
//  CREATED : 1.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__LEGEND_HXX__)
#define __LEGEND_HXX__


#include "legelmnt.hxx"
#include "plotobj.hxx"
#include "colors.hxx"

DECLARE_LIST( ElementList, LegendElement* )

class Legend : public PlotObject
{
protected:
     ElementList     element_list;

     enum  COLOR_SCHEME_TYPE color_scheme;

     BOOL8             vary_line_style;
     BOOL8             vary_color;
     BOOL8             show_markers;
     BOOL8             show_line;
     BOOL8             vary_marker_style;

public:

                     Legend( OutputDevice* pParent );

                    ~Legend();


     virtual void        DrawObject( );
     virtual void        ResizeObject( );
     virtual BOOL8       EditObject( const Point Position );

     virtual PLOT_OBJECT_TYPE    Type() const { return LEGEND_OBJECT; }

     BOOL8               AddElement( const String& rText );

     void                VaryLineStyle( const BOOL8 State );
     void                VaryColor( const BOOL8 State );
     void                ShowMarkers( const BOOL8 State );
     void                ShowLines( const BOOL8 State );
     void                VaryMarkerStyle( const BOOL8 State );

     void                SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme )  { color_scheme = NewScheme; VaryColor( vary_color); }

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent );
};

#endif
 
 