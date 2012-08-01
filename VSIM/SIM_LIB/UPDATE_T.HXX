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
//      Declaration of class UPDATE_TABLE
//      Base class
//      This object contains a matrix object and an array to list
//		and retrieve the indices of chemical reaction steps
//		which must be updated after event selection for
//              simulations where T and V do not vary
//      Date last modified: February 13, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef UPDATE_T_HXX
#define UPDATE_T_HXX

#include "matrix.hxx"
#include "vsimtype.hxx"
#include "vsimglob.hxx"


class update_table {   //  base class

protected:
	UINT32 *NoValuesInTableRow;
	matrix Table;

public:
     update_table () {}
	update_table (UINT32 nr); // n=number of reactions for compartment
	update_table (UINT32 nts, UINT32 nr);
	update_table (const update_table& u_t);
	virtual ~update_table ();
	UINT32 GetRowLength (UINT32 jr) {return NoValuesInTableRow[jr]; }
	void IncRowLength (UINT32 jr) { NoValuesInTableRow[jr] ++; }
	UINT32 GetStepNumber (UINT32 jr, UINT32 jc) {return Table.ElementValue (jr,jc);}
};

#endif
