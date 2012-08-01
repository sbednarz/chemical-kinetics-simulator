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
//  COLORS.HXX - a header file for use in the VSIM User interface
// ............................................................................
//
//  This module declares color scehemes for use in displaying simulation
//  results in VSIM
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__COLORS_HXX__)
     #define __COLORS_HXX__


#include "vsimtype.hxx"
#include <sv.hxx>


const UINT16 NUM_COLORS = 7;
const UINT16 NUM_COLOR_SCHEMES = 7;


enum  COLOR_SCHEME_TYPE { COL_SCHEME_BLACK, COL_SCHEME_WHITE, COL_SCHEME_GRAY,
                          COL_SCHEME_CYAN, COL_SCHEME_MONO_WHITE,
                          COL_SCHEME_MONO_BLACK, COL_SCHEME_PAPER };



const enum COLOR_SCHEME_TYPE DEFAULT_COLOR_SCHEME = COL_SCHEME_WHITE;


enum COLOR_SCHEME_TYPE GetColorSchemeFromName( const String& rName );

String GetNameFromColorScheme( enum COLOR_SCHEME_TYPE Type );


struct COLOR_STRUCT
{
public:
     enum ColorName background_color;
     enum ColorName axis_color;
     enum ColorName line_color[ NUM_COLORS ];

     enum ColorName GetBackgroundColor( ) const { return background_color; }
     enum ColorName GetAxisColor( ) const { return axis_color; }
     enum ColorName GetDataColor( UINT16 I ) const { return line_color[ I % NUM_COLOR_SCHEMES]; }
     enum ColorName GetDefaultDataColor( ) const { return line_color[ 0 ]; }
     enum ColorName GetTextColor( ) const { return axis_color; }

};

extern const struct COLOR_STRUCT ColorScheme[ NUM_COLOR_SCHEMES ];


#endif

