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
//  CHILDPRC.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the ChildProcess object. 
//
//  CREATED : 5.05.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__CHILDPRC_HXX__)
#define __CHILDPRC_HXX__

#include "vsimtype.hxx"
#include "events.hxx"

enum CHILD_PRC_RC { CP_FAIL, CP_SUCCESS, CP_READY, CP_RUNNING, CP_IPC_ERROR, CP_ILLEGAL_OPERATION };

typedef unsigned long SESSION_ID;

#if !defined(__IBMCPP__)
     #define _Optlink 
#endif


const SESSION_ID INVALID_SESSION_ID = 0;

class ChildProcess
{
     friend void _Optlink Start( void* pObject );

private:
     SESSION_ID      session_id;
     String          executable_filename;
     String          parameters;
     String          program_title;
     String          queue_name;
     CHILD_PRC_RC    status;
     BOOL8           wait;
     BOOL8           thread_running;

     Time            execution_time;

     // these allow user to generate a UserEvent to Application class upon completion of child process

     BOOL8           event_on_complete;
     EVENT_TYPE      event;
     void*           pevent_data;


     CHILD_PRC_RC    Initialize( );

     void            SetStatus( const CHILD_PRC_RC Rc ) { status = Rc; }

public:
                    ChildProcess();
                   ~ChildProcess();
                    ChildProcess( const String& ProgramName );
                    ChildProcess( const String& ProgramName, const String& CommandLineParms );

     void           SetProgramName( const String& ProgramName ) 
                    { executable_filename = ProgramName; }

     void           SetProgramTitle( const String& ProgramTitle ) 
                    { program_title = ProgramTitle; }

     void           SetCommandLineParameters( const String& Parms ) 
                    { parameters = Parms; }

     void           SendEventOnCompletion( BOOL8 State ) { event_on_complete = State; }
     void           SetCompletionEvent( EVENT_TYPE Event ) { event = Event; }
     void           SetpCompletionData( void* pData ) { pevent_data = pData; }

     const String&  GetProgramName(  ) const 
                    { return executable_filename; }

     const String&  GetProgramTitle( ) const
                    { return program_title; }

     const String&  GetCommandLineParameters( ) const
                    { return parameters; }

     const Time&    GetExecutionTime() const { return execution_time; }

     BOOL8          RunAndWait();
     BOOL8          Abort();
     BOOL8          RunWithoutWait();
     BOOL8          DoesChildExist() const;

     CHILD_PRC_RC   GetStatus() const { return status; }
};

#endif

