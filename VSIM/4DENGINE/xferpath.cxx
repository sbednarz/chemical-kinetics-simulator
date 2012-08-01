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
//        Date last modified December 17, 1996
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
	ConstP = TRUE;
	ConstV = TRUE;
	AvgPressure = 0;

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

