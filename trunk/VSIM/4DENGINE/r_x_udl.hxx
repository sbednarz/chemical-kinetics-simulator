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
//     REACTION_AFTER_TRANSFER_UPDATE_LIST
//     Update list class for updating affected reaction steps after an occurrence
//		of a transfer step in a connected transfer path
//     Derived from base class update_list
//     Date last modified: February 9, 1996
//     Written by Frances Houle
//     IBM  
//--------------------------------------------------------------------------


#ifndef R_X_UDL_HXX
#define R_X_UDL_HXX

#include "update_l.hxx"
#include "cmprtmnt.hxx"
#include "massxfer.hxx"

class rxn_after_xfr_update_list : public update_list  { //derived class


protected:
     BOOL8     CalcSourceProb;
     BOOL8     CalcTargetProb;

public:
	rxn_after_xfr_update_list ();
	rxn_after_xfr_update_list ( const rxn_after_xfr_update_list& r_x_u_l );
	virtual ~rxn_after_xfr_update_list ();

	void      Build ( reaction_compartment* SourceCmpt, reaction_compartment* TargetCmpt, mass_transfer* TransferStep );
	BOOL8     CalcTotalSourceProb() { return CalcSourceProb; }
	BOOL8     CalcTotalTargetProb() { return CalcTargetProb; }

};

#endif