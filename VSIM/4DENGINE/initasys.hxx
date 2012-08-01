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

//---------------------------------------------------------------------------
//
//      Declaration of INITIALIZE_AMORPHOUS_SYSTEM
//      This object is the class needed to initialize and run a simulation
//		using the amorphous_system simulator.
//      Date last modified: September 25, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef INITASYS_HXX
#define INITASYS_HXX

#include "init_sim.hxx"
#include "amrphsys.hxx"



class initialize_amorphous_system : public initialize_simulator

{

protected:
	void ReadInData();
     void SetUpSimulation();

public:
	initialize_amorphous_system () {}
	initialize_amorphous_system ( const initialize_amorphous_system& i_a_s );
	~initialize_amorphous_system() {}

};


#endif