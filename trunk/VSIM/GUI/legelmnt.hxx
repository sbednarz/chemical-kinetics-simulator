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
//  LEGELMNT.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This module  declares the LegendElement class for
//  the VSIM user interface. This class is derived from 
//  PlotLine and provides a means for specifying and drawing 
//  elements of a plot legend
//
//  CREATED : 1.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__LEGELMNT_HXX__)
     #define __LEGELMNT_HXX__

#include "plotline.hxx"

class LegendElement : public PlotLine
{
private:

     void           DrawTheMarker( const Point Pt );

protected :

     Point          start_pt;
     Point          mid_pt;
     Point          end_pt;
     Point          text_position;
     String         text;

public :
                    LegendElement( OutputDevice* pParent, const String& rText );

     virtual       ~LegendElement();

     virtual void   DrawObject( );
     virtual void   ResizeObject( );
     virtual BOOL8  EditObject( const Point Position );

     virtual PLOT_OBJECT_TYPE    Type() const { return LEGEND_ELEMENT_OBJECT; }
     void           SetText ( const String& rNewText ) { text = rNewText; }

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent )
                            { return PlotLine::ChangepParent( pNewParent ); }
};



#endif

