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

/* []---------------------------------------------------------------------[]

      ******************      IBM Internal Use Only      ******************

    SIM_IO.CXX contains those functions in the MSIM simulation engine
    which interact with the Starview Class library. This stuff is split
    out from the actual calculation function so as to allow them to
    be compiled using compilers/options that provide optimum performance

[]------------------------------------------------------------------------[]*/


#define __SIMEQUIL_BUILD__

#include "msim2.hxx"
#include "msimstrg.hxx"
#include "simequil.hrc"
#include "simequil.hxx"

#if defined (__OS2__)
#include <process.h>
#endif

#if defined(__AIX__) || defined(__MAC__)
#define cdecl
#endif

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>

/*    ----- define constants: -----                                         */

#define  CURRENT_TIME_TITLE                      "Current time in simulation :"
#define  EVENT_COUNT_TITLE                       "Current Event :"
#define  TIME_STRING                             "%5.2e %s"
#define  EVENT_STRING                            "%lu"
#define  EVENT_TEXT                              "of a total of %lu events"
#define  BUTTON_TEXT_INTERRUPT                   "Interrupt Simulation"
#define  MIN_SPACE_PIXELS                        20
#define PAUSE_PERIOD                               10.0
#define WAIT_PERIOD                                10.0

#if defined(__MAC__)

#define  RESCHEDULE_INTERVAL 12    // 60/sec

 clock_t NextReschedule;  // unsigned long under Sys 7

#endif

class simMainWin : public WorkWindow
{
public :
     FixedText aActiveFilename;
     GroupBox aGroupBox1;

     FixedText aStatusLine1;
     FixedText aStatusLine2;
     FixedText aStatusLine3;
     FixedText aStatusLine4;
     FixedText aStatusLine5;
     FixedText aStatusLine6;
     GroupBox aGroupBox2;

     PushButton aPushButton;

     simMainWin( );
     void PushBtnHandler( PushButton PTR );

protected :
     virtual void Paint( const Rectangle& rRect );

     virtual void KeyInput( const KeyEvent& rKEvent );

};

void simMainWin::KeyInput( const KeyEvent& rKeyEvt )
{
     // interrupt if enter key pressed

     if ( rKeyEvt.GetKeyCode( ) .GetCode( ) == KEY_RETURN )
          PushBtnHandler( NULL );
     return;
}



class simMainApp : public Application
{

public :
     void InitializeSimulation( );

#if defined(USE_THREAD)
static          void               InvokeSimulationLoop(void PTR);
       void                   DoIt(simMainApp PTR);
#else
     void InvokeSimulationLoop( simMainApp PTR );
#endif
     virtual void Main( int argc, char *argv[] );

#if defined(__MAC__)
    virtual void AppEvent( const ApplicationEvent& rAppEvent );
    void WaitForInput( simMainApp PTR );
#endif
};


static int SourceLineErrorLocation = 0;
static msimFILE_STRING ErrorModuleName = __FILE__;// initialized
static msimFILE_STRING ErrorTimeStamp = __TIME__;// initialized
static PCHAR pErrorSubjectFilename;

extern clock_t StopTime;
static clock_t StartTime;


static msimFILE_STRING OutputFileName;
static msimFILE_STRING ParentFilename;
static msimFILE_STRING RunParmFilename;

time_t StartWallClockTime;
msimREAL_STRING ExecutionTime;

/* the following are for the window display                                 */

SHORT MainWindowX, MainWindowY;
SHORT MainWindowWidth, MainWindowHeight;
USHORT TimeUnitsOption;

USHORT ReasonForExit = msimNO_ERROR;
msimBOOL PushButtonWasPressed = FALSE;
simMainWin PTR pAppWindow = NULL;

/* ---- function prototypes follow... ---------------------------------------*/

void Pause( void );

/* for handling IPC, exceptions and signals                                 */

static void NotifyParentOfTermination( );
static void cdecl SignalHandler( int sig );

/* instantiate the object asimMainApp                                       */

simMainApp asimMainApp;


#if defined(__MAC__)

