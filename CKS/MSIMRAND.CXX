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

#include <iostream.h>
#include <math.h>
#include <limits.h>

#include <time.h>


#define TIMING
#define TIMING_PERIOD 10000000

signed short RandomNoSeed[3];

const unsigned long MAX_COUNT =  ULONG_MAX;
const unsigned long PRINT_INTERVAL = 10000;

const long int SEED_1 = 30873;
const long int SEED_2 = 28541;


class RandomNumberSeries
{

protected :
     long int internal_seed1;
     long int internal_seed2;
     long int external_seed;


public:
     RandomNumberSeries( const long int ExternalSeed );

     double NextValue( );

     void ResetSeed( long int ExternalSeed )
     {
          external_seed = ExternalSeed;
     }
};


RandomNumberSeries::RandomNumberSeries( const long int ExternalSeed = 1 )
{
     internal_seed1 = SEED_1;
     internal_seed2 = SEED_2;
     external_seed  = ExternalSeed;
}



double RandomNumberSeries::NextValue()
{

#if defined (__MAC__)
     long double dummy;
#else
     double dummy;
#endif

    // recalc all three internal random numbers

     external_seed = ( 171 * external_seed ) % 30269;
     internal_seed1 = ( 172 * internal_seed1 ) % 30307;
     internal_seed2 = ( 170 * internal_seed2 ) % 30323;

     return ( modf( external_seed / 30269.0 + internal_seed1 / 30307.0
                    + internal_seed2 / 30323.0, &dummy ));

}







/* +---------------------------------------------------------------------+
   |  DEFINITION : random number generator function                      |
   |  This random number generator was taken from the PASCAL version     |
   |  published in BYTE March 1987.                                      |
   |  Function uses the global array RandomNoSeed which was initialized  |
   |  in the  ReadInputData function. The function returns a             |
   |  pseudo-random number between 0 and 1.0                             |
   |  Output is identical to that in the Turbo BASIC version of Simulate |
   +---------------------------------------------------------------------+  */

double RandomNoGen( void )
{                                      /* start of function RandomNoGen       */
     double x;
#if defined (__MAC__)
     long double dummy;
#else
     double dummy;
#endif
     signed short DivQuot, DivRem;

     DivQuot = RandomNoSeed[0] / 177;
     DivRem = RandomNoSeed[0] % 177;
     RandomNoSeed[0] = 171 * DivRem - 2 * DivQuot;
     if ( RandomNoSeed[0] < 0 )
     {
          RandomNoSeed[0] += 30269;
     }

     DivQuot = RandomNoSeed[1] / 176;
     DivRem = RandomNoSeed[1] % 176;
     RandomNoSeed[1] = 172 * DivRem - 35 * DivQuot;
     if ( RandomNoSeed[1] < 0 )
     {
          RandomNoSeed[1] += 30307;
     }

     DivQuot = RandomNoSeed[2] / 178;
     DivRem = RandomNoSeed[2] % 178;
     RandomNoSeed[2] = 170 * DivRem - 63 * DivQuot;
     if ( RandomNoSeed[2] < 0 )
     {
          RandomNoSeed[2] += 30323;
     }

     x = RandomNoSeed[0] / 30269.0 + RandomNoSeed[1] / 30307.0 + RandomNoSeed[2] /
     30323.0;
     return ( modf( x, &dummy ) );
}                                      /* end of function RandomNoGen         */

void main()
{

    RandomNoSeed[0] = 2300;
    RandomNoSeed[1] = SEED_1;
    RandomNoSeed[2] = SEED_2;

    double new_rn;
    double old_rn;

    RandomNumberSeries rn( RandomNoSeed[0] );


    unsigned long count = 0;
    unsigned long printcount = 0;

#if defined(TIMING)
    char ExecutionTime[80];

     time_t StartWallClockTime, current_wallclock_time;

     time( &StartWallClockTime );

     while (count < TIMING_PERIOD )
     {
//       new_rn = rn.NextValue();
       old_rn = RandomNoGen();
       count++;
     }

     time( &current_wallclock_time );

     cout << count << " iterations in " << difftime( current_wallclock_time, StartWallClockTime )  <<  " seconds\n";



#else


    while (count < MAX_COUNT)
    {
       new_rn = rn.NextValue();
       old_rn = RandomNoGen();
       count++;
       printcount++;

       if ( new_rn != old_rn )
       {
          cout << "Inequality after " << count << " iterations; new = " << new_rn << ", old = " << old_rn ;
          return;
       }
       else
       {
             if (printcount >= PRINT_INTERVAL)
             {
                  cout << count << " iterations; new = " << new_rn << ", old = " << old_rn << "\n" ;
                  printcount = 0;
             }
       }
    }
    cout << count << " iterations; new = " << new_rn << ", old = " << old_rn << "\n" ;
#endif

}


