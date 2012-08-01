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
//      Methods for class REACTION_TRANSFER_K
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "r_x_k.hxx"
#include "simulatr.hxx"

//---------------------------------------------------------------------------
//        Constructor
//---------------------------------------------------------------------------

reaction_transfer_k :: reaction_transfer_k ()
: k_info ()

{

}


//-----------------------------------------------------------------------------------
//        Definition of function Get_k
//        Purpose: this is to calculate the current value of a rate constant for
//         use in a probability caculation
//        Returns: FLOAT64 rate constant
//-----------------------------------------------------------------------------------

FLOAT64 reaction_transfer_k :: Get_k ( FLOAT64 T )

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


//---------------------------------------------------------------------------
//	Definition of method Initialize_k
//	Purpose: to convert units of rate constants from moles/liter-sec to
//		particles/sec in compartments and transfer paths, and then
//		pre-calculate rate constants for cases where T is constant
//		and Arrhenius parameters are used
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void reaction_transfer_k :: Initialize_k ()

{
	UINT16 		j;
	FLOAT64 	     ConversionFactor, Order, T;
	process_info	ProcessData;
	enum REC_TYPE	AreaType;
	BOOL8		Proceed;

	Proceed = FALSE;

	// get process data for step
	ProcessData = pStep->GetProcessInfo();

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

	  AreaType = pProcessArea->GetProcessAreaType();

	  switch ( AreaType ) {

	  case COMPARTMENT_TYPE_CONTAINER:
	  case COMPARTMENT_TYPE_3D:
	  case COMPARTMENT_TYPE_INTERFACE:

		if ( ( ((reaction_compartment *)pProcessArea)->QueryTOption() == CONSTANT_TEMP )
			&& (RateConstant.Format == TEMP_DEPENDENT) )
		{
			Proceed = TRUE;
		}
		break;

	  case TRANSFER_TYPE_XFER:
	  case TRANSFER_TYPE_INTERFACE_XFER:

		if ( ( ((transfer_path *)pProcessArea)->IsConstT() == TRUE )
			&& (RateConstant.Format == TEMP_DEPENDENT) )
		{
			Proceed = TRUE;
		}
		break;

	  default:
		break;

	}

	if ( Proceed )
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
//	Definition of method Update_k
//	Purpose: to renormalize A factor when the compartment volume changes
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void reaction_transfer_k :: Update_k ( FLOAT64 SizeRatio )

{

	if ( RateConstant.UnitsConversionExponent )
	{
		RateConstant.Coefficient[0] /= pow ( SizeRatio, RateConstant.UnitsConversionExponent );
	}

	return;

}


//-----------------------------------------------------------------------------
//	Definition of input operator >>
//	Purpose: to read in and set up reaction step data - generic for reactions
//		and simple mass transfer (not diffusion)
//	Parameters: file name and object pointer
//	Returns: file pointer
//-----------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, reaction_transfer_k& rObject )

{
	rate_coefficient_info k_Data;


	while ( ( DataCode != ( IDType( END_RXN_RATE_CONSTANT_INIT ) ) ) ||
		   ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) ) )
	{
		rTIS >> DataCode;

		switch ( DataCode ) {

		case RXN_FWD_ACT_PARMS:
			rTIS >> rObject.RateConstant.Coefficient[0];
			rTIS >> rObject.RateConstant.Coefficient[1];
			rTIS >> rObject.RateConstant.Coefficient[2];
			break;

		case RXN_REV_ACT_PARMS:
			rTIS >> k_Data.Coefficient[0];
			rTIS >> k_Data.Coefficient[1];
			rTIS >> k_Data.Coefficient[2];

               if ( k_Data.Coefficient[0] )
               {
                    // reversible rate constant - copy values
				k_Data.Format = rObject.RateConstant.Format;
				rObject.pStep->InitTempKData( k_Data );
			}
			break;

		case XFER_STEP_FWD_ACT_PARMS:
			rTIS >> rObject.RateConstant.Coefficient[0];
			rTIS >> rObject.RateConstant.Coefficient[1];
			rTIS >> rObject.RateConstant.Coefficient[2];
			break;

		case XFER_STEP_REV_ACT_PARMS:
			rTIS >> k_Data.Coefficient[0];
			rTIS >> k_Data.Coefficient[1];
			rTIS >> k_Data.Coefficient[2];

               if ( k_Data.Coefficient[0] )
               {
                    // reverse rate coefficient - copy over
				k_Data.Format = rObject.RateConstant.Format;
				rObject.pStep->InitTempKData( k_Data );
			}
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


//---------------------------------------------------------------------------


