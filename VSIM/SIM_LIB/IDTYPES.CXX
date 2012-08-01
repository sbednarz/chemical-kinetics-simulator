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
// VSIM - a Visual Kinetics Simulator
// ............................................................................
//
//  MODULE  : idtypes.cxx
//
//             This module contains the definition out the class IDType,
//             which is used for creating identification tags in data files
//             such that the data type/content can be unambiguously identified.
//
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)
//
//  CHANGE HISTORY :
//        created Nov. 10 1994
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "idtypes.hxx"

#endif




// -------------------------------------------------------------------
//               IDType( )
// ...................................................................
//
// Constructor for the class IDType.
//
// we just zero out the data structure in this case
//
// -------------------------------------------------------------------

IDType::IDType()
{
     for ( unsigned int i = 0; i < sizeof data.as_str ; i++)
          data.as_str[i] = '\0';
}




// -------------------------------------------------------------------
//               IDType( const CHAR8 PTR )
// ...................................................................
//
// Constructor for the class IDType.
//
//
// -------------------------------------------------------------------

IDType::IDType( const CHAR8 PTR pChar )
{
     // copy the first 4 chars

     for ( unsigned int  i = 0; i < ( sizeof data.as_str - 1 ) ; i++ )
          data.as_str[i] = pChar[i] ;

     // make sure 5th char is terminating zero

     data.as_str[i] = '\0';
}


// -------------------------------------------------------------------
//               IDType( String& rStr )
// ...................................................................
//
// Constructor for the class IDType.
//
// we copy characters from the Source string and zero-terminate
//
// -------------------------------------------------------------------

IDType::IDType( const String& rStr )
{
     // copy the first 4 chars

     for ( unsigned int i = 0; i < ( sizeof data.as_str - 1 ) ; i++ )
          data.as_str[i] = rStr[i] ;

     // make sure 5th char is terminating zero

     data.as_str[ 4 ] = '\0';
}



// -------------------------------------------------------------------
//               IDType( REC_TYPE )
// ...................................................................
//
// Constructor for the class IDType.
//
// We just initialize the data structure with the parameter and
// terminate with zero
//
// -------------------------------------------------------------------

IDType::IDType( REC_TYPE I )
{
          data.as_rec_type = I;
          data.as_str[4] = '\0';
}


// -------------------------------------------------------------------
//               operator << BinaryOutputStream
// ...................................................................
//
//  for saving the state of an IDType object to a binary file 
//
// -------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const IDType& rID )
{
#if defined(__IBMCPP__)

     // reverse byte order if we are writing to file 

     char temp[4];

     temp[0] = rID.data.as_str[3];
     temp[1] = rID.data.as_str[2];
     temp[2] = rID.data.as_str[1];
     temp[3] = rID.data.as_str[0];

     rBOS.Write( (const char *) temp, 4 );

#else

     rBOS.Write( (const char *) rID.data.as_str, 4 );

#endif
     return rBOS;
}





// -------------------------------------------------------------------
//               operator << BinaryInputStream
// ...................................................................
//
//  for initializing an IDType object from a binary file 
//
// -------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, IDType& rID )
{
#if defined(__IBMCPP__)

     // reverse byte order if we are reading from file 

     char temp[4];

     rBIS.Read( (char *) temp, 4 );

     rID.data.as_str[3] = temp[0];
     rID.data.as_str[2] = temp[1];
     rID.data.as_str[1] = temp[2];
     rID.data.as_str[0] = temp[3];

#else
     // read in 4 characters

     rBIS.Read( (char *) rID.data.as_str, 4 );

#endif
     // place null terminator in the fifth position

     rID.data.as_str[ 4 ] = '\0'; 

     return rBIS;
}


// -------------------------------------------------------------------
//               operator << TextOutputStream
// ...................................................................
//
//  for saving the state of an IDType object to a text file 
//
// -------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const IDType& rID )
{
#if defined(__IBMCPP__)

     // reverse byte order if we are writing to file 

     char temp[5];

     temp[0] = rID.data.as_str[3];
     temp[1] = rID.data.as_str[2];
     temp[2] = rID.data.as_str[1];
     temp[3] = rID.data.as_str[0];
     temp[4] = '\0';


     fprintf( rTOS.pf, "%s ", temp );

#else

     fprintf( rTOS.pf, "%s ", rID.data.as_str );

#endif

     return rTOS;
}


