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
//	declaration of derived class MASS_INTERFACE_TRANSFER
//	base class: PROCESS
//	This object handles diffusion of chemical species between two
//		interface compartments via a diffusion path
//	written by Frances Houle
//	Date last modified: January 23, 1997
//	IBM  
//
//-----------------------------------------------------------------------------

#ifndef MASSINTX_HXX
#define MASSINTX_HXX

#include "process.hxx"
#include "vsimtype.hxx"
#include <math.h>
#include "txtstrem.hxx"
#include "r_x_k.hxx"
#include "prog_k.hxx"


class mass_interface_transfer : public process {

public:
	mass_interface_transfer ();
	mass_interface_transfer ( const mass_interface_transfer& m_i_x );
	~mass_interface_transfer () {}

	void 	ProbCalc();

	friend TextInputStream& operator >> ( TextInputStream& rTIS, mass_interface_transfer& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, mass_interface_transfer& rObject );

#endif


