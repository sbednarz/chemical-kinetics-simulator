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
//  RESULTS.CXX - a header file for the VSIM User interface
// ............................................................................
//
//  This module defines  the SimulationResults class to be used to
//  display simulation results in various formats
//
//  CREATED : 6.21.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "results.hxx"
#include "mainapp.hxx"
#include "rxnschem.hxx"
#include "idtypes.hxx"
#include "cmpstate.hxx"

#endif




// ----------------------------------------------------------------------------
//                   SimulationResult::SimulationResult( )
// ............................................................................
//
// constructor 
//
// ----------------------------------------------------------------------------

SimulationResult::SimulationResult( ReactionScheme* pReactionScheme ) :
initialized( TRUE ),
pscheme( pReactionScheme ),
num_data_pts( GetpScheme()->GetNumSimulationDataPoints() ),
num_compartments( GetpScheme()->GetNumCompartments() ),
num_species( GetpScheme()->GetpSpeciesDatabase()->GetNumberOfSpecies() ),
compartment_list(),
elapsed_time( GetNumDataPts() )
{

     for ( UINT16 i = 0; i < num_compartments; i++ )
     {

          CompartmentObject*  p_compt = GetpScheme()->GetCompartmentObjectPtrFromIndex( i );

          CompartmentState* ptr = new CompartmentState( p_compt, GetNumSpecies(), GetNumDataPts() );

          if ( ptr )
               compartment_list.Insert( ptr, LIST_APPEND );
          else
               initialized = FALSE;
     }

     ReadFromFile();
}







// ----------------------------------------------------------------------------
//                   SimulationResult::GetpComptState( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

CompartmentState* SimulationResult::GetpComptState( const String& rName)
{
     compartment_list.First();

     CompartmentState* pco;

     while ( NULL != (pco = compartment_list.GetCurObject() )  )
     {
          if ( pco->GetName() == rName )
                    return  pco;               // found it - return immediately
               else
                   compartment_list.Next( );
     }

     // should not get here - we ran off the end of list w/o finding name

     ASSERT( 0 );
}


// ----------------------------------------------------------------------------
//                   SimulationResult::~SimulationResult( )
// ............................................................................
//
// destructor
//
// ----------------------------------------------------------------------------

SimulationResult::~SimulationResult()
{
     // delete any CompartmentState objects we have created

     if ( compartment_list.First() )
     {
          delete compartment_list.GetCurObject();

          while ( compartment_list.Next() )
               delete compartment_list.GetCurObject();
     }

}




// ----------------------------------------------------------------------------
//                   SimulationResult::SimulationResult( )
// ............................................................................
//
// destructor 
//
// ----------------------------------------------------------------------------

BOOL8 SimulationResult::ReadFromFile( )
{
     BinaryInputStream input_file( GetpScheme()->GetName() );

     if ( !input_file.IsOpen() )
     {
          GetpMainApp( )->FileOpenError( input_file.GetName() );
          return FALSE;
     }
     else
     {
          input_file >> *this;

          return !input_file.FileError( );
     }
}



// ----------------------------------------------------------------------------
//                  operator >>   SimulationResults
// ............................................................................
// 
//  this function sets the internal state of a SimulationResults object using 
//  data from a BinaryInputStream object
//
// ----------------------------------------------------------------------------
 
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SimulationResult& rObject )
{

     // move to file position where the simulation data records start 

     rBIS.MoveTo( rObject.GetpScheme()->GetSimulationDataFilePosition() );

     // first read initial info - look for ID 

     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SIMULATION_DATA ) )
     {
          GetpMainApp()->InvalidInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }

     // now read in simulation results, placing the data in CompartmentState
     // objects as we go

     for ( UINT32 current_rec = 0; current_rec < rObject.num_data_pts; current_rec++ )
     {
          // each record is preceded by an SIMULATION_DATA_RECORD IDType

          rBIS >> id_type;

          if ( id_type != IDType( SIMULATION_DATA_RECORD ) )
          {
               GetpMainApp()->InvalidInputFile( rBIS.GetName() );
               rBIS.SetExtErrorFlag( );
               return rBIS;
          }

          rBIS >> rObject.elapsed_time[ current_rec ];

          for ( UINT16 current_compt = 0; current_compt < rObject.num_compartments ; current_compt++ )
          {
               rObject.compartment_list.GetObject( current_compt )->SetDataRecord( current_rec, rBIS );
          }

     }

     return rBIS;
}


