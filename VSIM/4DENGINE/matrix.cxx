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
//      Methods for class MATRIX
//      Date last modified: March 7, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "matrix.hxx"
#include "simulatr.hxx"

//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------

matrix :: matrix ( UINT32 nr, UINT32 nc )

{
	UINT32 j;
	UINT32 n;

	no_rows = nr;
	no_columns = nc;
	n = nr * nc;
	Element = new UINT32 [ n ];

	if (Element == 0)
	{
		Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
	}

	for ( j = 0; j < n; j++ )
	{
		Element[j] = 0;
	}

	return;

}

//--------------------------------------------------------------------------
//      Definition of method InitMatrix
//	Purpose: to read a predetermined set of values into the matrix
//	Parameters: an existing matrix of the same # rows and columns
//	Returns: nothing
//--------------------------------------------------------------------------

void matrix :: InitMatrix ( matrix g )

{
	UINT32 nr, nc;

	for ( nr = 0; nr < no_rows; nr++ )
	{
		for ( nc = 0; nc < no_columns; nc++ )
		{
			SetValue ( g.ElementValue(nr,nc), nr, nc );
		} //end for nc
	} //end	for nr

	return;
}

//--------------------------------------------------------------------------





