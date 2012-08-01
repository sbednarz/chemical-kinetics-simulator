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
//      Declaration of class PROB_UPDATE
//      derived from state_update
//      This class manages updating and resetting of system during an
//		equil detect/emulation cycle
//      Date last modified: May 3, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef UD_EQUIL_HXX
#define UD_EQUIL_HXX

#include "ud_state.hxx"

class equil_emul_update : public state_update {  // derived class

protected:

public:
	equil_emul_update () {}
	equil_emul_update ( const equil_emul_update& e_e_u );
	~equil_emul_update() {}
	void UpdateState ();

};

#endif
