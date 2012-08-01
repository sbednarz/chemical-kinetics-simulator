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

// ----------------------------------------------------------------------------
//  CHILDPRC.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChildProcess object. 
//  this version of the file is specific to OS/2
//
//  CREATED : 5.05.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#pragma hdrstop

#define INCL_DOSSESMGR
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_DOSQUEUES
#include <svpm.h>

#include <string.h>
#include <process.h>
#include <time.h>

#include "assert.hxx"
#include "childprc.hxx"
#include "killproc.hxx"

// we include here declarations specific to the OS/2 implementation 

static const size_t THREAD_STACK_SIZE = 8192;
static const size_t BUFFER_SIZE       = 260;
static const int NUM_ATTEMPTS_TO_OPEN_QUEUE = 10;
static const String QUEUE_NAME_ROOT( "\\QUEUES\\VSIMQ");

static void Start( void * );





ChildProcess::ChildProcess() :
session_id( INVALID_SESSION_ID ),
executable_filename(),
parameters(),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
execution_time( 0 ),
event_on_complete( FALSE ),
event( 0 ),
pevent_data( NULL )
{
     SetStatus( Initialize() );
}


ChildProcess::ChildProcess( const String& ProgramName ) :
session_id( INVALID_SESSION_ID ),
executable_filename( ProgramName ),
parameters(),
status( CP_FAIL ),
wait( TRUE ), 
thread_running( FALSE ),
execution_time( 0 ),
event_on_complete( FALSE ),
event( 0 ),
pevent_data( NULL )
{
     SetStatus( Initialize() );
}


ChildProcess::ChildProcess( const String& ProgramName, 
 const String& CommandLineParms ) :
session_id( INVALID_SESSION_ID ),
executable_filename( ProgramName ),
parameters( CommandLineParms ),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
execution_time( 0 ),
event_on_complete( FALSE ),
event( 0 ),
pevent_data( NULL )
{
     SetStatus( Initialize() );
}



ChildProcess::~ChildProcess()
{
     if ( GetStatus() == CP_RUNNING )
          Abort();
}





CHILD_PRC_RC ChildProcess::Initialize( )
{
     return CP_READY;
}




BOOL8 ChildProcess::RunAndWait()
{
     wait = TRUE;

     Start( this );

     if ( CP_SUCCESS == GetStatus() )
     {
          SetStatus( CP_READY );
          return TRUE;
     }
     else
          return FALSE;
}




BOOL8 ChildProcess::RunWithoutWait()
{
      wait = FALSE;

     _beginthread( Start, THREAD_STACK_SIZE, this );

     if ( CP_SUCCESS == GetStatus() )
     {
          SetStatus( CP_READY );
          return TRUE;
     }
     else
          return FALSE;

}



