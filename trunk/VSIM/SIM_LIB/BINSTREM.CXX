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
//  BINSTREM.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the class BinaryStream, which provides for output
//  of objects to a binary file 
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

#include "binstrem.hxx"

#endif




// ----------------------------------------------------------------------------
//            BinaryOutputStream::~BinaryOutputStream()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

BinaryOutputStream::~BinaryOutputStream( )
{
}




// ----------------------------------------------------------------------------
//            BinaryStream::~BinaryStream()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

BinaryStream::~BinaryStream( )
{
     fclose( pf );
}

// ----------------------------------------------------------------------------
//            BinaryStream::OpenFile()
// ............................................................................
//
//   reopens the Stream as a new file. If successful then returns TRUE, other
//   wise returns FALSE. If the BinaryStream was already open before this
//   function call then the initial file is closed
//
// ----------------------------------------------------------------------------

BOOL8     BinaryStream::OpenFile( const String& FileName, const char* Flags )
{
     FILE* new_pf = fopen( FileName, Flags );

     if ( new_pf )
     {
          if (pf)
               fclose( pf );

          name = FileName;
          pf = new_pf;

          return TRUE;
     }
     else
          return FALSE;
}


BOOL8 BinaryStream::GetCurrentFilePosition( INT32& rPosition )
{
     INT32 rc;

     rc =  ftell( pf);

     if ( rc != -1 )
     {
          rPosition = rc;
          return TRUE;
     }
     else
          return FALSE;
}




// -------------------  primitive output operators ...  -----------------------






// ----------------------------------------------------------------------------
//            operator << Size
// ............................................................................
// 
// allows output of objects of class Size  to
// my BinaryOutputStream class.
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Size& rObject)
{
     rBOS << rObject.Width();
     rBOS << rObject.Height();

     return rBOS;
}




// ----------------------------------------------------------------------------
//            operator << Time
// ............................................................................
// 
// allows output of objects of class Time  to
// my BinaryOutputStream class.
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Time& rValue )
{
   rBOS << rValue.GetTime();
   return rBOS;
}





// ----------------------------------------------------------------------------
//            operator << DateTime
// ............................................................................
// 
// allows output of objects of class DateTime from
// my BinaryOutputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const DateTime& rValue )
{
     rBOS << rValue.GetTime();
     rBOS << rValue.GetDate();

     return rBOS;
}






// ----------------------------------------------------------------------------
//            operator << String
// ............................................................................
// 
// allows output of NumericString and String to 
// my BinaryOutputStream class. includes length data so that variable 
// length strings are implemented
//  
// ----------------------------------------------------------------------------
      
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const String& rValue )
{
     // first write out length of String

     UINT16 length = (UINT16) rValue.Len();

     rBOS << length;
     rBOS.Write( (const char * ) rValue, length );

     return rBOS;
}




// ----------------------------------------------------------------------------
//            operator << Point
// ............................................................................
// 
// allows output of objects of class Point to 
// my BinaryOutputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Point& rObject )
{
     rBOS << rObject.X();
     rBOS << rObject.Y();

     return rBOS;
}




// ----------------------------------------------------------------------------
//            operator << Rectangle
// ............................................................................
// 
// allows output of objects of class Rectangle to 
// my BinaryOutputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Rectangle& rValue )
{
     rBOS << rValue.TopLeft( );
     rBOS << rValue.GetSize( );

     return rBOS;
}




// -------------------  primitive input operators ...  ------------------------



// ----------------------------------------------------------------------------
//            operator >> String
// ............................................................................
// 
// allows Input of NumericString and String to 
// my BinaryInputStream class. includes length data so that variable 
// length strings are implemented
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, String& rValue )
{
     // first read in length of String

     UINT16 length;

     rBIS >> length;

     // allocate memory for char array - add an extra byte for terminating zero 

     char * ptr = new char[ length +1 ];

     if ( NULL == ptr )
     {
          rBIS.SetExtErrorFlag( );
     }
     else
     {
          // fill in char array with asciiz 

          rBIS.Read(  ptr, length );
          ptr[ length ] = '\0';

          rValue = ( const char* ) ptr;

          delete[] ptr;
     }

     return rBIS;
}





// ----------------------------------------------------------------------------
//            operator >> Size
// ............................................................................
// 
// allows Input of objects of class Size  to 
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Size& rObject)
{
     rBOS >> rObject.Width();
     rBOS >> rObject.Height();

     return rBOS;
}


// ----------------------------------------------------------------------------
//            operator >> Point
// ............................................................................
// 
// allows Input of objects of class Point to 
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Point& rObject )
{
     rBOS >> rObject.X();
     rBOS >> rObject.Y();

     return rBOS;
}




// ----------------------------------------------------------------------------
//            operator >> Rectangle
// ............................................................................
// 
// allows Input of objects of class Rectangle to 
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Rectangle& rValue )
{
     Point point; 
     Size  size;

     rBOS >> point;
     rBOS >> size;

     rValue.ChangePos( point );
     rValue.ChangeSize( size );

     return rBOS;
}


// ----------------------------------------------------------------------------
//            operator >> Time
// ............................................................................
// 
// allows Input of objects of class Time from
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Time& rValue )
{
     long temp;

     rBIS >> temp;
     rValue.ChangeTime( temp );

	return rBIS;
}




// ----------------------------------------------------------------------------
//            operator >> DateTime
// ............................................................................
// 
// allows Input of objects of class DateTime from
// my BinaryInputStream class. 
//  
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, DateTime& rValue )
{
     long    templong;
     ULONG   tempulong;

     rBIS >> templong;
     rBIS >> tempulong;

     rValue.ChangeTime( templong );
     rValue.ChangeDate( tempulong );

     return rBIS;
}


