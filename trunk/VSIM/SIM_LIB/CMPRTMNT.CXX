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
// methods for class reaction_compartment
// Date last modified: August 5, 1998
// Written by Frances Houle
// IBM  
//---------------------------------------------------------------------------


#include "cmprtmnt.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

reaction_compartment :: reaction_compartment ()
: process_area()

{

	ChemicalSpecies = 0;
	NumberOfChemicalSpecies = 0;
	MolarConversionFactor = 0.0;

	NumberOfTransferPaths = 0;
	TransferPathID = 0;
	ConnectedTransferPath = 0;
	AdjacentArea = 0;
	ReactionUpdateTable = 0;
	UpdateTransferAfterReaction = 0;

	T_option = CONSTANT_TEMP;
	V_option = CONSTANT_VOLUME;
	P_option = CONSTANT_PRESSURE;
	Volume = 0.0;
	Pressure = 0.0;
	Phase = NO_PHASE;

}

//---------------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------------

reaction_compartment :: ~reaction_compartment ()

{
     delete [] ( ChemicalSpecies );
     delete [] ( TransferPathID );
     delete [] ( ConnectedTransferPath );
     delete [] ( AdjacentArea );
     delete ( ReactionUpdateTable );
     delete [] ( UpdateTransferAfterReaction );

}


//---------------------------------------------------------------------------
//         Definition of method UpdateChemicalSpecies
//         Purpose: to update reactant and product particles after a reaction
//         Parameters: event information
//         Returns: nothing
//---------------------------------------------------------------------------
void reaction_compartment :: UpdateChemicalSpecies (selected_event_info& event)
{   // begin

	// update particle concentrations using reaction stoichiometry

    UINT16 j, n, SpeciesNumber;
    process_info Rxn;

     n = event.EventNo;
     Rxn = GetStepInfo( n );

     for ( j = 0; j < Rxn.numreactants; j++ )
     {
	  SpeciesNumber = Rxn.ReactantsInStep[j];
	  ChemicalSpecies[SpeciesNumber].DecrementParticles((ParticleType) Rxn.ReactantStoich[j]);

     }

     for ( j = 0; j < Rxn.numproducts; j++ )
     {
	  SpeciesNumber = Rxn.ProductsInStep[j];
	  ChemicalSpecies[SpeciesNumber].IncrementParticles((ParticleType) Rxn.ProductStoich[j]);
     }
     return;

}    // end method

//---------------------------------------------------------------------------
//         Definition of method UpdateChemicalSpecies
//         Purpose: to update particles after transfer
//         Parameters: species info, stoichiometry,flag to signal
//		increment or decrement)
//         Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: UpdateChemicalSpecies (UINT16 SpeciesNumber, UINT16 Stoichiometry, BOOL8 Add )
{   // begin


    if ( Add ) // increment target compartment
    {
	  ChemicalSpecies[SpeciesNumber].IncrementParticles((ParticleType) Stoichiometry);

    } else {    // decrement source compartment

	  ChemicalSpecies[SpeciesNumber].DecrementParticles((ParticleType) Stoichiometry);

    }
     return;

}    // end method


