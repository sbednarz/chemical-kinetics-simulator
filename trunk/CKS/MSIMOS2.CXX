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
/*  FILE NAME : msimos2.cxx                                                 */
/*                                                                          */
/*  This module  process control functions specific to OS/2                 */
/*                                                                          */
/*  Version 1.0  started August 16 1993                                     */
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


#define INCL_DOSSESMGR
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_DOSQUEUES
#include <os2.h>

#include <string.h>
#include "msim1.hxx"

#define QUEUE_NAME                      "\\QUEUES\\MSIMQ"
#define NUM_ATTEMPTS_TO_OPEN_QUEUE      10

static HQUEUE QueueHandle;
static msimFILE_STRING QueueName;

ULONG msimStopSession( ULONG Session )
{
     return DosStopSession( STOP_SESSION_SPECIFIED, Session );
}



ULONG msimRunSimulation( PULONG pSessionID, PCHAR SimulatorFilename, PCHAR RunparmFilename, PCHAR SimOwnerName )
{
     /* start the simulation as a separate session */

     STARTDATA StartData;              /* Start session data structure        */
     PID pid;                          /* Process ID (returned)               */
     UCHAR PgmTitle[40];               /* Program title string                */
     UCHAR ObjBuf[100];                /* Object buffer                       */
     UCHAR simulator_parms[256];
     APIRET rc;

     sprintf( simulator_parms, "%s %s", RunparmFilename, SimOwnerName );
     /* save pointer in location accesible to all functions in module */

     /* make sure there is nothing in queue */
     DosPurgeQueue( QueueHandle );

     strcpy( PgmTitle, "MSIM - Simulation Engine" );

     /* Specify the various session start parameters  */

     StartData.Length = sizeof ( STARTDATA );/* Length of STARTDATA structure */
     StartData.Related = SSF_RELATED_CHILD;/* Child session                   */
     StartData.FgBg = SSF_FGBG_BACK;   /* Start child session in background   */
     StartData.TraceOpt = SSF_TRACEOPT_NONE;/* Don't trace session            */
     StartData.PgmTitle = PgmTitle;    /* Session Title string                */
     StartData.PgmName = SimulatorFilename;/* Program path-name string        */
     StartData.PgmInputs = simulator_parms;/* input arguments passed to the prog*/
     StartData.TermQ = QueueName;      /* point to queue name                 */
     StartData.Environment = 0;        /* no environment string               */
     StartData.InheritOpt = SSF_INHERTOPT_PARENT;/* Inherit environment and open file handles from parent*/
     StartData.SessionType = SSF_TYPE_DEFAULT;/* Allow the Shell to establish the session type*/
     StartData.IconFile = 0;
     StartData.PgmHandle = 0;
     StartData.PgmControl = SSF_CONTROL_VISIBLE | SSF_CONTROL_MAXIMIZE;/* Start the program as visible  and maximized*/
     StartData.InitXPos = 30;          /* Initial window coordinates  and size*/
     StartData.InitYPos = 40;
     StartData.InitXSize = 200;
     StartData.InitYSize = 140;
     StartData.Reserved = 0;           /* Reserved, must be zero              */
     StartData.ObjectBuffer = ObjBuf; /* buffer to hold DosExecPgm failure msg*/
     StartData.ObjectBuffLen = 100;

     /* ok ready to go */
     if ( rc = DosStartSession( &StartData, pSessionID, &pid ) )
          return rc;

     return 0;
}




// returns 0 if no error

ULONG msimWaitForSimCompletion( void )
{
     APIRET rc;
     REQUESTDATA queue_request_data;
     ULONG queue_data_length;
     PVOID queue_data_address;
     BYTE queue_element_priority;
     HEV queue_sem_handle;          // value is ignored when parm 6 == DCWW_WAIT
     PTIB p_thread_info_block;
     PPIB p_process_info_block;

     /* get our own process id, which we need to pass to DosReadQueue */

     DosGetInfoBlocks( &p_thread_info_block, &p_process_info_block );

     queue_request_data.pid = p_process_info_block -> pib_ulpid;

     /* now wait for termination message in the queue from child  session */
     /* this will wait for a message to come signifying completion of */
     /* simulation */

     if ( rc = DosReadQueue( QueueHandle, &queue_request_data, &queue_data_length,
                    &queue_data_address, 0, DCWW_WAIT,
                    &queue_element_priority, queue_sem_handle )
          )
          return rc;


     /* according to OS/2 progr docs we need to free the memory allocated */
     /* to the queue element after reading it */

     if ( rc = DosFreeMem( queue_data_address ) )
          return rc;
     return 0;
}


/*--------------------------------------------------------------------------*/
/*                        msimCreateQueue()                                 */
/*..........................................................................*/
/*                                                                          */
/* calls  OS/2 functions to create a queue.                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
msimRC msimCreateQueue( void )
{
     int i = 1;
     APIRET rc;

     /* create a queue for notification of child session completion */

     /* build queue name by appending numeral to base name */

     do
     {
          sprintf( QueueName, "%s%d", QUEUE_NAME, i++ );
          rc = DosCreateQueue( &QueueHandle, QUE_FIFO | QUE_NOCONVERT_ADDRESS,
               QueueName );
          if ( rc == ( APIRET ) 0 )
               return msimNO_ERROR;
     }
     while ( i <= NUM_ATTEMPTS_TO_OPEN_QUEUE );

     return msimQUEUE_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        msimCloseQueue()                                  */
/*..........................................................................*/
/*                                                                          */
/* calls  OS/2 functions to close  a queue.                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
msimRC msimCloseQueue( void )
{

     if ( DosCloseQueue( QueueHandle ) )
          return msimQUEUE_ERROR;
     else
          return msimNO_ERROR;
}



