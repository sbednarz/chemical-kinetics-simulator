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
// Date last modified: December 16, 1996
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
	pConnectedTransferPath = 0;
	pAdjacentArea = 0;
     NumberOfInterfaces = 0;
     pInterfacedCompartment = 0;
	ReactionUpdateTable = 0;
	UpdateTransferAfterReaction = 0;

	T_option = CONSTANT_TEMP;
	V_option = CONSTANT_VOLUME;
	P_option = CONSTANT_PRESSURE;
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
     delete [] ( pConnectedTransferPath );
     delete [] ( pAdjacentArea );
     delete [] ( pInterfacedCompartment );
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
//
//	Definition of method UpdateConcentrations
//	Purpose: to renormalize units of rate constants when colume changes
//	Parameters; volume ratio
//	Returns: nothing
//
//---------------------------------------------------------------------------

void reaction_compartment :: UpdateConcentrations( FLOAT64 SizeRatio )

{
	UINT32 	j;

	// update reaction rate constants
	for ( j=0; j<NumberOfSteps; j++ )
	{
		Step[j]->UpdateRateCoefficients( SizeRatio );
	}

	// update particles - mol/l conversion factor
	MolarConversionFactor *= SizeRatio;

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
			(pConnectedTransferPath[ j ])->CalcAllProbabilities();
		}

	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitSpecies
//	Purpose: to set initial number of particles according to the initial
//		moles for each species
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_compartment :: InitSpecies()

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
		ChemicalSpecies[j].SetInitMoles( CmptGeometry.Volume );
		TotalMoles += ChemicalSpecies[j].GetInitMoles();
	}

	return TotalMoles;

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
						NumberOfTransferPaths, pConnectedTransferPath );
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
     rBOS << rObject.CmptGeometry.Volume;

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

