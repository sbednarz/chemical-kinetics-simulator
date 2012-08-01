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

//--------------------------------------------------------------------------
//
// 	Methods for class PROPS_LIBRARY
//	Date last modified: December 16, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#include "prop_lib.hxx"
#include "simulatr.hxx"

//--------------------------------------------------------------------------
//   Definition of operator >>
//   Purpose: to allocate space and read in materials properties data for
//        the simulation. Only needed for variable T and variable V simulations
//   Parameters: addresses of input file and of data object
//   Returns: nothing
//--------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, props_library& rObject )

{
     UINT16 j, n, NoSpecies, temp;

     //read in number of species in table
     rTIS >> DataCode;
     ASSERT ( DataCode == NUM_SPECIES_IN_DATA_TABLE );
     rTIS >> NoSpecies;

     // add 1 to NoSpecies for table allocation - converts from 0base
     // to 1based array used for species numbering

     NoSpecies++;

     //allocate memory for table
     rObject.MaterialsProps = new properties [ NoSpecies ];
     if ( rObject.MaterialsProps == 0 )
	  {
	       Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
	       return rTIS;
	  }


     for ( j=1; j < NoSpecies; j++ )
     {
	       rTIS >> DataCode;
	       ASSERT ( DataCode == SPECIES_DATA_TABLE_ENTRY );

	       rTIS >> n;

            // read in properties data, store species ID
	       rObject.MaterialsProps[n].ID = n;
	       rTIS >> rObject.MaterialsProps[n].Density;
	       rTIS >> temp;
            rObject.MaterialsProps[n].Phase = ( COMPARTMENT_PHASE_TYPE )temp;
	       rTIS >> rObject.MaterialsProps[n].Enthalpy;
	       rTIS >> rObject.MaterialsProps[n].CoeffA;
	       rTIS >> rObject.MaterialsProps[n].CoeffB;
            rTIS >> rObject.MaterialsProps[n].CoeffC;
            rTIS >> rObject.MaterialsProps[n].CoeffD;

     }   // end for

     // check for end of species data table
     rTIS >> DataCode;
     ASSERT ( DataCode == END_SPECIES_DATA_TABLE_INIT );

     return rTIS;

}


//--------------------------------------------------------------------------
