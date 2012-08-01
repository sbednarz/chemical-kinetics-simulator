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

//-----------------------------------------------------------------------------------
//       Methods for base class PROCESS
//       Date last modified September 12, 1996
//       Written by Frances Houle
//       IBM  
//-----------------------------------------------------------------------------------

#include "process.hxx"
#include "procarea.hxx"


//-----------------------------------------------------------------------------------
//
//	Constructor
//
//-----------------------------------------------------------------------------------

process :: process ()

{     // begin function
	UINT16 j;

	ProcessData.numreactants = 0;
	ProcessData.numproducts = 0;
	for ( j=0; j<MAX_NUMBER_OF_COMPONENTS; j++ )
	{
		ProcessData.ReactantsInStep[j] = 0;
		ProcessData.ReactantStoich[j] = 0;
		ProcessData.ProductsInStep[j] = 0;
		ProcessData.ProductStoich[j] = 0;
	}   //end for
	ProcessData.numSpeciesInRateLaw = 0;
	for ( j=0; j<MAX_NUMBER_OF_COMPONENTS; j++ )
	{
		ProcessData.SpeciesInRateLaw[j] = 0;
		ProcessData.ReactantOrder[j] = 0.0;
	}   //end for
	ProcessData.Direction = SOURCE_TO_TARGET; // or reactants to products
	ProcessData.PartnerStepNo = 0;

	for ( j=0; j<3; j++ )
	{
		RateConstant.Coefficient[j] = 0.0;
	}   // end for
	RateConstant.UnitsConversionExponent = 0.0;
	RateConstant.Format = TEMP_INDEPENDENT;

	Probability = 0.0;

	TemporaryStep = 0;
	TemporaryK = 0;
	pProcessArea = 0;

	pSource = 0;
	pTarget = 0;

}    // end function



//-----------------------------------------------------------------------------------
//        Definition of function GetRateCconstant
//        Purpose: this is to calculate the current value of a rate constant for
//         use in a probability caculation
//        Returns: FLOAT64 rate constant
//-----------------------------------------------------------------------------------

FLOAT64 process :: GetRateConstant ( FLOAT64 T )

{         // begin function

     FLOAT64 q, zexp;

     q = RateConstant.Coefficient[0];

     switch ( RateConstant.Format )
    {

     case TEMP_INDEPENDENT :
     break;

     default :  // TEMP_DEPENDENT
	     if (RateConstant.Coefficient[2])
	     {
		zexp = ( RateConstant.Coefficient[2] ) / ( R * T );
		if ( - zexp < MINIMUM_EXPONENT ) // check for underflow
			zexp = - MINIMUM_EXPONENT;
		q *= ( exp( - zexp ) );
	     }                            // endif

	    if ( RateConstant.Coefficient[1] )
	   {
		q *= ( pow( T, ( RateConstant.Coefficient[1] ) ) );
	   }                            // endif

     break;

     }    // end switch

     return q;

}       //end function


//-----------------------------------------------------------------------------
//	Definition of method SetProcessData
//	Purpose: to initialize values for a process step
//	Parameters: a process info structure built from input data
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: SetProcessData ( process_info pd )

{     // begin function
	UINT16 j;

	ProcessData.numreactants = pd.numreactants;
	ProcessData.numproducts = pd.numproducts;
	for ( j=0; j<pd.numreactants; j++ )
	{
		ProcessData.ReactantsInStep[j] = pd.ReactantsInStep[j];
		ProcessData.ReactantStoich[j] = pd.ReactantStoich[j];
	}   //end for
	for ( j=0; j<pd.numproducts; j++ )
	{
		ProcessData.ProductsInStep[j] = pd.ProductsInStep[j];
		ProcessData.ProductStoich[j] = pd.ProductStoich[j];
	}   //end for
	ProcessData.numSpeciesInRateLaw = pd.numSpeciesInRateLaw;
	for ( j=0; j<pd.numSpeciesInRateLaw; j++ )
	{
		ProcessData.SpeciesInRateLaw[j] = pd.SpeciesInRateLaw[j];
		ProcessData.ReactantOrder[j] = pd.ReactantOrder[j];
	}   //end for
	ProcessData.Direction = pd.Direction;
	ProcessData.PartnerStepNo = pd.PartnerStepNo;
	return;

}   // end function

//-----------------------------------------------------------------------------
//	Definition of method SetRateConstant
//	Purpose: to read in values for rate parameter
//	Parameters: A, m, Ea
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: SetRateConstant ( k_info k )

{   // begin

	RateConstant.Coefficient[0] = k.Coefficient[0];
	RateConstant.Coefficient[1] = k.Coefficient[1];
	RateConstant.Coefficient[2] = k.Coefficient[2];

	RateConstant.UnitsConversionExponent = k.UnitsConversionExponent;
	RateConstant.Format = k.Format;

	return;

}   // end


//-----------------------------------------------------------------------------
//	Definition of method InitTempStepData
//	Purpose: to allocate space for storing reversible step kinetics
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: InitTempStepData ()

{
	UINT16 j;

	TemporaryStep = new process_info;

	TemporaryStep->numreactants = 0;
	TemporaryStep->numproducts = 0;
	for ( j=0; j<MAX_NUMBER_OF_COMPONENTS; j++ )
	{
		TemporaryStep->ReactantsInStep[j] = 0;
		TemporaryStep->ReactantStoich[j] = 0;
		TemporaryStep->ProductsInStep[j] = 0;
		TemporaryStep->ProductStoich[j] = 0;
	}   //end for
	TemporaryStep->numSpeciesInRateLaw = 0;
	for ( j=0; j<MAX_NUMBER_OF_COMPONENTS; j++ )
	{
		TemporaryStep->SpeciesInRateLaw[j] = 0;
		TemporaryStep->ReactantOrder[j] = 0.0;
	}   //end for}

	return;

}


//-----------------------------------------------------------------------------
//	Definition of method InitTempKData
//	Purpose: to allocate space for storing reversible step rate constant
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: InitTempKData ()

{
	UINT16 j;

	TemporaryK = new k_info;

	for ( j=0; j < 3; j++ )
	{
		TemporaryK->Coefficient[j] = 0.0;
	}

	TemporaryK->UnitsConversionExponent = 0.0;
	TemporaryK->Format = TEMP_INDEPENDENT;
	return;

}


//-----------------------------------------------------------------------------
//	Definition of method MoveTempStepData
//	Purpose: to move data from temp storage to permanent object
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: MoveTempStepData ( UINT16 StepNo)
{
	process_info p;

	// copy data into data structure
	p = pProcessArea->GetTempStepData( StepNo );
	SetProcessData ( p );
	ProcessData.PartnerStepNo = StepNo;

	// release temporary storage
	pProcessArea->DeleteTempStep( StepNo );
	return;

}




//-----------------------------------------------------------------------------
//	Definition of method MoveTempKData
//	Purpose: to move data from temp storage to permanent object
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: MoveTempKData ( UINT16 StepNo )
{
	k_info k;

	// copy data into data structure
	k = pProcessArea->GetTempKData( StepNo );
	SetRateConstant ( k );

	// release temporary storage
	pProcessArea->DeleteTempK( StepNo );

	return;

}

//-----------------------------------------------------------------------------








