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
//      Methods for class SIMULATION_STATUS
//      Date last modified: August 30, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "sim_stat.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

simulation_status :: simulation_status ()

{
	// initialize counters to 1 since initial state of system is stored
	// before an event is selected.
	// TotalNumberOfEvents, PrintInterval and ElapsedTimeLimit
	// are read in from the runparm.dat file

	ReactionOver = FALSE;
	TotalNumberOfEvents = 0;
	EventCount = 1;
	ElapsedTimeLimit = 0.0;
	PrintIntervalCount = 1;
	PrintInterval = 0;
	DataRecordCount = 1;
	EquilDetectCycleLength = 0;
	EquilDetectCount = 1;
	StatusCode = SIM_RUNNING;

}

//---------------------------------------------------------------------------
//	Definition of method UPDATE_STATUS
//	Purpose: to increment event count and check for simulation termination
//		conditions, and print if it is time
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_status :: UpdateStatus ()

{
	FLOAT64 time;
	FLOAT64 probability;
	BOOL8	Equilibrium;

	EventCount++;

	if ( EventCount > TotalNumberOfEvents )
	{
		StatusCode = SIM_TERMINATE_EVENT_LIMIT;
		ReactionOver = TRUE;
	}

	time = Simulator->GetTime();
	if ( ElapsedTimeLimit > 0.0 && time >= ElapsedTimeLimit )
	{
		StatusCode = SIM_TERMINATE_ELAPSED_TIME_LIMIT;
		ReactionOver = TRUE;
	}


	PrintIntervalCount++;
	if ( PrintIntervalCount > PrintInterval )
	{
		Simulator->RecordState();
		DataRecordCount ++;
		PrintIntervalCount = 1;

		// check to see if a key has been pressed. If the engine window has
		// the focus and a key is pressed the simulation will be interrupted.
		// at present the simulation cannot be restarted

		if ( kbhit() )
		{
			StatusCode = SIM_TERMINATE_USER_ABORT;
			ReactionOver = TRUE;
		}

	}


	probability = Simulator->GetTotalProbability();
	if ( probability == 0.0 )
	{
		StatusCode = SIM_TERMINATE_ZERO_PROBABILITY;
		ReactionOver = TRUE;
	}


	if ( (Simulator->QueryEmulatorOn()) && (ReactionOver == FALSE) )
	{
		EquilDetectCount ++;
		if ( EquilDetectCount > EquilDetectCycleLength )
		{
			Equilibrium = Simulator->CheckForEquilibration();
			if ( Equilibrium )
			{
				EquilDetectCount = 0; // set to zero to skip 1 event before starting cycle

			} else {   //no equilibrated steps found

				EquilDetectCount = 1; // set to one and start cycle
			}

		}
	}
	return;

}


//---------------------------------------------------------------------------
//	Definition of data input operator >>
//	Purpose: reads some data into simulation_status object using global
//		DataCode
//	Parameters: addresses of file and simulation_status object
//	Returns: nothing
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, simulation_status& rObject )

{
	switch (DataCode) {

	case WRITE_INTERVAL:
		rTIS >> rObject.PrintInterval;
		break;

	case MAX_EVENTS:
		rTIS >> rObject.TotalNumberOfEvents;
		break;

	case ELAPSED_TIME_LIMIT:
		rTIS >> rObject.ElapsedTimeLimit;
		break;

	case EQUIL_DETECT_CYCLE_LEN:
		rTIS >> rObject.EquilDetectCycleLength;
		break;

	default:
		Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		break;

	} // end switch

     return rTIS;

}

//---------------------------------------------------------------------------

