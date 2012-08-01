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
//	declaration of derived class INTERFACE_DIFFUSION
//	base class: PROCESS
//	This object handles diffusion of chemical species between two
//		compartments via a diffusion path
//	written by Frances Houle
//	Date last modified: September 19, 1996
//	IBM  
//
//-----------------------------------------------------------------------------

#ifndef MASSINTF_HXX
#define MASSINTF_HXX

#include "process.hxx"
#include "vsimtype.hxx"
#include <math.h>
#include "txtstrem.hxx"


class interface_diffusion : public process {

public:
	interface_diffusion ();
	interface_diffusion ( const interface_diffusion& m_d );
	~interface_diffusion () {}

	void 	ProbCalc();
	void 	InitializeRateCoefficients ();
	void	     UpdateRateCoefficients ( FLOAT64 VolumeRatio );

	friend TextInputStream& operator >> ( TextInputStream& rTIS, interface_diffusion& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, interface_diffusion& rObject );

#endif


