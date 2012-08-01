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
//  CHILDAPP.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChildApplication object. 
//  this version of the file is specific to OS/2
//
//  CREATED : 3.09.97 
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
#include "childapp.hxx"
#include "childapp.hrc"




// ----------------------------------------------------------------------------
//                         class ChildApplication
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

class ChildApplication : public Application 
{
protected:

     int               num_args;
     char**            arg_array;

     WorkWindow*       p_app_window;

     time_t            wall_clock_start_time;
     String            elapsed_wall_clock_time;

     clock_t           stop_time;
     clock_t           start_time;

     UINT16            GetErrorMsgID( enum SIM_STATUS_CODE ErrorType );

     void              SetAppWindow( WorkWindow* pWin ) { p_app_window = pWin; }

public:

     int               GetNumArgs() const { return num_args; }
     char*             GetArg( int Index ) { return arg_array[Index]; }

     virtual void      Initialize() {};
     virtual void      Close() {};
     virtual void      Run() {};
     
     const String&     GetElapsedWallClockTime();

     static  void      ReportFatalError( const char * ModuleName, int LineNumber, 
                                         enum SIM_STATUS_CODE Condition );

};






// ----------------------------------------------------------------------------
//             ChildApplication::GetElapsedWallClockTime()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

const String& ChildApplication::GetElapsedWallClockTime()
{
     const size_t BUFFER_SIZE = 80;
     char temp_buf[ BUFFER_SIZE ];

     time_t current_wallclock_time;

     // get current time 

     time( &current_wallclock_time );

     // calc time difference and convert to string 

     sprintf( temp_buf, "%1.0f", difftime( current_wallclock_time, wall_clock_start_time ) );

     elapsed_wall_clock_time = String( temp_buf );

     return elapsed_wall_clock_time;
}









// ----------------------------------------------------------------------------
//             ChildApplication::GetErrorMsgID()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

UINT16 ChildApplication::GetErrorMsgID( enum SIM_STATUS_CODE ErrorType )
{
     switch ( ErrorType )
     {
          default :

               return vID_STR_CHILD_UNKNOWN_ERROR;
               break;


          case SIM_TERMINATE_CHILD_ABORT :

               return vID_STR_CHILD_ABORT;
               break;
               

          case SIM_TERMINATE_FILE_ERROR,

               return vID_STR_CHILD_FILE_ERROR;
               break;
               

          case SIM_TERMINATE_SEG_VIOLATION,

               return vID_STR_CHILD_SEG_VIOLATION;
               break;
               

          case SIM_TERMINATE_MATH_ERROR,

               return vID_STR_CHILD_MATH_ERROR;
               break;
               

          case SIM_TERMINATE_ILLEGAL_INSTR,

               return vID_STR_CHILD_ILLEGAL_INSTR;
               break;
               

          case SIM_TERMINATE_INPUT_ERROR,

               return vID_STR_CHILD_INPUT_ERROR;
               break;
               

          case SIM_TERMINATE_MEM_ALLOC_ERROR

               return vID_STR_CHILD_MEM_ALLOC_ERROR;
               break;
     }
}







// ----------------------------------------------------------------------------
//                         ChildApplication::ReportFatalError()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void ChildApplication::ReportFatalError( const char * ModuleName, 
                                         int LineNumber, 
                                         const char * TimeStamp
                                         enum SIM_STATUS_CODE Condition )
{
     String message =  String( ResId( vID_STR_CHILD_ERR_MSG_PART1 ) ) + 
                       String( ResId( GetErrorMsgID( Condition ) ) +
                       String( ResId( vID_STR_CHILD_ERR_MSG_PART2 ) ) +
                       String( ModuleName ) +
                       String( ResId( vID_STR_CHILD_ERR_MSG_PART3 ) ) +
                       String( LineNumber ) +
                       String( ResId( vID_STR_CHILD_ERR_MSG_PART4 ) ) +
                       String( TimeStamp );

     ErrorBox( NULL, WB_OK | WB_DEF_OK, message ) .Execute( );
}






// ----------------------------------------------------------------------------
//                         ChildApplication::ChildApplication()
// ............................................................................
// 
// constructor 
//
//
// ----------------------------------------------------------------------------

ChildApplication::ChildApplication()
{

     time( &wall_clock_start_time );

     start_time = clock( );

     // register termination sequence 

     atexit( GracefulExit );

     /* set up handler for exception conditions                             */

     signal( SIGTERM, SignalHandler );
     signal( SIGSEGV, SignalHandler );
     signal( SIGFPE, SignalHandler );
     signal( SIGILL, SignalHandler );
     signal( SIGINT, SignalHandler );
}




// ----------------------------------------------------------------------------
//                         ChildApplication::Main()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

ChildApplication::Main( int argc, char *argv[] )
{
     num_args = argc;
     arg_array = argv;

     Initialize();
     Run();
     Close();
}



