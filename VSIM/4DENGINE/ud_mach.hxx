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
//
//  declaration of UPDATE_MACHINE 
//  This object holds an array of pointers to required derived
//	state_update objects, and has a method to step through them
//	sequentially after each event selection
//  Date last modified: January 22, 1997
//  Written by Frances A. Houle
//  IBM  
//
//---------------------------------------------------------------------------

#ifndef UD_MACH_HXX
#define UD_MACH_HXX

#include "string.h"
#include "vsimtype.hxx"
#include "ud_state.hxx"
#include "ud_chem.hxx"
#include "ud_press.hxx"
#include "ud_vol.hxx"
#include "ud_geom.hxx"
#include "ud_k.hxx"
#include "ud_prob.hxx"
#include "ud_equil.hxx"
#include "vsimglob.hxx"


class update_machine {

private:
	state_update**		     UpdateStep; // ptr to array of [NumberOfUpdateSteps] pointers
	UINT16 			     NumberOfUpdateSteps;
	UINT16 			     MaxNoOfUpdateSteps;
	chem_update 		     ChemUpdate;
	vol_update 		     VolumeUpdate;
     press_update             PressureUpdate;
     geom_update              GeometryUpdate;
     rate_constant_update     RateConstantUpdate;
	prob_update 		     ProbUpdate;
	equil_emul_update 	     EmulUpdate;

public:
	update_machine ();
	update_machine (const update_machine& u_m);
	~update_machine () { delete [] ( UpdateStep ); }

	void 	Build ();
	void 	UpdateTheSystem (); // steps through array
	UINT16 	GetNoSteps () { return NumberOfUpdateSteps; }
	void 	SetNoSteps ( UINT16 n )  { NumberOfUpdateSteps = n; }

};

#endif


