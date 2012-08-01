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
//  COMPONNT.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Component object, which contains
//  data describing a species that is a component of an equation
//
//  CREATED : 1.23.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "componnt.hxx"
#include "idtypes.hxx"
#include "mainapp.hxx"

#endif




//--------------------------------------------------------------------------
//                     Component::Component()
//..........................................................................
//                                                                          
//  default constructor 
//                                                                          
//--------------------------------------------------------------------------

Component::Component() :
species_id( INVALID_ID ),
name(),
stoich_coeff(),
exponent("")
{
}




//--------------------------------------------------------------------------
//                     Component::Component()
//..........................................................................
//                                                                          
//  copy constructor 
//                                                                          
//--------------------------------------------------------------------------

Component::Component( const Component& rComponent ) :
species_id( rComponent.GetSpeciesID() ),
name( rComponent.GetName() ),
stoich_coeff( rComponent.GetStoichCoeff() ),
exponent(rComponent.GetExponent() )
{
}



//--------------------------------------------------------------------------
//                     Component::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator    
//                                                                          
//--------------------------------------------------------------------------

Component&   Component::operator = ( const Component& rComponent )
{
     // avoid assignment to self 

     if ( &rComponent == this )
          return *this;

     // simple types 

     species_id    = rComponent.species_id;
     name          = rComponent.name;
     stoich_coeff  = rComponent.stoich_coeff;
     exponent      = rComponent.exponent;

     return *this;
}







//--------------------------------------------------------------------------
//                     operator << 
//..........................................................................
//                                                                          
//  binary output operator    
//                                                                          
//--------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Component& rObject )
{
     rBOS << IDType( RXN_COMPONENT );

     rBOS << rObject.species_id;             // unique ID 
     rBOS << rObject.name;             // name in text 
     rBOS << rObject.stoich_coeff;     // in equation
     rBOS << rObject.exponent;         // in rate law 

     return rBOS;
}

 



//--------------------------------------------------------------------------
//                     operator << 
//..........................................................................
//                                                                          
//  a text output operator    
//                                                                          
//--------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const Component& rObject )
{
     rTOS << IDType( RXN_SPECIES_DATA ) << rObject.species_id <<
      rObject.stoich_coeff << rObject.exponent << EOL;

     return rTOS;
}

 



//--------------------------------------------------------------------------
//                     operator >> 
//..........................................................................
//                                                                          
//  binary input operator    
//                                                                          
//--------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Component& rObject )
{
     // first check the IDType

     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( RXN_COMPONENT ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }

     rBIS >> rObject.species_id;             // unique ID 
     rBIS >> rObject.name;             // name in text 
     rBIS >> rObject.stoich_coeff;     // in equation
     rBIS >> rObject.exponent;         // in rate law 


     return rBIS;
}

