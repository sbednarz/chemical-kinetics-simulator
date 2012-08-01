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
// X_R_UDL.CXX
// Methods for derived class transfer_update_table_transferpath
// Date last modified: January 14, 1997
// Written by Frances Houle
// IBM  
//
//--------------------------------------------------------------------------

#include "x_r_udl.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//
//   Constructor
//
//--------------------------------------------------------------------------

xfr_after_rxn_update_list :: xfr_after_rxn_update_list ()

{
     CalcTotalPathProbList = 0;
     NoPaths = 0;
}


//--------------------------------------------------------------------------
//
//	Destructor
//
//--------------------------------------------------------------------------

xfr_after_rxn_update_list :: ~xfr_after_rxn_update_list ()

{
	UINT32              k;
	update_step_info*   DataPtr;

	for ( k=0; k<Length; k++ )
	{
		DataPtr = StepList.GetObject(k);
		delete DataPtr;

	}

     delete [] CalcTotalPathProbList;

}


//--------------------------------------------------------------------------
// Definition of method Build
// Purpose: to create list of transfer steps to update after occurrence of a
//        particular reaction for each connected transfer path. Do for each
//        reaction step
// Parameters: none
// Returns: nothing
// --------------------------------------------------------------------------

void xfr_after_rxn_update_list :: Build (reaction_step* RxnStep, reaction_compartment* CurrentCompartment,
			UINT16 NoTransferPaths, transfer_path** ConnectedTransferPath)
{

     BOOL8          	     Proceed, FoundMatch, PathNoAddedToList;
     UINT16         	     np, ListCounter;
     UINT32         	     ComparisonStepNo, nrs, nts, n1, n2;
     process_info   	     StepData, ComparisonStepData;
     reaction_compartment*   SourceCompartmentForStep;
     transfer_path*          pConnectedPath;
     enum REC_TYPE  	     AreaType;


  // initialize flags
  FoundMatch = FALSE;
  PathNoAddedToList = FALSE;

  // get compartment type - build details depend on this
  AreaType = CurrentCompartment->GetProcessAreaType();

  // get data for reaction step to be compared to transfer steps
     StepData = RxnStep->GetProcessInfo();


  // transfer paths for which matches are found will need to have total probabilities
  // recalculated (this happens automatically if all steps are updated).  Allocate
  // array to store these

  CalcTotalPathProbList = new transfer_path *[ NoTransferPaths ];
  if ( CalcTotalPathProbList == 0 )
  {
	Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
  }

  // initialize list counter - keeps track of index of CalcTotalProbList
  ListCounter = 0;

  // Set up list for updating transfer step probabilities after SelectEvent returns a
  // reaction step. Go through each of the transfer paths, and compare transfer steps to
  // the reactants and products. Use for all paths, whether const or variable T & V since an
  // event in a const T&V compartment will not affect transfer T&V values
  // this build method is to be called from within the compartment during its initialization
  // go through loop for each connected transfer path
  for ( np = 0; np < NoTransferPaths; np++ )
  {
	// set flag that labels transfer path steps for examination - set following test
	// for source compartment match.
	Proceed = FALSE;

	// Get id number of connected path from path list
	pConnectedPath = *( ConnectedTransferPath + np );

     // Set up list for updating transfer step probabilities after SelectEvent returns a
	// reaction step. Go through each of the mass transfer steps in a transfer path and
	// compare species in the rate law to the reactants required for the reaction step in the compartment
	// this build method is to be called from within the compartment during its initialization
	nts = pConnectedPath->GetNoSteps();

	  // compare to transfer steps
	     for (ComparisonStepNo=0; ComparisonStepNo<nts; ComparisonStepNo++)      // step through transfer steps
	     {

		switch ( AreaType ) {

		case COMPARTMENT_TYPE_CONTAINER:

	       // test to see if Source compartment no. for step is the same as the overall source compartment for
	       // the transfer step -- if it is not then move on to next step. This is needed because transfer steps
	       // can be reversible
	       SourceCompartmentForStep = pConnectedPath->GetStepSource( ComparisonStepNo );

	       if ( SourceCompartmentForStep == CurrentCompartment )
	       {
			Proceed = TRUE;

	       }
	       break;

		case COMPARTMENT_TYPE_3D:
		// set flag to proceed in all cases - concentration gradient requires that both directions of a
		// transfer step be updated.

		Proceed = TRUE;
		break;

		default:
		break;

		}  //end switch



	       if ( Proceed )
	       {
		       // get process info for transfer step
		       ComparisonStepData = pConnectedPath->GetStepInfo( ComparisonStepNo );

		       // search for matches
		       // n1 = index for ReactionStepNo's reactant stoichiometry
		       // n2 = index for TransferStepNo's stoichiometry

		       // first do for reactants in reaction step

		       for ( n1 = 0; n1 < StepData.numreactants; n1++ )
		       {

			    for ( n2 = 0; n2 < ComparisonStepData.numreactants; n2++ )
			    {
				 FoundMatch = FALSE;
				 if ( ( StepData.ReactantsInStep[n1] ) == ( ComparisonStepData.ReactantsInStep[n2] ) )
				 {
				      FoundMatch = TRUE;

				      // allocate and store at end of update list
				      StepList.Insert (new update_step_info ( pConnectedPath, ComparisonStepNo ), LIST_APPEND);

				      // store TransferPathNo in CalcTotalProb list
				      CalcTotalPathProbList[ ListCounter ] = pConnectedPath;
				      PathNoAddedToList = TRUE;

				 }             // end if
				 if ( FoundMatch )
				      { break; }
			    }                  // endfor n2
			    if ( FoundMatch )
				 { break; }
		       }                       // endfor n1


		  // do for products in reaction step if no match yet
		  if ( !FoundMatch )
		  {
		    for ( n1 = 0; n1 < ( StepData.numproducts ); n1++ )
		    {
			 for ( n2 = 0; n2 < ( ComparisonStepData.numreactants ); n2++ )
			 {
			      FoundMatch = FALSE;
			      if ( ( StepData.ProductsInStep[n1] ) == ( ComparisonStepData.ReactantsInStep[n2] ) )
			      {
				   FoundMatch = TRUE;
				   //allocate and store in update list
				   StepList.Insert (new update_step_info (pConnectedPath, ComparisonStepNo), LIST_APPEND );
				   CalcTotalPathProbList[ ListCounter ] = pConnectedPath;
				   PathNoAddedToList = TRUE;
			      }
			      if ( FoundMatch )
				   break;
			 }             // end for n2
			 if ( FoundMatch )
			      break;
		    }                  // endfor n1
		  }  // end if not Foundmatch

		}    // end if source compartment numbers match

	}     // end for ComparisonStepNo

	if ( PathNoAddedToList )
	{
		ListCounter++;
		PathNoAddedToList = FALSE;
	}

 }  // end for np

  // store length of StepList
  Length = StepList.Count();
  // store length of CalcTotalPathProbList
  NoPaths = ListCounter;

  return;
}     // end method


//--------------------------------------------------------------------------
