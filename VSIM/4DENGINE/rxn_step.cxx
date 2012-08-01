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
//      Methods for class REACTION_STEP
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "rxn_step.hxx"
#include "simulatr.hxx"

//---------------------------------------------------------------------------
//        Constructor
//---------------------------------------------------------------------------

reaction_step :: reaction_step ()
: process ()

{

}

//---------------------------------------------------------------------------
//    Definition of method ProbCalc for reactions
//     Purpose: this is a general function for calculating a rate from
//     rate law parameters. It assigns the new value for the probability.
//    Returns: nothing
//---------------------------------------------------------------------------

void reaction_step :: ProbCalc()

{                                      // begin function
     FLOAT64 		q, np, T;
     FLOAT64 		Order;
     UINT16 		ReactantCounter;
     UINT16 		SpeciesNumber;
     BOOL8		EnoughReactants;
     ParticleType 	nparticles;

     ReactantCounter = 0;
     EnoughReactants = TRUE;

     while ( ReactantCounter < ProcessData.numreactants )
     {
	  SpeciesNumber = ProcessData.ReactantsInStep[ReactantCounter];
	  nparticles = ((reaction_compartment *)pProcessArea)->GetSpeciesCount( SpeciesNumber );
	  if ( nparticles < ( ParticleType )( ProcessData.ReactantStoich[ReactantCounter] ) )
	  {
	       EnoughReactants = FALSE;
	       break;
	  }                            // end if
	  ReactantCounter++;
     }                                 // endwhile ReactantCounter

     if ( EnoughReactants )
     {
	  T = pProcessArea->GetTemperature(); //temperature
	  q = GetRateConstant( T );

	     ReactantCounter = 0;

	     while ( ReactantCounter < ProcessData.numSpeciesInRateLaw )
	     {
		SpeciesNumber = ProcessData.SpeciesInRateLaw[ReactantCounter];
		np = ( FLOAT64 ) (((reaction_compartment *)pProcessArea)->GetSpeciesCount(SpeciesNumber));
		Order = ProcessData.ReactantOrder[ReactantCounter];

		// check values of order to avoid unnecessary floating point calcs
		// skip if zero order
		  if (Order == 1.0)
		     {
			  q *= np;
		     }

		  if ((Order != 0.0) && (Order != 1.0))
		     {
			  q *= ( pow( np, ProcessData.ReactantOrder[ReactantCounter] ) );
		     }

		  ReactantCounter++;

     }                                 // endwhile ReactantCounter

	     // set new value for probability
	     Probability = q;

     } else {  // not enough reactants

	     ZeroProbability();

     }   // end if EnoughReactants

     return;

}                                      // end function


//-----------------------------------------------------------------------------
//	Definition of input operator >>
//	Purpose: to read in and set up reaction step data
//	Parameters: file name and object pointer
//	Returns: file pointer
//-----------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, reaction_step& rObject )

