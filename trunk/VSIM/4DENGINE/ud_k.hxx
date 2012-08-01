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

//-----------------------------------------------------------------------------
//
//      Declaration of class RATE_CONSTANT_UPDATE
//      derived from base class state_update
//      This object manages the process of updating rate constants for process areas
//             when T/P/V are variable
//      Date last modified: September 27, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------


#ifndef UD_K_HXX
#define UD_K_HXX

#include "ud_state.hxx"

class rate_constant_update : public state_update {  // derived class

protected:

public:
	rate_constant_update () {}
	rate_constant_update ( const rate_constant_update& r_c_u );
	~rate_constant_update() {}
	void UpdateState ();

};

#endif
