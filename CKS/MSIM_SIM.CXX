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
/*  FILE NAME : msim_sim.cxx                                                */
/*                                                                          */
/*  This module  contains the functions for starting the simulation engine  */
/*  as a separate session/process                                           */
/*                                                                          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
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
/*  CHANGE HISTORY :                                                        */
/*             5.26.95 change OS/2, WIN simulator names to ENGINE.EXE (wdh) */
/*             and AIX, MAC simulator names to engine                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimtprg.hxx"
#include "msimfile.hxx"
#include "msimque.hxx"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ABORT_STRING   "abort"
#define DONE_STRING    "done"

#if      defined(__OS2__)
#include <process.h>
#endif

#if      defined(__MSDOS__)
typedef const void near *HANDLE;

typedef char       far *LPSTR;

extern HANDLE msimRunSimulation(LPSTR SimulatorFilename,LPSTR RunParmFileName,
                                 LPSTR ParentName);

extern BOOL msimStopSession(HANDLE htask);

#endif

#ifdef   __AIX__
// #include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>
#define  SIMULATION_PROGRAM                      "engine"
#define  MAX_NUM_STEPS                           32767

static void SigHandler(int S);

#endif


// this defined(__AIX__)  section added 9.6.94 wdh

#if defined(__AIX__)

#include <sysdep.hxx>
// #include <sys/types.h>
#include <prex.h>
#include <X11/Intrinsic.h>
#include <postx.h>

static XtInputId    InputID;
static XtAppContext AppContext;

static void         SimCompleteCallback(caddr_t ClientData, int *FileNum, XtInputId * ID);

#endif



#if defined(__MAC__)

#define  SIMULATION_PROGRAM                      "engine"
#define  MAX_NUM_STEPS                           32767

#if defined(__PPC__)
#include <SysDep.hxx>
#endif

#endif

/* declarations for common local data/functions                             */

static msimPINSTANCE SimInstance;
static msimWID SimOwner;
static msimFILE_STRING RunparmFilename = msimOUTPUT_FILENAME;
static msimFILE_STRING SimulatorFilename;

static msimRC WriteSchemeAnalysis( msimPINSTANCE Instance, msimWID Owner,
                   msimBOOL Resume, PCHAR Filename, size_t BufSize );

static msimBOOL KillSimulation( msimSESSION_ID_TYPE Session );

static void WriteRateConstantData( FILE *F, msimPRXN Reaction, USHORT ConcUnits,
                 USHORT TimeUnits, USHORT EnergyUnits );

static void WriteSpeciesThermochemistry( FILE *F, msimPSPECIES StartOfList,
                 USHORT SpeciesCount, USHORT
                 EnergyUnits );

static void WriteAnalyticalTempProgData( FILE *F, msimPTEMP_COEFFS pTempData,
                 USHORT CurrentTimeUnits );

static PCHAR ConcStrInMolarUnits( PCHAR Str, USHORT CurrentConcUnits, USHORT VolumeOption );
static PCHAR EaStrInKcalUnits( PCHAR Str, USHORT CurrentEnergyUnits );
static PCHAR AFactorStrInSecUnits( PCHAR Str, USHORT TimeUnits, USHORT ConcUnits,
                  msimFLOAT Exponent );

static PCHAR TimeStrInSecUnits( PCHAR Str, USHORT TimeUnits );

static msimRC ReadTempProfileFile( msimWID Owner, const PCHAR Filename, FILE PTR OutFile,
                   USHORT TimeUnits );


#if defined(__MAC__)

#include <mac_start.h>

#include <processes.h>
#include <AppleEvents.h>
#include <StandardFile.h>

static ProcessSerialNumber SimProcessSN;


#if defined(__PPC__)
void ChildDied( )
 {
       msimUpdateAfterSimulation( SimOwner, ApplicationEvent( aMainApp.GetAppName( ),
                    String( ),
                    String( DONE_STRING ),
                    String( RunparmFilename ) ) );

}

#else
pascal OSErr ChildDied( AppleEvent, AppleEvent, long )
 {
       msimUpdateAfterSimulation( SimOwner, ApplicationEvent( aMainApp.GetAppName( ),
                    String( ),
                    String( DONE_STRING ),
                    String( RunparmFilename ) ) );

     return noErr;
 }

#endif

