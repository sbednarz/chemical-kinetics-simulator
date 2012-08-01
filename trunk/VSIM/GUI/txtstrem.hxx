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
//  TXTSTREM.HXX - a header of the VSIM User interface 
// ............................................................................
// 
//  This header declares the class TextStream, which provides for I_O
//  of objects to a text file
//
//  CREATED : 1.10.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------
 
#if !defined(__TXTSTREM_HXX__)
#define __TXTSTREM_HXX__

#include <sv.hxx>
#include <stdio.h>

#include "numstrng.hxx"
#include "vsimtype.hxx"
#include "idtypes.hxx"

#define OPEN_WRITE_FLAGS_TEXT         "w+"
#define OPEN_READ_FLAGS_TEXT          "r+"

const CHAR8 EOL = '\n';


class IDType;

// -------------------------------------------------------------------
//               class TextStream
// ...................................................................
//
// base class for objects implementing text file IO
//
// -------------------------------------------------------------------

class TextStream
{
protected :
     FILE *          pf;
     BOOL8           error;
     String          name;

public:
               TextStream( const String& rFileName, const char * Flags ) :
                    error( FALSE ),
                    name( rFileName )
                    { pf = fopen( name, Flags ); }

              ~TextStream( ) 
                   { fclose( pf ); }

     BOOL8     FileError(  ) const 
                    { return ( ( 0 == pf ) || ( TRUE == error) || ( 0 != ferror( pf ) ) );}

     BOOL8     IsOpen( ) const 
                    { return ( pf != 0 ); }

     void      SetExtErrorFlag( ) { error = TRUE; }
     void      ClearExtErrorFlag( ) { error = FALSE; }

     const String& GetName( ) const { return name; }
};



// -------------------------------------------------------------------
//               class TextOutputStream
// ...................................................................
//
// this class provides the capability of saving data in text format
// to an output file. 
//
// -------------------------------------------------------------------

class TextOutputStream : public TextStream
{
protected :

public:
               TextOutputStream( const String& FileName ) :
                    TextStream( FileName, OPEN_WRITE_FLAGS_TEXT ) { }


friend TextOutputStream& operator << ( TextOutputStream& rTOS, const String& rValue );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const NumericString& rValue );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const Size& rValue );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const Point& rValue );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const UINT16 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const INT16 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const UINT32 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const INT32 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const BOOL8 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const FLOAT64 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const CHAR8 Value );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const IDType& rID );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const char* rValue );
};



inline TextOutputStream& operator << ( TextOutputStream& rTOS, const CHAR8 Value )
{ 
     fprintf( rTOS.pf, "%c", Value ); 
     return rTOS; 
}




inline TextOutputStream& operator << ( TextOutputStream& rTOS, const NumericString& rValue )
{
     fprintf( rTOS.pf, "%s ", (const char *) rValue ); // note the trailing space and no string length info 
     return rTOS;
}

inline TextOutputStream& operator << ( TextOutputStream& rTOS, const UINT16 Value )
{
     fprintf( rTOS.pf, "%hu ", Value ); // note the trailing space
     return rTOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const INT16 Value )
{
     fprintf( rTOS.pf, "%hd ", Value ); // note the trailing space
     return rTOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const char* rValue )
{
     fprintf( rTOS.pf, "%s", rValue ); 
     return rTOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const UINT32 Value )
{
     fprintf( rTOS.pf, "%lu ", Value ); // note the trailing space
     return rTOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const INT32 Value )
{
     fprintf( rTOS.pf, "%ld ", Value ); // note the trailing space
     return rTOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const BOOL8 Value )
{
     return rTOS << (UINT16) Value;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const FLOAT64 Value )
{
     fprintf( rTOS.pf, "%-12.8E ", Value ); // note the trailing space
     return rTOS;
}












// -------------------------------------------------------------------
//               class TextInputStream
// ...................................................................
//
// this class provides the capability of retrieving data 
// from an text input file 
//
// -------------------------------------------------------------------

class TextInputStream : public TextStream
{
public:
               TextInputStream( const String& FileName ) :
                    TextStream( FileName, OPEN_READ_FLAGS_TEXT ) { }

friend TextInputStream& operator >> ( TextInputStream& rTIS, UINT16& rI );
friend TextInputStream& operator >> ( TextInputStream& rTIS, INT16& rI );
friend TextInputStream& operator >> ( TextInputStream& rTIS, UINT32& rI );
friend TextInputStream& operator >> ( TextInputStream& rTIS, INT32& rI );
friend TextInputStream& operator >> ( TextInputStream& rTIS, FLOAT64& rF );
friend TextInputStream& operator >> ( TextInputStream& rTIS, String& rValue );
friend TextInputStream& operator >> ( TextInputStream& rTIS, NumericString& rValue );
friend TextInputStream& operator >> ( TextInputStream& rTIS, Point& rObject );
friend TextInputStream& operator >> ( TextInputStream& rTIS, Size& rObject);
friend TextInputStream& operator >> ( TextInputStream& rTIS, BOOL8& rB );
friend TextInputStream& operator >> ( TextInputStream& rBIS, IDType& rID );
};





inline TextInputStream& operator >> ( TextInputStream& rTIS, UINT16& rI )
{
     fscanf( rTIS.pf, " %hu ", &rI );
     return rTIS;
}


inline TextInputStream& operator >> ( TextInputStream& rTIS, INT16& rI )
{
     fscanf( rTIS.pf, " %hd ", &rI );
     return rTIS;
}


inline TextInputStream& operator >> ( TextInputStream& rTIS, UINT32& rI )
{
     fscanf( rTIS.pf, " %lu ", &rI );
     return rTIS;
}


inline TextInputStream& operator >> ( TextInputStream& rTIS, INT32& rI )
{
     fscanf( rTIS.pf, " %ld ", &rI );
     return rTIS;
}


inline TextInputStream& operator >> ( TextInputStream& rTIS, FLOAT64& rF )
{
     fscanf( rTIS.pf, " %lf ", &rF );
     return rTIS;
}









#endif




