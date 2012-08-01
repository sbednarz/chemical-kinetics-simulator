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
//      Methods for derived class MASS_DIFFUSION
//      Date last modified August 7, 1998
//      Written by Frances A. Houle
//      IBM  
//---------------------------------------------------------------------------


#include "massdifn.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

mass_diffusion :: mass_diffusion ()
: process ()

{

}


//---------------------------------------------------------------------------
//    Definition of function ProbCalc for reversible mass diffusion
//    Purpose: this is a general function for calculating a rate from
//     rate law parameters. New values for the probabilities are set
//    Parameters: none
//    Returns: nothing
//---------------------------------------------------------------------------

void mass_diffusion :: ProbCalc()

{

     FLOAT64 		q, np, T, Order;
     FLOAT64 		conc_source, conc_target, ConcentrationDifference;
     UINT16 		SpeciesNumber, ReactantCounter;
     BOOL8		EnoughReactants;
     ParticleType 	nparticles;


     // first see if there are going to be enough reactants in source
     // compartment. Since only one particle of the diffuser is to be transferred
     // just need to test whether there is at least one of it and 1 of any non-Fickian
     // environmental species.

     ReactantCounter = 0;
     EnoughReactants = TRUE;

     while ( ReactantCounter < ProcessData.numreactants )
     {

	  SpeciesNumber = ProcessData.ReactantsInStep[ReactantCounter];
	  nparticles = pSource->GetSpeciesCount( SpeciesNumber );
	  if ( nparticles < 1 )
	  {
	       EnoughReactants = FALSE;
	       break;
	  }                            // end if
	  ReactantCounter++;

     }        // endwhile ReactantCounter



     if ( EnoughReactants )

     {
	     T = pProcessArea->GetTemperature();

	     // calculate concentrations and concentration difference on going from
	     // source -> target. Assumes that the first species is the diffuser per
	     // convention

	     ReactantCounter = 0;

	     SpeciesNumber = ProcessData.SpeciesInRateLaw[ReactantCounter];
	     conc_source = ( FLOAT64 ) (pSource->GetSpeciesCount(SpeciesNumber)) / pSource->GetVolume();
	     conc_target = ( FLOAT64 ) (pTarget->GetSpeciesCount(SpeciesNumber)) / pTarget->GetVolume();
	     ConcentrationDifference = conc_source - conc_target;

	     // calculate mass transfer probabilities
	     // if there is a positive gradient from source to target,
	     // otherwise set probability == 0

	     // assume that kinetic order of diffuser is 1.0 - may change in future

	     if (ConcentrationDifference > 0)

	     {
		q = GetRateConstant( T );

		//calculate probability

		if ( ((transfer_path *)pProcessArea)->IsConstV() )
		{
			q *= ConcentrationDifference;

		} else {  // variable volume
			q *= ConcentrationDifference * ((transfer_path *)pProcessArea)->GetTransferArea()
					/ ((transfer_path *)pProcessArea)->GetTransferLength();
		}


		// proceed to add other concentration dependences if specified - handles
		// case of non-Fickian diffusion

		ReactantCounter = 1;

		while ( ReactantCounter < ProcessData.numSpeciesInRateLaw )
		{
			SpeciesNumber = ProcessData.SpeciesInRateLaw[ReactantCounter];
			np = ( FLOAT64 ) (pSource->GetSpeciesCount(SpeciesNumber));
			Order = ProcessData.ReactantOrder[ReactantCounter];

			// check values of order to avoid unnecessary floating point calcs
			if (Order == 1.0)
			{
				q *= np;
			}

			if ((Order != 0.0) && (Order != 1.0))
			{
				q *= ( pow( np, ProcessData.ReactantOrder[ReactantCounter] ) );
			}

				ReactantCounter++;
		}                  // endwhile ReactantCounter


		// set new value for probability
		Probability = q;

	   } else {   // ConcentrationDifference == 0 or < 0

		ZeroProbability();

	   }   // end if ConcentrationDifference

	} else {   // not enough reactants

		ZeroProbability();

	}	// end if EnoughReactants


     return;

}


//-----------------------------------------------------------------------------
//	Definition of method InitializeRateCoefficients
//	Purpose: to initialize values for rate parameter
//	Parameters: Transfer Path Number note: must be called AFTER set process
//		and rate data for step
//	Returns: nothing
//-----------------------------------------------------------------------------

void mass_diffusion :: InitializeRateCoefficients ()

{   // begin

	UINT16 	j;
	FLOAT64 Order, ConversionFactor, T;

	//determine and set total order of reaction
	//this is here for future use of complex diffusion dependencies
	//for Fickian diffusion Order will be 1 for diffusing species

	if ( RateConstant.Coefficient[0] )
	{
		Order = 0.0;
		for (j=0; j<ProcessData.numSpeciesInRateLaw; j++ )
		{
			Order += ProcessData.ReactantOrder[j];
		}    // end for

		Order -= 1.0;

		RateConstant.UnitsConversionExponent = Order;

		if ( Order != 0.0 )
		{
			ConversionFactor = pow( (pSource->GetMolarConversionFactor()), Order );
			RateConstant.Coefficient[0] /= ConversionFactor;
		}

		// divide by 1e3 to take care of cm to liters units mismatch in rate calculation
		RateConstant.Coefficient[0] /= 1000;

	  }

	  // if constT Arrhrate calculate rate constants and store in RateConstant.Coefficient[0]. Change flag
	  // to temp_independent so that k will not be recalculated

	  if ( (((transfer_path *)pProcessArea)->IsConstT()) && ( RateConstant.Format == TEMP_DEPENDENT ) )
	  {
		T = pProcessArea->GetTemperature();


		if ( RateConstant.Coefficient[0] )
		{
			if (RateConstant.Coefficient[1])
			{
				RateConstant.Coefficient[0] *= pow( T, RateConstant.Coefficient[1] );
			}

			if (RateConstant.Coefficient[2])
			{
				ConversionFactor = ( RateConstant.Coefficient[2] ) / ( R * T );
				if ( - ConversionFactor < MINIMUM_EXPONENT ) ConversionFactor = - MINIMUM_EXPONENT;
				RateConstant.Coefficient[0] *= exp( - ConversionFactor );
			}
		}

		RateConstant.Format = TEMP_INDEPENDENT;

	}    // end if const T

	// add geometric factors for constant volume calculations

	if ( ((transfer_path *)pProcessArea)->IsConstV() )
	{
		RateConstant.Coefficient[0] *= ((transfer_path *)pProcessArea)->GetTransferArea();
		RateConstant.Coefficient[0] /= ((transfer_path *)pProcessArea)->GetTransferLength();
	}

	return;

}    // end function


