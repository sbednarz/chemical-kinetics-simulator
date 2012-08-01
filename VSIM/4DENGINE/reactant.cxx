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
//      Methods for class REACTANT
//      derived from base class SPECIES
//      Date last modified: September 24, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "reactant.hxx"
#include "simulatr.hxx"

//--------------------------------------------------------------------------
//        Constructor
//--------------------------------------------------------------------------

reactant :: reactant ()
: species ()

{
     InitialNoOfMoles = 0.0;
     VolumePerParticle = 0.0;
     Phase = NO_PHASE;
}


//--------------------------------------------------------------------------
//	Description of method InitParticles
//	Purpose: to convert an initial amount to number of particles 
//	Parameters: none
//	returns: nothing
//--------------------------------------------------------------------------

void reactant :: InitParticles ()

{    // begin

	FLOAT64 ConvFactor;

	// calculates the number of particles from the initial concentration
	// and does a cast to ParticleType

	if ( InitialNoOfMoles )
	{
		ConvFactor = Simulator->GetParticlesPerMol();
		no_particles = (ParticleType) (round ( InitialNoOfMoles  * ConvFactor ) );
	}

	else

	{
		no_particles = 0;
	}


	return;

}   // end


//---------------------------------------------------------------------------
//
//	Definition of method CalcVolPerParticle
//	Purpose: to pre-calculate conversion factor to calculate new volumes.
//		assumes that density is independent of temperature
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void reactant :: CalcVolPerParticle( UINT16 SpeciesNo )

{
	UINT16 	j;
	FLOAT64 Density;

	// calculation for gas, liquid and solid phases

	Density = ( Simulator->GetMaterialsProperties( SpeciesNo ) ).Density;
	VolumePerParticle = 1 / ( Density * Simulator->GetParticlesPerMol() );

	return;

}

//---------------------------------------------------------------------------
//   Definition of text input operator >>
//   Purpose: to store data for a single reactant
//   Parameters: input file and compartment reference addresses
//   Returns: address of input file
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, reactant& rObject )

{
     // read in concentration in moles/liter units
     rTIS >> rObject.InitialNoOfMoles;

     return rTIS;
}


//--------------------------------------------------------------------------

