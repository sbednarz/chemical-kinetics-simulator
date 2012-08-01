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
//      Date last modified: January 23, 1997
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
	ElapsedTime = 0.0;
	DeltaTime = 0.0;
	MeanDeltaTime = 0.0;
	ProgrammedVariable = FALSE;
	pProgrammedVariable = 0;
	pLocationOfProgrammedVariable = 0;
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
//   Purpose : to calculate time step associated with each event selection.
//		Also checks whether time step exceeds that allowe if programmed
//		variables are used.
//   Parameters : none
//   Returns: nothing
//--------------------------------------------------------------------------

void simulation_time :: SetNewTime (void)
{
	BOOL8	GoIntoIdler;
	FLOAT64	OneOverProbs, TotalProbability;

	TotalProbability = Simulator->GetTotalProbability();
	OneOverProbs = 1.0 / TotalProbability;

	// calculate time step using Gillespie's expression
	DeltaTime = - log( Simulator->GetRandomNumber() ) * OneOverProbs;

	if (ProgrammedVariable)
	{

		GoIntoIdler = pProgrammedVariable->IncrementValue( DeltaTime + ElapsedTime );

		if ( (pProgrammedVariable->IsOver() == FALSE) && (GoIntoIdler == TRUE) )
		{
			// go into idler, when conditions to return to normal simulation
			// operations are met, method returns new current time
			ElapsedTime = Idler.RunIdler();
		}

		if ( pProgrammedVariable->IsOver() )
		{
			// have run to end of profile, no need to continue to advance it with time base
			ProgrammedVariable = FALSE;
		}
	} else {

		ElapsedTime += DeltaTime;
	}

	return;

}          // end


//----------------------------------------------------------------------------
// 	Definition of method CalcMeanTimeStep
//      Purpose: estimates time step by calculating total probability and taking its
//	   inverse  (mean time step). This is SPECIFIC TO PROGRAMMED RATE CONSTANTS
//		NEEDS MODIFICATION FOR PROGRAMMED TEMPERATURE
//	Parameters: pointer to programmed variable object
//	Returns: time step value
//----------------------------------------------------------------------------

FLOAT64 simulation_time :: CalcMeanTimeStep()

{

	FLOAT64 TotalProbability, AreaProbability;

	TotalProbability = Simulator->GetTotalProbability();

	// get old probability for area where programmed variable is and subtract. Need to update
	// entire process area so subtotals are correct
	AreaProbability = pLocationOfProgrammedVariable->GetTotalProbability();
	TotalProbability -= AreaProbability;

	// update area probability using new value
	pLocationOfProgrammedVariable->CalcAllProbabilities();
	AreaProbability = pLocationOfProgrammedVariable->GetTotalProbability();

	// calculate new total probability
	TotalProbability += AreaProbability;

	if ( TotalProbability <= 0.0 )
	{
		Simulator->SetSimulationOver();
		Simulator->SetStatusCode( SIM_TERMINATE_ZERO_PROBABILITY );
		MeanDeltaTime = 0.0;
		return MeanDeltaTime;
	}

	Simulator->SetTotalProbability( TotalProbability );
     MeanDeltaTime = 1 / TotalProbability;

	return MeanDeltaTime;

}


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