int macRunSimulation( char * SimulatorFilename, char * RunParmFilename,
     ProcessSerialNumber **pPSN, short * pErrCode )
{

     LaunchParamBlockRec   parms;
      OSErr                 err;
      FSSpec                file_spec;
      FSSpec                runparm_filespec;
      char                  filename[256];


      // first install ApplicationDied event handler so we know when simulator ended

#if !defined(__PPC__)

      err = AEInstallEventHandler( kCoreEventClass, kAEApplicationDied,
       ( EventHandlerProcPtr) ChildDied, 0L, false );

     if (err)
      {
          *pErrCode = err;
          return FALSE;
      }

#endif

      // contruct a pascal-style string of the simulator filename for making a FSSpec

      msimStringCopy( &filename[1], SimulatorFilename, (sizeof filename) - 1 );
      filename[0] = (unsigned char) strlen( &filename[1]);

      // now make the FSSpec - we need this for LaunchApplication call

      err = FSMakeFSSpec( 0, 0, (ConstStr255Param) filename, &file_spec);

     if (err)
      {
          *pErrCode = err;
          return FALSE;
      }

      // contruct a pascal-style string for the runparm filename - we pass this
      // to simulator after launch as an open documents Apple Event
      // note that the SV mwthod for sending ApplicationEvents does not work here
      // but the receipt of these events by the simultor using
      // the SV mechanism seems to work OK

      msimStringCopy( &filename[1], RunParmFilename, ( sizeof filename) -1 );
      filename[0] = (unsigned char) strlen( &filename[1]);

      err = FSMakeFSSpec( 0, 0, (ConstStr255Param) filename, &runparm_filespec);

     if (err)
      {
          *pErrCode = err;
          return FALSE;
      }

     // now launch the simulator application

     parms.launchBlockID        = extendedBlock;
      parms.launchEPBLength      = extendedBlockLen;
      parms.launchFileFlags      = 0;
      parms.launchControlFlags   = launchContinue + launchNoFileFlags;
      parms.launchAppSpec        = &file_spec;
      parms.launchAppParameters  = nil;

      // temp to get the FSSpec constructed correctly

      err = LaunchApplication (&parms);

      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }

      // the simulator is now running - send an "open documents" event

      AEAddressDesc address;
      AppleEvent  ae, reply;
      AEDesc         parm_desc;

      // make the address descriptor using the process serial number we got from
      // LunchApplication

      err = AECreateDesc( typeProcessSerialNumber, (Ptr) &parms.launchProcessSN,
      sizeof( ProcessSerialNumber), &address);

      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }

      // create the event structure

      err = AECreateAppleEvent( kCoreEventClass, kAEOpenDocuments, &address,
      kAutoGenerateReturnID, 1L, &ae );
      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }

     // create a descriptor containing the FSSpec describing the run parm file

     err = AECreateDesc( typeFSS, &runparm_filespec, sizeof( runparm_filespec), &parm_desc );
      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }

     // add the descriptor to the AppleEvent

     err = AEPutParamDesc( &ae, keyDirectObject,
                &parm_desc );
      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }

     // the AppleEvent is complete - send it on its way

      err = AESend( &ae, &reply, kAENoReply + kAECanInteract, kAENormalPriority,
      kAEDefaultTimeout, NULL, NULL );
      if ( err)
      {
          *pErrCode = err;
          return FALSE;
      }


      // we made it to here only if simulation engine started w/o problems
     // save the relevant data and return

     SimProcessSN = parms.launchProcessSN;
     *pErrCode = 0;
      *pPSN = &SimProcessSN;
      return TRUE;
}





void RunSimulation( msimPINSTANCE Instance )
{
     ProcessSerialNumber PTR  pPSN;
      short errcode;

      aMainApp.OKToStartSimulation = FALSE; // global flag

     if ( 1 == macRunSimulation( SimulatorFilename, RunparmFilename, &pPSN, &errcode ) )
     {
          // we come here if we succesfully started simulation

          SimInstance = (msimPINSTANCE)Instance;
          SimInstance->sim_return_code = msimSIMULATION_RUNNING;

          // set session id to address of saved ProcessSerialNumber
            // session_id is set to null (msim_NO_SESSION) when no process running

          SimInstance->session_id = (msimSESSION_ID_TYPE) pPSN;

          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY);
      }
      else
      {
          String message = "Error starting simulation.\nError code = ";

          message += String( errcode );

          ErrorBox( SimOwner, WB_OK | WB_DEF_OK, message ) .Execute( );

          aMainApp.OKToStartSimulation = TRUE;
            return;

      }
}




static msimBOOL KillSimulation( msimSESSION_ID_TYPE Session )
{

      AEAddressDesc address;
      OSErr                 err;
      AppleEvent  ae, reply;

      err = AECreateDesc( typeProcessSerialNumber, (ProcessSerialNumber PTR) Session,
      sizeof( ProcessSerialNumber), &address);

      if ( err)
          return FALSE;

      err = AECreateAppleEvent( kCoreEventClass, kAEQuitApplication, &address,
      kAutoGenerateReturnID, 1L, &ae );

      if ( err)
          return FALSE;

      err = AESend( &ae, &reply, kAENoReply + kAECanInteract, kAENormalPriority,
      kAEDefaultTimeout, NULL, NULL );

      if ( err)
          return FALSE;
		  
#if defined(__PPC__)	

     msimUpdateAfterSimulation(SimOwner,  ApplicationEvent( aMainApp.GetAppName(),
                                                            String( ),
                                                            String(ABORT_STRING),
                                                             String(RunparmFilename) ));
#endif															 


     return TRUE;

}

#include <mac_end.h>


#endif





