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
//  TXTSTREM.CXX - a module of the VSIM User interface
// ............................................................................
// 
//  This module defines the class TextStream, which provides for output
//  of objects to a text file 
//
//  CREATED : 3.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------
 

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "txtstrem.hxx"

#endif



// -------------------  primitive output operators ...  -----------------------



// -------------------------------------------------------------------
//               TextOutputStream& operator << String& 
// ...................................................................
//
// this operator is intended to be use only to write to a runparm
// file in text format. It will be used to indicate the fully-qualified
// name of the output rxn file 
//
// -------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const String& rValue )
{
     // forst write out IDType and length of name followed by eol
     // so as to delimit string contents on next line

     rTOS << IDType( OUT_FILE_NAME ) << rValue.Len() << EOL;

     // note that we write the string length and NO trailing space  after contents
     // note that the string length does not include the terminating '\0'

     fprintf( rTOS.pf, "%s", (const char *) rValue ); 

     rTOS << EOL;

     return rTOS;
}


TextOutputStream& operator << ( TextOutputStream& rTOS, const Size& rValue )
{
     rTOS << IDType( PARENT_SCREEN_SIZE );
     rTOS << rValue.Width() << rValue.Height() << EOL;
     
     return rTOS;
}


TextOutputStream& operator << ( TextOutputStream& rTOS, const Point& rValue )
{
     rTOS << IDType( PARENT_SCREEN_POSN );
     rTOS << rValue.X() << rValue.Y() << EOL;

     return rTOS;
}



// -------------------  primitive input operators ...  ------------------------



// ----------------------------------------------------------------------------
//            operator >> String
// ............................................................................
// 
// allows Input of String to the TextInputStream
// Includes reading of previously output length data so that variable 
// length strings are implemented
//  
// ----------------------------------------------------------------------------
 
TextInputStream& operator >> ( TextInputStream& rTIS, String& rValue )
{
     // read in length of String

     UINT16 length;

     rTIS >> length;

     // allocate memory for char array - add an extra byte for terminating zero 

     char * ptr = new char[ length + 1 ];

     if ( NULL == ptr )
     {
          rTIS.SetExtErrorFlag( );
     }
     else
     {
          // fill in char array with asciiz line from stream

	  fgets( ptr, (length+1), rTIS.pf );
	  ptr[ length ] = '\0';

	  rValue = ( const char* ) ptr;

	  delete[] ptr;
     }

     return rTIS;
}


// ----------------------------------------------------------------------------
//            operator >> Size
// ............................................................................
// 
// allows Input of objects of class Size  to 
// my TextInputStream class. 
//  
// ----------------------------------------------------------------------------
 
TextInputStream& operator >> ( TextInputStream& rTIS, Size& rObject)
{
     // read in IDType 

     IDType id_type;

     rTIS >> id_type;

     if ( id_type != IDType( PARENT_SCREEN_SIZE ) )
     {
          rTIS.SetExtErrorFlag( );

          return rTIS;
     }

     rTIS >> rObject.Width();
     rTIS >> rObject.Height();

     return rTIS;
}


// ----------------------------------------------------------------------------
//            operator >> Point
// ............................................................................
// 
// allows Input of objects of class Point to 
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
TextInputStream& operator >> ( TextInputStream& rTIS, Point& rObject )
{
     // read in IDType 

     IDType id_type;

     rTIS >> id_type;

     if ( id_type != IDType( PARENT_SCREEN_SIZE ) )
     {
          rTIS.SetExtErrorFlag( );

          return rTIS;
     }

     rTIS >> rObject.X();
     rTIS >> rObject.Y();

     return rTIS;
}




// -------------------------------------------------------------------
//               operator >> BOOL8&
// ...................................................................
//
// Reads in an ASCII representation of "0" or "1", and sets rB = 
// FALSE if "0" was read in, TRUE otherwise. If the input is not "0" or "1" then
// sets rB = FALSE and fcn returns FALSE, otherwise the fcn 
// returns TRUE 
//
// -------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, BOOL8& rB )
{
     INT32 i;

     rTIS >> i;

     switch ( i )
     {

     case 0 :

          rB = FALSE;
          break;

     case 1 :

          rB = TRUE;
          break;
              
     default :
          rTIS.error = TRUE;
     }


     return rTIS;
}









