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
//  LINELIST.HXX - a header for the VSIM User interface
// ............................................................................
//
//  This module file defines the class LineList, a generic container for a 
//  set of Strings
//
//  CREATED : 8.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__LINELIST_HXX__)
     #define __LINELIST_HXX__


#if defined(OS2)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MSDOS__)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\r\n"
#endif

#if defined(__AIX__)
#define FILE_END_OF_LINE     "\n"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MAC__)
#define FILE_END_OF_LINE   "\r\n\xOD"
#define MLE_END_OF_LINE    "\n"
#endif



enum LINE_TERMINATOR_TYPE { MLE_TYPE, TEXTFILE_TYPE };


class LineList : private List
{
protected:

     BOOL8           recalc_needed;
     UINT32          overall_num_chars;
     UINT16          max_line_length;

     void            CalcLengths();

public :
                     LineList();
                    ~LineList();

     String*         AddAnotherLine( );

     UINT32          GetNumLines() const {return Count(); }

     String*         GetLineNum( UINT32 LineNum ) const {return (String*) GetObject( LineNum ); }

     UINT32          GetOverallNumChars( ) { if (recalc_needed) CalcLengths(); return overall_num_chars; }
     UINT32          GetMaxLineLength( )   { if (recalc_needed) CalcLengths(); return max_line_length; } 

//     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const LineList& rObject );
};

TextOutputStream& operator << ( TextOutputStream& rTOS, const LineList& rObject );

#endif
