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
//        Methods for derived class TRANSFER_PATH
//        Date last modified August 5,1998
//        Written by Frances A. Houle
//        IBM  
//---------------------------------------------------------------------------


#include "xferpath.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

transfer_path :: transfer_path ()
: process_area()

{

	UpdateReactionAfterTransfer = 0;
	UpdateTransferAfterTransfer = 0;
	pSourceCompartment = 0;
	pTargetCompartment = 0;
	SourceID = 0;
	TargetID = 0;
	ConstT = TRUE;
	ConstV = TRUE;

}

//---------------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------------

transfer_path :: ~transfer_path ()

{
     delete [] ( UpdateReactionAfterTransfer );
     delete [] ( UpdateTransferAfterTransfer );
}


//---------------------------------------------------------------------------
//         Definition of method UpdateProbabilities
//         Purpose: to update transfer path probability after an event, and then
//		manage update of reaction step probabilities in target and
//		source compartments using list if const T & V
//         Parameters: selected event data structure
//         Returns: nothing
//---------------------------------------------------------------------------

void transfer_path :: UpdateProbabilities (selected_event_info& Event)
{    // begin

	UINT16              Counter, j;
	UINT32              NoSteps;
	update_step_info*   ListPtr;
     transfer_path*      pPath;


	if ( ConstT && ConstV )
	{
		// THIS TRANSFER PATH

		// update transfer event probability for path. Also update the
		// companion step if the step is reversible.
		Step[Event.EventNo]->ProbCalc();
		if ( Step[Event.EventNo]->QueryDirection() == REVERSIBLE )
		{
			j = Step[Event.EventNo]->GetPartnerStepNo();
			Step[j]->ProbCalc();
		}

		//calculate new total probability for path
		CalcTotalProbability();


		// SOURCE AND TARGET COMPARTMENTS

		// step through update list for selected transfer step, tell
		// compartments to update themselves
		NoSteps = UpdateReactionAfterTransfer[ Event.EventNo ].GetLength();

		// if NoSteps == 0, then nothing happens, otherwise updating
		// occurs step by step

		for ( Counter = 0; Counter < NoSteps; Counter++ )
		{
		      ListPtr = UpdateReactionAfterTransfer[ Event.EventNo ].GetData( Counter );
		      ListPtr->pLocation->UpdateStepProbability( ListPtr->StepNo );
		}

		// update connected transfer paths' total probability

		if ( UpdateReactionAfterTransfer[ Event.EventNo ].CalcTotalSourceProb() )
		{
			pSourceCompartment->CalcTotalProbability();
		}

		if ( UpdateReactionAfterTransfer[ Event.EventNo ].CalcTotalTargetProb() )
		{
			pTargetCompartment->CalcTotalProbability();
		}


		// TRANSFER PATHS CONNECTED TO SOURCE AND TARGET COMPARTMENTS

		if (UpdateTransferAfterTransfer)   // only used if there are more than one path
		{

			// step through update list for selected transfer step, tell
			// transfer paths to update themselves
			NoSteps = UpdateTransferAfterTransfer[ Event.EventNo ].GetLength();

			// if NoSteps == 0, then nothing happens, otherwise updating
			// occurs step by step

			for ( Counter = 0; Counter < NoSteps; Counter++ )
			{
				ListPtr = UpdateTransferAfterTransfer[ Event.EventNo ].GetData( Counter );
				ListPtr->pLocation->UpdateStepProbability ( ListPtr->StepNo );
			}

			// update connected transfer paths' total probability
			for ( Counter = 0; Counter < UpdateTransferAfterTransfer[ Event.EventNo ].GetNoToRecalc(); Counter++ )
			{
			       pPath = UpdateTransferAfterTransfer[ Event.EventNo ].GetPathNoToRecalc( Counter );
			       pPath->CalcTotalProbability();
			}

		}  // end if update transfer after transfer


	} else {    // transfer path not const T & V

		// recalculate all probabilities for THIS TRANSFER PATH
		CalcAllProbabilities();

		// recalculate all probabilities for CONNECTED COMPARTMENTS
		pSourceCompartment->CalcAllProbabilities();
		pTargetCompartment->CalcAllProbabilities();

		// recalculate all probabilities for CONNECTED TRANSFER PATHS
		pSourceCompartment->UpdateAllConnectedTransferPathProbs( ID );
		pTargetCompartment->UpdateAllConnectedTransferPathProbs( ID );
	}


	return;

}


