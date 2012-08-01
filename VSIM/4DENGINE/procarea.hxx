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
//	Declaration of class PROCESS_AREA
//	base class
//	Date last modified: January 23, 1997
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef PROCAREA_HXX
#define PROCAREA_HXX

#include "vsimtype.hxx"
#include "process.hxx"
#include "datastru.hxx"

class reaction_compartment;


class process_area {         //base class

protected:
     process**      Step;
     UINT32         NumberOfSteps;
     UINT32         NumberOfRevSteps;
     UINT16         ID;
     FLOAT64        Temperature;
     FLOAT64        TotalProbability;
     BOOL8          ZeroProbability;
     enum REC_TYPE  ProcessAreaType;


public:
	process_area ();
	process_area ( const process_area& p_a );
	virtual ~process_area ();

     void                SetProcessAreaType( enum REC_TYPE AreaType ) { ProcessAreaType = AreaType; }
     enum REC_TYPE       GetProcessAreaType() {return ProcessAreaType; }
	UINT32              GetNoSteps() { return NumberOfSteps; }
	UINT32              GetNoRevSteps() { return NumberOfRevSteps; }
	UINT16              GetID() { return ID; }
	FLOAT64             GetTemperature() { return Temperature; }
	void                SetTemperature( FLOAT64 T ) { Temperature = T; }

	process_info&            GetStepInfo ( UINT32 StepNo ) {
				    return Step[StepNo]->GetProcessInfo(); }
	reaction_compartment*    GetStepSource( UINT32 StepNo ) {
				   return Step[StepNo]->GetSource(); }

	FLOAT64	       FreezeSteps( UINT32 Step1, UINT32 Step2 ); // for equilibria
	FLOAT64	       RestoreSteps( UINT32 Step1, UINT32 Step2 ); // for equilibria
	FLOAT64        GetStepProbability (UINT32 StepNo) {
			 return Step[StepNo]->GetProbability();}
	FLOAT64        GetTotalProbability() { return TotalProbability;}
	BOOL8	       QueryZeroProbability () { return ZeroProbability; }
	void           CalcTotalProbability();
	void           CalcAllProbabilities();

	void           UpdateStepProbability(UINT32 StepNo) {
				  Step[ StepNo ]->ProbCalc(); }
	virtual void   UpdateProbabilities(selected_event_info& Event) {}

	virtual void   UpdateTemperature() {}

     // the following are for initialization of a process area
	void                     SetID( UINT16 no ) { ID = no; }
	void                     InitRateConstants();
	virtual void             InitUpdateTables() {}
	process_info&            GetTempStepData ( UINT32 StepNo ) { return Step[StepNo]->GetTempProcessData (); }
	void                     DeleteTempStep ( UINT32 StepNo ) { Step[StepNo]->DeleteTempStepData(); }
	rate_coefficient_info&   GetTempKData ( UINT32 StepNo ) { return Step[StepNo]->GetTempRateConstant(); }
	void                     DeleteTempK ( UINT32 StepNo ) { Step[StepNo]->DeleteTempKData(); }


};

#endif