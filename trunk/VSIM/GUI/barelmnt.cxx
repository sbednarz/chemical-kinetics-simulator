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
//  BARELMNT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the BarGraphElement class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing qunantitative data in a bar graph 
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED :  1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------





#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "barelmnt.hxx"

#endif




const FLOAT64 DEFAULT_FRACTIONAL_BAR_WIDTH = 0.50;
const FLOAT64 DEFAULT_BAR_LENGTH_FLOAT     = 0.0;
const FLOAT64 DEFAULT_MAX_BAR_LENGTH_FLOAT = 1.0;
const Brush  DEFAULT_FILL_BRUSH; 
const Pen    DEFAULT_FILL_PEN;
const Pen    DEFAULT_UNFILL_PEN( PEN_NULL );





// ----------------------------------------------------------------------------
//                  BarGraphElement::BarGraphElement( )
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

BarGraphElement::BarGraphElement( OutputDevice* pParent, const String& rName ) :
PlotObject( pParent ),
bar_width_as_fraction( DEFAULT_FRACTIONAL_BAR_WIDTH ),
bar_length_flt( DEFAULT_BAR_LENGTH_FLOAT ),
max_bar_length_flt( DEFAULT_MAX_BAR_LENGTH_FLOAT ),
bar_width(0),
max_bar_length(0),
filled_bar(),
unfilled_bar(),
fill_brush( DEFAULT_FILL_BRUSH ),
background_brush( GetpParent()->GetBackgroundBrush() ),
fill_pen( DEFAULT_FILL_PEN ),
unfill_pen( DEFAULT_UNFILL_PEN ),
name( rName )
{
     ResizeObject();
}




// ----------------------------------------------------------------------------
//                  BarGraphElement::DrawObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::Paint( const Rectangle& )
{
     // get ready to draw unfilled bar 
     // save the state variables we will modify 

     Brush old_brush = ChangeFillInBrush( background_brush );
     Pen old_pen     = ChangePen( unfill_pen ); 

     DrawRect( unfilled_bar );

     // now draw the filled bar 

     ChangePen( fill_pen ); 
     ChangeFillInBrush( fill_brush );

     DrawRect( filled_bar );

     // restore old state variables

     ChangeFillInBrush( old_brush );
     ChangePen( old_pen ); 
}





// ----------------------------------------------------------------------------
//                  BarGraphElement::SetCurrentBarLength()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void BarGraphElement::SetCurrentBarLength( FLOAT64 Length )
{
     bar_length_flt = Length;

     CalcBarSizes();
     DrawObject();
}






// ----------------------------------------------------------------------------
//                  BarGraphElement::CalcBarSizes()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::CalcBarGeometries()
{
     INT16 unfilled_bar_length = ( 1.0 - (bar_length_flt / max_bar_length_flt) ) * max_bar_length;

    if ( vertical ) 
    {
          Size size( bar_width, unfilled_bar_length );
     
          unfilled_bar.ChangeSize( size  );
     
          filled_bar.ChangePos( unfilled_bar.BottomLeft() );
     
          size.Height() = max_bar_length - unfilled_bar_length;
     
          filled_bar.ChangeSize( size );
    } 
    else 
    {
          Size size( unfilled_bar_length, bar_width );
     
          unfilled_bar.ChangeSize( size  );
     
          size.Width() = max_bar_length - unfilled_bar_length;
     
          filled_bar.ChangeSize( size );

          unfilled_bar.ChangePos( filled_bar.TopRight() );
    } 
}






// ----------------------------------------------------------------------------
//                  BarGraphElement::ResizeObject()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void BarGraphElement::ResizeObject( )
{
     bar_width  = bar_width_as_fraction * active_area.GetWidth();
     max_bar_length = active_area.GetHeight();

     // calc the unfilled_bar rectangle  position 
               
     Point pt( active_area.Left() + ( active_area.GetWidth() - bar_width ) /2, active_area.Top() );
     unfilled_bar.ChangePos( pt );

     CalcBarSizes();

     coordinates_invalid = FALSE;
}











// ----------------------------------------------------------------------------
//                  BarGraphElement::SetBarColor()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::SetBarColor ( enum ColorName NewColor )
{ 
     Color color( NewColor );
     fill_pen.ChangeColor( color ); 
     fill_pen.ChangeColor( color ); 
}

