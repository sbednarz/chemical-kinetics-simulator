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

//-----------------------------------------------------------------------------
//            MAIN
//	This is the main simulation loop for a 3-d simulation
//	Date last modified: March 14, 1996
//	Written by Frances A. Houle
//	IBM  
//-----------------------------------------------------------------------------

// header for desired simulator type
#include "3d_sys.hxx"

// header to set up simulation for desired type
#include "init3sys.hxx"

//instantiate the simulator pointer and link to desired simulator type
simulation_system* Simulator = new three_d_system;

void main()
{

	//instantiate initialization object
	initialize_three_d_system Initializer;

	//read in data and set up simulation
	Initializer.Go();

	// check for allocation errors, file errors and zero probability
	if ( Simulator->GetStatusCode() != SIM_RUNNING )
	{
		Simulator->Terminate ();
	  return;
	}

	//execute simulation loop
	while ( !Simulator->IsSimulationOver() )
	{
		Simulator->SelectEvent();
		Simulator->UpdateSystem();
		Simulator->UpdateEventCounters();
	}  //end while

	// end simulation
	Simulator->Terminate ();

}


//-----------------------------------------------------------------------------