TextInputStream& operator >> ( TextInputStream& rTIS, IDType& rID )
{
#if defined(__IBMCPP__)

     // reverse byte order if we are reading from file 

     char temp[5];

     fscanf( rTIS.pf, " %4s", temp );

     rID.data.as_str[3] = temp[0];
     rID.data.as_str[2] = temp[1];
     rID.data.as_str[1] = temp[2];
     rID.data.as_str[0] = temp[3];

#else
     // copy the first 4 chars

     fscanf( rTIS.pf, " %4s", &rID.data.as_str );

#endif

     rID.data.as_str[4] = '\0';

     return  rTIS;
}





// -------------------------------------------------------------------
//               operator != IDType
// ...................................................................
//
//  for comparing the values of two IDType objects
//
// -------------------------------------------------------------------

BOOL8  operator != ( const IDType& rID1,  const IDType& rID2  )
{
     return ( rID1.data.as_rec_type != rID2.data.as_rec_type );
}


#if defined(__UNIT_TEST__)


#include <iostream.h>
#include <iomanip.h>


void main( )
{
     IDType id1;   // null
     IDType id2( "tEsT" );
     IDType id3( 'TEST' );
     String str( "test" );
     IDType id4( str );

     cout << "CONSTRUCTORS" << endl;
     cout << "default" << endl;
     cout << "id1 = '" << (const char *)  id1 << "'" << endl;
     cout << "id1 = '" << dec << (unsigned long) id1 << "'" << endl;
     cout << "id1 = '" << hex << (unsigned long) id1 << "x'" << endl;

     cout << "\n\nShould be tEsT" << endl;
     cout << "id2 = '" << (const char *)  id2 << "'" << endl;
     cout << "id2 = '" << dec << (unsigned long) id2 << "'" << endl;
     cout << "id2 = '" << hex << (unsigned long) id2 << "x'" << endl;

     cout << "\n\nShould be TEST" << endl;
     cout << "id3 = '" << (const char *)  id3 << "'" << endl;
     cout << "id3 = '" << dec << (unsigned long) id3 << "'" << endl;
     cout << "id3 = '" << hex << (unsigned long) id3 << "x'" << endl;

     cout << "\n\nShould be test" << endl;
     cout << "id4 = '" << (const char *)  id4 << "'" << endl;
     cout << "id4 = '" << dec << (unsigned long) id4 << "'" << endl;
     cout << "id4 = '" << hex << (unsigned long) id4 << "x'" << endl;


     cout << "\n\nIDType ASSIGNMENT" << endl;
     id1 = id4;
     cout << "Should be test" << endl;
     cout << "id1 = '" << (const char *)  id1 << "'" << endl;
     cout << "id1 = '" << dec << (unsigned long) id1 << "'" << endl;
     cout << "id1 = '" << hex << (unsigned long) id1 << "x'" << endl;

     cout << "\n\nlong int ASSIGNMENT" << endl;
     id1 = START_APP_INIT;
     cout << "Should be AppI" << endl;
     cout << "id1 = '" << (const char *)  id1 << "'" << endl;
     cout << "id1 = '" << dec << (unsigned long) id1 << "'" << endl;
     cout << "id1 = '" << hex << (unsigned long) id1 << "x'" << endl;

     cout << "\n\nlong int ASSIGNMENT" << endl;
     id4 = "RXDT";
     cout << "Should be RXDT (52 58 44 54x)" << endl;
     cout << "id4 = '" << (const char *)  id4 << "'" << endl;
     cout << "id4 = '" << dec << (unsigned long) id4 << "'" << endl;
     cout << "id4 = '" << hex << (unsigned long) id4 << "x'" << endl;


     cout << "\n\ntesting streams insertion,extraction" << endl;
     cout << id4 <<    endl;
//   cout << "\n\nEnter 4 chars" << endl;
//   cin >> id4 ;
//   cout << id4 <<    endl;

}

#endif
