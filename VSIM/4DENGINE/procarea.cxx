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
//	Declaration of methods for class PROCESS_AREA
//	base class
//	Date last modified: December 17, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#include "procarea.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

process_area :: process_area ()

{

	Step = 0;
     NumberOfSteps = 0;
     NumberOfRevSteps = 0;
     ID = 0;
     Temperature = 0;
     TotalProbability = 0;
     ZeroProbability = FALSE;
     ProcessAreaType = COMPARTMENT_TYPE_CONTAINER;

}

//---------------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------------

process_area :: ~process_area ()

{
     delete [] ( Step );
}


//---------------------------------------------------------------------------
//	Definition of method FreezeSteps
//	Purpose: to set to zero probabilities of equilibrated steps, and
//		adjust probability subtotal of process area
//	Parameters: indices of steps to be frozen (fwd/rev pair)
//	Returns: nothing
//
//---------------------------------------------------------------------------

FLOAT64 process_area :: FreezeSteps( UINT32 Step1, UINT32 Step2 )

{
	FLOAT64 	EquilProbs;


	EquilProbs = 0.0;

	EquilProbs += Step[ Step1 ]->GetProbability();
	Step[ Step1 ]->ZeroProbability();

	EquilProbs += Step[ Step2 ]->GetProbability();
	Step[ Step2 ]->ZeroProbability();

	TotalProbability -= EquilProbs;

	return EquilProbs;

}


//---------------------------------------------------------------------------
//
//	Definition of method RestoreSteps
//	Purpose: to reactivate equilibrated steps that had been frozen previously
//	Parameters: Indices of the fwd-rvs reaction step pair
//	Returns: probability subtotal of affected steps
//
//---------------------------------------------------------------------------

FLOAT64 process_area :: RestoreSteps( UINT32 Step1, UINT32 Step2 )

{
	FLOAT64 	EquilProbs, StepProb;


	EquilProbs = 0.0;

	// check to see if Probability is non-zero already - if so, don't need to
	// recalculate
	StepProb = Step[ Step1 ]->GetProbability();
	if ( StepProb == 0.0 )
	{
		Step[ Step1 ]->ProbCalc();
		EquilProbs += Step[ Step1 ]->GetProbability();
	}

	// repeat for other half of pair
	StepProb = Step[ Step2 ]->GetProbability();
	if ( StepProb == 0.0 )
	{
		Step[ Step2 ]->ProbCalc();
		EquilProbs += Step[ Step2 ]->GetProbability();
	}
	if ( EquilProbs )
	{
		TotalProbability += EquilProbs;

	}

	return EquilProbs;

}


//---------------------------------------------------------------------------
//         Definition of method CalcTotalProbability
//         Purpose: to calculate the probability subtotal for the process area
//         Parameters: none
//         Returns: nothing
//---------------------------------------------------------------------------
void process_area :: CalcTotalProbability (void)

{      // begin
	UINT16 n;
	FLOAT64 ProbSubtotal, StepProb;

	for ( n = 0, ProbSubtotal = 0.0; n < NumberOfSteps; n++ )
     {
          StepProb = GetStepProbability (n);

          // explicitly add only non-zero probabilities
          if ( StepProb )
          {
               ProbSubtotal += StepProb;
          }
     }

	TotalProbability = ProbSubtotal;

	return;

}    // end


//---------------------------------------------------------------------------
//	Definition of method CalcAllProbabilities
//	Purpose: to calculate all step probabilities and the total
//		probability for the process area
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void process_area :: CalcAllProbabilities()

{
	UINT32 j;

	for ( j=0; j<NumberOfSteps; j++ )
	{
		Step[j]->ProbCalc();
	}

	CalcTotalProbability();

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitRateConstants
//	Purpose: to initialize rate constants for all steps in process area
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void process_area :: InitRateConstants()

{
	UINT16 j;

	for ( j=0; j<NumberOfSteps; j++ )
	{
		Step[j]->InitializeRateCoefficients();
	}

	return;

}


//---------------------------------------------------------------------------