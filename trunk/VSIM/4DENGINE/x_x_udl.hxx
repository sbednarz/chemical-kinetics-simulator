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
//     TRANSFER_AFTER_TRANSFER_UPDATE_LIST
//     Update list class for updating affected transfer steps after an occurrence
//		of a transfer step in a transfer path connected to the source
//	        and target compartments
//     Derived from base class update_list
//     Date last modified: February 13, 1996
//     Written by Frances Houle
//     IBM  
//--------------------------------------------------------------------------


#ifndef X_X_UDL_HXX
#define X_X_UDL_HXX

#include "update_l.hxx"
#include "massxfer.hxx"
#include "xferpath.hxx"

class transfer_path;


class xfr_after_xfr_update_list : public update_list  { //derived class


protected:
	transfer_path**     RecalcTotalProbList;
	UINT16  	      	NoToRecalc;

public:
	xfr_after_xfr_update_list ();
	xfr_after_xfr_update_list ( const xfr_after_xfr_update_list& x_x_u_l );
	virtual ~xfr_after_xfr_update_list ();

	void                Build ( reaction_compartment* SourceCmpt, reaction_compartment* TargetCmpt,
                              mass_transfer* TransferStep, UINT16 TransferPathNo );
	UINT16	          GetNoToRecalc() { return NoToRecalc; }
	transfer_path*	     GetPathNoToRecalc ( UINT16 n ) { return RecalcTotalProbList[ n ]; }

};

#endif