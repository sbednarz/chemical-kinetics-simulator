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
//      Declaration of SIMULATION_STATUS
//      This object controls the progress of the simulation: tracking events,
//		determining termination status etc..
//      Date last modified: January 17, 1997
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef SIM_STAT_HXX
#define SIM_STAT_HXX

#include "vsimtype.hxx"
#include "txtstrem.hxx"
#include "idtypes.hxx"

// the following is for the temporary use of kbhit to interrupt a simulation
// in sim_stat.cxx
#include <conio.h>


class simulation_status  {

private:
	BOOL8 		ReactionOver;
	UINT32 		TotalNumberOfEvents;
	UINT32 		EventCount;
	FLOAT64 	     ElapsedTimeLimit;
	UINT32 		PrintIntervalCount;
	UINT32 		PrintInterval;
	UINT32 		DataRecordCount;
	UINT32		EquilDetectCycleLength;
	UINT32		EquilDetectCount;
	SIM_STATUS_CODE StatusCode;

public:
	simulation_status();
	simulation_status ( const simulation_status& s_s );
	virtual ~simulation_status() {}

	void 		UpdateStatus();
	void 		SetCode ( SIM_STATUS_CODE s ) { StatusCode = s; }
	SIM_STATUS_CODE GetCode () { return StatusCode; }
	UINT32 		GetNoRecords () { return DataRecordCount; }
	void		SetReactionOver () { ReactionOver = TRUE; }
	BOOL8 		IsReactionOver () { return ReactionOver; }
	UINT32	     GetEquilDetectCycleLength() { return EquilDetectCycleLength; }

	friend TextInputStream& operator >> ( TextInputStream& rTIS, simulation_status& rObject );
	friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_status& rObject );


};

TextInputStream& operator >> ( TextInputStream& rTIS, simulation_status& rObject );
BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_status& rObject );


#endif