/* OS/2 specific code follows                                               */
/* []----------------------------------------------------------------------[]
   |  NOTES ON OS/2 IMPLEMENTATION :                                        |
   |                                                                        |
   |  Simulation engine is started as a separate session. Parent is notified|
   |  of child termination by use of Starview PostAppMessage IPC facility.  |
   |  Upon completion of the simulation the first record of the             |
   |  rxn file is read in and the Instance struct is updated.               |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#if      defined(__OS2__)

#define  SIMULATION_PROGRAM                      "ENGINE.EXE"
#define  THREAD_STACK_SIZE                       8092
#define  MAX_NUM_STEPS                           32767
// static void WaitForSimCompletion(void);

static void RunSimulation( void *Instance );

void RunSimulation( void *Instance )
{

     /* start the simulation as a separate session                       */

     ULONG rc;
     msimFILE_STRING name;
     msimStringCopy( name, aMainApp.GetAppName( ), sizeof name );

     aMainApp.OKToStartSimulation = FALSE;/* global flag indicates a
                                             simulation is running            */

     rc = msimRunSimulation( &((( msimPINSTANCE ) Instance ) ->session_id ), SimulatorFilename, RunparmFilename, name );

     if ( rc )
     {
          String message = "Error starting simulation.\nError code = ";

          message += String( rc );

          ErrorBox( SimOwner, WB_OK | WB_DEF_OK, message ) .Execute( );

          aMainApp.OKToStartSimulation = TRUE;
     }
     else
     {
          SimInstance = ( msimPINSTANCE ) Instance;
          SimInstance->sim_return_code = msimSIMULATION_RUNNING;

          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

          if ( rc = msimWaitForSimCompletion( ) )
          {
               String message = "Error waiting for child\nError code = ";

               message += String( rc );

               ErrorBox box( SimOwner, WB_OK | WB_DEF_OK, message );

               box.Execute( );

          }

          /* the following sends a message to main window which, when */
          /* processed, takes care of the details to update the data  */
          /* for the just-completed simulation                        */

          msimUpdateAfterSimulation( SimOwner, ApplicationEvent( aMainApp.GetAppName( ),
                    String( ),
                    String( DONE_STRING ),
                    String( RunparmFilename ) ) );



     }

     _endthread( );
}


/*--------------------------------------------------------------------------*/
/*                        KillSimulation()                                  */
/*..........................................................................*/
/*                                                                          */
/* calls  OS/2 function to close  a session. Returns TRUE  if successful,   */
/* FALSE otherwise                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL KillSimulation( msimSESSION_ID_TYPE session )
{
     msimBOOL rc = msimStopSession ( session );


     if ( rc )
     {

          String message = "DOS Error stopping "
          "simulation - error code = ";

          message += rc;

          ErrorBox( SimOwner, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }

     return TRUE;
}

#endif

/* end of OS/2-specific code                                                */



/* []----------------------------------------------------------------------[]
   |  NOTES ON DOS.Windows 3.x implementation                               |
   |                                                                        |
   |  Simulation engine is started as a separate session.  Completion of the|
   |  simulation is detected by a AppEvent sent from simulator.             |
   |  Upon completion of the simulation the first record of the             |
   |  rxn file is read in and the Instance struct is updated.               |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#if      defined(__MSDOS__)

#define  SIMULATION_PROGRAM                      "ENGINE.EXE"
#define  MAX_NUM_STEPS                           180
// for any platform MAX_NUM_STEPS = round ( sqrt( UINT_MAX/ sizeof( unsigned int) ) -1 )

void RunSimulation(void *Instance)
{
     HANDLE             handle;
     msimSTRING         name;

     msimStringCopy( name, aMainApp.GetAppName(), sizeof name );

     aMainApp.OKToStartSimulation = FALSE;


     handle = msimRunSimulation((LPSTR)SimulatorFilename, (LPSTR)
      RunparmFilename, name);

     while ( (unsigned int)handle < 32 )
     {
          String             message =
           "Error starting simulation. Error no = ";

          message += String((unsigned int)handle);
          message += String("\n\nTry again?");

          ErrorBox box(SimOwner, WB_RETRY_CANCEL|WB_DEF_RETRY, message);

          if (RET_CANCEL == box.Execute() )
          {
               aMainApp.OKToStartSimulation = TRUE;
               msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY);
               return;
          }
          else
               handle = msimRunSimulation((LPSTR)SimulatorFilename, (LPSTR)
                    RunparmFilename, name);

     }

     // we come here if we succesfully started simulation

     SimInstance = (msimPINSTANCE)Instance;
     SimInstance->sim_return_code = msimSIMULATION_RUNNING;
     SimInstance->session_id = (msimSESSION_ID_TYPE)handle;

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY);

}

msimBOOL KillSimulation(msimSESSION_ID_TYPE session)
{
     if (!msimStopSession( (HANDLE)session) )
     {
          ErrorBox(SimOwner, WB_OK|WB_DEF_OK,String("Error stopping simulation") ).Execute();
          return  FALSE;
     }

     msimUpdateAfterSimulation(SimOwner,  ApplicationEvent( aMainApp.GetAppName(),
                                                            String( ),
                                                            String(ABORT_STRING),
                                                             String(RunparmFilename) ));

     return  TRUE;

}

#endif

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

#if defined(__AIX__)

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


/*--------------------------------------------------------------------------*/
/*                   msimAbortSimulation( )                                 */
/*..........................................................................*/
/*                                                                          */
/* sends a signal to a running simulation to kill it                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimAbortSimulation( msimWID Owner, msimPINSTANCE Instance )
{
     msimBOOL rc;
     String message;

     if ( Instance->session_id != msimNO_SESSION )
     {
          message = String( ResId( msimABORT_SIM_STRING1 ) );

          message += Instance->base_filename;
          message += String( ResId( msimABORT_SIM_STRING2 ) );

          if ( RET_YES == WarningBox( Owner, WB_YES_NO | WB_DEF_NO, message ) . Execute( ) )
          {
               rc = KillSimulation( Instance->session_id );

          }
          else
               return;

          if ( ! rc )
          {
               message = "The simulation of file \"";

               message += Instance->base_filename;
               message += "\" was NOT successfully aborted";

               ErrorBox( Owner, WB_OK | WB_DEF_OK, message ) .Execute( );
          }
     }
     return;
}



/*--------------------------------------------------------------------------*/
/*                   msimCheckForActiveSimulation                           */
/*..........................................................................*/
/*                                                                          */
/* scans through all reaction instances looking for an active simulation    */
/* session. If one is found the user is given a message. FALSE is returned  */
/* if no active simulations were found, true is returned otherwise and a    */
/* dialog box is displayed                                                  */
/*--------------------------------------------------------------------------*/

