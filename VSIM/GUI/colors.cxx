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
//  COLORS.CXX - a module for use in the VSIM User interface
// ............................................................................
//
//  This module defines color schemes for use in displaying simulation
//  results in VSIM
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "colors.hxx"

#endif



#include "strings.hrc"



const struct COLOR_STRUCT ColorScheme[ NUM_COLOR_SCHEMES ] =
{
     // black bkgrd 

     {    
          COL_BLACK, 
          COL_WHITE, 
          {
               COL_YELLOW, 
               COL_LIGHTRED, 
               COL_LIGHTBLUE, 
               COL_LIGHTGREEN, 
               COL_LIGHTCYAN, 
               COL_LIGHTMAGENTA, 
               COL_WHITE
          }
     },

     // white bkgrd 

     {
          COL_WHITE, 
          COL_BLACK, 
          {
               COL_RED, 
               COL_BLUE, 
               COL_GREEN, 
               COL_CYAN, 
               COL_BROWN, 
               COL_MAGENTA, 
               COL_BLACK
          }
     },

     //  gray bkgrd 

     {
          COL_LIGHTGRAY, 
          COL_BLACK, 
          {
               COL_RED, 
               COL_BLUE, 
               COL_GREEN, 
               COL_CYAN, 
               COL_BROWN, 
               COL_MAGENTA, 
               COL_BLACK
          }
     },

     // cyan bkgrd  

     {
          COL_CYAN, 
          COL_WHITE, 
          {
               COL_YELLOW, 
               COL_LIGHTRED, 
               COL_BLACK, 
               COL_LIGHTGREEN, 
               COL_LIGHTCYAN, 
               COL_LIGHTMAGENTA, 
               COL_WHITE
          }
     },

     // white on black 

     {
          COL_BLACK, 
          COL_WHITE, 
          {
               COL_WHITE, 
               COL_WHITE, 
               COL_WHITE, 
               COL_WHITE, 
               COL_WHITE, 
               COL_WHITE, 
               COL_WHITE
          }
     },

     // black on white 

     {
          COL_WHITE, 
          COL_BLACK, 
          {
               COL_BLACK, 
               COL_BLACK, 
               COL_BLACK, 
               COL_BLACK,
               COL_BLACK, 
               COL_BLACK, 
               COL_BLACK
          }
     },

     // paper color set 

     {
          COL_WHITE, 
          COL_BLACK, 
          {
               COL_RED, 
               COL_BLUE,
               COL_GREEN, 
               COL_CYAN, 
               COL_BROWN, 
               COL_MAGENTA, 
               COL_BLACK
          }
     }
};


enum COLOR_SCHEME_TYPE GetColorSchemeFromName( const String& rName )
{
     if ( rName.Len() )
     {
          if ( String( ResId( vID_STR_COLOR_SCHEME_BLACK )) == rName )
               return COL_SCHEME_BLACK;
          else
          if ( String( ResId( vID_STR_COLOR_SCHEME_WHITE )) == rName )
               return COL_SCHEME_WHITE;
          else
          if ( String( ResId( vID_STR_COLOR_SCHEME_GRAY )) == rName )
               return COL_SCHEME_GRAY;
          else
          if ( String( ResId( vID_STR_COLOR_SCHEME_CYAN )) == rName )
               return COL_SCHEME_CYAN;
          else
               return DEFAULT_COLOR_SCHEME;

     }
     else
          return DEFAULT_COLOR_SCHEME;

}



String GetNameFromColorScheme( enum COLOR_SCHEME_TYPE Type )
{
     String name;

     switch ( Type )
     {
     case  COL_SCHEME_BLACK :

          name = String( ResId( vID_STR_COLOR_SCHEME_BLACK ));
          break;

     case COL_SCHEME_WHITE :

          name = String( ResId( vID_STR_COLOR_SCHEME_WHITE ));
          break;

     case  COL_SCHEME_CYAN :

          name =  String( ResId( vID_STR_COLOR_SCHEME_CYAN ));
          break;

     case  COL_SCHEME_GRAY :

          name =  String( ResId( vID_STR_COLOR_SCHEME_GRAY ));
          break;

     default :

          name = GetNameFromColorScheme( DEFAULT_COLOR_SCHEME );
          break;
     }

     return name;
}



