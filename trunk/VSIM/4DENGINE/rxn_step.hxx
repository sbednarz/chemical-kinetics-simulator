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
//      Declaration of class REACTION_STEP
//      derived from base class PROCESS
//      Date last modified: Jnauary 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef RXN_STEP_HXX
#define RXN_STEP_HXX

#include "process.hxx"
#include <math.h>
#include "txtstrem.hxx"
#include "assert.hxx"
#include "vsimglob.hxx"
#include "r_x_k.hxx"
#include "prog_k.hxx"


class reaction_step : public process {

protected:

public:
	reaction_step ();
	reaction_step ( const reaction_step& r_s );
	~reaction_step () { ;}

	void	ProbCalc();

	friend TextInputStream& operator >> ( TextInputStream& rTIS, reaction_step& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, reaction_step& rObject );

#endif