//---------------------------------------------------------------------------
//         Definition of method UpdateTransferredSpecies
//         Purpose: to update particle counts in connected compartments after transfer
//         Parameters:
//         Returns: nothing
//---------------------------------------------------------------------------
void transfer_path :: UpdateTransferredSpecies (selected_event_info& Event)
{

	// update particle concentrations using reaction stoichiometry

    UINT16			j;
    reaction_compartment*	pStep;
    process_info		Xfr;
    BOOL8			Add;
    BOOL8			Swap;

     Xfr = GetStepInfo( Event.EventNo );

     // first test to see whether this is straight transfer (simple diffusion, or
     // mass transfer), or virtual transfer. Straight transfer involves movement
     // of a single species, virtual transfer involves swapping two species.

     // First test for swap - flag for virtual diffusion
     // is numreactants > 1 and stoichiometry of second reactant in array == 0

     Swap = FALSE;
     j=1;
     if ( ( Xfr.numreactants > 1 ) && ( Xfr.ReactantStoich[j] == 0 ) )
     {
	Swap = TRUE;
     }

     j = 0;

     // update target compartment

     pStep = Step[ Event.EventNo ]->GetTarget();
     Add = TRUE;
     pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add); //increment
     if (Swap)
     {
	Add = FALSE;
	pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j + 1], Xfr.ReactantStoich[j], Add); //decrement
     }

     // update source compartment

     pStep = Step[ Event.EventNo ]->GetSource();
     Add = FALSE;
     pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add); //decrement
     if (Swap)
     {
	Add = TRUE;
	pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j + 1], Xfr.ReactantStoich[j], Add); //decrement
     }     return;

}    // end method



//---------------------------------------------------------------------------
//         Definition of method CalcTemperature
//         Purpose: to update average temperature at interface between
//		compartments, Calculated only once for constant T
//		simulations, updated for variable T simulations
//         Parameters: none
//         Returns: nothing
//---------------------------------------------------------------------------
void transfer_path :: CalcTemperature ()

{   //begin

	FLOAT64 t_source, t_target;

	t_source = pSourceCompartment->GetTemperature();
	t_target = pTargetCompartment->GetTemperature();
	Temperature = (t_source + t_target) / 2.0;
	return;

}  // end


//---------------------------------------------------------------------------
//
//	Definition of method SetSourceCompartmentPtr
//	Purpose: to install pointers to source compartment in transfer path and
//		in transfer steps
//	Parameters: pointer to compartment
//	Returns: nothing
//
//---------------------------------------------------------------------------

void transfer_path ::  SetSourceCompartmentPtr( reaction_compartment* p )

{
	UINT32 n;

	// set value for source compartment pointer for path
	pSourceCompartment = p;

	// set values for source for transfer steps
	// when steps are reversible source and target are reversed for companion step

	n = 0;
	while ( n < NumberOfSteps )
	{

		switch ( Step[n]->QueryDirection() )
		{
			case SOURCE_TO_TARGET:
				Step[n]->SetSource( p );
				n++;
				break;

			case REVERSIBLE:
				Step[n]->SetSource( p );
				n++;
				Step[n]->SetTarget( p );
				n++;
				break;

			case TARGET_TO_SOURCE:
				Step[n]->SetTarget( p );
				n++;
				break;

			default:
				break;

		}

	}

	return;

}

//---------------------------------------------------------------------------
//
//	Definition of method SetTargetCompartmentPtr
//	Purpose: to install pointers to target compartment in transfer path and
//		in transfer steps
//	Parameters: pointer to compartment
//	Returns: nothing
//
//---------------------------------------------------------------------------

void transfer_path ::  SetTargetCompartmentPtr( reaction_compartment* p )

