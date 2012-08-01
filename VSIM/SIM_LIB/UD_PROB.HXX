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
//      This class manages updating event probabilities after selection
//		of an event. Individual probabilities and total
//		probabilities are updated
//      Date last modified: November 7, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef UD_PROB_HXX
#define UD_PROB_HXX

#include "ud_state.hxx"

class prob_update : public state_update {  // derived class

protected:

public:
     prob_update () {}
	prob_update ( const prob_update& p_u );
	~prob_update() {}
	void UpdateState ();

};

#endif
