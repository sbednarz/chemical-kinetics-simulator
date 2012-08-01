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

//---------------------------------------------------------------------------
//      Methods for class PROFILE
//      Date last modified January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//---------------------------------------------------------------------------


#include "profile.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

profile :: profile ()

{
	ProfileData = 0;
	NumberOfPoints = 0;
	Index = 0;
	JumpStart = TRUE;
	Terminated = FALSE;
	LastValue = 0.0;
	StatusInfo.NextProfilePoint = 0.0;
	StatusInfo.Slope = 0.0;
	StatusInfo.IdlingValueStep = 0.0;
	StatusInfo.MaxTimeStep = 0.0;
	StatusInfo.CurrentValueInProfile = 0.0;
	StatusInfo.CurrentTimeInProfile = 0.0;

	return;

}

//---------------------------------------------------------------------------
//	Description of method GetValueStep
//	Purpose: to retrieve the step in value in a profile interpolated using the current
//		simulation time.
//	Parameter: time
//	Returns: corresponding vale step
//---------------------------------------------------------------------------

FLOAT64 profile :: GetValueStep( FLOAT64 CurrentTime )

{
    FLOAT64	 InterpVal, DeltaValue;
 
	if ( CurrentTime == ProfileData[ Index ] )
	{    //value is one of the profile points
	     DeltaValue = ProfileData[ Index + 1 ] - StatusInfo.CurrentValueInProfile;
	}
	else
	{    // interpolate
	     InterpVal = ProfileData[ Index + 1 ] - StatusInfo.Slope * ( ProfileData[ Index ] -
			      CurrentTime );
	     DeltaValue = InterpVal - StatusInfo.CurrentValueInProfile;
	}  

      return DeltaValue;

}


//---------------------------------------------------------------------------
//	Description of method GetTimeStep
//	Purpose: to retrieve the current time step interpolated using the current value
//	Parameter: value
//	Returns: corresponding time step
//---------------------------------------------------------------------------

FLOAT64 profile :: GetTimeStep( FLOAT64 CurrentValue )

{
    FLOAT64	 InterpTime, DeltaTime;
 
	if ( CurrentValue == ProfileData[ Index + 1 ] )
	{    //Time is one of the profile points
	     DeltaTime = ProfileData[ Index ] - StatusInfo.CurrentTimeInProfile;
	}
	else
	{    // interpolate
	     InterpTime = ProfileData[ Index ] - StatusInfo.Slope * ( ProfileData[ Index + 1 ] -
			      CurrentValue );
	     DeltaTime = InterpTime - StatusInfo.CurrentTimeInProfile;
	} 

      return DeltaTime;

}

//---------------------------------------------------------------------------
//      Definition of method IncrementValue
//      Purpose: to advance position in profile when time step has been calculated
//      Parameters: TimeStep from timebase
//      Returns: GoIntoIdler flag
//---------------------------------------------------------------------------

