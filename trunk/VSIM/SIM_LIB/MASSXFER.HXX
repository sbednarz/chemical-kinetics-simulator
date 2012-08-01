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

//-----------------------------------------------------------------------------
//
//	declaration of derived class MASS_TRANSFER
//	base class: PROCESS
//	This object handles bulk transfer of chemical species between two
//		compartments via a transfer path. The expressions will
//		not apply to diffusive transfer
//	written by Frances Houle
//	Date last modified: March 1, 1996
//	IBM  
//
//-----------------------------------------------------------------------------


#ifndef MASSXFER_HXX
#define MASSXFER_HXX

#include "vsimtype.hxx"
#include "process.hxx"
#include <math.h>
#include "txtstrem.hxx"


class mass_transfer : public process {

protected:

public:
	mass_transfer 	();
	mass_transfer 	(const mass_transfer& m_t);
	~mass_transfer 	() {}

	void 	ProbCalc();
	void    InitializeRateCoefficients ();
	void	UpdateRateCoefficients ( FLOAT64 VolumeRatio );



	friend 	TextInputStream& operator >> ( TextInputStream& rTIS, mass_transfer& rObject );
};

TextInputStream& operator >> ( TextInputStream& rTIS, mass_transfer& rObject );

#endif


