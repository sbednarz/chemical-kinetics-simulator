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
//      Methods for class EQUILIBRIUM_EMULATOR
//      Date last modified: October 9, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "eq_emul.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

equilibrium_emulator :: equilibrium_emulator ()

{
	MinEfficiency = 0.0;
	SelectionFrequency = 0.0;
	NoRevStepsInSystem = 0;
	EventHistory = 0;
	Counter = 0;
	Equilibrium = FALSE;

}


//---------------------------------------------------------------------------
//	Definition of method LogEvent
//	Purpose: to store information about a selected event when the
//		equilibrium emulator is in use
//	Parameters: Counter (location in array)
//	Returns: nothing
//---------------------------------------------------------------------------

void  equilibrium_emulator :: LogEvent()
{
	selected_event_info 	Event;
	process_info       	StepData;
	BOOL8			Logged;
	UINT32			j;

	// check flag to see if this is a regular equil detect cycle, or selection of a non-equilibrium event
	// after the equilibrated steps have been frozen.
	if ( !Equilibrium )  // events are only logged if the selection cycle is a normal one
	{
		// get selected event info
		Event = Simulator->GetEventInfo();

		//Check to see if selected step is reversible
		switch ( Event.Location ) {

		case  REACTION_COMPARTMENT:
			StepData = Simulator->GetStepDataFromCompartment( Event.ProcessAreaNo, Event.EventNo);
			break;

		case  MASS_TRANSFER:
			StepData = Simulator->GetStepDataFromTransferPath( Event.ProcessAreaNo, Event.EventNo);
			break;

		default:
			break;

		}  // end switch

		if ( StepData.Direction == REVERSIBLE )
		{
			j = 0;
			Logged = FALSE;

			// go through existing list to see if there are matches

			while ( j <= Counter )
			{
				if ( EventHistory[ j ].EventInfo == Event )
				{
					EventHistory[ j ].NoTimesSelected ++;
					Logged = TRUE;
					break;
				}

				j++;
			}

			if ( !Logged )  // add new member to array of selected reversible events
			{
				Counter ++;
				j = Counter - 1;  // base 0 array
				EventHistory[ j ].EventInfo.EventNo = Event.EventNo;
				EventHistory[ j ].EventInfo.ProcessAreaNo = Event.ProcessAreaNo;
				EventHistory[ j ].EventInfo.Location = Event.Location;
				EventHistory[ j ].PartnerStepNo = StepData.PartnerStepNo;
				EventHistory[ j ].NoTimesSelected ++;

			}

		}  // end if StepData

	} else {

		// reset Equilibrium flag, non-equil event has been selected and now start a new detect cycle
		Equilibrium = FALSE;

	} // end if not Equilibrium


	return;

}


//---------------------------------------------------------------------------
//	Definition of method CheckEventPattern
//	Purpose: to examine event record and see if any reversible reactions are in
//		equilibrium, then set flag and reset counter
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

BOOL8 equilibrium_emulator :: CheckEventPatternForEquilibrium()

{
	UINT32			j, k;
	UINT32			PartnerStepNo;
	selected_event_info	Event;
	UINT32			TotalNoOfEvents;
	BOOL8 			FoundMatch, ContinueEmul;
	FLOAT64			PairEfficiency, CurrentTotalProb;


	TotalNoOfEvents = 0;
	ContinueEmul = FALSE;
     CurrentTotalProb = Simulator->GetTotalProbability();

	j = 0;

	while ( j < Counter )
	{
		// check to see that the times selected datum is non-zero - this is a flag
		// that an event has already been examined
		if ( EventHistory[j].NoTimesSelected )
		{
			// construct partner event data structure for jth element of array
			Event.ProcessAreaNo = EventHistory[j].EventInfo.ProcessAreaNo;
			Event.EventNo = EventHistory[j].PartnerStepNo;
			Event.Location = EventHistory[j].EventInfo.Location;

			// compare data to rest of array
			k = j + 1;
			FoundMatch = FALSE;

			while ( k <= Counter )
			{
				if ( Event == EventHistory[k].EventInfo )
				{
					FoundMatch = TRUE;
					break;
				}

				k++;
			}  // end while

			if ( FoundMatch )
			{
				PairEfficiency = fabs(((FLOAT64)EventHistory[j].NoTimesSelected - (FLOAT64)EventHistory[k].NoTimesSelected)
					/ (FLOAT64)EventHistory[j].NoTimesSelected);

				if (PairEfficiency < MinEfficiency)				{
					// steps are in equilibrium
					TotalNoOfEvents += ( EventHistory[j].NoTimesSelected + EventHistory[k].NoTimesSelected );
					// mark events as having been looked at by setting partner selection count = 0
					EventHistory[k].NoTimesSelected = 0;

				} else {
					// set event count to 0 to flag that these steps are not equilibrated
					EventHistory[j].NoTimesSelected = 0;
					EventHistory[k].NoTimesSelected = 0;

				} // end if

			} else {
				// no match
				EventHistory[j].NoTimesSelected = 0;

			} // end if FoundMatch

		}  // end if EventHistory[j]

		j++;

	} // end while j

	// check to see if total number of equilibrium events passes SelectionFrequency test

	if ( (FLOAT64)TotalNoOfEvents >= SelectionFrequency )
	{
		Equilibrium = TRUE; // set emulator flag

		// freeze equilibrated steps for next event selection cycle until all equilibria are
          // frozen or probability drops to zero. If all are frozen and the total probability > 0
          // then ContinueEmul will be TRUE, otherwise it will be false
          j=0;
		while ( j < Counter )
		{
			if ( EventHistory[j].NoTimesSelected )
			{
				ContinueEmul = Simulator->FreezeEquilibrium( EventHistory[j] );
                    if ( !ContinueEmul ) break;
			}

               j++;

		}

	}

	// if total events < selection frequency, or no finite probs are left after
	// equilibria are frozen, reset the emulator and the system total probability
     // (reverse freeze)
	if ( !ContinueEmul )
	{
		ResetEmulator();
          Simulator->SetTotalProbability( CurrentTotalProb );
	}


	// returns flag
	return Equilibrium;

}


