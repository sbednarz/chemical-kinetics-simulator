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
//  BAR.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This header file defines  the Bar class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing qunantitative data to be used in a bar graph 
//
//  CREATED :  2.2.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "bar.hxx"

#endif




const FLOAT64 DEFAULT_FRACTIONAL_BAR_WIDTH = 0.50;
const FLOAT64 DEFAULT_BAR_LENGTH_FLOAT     = 0.0;
const FLOAT64 DEFAULT_MAX_BAR_LENGTH_FLOAT = 1.0;
const Brush  DEFAULT_FILL_BRUSH; 
const Pen    DEFAULT_FILL_PEN;
const Brush  DEFAULT_FRAME_BRUSH( BRUSH_NULL); 
const Pen    DEFAULT_FRAME_PEN;
const Pen    DEFAULT_UNFILL_PEN( PEN_NULL );
const INT16  SPACER_PIXELS                 = 2;







// ----------------------------------------------------------------------------
//                  Bar::Bar( )
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

Bar::Bar( Window* pParent, WinBits WinStyle ) :
Control( pParent ),
max_bar_length_flt( DEFAULT_MAX_BAR_LENGTH_FLOAT ),
bar_length_flt( DEFAULT_BAR_LENGTH_FLOAT ),
bar_width(1),
max_bar_length(1),
filled_bar(),
unfilled_bar(),
fill_brush( DEFAULT_FILL_BRUSH ),
frame_brush( DEFAULT_FRAME_BRUSH ),
background_brush( GetParent()->GetBackgroundBrush() ),
fill_pen( DEFAULT_FILL_PEN ),
frame_pen( DEFAULT_FRAME_PEN ),
unfill_pen( DEFAULT_UNFILL_PEN ),
vertical( BAR_VERTICAL & WinStyle ),
spacer(0)
{

     fill_brush.ChangeFillColor( background_brush.GetColor() );
          if ( vertical)
               spacer = PixelToLogic( Size( SPACER_PIXELS,SPACER_PIXELS ) ).Height() ;
          else
               spacer = PixelToLogic( Size( SPACER_PIXELS,SPACER_PIXELS ) ).Width() ;
}







// ----------------------------------------------------------------------------
//                  Bar::SetMaxBarValue()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::SetMaxBarValue( FLOAT64 MaxLength ) 
{ 
     max_bar_length_flt = MaxLength; 
     CalcBarGeometries(); 
     InvalidateForeground();
} 






// ----------------------------------------------------------------------------
//                  Bar::SetCurrentBarValue()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::SetCurrentBarValue( FLOAT64 Length )
{ 
     // query the current geometry - we will invalidate only a small amount of the
     // window based on the delta 

     INT16 prev_bar_length = vertical ? filled_bar.GetHeight() : filled_bar.GetWidth();

     bar_length_flt = Length; 

     CalcBarGeometries(); 

     INT16 new_bar_length = vertical ? filled_bar.GetHeight() : filled_bar.GetWidth(); 

     if ( new_bar_length > prev_bar_length )
     {
          if ( vertical)
          {
               INT16 top = max( max_bar_length - new_bar_length - spacer , 0 );
               INT16 bottom = min( max_bar_length - prev_bar_length + spacer, max_bar_length );

               InvalidateForeground( Rectangle( 0, top, bar_width, bottom ) );
               Update();
          }
          else
          {
               INT16 right = min( new_bar_length + spacer , max_bar_length );
               INT16 left  = max( prev_bar_length - spacer, 0 );

               InvalidateForeground( Rectangle( left, 0, right, bar_width ) );
          }
     }

     if ( new_bar_length < prev_bar_length )
     {
          if ( vertical)
          {
               INT16 top = max( max_bar_length - prev_bar_length - spacer , 0 );
               INT16 bottom = min( max_bar_length - new_bar_length + spacer, max_bar_length );

               InvalidateForeground( Rectangle( 0, top, bar_width, bottom ) );
               Update();
          }
          else
          {
               INT16 right = min( prev_bar_length + spacer , max_bar_length );
               INT16 left  = max( new_bar_length - spacer, 0 );

               InvalidateForeground( Rectangle( left, 0, right, bar_width ) );
          }
     }
}  






// ----------------------------------------------------------------------------
//                  Bar::Paint( )
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

     // now draw the frame

     ChangePen( frame_pen ); 
     ChangeFillInBrush( frame_brush );

     DrawRect( Rectangle( Point(), GetOutputSize() ) );

     // restore old state variables

     ChangeFillInBrush( old_brush );
     ChangePen( old_pen ); 

}







// ----------------------------------------------------------------------------
//                  Bar::Resize()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::Resize()
{
     Size size = GetOutputSize();

     if ( vertical ) 
     {
          bar_width = size.Width();
          max_bar_length = size.Height();
     } 
     else 
     {
          bar_width = size.Height();
          max_bar_length = size.Width();
     } 

     CalcBarGeometries();
}




// ----------------------------------------------------------------------------
//                  Bart::CalcBarGeometries()
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
//                  Bart::SetBarColor()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Bar::SetBarColor ( enum ColorName NewColor )
{ 
     Color color( NewColor );
     fill_brush.ChangeColor( color ); 
     fill_pen.ChangeColor( color ); 
}