BOOL8 profile :: IncrementValue( FLOAT64 TimeStep )
{
	BOOL8	GoIntoIdler;
	FLOAT64   DeltaValue, NewTime, dummy;
	UINT32	NewIndex;

     dummy = 0.0;
     GoIntoIdler = FALSE;

     if ( Terminated )
     {
	return GoIntoIdler;
     }

     // calculate new value from profile using the new time
     NewTime = Simulator->GetTime() + TimeStep;

     // check to see if following the profile needs to be jump started
     // (only used at the beginning of a simulation - flag reset in this
     // first pass). Jump starting causes the simulator to idle up through
     // the first segment to avoid skipping over it if initial probabilities
     // are small
     if ( JumpStart )
     {
	     if ( (NewTime > ProfileData[ Index ]) && (Index == 2) )
	     {
		     JumpStart = FALSE;
		     GoIntoIdler = TRUE;
		     return GoIntoIdler;
	     }

	     JumpStart = FALSE;

     }


     // if NewTime is out of current segment in profile
     if ( NewTime > ProfileData[ Index ] )
     {
	     NewIndex = Index + 2;     // even entries are always time

	     // test that new index is within profile
	     if ( NewIndex > ( NumberOfPoints - 2 ) )
	     {
		     Terminated = TRUE;
	     }

	// if NewTime is out of range of next segment too, send
	// simulation into idle mode to follow profile closely
	if ( (NewTime > ProfileData[ NewIndex ]) && !Terminated )
	{
	     GoIntoIdler = TRUE;
	     return GoIntoIdler;
	}

	// sets Index = NewIndex and updates status. Use dummy and
	// current time as arguments - NewTime only is used in move
	Terminated = MoveToNextSegment( dummy, NewTime );

     }  // end if new time

     if ( (!GoIntoIdler) && (!Terminated) )
     {
	// determine value change in profile
	     DeltaValue = GetValueStep( NewTime );
     }

     // now see if value change jump is of the right size to continue, and check
     // whether flag is sending simulation into idler

     if ( ( fabs( DeltaValue ) > MaxValueStep ) )
     {
	GoIntoIdler = TRUE;
	return GoIntoIdler;
     }
     else
     {
	// save place in program file
	StatusInfo.CurrentValueInProfile += DeltaValue;
	StatusInfo.CurrentTimeInProfile = NewTime;

     } // end if fabs DeltaTemp

     return GoIntoIdler;

}


//---------------------------------------------------------------------------
//      Definition of method IncrementValue
//      Purpose: to advance position in profile during idling
//      Parameters: none
//      Returns: Terminated
//---------------------------------------------------------------------------

BOOL8 profile :: IncrementValue()
{
     FLOAT64   dummy = 0.0;

     if ( StatusInfo.Slope > 0 )
     {
          StatusInfo.CurrentValueInProfile += StatusInfo.IdlingValueStep;
          if ( StatusInfo.CurrentValueInProfile > StatusInfo.NextProfilePoint )
          {
               Terminated = MoveToNextSegment( StatusInfo.CurrentValueInProfile, dummy );
          }
     }
     else
     {
          StatusInfo.CurrentValueInProfile -= StatusInfo.IdlingValueStep;
          if ( StatusInfo.CurrentValueInProfile < StatusInfo.NextProfilePoint )
          {
               Terminated = MoveToNextSegment( StatusInfo.CurrentValueInProfile, dummy );
          }     }

     return Terminated;
}

//----------------------------------------------------------------------------
//	Definition of method MoveToNextSegment
//	Purpose: moves to next segment, then update StatusInfo data structure
//	Parameters: none
//	Returns: nothing
//----------------------------------------------------------------------------

BOOL8 profile :: MoveToNextSegment( FLOAT64 NewValue, FLOAT64 NewTime )
{

     FLOAT64 DeltaValue, DeltaTime, InterpVal, BiggestStep;

	// move to next segment, update following quantities
	// StatusInfo.NextProfilePoint (value)
	// MaxValueStep
	// StatusInfo.Slope
	// StatusInfo.IdlingValueStep
	// StatusInfo.MaxTimeStep
	// StatusInfo.CurrentValueInProfile
	// StatusInfo.CurrentTimeInProfile

	// if NewValue is nonzero, then use it to set up StatusInfo. If NewTime is nonzero, then
	// use it to set up StatusInfo

	Index += 2;

	// test that new index is within profile
	if ( Index > ( NumberOfPoints - 2 ) )
	{
		Terminated = TRUE;
		return Terminated;
	}

	StatusInfo.NextProfilePoint = ProfileData[ Index + 1 ];

     // find slope in new profile segment
     DeltaValue = ProfileData[ Index + 1 ] - ProfileData[ Index - 1 ];
     DeltaTime = ProfileData[ Index ]  -  ProfileData [ Index - 2 ];
     StatusInfo.Slope = DeltaValue / DeltaTime;

	// if NewValue is nonzero, use it to find new time, and set current position in profile
	// if NewTime is nonzero, use it to find new value, and set current position in profile

	if ( NewValue )
	{
		NewTime = GetTimeStep( NewValue );

	}
	else // NewTime > 0.0
	{
		NewTime = GetTimeStep( NewValue );
	}

	StatusInfo.CurrentValueInProfile = NewValue;
	StatusInfo.CurrentTimeInProfile = NewTime;

	// find max time and value steps for segment and idling step size
	if ( StatusInfo.Slope )
	{
	    // find max value step within segment and calculate idling step size for value
	    // take max value step to be 5% of the larger value or full segment step, whichever is
	    // smaller
	    // take idling value step to be 1% of the max step
	    // take max time step to be same proportion of segment as max value step

	    if ( DeltaValue > 0.0 ) // increasing
         {
     	    MaxValueStep = ProfileData[ Index + 1 ] * 0.02;
              BiggestStep = fabs( ProfileData[ Index + 1 ] - StatusInfo.CurrentValueInProfile );

          } else {   // decreasing
     	    MaxValueStep = ProfileData[ Index - 1 ] * 0.02;
              BiggestStep = fabs( ProfileData[ Index - 1 ] - StatusInfo.CurrentValueInProfile );

          }

    	     if ( MaxValueStep > BiggestStep ) MaxValueStep = BiggestStep;
          StatusInfo.IdlingValueStep = MaxValueStep * 0.01;

          StatusInfo.MaxTimeStep = DeltaTime * ( MaxValueStep / DeltaValue );
     }
     else
     {
     	// Slope is 0.0, value will not increase so set steps to max within segment
	     StatusInfo.MaxTimeStep = ProfileData[ Index ] - StatusInfo.CurrentTimeInProfile;
     	MaxValueStep = fabs( StatusInfo.NextProfilePoint - StatusInfo.CurrentValueInProfile );
	     StatusInfo.IdlingValueStep = MaxValueStep;

     }

     return Terminated;

}


