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

/* start of AIX-specific code                                               */
/* []----------------------------------------------------------------------[]
   |  NOTES ON AIX  IMPLEMENTATION :                                        |
   |                                                                        |
   |  Simulation engine is started as a separate process. The mechanism for |
   |  detecting completion of the simulation is as follows: prior to  the   |
   |  fork()/exec() call we set up a pipe such that stdout of the simulator |
   |  writes to stdin of the front end. We then call XtAppAddInput which    |
   |  results in a callback upon data being made available at stdin. This   |
   |  is a signal that the simulator has completed. We also catch signal    |
   |  SIGUSR1 as an indicator that the exec of the simulator  has failed.   |
   |  Since the parent seems to hang when this happens, the SIGUSR handler  |
   |  simply causes the parent to exit                                      |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#if defined(__AIX_)

/*--------------------------------------------------------------------------*/
/*                        RunSimulation()                                   */
/*..........................................................................*/
/*                                                                          */
/* starts the simulation engine.          Returns TRUE  if successful,      */
/* FALSE otherwise                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL RunSimulation(msimPINSTANCE Instance)

{
     msimSESSION_ID_TYPE sim_pid;
     String message;
     msimSTRING         name;

     int             to_parent[2];
     msimBOOL        error = FALSE;

     msimStringCopy( name, aMainApp.GetAppName(), sizeof name );

     // this installs sig handler so child can kill parent if necessary
     // this may not be needed if I can figure out how to make it so a failure
     // in the child starting does not lead to a hung parent

     if (signal(SIGUSR1, SigHandler) == SIG_ERR)
     {
          ErrorBox(SimOwner, WB_OK|WB_DEF_OK,
           String( "Operating system error setting signal handler" ) ).Execute( );

          return  FALSE;
     }

// added 9.4.94

     Widget  botw =  Sysdepen::GetBottomWidget( aMainApp.GetAppWindow( ) );

     /* get the appp context - we need it to call XtAppAddInput which is the */
     /* mechanism we use to be notified of simulator completion */

     AppContext = XtWidgetToApplicationContext( botw );

     /* create pipe for IPC with simulator */

     if (pipe(to_parent) < 0)
     {
          ErrorBox(SimOwner, WB_OK|WB_DEF_OK,
           String( "Operating system error creating pipe" ) ).Execute( );

          return FALSE;
     }

// end 9.4.94 addn



     // do the fork

     sim_pid = fork();

     switch (sim_pid)
     {
     case (-1) :

          // error - fork failed

          message = String("Operating system error starting "
           "simulation - error code = ") + String( errno );

          ErrorBox(SimOwner, WB_OK|WB_DEF_OK, message).Execute( );

          return  FALSE;

     case 0 :

// added 9.4.94
          /* close stdout and redirect to pipe */
          if (close(1) < 0)
               error = TRUE;

          if (dup(to_parent[1]) < 0)
               error = TRUE;

          /* this is the input side of pipe - close since we won't use it */

          if (close(to_parent[0]) < 0)
               error = TRUE;

          /* we have dup'ed this - close original */

          if (close(to_parent[1]) < 0)
               error = TRUE;

          if (error)
          {
               printf("Pipe error in child - errno = %d\n", errno);
               exit(1);
          }

// end 9.6.94 addn

          // we are in child - overlay with simulator

          msimStringCopy( name, aMainApp.GetAppName(), sizeof name );

          execl( SimulatorFilename, SimulatorFilename,
                RunparmFilename, name, 0);

          // if we make it to this point simulation engine has not started
          // and execl has returned

          // notify parent and print to stderr that we have problems

          fprintf(stderr,
            "\nExec error in child starting\n%s...aborting (errno = %d)\n\n",
                SimulatorFilename, errno);

          fflush(stderr);

          // now get rid of parent since it is hung somewehere for reasons
          // that are not clear to me  at the moment

          kill(getppid(), SIGUSR1);
          exit(msimCHILD_ABORT);


     default  :

          // we are in parent - proceed. sim_pid == PID of simulation

// 9.6.94 addn wdh
          /* close stdin and redirect to pipe */

          if (close(0) < 0)
               error = TRUE;

          if (dup(to_parent[0]) < 0)
               error = TRUE;

          /* we have dup'ed this so close original */
          if (close(to_parent[0]) < 0)
               error = TRUE;

          /* this is the output end of pipe - not used so close it */
          if (close(to_parent[1]) < 0)
               error = TRUE;

          if (error)
          {
               ErrorBox(SimOwner, WB_OK|WB_DEF_OK,
                String( "Pipe error in parent" ) ).Execute( );

               return FALSE;
          }                                      /* end if */

          InputID = XtAppAddInput(AppContext, 0, (XtPointer) XtInputReadMask,
                       (XtInputCallbackProc)  SimCompleteCallback, NULL);

// end 9.6.94 addn

          SimInstance = Instance;
          Instance->sim_return_code = msimSIMULATION_RUNNING;

          Instance->session_id = sim_pid;

          aMainApp.OKToStartSimulation = FALSE;

          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

          return  TRUE;

     }                                 /* end switch                          */
}


