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
//      Declaration of INITIALIZE_SIMULATOR
//      This object is the base class for more specialized objects which will
//		read in all data needed to run a simulation and initialize
//		required internal variables
//      Date last modified: September 25, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef INIT_SIM_HXX
#define INIT_SIM_HXX


#include "txtstrem.hxx"
#include "binstrem.hxx"
#include "idtypes.hxx"
#include "vsimtype.hxx"
#include "vsimglob.hxx"
#include "simulatr.hxx"


class initialize_simulator

{

protected:
	virtual void ReadInData() {}
	virtual void SetUpSimulation() {}
	void SetUpOutputFile();

public:
	initialize_simulator () {}
	initialize_simulator ( const initialize_simulator& i_s );
	virtual ~initialize_simulator() {}

	void Go();

};


#endif