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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimwin.cxx                                                 */
/*                                                                          */
/*  This module  contains the functions for starting the simulation engine  */
/*  under MS Windows                                                        */
/*                                                                          */
/*  Version 1.0  started September 16 1993                                  */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*  This module contains significant OS-specific code, since the mechanisms */
/*  for starting and controlling child sessions differs significantly       */
/*  between operating systems                                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <windows.h>
#include <toolhelp.h>
#include <dos.h>
#include <time.h>

#define PAUSE_PERIOD   0.5

struct LOADPARMS
{
     WORD segEnv;                      /* child environment                   */

     LPSTR lpszCmdLine;                /* child command tail                  */
     LPWORD lpwShow;                   /* how to show child                   */
     LPWORD lpwReserved;               /* must be NULL                        */
};

typedef const void near *msimHANDLE;
// returns true is succesful. false otherwise

HINSTANCE msimRunSimulation( LPSTR SimulatorFilename, LPSTR RunParmFileName,
               LPSTR ParentName )
{
     struct LOADPARMS lp;
     WORD show_array[2] =
     {
          2, SW_SHOWNORMAL
     }
     ;
     char name[120];

     int num_attempts = 10;
     unsigned int rc;
     extern void msimReschedule( void );


     sprintf( name, " %s %s", RunParmFileName, ParentName );

     lp.segEnv = 0;
     lp.lpszCmdLine = ( LPSTR ) name;  /* child command tail                  */
     lp.lpwShow = ( LPWORD ) show_array;/* how to show child                  */
     lp.lpwReserved = ( LPWORD ) NULL; /* must be NULL                        */

     rc = LoadModule( SimulatorFilename, &lp );

     while ( rc < 32 && num_attempts-- )// try 10 times to load module
     {
          // wait a bit, try again, 10 times total

          time_t first = time( NULL );

          while ( difftime( time( NULL ), first ) < PAUSE_PERIOD )
               msimReschedule( );


          rc = LoadModule( SimulatorFilename, &lp );
     }

     return rc;


}


// Turn hInstance into hTask:  walk the task list, look for hInstance's hTask

WORD hTaskFromhInstance( WORD hInstance )
{
     TASKENTRY te;

     te.dwSize = sizeof ( TASKENTRY ); // initialize

     if ( TaskFirst( &te ) )
     {

          if ( te.hInst == hInstance )
               return te.hTask;

          while ( TaskNext( &te ) )
          {
               if ( te.hInst == hInstance )
                    return te.hTask;
          }
     }

     return 0;                         // if unable to find hInstance

}



// hInstance here is the HINSTANCE values returned by the Win3 fcn LoadModule
BOOL msimStopSession( const void near *hInstance )
{

     HTASK myTask = hTaskFromhInstance(( HINSTANCE ) hInstance );

     if ( myTask )
     {
          TerminateApp( myTask, NO_UAE_BOX );
          return TRUE;
     }
     return FALSE;
}



