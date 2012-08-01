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
//      Methods for class SELECTOR
//      Date last modified: December 16, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "selector.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

selector :: selector (void)

{
	//initialize to arbitrary values: event 0 in compartment 0

	SelectedEvent.ProcessAreaNo = 0;
	SelectedEvent.EventNo = 0;
	SelectedEvent.Location = REACTION_COMPARTMENT;

}


//--------------------------------------------------------------------------
//  Definition of function SetSelectedEventInfo
//  Purpose: to store info about the selected event
//  Parameters: data for the structure
//  Returns: nothing
//--------------------------------------------------------------------------

void selector :: SetSelectedEventInfo (UINT16 pan, UINT32 en, enum PROCESS_AREA_TYPE l )

{
	SelectedEvent.ProcessAreaNo = pan;
	SelectedEvent.EventNo = en;
	SelectedEvent.Location = l;
}

//--------------------------------------------------------------------------
//  Definition of function Select
//  Purpose:  to use RandomNumber to Select Event
//  Parameters: none
//  Returns: selected event no
//--------------------------------------------------------------------------

void selector :: Select( void )

{                                      // begin function
     FLOAT64 	               ProbSubtotal, TotalProbability;
     FLOAT64 	               RandomNo;
     UINT16 	               nc, ntp, ns;
     UINT16 	               j;  //j tracks process areas,
     UINT32 	               k;  // k tracks steps
     enum PROCESS_AREA_TYPE   Place;
     BOOL8 	               CoarseSearch = TRUE; //flag to track whether process areas or
				//individual steps are scanned

     // get required quantities from Simulator object
     TotalProbability = Simulator->GetTotalProbability();
     nc = Simulator->GetNoOfCompartments();
     ntp = Simulator->GetNoOfTransferPaths();

     // get and normalize random number
     RandomNo = Simulator->GetRandomNumber();
     RandomNo *= TotalProbability;

     // select event

     ProbSubtotal = 0.0;
     j = 0;

     //search reaction compartments
     Place = REACTION_COMPARTMENT;

     while ( j < nc )
     {
	  ProbSubtotal += Simulator->GetCompartmentProbability(j);
	  if ( ProbSubtotal > RandomNo )
	  {
	       ProbSubtotal -= Simulator->GetCompartmentProbability(j); //back up
	       CoarseSearch = FALSE;
	       break;
	  }                            // endif

	  j++;
     }                                 // endwhile j

     if (CoarseSearch == FALSE)
     {       //do fine search of compartment
	ns = Simulator->GetNoStepsInCompartment(j);
	k = 0;

	while ( k < ns )
	{      // scan reaction steps
	   ProbSubtotal += Simulator->GetStepProbFromCompartment(j, k);
	   if (ProbSubtotal > RandomNo)
	   {
		SetSelectedEventInfo (j, k, Place);
		return;
	   }                       // endif

	   k++;

	 }                         //end while k
      }                             // endif

     // search transfer paths
     CoarseSearch = TRUE;
     j = 0;
     Place = MASS_TRANSFER;

     while ( j < ntp )
     {                   //scan TransferPath probabilities
	  ProbSubtotal += Simulator->GetTransferPathProbability(j);
	  if ( ProbSubtotal > RandomNo )
	  {
	       ProbSubtotal -= Simulator->GetTransferPathProbability(j);; //back up
	       CoarseSearch = FALSE;
	       break;
	  }                            // endif

	  j++;

     }                                 // endwhile j

     if (CoarseSearch == FALSE)
     {       //do fine search of transfer path
	ns = Simulator->GetNoStepsInTransferPath(j);
	k = 0;

	while ( k < ns )
	{      // scan forward reaction steps
	   ProbSubtotal += Simulator->GetStepProbFromTransferPath(j, k);
	   if (ProbSubtotal > RandomNo)
	   {
		SetSelectedEventInfo (j, k, Place);
		return;
	   }                       // endif

	   k++;

	 }	  // end while k
      }      // end if

	// there is no failure to select trap here

}             // end of function


//--------------------------------------------------------------------------


