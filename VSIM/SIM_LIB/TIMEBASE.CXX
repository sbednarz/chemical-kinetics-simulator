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
//      Methods for class SIMULATION_TIME
//      Date last modified: March 8, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include <math.h>
#include "vsimtype.hxx"
#include "timebase.hxx"
#include "simulatr.hxx"
#include "idtypes.hxx"

//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

simulation_time :: simulation_time ()

{
	DeltaTime = 0.0;
	ElapsedTime = 0.0;
	return;
}

//--------------------------------------------------------------------------
//	Definition of method InitTime
//	Purpose: to set time base to pre-determined values (non-zero)
//	Parameters: time and deltatime values
//	Returns: nothing
//--------------------------------------------------------------------------

void simulation_time :: InitTime ( FLOAT64 t, FLOAT64 dt )

{
	DeltaTime = dt;
	ElapsedTime = t;
	return;
}

//--------------------------------------------------------------------------
//  Definition of function SetNewTime
//   Purpose : to calculate time step associated with each event selection
//   Parameters : none
//   Returns: nothing
//--------------------------------------------------------------------------

void simulation_time :: SetNewTime (void)
{                                   //begin
	FLOAT64 TotalProbability;

	TotalProbability = Simulator->GetTotalProbability();

	// calculate using Gillespie's expression
	DeltaTime = - log( Simulator->GetRandomNumber() ) / TotalProbability;
	ElapsedTime += DeltaTime;

}          // end


//--------------------------------------------------------------------------
//        Definition of binary output operator <<
//        Purpose: to write out current system time
//        Parameters: output file and object references
//        Returns: pointer
//--------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_time& rObject )

{
     rBOS << rObject.ElapsedTime;
     return rBOS;

}

//--------------------------------------------------------------------------



