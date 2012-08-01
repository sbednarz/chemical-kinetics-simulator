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
// RXN_UD_T.CXX
// Methods for derived class reaction_update_table
// Date last modified: February 13, 1996
// Written by Frances Houle
// IBM  
//
//--------------------------------------------------------------------------

#include "rxn_ud_t.hxx"
#include "simulatr.hxx"

//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

reaction_update_table :: reaction_update_table (UINT32 NumberOfReactions)
: update_table (NumberOfReactions)
{
	pCompartment = 0;
}

//--------------------------------------------------------------------------




//--------------------------------------------------------------------------
//Definition of method BuildTable
//Purpose: to create reaction table for expediting probability updates
//Parameters: none
//Returns: nothing
//--------------------------------------------------------------------------

void reaction_update_table :: BuildTable ( reaction_compartment* p )

{                                      // begin
     BOOL8 FoundMatch;
     UINT32 jc, jr, ReactionStepNo, ComparisonStepNo, n1, n2, sr, sc, NumberOfReactions;
     process_info StepData, ComparisonStepData;

     // initialize pointer to compartment
     pCompartment = p;

      // initialize flag
     FoundMatch = FALSE;

     // Set up lookup table for updating probabilities after SelectEvent

     jr = jc = 0;    //jr = row count, jc = column count for ReactionStepNo
     ReactionStepNo = 0;

     NumberOfReactions = pCompartment->GetNoSteps();

     while ( ReactionStepNo < NumberOfReactions )
     {
        jr = ReactionStepNo;
	jc = GetRowLength ( jr );
	ComparisonStepNo = ReactionStepNo + 1;

	if ( ReactionStepNo >= NumberOfReactions ) { break; }
	StepData = pCompartment->GetStepInfo( ReactionStepNo );

	 // saves reaction's own index
	Table.SetValue ( ReactionStepNo, jr, jc );
	IncRowLength ( jr );
	jc ++;   //increment to next column in row

	  // compare to other reactions
	  while ( ComparisonStepNo < NumberOfReactions )      // step through reaction list
	  {

	       if ( ComparisonStepNo >= NumberOfReactions ) { break; }
	       ComparisonStepData = pCompartment->GetStepInfo( ComparisonStepNo );

	       // search for matches
	       // n1 = index for ReactionStepNo's reactant stoichiometry
	       // n2 = index for stoichiometry of comparison step
	       // sr, sc are the swapped row and column for symmetric table building
	       for ( n1 = 0; n1 < StepData.numreactants; n1++ )
	       {

		    for ( n2 = 0; n2 < ComparisonStepData.numreactants ; n2++ )
		    {
			 FoundMatch = FALSE;
			 if ( ( StepData.ReactantsInStep[n1] ) ==
				( ComparisonStepData.ReactantsInStep[n2] ) )
			 {
			      FoundMatch = TRUE;
			      //store in ReactionStepNo row
			      Table.SetValue ( ComparisonStepNo, jr, jc );
			      IncRowLength( jr );
			      //store also in Comparison Step row
			      sr = ComparisonStepNo;
			      sc = GetRowLength( sr );
			      Table.SetValue ( ReactionStepNo, sr, sc );
			      IncRowLength( sr );
			      //increment counter for column in ReactionStepNo row
			      jc++;
			 }             // end if
			 if ( FoundMatch )
			      break;
		    }                  // endfor n2
		    if ( FoundMatch )
			 break;
	       }                       // endfor n1
	       if ( ! FoundMatch )
	       {        	  // search product list

		  // do for ReactionStepNo products
		    for ( n1 = 0; n1 < ( StepData.numproducts ); n1++ )

		    {
			 for ( n2 = 0; n2 < ( ComparisonStepData.numreactants ); n2++ )
			 {
			      FoundMatch = FALSE;
			      if ( ( StepData.ProductsInStep[n1] ) ==
				     ( ComparisonStepData.ReactantsInStep[n2] ) )
			      {
				   FoundMatch = TRUE;
				   Table.SetValue ( ComparisonStepNo, jr, jc );
				   IncRowLength( jr );
				   //increment counter for column in ReactionStepNo row
				   jc++;		      }        ///
			      if ( FoundMatch )
				   break;
			 }             // end for n2
			 if ( FoundMatch )
			      break;
		    }                  // endfor n1

		    // now  do for Comparison Step products
		    for ( n1 = 0; n1 < ( ComparisonStepData.numproducts ); n1++ )

		    {
			 for ( n2 = 0; n2 < ( StepData.numreactants ); n2++ )
			 {
			      FoundMatch = FALSE;
			      if ( ( ComparisonStepData.ProductsInStep[n1] ) ==
				  ( StepData.ReactantsInStep[n2] ) )
			      {
				   FoundMatch = TRUE;
				   sr = ComparisonStepNo;
				   sc = GetRowLength( sr );
				   Table.SetValue ( ReactionStepNo, sr, sc );
				   IncRowLength( sr );
			      }        //  end if
			      if ( FoundMatch )
				   break;
			 }             // end for n2
			 if ( FoundMatch )
			      break;
		    }                  // endfor n1
	       }                       // endif not FoundMatch
	       ComparisonStepNo ++;
	  }                            // endwhile ComparisonStepNo
	  ReactionStepNo ++;
      }    // end while ReactionStepNo
}     // end method


//--------------------------------------------------------------------------
