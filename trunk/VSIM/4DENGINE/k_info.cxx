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
//       Methods for base class K_INFO
//       Date last modified January 24, 1997
//       Written by Frances Houle
//       IBM  
//-----------------------------------------------------------------------------------

#include "k_info.hxx"


//-----------------------------------------------------------------------------------
//
//	Constructor
//
//-----------------------------------------------------------------------------------

k_info :: k_info ()

{
	UINT16	j;

	pProcessArea = 0;
	pStep = 0;

	for ( j=0; j<3; j++ )
	{
		RateConstant.Coefficient[j] = 0.0;
	}

	RateConstant.UnitsConversionExponent = 0.0;
	RateConstant.Format = TEMP_INDEPENDENT;
	kProfile = 0;


}


//-----------------------------------------------------------------------------
//	Definition of method Get_k
//	Purpose: to calculate/return rate constant for current conditions
//	Parameters: temperature
//	Returns: rate constant
//-----------------------------------------------------------------------------

FLOAT64 k_info :: Get_k ( FLOAT64 T )
{
	return T;
}


//-----------------------------------------------------------------------------
//	Definition of method Set_k
//	Purpose: to read in values for Arrhenius rate parameters
//	Parameters: rate_coefficient_info
//	Returns: nothing
//-----------------------------------------------------------------------------

void k_info :: Set_k ( rate_coefficient_info& k )

{   // only copies Arrhenius parameters and format - will not copy profile

	RateConstant.Coefficient[0] = k.Coefficient[0];
	RateConstant.Coefficient[1] = k.Coefficient[1];
	RateConstant.Coefficient[2] = k.Coefficient[2];

	RateConstant.Format = k.Format;


	return;

}   // end

//-----------------------------------------------------------------------------------