void Start( void* pPtr )
{
     // start the simulation as a separate session 

     SESSION_ID    new_session_id;
     STARTDATA     start_data;              // Start session data structure
     PID           pid;                    // Process ID (returned)   
     APIRET        rc;
    
     // used for dealing with thread IPC via queue

     REQUESTDATA   queue_request_data;
     ULONG         queue_data_length;
     PVOID         queue_data_address;
     BYTE          queue_element_priority;
     HEV           queue_sem_handle;          // value is ignored when parm 6 == DCWW_WAIT
     PTIB          p_thread_info_block;
     PPIB          p_process_info_block;

     String        queue_name_str;
     HQUEUE        queue_handle;

     // char buffers

     UCHAR         prog_title[BUFFER_SIZE];               // Program title string 
     UCHAR         obj_buf[BUFFER_SIZE];                  // Object buffer
     UCHAR         command_line_parms[BUFFER_SIZE];
     UCHAR         exec_name[BUFFER_SIZE];
     UCHAR         queue_name[BUFFER_SIZE];

     // for determining execution time 

     Time start_wallclock_time;

     // ok, let's start 

     ChildProcess* p_object = ( ChildProcess* ) pPtr;  // for cleaner code 

     p_object->execution_time = 0;

     if ( FALSE == p_object->wait )
          p_object->thread_running = TRUE;


     BOOL8         no_errors_yet = TRUE;


     // don't start if process is already running

     if ( p_object->session_id != INVALID_SESSION_ID )
     {
          p_object->SetStatus( CP_ILLEGAL_OPERATION );
          no_errors_yet = FALSE;
     }
     

     if ( no_errors_yet )
     {

          // create a queue for notification of child session completion 
          // build queue name by appending numeral to base name 

          int i = 1;

          do
          {
               queue_name_str = QUEUE_NAME_ROOT + String( i );
               i++;
               rc = DosCreateQueue( &queue_handle, QUE_FIFO | QUE_NOCONVERT_ADDRESS,
                   (char *) queue_name_str.GetStr() );

               if ( rc == ( APIRET ) 0 )   // successfully created queue so continue 
                    break;
          }
          while ( i <= NUM_ATTEMPTS_TO_OPEN_QUEUE );

          // if we terminated the loop above with a nonzero rc then we did not make the 
          // queue so set status code to refelct that and exit 

          if ( rc != (APIRET) 0 )
          {
               p_object->SetStatus( CP_IPC_ERROR );
               no_errors_yet = FALSE;
          }
     }


     if ( no_errors_yet )
     {
          // fill static char buffers with parameters

          strcpy( command_line_parms, p_object->parameters.GetStr() );
          strcpy( prog_title, p_object->program_title );
          strcpy( exec_name, p_object->executable_filename );
          strcpy( queue_name, queue_name_str );


          // make sure there is nothing in queue 

          DosPurgeQueue( queue_handle );
      
          // fill in the data structure

          start_data.Length       = sizeof ( STARTDATA );  // Length of STARTDATA structure
          start_data.Related      = SSF_RELATED_CHILD;     // Child session
          start_data.FgBg         = SSF_FGBG_BACK;         // Start child session in background
          start_data.TraceOpt     = SSF_TRACEOPT_NONE;     // Don't trace session
          start_data.PgmTitle     = prog_title;            // Session Title string
          start_data.PgmName      = exec_name;             // Program path-name string
          start_data.PgmInputs    = command_line_parms;    // input arguments passed to the prog
          start_data.TermQ        = queue_name;            // point to queue name
          start_data.Environment  = 0;                     // no environment string
          start_data.InheritOpt   = SSF_INHERTOPT_PARENT;  // Inherit environment and open file handles from parent
          start_data.SessionType  = SSF_TYPE_DEFAULT;      // Allow the Shell to establish the session type
          start_data.IconFile     = 0;
          start_data.PgmHandle    = 0;
          start_data.PgmControl   = SSF_CONTROL_VISIBLE | 
           SSF_CONTROL_MAXIMIZE;                           // Start the program as visible  and maximized
          start_data.InitXPos     = 30;                     // Initial window coordinates  and size
          start_data.InitYPos     = 40;
          start_data.InitXSize    = 200;
          start_data.InitYSize    = 140;
          start_data.Reserved     = 0;                      // Reserved, must be zero
          start_data.ObjectBuffer = obj_buf;                // buffer to hold DosExecPgm failure msg
          start_data.ObjectBuffLen = BUFFER_SIZE;

          // OK, ready to go - record the time 

          Time start_wallclock_time; // creates Time object with current time 

          if ( 0 != DosStartSession( &start_data, &new_session_id, &pid ) )
          {
               // a nonzero rc means failure - set status and return 

               p_object->SetStatus( CP_FAIL );
               no_errors_yet = FALSE;
          }
          else
          {
               p_object->session_id = new_session_id;
               p_object->SetStatus( CP_RUNNING );
          }
     }


     if ( no_errors_yet )
     {

          // now wait for child process to end, which is signalled by it writing 
          // to the queue we just created 

          // first we get our own process id, which we need to pass to DosReadQueue

          DosGetInfoBlocks( &p_thread_info_block, &p_process_info_block );

          queue_request_data.pid = p_process_info_block -> pib_ulpid;

          // now wait for termination message in the queue from child  session 
          // this will wait for a message to come signifying completion of simulation 
          // DosReadQueue returns 0 upon success

          if ( 0 != DosReadQueue( queue_handle, &queue_request_data, &queue_data_length,
                    &queue_data_address, 0, DCWW_WAIT,
                    &queue_element_priority, queue_sem_handle )
               )
          {
               p_object->SetStatus( CP_IPC_ERROR );
               no_errors_yet = FALSE;
          }
     }


     if ( no_errors_yet )
     {
          p_object->session_id = INVALID_SESSION_ID;  // reset this to an invalid value 

          /* according to OS/2 progr docs we need to free the memory allocated */
          /* to the queue element after reading it */

          if ( 0 != DosFreeMem( queue_data_address ) )
          {
               p_object->SetStatus( CP_IPC_ERROR );
               no_errors_yet = FALSE;
          }

          if ( DosCloseQueue( queue_handle ) )
          {
               p_object->SetStatus( CP_IPC_ERROR );
               no_errors_yet = FALSE;
          }
     }


     if ( no_errors_yet )
     {

          p_object->SetStatus( CP_SUCCESS );

          // record the ending time 

          Time end_wallclock_time;

          p_object->execution_time =  end_wallclock_time - start_wallclock_time;

          if ( p_object->event_on_complete )
               GetpApp()->PostUserEvent( p_object->event, p_object->pevent_data );

          if ( FALSE == p_object->wait )
          {
               p_object->thread_running = FALSE;
               _endthread();
          }
     }
}




