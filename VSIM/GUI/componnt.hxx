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
//  COMPONNT.CXX - a module  of the VSIM User interface 
// ............................................................................
// 
//  This header file defines the Component object, which contains
//  data describing a species that is a component of an equation
//
//  CREATED : 1.27.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__COMPONNT_HXX__)
#define  __COMPONNT_HXX__


#include "specdb.hxx"
#include "vsimtype.hxx"
#include "specid.hxx"

class BinaryOutputStream;
class TextOutputStream;

class Component
{
private:
     SPECIES_ID       species_id;             // unique ID 
     String           name;             // name in text 
     NumericString    stoich_coeff;     // in equation
     NumericString    exponent;         // in rate law 

public:
            Component();
            Component( const Component& rComponent );

     Component&      operator = ( const Component& rComponent );

     void   SetSpeciesID( SPECIES_ID NewIndex ) { species_id = NewIndex; }
     SPECIES_ID GetSpeciesID(  ) const { return species_id; }

     void   SetStoichCoeff( const NumericString& rNewStr ) { stoich_coeff = rNewStr; } 
     const NumericString&    GetStoichCoeff( ) const { return stoich_coeff; }

     void  SetExponent( const NumericString& rNewStr ) { exponent = rNewStr; } 
     const NumericString&    GetExponent( ) const { return exponent; };

     void  SetName( const String& rNewStr ) { name = rNewStr; } 
     const String&    GetName( ) const { return name; };



     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Component& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Component& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const Component& rObject );

};




BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Component& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Component& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const Component& rObject );


#endif