{
	UINT16 				temp, j, counter;
	FLOAT64 			Order;
	enum RATE_CONST_FORMAT	kFormat;

	while ( DataCode != IDType( END_RXN_INIT ) )

	{

		rTIS >> DataCode;

		switch (DataCode) {

		case RXN_DATA:
			// read enum into temporary variable, cast to type
			rTIS >> temp;
			rObject.ProcessData.Direction = ( enum XFER_DIRECTION )temp;
			break;

		case RXN_RATE_CONSTANT_INIT:
			// allocate memory for rate constant
			rObject.RateConstant = new k_info;
			if ( rObject.RateConstant == 0 )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
			}

			// read in next line - tells type - and store in k_info
			rTIS >> DataCode;
			ASSERT ( DataCode == IDType( RXN_RATE_CONSTANT_TYPE ) );

			rTIS >> temp;
			kFormat = ( enum RATE_CONST_FORMAT )temp;
			rObject.RateConstant->SetFormat( kFormat );

			// store pointer to this step
			rObject.RateConstant->SetStepPointer( &rObject );
			switch ( kFormat ) {

				case TEMP_INDEPENDENT:
				case TEMP_DEPENDENT:
					// read in data for generic reaction/mass transfer rate constant
					rTIS >> *( (reaction_transfer_k*) rObject.RateConstant );
					break;

				case PROGRAMMED:
					// read in data for programmed rate constant
					rTIS >> *( (programmed_k*) rObject.RateConstant );
					break;

				default:
					break;

			}
			break;

		case RXN_FWD_RATE_LAW:
			rTIS >> rObject.ProcessData.numreactants;

			// start counter for number of species in rate law
			counter = 0;

			for (j = 0; j < rObject.ProcessData.numreactants; j++)
			{
				rTIS >> DataCode;
				ASSERT (DataCode == RXN_SPECIES_DATA);
				rTIS >> rObject.ProcessData.ReactantsInStep[j];
				rTIS >> rObject.ProcessData.ReactantStoich[j];
				rTIS >> Order;
				if ( Order != 0.0 )
				{   // set up kinetics
					rObject.ProcessData.SpeciesInRateLaw[counter] = rObject.ProcessData.ReactantsInStep[j];
					rObject.ProcessData.ReactantOrder[counter] = Order;
					counter++;
				}
			}  // end for

			rObject.ProcessData.numSpeciesInRateLaw = counter;

	  // if the step is REVERSIBLE

			if (rObject.ProcessData.Direction == REVERSIBLE)
			{
			  // allocate temporary storage for reverse step data
				rObject.InitTempStepData();
		       rObject.TemporaryStep->Direction = REVERSIBLE;

		    // copy reactant info into product fields
			rObject.TemporaryStep->numproducts = rObject.ProcessData.numreactants;
			     for (j = 0; j < rObject.TemporaryStep->numproducts; j++)
		    {
			 rObject.TemporaryStep->ProductsInStep[j] = rObject.ProcessData.ReactantsInStep[j];					rObject.TemporaryStep->ProductStoich[j] = rObject.ProcessData.ReactantStoich[j];     			}
	       } // end if reversible

		// read in product/reverse data

			rTIS >> DataCode;
			ASSERT ( DataCode == RXN_REV_RATE_LAW );
			rTIS >> rObject.ProcessData.numproducts;

			counter = 0;

			for (j = 0; j < rObject.ProcessData.numproducts; j++)
			{
				rTIS >> DataCode;
				ASSERT (DataCode == RXN_SPECIES_DATA);
				rTIS >> rObject.ProcessData.ProductsInStep[j];
				rTIS >> rObject.ProcessData.ProductStoich[j];
				rTIS >> Order;

				if (rObject.ProcessData.Direction == REVERSIBLE)
				{
			// copy product data into reactant data fields for reverse step
				     rObject.TemporaryStep->numreactants = rObject.ProcessData.numproducts;
					rObject.TemporaryStep->ReactantsInStep[j] = rObject.ProcessData.ProductsInStep[j];
					rObject.TemporaryStep->ReactantStoich[j] = rObject.ProcessData.ProductStoich[j];

					// Store kinetics data for reverse step
					if ( Order != 0.0 )
					{
						rObject.TemporaryStep->SpeciesInRateLaw[counter] = rObject.TemporaryStep->ReactantsInStep[j];
						rObject.TemporaryStep->ReactantOrder[counter] = Order;
						counter ++;
					}  // end if Order

				}  // end if ProcessData

			}  // end for j

			if (rObject.ProcessData.Direction == REVERSIBLE)
			{
				//finish setting up data for reverse step
				rObject.TemporaryStep->numSpeciesInRateLaw = counter;
			}
	       break;


		default:
			if ( DataCode != IDType (END_RXN_INIT) )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
			}
			break;


		}   // end switch


	} // end while

	return rTIS;

}       // end >>


//---------------------------------------------------------------------------


