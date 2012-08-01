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
//  LEGELMNT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module  defines the LegendElement class for
//  the VSIM user interface. This class is derived from
//  PlotLine and provides a means for specifying and drawing
//  elements of a plot legend
//
//  CREATED : 1.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "legelmnt.hxx"
#include "math.hxx"

#endif




// ----------------------------------------------------------------------------
//                     LegendElement::LegendElement()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

LegendElement::LegendElement( OutputDevice* pParent, const String& rText ):
PlotLine( pParent),
start_pt(),
mid_pt(),
end_pt(),
text_position(),
text( rText )
{
}





// ----------------------------------------------------------------------------
//                     LegendElement::~LegendElement()
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

LegendElement::~LegendElement()
{
}




// ----------------------------------------------------------------------------
//                     LegendElement::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 LegendElement::EditObject( const Point )
{
     return FALSE;
}








// ----------------------------------------------------------------------------
//                     XyDataPlot::DrawObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void LegendElement::DrawObject( )
{
     // use solid line for text and markers 
     // save old style and original parent's pen while we are at it 

     enum PenStyle style = pen.ChangeStyle( PEN_SOLID );

     Pen old_pen = GetpParent()->ChangePen( pen );

     GetpParent()->DrawText( text_position, text ); 

     if ( show_points)
     {
          DrawMarker( start_pt );
          DrawMarker( mid_pt );
          DrawMarker( end_pt );
     }

     // restore the style we started with 

     pen.ChangeStyle( style );
     GetpParent()->ChangePen( pen );

     if ( show_lines )
          GetpParent()->DrawLine( start_pt, end_pt );


     if ( highlight )
          HighlightArea();

      GetpParent()->ChangePen( old_pen );
}




// ----------------------------------------------------------------------------
//                     XyDataPlot::ResizeObject( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void LegendElement::ResizeObject( )
{
     const INT16 font_ht = GetpParent()->GetFont().GetSize().Height();

     const INT16 val =  Round( marker_scaling_factor * font_ht );
     marker_size = Size( val, val );

     const INT16 line_offset = (12 * font_ht) / 10;

     
     start_pt      = active_area.TopLeft() + Point( 2 * val,  line_offset );
     end_pt        = active_area.TopRight() + Point( -2 * val,  line_offset ); 
     mid_pt        = ( start_pt + end_pt ) / 2;
     text_position = active_area.TopLeft() + Point( 2 * val , 0 ); 
}







