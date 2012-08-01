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

//------------------------------------------------------------------------
//
//	Physical and defined constants for calculations
//	Date last modified: January 23, 1996
//	Written by Frances A. Houle
//	IBM  
//
//------------------------------------------------------------------------

#ifndef VSIMCONS_HXX
#define VSIMCONS_HXX

#include "vsimtype.hxx"

#define  STANDARD_VOLUME                     ((double) 1.0)
#define  STANDARD_DEPTH                      ((double) 1.0)
#define  GAS_CONSTANT                            0.08205
#define  R                                       0.00198726
#define  EPSILON                                 ( (double) 0.0001 )
#define  DBL_MIN_10_EXP				-308
#define  MINIMUM_EXPONENT                        ( ((DBL_MIN_10_EXP+100) * \
	     2.302585093) )

const UINT32 SEED_1 = 30873;
const UINT32 SEED_2 = 28541;

#endif


//------------------------------------------------------------------------