msimBOOL GotRunParmFile = FALSE;

// this is how we get "command line"parameters from msim
// under Macintosh OS

void simMainApp::AppEvent( const ApplicationEvent& rAppEvent )
{
      if ( rAppEvent.IsOpenEvent() && !GotRunParmFile )
      {
          strncpy( RunParmFilename, rAppEvent.GetData(), sizeof RunParmFilename  );
           GotRunParmFile = TRUE;
           asimMainApp.InitializeSimulation( );

            NextReschedule = clock() + RESCHEDULE_INTERVAL;

             asimMainApp.InvokeSimulationLoop( &asimMainApp);
      }
}


#include <mac_start.h>

#include <processes.h>
#include <AppleEvents.h>
#include <StandardFile.h>



pascal OSErr QuitAppHandler( AppleEvent, AppleEvent, long )
 {
     raise(SIGTERM);
     return noErr;
 }


void simMainApp::WaitForInput( simMainApp PTR )
{
     RemoveIdleHdl( LINK( this, simMainApp, WaitForInput ) );

     time_t first = time( NULL );

     // the loop - wait for 10 secs for some IPC to get runparm filename

     while ( ! GotRunParmFile && ( difftime( time( NULL ), first ) < WAIT_PERIOD ) )
          asimMainApp.Reschedule( );

     // if we did not get the runparm filename by nowe then assume its an error

     if ( !GotRunParmFile)
     {
          RecordFatalErrorLocation( msimIPC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }


}





msimBOOL InstallEventHandlers()
{
      // install QuitApplication event handler
#if defined(__PPC__)
      return ( noErr == AEInstallEventHandler( kCoreEventClass,
       kAEQuitApplication, ( AEEventHandlerUPP) QuitAppHandler,
       0L, false ) );
#else
      return ( noErr == AEInstallEventHandler( kCoreEventClass,
       kAEQuitApplication, ( EventHandlerProcPtr) QuitAppHandler,
       0L, false ) );
#endif

 }

#include <mac_end.h>

#endif




/* function definitions follow                                              */
/* constructor for Main Window                                              */

simMainWin::simMainWin( ) :
WorkWindow ( NULL, ResId ( sim_MAIN_WINDOW ) ),
aActiveFilename ( this, ResId ( sim_ACTIVE_FILENAME ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aStatusLine1 ( this, ResId ( simLINE_1 ) ),
aStatusLine2 ( this, ResId ( simLINE_2 ) ),
aStatusLine3 ( this, ResId ( simLINE_3 ) ),
aStatusLine4 ( this, ResId ( simLINE_4 ) ),
aStatusLine5 ( this, ResId ( simLINE_5 ) ),
aStatusLine6 ( this, ResId ( simLINE_6 ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aPushButton ( this, ResId ( sim_PUSHBTN ) )
{
     FreeResource( );

#if !defined(__AIX__)
     ChangeIcon( ResId( simMAIN_ICON ) );
#endif

     /* install handler for pushbutton click                                 */

     aPushButton.ChangeClickHdl( LINK( this, simMainWin, PushBtnHandler ) );
}

void simMainWin::PushBtnHandler( PushButton PTR )
{
     PushButtonWasPressed = TRUE;

     if ( ReasonForExit == msimNO_ERROR )
     {
          aStatusLine4.SetText( String( "User interrupt pending..." ) );
          aStatusLine5.SetText( String( "Completing last" ) );
          aStatusLine6.SetText( String( "print cycle" ) );
     }
}

void simMainApp::InitializeSimulation( )
{
     /* read in input file - exit if an error is detected                   */

     if ( msimNO_ERROR != ReadInputData( RunParmFilename,
                    OutputFileName,
                    &MainWindowX, &MainWindowY,
                    &MainWindowWidth, &MainWindowHeight,
                    &TimeUnitsOption )
          )
          // location of fatal error recorded in fcn ReadInputData
          FatalError( );

     /*  now create the main window                                          */

     if ( msimNO_ERROR != OpenOutputFile( OutputFileName ) )
          FatalError( );

     if ( msimNO_ERROR != SetUpCalculation( ) )
          FatalError( );

#if !defined(__MAC__)

#if defined(USE_THREAD)
     InsertIdleHdl(LINK(pAppWindow, simMainApp, DoIt), 1);
#else
     InsertIdleHdl( LINK( pAppWindow, simMainApp, InvokeSimulationLoop ), 1 );
#endif

#if defined (__OS2__)
     ResetPriority( );
     StartRescheduleTimer( );
#endif

#endif

}


void simMainApp::Main( int argc, char *argv[] )
{
     EnableSVLook( );

     simMainWin aAppWindow;
     pAppWindow = &aAppWindow;


     time( &StartWallClockTime );

     StartTime = clock( );

     /* register termination sequence  */

     atexit( GracefulExit );

     /* set up handler for exception conditions                             */

     signal( SIGTERM, SignalHandler );
     signal( SIGSEGV, SignalHandler );
     signal( SIGFPE, SignalHandler );
     signal( SIGILL, SignalHandler );
     signal( SIGINT, SignalHandler );


#if defined(__MAC__)

    if ( !InstallEventHandlers() )
     {
          RecordFatalErrorLocation( msimIPC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     InsertIdleHdl( LINK( this, simMainApp, WaitForInput ), 1 );

#else

     /* first be sure that we were passed command line parameters             */

     if ( argc < 3 )
     {
          RecordFatalErrorLocation( msimCOMMANDLINE_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     msimStringCopy( ParentFilename, argv[2], sizeof ParentFilename );
     msimStringCopy( RunParmFilename, argv[1], sizeof RunParmFilename );

     InitializeSimulation( );

#endif


     Execute( );
}



#if defined(USE_THREAD)
void               simMainApp::DoIt(simMainApp PTR)
{
     RemoveIdleHdl(LINK(pAppWindow, simMainApp, DoIt));
     _beginthread( &simMainApp::InvokeSimulationLoop, 32000, NULL);
}

void        simMainApp::InvokeSimulationLoop(void  PTR)
{
     ResetThreadPriority();

#else

void simMainApp::InvokeSimulationLoop( simMainApp PTR )
{
     RemoveIdleHdl( LINK( pAppWindow, simMainApp, InvokeSimulationLoop ) );
#endif

     RunSimulation( );

     exit( msimNO_ERROR );
}



/*--------------------------------------------------------------------------*/
/*      Definition of Function InitializeScreeDisplay()                     */
/*                                                                          */
/*      Purpose is to initialize the static text in SimulationWindow        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void simMainWin::Paint( const Rectangle& rRect )
{
     Window::Paint( rRect );
     Update( );
}

void FAR InitializeScreenDisplay( msimFLOAT ElapsedTime, ULONG EventCount, ULONG TotalNumberOfEvents )
{
     Size dlg_size = pAppWindow->GetSizePixel( );

     SHORT dlg_width = dlg_size.Width ( );
     SHORT dlg_height = dlg_size.Height ( );

     Size screen = System::GetScreenSizePixel( );
     SHORT screen_width = screen.Width ( );
     SHORT screen_height = screen.Height ( );

     SHORT new_x, new_y;
     msimSTRING time_str, event_text;

     //  recalc coords for Dialog
     // center horizontally first

     new_x = ( MainWindowWidth - dlg_width ) / 2 + MainWindowX;

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = MIN_SPACE_PIXELS;

     // are we off right side of screen ??
     // if so then move back such that we are MIN_SPACE_PIXELS from right edge
     // of screen

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = screen_width - MIN_SPACE_PIXELS - dlg_width;
     // now center vertically

     new_y = ( MainWindowHeight - dlg_height ) / 2 + MainWindowY;
     if ( new_y < MIN_SPACE_PIXELS )
          new_y = MIN_SPACE_PIXELS;
     // are we off bottom ?

     if ( new_y + dlg_height > screen_height - MIN_SPACE_PIXELS )
          new_y = screen_height - MIN_SPACE_PIXELS - dlg_height;

     pAppWindow->ChangePosPixel( Point( new_x, new_y ) );

     pAppWindow->aActiveFilename.SetText( String( msimBaseFilename( OutputFileName ) ) );

     pAppWindow->aStatusLine1.SetText( String( CURRENT_TIME_TITLE ) );

     sprintf( time_str, TIME_STRING,
          ElapsedTime * msimConvFactorTimeFromSec( TimeUnitsOption ),
          msimTimeUnits( TimeUnitsOption ) );

     pAppWindow->aStatusLine2.SetText( String( time_str ) );

     pAppWindow->aStatusLine3.SetText( String( msimNULL_STR ) );

     pAppWindow->aStatusLine4.SetText( String( EVENT_COUNT_TITLE ) );

     pAppWindow->aStatusLine5.SetText( String( EventCount - 1 ) );

     sprintf( event_text, EVENT_TEXT, TotalNumberOfEvents );

     pAppWindow->aStatusLine6.SetText( String( event_text ) );

     pAppWindow->aPushButton.SetText( String( BUTTON_TEXT_INTERRUPT ) );

     pAppWindow->Show( );

     pAppWindow->Update( );
     return;
}

/*--------------------------------------------------------------------------*/
/*      Definition of Function DisplayCalcStatus()                          */
/*                                                                          */
/*      Purpose is to update the statics text in SimulationWindow           */
/*      so as to reflect the current status of the simulation               */
/*      We check for the pushbutton press here and exit if is has been      */
/*      pressed.                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void FAR DisplayCalcStatus( msimFLOAT ElapsedTime, ULONG EventCount )
{
     msimSTRING time_str;

     sprintf( time_str, TIME_STRING, ElapsedTime * msimConvFactorTimeFromSec

          ( TimeUnitsOption ), msimTimeUnits( TimeUnitsOption ) );

     pAppWindow->aStatusLine2.SetText( String( time_str ) );
     pAppWindow->aStatusLine5.SetText( String( EventCount - 1 ) );

     /* now check for button press to kill simulation                       */

     if ( asimMainApp.AnyInput( INPUT_ANY ) )
          asimMainApp.Reschedule( );

     if ( ReasonForExit == msimNO_ERROR )
     {

          if ( PushButtonWasPressed )
          {
               ReasonForExit = msimUSER_ABORT;
               exit( msimNO_ERROR );
          }
     }

     return;
}                                      /* end function DisplayCalcStatus      */




void FAR DisplaySimulationExitStatus( ULONG EventCount, msimFLOAT T, msimFLOAT ElapsedTimeLimit )
{
     msimSTRING et_str;
     msimSTRING tmp_str1, tmp_str2, tmp_str3;

     switch ( ReasonForExit )
     {
     case msimSEG_VIOLATION :
          sprintf( tmp_str1, "Segment violation in %s", ErrorModuleName );
          sprintf( tmp_str2, "Time stamp %s", ErrorTimeStamp );
          break;

     case msimMATH_ERROR :
          sprintf( tmp_str1, "Floating pt error in %s", ErrorModuleName );
          sprintf( tmp_str2, "Time stamp %s", ErrorTimeStamp );
          break;

     case msimILLEGAL_INSTR :
          sprintf( tmp_str1, "Illegal intruction in %s", ErrorModuleName );
          sprintf( tmp_str2, "Time stamp %s", ErrorTimeStamp );
          break;

     case msimOTHER :
          sprintf( tmp_str1, "Unknown error in %s", ErrorModuleName );
          sprintf( tmp_str2, "Time stamp %s", ErrorTimeStamp );
          break;

     case msimWRITE_ERROR :
          sprintf( tmp_str1, "Output error in %s line %d", ErrorModuleName,
               SourceLineErrorLocation );
          sprintf( tmp_str2, "Time stamp (%s)", ErrorTimeStamp );
          break;

     case msimMEM_ALLOC_ERROR :
          sprintf( tmp_str1, "Memory error in %s line %d", ErrorModuleName,
               SourceLineErrorLocation );
          sprintf( tmp_str2, "Time stamp (%s)", ErrorTimeStamp );
          break;

     case msimFILE_OPEN_ERROR :
          sprintf( tmp_str1, "File open error in %s line %d", ErrorModuleName,
               SourceLineErrorLocation );
          sprintf( tmp_str2, " Time stamp (%s)", ErrorTimeStamp );
          break;

     case msimINPUT_ERROR :
          sprintf( tmp_str1, "Input error in %s line %d", ErrorModuleName,
               SourceLineErrorLocation );
          sprintf( tmp_str2, "Time stamp (%s)", ErrorTimeStamp );
          break;

     case msimUSER_ABORT :

          strcpy( tmp_str1, "Simulation interrupted after" );
          sprintf( tmp_str2, "%lu events...", EventCount - 1 );
          break;

     case msimCHILD_ABORT :

          strcpy( tmp_str1, "Simulation aborted after" );
          sprintf( tmp_str2, "%lu events...", EventCount - 1 );
          break;

     case msimZERO_PROBABILITY :

          strcpy( tmp_str1, "All Probabilities are zero" );
          sprintf( tmp_str2, "after %lu events...", EventCount - 1 );
          break;

     case msimFINAL_TEMP_REACHED :

          sprintf( tmp_str1, "Final Temperature of %5.4g deg K", T );
          sprintf( tmp_str2, "reached after %lu events...", EventCount - 1 );
          break;

     case msimELAPSED_TIME_LIMIT :

          strcpy( tmp_str1, "Simulation reached elapsed time" );
          sprintf( tmp_str2, "limit of %5.2e %s...", ElapsedTimeLimit
               *msimConvFactorTimeFromSec( TimeUnitsOption ), msimTimeUnits
               ( TimeUnitsOption ) );
          break;

#if defined (LIMIT_OUTPUT)
     case msimSIM_RECORD_LIMIT :

          strcpy( tmp_str1, "System state has been recorded" );
          sprintf( tmp_str2, "%lu times (max allowed)...", MAX_NO_RECORDS );
          break;
#endif

     case msimSIM_EVENT_LIMIT :
     default :

          strcpy( tmp_str1, "Simulation reached event" );
          sprintf( tmp_str2, "limit of %lu events...", EventCount );
          break;

     }

     sprintf( tmp_str3, "This simulation ran for %s seconds", ExecutionTime );

#if  defined(__MSDOS__)
     strcpy(et_str, msimNULL_STR);

#else
     sprintf( et_str, "and used %5.1f sec processor time", ( msimFLOAT ) ( StopTime - StartTime ) /
          CLOCKS_PER_SEC );
#endif

     pAppWindow->aStatusLine1.SetText( String( tmp_str1 ) );
     pAppWindow->aStatusLine2.SetText( String( tmp_str2 ) );
     pAppWindow->aStatusLine3.SetText( String( msimNULL_STR ) );
     pAppWindow->aStatusLine4.SetText( String( tmp_str3 ) );
     pAppWindow->aStatusLine5.SetText( String( et_str ) );
     pAppWindow->aStatusLine6.SetText( String( msimNULL_STR ) );
     pAppWindow->aPushButton.SetText( String( "OK" ) );

     pAppWindow->Restore( );
     pAppWindow->Show( );
     pAppWindow->ToTop( );
     pAppWindow->Update( );
     Sound::Beep( );

     /* now wait for pushbutton /timeout                            */

     PushButtonWasPressed = FALSE;

     Pause( );

     NotifyParentOfTermination( );

     /* clean up the screen display stuff                                   */

}

void Pause( void )
{
#if defined(__MSDOS__) || defined(__MAC__) || defined(__AIX__)

     time_t first = time( NULL );

     while ( ! PushButtonWasPressed && ( difftime( time( NULL ), first ) < PAUSE_PERIOD ) )
          asimMainApp.Reschedule( );

#endif

#if defined(__OS2__)

     msimFLOAT t = PAUSE_PERIOD;

     while ( ! PushButtonWasPressed && ( t > 0.0 ) )
     {
          WaitForMSecs( 100 );
          asimMainApp.Reschedule( );
          t -= 0.1;
     }

#endif
}








#if defined(__PPC__)

msimBOOL  SendAnswerToParent()
{
	// first get parent PSN 
	
	OSErr                 err;

	ProcessSerialNumber psn;
	ProcessInfoRec      info;
	
	psn.highLongOfPSN = 0;
	psn.lowLongOfPSN  = kNoProcess;
	
	info.processInfoLength = sizeof( ProcessInfoRec );
	info.processName = NULL;
	info.processAppSpec = NULL;
	
	BOOL got_it = FALSE;
	
	while ( ( noErr == GetNextProcess( &psn ) ) && (FALSE == got_it ) )
	{
	     if ( noErr == GetProcessInformation( &psn, &info) )
		 {
		      if ( ( ( (long int) 'APPL') == info.processType ) &&
			       ( ( (OSType) 'ENGN') == info.processSignature ) )
				 got_it = TRUE;
	     }		 
				 
			// 	info.processLauncher should now contain parent psn 
	}		
		
	  if (FALSE == got_it )
	        return FALSE;
//			
	ProcessInfoRec      parentinfo;

    FSSpec                parent_filespec;

	parentinfo.processInfoLength = sizeof( ProcessInfoRec );
	parentinfo.processName = NULL;
	parentinfo.processAppSpec = &parent_filespec;
	
	GetProcessInformation( &info.processLauncher, &parentinfo);
	
	
//		 
	  // now send an "answer" event

      AEAddressDesc address;
      AppleEvent  ae, reply;
      AEDesc         parm_desc;

      FSSpec                runparm_filespec;
      char                  filename[256];
  
      // contruct a pascal-style string for the runparm filename - we pass this
      // to simulator after launch as an open documents Apple Event
      // note that the SV mwthod for sending ApplicationEvents does not work here
      // but the receipt of these events by the simultor using
      // the SV mechanism seems to work OK

      msimStringCopy( &filename[1], ParentFilename, ( sizeof filename) -1 );
      filename[0] = (unsigned char) strlen( &filename[1]);

      err = FSMakeFSSpec( 0, 0, (ConstStr255Param) filename, &runparm_filespec);

     if (err)
      {
          return FALSE;
      }


      // make the address descriptor using the parent process serial number we got 

      err = AECreateDesc( typeProcessSerialNumber, (Ptr) &parentinfo.processNumber,
      sizeof( ProcessSerialNumber), &address);

      if ( err)
      {
          return FALSE;
      }

      // create the event structure

      err = AECreateAppleEvent( kCoreEventClass, kAEOpenDocuments, &address,
      kAutoGenerateReturnID, 1L, &ae );
      if ( err)
      {
          return FALSE;
      }

     // create a descriptor containing the FSSpec describing the run parm file

     err = AECreateDesc( typeFSS, &parent_filespec, sizeof( parent_filespec), &parm_desc );
      if ( err)
      {
          return FALSE;
      }

     // add the descriptor to the AppleEvent

     err = AEPutParamDesc( &ae, keyDirectObject,
                &parm_desc );
      if ( err)
      {
          return FALSE;
      }

     // the AppleEvent is complete - send it on its way

      err = AESend( &ae, &reply, kAENoReply + kAECanInteract, kAENormalPriority,
      kAEDefaultTimeout, NULL, NULL );
      if ( err)
      {
          return FALSE;
      }


	  return TRUE;			 

}
#endif







void NotifyParentOfTermination( )
{

// do nothing under OS/2 - parent is watching queue for msg from child ( this)

// post event to parent under Win 3.1

#if defined(__MSDOS__) || defined(__MAC__)

     #if defined(__PPC__)
	 
         SendAnswerToParent();

     #else	  
          // post event to tell user interface of completion of simulation
		  
          asimMainApp.PostAppEvent(String(ParentFilename), String(msimNULL_STR),
           String("close"), String(RunParmFilename));
     #endif

#endif

// under AIX we are watching pipe using X fcns for msg from this child

#if defined(__AIX__)

     /* this snippet writes a message to stdout to notify parent of completion*/

     setbuf(stdout, NULL);
     fprintf(stdout, msimDONE_MSG);
     fflush(stdout);

#endif

}

/* fcn to deal with receipt of SIGTERM                                      */


void cdecl SignalHandler( int sig )
{
     switch ( sig )
     {
     case SIGTERM :
     case SIGINT :

          ReasonForExit = msimCHILD_ABORT;
          break;

     case SIGSEGV :

          ReasonForExit = msimSEG_VIOLATION;
          break;

     case SIGFPE :

          ReasonForExit = msimMATH_ERROR;
          break;

     case SIGILL :

          ReasonForExit = msimILLEGAL_INSTR;
          break;

     default :

          ReasonForExit = msimOTHER;
          break;

     }
     exit( sig );
     signal( sig, SIG_DFL );

}


void FAR FatalError( void )
{

     String message;

     switch ( ReasonForExit )
     {

     case msimCOMMANDLINE_ERROR :
          message = "Error in simulator command line.\nAborting simulation.";
          break;

     case msimIPC_ERROR :
          message = "Error in interprocess communication.\nAborting simulation.";
          break;


     case msimINPUT_ERROR :

          message = "Error in format of input file " + String( pErrorSubjectFilename ) +
          "\nin module " + String( ErrorModuleName ) + ", line " + String
          ( SourceLineErrorLocation ) + ".\nModule time stamp (" + String
          ( ErrorTimeStamp ) + ").";
          break;

     case msimMEM_ALLOC_ERROR :
          message = "Memory error in module " + String( ErrorModuleName ) + ", line " +
          String( SourceLineErrorLocation ) + ". Module time stamp (" + String
          ( ErrorTimeStamp ) + ").";
          break;

     case msimFILE_OPEN_ERROR :
          message = "Error opening file " + String( pErrorSubjectFilename ) + "\nin module "
               + String( ErrorModuleName ) + ", line " + String( SourceLineErrorLocation ) +
          ".\nModule time stamp (" + ErrorTimeStamp + ").";
          break;

     case msimREAD_ERROR :
     default :
          message = "Error reading file " + String( pErrorSubjectFilename ) + "\nin module "
               + String( ErrorModuleName ) + ", line " + String( SourceLineErrorLocation ) +
          ".\nModule time stamp (" + ErrorTimeStamp + ").";
          break;
     }

     ErrorBox( NULL, WB_OK | WB_DEF_OK, message ) .Execute( );

     NotifyParentOfTermination( );
     asimMainApp.Abort( message );

}


void FAR SetReasonForExit( msimRC Reason )
{
     ReasonForExit = Reason;
}

msimRC FAR GetReasonForExit( void )
{
     return ReasonForExit;
}

void FAR RecordFatalErrorLocation( msimRC ErrorType, PCHAR pFilename, PCHAR ModuleName, PCHAR TimeStamp, int LineNo )
{
     ReasonForExit = ErrorType;
     strcpy( ErrorModuleName, ModuleName );
     strcpy( ErrorTimeStamp, TimeStamp );
     SourceLineErrorLocation = LineNo;
     pErrorSubjectFilename = pFilename;
}


void FAR Reschedule( void )
{
#if defined(__MAC__)

      if (clock() > NextReschedule)
      {
//         if ( asimMainApp.AnyInput( INPUT_ANY) ) // old form 
//         if ( asimMainApp.AnyInput( INPUT_ANY) || !pAppWindow->IsVisible() )
               asimMainApp.Reschedule( );

//         if (pAppWindow->HasFocus( ) )
//          {
               NextReschedule = clock() + RESCHEDULE_INTERVAL;
//          }
//          else
//          {
//               NextReschedule = clock() + ( 2 * RESCHEDULE_INTERVAL);
//          }
      }
#else
     asimMainApp.Reschedule( );
#endif

#if defined(__OS2__)
     RescheduleFlag = 0;
#endif

}