// this fcn added 9.6.94 wdh

/*--------------------------------------------------------------------------*/
/*                        SimCompleteCallback()                             */
/*..........................................................................*/
/*                                                                          */
/* called when input is avaiables at stdin. child is set up with a pipe     */
/* such that input becomes available when the simulation terminates         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void            SimCompleteCallback(caddr_t ClientData, int *FileNum, XtInputId * ID)
{
     char            buf[sizeof(msimDONE_MSG)];

     if (read(*FileNum, buf, sizeof(buf)))       /* returns num of bytes read */
     {
          /* msg indicating successful start+stop of simulation */

          if (0 == strncmp(buf, msimDONE_MSG, sizeof(msimDONE_MSG)))
          {
               /* delete temporary file */
#ifndef DEBUG
               remove(RunparmFilename);
#endif
               XtRemoveInput(InputID);

               msimUpdateAfterSimulation( SimOwner, ApplicationEvent( aMainApp.GetAppName( ),
                    String( ),
                    String( DONE_STRING ),
                    String( RunparmFilename ) ) );

               wait( NULL );  // added 10.05.94 to preent zombies

          }
     }
     return;
}




/*--------------------------------------------------------------------------*/
/*                        SigHandler()                                      */
/*..........................................................................*/
/*                                                                          */
/* called when execlp fails in the child.                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void SigHandler(int S)
{
     int                rc;

     /* wait for child so it does not become zombie                        */

     wait(&rc);

     exit(msimCHILD_ABORT);
}


/*--------------------------------------------------------------------------*/
/*                        KillSimulation()                                  */
/*..........................................................................*/
/*                                                                          */
/* sends KILL signal to simulation engine. Returns TRUE  if successful,     */
/* FALSE otherwise                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL KillSimulation(msimSESSION_ID_TYPE Session)
{
     int                rc;

     kill(Session, SIGTERM);
     rc = wait(NULL);

     if (rc != -1)
          msimUpdateAfterSimulation(SimOwner,  ApplicationEvent( aMainApp.GetAppName(),
                                                            String( ),
                                                            String(ABORT_STRING),
                                                             String(RunparmFilename) ));

     return (msimBOOL)(rc != -1);
}

#endif

/* end of AIX_specific code                                                 */
// ----------------------------------------------------------------------------
//  CHLDPRCX.CXX - a module of the VSIM User interface for AIX
// ............................................................................
//
//  This module defines the ChildProcess object.
//  this version of the file is specific to AIX
//
//  CREATED : 8.11.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#pragma hdrstop

#include "assert.hxx"
#include "childprc.hxx"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>

static void SigHandler(int S);

#include <sysdep.hxx>
#include <prex.h>
#include <X11/Intrinsic.h>
#include <postx.h>

static XtInputId    InputID;
static XtAppContext AppContext;

static void         SimCompleteCallback(caddr_t ClientData, int *FileNum, XtInputId * ID);



// ----------------------------------------------------------------------------
//                      ChildProcess::ChildProcess()
// ............................................................................
//
//      default constructor
//
//
// ----------------------------------------------------------------------------

ChildProcess::ChildProcess() :
session_id( INVALID_SESSION_ID ),
executable_filename(),
parameters(),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
process_complete_handler( NULL )
{
     SetStatus( Initialize() );
}







// ----------------------------------------------------------------------------
//                      ChildProcess::ChildProcess()
// ............................................................................
//
//      constructor with executable filename as parameter
//
//
// ----------------------------------------------------------------------------

ChildProcess::ChildProcess( const String& ProgramName ) :
session_id( INVALID_SESSION_ID ),
executable_filename( ProgramName ),
parameters(),
status( CP_FAIL ),
wait( TRUE ),
thread_running( FALSE ),
process_complete_handler( NULL )
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
process_complete_handler( NULL )
{
     SetStatus( Initialize() );
}



// ----------------------------------------------------------------------------
//                      ChildProcess::~ChildProcess()
// ............................................................................
//
//      destructor
//
//
// ----------------------------------------------------------------------------

ChildProcess::~ChildProcess()
{
     if ( GetStatus() == CP_RUNNING )
          Abort();
}





// ----------------------------------------------------------------------------
//                      ChildProcess::Initialize( )
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
//                      ChildProcess::RunAndWait()
// ............................................................................
//
//  starts the child process and waits for it to end before returning. Returns
//  TRUE if successfully started child process, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 ChildProcess::RunAndWait()
{
     return TRUE;
}






// ----------------------------------------------------------------------------
//                      ChildProcess::RunWithoutWait()
// ............................................................................
//
//  starts the child process and returns immediately upon starting it. Returns
//  TRUE if successfully started child process, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 ChildProcess::RunWithoutWait()
{
     return TRUE;
}



BOOL8 ChildProcess::Abort()
{
     if ( session_id != INVALID_SESSION_ID )
     {

     }
     else
          return FALSE;
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