BOOL8  ChildProcess::DoesChildExist() const
{
     return DoesProcessExist( session_id );
}




BOOL8 ChildProcess::Abort()
{
     EVENT_TYPE old_event = event;

     if ( session_id != INVALID_SESSION_ID )
     {
          event = vEVT_UPDATE_AFTER_ABORT;

          if ( KillProcess( session_id ) )
          {
               // if thread is running then wait for it to end 

               if ( thread_running )
               {
                    while ( thread_running)
                       DosSleep( 100 );   // pause for 100 msec w/o using cpu cycles
               }

               session_id = INVALID_SESSION_ID;
               SetStatus( CP_READY );
               return TRUE;
          }
          else
          {
               event = old_event;

               SetStatus( CP_FAIL );
               return FALSE;
          }
     }
     else
     {
          SetStatus( CP_ILLEGAL_OPERATION );
          return FALSE;
     }
}



#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>
#include <dos.h>

void fcn( ChildProcess* pChild )
{
     cout << "Process ended as expected" << endl;
     cout << (const char *) pChild->GetProgramName() << endl;
     cout << (const char *) pChild->GetProgramTitle() << endl;
     cout << (const char *) pChild->GetCommandLineParameters( ) << endl;
}


void fcn2( ChildProcess* pChild )
{
     cout << "Process ended early!!" << endl;
     cout << (const char *) pChild->GetProgramName() << endl;
     cout << (const char *) pChild->GetProgramTitle() << endl;
     cout << (const char *) pChild->GetCommandLineParameters( ) << endl;
}


void main()
{
     ChildProcess  child;
     cout << "status = " << (int) child.GetStatus() << endl;

//   String name( "c:\\util\\t2.exe" );
     String name( "D:\\t2.exe" );
     String parms( "runparm.tst" );

     child.SetProgramName( name );
     child.SetCommandLineParameters( parms );
     child.SetProcessCompleteHandler( fcn );

     cout << "status = " << (int) child.GetStatus() << endl;

     cout << "RunAndWait" << endl;
     child.RunAndWait( );
     cout << "status = " << (int) child.GetStatus() << endl;

     cout << "RunWithoutWait, testing Abort() 10 sec pause then kill" << endl;

     child.RunWithoutWait( );

     for (int i = 0; i < 10; i++ )
     {
          sleep(1);
          cout << (i+1) << " sec" << endl;
          cout << "status = " << (int) child.GetStatus() << endl;
     }
     

     child.Abort();
     cout << "status = " << (int) child.GetStatus() << endl;

     // now change the completion handler 

     child.SetProcessCompleteHandler( fcn2 );

     cout << "RunWithoutWait" << endl;
     child.RunWithoutWait( );

     // wait for it to start running 
     while ( child.GetStatus() != CP_RUNNING )
     {
          ;
     }


     for ( i= 0; (i < 10) &&  ( CP_RUNNING == child.GetStatus() ); i++ )
     {
          sleep(1);
          cout << i << " sec" << endl;;
          cout << "status = " << (int) child.GetStatus() << endl;
     }
     
     cout << "test prog ended" << endl;
}



#endif


