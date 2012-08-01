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
//      Declaration of class SYSTEM_IDLER
//      base class
//	This class handles system idling when programmed variables are used
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef SYSIDLER_HXX
#define SYSIDLER_HXX

#include "vsimtype.hxx"
#include "profile.hxx"
#include "txtstrem.hxx"

class simulation_time;

class system_idler {

private:
	profile*		     pProgrammedVariable;
	simulation_time*	pTimeBase;


public:
	system_idler ();
	system_idler ( const system_idler& s_i );
	virtual ~system_idler () {}


	FLOAT64	RunIdler();
	void		SetPointers( profile* p, simulation_time* s );

};


#endif