{
	UINT32 n;

	// set value for source compartment pointer for path
	pTargetCompartment = p;

	// set values for target for transfer steps
	// when steps are reversible source and target are reversed for companion step

	n = 0;
	while ( n < NumberOfSteps )
	{
		switch ( Step[n]->QueryDirection() )
		{
			case SOURCE_TO_TARGET:
				Step[n]->SetTarget( p );
				n++;
				break;

			case REVERSIBLE:
				Step[n]->SetTarget( p );
				n++;
				Step[n]->SetSource( p );
				n++;
				break;

			case TARGET_TO_SOURCE:
				Step[n]->SetSource( p );
				n++;
				break;

			default:
				break;

		}	}

	return;

}
//---------------------------------------------------------------------------
//	Definition of method SetConstTFlag
//	Purpose: to signal whether both the source and target compartments
//		are const T or not. If so, then the rate constant is precalculated,
//		if not then the rate constant must be updated as T varies
//      Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void transfer_path :: SetConstTFlag()

{
	// ConstT = TRUE, set during object construction

	if ( (pSourceCompartment->QueryTOption()) != CONSTANT_TEMP )
	{
		ConstT = FALSE;
	}

	if ( (pTargetCompartment->QueryTOption() ) != CONSTANT_TEMP )
	{
		ConstT = FALSE;
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method SetConstVFlag
//	Purpose: to signal whether both the source and target compartments
//		are const V or not. If so, then a diffusion rate constant is precalculated,
//		if not then diffusion rate constants must be updated as V varies
//		Mass transfer rate constants do not depend on geometry (volume)
//      Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void transfer_path :: SetConstVFlag()

{
	enum VOLUME_OPTION Vs, Vt;

	// ConstV = TRUE, set during object construction

	if ( (pSourceCompartment->QueryVOption()) != CONSTANT_VOLUME )
	{
		ConstV = FALSE;
	}

	if ( (pTargetCompartment->QueryVOption() ) != CONSTANT_VOLUME )
	{
		ConstV = FALSE;
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitUpdateTables
//	Purpose: to allocate and set up update tables for compartments and
//		transfer paths after a transfer event
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void transfer_path :: InitUpdateTables ()

{
     UINT32 		j, Count;
     mass_transfer*	pStep;


     // only build these if transfer path is constant T&V ( and compartments are too) -
     // otherwise do a brute force update every time.

     if ( ConstT && ConstV )
     {

	// UPDATE REACTIONS AFTER A TRANSFER EVENT

	// allocate memory for list
	UpdateReactionAfterTransfer = new rxn_after_xfr_update_list[ NumberOfSteps ];
	if ( UpdateReactionAfterTransfer == 0 )
	{
		Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
	}

	// set up list for each transfer step
	for ( j=0; j<NumberOfSteps; j++ )
	{
		// get address of data for transfer step
		pStep = ( mass_transfer* )Step[j];

		// build list
	       UpdateReactionAfterTransfer[j].Build( pSourceCompartment, pTargetCompartment, pStep );
	}


	// UPDATE TRANSFER PATHS AFTER A TRANSFER EVENT
	// implement only if there are more than one transfer path
	if ( Simulator->GetNoOfTransferPaths() > 1 )
	{

		// assumes that whether paths are const T & V or not, their characteristics will be unaffected by
		// an event in a const T&V compartment, since the compartment on the other side will not change.
		// only exception to this is programmed T ( *** not yet implemented *** ) - therefore only single
		// steps will be updated

		UpdateTransferAfterTransfer = new xfr_after_xfr_update_list[ NumberOfSteps ];
		if ( UpdateTransferAfterTransfer == 0 )
		{
			Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			return;
		}

		// set up list for each transfer step
		for ( j=0; j<NumberOfSteps; j++ )
		{
			// get address of data for transfer step
			pStep = (mass_transfer*)Step[j];

			// build list
	       UpdateTransferAfterTransfer[j].Build( pSourceCompartment, pTargetCompartment, pStep, ID );
		}

	}  // end if

     }  // end if const T and V

      return;

}


//---------------------------------------------------------------------------

