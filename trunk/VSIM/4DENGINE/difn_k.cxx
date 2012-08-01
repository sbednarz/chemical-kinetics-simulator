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
//      Methods for derived class DIFN_K
//      Date last modified January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//---------------------------------------------------------------------------


#include "difn_k.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

diffusion_k :: diffusion_k ()
: k_info ()

{

}


//-----------------------------------------------------------------------------------
//        Definition of function Get_k
//        Purpose: this is to calculate the current value of a rate constant for
//         use in a probability caculation
//        Returns: FLOAT64 rate constant
//-----------------------------------------------------------------------------------

FLOAT64 diffusion_k :: Get_k ( FLOAT64 T )

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
//	Definition of method Initialize_k
//	Purpose: to initialize values for rate parameter
//	Parameters: Transfer Path Number note: must be called AFTER set process
//		and rate data for step
//	Returns: nothing
//-----------------------------------------------------------------------------

void diffusion_k :: Initialize_k ()

{   // begin

	UINT16                   j;
	FLOAT64                  Order, ConversionFactor, T;
     process_info             ProcessData;
     reaction_compartment*    pSource;

	//determine and set total order of reaction
	//this is here for future use of complex diffusion dependencies
	//for Fickian diffusion Order will be 1 for diffusing species

	if ( RateConstant.Coefficient[0] )
	{
          ProcessData = pStep->GetProcessInfo();
		Order = 0.0;
		for (j=0; j<ProcessData.numSpeciesInRateLaw; j++ )
		{
			Order += ProcessData.ReactantOrder[j];
		}    // end for

		Order -= 1.0;

		RateConstant.UnitsConversionExponent = Order;

		if ( Order != 0.0 )
		{
               pSource = pStep->GetSource();
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

     // add P/T term if pressure and temperature are constant and phase is GAS for both source and
     // target compartments, pressure will only be non-zero for gas systems

    if ( (((transfer_path *)pProcessArea)->GetPressure()) &&
	 (((transfer_path *)pProcessArea)->IsConstT()) &&
	 (((transfer_path *)pProcessArea)->IsConstP()) )
       {
	  RateConstant.Coefficient[0] *= ( (pow ( pProcessArea->GetTemperature(),1.50) ) /
                    (((transfer_path *)pProcessArea)->GetPressure()) );
       }

	return;

}    // end function


//-----------------------------------------------------------------------------
//	Definition of method Update_k
//	Purpose: to renormalize A factor when the source compartment volume changes
//		geometrical factors for diffusion coefficient are updated as part of
//		probability calculation
//	Parameters: none
//	Returns: nothing
//-----------------------------------------------------------------------------

void diffusion_k :: Update_k ( FLOAT64 VolumeRatio )

{

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

TextInputStream& operator >> ( TextInputStream& rTIS, diffusion_k& rObject )

{
	rate_coefficient_info k_Data;


	while ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) )
	{
		rTIS >> DataCode;

		switch ( DataCode ) {

		case XFER_STEP_FWD_ACT_PARMS:
			rTIS >> rObject.RateConstant.Coefficient[0];
			rTIS >> rObject.RateConstant.Coefficient[1];
			rTIS >> rObject.RateConstant.Coefficient[2];
			break;

     	case XFER_STEP_REV_ACT_PARMS:
          	rTIS >> k_Data.Coefficient[0];
			rTIS >> k_Data.Coefficient[1];
			rTIS >> k_Data.Coefficient[2];

               if ( k_Data.Coefficient[0] ) // if zero no rate constant read over
               {
				k_Data.Format = rObject.RateConstant.Format;
				rObject.pStep->InitTempKData( k_Data );
			}
			break;

		default:
			if ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
			}
			break;

			}// end switch

	} // end while
	return rTIS;

}
//---------------------------------------------------------------------------

