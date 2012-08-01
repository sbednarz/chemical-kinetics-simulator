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
//      Declaration of class VOL_UPDATE
//      derived from base class state_update
//      This object manages the process of updating compartment volumes
//		after an event selection
//      Date last modified: February 26, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------


#ifndef UD_VOL_HXX
#define UD_VOL_HXX

#include "ud_state.hxx"

class vol_update : public state_update {  // derived class

protected:

public:
	vol_update () {}
	vol_update ( const vol_update& v_u );
	~vol_update() {}
	void UpdateState ();

};

#endif
