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
//	Utilities functions for VSIM
//	Written by Frances Houle and Bill Hinsberg
//	Date last modified: June 14, 1995
//	IBM  
//
//--------------------------------------------------------------------------

#include <math.h>
#include "vsimutil.hxx"

//--------------------------------------------------------------------------
//	definition of function ROUND
//	Purpose: returns the properly rounded double precision value of
//		the argument - intended to handle arguments > 0.0
//	Parameters: a double precision real value
//	Returns: the rounded value as a long integer
//--------------------------------------------------------------------------

LargeUnsignedInt round ( FLOAT64 x )
{
	return (( LargeUnsignedInt) ( floor(x + 0.5) ));
}

//--------------------------------------------------------------------------

