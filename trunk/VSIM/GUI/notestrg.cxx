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
//  NOTESTRG.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the NotebookString class
//
//  CREATED : 5.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "notestrg.hxx"
#include "mainapp.hxx"

#endif


#include <stdio.h>
#include <stdlib.h>


const char FILL_CHAR = ' ';

//--------------------------------------------------------------------------
//                     NumericString::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator
//                                                                          
//--------------------------------------------------------------------------


NotebookString& NotebookString::operator =  ( const NotebookString& rStr )
{ 
     if ( this == &rStr)
          return *this;

     String::operator =( rStr );
     return *this;
}


//--------------------------------------------------------------------------
//                     NumericString::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator
//                                                                          
//--------------------------------------------------------------------------


NotebookString& NotebookString::operator =  ( const String& rStr )
{ 
     if ( this == &rStr)
          return *this;

     String::operator =( rStr );
     return *this;
}




// ----------------------------------------------------------------------------
//            operator << NotebookString
// ............................................................................
// 
// allows output of NumericString and String to 
// my BinaryOutputStream class. includes length data so that variable 
// length strings are implemented
//  
// ----------------------------------------------------------------------------
      
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const NotebookString& rValue )
{
     // expand/truncate copy of NotebookString to maximum length 

     NotebookString tempstr( rValue, 0, rValue.GetMaxLength() );
     
     if ( tempstr.Len() < tempstr.GetMaxLength() )
          tempstr.Expand( tempstr.GetMaxLength(), FILL_CHAR );
                       
     // write out length of String, then contents

     ASSERT( tempstr.Len() == tempstr.GetMaxLength() );                       
     ASSERT( tempstr.Len() == DEFAULT_MAX_LENGTH );                       

     UINT16 length = tempstr.Len();
     rBOS << length;
     rBOS.Write(  ( const char *) tempstr, length );

     return rBOS;
}


// ----------------------------------------------------------------------------
//            operator >> String
// ............................................................................
// 
// allows Input of NotebookString from
// my BinaryInputStream class. includes length data so that variable 
// length strings are implemented
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, NotebookString& rValue )
{
     // first read in length of String

     UINT16 length;

     rBIS >> length;

     // allocate memory for char array - add an extra byte for terminating zero 

     char * ptr = new char[ length +1 ];

     if ( NULL == ptr )
     {
          rBIS.SetExtErrorFlag( );
          GetpMainApp()->OutOfMemory(  );
     }
     else
     {
          // fill in char array with asciiz 

          rBIS.Read(  ptr, length );
          ptr[ length ] = '\0';

          rValue = NotebookString( (const char* ) ptr );
          rValue.SetMaxLength( length );

          rValue.EraseTrailingChars( FILL_CHAR );

          delete[] ptr;
     }

     return rBIS;
}



