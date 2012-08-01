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
//      Date last modified: August 5, 1998
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

//---------------------------------------------------------------------------
//	Definition of method InitializeRateCoefficients
//	Purpose: to convert units of rate constants from moles/liter-sec to
//		particles/sec in compartments and transfer paths, and then
//		pre-calculate rate constants for cases where T is constant
//		and Arrhenius parameters are used
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_step :: InitializeRateCoefficients ()

{
	UINT16 j;
	FLOAT64 ConversionFactor, Order, T;

	// Convert units of rate constant by multiplying either k or A
	// by  constant factor - applies to all TPV options

	if ( RateConstant.Coefficient[0] )
	{
		Order = 0.0;
		for ( j=0; j<ProcessData.numSpeciesInRateLaw; j++ )
		{
			Order += ProcessData.ReactantOrder[j];
		}

		Order -= 1.0;

		RateConstant.UnitsConversionExponent = Order;

		if ( Order != 0.0 )
		{
			ConversionFactor = pow( ((reaction_compartment *)pProcessArea)->GetMolarConversionFactor(), Order );
			RateConstant.Coefficient[0] /= ConversionFactor;
		}
	  }

	  // if constT Arrhrate calculate rate constants and store in RateConstant.Coefficient[0], set temp independent
	  // flag in data structure so that k will not be recalculated

	  if ( ( ((reaction_compartment *)pProcessArea)->QueryTOption() == CONSTANT_TEMP )
			&& (RateConstant.Format == TEMP_DEPENDENT) )
	  {
		T = pProcessArea->GetTemperature();

		if ( RateConstant.Coefficient[0] )
		{
			if ( RateConstant.Coefficient[1] )
			{
				RateConstant.Coefficient[0] *= pow( T, RateConstant.Coefficient[1] );
			}
			if ( RateConstant.Coefficient[2] )
			{
				ConversionFactor = ( RateConstant.Coefficient[2] ) / ( R * T );
				if ( - ConversionFactor < MINIMUM_EXPONENT ) ConversionFactor = - MINIMUM_EXPONENT;
				RateConstant.Coefficient[0] *= exp( - ConversionFactor );
			}

		}

		RateConstant.Format = TEMP_INDEPENDENT;
	}
	return;

}


//-----------------------------------------------------------------------------
//	Definition of method UpdateRateCoefficients
//	Purpose: to renormalize A factor when the compartment volume changes
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void reaction_step :: UpdateRateCoefficients ( FLOAT64 VolumeRatio )

{
	UINT32	j;

	if ( RateConstant.UnitsConversionExponent )
	{
		RateConstant.Coefficient[0] /= pow ( VolumeRatio, RateConstant.UnitsConversionExponent );
	}

	return;

}


//-----------------------------------------------------------------------------
//	Definition of input operator >>
//	Purpose: to read in and set up reaction step data
//	Parameters: file name and object pointer
//	Returns: file pointer
//-----------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, reaction_step& rObject )

{
	UINT16 j, counter, temp;
	FLOAT64 Dummy, Order;

	while ( DataCode != IDType( END_RXN_INIT ) )

	{

		rTIS >> DataCode;

		switch (DataCode) {

		case RXN_DATA:
			rTIS >> temp;
			rObject.ProcessData.Direction = ( enum XFER_DIRECTION )temp;
			break;

		case RXN_RATE_CONSTANT_INIT:
			rTIS >> DataCode;  //RXN_RATE_CONST_TYPE;
			// read enums into temporary variable, cast to type

			rTIS >> temp;
			rObject.RateConstant.Format = ( enum RATE_CONST_FORMAT )temp;

			if ( rObject.RateConstant.Format != PROGRAMMED )
			{

				// case TEMP_INDEPENDENT or TEMP_DEPENDENT:

				// 8/98 for future implementation of programmed rate constant need to assign rate constant to
				// an initialized pointer rather than a k_info structure. This way either a k_info or a
				// programmed rate constant file can be used.

				rTIS >> DataCode;

				ASSERT ( DataCode == RXN_FWD_ACT_PARMS);
				rTIS >> rObject.RateConstant.Coefficient[0];
				rTIS >> rObject.RateConstant.Coefficient[1];
				rTIS >> rObject.RateConstant.Coefficient[2];

				rTIS >> DataCode;
				ASSERT ( DataCode == RXN_REV_ACT_PARMS );

				if ( rObject.ProcessData.Direction == REVERSIBLE )
				{
					rObject.InitTempKData();
					rTIS >> rObject.TemporaryK->Coefficient[0];
					rTIS >> rObject.TemporaryK->Coefficient[1];
					rTIS >> rObject.TemporaryK->Coefficient[2];
					rObject.TemporaryK->Format = rObject.RateConstant.Format;
				}
				else
				{       // read and discard
					rTIS >> Dummy;
					rTIS >> Dummy;
					rTIS >> Dummy;
				}
			  }

				// would put code for case PROGRAMMED here - may bne best to do this with
				// rate constants defined as objects

				// stub only - file structure for programmed rate constant has not been implemented,
				// also need to write the code in update rate constant to accommodate program

				// read in end-of-rate-constant code
				rTIS >> DataCode;
				ASSERT ( DataCode == END_RXN_RATE_CONSTANT_INIT );			   break;


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


