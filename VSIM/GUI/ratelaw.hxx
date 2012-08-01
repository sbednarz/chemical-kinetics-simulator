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

// ----------------------------------------------------------------------------
//  RATELAW.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SpecialRateLawDialog object, which allows
//  user netry of a non-stoiciometric rate law 
//
//  CREATED : 7.26.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if !defined(__RATELAW_HXX__)
#define __RATELAW_HXX__


#include "reaction.hxx"

enum RATE_LAW_RC  { RL_RC_FAIL, RL_RC_INIT_FAILURE, RL_RC_CANCEL, RL_RC_ACCEPT  };



RATE_LAW_RC EditRateLaw( Window* pParent, Reaction& rReaction );


#endif
