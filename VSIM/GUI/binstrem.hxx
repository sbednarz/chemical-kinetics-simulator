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
//  BINSTREM.HXX - a header of the VSIM User interface
// ............................................................................
//
//  This header declares the class BinaryStream, which provides for output
//  of objects to a binary file
//
//  CREATED : 1.10.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__BINSTREM_HXX__)
#define __BINSTREM_HXX__

#include <stdio.h>


#include "vsimtype.hxx"
#include <tools.hxx>

#define OPEN_WRITE_FLAGS         "rb+"
#define OPEN_READ_FLAGS          "rb"
#define CREATE_WRITE_FLAGS       "wb+"

enum    FILE_OPEN_MODE   { FILE_MUST_EXIST, REPLACE_IF_EXISTS };


class String;
class Size;
class Point;
class Rectangle;



// -------------------------------------------------------------------
//               class BinaryStream
// ...................................................................
//
// base class for objects implementing binary file IO
//
// -------------------------------------------------------------------

class BinaryStream
{
protected :
     FILE *          pf;
     BOOL8           ext_error;
     String          name;

     BOOL8     OpenFile( const String& FileName, const char* Flags );

               BinaryStream( const String& rFileName, const char * Flags ) :
                    ext_error( FALSE ),
                    name( rFileName )
                    { pf = fopen( name, Flags ); }

               BinaryStream( ) : pf( NULL ), ext_error( FALSE ), name() {}


public:

	      ~BinaryStream( );

     BOOL8     FileError(  ) const
                    { return ( ( 0 == pf ) || ( TRUE == ext_error) || ( 0 != ferror( pf ) ) );}

     BOOL8     IsOpen( ) const
                    { return ( pf != 0 ); }

     BOOL8     MoveTo( INT32 NewFilePosition )
                    { return ( 0 == fseek( pf, NewFilePosition, SEEK_SET ) ); }

     BOOL8     MoveToEnd( )
                    { return ( 0 == fseek( pf, 0, SEEK_END ) ); }

     BOOL8     GetCurrentFilePosition( INT32& rPosition );

     void      SetExtErrorFlag()  { ext_error = TRUE; }
     void      ClearExtErrorFlag()  { ext_error = FALSE; }

     const String& GetName( ) const { return name; }
};



// -------------------------------------------------------------------
//               class BinaryOutputStream
// ...................................................................
//
// this class provides the capability of saving data in binary format
// to an output file.
//
// -------------------------------------------------------------------

class BinaryOutputStream : public BinaryStream
{
public:
               BinaryOutputStream( const String& FileName, 
                                   const FILE_OPEN_MODE eFileMode = FILE_MUST_EXIST ) :
                    BinaryStream( FileName, 
                                  eFileMode == FILE_MUST_EXIST ? 
                                  OPEN_WRITE_FLAGS : CREATE_WRITE_FLAGS ) { }

	       BinaryOutputStream( ) : BinaryStream() {}

	      ~BinaryOutputStream( );

     BOOL8     SetName( const String& FileName )
               { return BinaryStream::OpenFile( FileName, OPEN_WRITE_FLAGS); }

     void      Write( const void * pSource, size_t Size)
                    {  fwrite( pSource, Size, 1, pf ); }

     void      Write( const char * pSource, size_t Size)
                    {  fwrite( pSource, Size, 1, pf ); }
};





BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const String& rValue );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Size& rValue );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Point& rValue );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Rectangle& rValue );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Time& rValue );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const DateTime& rValue );





inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const UINT16& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const FLOAT64& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const INT16& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const UINT32& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const INT32& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const BOOL8& rValue )
{
     rBOS.Write( &rValue, sizeof rValue );
     return rBOS;
}








// -------------------------------------------------------------------
//               class BinaryInputStream
// ...................................................................
//
// this class provides the capability of retrieving data in binary format
// from an input file
//
// -------------------------------------------------------------------

class BinaryInputStream : public BinaryStream
{
public:
               BinaryInputStream( const String& FileName ) :
		    BinaryStream( FileName, OPEN_READ_FLAGS ) { }

	      ~BinaryInputStream( ) { }

     void      Read( void * pTarget, size_t Size)
                    {  fread( pTarget, Size, 1, pf ); }

     void      Read( char * pTarget, size_t Size)
                    {  fread( pTarget, Size, 1, pf ); }

};


inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, UINT16& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, FLOAT64& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, INT16& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, UINT32& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, INT32& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}

inline BinaryInputStream& operator >> ( BinaryInputStream& rBOS, BOOL8& rValue )
{
     rBOS.Read( &rValue, sizeof rValue );
     return rBOS;
}


BinaryInputStream& operator >> ( BinaryInputStream& rBOS, String& rValue );
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Size& rValue );
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Point& rValue );
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Rectangle& rValue );
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, Time& rValue );
BinaryInputStream& operator >> ( BinaryInputStream& rBOS, DateTime& rValue );


#endif

