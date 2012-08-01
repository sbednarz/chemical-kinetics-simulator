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
//  CMPSTATE.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the CompartmentState class, to be used as a container
//  for the results of a simulation specific to a given compartment 
//
//  CREATED : 6.23.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if !defined(__CMPSTATE_HXX__)
#define __CMPSTATE_HXX__

#include <sv.hxx>

#include "vsimtype.hxx"
#include "flotarry.hxx"
#include "comptobj.hxx"



DECLARE_LIST( FloatArrayList, FloatArray* )

class CompartmentState
{
protected:
     BOOL8            initialized;

     const UINT16     num_species;
     const FLOAT64    conv_factor_particles_to_moles;

     const String     name;


     FloatArray       temperature;
     FloatArray       volume;
     FloatArray       pressure;

     FloatArrayList   conc_list;
     FloatArrayList   amount_list;

public:

     CompartmentState( const CompartmentObject* pCompt, const UINT16 NumSpecies, const UINT32 ArraySize );
     ~CompartmentState( );

     BOOL8            IsInitialized( ) const { return initialized; }

     const FloatArray&     GetTemperatures( ) const { return temperature; }
     const FloatArray&     GetVolumes( ) const { return volume; }
     const FloatArray&     GetPressures( ) const { return pressure; }
     const String&         GetName( ) const { return name; }

     const FloatArray&     GetConcentrations( const SPECIES_ID Id ) const { return *(conc_list.GetObject( Id.AsIndex()-1) ); }
     const FloatArray&     GetAmounts( const SPECIES_ID Id ) const { return *(amount_list.GetObject( Id.AsIndex()-1) ); }
     BOOL8 SetDataRecord( const UINT32 RecNum, BinaryInputStream& rBIS );


};

#endif

