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
//	Methods for class UPDATE_TABLE
//	Date last modified: March 8, 1996
//	Written by Frances A. Houle
//	IBM  
//--------------------------------------------------------------------------

#include "update_t.hxx"
#include "simulatr.hxx"


//--------------------------------------------------------------------------
//	Constructor for update tables where n(rows) = n(columns)
//--------------------------------------------------------------------------

update_table :: update_table ( UINT32 nr )
: Table (nr,nr)

{
	UINT32 j;


	NoValuesInTableRow = new UINT32 [ nr ];

	if (NoValuesInTableRow == 0)
	{
		Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
	}

	for ( j = 0; j < nr; j++ )
	{
		NoValuesInTableRow[j] = 0;
	}

	return;

}


//--------------------------------------------------------------------------
//	Constructor for update tables where n(rows) .ne. n(columns)
//--------------------------------------------------------------------------

update_table :: update_table ( UINT32 nts, UINT32 nr )
: Table (nts, nr)

{
	UINT32 j;


	NoValuesInTableRow = new UINT32 [ nts ];

	if (NoValuesInTableRow == 0)
	{
		Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		return;
	}

	for ( j = 0; j < nts; j++ )
	{
		NoValuesInTableRow[j] = 0;
	}

	return;

}

//--------------------------------------------------------------------------
//	Destructor
//--------------------------------------------------------------------------

 update_table :: ~update_table ()

{

	delete [] (NoValuesInTableRow);

}

//--------------------------------------------------------------------------
