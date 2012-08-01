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
//  NUMSTRNG.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the NumericString class, which provides the means of 
//  validating and converting character string data into  various forms ofh
//  numeric data 
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "numstrng.hxx"

#endif



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>


const size_t BUFFER_LENGTH = 256;



// ----------------------------------------------------------------------------
//              NumericString::NumericString ( FLOAT64 f ) 
// ............................................................................
// 
//  a constructor for NumericString
//
// ----------------------------------------------------------------------------

NumericString::NumericString ( FLOAT64 f, UINT16 MinFieldWidth, UINT16 DigitsAfterDecPt ) :
String()
{
     // use std lib fcn to convert real number to string 
     char buffer[BUFFER_LENGTH];

//     sprintf( buffer, "%-*.*g", (int) MinFieldWidth, (int) DigitsAfterDecPt, f );
     sprintf( buffer, "%-*.*e", (int) MinFieldWidth, (int) DigitsAfterDecPt, f );

     *this = String( buffer );
}





//--------------------------------------------------------------------------
//                          NumericString::IsValidFloat( )
//..........................................................................
//                                                                          
// tests that the NumericString converts to a valid real number w/o problems 
// returns TRUE if no errors occur during conversion, FALSE otherwise 
// if the parameter pResult != NULL then the converted result is returned
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidFLOAT64( PFLOAT64 pResult ) const
{
     // make a temporary copy of object so we do not modifiy original 

     String tmpstr = *this;

     tmpstr.EraseTrailingChars( );

     // if what's left is a null string return FALSE 

     USHORT length = tmpstr.Len( );

     if ( ( length == 0 )  || ( length > BUFFER_LENGTH ) )
          return FALSE;

     // check if last char in string is 'e','E', '-' or '+'. Invalid data if it's there
     // but at least the Borland 3.1 compiler for DOS accepts it as a valid
     // component in the string. We don't want that so reject the string

     char last_char = tmpstr.Copy( length-1, 1 );      // get last character in string

     if ( (last_char == 'e') || (last_char == 'E') ||
          (last_char == '-') || (last_char == '+') )
          return FALSE;

     // do the conversion

     FLOAT64 result;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( tmpstr.GetStr(), " %lf%s", &result, remainder );


     if ( ( 1 == i ) && ( result != HUGE_VAL ) && ( result != - HUGE_VAL ) )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;

}



