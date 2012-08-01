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
// XFR_UD_T.CXX
// Methods for derived class transfer_update_table
// Date last modified: July 11, 1995
// Written by Frances Houle
// IBM  
//
//--------------------------------------------------------------------------

#include "xfr_ud_t.hxx"
#include "datastru.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

transfer_update_table :: transfer_update_table (UINT32 NumberOfTransferSteps, UINT32 NumberOfReactions)
: update_table (NumberOfTransferSteps, NumberOfReactions)
{
	;
}


//--------------------------------------------------------------------------
//Definition of method BuildTable
//Purpose: to create reaction table for expediting probability updates
//Parameters: none
//Returns: nothing
//--------------------------------------------------------------------------

void transfer_update_table :: BuildTable (UINT16 TransferPathNo, UINT16 ProcessAreaNo, UINT32 NumberOfReactions)

{                                      // begin
     BOOL8 FoundMatch;
     UINT32 jc, jr, TransferStepNo, ComparisonStepNo, nts, n1, n2;
     process_info StepData, ComparisonStepData;

      // initialize flag
     FoundMatch = FALSE;

     // Set up lookup table for updating probabilities after SelectEvent returns a mass
     // transfer step. Go through each of the mass transfer steps in a transfer path and
     // compare them to the reactants required for a reaction step in the compartment
     // this build method is to be called from within the compartment during its initialization

     jr = jc = 0;    //jr = row count, jc = column count for TransferStepNo
     TransferStepNo = 0;
     nts = Simulator.GetNoStepsInTransferPath ( TransferPathNo );

     while ( TransferStepNo < nts )
     {
	jr = TransferStepNo;
	jc = GetRowLength ( jr );
	ComparisonStepNo = 0;

	if ( TransferStepNo >= nts) { break; }
	StepData = Simulator.GetStepDataFromTransferPath (TransferPathNo, TransferStepNo);


	  // compare to other reactions
	  while ( ComparisonStepNo < NumberOfReactions )      // step through reaction list
	  {

	       if ( ComparisonStepNo >= NumberOfReactions ) { break; }
	       ComparisonStepData = Simulator.GetStepDataFromCompartment (ProcessAreaNo, ComparisonStepNo);
	       // search for matches
	       // n1 = index for TransferStepNo's reactant stoichiometry
	       // n2 = index for stoichiometry of comparison step

	       for ( n1 = 0; n1 < StepData.numreactants; n1++ )
	       {

		    for ( n2 = 0; n2 < ComparisonStepData.numreactants; n2++ )
		    {
			 FoundMatch = FALSE;
			 if ( ( StepData.ReactantsInStep[n1] ) ==
				( ComparisonStepData.ReactantsInStep[n2] )      )
			 {
			      FoundMatch = TRUE;
			      //store in ReactionStepNo row
			      Table.SetValue ( ComparisonStepNo, jr, jc );
			      IncRowLength( jr );
			      //increment counter for column in ReactionStepNo row
			      jc++;
			 }             // end if
			 if ( FoundMatch )
			      { break; }
		    }                  // endfor n2
		    if ( FoundMatch )
			 { break; }
	       }                       // endfor n1
	       ComparisonStepNo ++;
	  }                            // endwhile ComparisonStepNo

       TransferStepNo++;
      }    // end while TransferStepNo
}     // end method


//--------------------------------------------------------------------------
