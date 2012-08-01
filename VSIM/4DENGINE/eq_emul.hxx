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
//      Declaration of EQUILIBRIUM_EMULATOR
//      This object contains objects and methods used for detecting and
//		emulating partial equilibria
//      Date last modified: April 30, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef EQ_EMUL_HXX
#define EQ_EMUL_HXX

#include "vsimtype.hxx"
#include "txtstrem.hxx"
#include "vsimglob.hxx"
#include "datastru.hxx"



class equilibrium_emulator {

protected:
	FLOAT64 		MinEfficiency;
	FLOAT64             	SelectionFrequency;
	UINT32 			NoRevStepsInSystem;
	equil_detect_info*	EventHistory;
	UINT32			Counter;
	BOOL8			Equilibrium;


public:
	equilibrium_emulator ();
	equilibrium_emulator ( const equilibrium_emulator& e_e );
	~equilibrium_emulator () { delete [] EventHistory; }

	void	     LogEvent();
	BOOL8	     CheckEventPatternForEquilibrium();
	BOOL8	     QueryEquilibrium() { return Equilibrium; }
	void	     ResetEmulator();
	void	     StartNewCycle();
	void         InitEventHistory( UINT32 NoSteps, UINT32 CycleLength );

	friend TextInputStream&  operator >> ( TextInputStream& rTIS, equilibrium_emulator& rObject );
	friend BOOL8             operator == ( selected_event_info& a, selected_event_info& b );
};

TextInputStream& operator >> ( TextInputStream& rTIS, equilibrium_emulator& rObject );
BOOL8            operator == ( selected_event_info& a, selected_event_info& b );

#endif
