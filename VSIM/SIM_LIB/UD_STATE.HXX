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
//      Declaration of class STATE_UPDATE
//      base class
//      This is a class to manage system updates of various variables
//      Date last modified: November 7. 1995
//      Written by: Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef UD_STATE_HXX
#define UD_STATE_HXX

#include "string.h"

class state_update {

protected:

public:
	state_update () {}
	state_update ( const state_update& s_u );
	~state_update () {}
	virtual void UpdateState () = 0;   //specific to process area and
					   //state variable
};

#endif

