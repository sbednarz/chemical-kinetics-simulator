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
//  NUMSTRNG.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the NumericString class, which provides the means of 
//  validating and converting character string data into  various forms ofh
//  numeric data 
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__NUMSTRNG_HXX__)
#define __NUMSTRNG_HXX__

#include "vsimtype.hxx"
#include "binstrem.hxx"

const int DEFAULT_FIELD_WIDTH = 12;
const int DEFAULT_DIGITS  = 8 ;

class NumericString : public String
{
private :

public :
     BOOL8 IsValidFLOAT64( PFLOAT64 pResult = NULL ) const;
     BOOL8 IsValidNonNegativeFLOAT64( PFLOAT64 pResult = NULL ) const; 
     BOOL8 IsValidPositiveFLOAT64( PFLOAT64 pResult = NULL ) const; 
     BOOL8 IsValidNonZeroFLOAT64( PFLOAT64 pResult = NULL ) const; 
     BOOL8 IsValidUINT32( PUINT32 pResult = NULL ) const; 
     BOOL8 IsValidUINT16( PUINT16 pResult = NULL ) const; 
     BOOL8 IsValidPositiveINT32( PINT32 pResult = NULL ) const; 
     BOOL8 IsValidPositiveINT16( PINT16 pResult = NULL ) const; 
     BOOL8 IsValidNonNegativeINT16( PINT16 pResult = NULL ) const; 

     // constructors - all those found in parent class and then some 

         NumericString ( ) : String("0") { } 
         NumericString( const ResId& rResId ) : String( rResId ) {}
         NumericString( const String& rStr ): String( rStr ) {}
         NumericString( const String& rStr, USHORT nPos, USHORT nLen ) : String( rStr, nPos, nLen ) {} 
         NumericString( const char* pCharStr ): String( pCharStr ) {}
         NumericString( const char* pCharStr, USHORT nLen ): String( pCharStr, nLen ) {}
         NumericString( char c ) : String( c ) {}
         NumericString( int n ) : String( n ) {}
         NumericString( unsigned int n ): String( n ) {}
         NumericString( short n ) : String( n ) {}
         NumericString( USHORT n ) : String( n ) {}
         NumericString( long n ): String( n ) {}
         NumericString( ULONG n ): String( n ) {}

         NumericString( const NumericString& rNumStr ) : String( rNumStr.GetStr() ) {}
         NumericString ( FLOAT64 f, UINT16 MinFieldWidth = DEFAULT_FIELD_WIDTH, UINT16 DigitsAfterDecPt = DEFAULT_DIGITS );

         // typecast 
         operator        FLOAT64() const;

          // operators 
        NumericString&   operator =  ( const NumericString& rStr );
};



#endif
