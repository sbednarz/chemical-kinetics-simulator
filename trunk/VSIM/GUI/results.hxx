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
//  RESULTS.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the SimulationResult class, to be used for 
//  manipulating the results of a simulation
//
//  CREATED : 6.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__RESULTS_HXX__)
#define __RESULTS_HXX__

#include <sv.hxx>

#include "vsimtype.hxx"
#include "cmpstate.hxx"

class ReactionScheme;

DECLARE_LIST( CompartmentStateList, CompartmentState* )

class SimulationResult
{
protected:
     BOOL8                      initialized;
     ReactionScheme*            pscheme;

     const UINT32               num_data_pts;
     const UINT16               num_compartments;
     const UINT16               num_species;

     CompartmentStateList       compartment_list;

     FloatArray                 elapsed_time;

     BOOL8                      ReadFromFile( );
     ReactionScheme*            GetpScheme( ) const { return pscheme; }
     UINT32                     GetNumDataPts( ) const { return num_data_pts; }
     UINT16                     GetNumCompartments( ) const { return num_compartments; }
     UINT16                     GetNumSpecies( ) const { return num_species; }

public:
     SimulationResult( ReactionScheme* pReactionScheme );

     ~SimulationResult();


     CompartmentState*          GetpComptState( UINT16 ComptIndex ) { return compartment_list.GetObject( ComptIndex ); }
     CompartmentState*          GetpComptState( const String& rName);
     const FloatArray&          GetElapsedTimes() const { return elapsed_time; }


     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SimulationResult& rObject );


};


BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SimulationResult& rObject );

#endif