//---------------------------------------------------------------------------
//         Definition of method UpdateProbabilities
//         Purpose: to update reaction step probabilities after a reaction using either the
//              update tables if the simulation is constant T, V, or scanning through
//              the entire list if the simulation has changing state variables, and
//              manage updating of connected transfer paths if reaction occurrence causes
//     	        transfer step probabilities to change
//         Parameters: selected event data structure
//         Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: UpdateProbabilities (selected_event_info& Event)
{    // begin


	UINT16              nr, Counter, NoPaths;
	UINT32              RowLength, NoSteps;
	update_step_info*   ListPtr;
	transfer_path*      pPath;


	if ( T_option == CONSTANT_TEMP && V_option == CONSTANT_VOLUME )

	{
		// COMPARTMENT
		// update selected reaction steps using ReactionUpdateTable

		RowLength = ReactionUpdateTable -> GetRowLength(Event.EventNo);

		for (Counter = 0; Counter < RowLength ; Counter++ )
		{
		   nr = ReactionUpdateTable->GetStepNumber( Event.EventNo, Counter );
		   UpdateStepProbability( nr );

		} // endfor

		// update compartment's total probability
		CalcTotalProbability();

		// TRANSFER PATHS
		if ( NumberOfTransferPaths )
		{
			// step through update list for selected reaction, tell
			// connected transfer paths to update themselves.
			NoSteps = UpdateTransferAfterReaction[ Event.EventNo ].GetLength();

			// if NoSteps == 0, then nothing happens, otherwise updating
			// occurs step by step

			for ( Counter = 0; Counter < NoSteps; Counter++ )
			{
			      ListPtr = UpdateTransferAfterReaction[ Event.EventNo ].GetData( Counter );
  		           ListPtr->pLocation->UpdateStepProbability ( ListPtr->StepNo );
			}

		       // update connected transfer paths' total probability if single steps were recalculated
			NoPaths = UpdateTransferAfterReaction[ Event.EventNo ].GetNoPaths();
		       for (Counter = 0; Counter < NoPaths; Counter++ )
		       {
			    pPath = UpdateTransferAfterReaction[ Event.EventNo ].GetPathNumber( Counter );
			    pPath->CalcTotalProbability();
		       }

		}  // end if NumberOfTransferPaths

	} else {   // not constant volume and temperature

		// update all reaction step probabilities
		CalcAllProbabilities();
		// tell all connected transfer paths to update all their probabilities
		for (Counter = 0; Counter < NumberOfTransferPaths; Counter++ )
		{
			(ConnectedTransferPath[ Counter ])->CalcAllProbabilities();
		}

	}   // end if constant volume and temperature

	return;

}      // end method


//---------------------------------------------------------------------------
//
//	Definition of method UpdateConcentrations
//	Purpose: to renormalize units of rate constants when colume changes
//	Parameters; volume ratio
//	Returns: nothing
//
//---------------------------------------------------------------------------

void reaction_compartment :: UpdateConcentrations( FLOAT64 VolumeRatio )

{
	UINT32 	j;

	// update reaction rate constants
	for ( j=0; j<NumberOfSteps; j++ )
	{
		Step[j]->UpdateRateCoefficients( VolumeRatio );
	}

	// update particles - mol/l conversion factor
	MolarConversionFactor *= VolumeRatio;

	return;

}



//---------------------------------------------------------------------------
//	Definition of method UpdateAllConnectedTransferPathProbs
//	Purpose: to cause all connected transfer path probs to
//		be fully updated except one. This method is invoked during
//		update of transfer path number TransferPathNo, which has
//		already been recalculated.
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: UpdateAllConnectedTransferPathProbs ( UINT16 TransferPathNo )