msimBOOL msimCheckForActiveSimulation( msimPINSTANCE Instance[], msimWID Owner )
{
     USHORT i;

     for ( i = 0; i < msimMAX_NO_INSTANCES; i++ )
     {
          if ( Instance[i] == NULL )
               continue;               /* skip NULL instances                 */
          if ( Instance[i]->session_id != msimNO_SESSION )
          {
               String message = "A simulation of reaction file \"";

               message += Instance[i]->base_filename;
               message += String( ResId( msimACTIVE_SIM_STRING ) );

               InfoBox( Owner, message ) .Execute( );

               return TRUE;
          }
     }
     return FALSE;
}

/*--------------------------------------------------------------------------*/
/*                        msimStartSimulation                               */
/*..........................................................................*/
/*                                                                          */
/* controls the start of the simulation process. Carries out a number of    */
/* tests to be sure data is valid and user is not making an irreversible    */
/* mistake                                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimStartSimulation( msimWID Owner, msimPINSTANCE Instance, msimBOOL Resume )
{
     msimRC rc;
     msimFILE_STRING runparm_filename;

     if ( Instance -> specieslist_altered )
          if ( msimNO_ERROR != msimVerifySpeciesDataDlg( Owner, Instance ) )
               return;

     while ( Instance -> nonzeroconcs < 1 )
     {
          if ( msimUSER_ABORT ==
                    msimSetConcentrations( Instance, Owner ) )
               return;
     }

     // this is added to allow checking that all species with non-zero
     // initial amounts are in the same phase [wdh 7.15.94]

     if ( Instance->volume_option == msimVAR_VOL)
     {
          while (FALSE == msimCheckInitialPhase( Instance) )
             if ( msimUSER_ABORT == msimChangePhaseDataDlg( Owner, Instance ) )
                  return;
     }


     /* check if plot dialog is open - close if user says ok                */

     if ( Instance->p_plot_dialog )
     {
          if ( RET_YES == QueryBox( Owner, ResId( msimCLOSE_PLOT_BEFORE_SIM ) ) .Execute( ) )

               /* if ok to close, send a message to plot dialog window to close*/
               msimClosePlotDialogWindow( Instance );
          else
               return;
     }
     if ( Instance->numsteps > MAX_NUM_STEPS )
     {
          InfoBox( Owner,
               (
                    String( ResId( msimTOO_MANY_RXNS_STR1 ) ) +
                    String( MAX_NUM_STEPS ) +
                    String( ResId( msimTOO_MANY_RXNS_STR2 ) )
               )
          ) .Execute( );
          return;
     }


     /* check if notebook window is open and belongs to the              */
     /* current instance - close the notebook if so                      */

     if ( Instance->p_notebook_window )
          msimCloseNotebookWindow( Instance );

     if ( ! Resume )
          if ( msimNO_ERROR != msimSaveRxnScheme( Instance, Owner, msimCHECK_FOR_OVERWRITE ) )
               return;

     aMainApp.Wait( TRUE );

     rc = WriteSchemeAnalysis( Instance, Owner, Resume, runparm_filename,
          sizeof runparm_filename );

     aMainApp.Wait( FALSE );

     if ( rc )
          return;

     SimOwner = Owner;

     if ( aMainApp.OKToStartSimulation )
     {
          // go ahead and start the simulation directly if there is not
          // another one running at the moment

          msimStringCopy( RunparmFilename, runparm_filename, sizeof RunparmFilename );
#if defined(__OS2__)
          _beginthread( RunSimulation, THREAD_STACK_SIZE, ( void PTR ) Instance );
#else
          RunSimulation(Instance);
#endif
     }
     else
     {
          // otherwise we add it to the queue of waiting simulations
          if ( msimSimQueue.AddToQueue( Instance, runparm_filename ) )
          {
               String msg = String( "Another simulation is currently running. Rxn file \"" );
               msg += String( Instance->base_filename );
               msg += String( "\" has been placed in the simulation queue." );

               InfoBox( Owner, msg ) .Execute( );
               Instance->sim_return_code = msimWAITING_IN_QUEUE;
               msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
          }
     }
     return;
}


