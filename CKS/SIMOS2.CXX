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

/* []------------------------------------------------------------------------[]
   |                                                                          |
   |  ******************      IBM Internal Use Only      ******************   |
   |                                                                          |
   |    SIMOS2.CXX contains those functions in the MSIM simulation engine     |
   |    which call OS/2 control program functions. This stuff is split        |
   |    out from the actual calculation function so as to avoid clashes in    |
   |    names defined in the OS/2 and Starview header files                   |
   |                                                                          |
   []------------------------------------------------------------------------[] */

#include "msim1.hxx"

#define INCL_DOSPROCESS
#include <os2.h>
#include <process.h>


msimRC FAR GetReasonForExit( void );
int RescheduleFlag = 0;


void TimerLoop( void * )
{
     while ( GetReasonForExit( ) == msimNO_ERROR )
     {
          DosSleep( 100 );
          RescheduleFlag = 1;
     }

     _endthread( );
}

void StartRescheduleTimer( )
{
     _beginthread( TimerLoop, 32000, NULL );

}


void WaitForMSecs( ULONG Period )
{
     DosSleep( Period );
     return;
}


// ----------------------------------------------------------------------------
//          function ResetPriority()
// ............................................................................
//
// change history :
//        WDH 7/1/96 : changed parameter PriorityDelta from value 0 to value
//                     PRTYD_MAXIMUM to improved multitasking in Merlin
//                     fixes Merlin, no detectable detrimental effect on Warp
//
//
// ----------------------------------------------------------------------------

ULONG ResetPriority( )
{
     return DosSetPriority( PRTYS_PROCESS, PRTYC_IDLETIME, PRTYD_MAXIMUM,
          0 );
}




