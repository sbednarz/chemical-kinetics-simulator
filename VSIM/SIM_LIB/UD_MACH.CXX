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
//
//      Definition of methods for class UPDATE_MACHINE
//      Date last modified : May 3, 1996
//      Written by Frances A. Houle
//      IBM  
//
//-----------------------------------------------------------------------------

#include "ud_mach.hxx"
#include "simulatr.hxx"


//-----------------------------------------------------------------------------
//	Constructor
//-----------------------------------------------------------------------------

update_machine :: update_machine ()

{
	UpdateStep = 0;
	NumberOfUpdateSteps = 0;
	// the following is 4 required + 3 options
	MaxNoOfUpdateSteps = 7;
}

//-----------------------------------------------------------------------------
//	Definition of method Build
//	Purpose: to set up the updating sequence (UpdateStep array)
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void update_machine :: Build()

{
	UINT16 j;

	// allocate an array of pointers, 4 for time, chem, probability and
	// the equilibrium emulator updating/resetting
	// which must always be updated, and one each for the optional updates
	// of T, P and V. The latter may or may not be assigned. The variable
	// NumberOfUpdateSteps keeps track of how many of the pointers are
	// actually in use.

	UpdateStep = new  state_update  *[ MaxNoOfUpdateSteps ];
	if ( UpdateStep == 0 )
	{
		Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
	}


       // This is a preliminary version, Eventually
       // Build() will have as arguments flags for selected options

	j=0;

	UpdateStep[j] = &TimeUpdate;
	j++;

	UpdateStep[j] = &ChemUpdate;
	j++;

	// put here optional updates for variable T and P
	// increment j after each assignment

	UpdateStep[j] = &VolumeUpdate;
	j++;

	UpdateStep[j] = &ProbUpdate;
	j++;

	if ( Simulator->QueryEmulatorOn() )
	{

		UpdateStep[j] = &EmulUpdate;
		j++;
	}

	NumberOfUpdateSteps = j;

	return;

}


//-----------------------------------------------------------------------------
//
//      Definition of method UpdateTheSystem
//      Purpose: to step through the array of pointers to individual update processes
//              and execute them
//      Parameters: array element number
//      Returns: nothing
//
//-----------------------------------------------------------------------------

void update_machine :: UpdateTheSystem (void)
{  // begin method

	UINT16 j;

	for (j = 0; j < NumberOfUpdateSteps ; j++ )

	{

		UpdateStep[j]->UpdateState();

	}  // end for

	// if equil detect is enabled, the system may need to return to the
	// emulator to reset the equil detect cycle

}  // update method

//-----------------------------------------------------------------------------
