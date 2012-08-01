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
//  TEXTWIN.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This module declares a TextWindow, specialized for presenting 
//  simulation results in text format
//
//  CREATED : 8.20.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__TEXTWIN_HXX__)
     #define __TEXTWIN_HXX__

#include "linelist.hxx"
#include "displywn.hxx"

const INT16 MAX_RANGE = 100;
const INT16 EXTRA_CHARS_LENGTH = 5;


class TextWindow : public DisplayWindow
{
     const LineList*    pline_list;
     INT16              char_ht;
     INT16              char_width;
     Point              increment;

     UINT16             max_line_length;
     UINT32             total_num_lines;

     UINT32             topmost_line_in_window;
     UINT32             bottom_line_in_window;
     UINT16             leftmost_char_in_window;

     UINT16             window_ht_lines;
     UINT16             window_width_chars;

     UINT16             page_ht_lines;
     UINT16             page_width_chars;

     virtual void       PrintPlot( Printer* pPrinter );
     void               DrawObject( OutputDevice* pDevice );

     void               MoveDown( INT16 nDeltaY );
     void               MoveUp( INT16 nDeltaY );
     void               MoveLeft( INT16 nDeltaX );
     void               MoveRight( INT16 nDeltaX );


public:

                        TextWindow( Window* pParent, LineList* pLineList );

     virtual void       Paint( const Rectangle& );
     virtual void       Resize( );

     virtual void       Scroll( INT16 nDeltaX, INT16 nDeltaY );

     UINT16             GetVerticalPageSize() { return page_ht_lines;}
     UINT16             GetHorizontalPageSize() { return page_width_chars; }
     UINT16             GetVerticalWindowSizeLines() { return window_ht_lines;}
     UINT16             GetHorizontalWindowSizeChars() { return window_width_chars; }

     UINT32             GetCurrentTopLineNum() { return topmost_line_in_window; }
     UINT32             GetCurrentLeftCharNum() { return leftmost_char_in_window; }

     void               PageRight() { Scroll( GetHorizontalPageSize(), 0 );   }
     void               CharRight() { Scroll( 1, 0 ); }
     void               PageLeft()  { Scroll( -GetHorizontalPageSize(), 0 );   } 
     void               CharLeft()  { Scroll( -1, 0 ); }                
     void               PageUp()    { Scroll( 0, -GetVerticalPageSize() );   }
     void               LineUp()    { Scroll( 0, -1 ); }               
     void               LineDown()  { Scroll( 0, 1 );   }  
     void               PageDown()  { Scroll( 0, GetVerticalPageSize() ); }                  

     void               HorizontalDrag( INT16 nDelta );
     void               VerticalDrag( INT16 nDelta );

     virtual Fraction   ChangeScale( const Fraction& rNewScale );
};

#endif
