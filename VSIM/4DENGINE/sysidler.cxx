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
//      Methods for class SYSTEM_IDLER
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "sysidler.hxx"
#include "simulatr.hxx"

//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

system_idler :: system_idler()
{
	pProgrammedVariable = 0;
	pTimeBase = 0;
}


//--------------------------------------------------------------------------
//	Definition of method RunIdler
//      Purpose: steps profile value until reaction probabilities reach a threshold
//        value (avg time step < max time step.
//	Parameters: starting time, avg time step
//      Returns: nothing
//--------------------------------------------------------------------------

FLOAT64 system_idler :: RunIdler()
{
     profile_status 	Profile;
     BOOL8		     OutOfRange;
     FLOAT64		     MeanTimeStep, dummy;

     dummy = 0.0;

	// get current characteristics of profile segment
	Profile = pProgrammedVariable->GetCurrentStatus();

	// set time step to max value to ensure that the idler goes through at least
	// one loop
	MeanTimeStep = DBL_MAX;

	// check that the current time-T point is not close to a profile point - can
	// lead to simulator spinning its wheels. Works for positive and negative slope.

	  if ( (Profile.Slope != 0.0) && ( (abs(Profile.CurrentValueInProfile -
				Profile.NextProfilePoint)) <= Profile.IdlingValueStep ) )
	  {
	       OutOfRange = pProgrammedVariable->MoveToNextSegment( Profile.NextProfilePoint, dummy );
	       if (OutOfRange)
	       {
			return Profile.CurrentTimeInProfile;
	       }
	       // refresh status
	       Profile = pProgrammedVariable->GetCurrentStatus();
	  }

	// enter idling cycle

	// Print state of system before starting
	Simulator->RecordState();

	//IDLING LOOP
	for (;; )
	{

	// #if  defined(__OS2__)
	//    if ( RescheduleFlag )
	// #endif
	//    Reschedule( );

		// TIME STEP TEST - POINT TO BREAK OUT OF LOOP
	       if ( Profile.MaxTimeStep >= MeanTimeStep )// tests 1/probs
	       {
		    break;
	       }

		// IF ZERO SLOPE FIND FINITE SLOPE

	       if ( Profile.Slope == 0.0 )
	       {
		 // move through progfile segments until slope becomes finite
		    do
		    {
			 OutOfRange = pProgrammedVariable->MoveToNextSegment( Profile.NextProfilePoint, dummy );

			if (OutOfRange)
		       {
				return Profile.CurrentTimeInProfile;
		       }
		       // refresh status
			Profile = pProgrammedVariable->GetCurrentStatus();
		       // update time using new place in profile and print status
		       pTimeBase->SetNewTime( Profile.CurrentTimeInProfile );
		       Simulator->RecordState();

		       // check to see if the new slope is finite
			 if ( Profile.Slope )
			 {
			      break;
			 }
		    }
		    while ( TRUE );  // end do

	       }  // end if Profile.Slope

		// FINITE SLOPE
	       if ( Profile.Slope )
	       {
			pProgrammedVariable->IncrementValue();
			Profile = pProgrammedVariable->GetCurrentStatus();

			if ( !OutOfRange )
			{
				// get new probabilities and  AVERAGE time step
				MeanTimeStep = pTimeBase->CalcMeanTimeStep();
				if ( Simulator->IsSimulationOver() )
				{
					// stop idling if all probabilities are zero
					return Profile.CurrentTimeInProfile;
				}

				// update system time to keep all in synch
				pTimeBase->SetNewTime( Profile.CurrentTimeInProfile );
			}
			else
			{		// OUT OF BOUNDS OF PROFILE
			    return Profile.CurrentTimeInProfile;
			}   // endif not OutOfRange

	       } // end if slope

	  }    // end for ( infinite loop )

	return Profile.CurrentTimeInProfile;
}


//--------------------------------------------------------------------------
//   Definition of method SetIdlerPointers
//   Purpose: to set pointers to profile and timebase object
//   Parameters: pointers
//   Returns: nothing
//--------------------------------------------------------------------------

void system_idler :: SetPointers( profile* p, simulation_time* s )
{
     pProgrammedVariable = p;
     pTimeBase = s;
     return;
}


//--------------------------------------------------------------------------


