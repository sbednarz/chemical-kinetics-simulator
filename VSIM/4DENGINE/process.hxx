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
//      Declaration of class PROCESS
//      base class
//	   This object is a generic process step - all actual steps are
//		derived from it
//      Date last modified: January 18, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef PROCESS_HXX
#define PROCESS_HXX

#include <math.h>
#include "vsimtype.hxx"
#include "vsimcons.hxx"
#include "datastru.hxx"
#include "idtypes.hxx"
#include "k_info.hxx"

class process_area;
class reaction_compartment;


class process {

protected:
	process_info   		ProcessData;
	k_info*        		RateConstant;
	FLOAT64        		Probability;
	process_info*   	TemporaryStep;
	rate_coefficient_info* 	TemporaryK;
	process_area*   	pProcessArea;
	reaction_compartment*   pSource; //for transfer
	reaction_compartment*   pTarget; //for transfer

public:
	process();
	process( const process& p );
	virtual ~process() {}



	virtual void   		ProbCalc() {}
	FLOAT64        		GetProbability () const { return Probability; }
	void           		ZeroProbability () { Probability = 0.0; }

	process_info&  		GetProcessInfo () { return ProcessData; }
	enum XFER_DIRECTION     QueryDirection () { return ProcessData.Direction; }
	void           		SetPartnerStepNo ( UINT32 StepNo ) { ProcessData.PartnerStepNo = StepNo; }
	UINT16         		GetPartnerStepNo () { return ProcessData.PartnerStepNo; }
	void           		SetProcessData ( process_info pd );

	FLOAT64        		GetRateConstant ( FLOAT64 T ) { return RateConstant->Get_k( T ); }
	void           		SetRateConstant ( rate_coefficient_info& k ) { RateConstant->Set_k( k ); }
	enum RATE_CONST_FORMAT 	Query_k_Format () { return RateConstant->GetFormat(); }

	void 			UpdateRateCoefficients ( FLOAT64 VolumeRatio ) { RateConstant->Update_k( VolumeRatio ); }


     // the following are for all steps involving transfer

	reaction_compartment* 	GetSource () { return pSource; }
	reaction_compartment*	GetTarget () { return pTarget; }

     // the following are used to initialize the process object
	void		SetSource ( reaction_compartment* s ) { pSource = s; } //transfer
	void		SetTarget ( reaction_compartment* t ) { pTarget = t; } //transfer
	void		SetPointer( process_area* r );
     profile*  GetProfilePointer() { return RateConstant->GetkProfilePointer(); }
	void   		InitializeRateCoefficients() { RateConstant->Initialize_k(); }

	void           	InitTempStepData ();
	void		MoveTempStepData ( UINT16 StepNo );
	void           	DeleteTempStepData () { delete TemporaryStep; }
	process_info&	GetTempProcessData () { return *TemporaryStep; }

	void           		InitTempKData ( rate_coefficient_info& k );
	void           		DeleteTempKData () { delete TemporaryK; }
	void			MoveTempKData ( UINT16 StepNo );
	rate_coefficient_info&	GetTempRateConstant () { return *TemporaryK; }

};


#endif

