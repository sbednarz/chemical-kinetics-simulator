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
//      Methods for class RANDOMNUMBERSERIES
//      Date last modified: July 24, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include <math.h>
#include "vsimtype.hxx"
#include "random.hxx"
#include "vsimcons.hxx"



//--------------------------------------------------------------------------
//    Definition of constructor for initializing with specific values
//--------------------------------------------------------------------------

RandomNumberSeries::RandomNumberSeries( const UINT16 ExternalSeed = 1 )
{
     internal_seed1 = SEED_1;
     internal_seed2 = SEED_2;
     external_seed  = ExternalSeed;
}

//--------------------------------------------------------------------------
//     Definition of NextValue
//     Purpose: to calculate a random number
//     Returns: a random number between 0 and 1
//--------------------------------------------------------------------------
 FLOAT64 RandomNumberSeries::NextValue()

{
	FLOAT64 dummy;

     // recalc all three internal random numbers
      external_seed = ( 171 * external_seed ) % 30269;
      internal_seed1 = ( 172 * internal_seed1 ) % 30307;
      internal_seed2 = ( 170 * internal_seed2 ) % 30323;
      return ( modf( external_seed / 30269.0 + internal_seed1 / 30307.0
		    + internal_seed2 / 30323.0, &dummy ));
}

//--------------------------------------------------------------------------
//     Definition of ResetSeed - used to set values for seeds when constructor
//        is called before the initial external seed is read in
//--------------------------------------------------------------------------
void RandomNumberSeries :: ResetSeed (const UINT16 ExternalSeed )
{
	const UINT32 SEED_1 = 30873;
	const UINT32 SEED_2 = 28541;

	external_seed = ExternalSeed;
	internal_seed1 = SEED_1;
	internal_seed2 = SEED_2;
}

//--------------------------------------------------------------------------
//     Definition of GetState
//     Purpose: to retrieve seeds for random number calculation
//--------------------------------------------------------------------------
void RandomNumberSeries :: GetState ( UINT16 SeedArray[] )
     {
	  SeedArray[0] = external_seed;
	  SeedArray[1] = internal_seed1;
	  SeedArray[2] = internal_seed2;
     }

//--------------------------------------------------------------------------
//     Definition of SetState
//     Purpose: gets seed array to save seed values
//--------------------------------------------------------------------------
void RandomNumberSeries :: SetState( UINT16 SeedArray[] )
     {
	  external_seed = SeedArray[0];
	  internal_seed1 = SeedArray[1];
	  internal_seed2 = SeedArray[2];
     }


//--------------------------------------------------------------------------
//   Definition of operator >>
//   Purpose: to read in random number seed for initializing generator. Also
//       copies preset seed values
//   Parameters: input file name and random number object address
//   Returns: nothing
//--------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, RandomNumberSeries& rObject )

{
     rTIS >> rObject.external_seed;

     rObject.internal_seed1 = SEED_1;
     rObject.internal_seed2 = SEED_2;

     return rTIS;

}

//--------------------------------------------------------------------------

