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
//      Declaration of class REACTION_TRANSFER_K
//	   This object is a derived rate constant object. It handles generic
//		reaction and transfer rate constants
//      Date last modified: January 18, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef R_X_K_HXX
#define R_X_K_HXX

#include <math.h>
#include "vsimtype.hxx"
#include "vsimcons.hxx"
#include "datastru.hxx"
#include "idtypes.hxx"
#include "k_info.hxx"


class reaction_transfer_k : public k_info {

protected:


public:
	reaction_transfer_k();
	reaction_transfer_k( const reaction_transfer_k& r_k );
	virtual ~reaction_transfer_k() {}

	FLOAT64		Get_k ( FLOAT64 T );
	void 		Update_k ( FLOAT64 SizeRatio );


     // the following are used to initialize the rate constant object
	void   	Initialize_k();

	friend TextInputStream& operator >> ( TextInputStream& rTIS, reaction_transfer_k& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, reaction_transfer_k& rObject );

#endif

