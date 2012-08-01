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


#if defined(__MSDOS__)
#define FAR far
#else
#define FAR
#endif


// this defines the maximum number of system state records which can
// be saved by the simulator - it varies with the OS
#include <limits.h>
#define MAX_NO_RECORDS    ( ( UINT_MAX / sizeof( msimFLOAT) ) - (msimEXTRA_PLOT_ARRAY_PTS + 1 ) )

void FAR Reschedule( void );
USHORT FAR ReadInputData( PCHAR InputFile, PCHAR OutputFileName, SHORT PTR pMainWinX, SHORT PTR pMainWinY,
                SHORT PTR pMainWinWidth, SHORT PTR pMainWinHeight,
                USHORT PTR pTimeUnits );
void FAR RecordFatalErrorLocation( msimRC ErrorType, PCHAR pFilename, PCHAR ModuleName, PCHAR TimeStamp, int LineNo );
void FAR RunSimulation( void );
void FAR InitializeScreenDisplay( msimFLOAT ElapsedTime, ULONG EventCount,
              ULONG TotalNumberOfEvents );
USHORT FAR OpenOutputFile( PCHAR OutputFileName );
void FAR DisplayCalcStatus( msimFLOAT ElapsedTime, ULONG EventCount );
void FAR DisplaySimulationExitStatus( ULONG EventCount, msimFLOAT T, msimFLOAT ElapsedTimeLimit );
void FAR GracefulExit( void );
msimRC FAR SetUpCalculation( void );
void FAR SetReasonForExit( msimRC Reason );
msimRC FAR GetReasonForExit( void );
void FAR FatalError( void );
// PCHAR FAR GetExecutionTimeStr( void);
extern msimREAL_STRING ExecutionTime;




extern int RescheduleFlag;
ULONG StartRescheduleTimer( );

#if defined(__OS2__)
ULONG ResetPriority( );
void WaitForMSecs( ULONG Period );
#endif


