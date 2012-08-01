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
//      Declaration of class SIMULATION_TIME
//      base class
//	This class contains the current time in the simulation, ElapsedTime,
//		and the most recent time step DeltaTime. It runs the Idler
//		for programmed variables CURRENTLY SPECIFIC TO REACTION STEPS
//		ONLY - EXTENDABLE TO OTHER TYPES
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef TIMEBASE_HXX
#define TIMEBASE_HXX

#include "vsimtype.hxx"
#include "txtstrem.hxx"
#include "sysidler.hxx"
#include "process.hxx"
#include "procarea.hxx"

class simulation_time {

private:
	FLOAT64 	ElapsedTime;
	FLOAT64 	DeltaTime;
	FLOAT64		MeanDeltaTime;
	system_idler	Idler;
	FLOAT64		MaxTimeStepForProfile;
	BOOL8		ProgrammedVariable;
	profile*	pProgrammedVariable;
	process_area*	pLocationOfProgrammedVariable;

public:
	simulation_time ();
	simulation_time ( const simulation_time& s_t );
	virtual ~simulation_time () {}

	void 	SetNewTime ();
	void	SetNewTime( FLOAT64 t ) { ElapsedTime = t; }
	void 	InitTime ( FLOAT64 t, FLOAT64 dt );
	FLOAT64 GetTime () const { return ElapsedTime; }
	FLOAT64 GetTimeStep () const { return DeltaTime; }
	FLOAT64   CalcMeanTimeStep ();
	FLOAT64   GetMeanTimeStep() { return MeanDeltaTime; }
	void      SetMaxTimeStepForProfile( FLOAT64 dtmax ) { MaxTimeStepForProfile = dtmax; }
	void      SetProgrammedVariableFlag( BOOL8 flag ) { ProgrammedVariable = flag; }
	BOOL8	GetProgrammedVariableFlag() { return ProgrammedVariable; }
     void      SetProgVariablePointer( profile* p ) { pProgrammedVariable = p; }
	void	     SetProgVariableLocation( process_area* p ) { pLocationOfProgrammedVariable = p; }
     void      SetIdlerPointers( profile* p ) { Idler.SetPointers( p, this ); }


	friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_time& rObject );


};

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_time& rObject );


#endif