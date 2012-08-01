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
//       Date last modified January 18, 1997
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
	ProcessData.Reversible = FALSE;
	ProcessData.PartnerStepNo = 0;

	RateConstant = 0;

	Probability = 0.0;

	TemporaryStep = 0;
	TemporaryK = 0;
	pProcessArea = 0;

	pSource = 0;
	pTarget = 0;

}    // end function


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
	ProcessData.Reversible = pd.Reversible;
	ProcessData.PartnerStepNo = pd.PartnerStepNo;
	return;

}   // end function


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
//		Arrhenius parameters
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: InitTempKData ( rate_coefficient_info& k )

{

	TemporaryK = new rate_coefficient_info;

	TemporaryK->Coefficient[0] = k.Coefficient[0];
	TemporaryK->Coefficient[1] = k.Coefficient[1];
	TemporaryK->Coefficient[2] = k.Coefficient[2];

	TemporaryK->Format = k.Format;

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
	rate_coefficient_info k;

	// copy data into data structure
	k = pProcessArea->GetTempKData( StepNo );
	SetRateConstant ( k );

	// release temporary storage
	pProcessArea->DeleteTempK( StepNo );

	return;

}

//-----------------------------------------------------------------------------
// 	Definition of method SetPointer
//	Purpose: to set process area pointer in both the process and in the
//		k_info object.
//	Parameters: pointer to process area
//	Returns: nothing
//-----------------------------------------------------------------------------

void process :: SetPointer( process_area* r )
{

	pProcessArea = r;
	RateConstant->SetAreaPointer( r );

	return;
}


//-----------------------------------------------------------------------------




