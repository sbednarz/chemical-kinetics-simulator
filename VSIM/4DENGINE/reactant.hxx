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
//      Declaration of class REACTANT
//      derived from base class SPECIES
//      Date last modified: December 16, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef REACTANT_HXX
#define REACTANT_HXX

#include "species.hxx"
#include "vsimutil.hxx"
#include "txtstrem.hxx"


class reactant : public species {

protected:
	FLOAT64 	                    InitialNoOfMoles;
	FLOAT64		               VolumePerParticle;
     enum COMPARTMENT_PHASE_TYPE   Phase;

public:
	reactant ();
	reactant ( const reactant& r );
	~reactant () { };

	void 		                 InitParticles ();
     enum COMPARTMENT_PHASE_TYPE     GetPhase() { return Phase; }
	void		                      SetInitMoles ( FLOAT64 v ) { InitialNoOfMoles *= v; }
	FLOAT64 	                      GetInitMoles () { return InitialNoOfMoles; }
	void		                      CalcVolPerParticle( UINT16 SpeciesNo );
	FLOAT64 	                      GetVolPerParticle() { return VolumePerParticle; }
	FLOAT64		                 PartialVolume() { return VolumePerParticle*( (FLOAT64)no_particles ); }


	friend TextInputStream& operator >> ( TextInputStream& rTIS, reactant& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, reactant& rObject );

#endif