//--------------------------------------------------------------------------
//                   NumericString::IsValidNonNegativeFloat( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid real number w/o problems   
// In this case the real number cannot be less than 0.0                     
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents a number >= 0.0                           
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidNonNegativeFLOAT64( PFLOAT64 pResult ) const 
{
     FLOAT64 result;

     if ( ! IsValidFLOAT64( &result ) )
          return FALSE;

     if ( result >= ( FLOAT64 ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;
}




//--------------------------------------------------------------------------
//                   NumericString::IsValidPositiveFloat( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid real number w/o problems   
// In this case the real number must be greater than 0.0 
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents a number > 0.0                           
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidPositiveFLOAT64( PFLOAT64 pResult ) const 
{
     FLOAT64 result;

     if ( ! IsValidFLOAT64( &result ) )
          return FALSE;

     if ( result > ( FLOAT64 ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;
}




//--------------------------------------------------------------------------
//                   NumericString::IsValidNonZeroFloat( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid real number w/o problems   
// In this case the real number cannot be exactly equal to 0.0
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents a number != 0.0                           
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidNonZeroFLOAT64( PFLOAT64 pResult ) const 
{
     FLOAT64 result;

     if ( ! IsValidFLOAT64( &result ) )
          return FALSE;

     if ( result != ( FLOAT64 ) 0.0 )
     {
          if ( pResult )
               *pResult = result;

          return TRUE;
     }
     else
          return FALSE;

}



//--------------------------------------------------------------------------
//                 NumericString::IsValidUINT32( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid unsigned 32 bit 
// integer without problems                                                         
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents an unsigned 32 bit integer > 0        
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidUINT32( PUINT32 pResult ) const 
{
     FLOAT64 result;

     if ( ! IsValidPositiveFLOAT64( &result ) ) // also strips the trailing whitespace
          return FALSE;

     if ( result > ( ( FLOAT64 ) ULONG_MAX )  )
          return FALSE;

     UINT32 tmp;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( GetStr(), " %lu%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp != ULONG_MAX ) )
     {
          if ( pResult )
               *pResult = tmp;

          return TRUE;
     }
     else
          return FALSE;

}



//--------------------------------------------------------------------------
//                 NumericString::IsValidUINT16( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid unsigned 16 bit 
// integer without problems                                                         
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents an unsigned 16 bit integer > 0        
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidUINT16( PUINT16 pResult ) const 
{


     if ( ! IsValidPositiveFLOAT64( ) )
          return FALSE;


     UINT32 tmp;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( GetStr(), " %lu%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp < USHRT_MAX ) && ( tmp > 0 ) )
     {
          if ( pResult )
               *pResult = ( UINT16 ) tmp; 

          return TRUE;
     }
     else
          return FALSE;

}





//--------------------------------------------------------------------------
//                          NumericString::IsValidPositiveINT32( )
//..........................................................................
//                                                                          
// tests that a given string converts to a valid long positive integer      
// without problems                                                         
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
// returns TRUE if str represents an signed long integer  > 0               
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidPositiveINT32( PINT32 pResult ) const 
{

     if ( ! IsValidPositiveFLOAT64( ) )
          return FALSE;

     INT32 tmp;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( GetStr(), " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp != LONG_MAX ) && ( tmp >= 1 ) )
     {
          if ( pResult )
               *pResult = tmp;

          return TRUE;
     }
     else
          return FALSE;
}

//--------------------------------------------------------------------------
//                     NumericString::IsValidPositiveINT16( )
//..........................................................................
//                                                                          
// tests for conversion to a valid short positive integer     
// without problems. We do this by converting to a long positive            
// integer and then testing that the results is <= the constant SHRT_MAX    
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidPositiveINT16( PINT16 pResult ) const 
{


     if ( ! IsValidPositiveFLOAT64( ) )
          return FALSE;


     INT32 tmp;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( GetStr(), " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp < SHRT_MAX ) && ( tmp > 0 ) )
     {
          if ( pResult )
               *pResult = ( INT16 ) tmp; 

          return TRUE;
     }
     else
          return FALSE;

}


//--------------------------------------------------------------------------
//                     NumericString::IsValidNonNegativeINT16( )
//..........................................................................
//                                                                          
// tests for conversion to a valid short positive integer     
// without problems. We do this by converting to a long positive            
// integer and then testing that the results is <= the constant SHRT_MAX    
//                                                                          
// returns TRUE if no errors occur during conversion, FALSE otherwise       
//                                                                          
//--------------------------------------------------------------------------

BOOL8 NumericString::IsValidNonNegativeINT16( PINT16 pResult ) const 
{

     if ( ! IsValidNonNegativeFLOAT64( ) )
          return FALSE;


     INT32 tmp;
     char remainder[ BUFFER_LENGTH ];

     int i = sscanf ( GetStr(), " %ld%s", &tmp, remainder );

     if ( ( 1 == i ) && ( tmp < SHRT_MAX ) && ( tmp >= 0 ) )
     {
          if ( pResult )
               *pResult = (INT16) tmp;

          return TRUE;
     }
     else
          return FALSE;

}


//--------------------------------------------------------------------------
//                     NumericString::operator FLOAT64( )
//..........................................................................
//                                                                          
//  returns the values of the string as a 64 bit floating pt value
//                                                                          
//--------------------------------------------------------------------------

NumericString::operator FLOAT64( ) const
{
     char* ptr;
     return strtod( GetStr(), &ptr );
}





//--------------------------------------------------------------------------
//                     NumericString::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator
//                                                                          
//--------------------------------------------------------------------------


NumericString& NumericString::operator =  ( const NumericString& rStr )
{ 
     if ( this == &rStr)
          return *this;

     String::operator =( rStr );
     return *this;
}

#if defined(__UNIT_TEST__)

#define NUMERIC_WIDTH      20
#define PRECISION          18


#include <iostream.h>
#include <iomanip.h>

void main()
{
     NumericString empty;
     NumericString fp( 1.23456e-7 );
     NumericString str( String( "7891023") );
     NumericString partial_str( "z7891023qwert" , 1, 7 );
     NumericString pchar ( "9.876543e2" );
     NumericString partialpchar( "9.876543e2", 3);
     NumericString fromchar( '5' );
     NumericString fromint(  ( int) -8756 ); 
     NumericString fromuint( (unsigned int) 5694 );
     NumericString fromshort( (short) 29876 );
     NumericString fromushort( (USHORT) 62000 );
     NumericString fromlong ( (long) -1000000 );
     NumericString fromulong( (ULONG) 2000000 );

     NumericString copy( fp );


     cout << "fp as str and as numeric, and copy fcns" << endl;

     cout << ( const char * ) fp << " " 
      << setw( NUMERIC_WIDTH ) << setprecision( PRECISION ) << (FLOAT64) fp << 
      " " << setw( NUMERIC_WIDTH ) << setprecision( PRECISION )  << 
      (FLOAT64) copy << endl;
    
     cout << (const char *) str << " ";
     cout << (FLOAT64) str << endl;
     cout << (const char *) partial_str << " ";
     cout << (long) partial_str << " ";
     cout << (FLOAT64) partial_str << endl;
     cout << (const char *) pchar << " ";
     cout << (FLOAT64) pchar << endl;
     cout << (const char *) partialpchar << " ";
     cout << (FLOAT64) partialpchar << endl;
     cout << (const char *) fromchar << " ";
     cout << (FLOAT64) fromchar << endl;
     cout << (const char *) fromint << " ";
     cout << (FLOAT64) fromint << endl;
     cout << (const char *) fromuint << " ";
     cout << (FLOAT64) fromuint << endl;
     cout << (const char *) fromshort << " ";
     cout << (FLOAT64) fromshort << endl;
     cout << (const char *) fromushort << " ";
     cout << (FLOAT64) fromushort << endl;
     cout << (const char *) fromlong << " ";
     cout << (FLOAT64) fromlong << endl;
     cout << (const char *) fromulong << " ";
     cout << (FLOAT64) fromulong << endl;

     NumericString short_fp( 1.23456789012345e20, 8, 3 );
     cout << (const char *) short_fp << endl;
     cout << (FLOAT64) short_fp << endl;

     empty =  short_fp;
     cout << (const char *) empty << endl;


}



#endif


