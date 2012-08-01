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
//      Methods for class PROGRAMMED_K
//      Date last modified: January 18, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "prog_k.hxx"
#include "simulatr.hxx"

//---------------------------------------------------------------------------
//        Constructor
//---------------------------------------------------------------------------

programmed_k :: programmed_k ()
: k_info ()

{

}


//---------------------------------------------------------------------------
//	Definition of method Initialize_k
//	Purpose: to convert units of programmed k from moles/liter-sec to
//		particles/sec in compartments and transfer paths
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void programmed_k :: Initialize_k ()

{
	UINT16 		j;
	FLOAT64    	ConversionFactor, Order;
	process_info	ProcessData;

	// Programmed rate constants are always single valued. Provide for
	// both first order and higher order reaction steps. Obtain factor
	// for units

	// get process data
	ProcessData = pStep->GetProcessInfo();

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
	}

	kProfile->InitializeProfile( ConversionFactor );

	return;

}


//-----------------------------------------------------------------------------
//	Definition of method Update_k
//	Purpose: to renormalize A factor when the compartment volume changes
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void programmed_k :: Update_k ( FLOAT64 SizeRatio )

{
	FLOAT64	Factor;

	if ( RateConstant.UnitsConversionExponent )
	{
		Factor = pow ( SizeRatio, RateConstant.UnitsConversionExponent );
		kProfile->AdjustUnits( Factor );
	}

	return;

}


//-----------------------------------------------------------------------------
//	Definition of input operator >>
//	Purpose: to read in programmed k profile. works for reactions and for
//		simple mass transfer
//	Parameters: file name and object pointer
//	Returns: file pointer
//-----------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, programmed_k& rObject )

{

	while ( ( DataCode != ( IDType( END_RXN_RATE_CONSTANT_INIT ) ) ) ||
             ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) ) )
	{
		rTIS >> DataCode;

		switch ( DataCode ) {

		case PROFILE_INIT:
			// allocate profile object
			rObject.kProfile = new profile;
			rTIS >> *rObject.kProfile;
			break;

		default:
			if ( ( DataCode != ( IDType( END_RXN_RATE_CONSTANT_INIT ) ) ) ||
			     ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) ) )
    			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
			}
			break;
		}// end switch

	} // end while
	return rTIS;

}
//-----------------------------------------------------------------------------







