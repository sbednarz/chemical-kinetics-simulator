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
//      Declaration of class SPECIES
//      base class
//      Date last modified: October 30, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef SPECIES_HXX
#define SPECIES_HXX

#include "vsimtype.hxx"
#include "datastru.hxx"
#include "binstrem.hxx"

class species {

protected:
	ParticleType no_particles;

public:
	species ( ParticleType np = 0 ) { no_particles = np; }
	species ( const species& s ) { no_particles = s.no_particles; }
	virtual ~species () {}
	void SetParticles ( ParticleType np ) { no_particles = np; }
	ParticleType GetParticles () { return no_particles; }
	virtual void InitParticles () = 0;
	void IncrementParticles ( ParticleType s ) { no_particles += s; }
	void DecrementParticles ( ParticleType s ) { no_particles -= s; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, species& rObject );

};

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, species& rObject );

#endif