/*--------------------------------------------------------------------------*/
/*                        WriteSchemeAnalysis()                             */
/*..........................................................................*/
/*                                                                          */
/* writes out all the encoded description of the reaction scheme to be      */
/* simulated to a temporary file. This file is in ASCII format and          */
/* contains string, numeric and keywords for initializing the simulation    */
/* engine                                                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC WriteSchemeAnalysis( msimPINSTANCE Instance, msimWID Owner, msimBOOL
            Resume, PCHAR Filename, size_t BufSize )
{
     FILE *f;
     msimPRXN rxnptr;
     msimPSPECIES species_ptr;
     USHORT tp_option;

     /* we initialize initial_temp_str it here but reset it if we        */
     /* are using the analytical prog temp option                        */

     PCHAR initial_temp_str;

     // select which initial temperature value to use based
     // on user's temperature selection
     // programmed temperature using a ext temp profile
     //  gets its initial temperature directly from the profile

     if ( Instance->temp_option == msimCONST_TEMP )
          initial_temp_str = Instance->temp_data.const_temp;

     if ( Instance->temp_option == msimVAR_TEMP )
          initial_temp_str = Instance->temp_data.initial_temp;

     if ( Instance->temp_option == msimPROGR_TEMP )
          initial_temp_str = Instance->temp_data.prog_initial_temp;

     /* create a temporary filename                                      */

     tmpnam( Filename );
     msimConstructFullyQualifiedFilename( Filename, BufSize );

     /* open the output file and check for problems                         */

     if ( NULL == ( f = fopen( Filename, "w" ) ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     /* calculate the current size/position of main window as the simulator */
     /* will initially locate itself relative to the main app window     */

     Point win_position = aMainApp.GetAppWindow( ) ->GetPosPixel( );
     Size win_size = aMainApp.GetAppWindow( ) ->GetSizePixel( );

     /* write out :
        1. the name of the file to which the simulation engine
        2a. window coords of parent process
        3. flag to indicate that mechanism data follows
        4. number of species
        5. number of reactions
      */

     fprintf( f, "%s\n%hd\n%hd\n%hd\n%hd\n%s\n%hu\n%hu\n", Instance->filename,
          win_position.X( ), win_position.Y( ), win_size.Height( ), win_size.Width( ),
          Resume ? msimRESUME_SIM_FLAG : msimMECHANISM_FLAG, Instance->speciescount,
          Instance->numsteps );

     /* now work through the scheme, one rxn step at a time              */

     rxnptr = Instance->ptr_to_rxnlist;

     while ( rxnptr )
     {
          USHORT i;

          fprintf( f, "%hu\n", rxnptr->numreactants );

          for ( i = 0; i < rxnptr->numreactants; i++ )
          {
               fprintf( f, "%hu\n%s\n", rxnptr->reactantlist[i].speciesindex,
                    rxnptr->reactantlist[i].stoichcoeff );
          }

          fprintf( f, "%lu\n%hu\n", rxnptr->reversible, rxnptr->numproducts );

          for ( i = 0; i < rxnptr->numproducts; i++ )
          {
               fprintf( f, "%hu\n%s\n", rxnptr->productlist[i].speciesindex,
                    rxnptr->productlist[i].stoichcoeff );
          }

          /* now deal with special kinetics if necessary                    */

          fprintf( f, "%lu\n", rxnptr->not_stoich );

          if ( rxnptr->not_stoich )
          {
               USHORT index_array[msimMAX_NUMBER_OF_COMPONENTS];
               PCHAR endptr;
               USHORT i, j;

               /* first count number of non-zero exponents, store index     */
               /* in index ond the count in j                               */

               j = 0;
               for ( i = 0; ( i < rxnptr->numreactants ) && ( i <
                              msimMAX_NUMBER_OF_COMPONENTS ); i++ )
               {
                    if ( strtod( rxnptr->fwdexponent[i], &endptr ) != 0.0 )
                         index_array[j++] = i;

               }                       /* end for                             */

               /* and write it all to outfil                                */

               fprintf( f, "%hu\n", j );

               /* then go through the array again and write the data with   */
               /* non-zero exponents to outfil                              */

               for ( i = 0; ( i < j ); i++ )
               {
                    fprintf( f, "%hu\n%s\n", rxnptr->reactantlist[index_array[i]
                         ].speciesindex, rxnptr->fwdexponent[index_array[i]] );
               }                       /* end for                             */
               if ( rxnptr->reversible )
               {
                    j = 0;
                    for ( i = 0; ( i < rxnptr->numproducts ) && ( i <
                                   msimMAX_NUMBER_OF_COMPONENTS ); i++ )
                    {
                         if ( strtod( rxnptr->revexponent[i], &endptr ) != 0.0 )
                              index_array[j++] = i;

                    }                  /* end for                             */

                    /* and write it all to outfil                           */

                    fprintf( f, "%hu\n", j );

                    /* then go through the array again and write the data with*/
                    /* non-zero exponents to outfil             */

                    for ( i = 0; ( i < j ); i++ )
                    {
                         fprintf( f, "%hu\n%s\n", rxnptr->productlist
                              [index_array[i]].speciesindex, rxnptr->revexponent
                              [index_array[i]] );
                    }                  /* end for                             */
               }                       /* end if                              */
          }                            /* done with special kinetics rate
                                          law                                 */
          WriteRateConstantData( f, rxnptr, Instance->conc_units, Instance->
               time_units, Instance->energy_units );

          /* and point to the next struct in the list                       */

          rxnptr = rxnptr->next;

     }                                 /* end while                           */

     /* now write out all the thermochemical data if required               */

     if ( Instance->temp_option == msimVAR_TEMP )
     {

          fprintf( f, "%s\n", msimTHERMOCHEM_FLAG );

          /* scan through the species linked list and write out the   */
          /* the thermochemical data in order of the species numbering scheme*/

          WriteSpeciesThermochemistry( f, Instance->ptr_to_species_list,
               Instance->speciescount, Instance->energy_units );

     }                                 /* end if variable temp                */

     /* now write out the execution data to the file, starting              */
     /* with concentrations                                                 */

     fprintf( f, "%s\n%u\n", msimEXECUTION_FLAG, Instance->nonzeroconcs );

     /* start at beginning of species list and write out data               */
     /* for all nonzero  concentrations                                     */

     species_ptr = Instance->ptr_to_species_list;

     while ( species_ptr )
     {
          if ( species_ptr->nonzero )
               fprintf( f, "%hu\n%s\n", species_ptr->index, ConcStrInMolarUnits
                    ( species_ptr->initialconc, Instance->conc_units,
                     Instance->volume_option ) );

          species_ptr = species_ptr->next;
     }                                 /* end while                           */

     /* then write out the simulation option value                       */

     tp_option = msimValidTPVCombo( Instance->variablepress, Instance->
          temp_option, Instance->volume_option );
     fprintf( f, "%hu\n", tp_option );

     switch ( tp_option )
     {
     case msimPROG_T_CONST_P_v :
     case msimPROG_T_VAR_P_CONST_V :
     case msimPROG_T_CONST_P_VAR_V :

          /* write out elements common to all prog t options                */

          fprintf( f, "%s\n%u\n", Instance->temp_data.max_step_size, Instance->
               temp_prog_data_format );

          if ( Instance->temp_prog_data_format == msimANALYTIC_TPROG )
          {

               /* write out the coefficients for temp programming           */
               /* if that option  has been selected                         */

               WriteAnalyticalTempProgData( f, &Instance->temp_data, Instance->
                    time_units );
          }
          else
          {                            /* == msimFILE_TPROG                   */
               msimRC rc;

               if ( msimNO_ERROR != ( rc = ReadTempProfileFile( Owner,
                                   Instance->temp_profile_data, f, Instance->time_units ) ) )
               {
                    fclose( f );
                    return rc;
               }

          }

          if ( tp_option != msimPROG_T_CONST_P_VAR_V )
               break;

          /* otherwise fall through to next case                      */

     case msimCONST_TP_VAR_V :
     case msimVAR_T_CONST_P_VAR_V :

          /* write out the data for doing variable volume             */
          /* if that option is activated - each species has           */
          /* its initial state and molar density                      */

     {
          USHORT i;

          /* scan through the species linked list and         */
          /* write out the the physical state data in order   */
          /* of the species numbering scheme                  */

          for ( i = 1; i <= Instance->speciescount; i++ )
          {

               /* start at beginning of list               */

               species_ptr = Instance->ptr_to_species_list;

               /* scan until we find data for species index = i*/

               while ( species_ptr->index != i )
                    species_ptr = species_ptr->next;

               // note that here the molar density has units of amt/volume
               // since the volume is in the denominator multiplying by
               // the msimConvFactorVolumeFromLiters return value gives
               // the desired conversion

               fprintf( f, "%u\n%-12.8E\n", species_ptr->phys_state, atof
                    ( species_ptr->molardensity )
                    *msimConvFactorVolumeFromLiters( Instance->volume_units ) );

          }                            /* end for                             */
     }
          break;

     default :

          break;

     }

     /* print the rest of the options contained in                       */
     /* the 'optionvalue' array                                          */

     {
          USHORT i;

          for ( i = 0; i < msimNUMBER_OF_OPTIONS; i++ )
               fprintf( f, "%s\n", Instance->optionvalue[i] );
     }

     /* then print the initial temperature and the temp convergence std     */
     /* and the status of the equil detect option flag                      */

     fprintf( f, "%s\n%s\n%lu\n%hu\n", initial_temp_str, Instance->
          temp_data.convergence_std, Instance->enable_equil_detect, Instance->
          time_units );

     fprintf( f, "%s\n%s\n%s\n", Instance->equil_data.min_eff, Instance->
          equil_data.cycle_length, TimeStrInSecUnits( Instance->elapsed_time_limit,
               Instance->time_units ) );

     /* test for errors - once should be enough since if an error occurred  */
     /* at any time during this output routine the error flag for f is set  */

     if ( ferror( f ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimWRITE_ERROR );
          fclose( f );
          return msimWRITE_ERROR;
     }

     fclose( f );
	 	 	 	 
#if defined(__MAC__) && defined(__PPC__)

     Sysdepen::SetFileInfo( String( Filename ), String( msimTEXTFILE_TYPE),
                  String( msimDEFAULT_CREATOR_NAME) );

#endif

     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        msimUpdateAfterSimulation( )                      */
/*..........................................................................*/
/*                                                                          */
/* This function is called whenever an ApplicationEvent is received from    */
/* a simulation process that is terminating. The user may have multiple     */
/* instances of the application running. To test whether the received       */
/* AppEvent is intended for this specific application instance, we compare  */
/* the current value of RunParmFilename to that passed from the terminating */
/* simulation process as a component of the ApplicationEvent struct. If     */
/* the two strings are identical then this is the parent of the terminating */
/* process. If so , we then open the reaction file whichi nows contains     */
/* simulation results and an updated header, read the header                */
/* record and update the instance struct appropriately. We also erase the   */
/* temporary file RunParmFilename used to pass initialization data to the   */
/* engine since it is no longer needed.                                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimUpdateAfterSimulation( msimWID Owner, const ApplicationEvent& rAppEvent )
{
     FILE *f;
     msimINSTANCE temp;

     // is this parent of simulation process sending Event ?
     if ( String( RunparmFilename ) != rAppEvent.GetData( ) )
          return msimNOT_PARENT_OF_CHILD;

#if defined(__MSDOS__)
     aMainApp.GetAppWindow( ) ->ToTop( );  // this keeps progman from popping up under Windows
#endif

     /* it's our child - go ahead and try to open the rxn file            */

     if ( NULL == ( f = fopen( SimInstance->filename, "rb" ) ) )
     {
          msimFileError( SimInstance->filename, Owner, ( USHORT )
               msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     fread( &temp, sizeof ( temp ), 1, f );

     if ( strcmp( SimInstance->msimID, msimRXN_FILE_FLG ) )
     {
          msimFileError( SimInstance->filename, Owner, ( USHORT )
               msimNOT_MSIM_FILE );
          fclose( f );
          *SimInstance = msimC_INSTANCE;
          return msimNOT_MSIM_FILE;
     }                                 /* endif                               */

     if ( ferror( f ) )
     {
          msimFileError( SimInstance->filename, Owner, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          return msimREAD_ERROR;
     }

     fclose( f );

     // we are done with file named RunparmFilename - delete it

     remove( RunparmFilename );

     /* here is where we update the record in SimInstance                */

     // be sure that if user aborted simulation that Instance.sim_return_code is
     // set correctly  - under Windows the Abort process does not give simulation
     // process a chance to clean things up and write out its termination state

     if ( String( ABORT_STRING ) == rAppEvent.GetEvent( ) )
          SimInstance->sim_return_code = msimCHILD_ABORT;
     else
          SimInstance->sim_return_code = temp.sim_return_code;

     SimInstance->num_simulation_data_records =
     temp.num_simulation_data_records;
     msimStringCopy( SimInstance->execution_time_str, temp.execution_time_str,
          sizeof SimInstance->execution_time_str );
     SimInstance->session_id = msimNO_SESSION;
     aMainApp.OKToStartSimulation = TRUE;

#if defined(__MAC__)

     // this is intended to ensure that window is updated - on MAC if Main Window
         // is hidden by other app windows when this fcn is called then
         // text is not updated

     aMainApp.GetAppWindow( )->UpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
#else

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

#endif


     // now try to retrieve another simulation from queue and start it
     msimStartDeferredSimulation( );

     return msimNO_ERROR;
}

void msimStartDeferredSimulation( )
{

     if ( aMainApp.OKToStartSimulation == TRUE &&
               msimSimQueue.IsAnotherSimulationReady( )
          )
     {
          msimPINSTANCE next_instance;

          msimSimQueue.RetrieveNextFromQueue( &next_instance, RunparmFilename,
               sizeof RunparmFilename );

#if defined(__OS2__)
          _beginthread( RunSimulation, THREAD_STACK_SIZE, ( void PTR ) next_instance );
#else

                RunSimulation( next_instance);
#endif
     }
}



void WriteRateConstantData( FILE *F, msimPRXN Reaction, USHORT ConcUnits, USHORT
          TimeUnits, USHORT EnergyUnits )
{
     msimFLOAT fwd_exponent, rev_exponent;

     /* now write out the rate constant data to outfil                   */
     /* if M and Ea are blank or null strings then print "0.0"           */
     /* we also do conversion to units the simulator expects before      */
     /* writing the string to file F. The simulator expects A factor data   */
     /* in l/mole-sec units and Ea in kcal/mole units                       */

     msimCalcAFactorConvExponent( Reaction, &fwd_exponent, &rev_exponent );

     fprintf( F, "%s\n%s\n%s\n", AFactorStrInSecUnits( Reaction->fwdA, TimeUnits
               , ConcUnits, fwd_exponent - 1.0 ),
          Reaction->singlerate ? msimREAL_ZERO_STRING : Reaction->fwdM,
          Reaction->singlerate ? msimREAL_ZERO_STRING :
               EaStrInKcalUnits( Reaction->fwdEa, EnergyUnits )
          );

     if ( Reaction->reversible )
     {
          fprintf( F, "%s\n%s\n%s\n", AFactorStrInSecUnits( Reaction->revA,
                    TimeUnits, ConcUnits, rev_exponent - 1.0 ),
               Reaction->singlerate ? msimREAL_ZERO_STRING : Reaction->revM,
               Reaction->singlerate ? msimREAL_ZERO_STRING :
                    EaStrInKcalUnits( Reaction->revEa, EnergyUnits )
               );
     }
     return;
}

void WriteSpeciesThermochemistry( FILE *F, msimPSPECIES StartOfList, USHORT
          SpeciesCount, USHORT EnergyUnits )
{
     USHORT i;
     msimPSPECIES species_ptr;
     msimFLOAT enthalpy, a, b, c, d;

     for ( i = 1; i <= SpeciesCount; i++ )
     {

          /* start at beginning of list                                     */

          species_ptr = StartOfList;

          /* scan until we find data for species index = i                  */

          while ( species_ptr->index != i )
               species_ptr = species_ptr->next;

          /* then convert all the thermochemical data for                   */
          /* species into the units expected by simulator                   */
          /* which is kcal or cal/ mole-deg^n)                              */

          enthalpy = atof( species_ptr->thermcoeff[0] )
          *msimConvFactorEnergyToCal( EnergyUnits );

          a = atof( species_ptr->thermcoeff[1] ) *msimConvFactorEnergyToCal
          ( EnergyUnits );

          b = atof( species_ptr->thermcoeff[2] ) *msimConvFactorEnergyToCal
          ( EnergyUnits );

          c = atof( species_ptr->thermcoeff[3] ) *msimConvFactorEnergyToCal
          ( EnergyUnits );

          d = atof( species_ptr->thermcoeff[4] ) *msimConvFactorEnergyToCal
          ( EnergyUnits );

          /* write it all to file F                                         */

          fprintf( F, "%-12.8E\n%-12.8E\n%-12.8E\n%-12.8E\n%-12.8E\n", enthalpy
               , a, b, c, d );
     }

     return;
}

void WriteAnalyticalTempProgData( FILE *F, msimPTEMP_COEFFS pTempData, USHORT
          CurrentTimeUnits )
{
     msimFLOAT a;

     a = atof( pTempData->progr_coeffA ) / msimConvFactorTimeToSec
     ( CurrentTimeUnits );

     fprintf( F, "%s\n%-12.8E\n", pTempData->max_temp, a );
     return;
}

PCHAR AFactorStrInSecUnits( PCHAR Str, USHORT TimeUnits, USHORT ConcUnits,
           msimFLOAT Exponent )
{
     static msimSTRING retval;
     msimFLOAT tempreal;

     tempreal = atof( Str );

     tempreal /= msimConvFactorTimeToSec( TimeUnits );

     tempreal /= pow( msimConvFactorConcToMolar( ConcUnits ), Exponent );

     sprintf( retval, "%-12.8E", tempreal );

     return retval;
}

PCHAR EaStrInKcalUnits( PCHAR Str, USHORT CurrentEnergyUnits )
{
     static msimSTRING retval;
     msimFLOAT tempreal;

     tempreal = atof( Str ) *msimConvFactorEnergyToCal( CurrentEnergyUnits );

     sprintf( retval, "%-12.8E", tempreal );

     return retval;
}

PCHAR ConcStrInMolarUnits( PCHAR Str, USHORT CurrentConcUnits, USHORT VolumeOption )
{
     static msimSTRING retval;
     msimFLOAT tempreal;

     // if this is a variable volume simulation then the conversion factor
     // does not include out any conversion in volume units

     if ( VolumeOption == msimVAR_VOL )
          tempreal = atof( Str ) * msimConvFactorAmtToMoles( CurrentConcUnits );
     else
          tempreal = atof( Str ) * msimConvFactorConcToMolar( CurrentConcUnits );

     sprintf( retval, "%-12.8E", tempreal );

     return ( PCHAR ) retval;
}

static PCHAR TimeStrInSecUnits( PCHAR Str, USHORT TimeUnits )
{
     static msimSTRING retval;
     msimFLOAT tempreal;

     tempreal = atof( Str ) *msimConvFactorTimeToSec( TimeUnits );

     sprintf( retval, "%-12.8E", tempreal );

     return ( PCHAR ) retval;
}

/*--------------------------------------------------------------------------*/
/*                        msimBuildSimulatorName()                          */
/*..........................................................................*/
/*                                                                          */
/* constructs a fully qualified name to be used in starting the simulation  */
/* engine. returns TRUE if successful in search, FALSE otherwise            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimBuildSimulatorName( VOID )
{
     return msimLocateAppFile( SIMULATION_PROGRAM, SimulatorFilename, sizeof SimulatorFilename );
}



/*--------------------------------------------------------------------------*/
/*                            ReadExtTempProfileFile()                      */
/*..........................................................................*/
/*                                                                          */
/* fcn opens a temperature profile file given by filename, reads it and     */
/* checks the data for validity. If invalid data is found, the data is      */
/* simply discarded and ignored. The fcn returns a count of the number      */
/* of valid data points. If a non-NULL FILE ptr is pased then the           */
/* valid data is written out to that file.  This is to                      */
/* be used with setting up the runparm.dat file for the simulation engine   */
/* A zero return value signifies an error of some sort                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC ReadTempProfileFile( msimWID Owner, const PCHAR Filename, FILE PTR OutFile,
            USHORT TimeUnits )
{
     msimTPRO_RC rc;

     TProfile temp_profile;

     // get the temperature profile file from disk

     if ( msimTPRO_NO_ERROR != ( rc = temp_profile.InitializeFromFile( Filename ) ) )
     {
          aMainApp.Wait( FALSE );

          temp_profile.ShowErrorMsg( Filename, rc, msimTPRO_STARTING_SIMULATION, Owner );

          return msimDATA_FILE_ERROR;
     }

     // now get number of data points

     if ( temp_profile.GetNumberOfValidDataPoints( ) )
          fprintf( OutFile, "%u\n", temp_profile.GetNumberOfValidDataPoints( ) );
     else
     {
          /* the temp profile data file has no valid data*/

          aMainApp.Wait( FALSE );

          temp_profile.ShowErrorMsg( Filename, msimTPRO_NO_DATA_PTS, msimTPRO_STARTING_SIMULATION, Owner );

          return msimDATA_FILE_ERROR;
     }


     // get the data, write it to runparm file
     // and check for errors

     if ( msimTPRO_NO_ERROR !=
               ( rc = temp_profile.WriteValidDataToTextFile( OutFile, TimeUnits ) ) )
     {
          aMainApp.Wait( FALSE );

          temp_profile.ShowErrorMsg( Filename, rc, msimTPRO_STARTING_SIMULATION, Owner );

          return msimDATA_FILE_ERROR;
     }

     return msimNO_ERROR;
}


void msimReschedule( )
{
     aMainApp.Reschedule( );
}