//-----------------------------------------------------------------------------
//	Definition of method UpdateRateCoefficients
//	Purpose: to renormalize A factor when the source compartment volume changes
//		geometrical factors for diffusion coefficient are updated as part of
//		probability calculation
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void mass_diffusion :: UpdateRateCoefficients ( FLOAT64 VolumeRatio )

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

TextInputStream& operator >> ( TextInputStream& rTIS, mass_diffusion& rObject )

{
	UINT16 j, temp;
	FLOAT64 Dummy, Order;

	// for now the data will only read in one species name, it is assumed there
	// is no explicit kinetic dependence of the rate of mass transfer on other species
	// it is also assumed that only one particle of the species is transferred at a time
	// but that the kinetic order can be different from 1.
	// j is the array index for the appropriate data

	j=0;

	while ( DataCode != IDType (END_TRANSFER_STEP_INIT) )

	{

		rTIS >> DataCode;

		switch (DataCode) {

		case XFER_STEP_DATA:
			// read enums into temp variable and type cast
			rTIS >> temp;
			rObject.RateConstant.Format = ( RATE_CONST_FORMAT )temp;

			rTIS >> temp;
			rObject.ProcessData.Direction = ( XFER_DIRECTION )temp;

			if (rObject.ProcessData.Direction == REVERSIBLE)
			{
				// allocate temporary storage for reverse step data
				rObject.InitTempStepData();
			}
			// read in number of reactants, number of products is assumed
			// to be the same
			rTIS >> rObject.ProcessData.numreactants;
			rObject.ProcessData.numproducts = rObject.ProcessData.numreactants;

			// fill in rest of process data structure

			// set number of species in rate law to be the same as number of reactants
			rObject.ProcessData.numSpeciesInRateLaw = rObject.ProcessData.numreactants;


		       for ( j = 0; j < rObject.ProcessData.numreactants; j++)
		       {
				// one particle of one species is transferred per step - set stoichiometries of all
				// = 1 as dummy values - only the rate law can be different from 1

				rObject.ProcessData.ReactantStoich[j] = 1;
				rObject.ProcessData.ProductStoich[j] = 1;

			       // read in code to be sure we are in the right place in the runparm file
			       rTIS >> DataCode;
			       ASSERT ( DataCode == XFER_STEP_RATE_LAW );

			       // read in species ID and copy into product data

			       rTIS >> rObject.ProcessData.ReactantsInStep[j];
			       rObject.ProcessData.ProductsInStep[j] = rObject.ProcessData.ReactantsInStep[j];
			       // read in order for kinetics
			       rTIS >> Order;

				// set up forward kinetics

				rObject.ProcessData.SpeciesInRateLaw[j] = rObject.ProcessData.ReactantsInStep[j];
				rObject.ProcessData.ReactantOrder[j] = Order;

				if (rObject.ProcessData.Direction == REVERSIBLE)
				{

				// fill in the rest of the TemporaryStep data structure
				// store stoichiometry
				rObject.TemporaryStep->ReactantsInStep[j] = rObject.ProcessData.ProductsInStep[j];
				rObject.TemporaryStep->ReactantStoich[j] = rObject.ProcessData.ProductStoich[j];
				rObject.TemporaryStep->ProductsInStep[j] = rObject.ProcessData.ReactantsInStep[j];
				rObject.TemporaryStep->ProductStoich[j] = rObject.ProcessData.ReactantStoich[j];

				// Store kinetics data for reverse step
				rObject.TemporaryStep->SpeciesInRateLaw[j] = rObject.TemporaryStep->ReactantsInStep[j];
				rObject.TemporaryStep->numSpeciesInRateLaw = rObject.ProcessData.numSpeciesInRateLaw;
				rObject.TemporaryStep->ReactantOrder[j] = rObject.ProcessData.ReactantOrder[j];

				//finish setting up data for reverse step
				rObject.TemporaryStep->numreactants = rObject.ProcessData.numproducts;
				rObject.TemporaryStep->numproducts = rObject.ProcessData.numreactants;
				rObject.TemporaryStep->Direction = REVERSIBLE;
				}

			}  // end for j
			break;

		case XFER_STEP_FWD_ACT_PARMS:
			rTIS >> rObject.RateConstant.Coefficient[0];
			rTIS >> rObject.RateConstant.Coefficient[1];
			rTIS >> rObject.RateConstant.Coefficient[2];
			break;


		case XFER_STEP_REV_ACT_PARMS:

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
			break;

		default:
		if ( DataCode != IDType (END_TRANSFER_STEP_INIT) )
	       {
		Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
	       }
			break;

		}   // end switch


	} // end while

	return rTIS;

}       // end >>


//---------------------------------------------------------------------------

