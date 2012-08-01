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
// X_X_UDL.CXX
// Methods for derived class xfr_after_xfr_update_list
// Date last modified: March 8, 1996
// Written by Frances Houle
// IBM  
//
//--------------------------------------------------------------------------

#include "x_x_udl.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//
//	Constructor
//
//--------------------------------------------------------------------------

xfr_after_xfr_update_list :: xfr_after_xfr_update_list ()

{
	RecalcTotalProbList = 0;
	NoToRecalc = 0;

}


//--------------------------------------------------------------------------
//
//	Destructor
//
//--------------------------------------------------------------------------

xfr_after_xfr_update_list :: ~xfr_after_xfr_update_list ()

{
	UINT32 k;
	update_step_info* DataPtr;

	for ( k=0; k<Length; k++ )
	{
		DataPtr = StepList.GetObject(k);
		delete DataPtr;
	}

	delete [] RecalcTotalProbList;


}


//--------------------------------------------------------------------------
// Definition of method Build
// Purpose: to create list of transfer paths to update after a transfer step
//   	in a specific transfer path.
// Parameters: none
// Returns: nothing
//--------------------------------------------------------------------------

void xfr_after_xfr_update_list :: Build ( reaction_compartment* SourceCmpt, reaction_compartment* TargetCmpt,
               mass_transfer* TransferStep, UINT16 TransferPathNo )

{

     BOOL8                    FoundMatch, PathNoAddedToList;
     reaction_compartment*    CompartmentNo[2];
     transfer_path*	      pConnectedPath;
     UINT16		      j, NoPaths[2], ConnectedPathNo, k, ListCounter;
     UINT32                   ComparisonStepNo, nts, n1, n2;
     process_info             StepData, ComparisonStepData;


     // initialize flags
     FoundMatch = FALSE;
     PathNoAddedToList = FALSE;

     // Get data for transfer step
     StepData = TransferStep->GetProcessInfo();


     // put source and target compartment numbers into temp array for looping
     CompartmentNo[0] = SourceCmpt;
     CompartmentNo[1] = TargetCmpt;

     // get number of transfer paths connected to the compartments and save
     NoPaths[0] = SourceCmpt->GetNoTransferPaths();
     NoPaths[1] = TargetCmpt->GetNoTransferPaths();

     // allocate memory to store path numbers that will need to have total probabilities updated
     // calculate # paths, subtract 2 because current path is not counted
     j = NoPaths[0] + NoPaths[1] - 2;
     RecalcTotalProbList = new transfer_path *[ j ];

     if ( RecalcTotalProbList == 0 )
     {
	Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
	return;
     }

     // initialize counter for stepping through recalc list
     ListCounter = 0;

     // transfer path steps can have either connected compartment as source and target. In this
     // case it does not matter since we are only looking for a change in concentration that will
     // affect a reaction step probability -> no test for source & target.

     for ( j=0; j<2; j++ )
     {

	  // Set up list for updating transfer step probabilities after SelectEvent returns a
	  // transfer step. Go through each of the transfer paths, and compare transfer steps to
	  // the transferred species. Use for all paths, whether const or variable T & V since an
	  // event in a const T&V compartment will not affect transfer T&V values
	  // this build method is to be called from within the transfer path during its initialization

	  // scan through connected paths
	  for ( k = 0; k < NoPaths[j]; k++ )
	  {

	     pConnectedPath = CompartmentNo[j]->GetConnectedPathPtr( k );

	     if ( (pConnectedPath->GetID()) != TransferPathNo ) // don't do self
	     {
		// compare to connected transfer path steps
		nts = pConnectedPath->GetNoSteps();

		for (ComparisonStepNo=0; ComparisonStepNo<nts; ComparisonStepNo++)
		{

			 // get process info for transfer step
	       ComparisonStepData = pConnectedPath->GetStepInfo( ComparisonStepNo );

			// flag step for update whether current compartment is source or not - applies to all types of
			// transfer steps (mass transfer and diffusion)

			// search for matches
			// n1 = index for TransferStepNo's stoichiometry
			// n2 = index for ReactionStepNo's stoichiometry

			// compare species ID of transfer step to reactants in transfer step. by convention the
			// first species is the transferred species - the only one whose concentration will change

			n1 = 0;
			for ( n2 = 0; n2 < ComparisonStepData.numreactants; n2++ )
			{
				FoundMatch = FALSE;
				if ( ( StepData.ReactantsInStep[n1] ) == ( ComparisonStepData.ReactantsInStep[n2] ) )
				{
					FoundMatch = TRUE;
					// allocate and store at end of update list
					StepList.Insert (new update_step_info ( pConnectedPath, ComparisonStepNo ), LIST_APPEND );
					// set flag to calculate total prob
					RecalcTotalProbList[ ListCounter ] = pConnectedPath;
					PathNoAddedToList = TRUE;

				 }   // end if
				 if ( FoundMatch )
				      { break; }
			}    // endfor n2

		  }     // end for ComparisonStepNo

	     }  // end if ConnectedPathNo

	     if ( PathNoAddedToList )
	     {
		ListCounter++;
		PathNoAddedToList = FALSE;
	     }

	  }  // end for k (counts paths connected to each compartment)

     }   // end for j (looks at source then target compartment)

     // store length of StepList
     Length = StepList.Count();
     // store length of RecalcTotalProbList
     NoToRecalc = ListCounter;

     return;

}     // end method


//--------------------------------------------------------------------------
