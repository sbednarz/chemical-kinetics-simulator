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
//	Declaration of class RANDOMNUMBERSERIES
//	base class
//	Date last modified: November 7, 1995
//	Written by Bill Hinsberg and Frances Houle
//	IBM  
//
//--------------------------------------------------------------------------


#ifndef RANDOM_HXX
#define RANDOM_HXX

#include "vsimtype.hxx"
#include "txtstrem.hxx"


class RandomNumberSeries  {

protected :
      // actual values of seeds when saved will always fit into a
      // short int ( 2 bytes )
      // we defined them as long ints so that overflow does not
      // occur during mathematical manipulations in method NextValue()

     UINT32 internal_seed1;
     UINT32 internal_seed2;
     UINT32 external_seed;

public:
     RandomNumberSeries () {}
     RandomNumberSeries( const UINT16 ExternalSeed );
     RandomNumberSeries ( const RandomNumberSeries& rand );
     ~ RandomNumberSeries () {}
     FLOAT64 NextValue ();
     void ResetSeed (const UINT16 ExternalSeed);
     void GetState ( UINT16 SeedArray[] );
     void SetState ( UINT16 SeedArray[] );
     friend TextInputStream& operator >> ( TextInputStream& rTIS, RandomNumberSeries& rObject );

};


TextInputStream& operator >> ( TextInputStream& rTIS, RandomNumberSeries& rObject );

#endif

