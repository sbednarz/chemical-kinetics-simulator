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
// R_X_UDL.CXX
// Methods for derived class rxn_after_xfr_update_list
// Date last modified: July 17, 1998
// Written by Frances Houle
// IBM  
//
//--------------------------------------------------------------------------

#include "r_x_udl.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//
//	Constructor
//
//--------------------------------------------------------------------------

rxn_after_xfr_update_list :: rxn_after_xfr_update_list ()
{
	CalcSourceProb = FALSE;
	CalcTargetProb = FALSE;
}


//--------------------------------------------------------------------------
//
//	Destructor
//
//--------------------------------------------------------------------------

rxn_after_xfr_update_list :: ~rxn_after_xfr_update_list ()

{
	UINT32 k;
	update_step_info* DataPtr;

	for ( k=0; k<Length; k++ )
	{
		DataPtr = StepList.GetObject(k);
		delete DataPtr;
	}


}


//--------------------------------------------------------------------------
// Definition of method Build
// Purpose: to create list of reaction steps to update after a transfer step
//   	in a specific transfer path.
// Parameters: none
// Returns: nothing
//--------------------------------------------------------------------------

void rxn_after_xfr_update_list :: Build (reaction_compartment* SourceCmpt, reaction_compartment* TargetCmpt, mass_transfer* TransferStep )

{

     BOOL8                    FoundMatch;
     UINT16		          j;
     reaction_compartment*    CompartmentNo[2];
     UINT32                   ReactionStepNo, ComparisonStepNo, nrs, n1, n2;
     process_info             StepData, ComparisonStepData;


     // initialize flag
     FoundMatch = FALSE;

     // get data for transfer step
     StepData = TransferStep->GetProcessInfo();

     // put source and target compartment numbers into temp array for looping
     CompartmentNo[0] = SourceCmpt;
     CompartmentNo[1] = TargetCmpt;

     // transfer path steps can have either connected compartment as source and target. In this
     // case it does not matter since we are only looking for a change in concentration that will
     // affect a reaction step probability -> no test for source & target.

     for ( j=0; j<2; j++ )
     {

	  // Set up list for updating reaction step probabilities after SelectEvent returns a
	  // transfer step. Go through each of the reaction steps in the compartment and
	  // compare species in the rate law to the transferred species
	  // this build method is to be called from within the transfer path during its initialization

	  nrs = CompartmentNo[j]->GetNoSteps();

	  // compare to reaction steps
	  for (ComparisonStepNo=0; ComparisonStepNo<nrs; ComparisonStepNo++)
	  {

	       // get process info for reaction step
	       ComparisonStepData = CompartmentNo[j]->GetStepInfo( ComparisonStepNo );

	       // search for matches
	       // n1 = index for TransferStepNo's stoichiometry
	       // n2 = index for ReactionStepNo's stoichiometry

	       // compare species ID of transfer step to reactants in reaction step. by convention the
	       // first species is the transferred species,

	       // SPECIAL NOTE FOR DIFFUSION STEPS
	       // the second species may also be transferred
	       // in the opposite direction if the step is a virtual diffusion step. The flag for this
	       // is that the stoichiometry of the second species is = 0. If the stoichiometry of a
	       // second, third, fourth etc species is = 1. then they are non-Fickian participants in the
	       // diffusion, and their concentration does not change during the transfer process


	       n1 = 0;    // transferred species
	       for ( n2 = 0; n2 < ComparisonStepData.numreactants; n2++ )
	       {
		   FoundMatch = FALSE;
		   if ( ( StepData.ReactantsInStep[n1] ) == ( ComparisonStepData.ReactantsInStep[n2] ) )
		   {
			      FoundMatch = TRUE;
			      // allocate and store at end of update list
			      StepList.Insert (new update_step_info ( CompartmentNo[j], ComparisonStepNo ), LIST_APPEND );
			      // set flag to calculate total prob
			      if ( j )  // current compartment is target
			      {
					CalcTargetProb = TRUE;
			      } else {
					CalcSourceProb = TRUE;
			      }  // endif j

		   }             // end if StepData

		   if ( FoundMatch )
			       { break; }

	       }          // endfor n2

	       n1 = 1;  // only used if this species is trasnsferred also as part of a virtual diffusion step
	       if ( StepData.ReactantStoich[n1] == 0 )	       {
		   for ( n2 = 0; n2 < ComparisonStepData.numreactants; n2++ )
		   {
			   FoundMatch = FALSE;
			   if ( ( StepData.ReactantsInStep[n1] ) == ( ComparisonStepData.ReactantsInStep[n2] ) )
			   {
			      FoundMatch = TRUE;
			      // allocate and store at end of update list
			      StepList.Insert (new update_step_info ( CompartmentNo[j], ComparisonStepNo ), LIST_APPEND );
			      // set flag to calculate total prob
			      if ( j )  // current compartment is target
			      {
					CalcTargetProb = TRUE;
			      } else {
					CalcSourceProb = TRUE;
			      }  // endif j


			   }             // end if StepData

			 if ( FoundMatch )
			       { break; }

		   }          // endfor n2
		}		   // endif n1

	  }     // end for ComparisonStepNo

     }   // end for j

     // store length of StepList
     Length = StepList.Count();

     return;

}     // end method


//--------------------------------------------------------------------------
