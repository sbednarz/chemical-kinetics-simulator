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
//  CMPSTATE.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module file defines the CompartmentState class, to be used as a container
//  for the results of a simulation specific to a given compartment
//
//  CREATED : 6.23.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "cmpstate.hxx"
#include "binstrem.hxx"
#include "idtypes.hxx"
#include "mainapp.hxx"

#endif




// ----------------------------------------------------------------------------
//                   CompartmentState::CompartmentState()
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

CompartmentState::CompartmentState( const CompartmentObject* pCompt, 
 const UINT16 NumSpecies, const UINT32 ArraySize ) :
initialized( TRUE ),
num_species( NumSpecies ),
conv_factor_particles_to_moles( pCompt->GetpParent()->GetScheme().GetConvFactorParticlesToMoles() ),
name( pCompt->GetName() ),
temperature(),
volume(),
pressure(),
conc_list( NumSpecies ),
amount_list( NumSpecies )
{
     temperature.Initialize( ArraySize );

     volume.Initialize( ArraySize );

     pressure.Initialize( ArraySize );

     for ( UINT16 i = 0; i < NumSpecies; i++ )
     {
          // create the FloatArray  objects for concentrations and amounts 
          // first for conc_list

          FloatArray*  ptr = new FloatArray( ArraySize );

          if ( ptr )
               conc_list.Insert( ptr, LIST_APPEND );
          else
               initialized = FALSE;

          // then for amount_list

          ptr = new FloatArray( ArraySize );

          if ( ptr )
               amount_list.Insert( ptr, LIST_APPEND );
          else
               initialized = FALSE;
     }

}



CompartmentState::~CompartmentState( )
{
     if ( conc_list.First() )
     {
          delete conc_list.GetCurObject();
 
          while ( conc_list.Next() )
               delete conc_list.GetCurObject();
     }


}



// ----------------------------------------------------------------------------
//                   CompartmentState::SetDataRecord( )
// ............................................................................
//
//  Adds one compartment state data record to the data arrays contained inside
//  the CompartmentState Object. A compartment state data record specifies the
//  complete internal state of a CompartmentObject at a specific point in time
//
// ----------------------------------------------------------------------------

BOOL8 CompartmentState::SetDataRecord( const UINT32 RecNum, BinaryInputStream& rBIS )
{
     // a check for file validity 

     IDType id_type; 

     rBIS >> id_type;

     if ( id_type != IDType( COMPARTMENT_STATE ) )
     {
          GetpMainApp()->InvalidInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return FALSE;
     }

     // now read in data

     // (1) temperature

      rBIS >> temperature[ RecNum ];

     // (2) pressure

     rBIS >> pressure[ RecNum ];

     // (3) volume

     FLOAT64 current_volume;
     rBIS >> current_volume;
     volume[ RecNum ] = current_volume;

     ASSERT( current_volume != 0.0 );

     // (4) and concentrations of all species

     for ( UINT16 i = 0; i < num_species; i++ )
     {
          FLOAT64  amount;

          rBIS >> amount;

          amount *= conv_factor_particles_to_moles;

          (*(amount_list.GetObject( i )))[ RecNum ] = amount;

          (*(conc_list.GetObject( i )))[ RecNum ] = amount/current_volume;
     }



     return !rBIS.FileError();
}