{
	UINT16 j;

	for ( j = 0; j < NumberOfTransferPaths; j++ )
	{
		if ( TransferPathID[ j ] != TransferPathNo )
		{
			(ConnectedTransferPath[ j ])->CalcAllProbabilities();
		}

	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitSpeciesCount
//	Purpose: to set initial number of particles according to the initial
//		moles for each species
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: InitSpeciesCount ()

{
	UINT16 j;

     // length of chem species array is nospecies+1 = 0th element unused
	for ( j=1; j <= NumberOfChemicalSpecies; j++ )
	{
		ChemicalSpecies[j].InitParticles();
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method CalcInitialNoOfMoles
//	Purpose: to calculate the total concentrations of species initially
//		present to determine the conversion factor for particle units
//	Parameters: none
//	Returns: value of total in moles/liter units
//---------------------------------------------------------------------------

FLOAT64 reaction_compartment :: CalcInitialNoOfMoles ()

{
	UINT16 j;
	FLOAT64 TotalMoles;

	// initial amounts of material are read in as concentrations in moles/liter.
	// first tell reactants to convert from conc to moles by multiplying by volume  -
	// internally we work in particles proportional to moles - then calculate total
	// initial moles

	TotalMoles = 0.0;

	for ( j=1; j <= NumberOfChemicalSpecies; j++ )
	{
		ChemicalSpecies[j].SetInitMoles( Volume );
		TotalMoles += ChemicalSpecies[j].GetInitMoles();
	}

	return TotalMoles;

}



//---------------------------------------------------------------------------
//	Definition of method InitPressure
//	Purpose: to calculate the initial gas pressure for a gas phase
//		system when the pressure is variable. Volume must be
//		initialized first!!
//	Parameters: no of chemical species
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: InitPressure ()

{
	FLOAT64 InitMoles;

	switch ( P_option ) {

	case CONSTANT_PRESSURE:
		break;

	case VARIABLE_PRESSURE:
		// for gas phase systems only. An explicit check for initial phase needs to be
		// put in when the variable volume option is fully implemented
		InitMoles = CalcInitialNoOfMoles();
		Pressure = ( InitMoles * GAS_CONSTANT * Temperature ) / Volume;
		break;

	default:
		break;

	}

	return;

}


//---------------------------------------------------------------------------
//
//	Definition of method InitVolume
//	Purpose: to initialize variables needed to update volume when variable
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void reaction_compartment :: InitVolume ()

{
	UINT32 j;

	switch (V_option) {

	case CONSTANT_VOLUME:
		break;

	case VARIABLE_VOLUME:
		for ( j=1; j <= NumberOfChemicalSpecies; j++ )
		{

			ChemicalSpecies[ j ].CalcVolPerParticle( j );

		}
		break;

	default:
		break;

	}

	// convert system value for ParticlesPerMol to molar units using volume of
	// compartment - this factor is used to convert rate constants to
	// particle units, to update conversion for variable volume
	// simulations and to convert particles to concentrations when simulation
	// status is saved

	MolarConversionFactor = (Simulator->GetParticlesPerMol()) * Volume;


	return;

}


//---------------------------------------------------------------------------
//	Definition of method Connect
//	Purpose: to retrieve connected compartment indices from transfer
//		paths
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: Connect()

{
	UINT16 j;

	for ( j=0; j<NumberOfTransferPaths; j++ )
	{

		AdjacentArea[j] = ( ConnectedTransferPath[j] )->GetSourceCompartmentPtr();

		// check to see if source no is the same number as this compartment,
		// if it is use target
		if ( (AdjacentArea[j])->GetID() == ID )
		{
			AdjacentArea[j] = ( ConnectedTransferPath[j] )->GetTargetCompartmentPtr();
		}

	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitUpdateTables
//	Purpose: to allocate and set up update tables for const T simulations
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: InitUpdateTables ()

{
	UINT32          j;
     process*        pStep;


	if ( (T_option == CONSTANT_TEMP) && (V_option == CONSTANT_VOLUME) )
	{

		// set up table and lists for updating reactions if there are any
		if ( NumberOfSteps )
		{
			// set up table for updating reaction probabilities after a reaction occurs
			// in a compartment
			ReactionUpdateTable = new reaction_update_table ( NumberOfSteps );
			if ( ReactionUpdateTable == 0 )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return;
			}
			ReactionUpdateTable->BuildTable ( this );

			// set up array of data lists for updating transfer paths after a reaction
			// if there are any transfer paths

			if ( NumberOfTransferPaths )
			{
				UpdateTransferAfterReaction = new xfr_after_rxn_update_list[ NumberOfSteps ];
				if ( UpdateTransferAfterReaction == 0 )
				{
					Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
					return;
				}

				for ( j=0; j<NumberOfSteps; j++ )
				{
					// get address of data for transfer step
					pStep = Step[j];

					// build list
					UpdateTransferAfterReaction[j].Build ( (reaction_step*) pStep, this,
						NumberOfTransferPaths, ConnectedTransferPath );
				}

			}  // end if NumberOfTransferPaths

		} // end if NumberOfReactions

	} // end if constant T and V

	return;

}


//---------------------------------------------------------------------------
 //       Definitions of binary output stream operator <<
 //       Purpose: to record state of compartment
 //       Parameters: pointers to file and to compartment
 //       returns: pointer
 //---------------------------------------------------------------------------

 BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, reaction_compartment& rObject )

 {
     UINT32 j, no_reactants;

     // write data identifier
     rBOS << IDType( COMPARTMENT_STATE );

     // write state data
     rBOS << rObject.Temperature;
     rBOS << rObject.Pressure;
     rBOS << rObject.Volume;

     // write particle counts. Species numbers range from 1 to no_reactants
     no_reactants = Simulator->GetNoOfChemicalSpecies();

     for ( j=1; j <= no_reactants; j++ )
     {
	//NOTE!!!! the following line is in particle units. This must be changed
	// to read
	//rBOS << ( *( rObject.ChemicalSpecies + j ) / MolarConversionFactor );

	  rBOS << *( rObject.ChemicalSpecies + j );
     }

     return rBOS;

}

//---------------------------------------------------------------------------

