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
//      Declaration of class PROFILE
//      base class
//	This object is for storage and retrieval of values from a time profile
//		used during a simulation. Retrieval is by interpolation.
//		rate constants are single valued, and stored in the array in the
//		order time..rate..time..rate. Profile is assumed to be everywhere
//        >= 0.0
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef PROFILE_HXX
#define PROFILE_HXX

#include <math.h>
#include "vsimtype.hxx"
#include "idtypes.hxx"
#include "datastru.hxx"


class profile {

protected:
	FLOAT64*    	ProfileData;  // in order time..value..time..value
	UINT32	    	NumberOfPoints;
	UINT32	    	Index;
	BOOL8		JumpStart;
	BOOL8	    	Terminated;
	FLOAT64		LastValue;
	FLOAT64		MaxValueStep;
	profile_status	StatusInfo;

public:
	profile();
	profile( const profile& p );
	virtual ~profile() { delete [] ProfileData; }

	FLOAT64		     GetValueStep( FLOAT64 CurrentTime );
	FLOAT64		     GetTimeStep( FLOAT64 CurrentValue );
	BOOL8		     NeedsJumpStart() { return JumpStart; }
	BOOL8		     IsOver() { return Terminated; }
	BOOL8		     MoveToNextSegment( FLOAT64 NewValue, FLOAT64 NewTime);
	BOOL8		     IncrementValue( FLOAT64 TimeStep );
     BOOL8               IncrementValue();
	void		          AdjustUnits( FLOAT64 Constant );
	profile_status&	GetCurrentStatus() { return StatusInfo; }

     // the following is used to initialize the profile
	void   	InitializeProfile( FLOAT64 cons );

	friend TextInputStream& operator >> ( TextInputStream& rTIS, profile& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, profile& rObject );

#endif

