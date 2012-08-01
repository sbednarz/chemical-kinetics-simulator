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
//     TRANSFER_UPDATE_TABLE
//     Update table class for updating a compartment's reaction step
//		probabilities after a mass transfer event in an attached
//		transfer path
//     Derived from base class update_table
//     Date last modified: October 30, 1995
//     Written by Frances Houle
//     IBM  
//--------------------------------------------------------------------------


#ifndef XFR_UD_T_HXX
#define XFR_UD_T_HXX

#include "update_t.hxx"

class transfer_update_table : public update_table   { //derived class


protected:

public:
	transfer_update_table ();
	transfer_update_table (UINT32 NumberOfTransferSteps, UINT32 NumberOfReactions);
	transfer_update_table ( const transfer_update_table& t_u_t );
	virtual ~transfer_update_table () {}
	void BuildTable (UINT16 TransferPathNo, UINT16 ProcessAreaNo, UINT32 NumberOfReactions);

};

#endif