//---------------------------------------------------------------------------
//
//	Definition of method ResetEmulator
//	Purpose: to reset flags and arrays after equildetect cycle
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void equilibrium_emulator :: ResetEmulator()

{
	UINT32  j;

	// reset flag
	Equilibrium = FALSE;

	// reset array length
	Counter = 0;

	// reset data array
	for ( j=0; j < NoRevStepsInSystem; j++ )
	{
		EventHistory[j].NoTimesSelected = 0;
	}

	return;

}


//---------------------------------------------------------------------------
//
//	Definition of method StartNewCycle
//	Purpose: to reactivate frozen equilibrium steps, and reset emulator
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void equilibrium_emulator :: StartNewCycle ()

{
	UINT32 j;

	for ( j=0; j < Counter; j++ )
	{
		if ( EventHistory[j].NoTimesSelected )
		{
			Simulator->RestoreEquilibrium( EventHistory[j] );

		}

	}

	ResetEmulator();

	return;

}


//---------------------------------------------------------------------------
//
//   Definition of method InitEventHistory
//   Purpose: to allocate and initialize histogramming array for reversible
//        event selection
//   Parameters: none
//   Returns: nothing
//
//---------------------------------------------------------------------------

void equilibrium_emulator :: InitEventHistory( UINT32 NoSteps, UINT32 CycleLength )

{
     UINT32    j;

     // initialize required variables
     NoRevStepsInSystem = 2 * NoSteps;
     SelectionFrequency = ( MinEfficiency / 100.0 ) * (FLOAT64)CycleLength;
     MinEfficiency = 1 - MinEfficiency / 100.0;

     // allocate EventHistory array - 2* # of reversible pairs

     EventHistory = new equil_detect_info [ NoRevStepsInSystem ];

     if ( EventHistory == 0 )
     {
	  Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
	  return;

     }

     // initialize NoTimesSelected and Assessed array elements - the rest of the data
     // structure will be overwritten as needed
     for ( j=0; j < NoRevStepsInSystem; j++ )
     {
	  EventHistory[j].NoTimesSelected = 0;
     }

     return;

}


//---------------------------------------------------------------------------
//
//	Definition of text input operator >>
//	Purpose: to read data from runparm file into equilibrium detect object
//		using global DataCode
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, equilibrium_emulator& rObject )

{
	switch (DataCode) {

	case EQUIL_DETECT_MIN_EFF:
		rTIS >> rObject.MinEfficiency;
		break;

	default:
		Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		break;

	} //end switch

     return rTIS;

}


//---------------------------------------------------------------------------
//
//   Definition of operator == for selected_event_info structures
//   Purpose: to determine equility of 2 structures
//   Parameters: structures
//   Returns: TRUE or FALSE
//
//---------------------------------------------------------------------------

BOOL8 operator == ( selected_event_info& a, selected_event_info& b )

{
     if (( a.ProcessAreaNo == b.ProcessAreaNo ) &&
         (a.EventNo == b.EventNo) &&
         (a.Location == b.Location))

         {
               return TRUE;

         }

         return FALSE;

}


//---------------------------------------------------------------------------


