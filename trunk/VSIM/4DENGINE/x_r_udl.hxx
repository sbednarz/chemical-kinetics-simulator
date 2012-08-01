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
//     TRANSFER_AFTER_REACTION_UPDATE_LIST
//     Update list class for updating affected transfer steps after an occurrence
//		of a reaction step in a connected compartment
//     Derived from base class update_list
//     Date last modified: February 23, 1996
//     Written by Frances Houle
//     IBM  
//--------------------------------------------------------------------------


#ifndef X_R_UDL_HXX
#define X_R_UDL_HXX

#include "update_l.hxx"
#include "xferpath.hxx"
#include "rxn_step.hxx"

class transfer_path;


class xfr_after_rxn_update_list : public update_list  { //derived class


protected:
     transfer_path**   	CalcTotalPathProbList;
     UINT16    		NoPaths;

public:
	xfr_after_rxn_update_list ();
	xfr_after_rxn_update_list ( const xfr_after_rxn_update_list& x_r_u_l );
	virtual ~xfr_after_rxn_update_list ();

	void           Build ( reaction_step* RxnStep, reaction_compartment* CurrentCompartment,
				UINT16 NoTransferPaths, transfer_path** ConnectedTransferPath );
	UINT16         GetNoPaths() { return NoPaths; }
	transfer_path* GetPathNumber( UINT16 n ) { return CalcTotalPathProbList[ n ]; }

};

#endif