//---------------------------------------------------------------------------
//   Definition of method AdjustUnits
//   Purpose: to scale profile by a constant if units conversion factor(s)
//        vary during a simulation
//   Parameters: constant for scaling
//   Returns: nothing
//---------------------------------------------------------------------------

void profile :: AdjustUnits( FLOAT64 Constant )
{
     UINT32 j;

     for ( j=1; j < NumberOfPoints; j += 2 )
     {
          ProfileData[ j ] *= Constant;
     }

     return;

}


//---------------------------------------------------------------------------
//	Description of method InitializeProfile
//	Purpose: to scale profile values (units correction) and set up initial
//		status
//	Parameter: scaling factor
//	Returns: nothing
//---------------------------------------------------------------------------

void profile :: InitializeProfile( FLOAT64 cons )

{
     FLOAT64   dummy = 0.0;

	AdjustUnits( cons );
	LastValue = ProfileData[ NumberOfPoints - 1 ];
	Terminated = MoveToNextSegment( ProfileData[ 1 ], dummy );

	return;
}


//-----------------------------------------------------------------------------
//	Definition of input operator >>
//	Purpose: to read in programmed variable profile.
//	Parameters: file name and object pointer
//	Returns: file pointer
//-----------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, profile& rObject )

{
	UINT32 j;

	while ( DataCode != ( IDType( END_PROFILE_INIT ) ) )
	{
		rTIS >> DataCode;

		switch ( DataCode ) {

		case NO_PROFILE_POINTS:
			rTIS >> rObject.NumberOfPoints;
			rObject.NumberOfPoints *= 2;

			// allocate array for storage of profile
			rObject.ProfileData = new FLOAT64 [ rObject.NumberOfPoints ];

               if ( rObject.ProfileData == 0 )
               {
                    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
                    return rTIS;
               }

			for ( j=0; j<rObject.NumberOfPoints; j++ )
			{
				rTIS >> DataCode;
				ASSERT ( DataCode == IDType ( PROFILE_DATA_PAIR ) );

				rTIS >> rObject.ProfileData[ j ];
				j++;
				rTIS >> rObject.ProfileData[ j ];
			}
			break;

		default:
			if ( ( DataCode != ( IDType( END_RXN_RATE_CONSTANT_INIT ) ) ) ||
			     ( DataCode != ( IDType( END_XFER_RATE_CONSTANT_INIT ) ) ) )
    			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
			}
			break;
		}// end switch

	} // end while
	return rTIS;

}
