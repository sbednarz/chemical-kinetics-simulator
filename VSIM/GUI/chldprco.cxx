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
//  CHLDPRCO.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChildProcess object. 
//  this version of the file is specific to OS/2 CSet++
//
//  CREATED : 5.05.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "assert.hxx"
#include "childprc.hxx"
#include "killproc.hxx"

#endif


#include <string.h>
#include <process.h>
#include <stdlib.h>






#if !defined (__IBMCPP__)
     #define _spawnl spawnl
     #define _cwait  cwait
#endif





// ----------------------------------------------------------------------------
//                         ChildProcess::ChildProcess()
// ............................................................................
// 
//	constructor 
//
// ----------------------------------------------------------------------------

ChildProcess::ChildProcess() :
session_id( INVALID_SESSION_ID ),
executable_filename(),
parameters(),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
execution_time( 0 )
{
     SetStatus( Initialize() );
}





// ----------------------------------------------------------------------------
//                         ChildProcess::ChildProcess()
// ............................................................................
// 
//	constructor 
//
// ----------------------------------------------------------------------------

ChildProcess::ChildProcess( const String& ProgramName ) :
session_id( INVALID_SESSION_ID ),
executable_filename( ProgramName ),
parameters(),
status( CP_FAIL ),
wait( TRUE ), 
thread_running( FALSE ),
execution_time( 0 )
{
     SetStatus( Initialize() );
}




// ----------------------------------------------------------------------------
//                         ChildProcess::ChildProcess()
// ............................................................................
// 
//	constructor 
//
// ----------------------------------------------------------------------------

ChildProcess::ChildProcess( const String& ProgramName, 
 const String& CommandLineParms ) :
session_id( INVALID_SESSION_ID ),
executable_filename( ProgramName ),
parameters( CommandLineParms ),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
execution_time( 0 )
{
     SetStatus( Initialize() );
}






// ----------------------------------------------------------------------------
//                         ChildProcess::~ChildProcess()
// ............................................................................
// 
//	destructor 
//
// ----------------------------------------------------------------------------

ChildProcess::~ChildProcess()
{
     if ( GetStatus() == CP_RUNNING )
          Abort();
}





// ----------------------------------------------------------------------------
//                         ChildProcess::Initialize()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

CHILD_PRC_RC ChildProcess::Initialize( )
{
     return CP_READY;
}







// ----------------------------------------------------------------------------
//                         ChildProcess::RunAndWait()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

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








// ----------------------------------------------------------------------------
//                         ChildProcess::RunWithoutWait()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 ChildProcess::RunWithoutWait()
{
     static const size_t THREAD_STACK_SIZE = 8192;

      wait = FALSE;

#if defined(__IBMCPP__)
     _beginthread( Start, NULL, THREAD_STACK_SIZE, this );
#else
     _beginthread( Start, THREAD_STACK_SIZE, this );
#endif

     if ( CP_SUCCESS == GetStatus() )
     {
          SetStatus( CP_READY );
          return TRUE;
     }
     else
          return FALSE;

}










// ----------------------------------------------------------------------------
//                         ChildProcess::Abort()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 ChildProcess::Abort()
{
     if ( session_id != INVALID_SESSION_ID )
     {
          if ( KillProcess( session_id ) )
          {
               // if thread is running then wait for it to end 

               if ( thread_running )
               {
                    while ( thread_running)
                       Sleep( 100 );   // pause for 100 msec w/o using cpu cycles
               }

               session_id = INVALID_SESSION_ID;
               SetStatus( CP_READY );
               return TRUE;
          }
          else
          {
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



// ----------------------------------------------------------------------------
//                         Start( )
// ............................................................................
// 
//    a friend of ChildProcess - actually starts the child
//
// ----------------------------------------------------------------------------

void _Optlink Start( void* pPtr )
{
     static const size_t BUFFER_SIZE       = 260;

     // char buffers

     char          prog_title[BUFFER_SIZE];               // Program title string 
     char          obj_buf[BUFFER_SIZE];                  // Object buffer
     char          command_line_parms[BUFFER_SIZE];
     char          exec_name[BUFFER_SIZE];


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
     
     int           pid;                    // Process ID (returned)   

     // OK, ready to go - record the time 

     Time start_wallclock_time;

     if ( no_errors_yet )
     {
          // fill static char buffers with parameters

          strcpy( command_line_parms, p_object->parameters.GetStr() );
          strcpy( prog_title, p_object->program_title );
          strcpy( exec_name, p_object->executable_filename );



          pid = _spawnl( P_NOWAIT, exec_name, exec_name, command_line_parms, NULL );

          if ( -1 == pid )
          {
               // a -1 rc means failure - set status and return 

               p_object->SetStatus( CP_FAIL );
               no_errors_yet = FALSE;
          }
          else
          {
               p_object->session_id = (SESSION_ID) pid;
               p_object->SetStatus( CP_RUNNING );
          }
     }


     if ( no_errors_yet )
     {
          int           rc; // value set by cwait - we do not use it yet...

          if ( pid != _cwait( &rc, pid, WAIT_CHILD ) )
          {
               p_object->SetStatus( CP_IPC_ERROR );
               no_errors_yet = FALSE;
          }
     }


     if ( no_errors_yet )
     {
          p_object->session_id = INVALID_SESSION_ID;  // reset this to an invalid value 

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


