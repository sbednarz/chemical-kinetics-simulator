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
//  FLOTARRY.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the FloatArray class, to be used for 
//  containing one-dimensional arrays of floating point values 
//
//  CREATED : 6.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__FLOTARRY_HXX__)
#define __FLOTARRY_HXX__

#include "vsimtype.hxx"
#include "assert.hxx"
#include "limits.hxx"


class FloatArray
{
protected :
     UINT32           array_size;
     FLOAT64*         ptr;

public :

     FloatArray();
     FloatArray( const UINT32 ArraySize, const FLOAT64 InitialValue = 0.0 );

     virtual ~FloatArray();

     BOOL8        IsValid( ) const { return ( 0 != ptr ); }
     UINT32       GetArraySize( ) const { return array_size; }

     BOOL8        Initialize( const UINT32 ArraySize, const FLOAT64 InitialValue = 0.0 ); 

     FLOAT64      GetMinimumValue() const;
     FLOAT64      GetMaximumValue() const;

     Limits       GetLimits( ) const { return Limits( GetMinimumValue(), GetMaximumValue() ); }

     // to set a value in the array 

     inline FLOAT64&  operator [] ( UINT32 Index );

     // to retrieve a value in the array 

     inline FLOAT64   operator [] ( UINT32 Index ) const;
};

// ----------------------------------------------------------------------------
//                        FloatArray::operator [] 
// ............................................................................
// 
// to set a value in the array
//
// ----------------------------------------------------------------------------

inline FLOAT64&   FloatArray::operator [] ( UINT32 Index )
{ 
     ASSERT( Index + 1 <= array_size); 

     return ptr[ Index ]; 
} 


// ----------------------------------------------------------------------------
//                        FloatArray::operator [] 
// ............................................................................
// 
// to retrieve a value in the array
//
// ----------------------------------------------------------------------------

inline FLOAT64 FloatArray::operator [] ( UINT32 Index ) const 
{ 
     ASSERT( Index + 1 <= array_size); 

     return ptr[ Index ]; 
}


#endif

