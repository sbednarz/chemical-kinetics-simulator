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
//  BARDSPLY.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This header file declares the BarDisplay class for
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

#include "bardsply.hxx"

#endif





const FLOAT64 DEFAULT_FRACTIONAL_BAR_WIDTH = 0.50;





// ----------------------------------------------------------------------------
//                  BarDisplay::BarDisplay()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

BarDisplay::BarDisplay( Window* pParent, WinBits WinStyle, const String& rName ) :
Control( pParent ),
bar( this, WinStyle ),
bar_width_as_fraction( DEFAULT_FRACTIONAL_BAR_WIDTH ),
name( rName ),
string_position( ),
vertical( BAR_VERTICAL & WinStyle )
{
     ChangeFont( GetParent()->GetFont() );

     bar.Show();
}







// ----------------------------------------------------------------------------
//                  BarDisplay::Resize()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void BarDisplay::Resize()
{
     Size output_size = GetOutputSizePixel();

     Size text_size   = GetCtrlTextSize( name );


     if ( vertical ) 
     {
          Size size( bar_width_as_fraction * output_size.Width() ,
                     output_size.Height() - ( ( 4*text_size.Height())/3 ) );

          // calc the unfilled_bar rectangle  position 
               
          Point pos( ( output_size.Width() - size.Width() )/2, 0 );

          bar.SetPosSizePixel( pos, size );

          string_position = Point( ( output_size.Width() - text_size.Width() ) /2,  
                                    output_size.Height() - text_size.Height() );

     } 
     else 
     {
          Size size( output_size.Width() - text_size.Width(),
                     bar_width_as_fraction * output_size.Height() );

          // calc the unfilled_bar rectangle  position 
               
          Point pos( text_size.Width(), (output_size.Height() - size.Height())/2 );

          bar.SetPosSizePixel( pos, size );

          string_position = Point( 0, ( output_size.Height() - text_size.Height() )/2 );

     } 
}







// ----------------------------------------------------------------------------
//                  BarDisplay::Paint()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void BarDisplay::Paint( const Rectangle& )
{
     DrawText( string_position, name );
}

