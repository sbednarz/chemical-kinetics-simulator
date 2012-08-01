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
//  LINELIST.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module file defines the class LineList, a generic container for a 
//  set of Strings
//
//  CREATED : 8.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>
#include "linelist.hxx"

#endif






const static String MLE_EOL( MLE_END_OF_LINE );
const static String TEXTFILE_EOL( FILE_END_OF_LINE );







// ----------------------------------------------------------------------------
//                LineList::LineList()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

LineList::LineList() : 
List(), 
recalc_needed( TRUE ),
overall_num_chars( 0 ),
max_line_length( 0 )
{
}



// ----------------------------------------------------------------------------
//                LineList::~LineList()
// ............................................................................
//
//   destructor
//
// ----------------------------------------------------------------------------

LineList::~LineList()
{
     UINT32  count = GetNumLines();

     for ( UINT32 i = 0 ; i < count; i++)
          delete ((String*) GetObject( i ));
}







// ----------------------------------------------------------------------------
//                LineList::AddAnotherLine()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

String* LineList::AddAnotherLine( )
{
     String* pstr = new String;

     if ( pstr )
     {
          recalc_needed = TRUE;
          Insert( pstr, LIST_APPEND );
     }

     return pstr;
}





// ----------------------------------------------------------------------------
//                LineList::CalcLengths()
// ............................................................................
//
//   calculates the overall length of text that comprises the table 
//   beginning and end to fill it out - we do it as a UINT32 so we 
//   can use the value to determine whether the sum exceeds STRING_MAXLEN 
//
// ----------------------------------------------------------------------------

void LineList::CalcLengths( ) 
{
     UINT32  count = GetNumLines();


     overall_num_chars = 0;
     max_line_length   = 0;

     for ( UINT32 i = 0 ; i < count; i++)
     {
          UINT16 length = ((String*) GetObject( i ))->Len();

          overall_num_chars += length;

          if ( length > max_line_length)
               max_line_length = length;
     }

     recalc_needed = FALSE;
}





// ----------------------------------------------------------------------------
//                operator << TextOutputStream
// ............................................................................
//
//
// ----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const LineList& rObject )
{
     UINT32 num_lines = rObject.GetNumLines();

     for ( UINT32 line_num = 0; line_num < num_lines ; line_num++)
     {
          String* pstr = rObject.GetLineNum( line_num );

          rTOS << (const char*) *pstr ;

          rTOS << (const char*) TEXTFILE_EOL;
     }

     return rTOS;
}
