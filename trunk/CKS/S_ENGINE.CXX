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

/* []------------------------------------------------------------------------[]
   |                                                                          |
   |   ******************      IBM Internal Use Only      ******************  |
   |                                                                          |
   | SIMEQUIL- stochastic mechanism simulator - portions derived from         |
   | the program MSIM4 by D. L. Bunker and F. A. Houle UC Irvine 1974         |
   | and the program MSIMPC v 2.0 by W. D. Hinsberg and F. A. Houle,          |
   | IBM, 1991.                                                               |
   |                                                                          |
   | This version of the program is derived from SIMULATE.C, a C              |
   | version of the simulator engine originally written in FORTRAN,           |
   | then revised for BASIC.  Its data structure has been significantly       |
   | modified to conform to the new standard devised for simulation of        |
   | multiple processes in inhomogeneous systems.  Species                    |
   | concentrations, stoichiometry information, kinetics data,                |
   | thermochemical data and probability data are all arranged in             |
   | separate groupings to comprise the four classifications of system        |
   | data (concentrations), process data (stoichiometry and kinetics),        |
   | probability data and materials property data (thermochemistry).          |
   | This will allow the pertinent functions to be used in a full             |
   | inhomogeneous simulator directly.  The program is written in ANSI        |
   | C.                                                                       |
   |                                                                          |
   | The program reads its input data from a file whose name is specied       |
   | on the command line as the first parameter. The input file               |
   | was created by the shell MSIMPC.  RUNPARM.DAT is an ASCII                |
   | file containing a description of the reaction stoichiometries, the       |
   | rate laws, initial conditions and option settings for the                |
   | simulation.                                                              |
   |                                                                          |
   | The output of the program is a binary file containing periodic           |
   | recordings of the state of the simulation.  The output is in IEEE        |
   | binary format.  For debugging purposes an ASCII output option is         |
   | available.                                                               |
   |                                                                          |
   | The functioning of the simulator is the same as the rest of the          |
   | MSIM class of simulators, except that two new variable temperature       |
   | options are supported:  programmed T, constant P, and programmed         |
   | T, variable P. Temperature variations are assumed to be uniform          |
   | throughout the simulation volume.  It has an equilibrium detect          |
   | and update option which significantly speeds up simulation of            |
   | coupled equilibria.                                                      |
   |                                                                          |
   | Programmers:  F. A. Houle and W. D. Hinsberg, IBM Almaden Research       |
   | Center.                                                                  |
   |                                                                          |
   | started 6/92, last modified 4/05/93.                                     |
   |                                                                          |
   | A shell for the simulation engine that is based on Devbase has been      |
   | added. It provides for a screen display of status of the simulation      |
   | and for a pushbutton to stop the simulation.                             |
   |                                                                          |
   []------------------------------------------------------------------------[]*/


/* The following #define determines whether output is in binary or
   ASCII format - leave ASCII_OUTPUT undefined to produce a binary file     */


#undef   ASCII_OUTPUT


#include "msim1.hxx"
#include "msimstrg.hxx"

#include "simequil.hxx"

/* ANSI C control files  */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>


// -----------------------------------------------------------------------
//
// wdh 10.11.94
// the following #defines are for profiling the internal operation of the
// simulation engine. The values of various data members are recorded to
// the file named PROFILE_FILENAME during every loop through the calculation
// if profiling is activated. To enable profiling, #define PROFILE to be
// the logical OR of all the data members you want to record; eg.
//
//     #define PROFILE     ( RECORD_INIT_STATE | RECORD_PROBS )
//
// To disable profiling, #define PROFILE to be RECORD_NOTHING

// wdh 10.21.96
// added code in SavesimulatorState to record info useful for establishing
// the rxn file structure for those cases where the rxn was interrupted by the 
// user. This code is activated by defining RECORD_SAVED_STATE

#define PROFILE_FILENAME      "simequil.prf"

#define RECORD_NOTHING        0x00
#define RECORD_CONCS          0x01
#define RECORD_PROBS          0x02
#define RECORD_EQUIL_TABLE    0x04
#define RECORD_INIT_STATE     0x08
#define RECORD_GLOBALS        0x10
#define RECORD_SAVED_STATE    0x20

#define RECORD_ALL            ( RECORD_CONCS | RECORD_PROBS | RECORD_EQUIL_TABLE | RECORD_INIT_STATE | RECORD_GLOBALS | RECORD_SAVED_STATE )

#define PROFILE            ( RECORD_NOTHING )

#if ( PROFILE != RECORD_NOTHING )

#include <fstream.h>
#include <iomanip.h>
#include <time.h>

msimSTRING PrfRxnFileName;
void PrfOpenFile( );
void PrfRecordInitState( );
void PrfRecordCurrentState( );
void PrfRecordEquilTable( );
void PrfCloseFile( );
void PrfRecordNonEquilRxnSelection( double Lambda, double DominantProbability );

ofstream  PrfFile;

ostream& tab( ostream& output )
{
     return output << "\t";
}

#define VAR_NAME_WIDTH     32
#define NUMERIC_WIDTH      20

#define VALUE( var )    tab << setw( VAR_NAME_WIDTH ) << #var << " = " << setw( NUMERIC_WIDTH ) << var << endl

msimNAME_STRING PTR PrfSpecNameArray;
msimEQUATION_STRING PTR PrfEquationArray;


#endif

// --------- end of profiling header -------------------------------------------------------------


extern time_t StartWallClockTime;
clock_t StopTime;

/*    ----- define constants: -----    */

#define  STANDARD_VOLUME                     ((double) 1.0)
#define  GAS_CONSTANT                            0.08205
#define  R                                       0.00198726
#define  EPSILON                                 ( (double) 0.0001 )
#define  MINIMUM_EXPONENT                        ( ((DBL_MIN_10_EXP+100) * \
             2.302585093) )

#define  EQUIL_DETECT                            1
#define  EQUIL_DETECT_LATER                      2

#define  POS                                     1
#define  NEG                                     -1

#define  IDLING_T_STEP                           0.10


/*  the following is specific to ASCII output                               */

#ifdef   ASCII_OUTPUT
#define  NUMBER_PER_LINE                         9
#define  OUTPUT_FORMAT                           "w"

#else
#define  OUTPUT_FORMAT                           "rb+"
#endif



/* ---------- declaration of data types------------- */

typedef struct
{
     unsigned int numreactants;
     unsigned int ReactantsInStep[msimMAX_NUMBER_OF_COMPONENTS];
     unsigned int ReactantStoich[msimMAX_NUMBER_OF_COMPONENTS];
     unsigned int numproducts;
     unsigned int ProductsInStep[msimMAX_NUMBER_OF_COMPONENTS];
     unsigned int ProductStoich[msimMAX_NUMBER_OF_COMPONENTS];
} STOICH_STRUCT;

typedef struct
{
     unsigned int numreactants;
     unsigned int ReactantsInStep[msimMAX_NUMBER_OF_COMPONENTS];
     double ReactantOrder[msimMAX_NUMBER_OF_COMPONENTS];
     double RateConstant[3];
     double UnitsConversionExponent;
} KINETICS_STRUCT;

typedef struct
{
     unsigned int Phase;
     double Density;                   /* moles/liter, from
                                          (grams/liter)*(mole/gram)           */
     double InitNoOfMoles;
} PHASE_STRUCT;

typedef struct
{
     double enthalpy;
     double coeffA;
     double coeffB;
     double coeffC;
     double coeffD;
} THERMO_STRUCT;

typedef struct
{
     double FinalTemp;
     double MaxDeltaTemp;
     double MaxDeltaTime;
     double IdlingTimeStep;
     double TRamp;
     int Slope;
} FUNCTION_STRUCT;



/* this struct is for use in saving the simulator state for resuming        */
/* an interrupted simulation                                                */

typedef struct
{
     unsigned long InitialPhase;       // needs a cast
     unsigned long TempAndPressOption; // needs a cast
     unsigned long EquilDetectOption;  // needs a cast
     unsigned long ProgTFileIndex;     // needs a cast
     unsigned long NewIndex;           // needs a cast
     unsigned long EventCount;
     unsigned long EquilTestCycleCount;
     unsigned long NumberOfRecordsWritten;
     double TotalProbability;
     double TotalRxnProbability;
     double ParticlesPerMol;
     double V;
     double P;
     double T;
     double TotalEnthalpy;
     double TotalHeatCapacity;
     double ConversionFactor;
     double MinFrequency;
     double PreviousTotalRxnProbability;
     double ElapsedTime;
     unsigned long NoOfProgTFilePoints;// needs a cast
     signed short RandomNoSeed[3];
     // this duplicates FUNCTION_STRUCT
     double FinalTemp;
     double MaxDeltaTemp;
     double MaxDeltaTime;
     double IdlingTimeStep;
     double TRamp;
     long Slope;                       // needs a cast

} STATE_STRUCT;



/* ---- define arrays and structures -----------------------------------------*/

FUNCTION_STRUCT Tfunction;



/*-----------define pointers to arrays----------------------------------------*/

unsigned long *pNP = ( unsigned long * ) NULL;/* NumberOfSpecies + 1          */
unsigned long *NoPrtcls = ( unsigned long * ) NULL;
unsigned int *pUT = ( unsigned int * )
NULL;                                  /* NumberOfRxns*NumberOfRxns+1         */
unsigned int *UpdateTable = ( unsigned int * )
NULL;
double *pRP = ( double * )
NULL;                                  /* NumberOfRxns                        */
double *RxnProbs = ( double * )
NULL;
double *pTP = ( double * )
NULL;                                  /* NumberOfRxns                        */
double *TempProbArray = ( double * )
NULL;
long *pRE = ( long * )
NULL;                                  /* NumberOfReactions                   */
long *RxnEventCtr = ( long * )
NULL;
unsigned int *pEqR = ( unsigned int * )
NULL;                                  /* NumberOfReactions/2                 */
unsigned int *EquilRxns = ( unsigned int * )
NULL;
double *pPTF = ( double * )
NULL;                                  /* 2*NoOfProgTFilePoints               */
double *ProgTempFile = ( double * )
NULL;



/*------------------------- define pointers to structures---------------------*/

THERMO_STRUCT *pThD = ( THERMO_STRUCT * ) NULL;/* dim. NumberOfSpecies +1     */
THERMO_STRUCT *ThermoData = ( THERMO_STRUCT * ) NULL;
STOICH_STRUCT *pStD = ( STOICH_STRUCT * ) NULL;/* dim. NumberOfRxns           */
STOICH_STRUCT *StoichData = ( STOICH_STRUCT * ) NULL;
KINETICS_STRUCT *pKD = ( KINETICS_STRUCT * ) NULL;/* dim. NumberOfRxns        */
KINETICS_STRUCT *KineticsData = ( KINETICS_STRUCT * ) NULL;
PHASE_STRUCT *pSpD = ( PHASE_STRUCT * ) NULL;/* dim. NumberOfSpecies +1       */
PHASE_STRUCT *SpeciesData = ( PHASE_STRUCT * ) NULL;



/* ---- non-array variable definitions --------------------------------------*/

double ElapsedTime, PreviousElapsedTime;
double DeltaTime, MeanDeltaTime, WaitingTime;
double TotalRxnProbability, PreviousTotalRxnProbability;
double TotalProbability;

double ParticlesPerMol, ConversionFactor;
double TotalHeatCapacity, TotalEnthalpy, P;/* ->>Internal
                                              values not normalized           */
double V;                              /* to actual moles in system,
                                          proportional                        */
double T, TConvergence, InitialT;
double DeltaTemp;
double RandomNo;
double ElapsedTimeLimit;

unsigned int TempAndPressOption;
unsigned int NoOfSpecWithNonZeroInitConc;
unsigned int InitialPhase = msimGAS;   /* default                             */
unsigned int NumberOfSpecies;
unsigned int NumberOfRxns;             /* 2x the number of steps, runs
                                          0..2n-1 to index arrays             */
unsigned int UpdateTableOffset;
unsigned int SelectedRxnNo;
unsigned int OutOfRange;

unsigned long WriteIntervalCount;
unsigned long EventCount;
unsigned long TotalNumberOfEvents;
unsigned long WriteInterval;
unsigned long NumberOfRecordsWritten = 0L;

long TotalNoOfParticles;
unsigned int NoOfProgTFilePoints;
unsigned int ProgTFileIndex;
unsigned int NewIndex;                 /* position in progT file              */
long ProgTFormat;                      /* polynomial or file                  */
unsigned long EquilTestCycle;

msimBOOL TempDependentRateConst = FALSE;
msimBOOL RxnIsReversible, spkin;
msimBOOL ReactionOver = FALSE;
msimBOOL FinalTempReached = FALSE;
msimBOOL OutputTempAnyway = FALSE;
msimBOOL ResumeSimulation = FALSE;

FILE *OutFile = NULL;
long ResumeDataOffset;



/* The following are for the equilibrium detect option                      */

unsigned long EquilTestCycleCount;
double MinEfficiency, MinFrequency;
unsigned int NumberOfEquilRxns;
msimBOOL DominantRxn = FALSE;
unsigned int EquilDetectOption;



/* ---- function prototypes follow... ---------------------------------------*/

int SetUpArrays( void );
int SetUpUpdateTables( void );
void CalcAllRxnProbabilities( void );
double RxnProbCalc( unsigned int n );
void UpdateRxnProbabilities( void );
void UpdateRxnStoichiometry( void );
void UpdateTime( void );
void SelectEvent( void );
void SelectAnyRxn( void );
int EnoughReactantsTest( unsigned int );
void TempPressUpdate( void );
void ProgTUpdate( void );
void AdiabaticPressUpdate( void );
void IsothermalPressUpdate( void );
void UpdateThermo( void );
void CondensedVolUpdate( void );
void GasVolUpdate( void );
unsigned long round( double x );
void TerminateProgT( void );
void ProgTSystemIdler( void );
void GetSlopeAndMaxDeltaTime( void );
void JumpStart( void );
void IdlingTimeStep( void );
void ConcentrationUpdate( double OldV );
void ZeroProbability( void );
void SaveSimulatorState( void );
void RestoreSimulatorState( void );

void OutputCalcStatus( void );



/* The following are specific to the Equilibrium detect and update option   */

void CheckForEquilibration( void );
void SelectNonEquilRxn( void );

// used in rn generator class

const long int SEED_1 = 30873;
const long int SEED_2 = 28541;

class RandomNumberSeries
{

protected :

      // actual values of seeds when saved will always fit into a
      // short int ( 2 bytes )
      // we defined them as long ints so that overflow does not
      // occur during mathematical manipulations in method NextValue()

     long int internal_seed1;
     long int internal_seed2;
     long int external_seed;

public:
     RandomNumberSeries( const short int ExternalSeed );

     double NextValue( );

     void ResetSeed( const short int ExternalSeed )
     {
          external_seed = ExternalSeed;
          internal_seed1 = SEED_1;
          internal_seed2 = SEED_2;
     }


     void GetState( signed short SeedArray[] )
     {
          SeedArray[0] = external_seed;
          SeedArray[1] = internal_seed1;
          SeedArray[2] = internal_seed2;
     }

     void SetState( signed short SeedArray[] )
     {
          external_seed = SeedArray[0];
          internal_seed1 = SeedArray[1];
          internal_seed2 = SeedArray[2];
     }

};


RandomNumberSeries::RandomNumberSeries( const short int ExternalSeed = 1 )
{
     internal_seed1 = SEED_1;
     internal_seed2 = SEED_2;
     external_seed  = ExternalSeed;
}



double RandomNumberSeries::NextValue()
{

#if defined (__MAC__)
	#if defined(__PPC__)
         double dummy;
	#else
    	long double dummy;
	#endif	 
#else
     double dummy;
#endif

    // recalc all three internal random numbers

     external_seed = ( 171 * external_seed ) % 30269;

     internal_seed1 = ( 172 * internal_seed1 ) % 30307;

     internal_seed2 = ( 170 * internal_seed2 ) % 30323;

     return ( modf( external_seed / 30269.0 + internal_seed1 / 30307.0
                    + internal_seed2 / 30323.0, &dummy ));
}

// instantiate random number series
RandomNumberSeries RandomNum;


// function definitions follow ...

void FAR RunSimulation( void )
{
     /* main Simulation loop                                                */

#if ( PROFILE != RECORD_NOTHING )

     PrfOpenFile( );

#if ( PROFILE & RECORD_INIT_STATE )

     PrfRecordInitState( );

#endif

#endif


     ReactionOver = FALSE;
     InitializeScreenDisplay( ElapsedTime, EventCount, TotalNumberOfEvents );

     while ( EventCount < TotalNumberOfEvents && ! ReactionOver )
     {
          for ( WriteIntervalCount = 1; WriteIntervalCount <= WriteInterval;
                    WriteIntervalCount++, EventCount++ )
          {

#if  defined(__OS2__)
               if ( RescheduleFlag )
#endif
                    Reschedule( );

               SelectEvent( );
               if ( ReactionOver )
                    break;
               UpdateTime( );
               UpdateRxnStoichiometry( );
               TempPressUpdate( );
               UpdateRxnProbabilities( );
               TotalProbability = TotalRxnProbability;

               if ( EventCount >= TotalNumberOfEvents )
               {
                    SetReasonForExit( msimSIM_EVENT_LIMIT );
                    break;
               }

#if defined(LIMIT_OUTPUT)
               if ( NumberOfRecordsWritten >= MAX_NO_RECORDS )
               {
                    SetReasonForExit( msimSIM_RECORD_LIMIT );
                    ReactionOver = TRUE;
                    break;
               }
#endif

               if ( EquilDetectOption )
               {
                    EquilTestCycleCount++;
               }


          }                            /* end for WriteIntervalCount          */



#if (  PROFILE & ( RECORD_CONCS | RECORD_PROBS | RECORD_GLOBALS ) )
          PrfRecordCurrentState( );
#endif
          OutputCalcStatus( );
          DisplayCalcStatus( ElapsedTime, EventCount );

     }                                 /* endwhile EventCount                 */

#if ( PROFILE != RECORD_NOTHING )

     PrfCloseFile( );

#endif

     return;
}

/*-----------------------------------------------------------------------------
Definition of function SetUpArrays
Purpose: to allocate storage for working arrays and set pointers
Parameters: none
Calls: nothing
Returns: value of NULL if allocation is unsuccessful for any array
---------------------------------------------------------------------------------*/

int SetUpArrays( void )
{

     /* allocate space and pointer for ThermoData[NumberOfSpecies+1]        */

     pThD = new THERMO_STRUCT[NumberOfSpecies + 1];

     if ( pThD == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else
          memset( pThD, '\0', ( NumberOfSpecies + 1 ) *sizeof ( THERMO_STRUCT ) );

     ThermoData = pThD;

     /* allocate space and pointer for NoPrtcls[NumberOfSpecies+1]          */

     pNP = new unsigned long[NumberOfSpecies + 1];

     if ( pNP == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pNP, '\0', ( NumberOfSpecies + 1 ) *sizeof ( long ) );

     NoPrtcls = pNP;

     /* allocate space and pointer for StoichData[NumberOfRxns]             */

     pStD = new STOICH_STRUCT[NumberOfRxns];

     if ( pStD == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pStD, '\0', NumberOfRxns * sizeof ( STOICH_STRUCT ) );

     StoichData = pStD;

     /* allocate space and pointer for KineticsData[NumberOfRxns]           */

     pKD = new KINETICS_STRUCT[NumberOfRxns];

     if ( pKD == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pKD, '\0', NumberOfRxns * sizeof ( KINETICS_STRUCT ) );

     KineticsData = pKD;

     /* allocate space and pointer for RxnProbs[NumberOfRxns]               */

     pRP = new double[NumberOfRxns];

     if ( pRP == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pRP, '\0', NumberOfRxns * sizeof ( double ) );
     RxnProbs = pRP;

     /* allocate space and pointer for TempProbArray[NumberOfRxns]          */

     pTP = new double[NumberOfRxns];
     if ( pTP == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pTP, '\0', NumberOfRxns * sizeof ( double ) );

     TempProbArray = pTP;

     /* allocate space and pointer for RxnEventCtr[NumberOfRxns]            */

     pRE = new long[NumberOfRxns];

     if ( pRE == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pRE, '\0', NumberOfRxns * sizeof ( long ) );

     RxnEventCtr = pRE;

     /* allocate space and pointer for EquilRxns[NumberOfRxns/2]            */

     pEqR = new unsigned int[NumberOfRxns / 2];

     if ( pEqR == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pEqR, '\0', ( NumberOfRxns / 2 ) *sizeof ( unsigned int ) );
     EquilRxns = pEqR;

     /* allocate space and pointer for SpeciesData[NumberOfSpecies+1]       */

     pSpD = new PHASE_STRUCT[NumberOfSpecies + 1];
     if ( pSpD == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pSpD, '\0', ( NumberOfSpecies + 1 ) *sizeof ( PHASE_STRUCT ) );

     SpeciesData = pSpD;

     return msimNO_ERROR;

}                                      /* end function SetUpArrays            */



/* +-------------------------------------------------------------------------+
   | definition of FUNCTION ReadInputData                                    |
   |                                                                         |
   | Purpose : to initialize variables using information obtained from the   |
   | ASCII file INPUT_FILE.                                                  |
   |                                                                         |
   | Calls functions SetUpArrays                                             |
   |                                                                         |
   | Parameters : the input file name and path as an ASCIIZ string.          |
   |                                                                         |
   | Returns: an integer return code ( nonzero if an error has occurred)     |
   |                                                                         |
   | Modifes almost ALL the global variables using values from INPUT_FILE    |
   |  calls function SetUpArrays                                             |
   +-------------------------------------------------------------------------+ */

USHORT FAR ReadInputData( PCHAR InputFile, PCHAR OutputFileName, SHORT PTR pMainWinX, SHORT PTR pMainWinY,
                SHORT PTR pMainWinWidth, SHORT PTR pMainWinHeight,
                USHORT PTR pTimeUnits )
{                                      /* start function ReadInputData        */

     /* local variables                                                     */

     unsigned int tmp;
     FILE *InFile;
     char TempString[20];
     unsigned int RxnNo, jr, j, jp;

     /* start                                                               */

     if ( ( InFile = fopen( InputFile, "r" ) ) == NULL )/* open for read-only */
     {
          RecordFatalErrorLocation( msimFILE_OPEN_ERROR, InputFile, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimFILE_OPEN_ERROR;
     }
     else
     {
          // this allows reading of OutputFileName with blanks in it ( MAC )

          fgets( OutputFileName, sizeof ( msimFILE_STRING ), InFile );

          int last_char_pos = strlen ( OutputFileName ) - 1;

          if ( OutputFileName[last_char_pos] == '\n' )
               OutputFileName[last_char_pos] = '\0';

#if ( PROFILE != RECORD_NOTHING )
          strcpy( PrfRxnFileName, OutputFileName ); // save for later write to profile file header
#endif

          fscanf( InFile, "%hd %hd %hd %hd %s %u %u",
               pMainWinX, pMainWinY, pMainWinWidth, pMainWinHeight,
               TempString, &NumberOfSpecies, &NumberOfRxns );
          NumberOfRxns *= 2;

#ifdef   ASCII_OUTPUT
#define  DEBUG_FILE                              "sim.out"
          strcpy(OutputFileName, DEBUG_FILE);
#endif

          /* now check for validity of file   */

          ResumeSimulation = ( msimBOOL ) ( strcmp( TempString, msimRESUME_SIM_FLAG
               ) == 0 );

          if ( ( strcmp( TempString, msimMECHANISM_FLAG ) != 0 ) &&
                    ! ResumeSimulation )
          {
               RecordFatalErrorLocation( msimINPUT_ERROR, InputFile, __FILE__, __TIMESTAMP__, __LINE__ );
               return msimINPUT_ERROR;
          }

          /* allocate array space for storage of all variables  and intialize pointers*/

          if ( SetUpArrays( ) != msimNO_ERROR )
               return msimMEM_ALLOC_ERROR;

          /* now read in reaction data                                      */

          RxnNo = 0;
          while ( RxnNo < NumberOfRxns )
          {
               fscanf( InFile, "%u", &StoichData[RxnNo].numreactants );

               for ( j = 0; j < ( pStD + RxnNo ) ->numreactants; j++ )
               {
                    fscanf( InFile, "%u %u", &StoichData[RxnNo].
                         ReactantsInStep[j], &StoichData[RxnNo].ReactantStoich[j] )
                    ;
               }                       /* end for j                           */
               fscanf( InFile, "%lu %u", &RxnIsReversible, &StoichData[RxnNo].
                    numproducts );

               for ( j = 0; j < ( pStD + RxnNo ) ->numproducts; j++ )
               {
                    fscanf( InFile, "%u %u", &StoichData[RxnNo].
                         ProductsInStep[j], &StoichData[RxnNo].ProductStoich[j] );
               }                       /* end for j                           */
               fscanf( InFile, "%lu", &spkin );

               if ( spkin )
               {
                    fscanf( InFile, "%u", &KineticsData[RxnNo].numreactants );

                    for ( j = 0; j < ( pKD + RxnNo ) ->numreactants; j++ )
                    {
                         fscanf( InFile, " %u %lf", &KineticsData[RxnNo].
                              ReactantsInStep[j], &KineticsData[RxnNo].
                              ReactantOrder[j] );
                    }                  /* end for j                           */
               }                       /* end if spkin                        */
               else

                    /* if not special kinetics then copy stoich for kinetics*/

               {
                    for ( j = 0; j < ( pStD + RxnNo ) ->numreactants; j++ )
                    {
                         ( pKD + RxnNo ) ->numreactants = ( pStD + RxnNo ) ->
                         numreactants;
                         ( pKD + RxnNo ) ->ReactantsInStep[j] = ( pStD + RxnNo ) ->
                         ReactantsInStep[j];
                         ( pKD + RxnNo ) ->ReactantOrder[j] = ( pStD + RxnNo ) ->
                         ReactantStoich[j];
                    }                  /* end for jr                          */
               }                       /* end else not spkin                  */
               jr = RxnNo;             /* now set up structures for reverse
                                          process                             */
               RxnNo++;

               if ( RxnIsReversible )
               {
                    ( pStD + RxnNo ) ->numreactants = ( pKD + RxnNo ) ->numreactants =
                    ( pStD + jr ) ->numproducts;

                    ( pStD + RxnNo ) ->numproducts = ( pStD + jr ) ->numreactants;

                    for ( j = 0; j < ( ( pStD + jr ) ->numproducts ); j++ )
                    {
                         ( pStD + RxnNo ) ->ReactantsInStep[j] = ( pKD + RxnNo ) ->
                         ReactantsInStep[j] = ( pStD + jr ) ->ProductsInStep[j];

                         ( pStD + RxnNo ) ->ReactantStoich[j] = ( pStD + jr ) ->
                         ProductStoich[j];
                         ( pKD + RxnNo ) ->ReactantOrder[j] = ( pStD + RxnNo ) ->
                         ReactantStoich[j];
                    }                  /* endfor j                            */
                    for ( j = 0; j < ( ( pStD + jr ) ->numreactants ); j++ )
                    {
                         ( pStD + RxnNo ) ->ProductsInStep[j] = ( pStD + jr ) ->
                         ReactantsInStep[j];
                         ( pStD + RxnNo ) ->ProductStoich[j] = ( pStD + jr ) ->
                         ReactantStoich[j];
                    }                  /* endfor                              */
                    if ( spkin )
                    {
                         fscanf( InFile, "%u", &KineticsData[RxnNo].
                              numreactants );

                         for ( j = 0; j < ( ( pKD + RxnNo ) ->numreactants ); j++ )
                         {
                              fscanf( InFile, "%u %lf", &KineticsData[RxnNo].
                                   ReactantsInStep[j], &KineticsData[RxnNo].
                                   ReactantOrder[j] );
                         }             /* end for j                           */
                    }                  /* endif spkin                         */
               }                       /* end if RxnIsReversible              */

               /* now read in rate coefficients                             */

               fscanf( InFile, "%le %lf %lf", &KineticsData[jr].RateConstant[0]
                    , &KineticsData[jr].RateConstant[1], &KineticsData[jr].
                    RateConstant[2] );

               if ( ( ( ( pKD + jr ) ->RateConstant[1] ) != 0.0 ) || ( ( ( pKD + jr ) ->
                                   RateConstant[2] ) != 0.0 ) )
               {
                    TempDependentRateConst = TRUE;
               }                       /* end if                              */
               if ( RxnIsReversible )  /* read in data for reverse rxn        */
               {
                    fscanf( InFile, "%le %lf %lf", &KineticsData[RxnNo].
                         RateConstant[0], &KineticsData[RxnNo].RateConstant[1],
                         &KineticsData[RxnNo].RateConstant[2] );

                    if ( ( ( ( pKD + RxnNo ) ->RateConstant[1] ) != 0.0 ) || ( ( ( pKD +
                                             RxnNo ) ->RateConstant[2] ) != 0.0 ) )
                    {
                         TempDependentRateConst = TRUE;
                    }                  /* end if pKD                          */
               }                       /* end if RxnIsReversible              */
               RxnNo++;
          }                            /* end while RxnNo...                  */

          /* read thermochemical data                                       */

          fscanf( InFile, "%s", TempString );/* a check for presence of
                                                thermo data                   */
          if ( 0 == strcmp( TempString, msimTHERMOCHEM_FLAG ) )
          {
               for ( j = 1; j <= NumberOfSpecies; j++ )
               {
                    fscanf( InFile, "%lf %lf %lf %lf %lf", &ThermoData[j].
                         enthalpy, &ThermoData[j].coeffA, &ThermoData[j].coeffB,
                         &ThermoData[j].coeffC, &ThermoData[j].coeffD );
               }                       /* endfor                              */
               fscanf( InFile, "%s", TempString );/* and get the next string  */
          }                            /* end if 0                            */

          /* read execution data                                            */

          if ( strcmp( TempString, msimEXECUTION_FLAG ) == 0 )
          {
               fscanf( InFile, "%u", &NoOfSpecWithNonZeroInitConc );

               j = 0;
               while ( j < NoOfSpecWithNonZeroInitConc )
               {
                    fscanf( InFile, "%u", &tmp );
                    fscanf( InFile, "%lf", &SpeciesData[tmp].InitNoOfMoles );
                    j++;
               }                       /* end while j                         */
          }
          else
          {
               RecordFatalErrorLocation( msimINPUT_ERROR, InputFile, __FILE__, __TIMESTAMP__, __LINE__ );
               return msimINPUT_ERROR;
          }                            /* end if strcmp                       */

          /* Finally read values of various options : variable temperature flag,*/
          /* T program coefficients, if that option has been selected       */
          /* total # particles, total # of events, write intvl to file, temperature,*/
          /* temperature convergence, 3 random number seeds                 */

          fscanf( InFile, "%u", &TempAndPressOption );

          switch ( TempAndPressOption )
          {

          case msimPROG_T_CONST_P_v :
          case msimPROG_T_VAR_P_CONST_V :
          case msimPROG_T_CONST_P_VAR_V :

               fscanf( InFile, "%lf %ld", &Tfunction.MaxDeltaTemp, &ProgTFormat
               );

               if ( ProgTFormat == msimANALYTIC_TPROG )
               {
                    fscanf( InFile, "%lf %lf", &Tfunction.FinalTemp,
                         &Tfunction.TRamp );
               }
               else
               {                       /* == msimFILE_TPROG                   */
                    fscanf( InFile, "%u", &NoOfProgTFilePoints );

                    /* now allocate space and pointer for ProgTempFile      */

                    pPTF = new double[NoOfProgTFilePoints * 2];

                    if ( pPTF == NULL )
                    {
                         RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, msimNULL_STR, __FILE__,
                              __TIMESTAMP__, __LINE__ );
                         return msimMEM_ALLOC_ERROR;
                    }
                    else

                         memset( pPTF, '\0', NoOfProgTFilePoints * 2 * sizeof
                              ( double ) );

                    ProgTempFile = pPTF;

                    /* read in file, order time, temp, time, temp...        */

                    for ( j = 0; j < NoOfProgTFilePoints; j++ )
                    {
                         jp = 2 * j;
                         fscanf( InFile, "%lf %lf", &ProgTempFile[jp],
                              &ProgTempFile[jp + 1] );

                    }                  /* endfor j                            */
               }                       /* endif ProgTFormat                   */
               if ( TempAndPressOption == msimPROG_T_CONST_P_VAR_V )
               {
                    for ( j = 1; j <= NumberOfSpecies; j++ )/* read in data
                                                               to calc vols   */
                    {
                         fscanf( InFile, "%u %lf", &SpeciesData[j].Phase,
                              &SpeciesData[j].Density );
                    }                  /* endfor                              */
               }                       /* endif TPOpt                         */
               break;

          case msimCONST_TP_VAR_V :
          case msimVAR_T_CONST_P_VAR_V :

               for ( j = 1; j <= NumberOfSpecies; j++ )/* read in data to
                                                          calc vols           */
               {
                    fscanf( InFile, "%u %lf", &SpeciesData[j].Phase,
                         &SpeciesData[j].Density );
               }                       /* endfor                              */
               break;

          default :

               break;

          }                            /* endswitch                           */

          /* now get the rest of the data                                   */
          short int rn_seed;

          fscanf( InFile, " %ld %lu %lu %hd %lf %lf %u %hu %lf %lu %lf"
               , &TotalNoOfParticles, &TotalNumberOfEvents, &WriteInterval,
               &rn_seed, &T,
               &TConvergence, &EquilDetectOption, pTimeUnits, &MinEfficiency
               , &EquilTestCycle, &ElapsedTimeLimit );

           RandomNum.ResetSeed( rn_seed );

          /* correct MinEfficiency to be consistent with F's usage          */

          InitialT = T;
          MinEfficiency = 1.0 - ( MinEfficiency / 100.0 );

          if ( ferror( InFile ) )
          {
               fclose( InFile );
               RecordFatalErrorLocation( msimREAD_ERROR, InputFile, __FILE__, __TIMESTAMP__, __LINE__ );
               return msimREAD_ERROR;
          }

          fclose( InFile );

     }                                 /* end else                            */
     return msimNO_ERROR;

}                                      /* end function ReadInputData          */

/* +-----------------------------------------------------------------------+
   |Definition of Function Round:                                          |
   |Purpose : returns the properly rounded double precision value of       |
   |the argument - intended to handle arguments greater than 0.0           |
   |Parameters : a double precision real value to be rounded               |
   |Returns : the rounded value as a double precision real                 |
   +-----------------------------------------------------------------------+*/

unsigned long round( double x )

{
     return ( ( unsigned long ) ( floor( x + 0.5 ) ) );
}

/* +----------------------------------------------------------------------+
   | Definition of Function SetUpCalculation :                            |
   | Purpose: to initialize some variables used in the probability        |
   | calculation                                                          |
   | Parameters: none                                                     |
   | Calls: SetUpUpdateTables                                         |
   | Returns:  error code if SetUpUpdateTables is unsuccessful |
  +----------------------------------------------------------------------+  */

msimRC FAR SetUpCalculation( void )
{                                      /* start function SetUpCalculation     */
     unsigned int j, jr;
     double Factor;
     double TotalMoles = 0.0;
     double Tsquare, Tcube;
     double Temp, InitVol, Order;


     if ( ResumeSimulation )
     {
          RestoreSimulatorState( );
          return msimNO_ERROR;
     }

     ElapsedTime = 0.0;
     EventCount = WriteIntervalCount = EquilTestCycleCount = 1;

     /* total up moles and apportion particles                              */

     for ( j = 1; j <= NumberOfSpecies; j++ )
     {
          if ( ( pSpD + j ) ->InitNoOfMoles )
          {
               TotalMoles += ( pSpD + j ) ->InitNoOfMoles;
          }
     }                                 /* end for                             */
     ParticlesPerMol = TotalNoOfParticles / TotalMoles;
     for ( j = 1; j <= NumberOfSpecies; j++ )
     {
          if ( ( pSpD + j ) ->InitNoOfMoles )
          {
               * ( pNP + j ) = round((( pSpD + j ) ->InitNoOfMoles ) *ParticlesPerMol );
          }
     }

     /* calculate initial volume of system                                  */

     switch ( TempAndPressOption )
     {

     case msimCONST_TP_VAR_V :
     case msimVAR_T_CONST_P_VAR_V :
     case msimPROG_T_CONST_P_VAR_V :
          InitVol = 0.0;
          for ( j = 1; j <= NumberOfSpecies; j++ )
          {
               if ( ( pSpD + j ) ->InitNoOfMoles )
               {
                    InitVol += ( ( ( pSpD + j ) ->InitNoOfMoles ) / ( ( pSpD + j ) ->Density ) )
                    ;
                    InitialPhase = ( ( pSpD + j ) ->Phase );
               }
          }                            /* end for                             */
          break;

     default :
          InitVol = STANDARD_VOLUME;   /* 1 liter                             */
          break;
     }                                 /* end switch                          */
     V = InitVol;

     /* calculate initial pressure of system - not always used              */

     if ( InitialPhase == msimGAS )
     {
          P = ( TotalMoles * GAS_CONSTANT * T ) / InitVol;
     }

     /* calculate quantities specific to TPV options                        */

     switch ( TempAndPressOption )
     {

          /* if variable T  calculate initial thermal properties of system  */

     case msimVAR_T_CONST_P_v :
     case msimVAR_T_CONST_P_VAR_V :
     case msimVAR_T_VAR_P_CONST_V :
          TotalHeatCapacity = 0.0;
          TotalEnthalpy = 0.0;
          Tsquare = T * T;
          Tcube = T * T * T;
          j = 1;
          while ( j <= NumberOfSpecies )
          {
               if ( * ( pNP + j ) )
               {
                    TotalEnthalpy += ( ( * ( pNP + j ) ) * (( ( ( pThD + j ) ->enthalpy ) * 1000 ) +
                              ( ( pThD + j ) ->coeffA ) + ( ( pThD + j ) ->coeffB ) *T + ( ( pThD + j ) ->coeffC
                              ) *Tsquare + ( ( pThD + j ) ->coeffD ) *Tcube ) );
                    TotalHeatCapacity += ( ( * ( pNP + j ) ) * (( ( pThD + j ) ->coeffB ) +
                              ( ( pThD + j ) ->coeffC ) *T * 2 + ( ( pThD + j ) ->coeffD ) *Tsquare * 3 ) );
               }                       /* endif                               */
               j++;
          }                            /* endwhile                            */
          break;

     case msimCONST_TP_v :
     case msimCONST_TP_VAR_V :
     case msimCONST_T_VAR_P_CONST_V :

          /* if constT Arrhrate calculate rate constants and store in RateConstant[0]*/

          if ( TempDependentRateConst )
          {
               for ( jr = 0; jr < NumberOfRxns; jr++ )
               {
                    Temp = ( pKD + jr ) ->RateConstant[0];
                    if ( Temp )
                    {
                         if ( ( pKD + jr ) ->RateConstant[1] )
                         {
                              Temp *= pow( T, ( pKD + jr ) ->RateConstant[1] );
                         }
                         if ( ( pKD + jr ) ->RateConstant[2] )
                         {
                              Factor = ( ( pKD + jr ) ->RateConstant[2] ) / ( R * T );

                              if ( - Factor < MINIMUM_EXPONENT )
                                   Factor = - MINIMUM_EXPONENT;

                              Temp *= exp( - Factor );

                         }             /* endif                               */
                         ( pKD + jr ) ->RateConstant[0] = Temp;
                    }                  /* endif Temp                          */
               }                       /* endfor jr                           */
          }                            /* endif TempDepRateConstant           */
          if ( TempAndPressOption != msimCONST_TP_VAR_V )
          {

               /* set up update tables to minimize recalculation of probabilities each cycle*/

               if ( SetUpUpdateTables( ) != msimNO_ERROR )
               {
                    return msimMEM_ALLOC_ERROR;
               }                       /* endif                               */
          }                            /* endif TPOpt                         */
          break;

     case msimPROG_T_CONST_P_v :
     case msimPROG_T_CONST_P_VAR_V :
     case msimPROG_T_VAR_P_CONST_V :

          /* be sure that the max temp step is magnitude only               */

          Tfunction.MaxDeltaTemp = fabs( Tfunction.MaxDeltaTemp );

          if ( ProgTFormat == msimANALYTIC_TPROG )
          {

               /*Temperature-time function is a linear ramp starting from an initial temperature*/
               /* calculate coefficients for maximum time step when system is idling between*/
               /*direct simulations                                                   */

               if ( Tfunction.TRamp > 0 )
               {
                    Tfunction.Slope = POS;
               }
               else
               {
                    Tfunction.Slope = NEG;
               }                       /* endif                               */
               Tfunction.MaxDeltaTime = Tfunction.Slope
               *Tfunction.MaxDeltaTemp / Tfunction.TRamp;
               Tfunction. IdlingTimeStep = Tfunction.Slope
               *IDLING_T_STEP / Tfunction.TRamp;

          }
          else
          {                            /* msimFILE_TPROG                      */
               NoOfProgTFilePoints *= 2;/* set max temp for simulation        */
               Tfunction.FinalTemp = * ( pPTF + ( NoOfProgTFilePoints - 1 ) );
               T = * ( pPTF + 1 );     /* be sure that the first T in the
                                          file is the initial system T        */
               /* start indexing through profile at second set of points  - extrapolate back */
               ProgTFileIndex = 2;

               if ( TempAndPressOption != msimPROG_T_CONST_P_VAR_V )
               {

                    /* set up update tables to minimize recalculation of probabilities each cycle*/

                    if ( SetUpUpdateTables( ) != msimNO_ERROR )
                    {
                         return msimMEM_ALLOC_ERROR;
                    }                  /* endif                               */
               }                       /* endif TPOpt                         */
          }                            /* endif ProgTFormat                   */
          break;

     default :
          break;
     }                                 /* end switch                          */

     /* Convert units of rate constant by multiplying either k or A         */
     /* by  constant factor - applies to all TP options                     */
     /* calculate conversion factor                                         */

     ConversionFactor = ParticlesPerMol * InitVol;

     for ( jr = 0; jr < NumberOfRxns; jr++ )
     {
          if ( ( pKD + jr ) ->RateConstant[0] )
          {
               Order = 0.0;
               for ( j = 0; j < ( ( pKD + jr ) ->numreactants ); j++ )
               {
                    Order += ( pKD + jr ) ->ReactantOrder[j];
               }                       /* endfor jr                           */
               Order -= 1.0;
               ( pKD + jr ) ->UnitsConversionExponent = Order;
               if ( Order != 0.0 )
               {
                    Factor = pow( ConversionFactor, Order );
                    ( pKD + jr ) ->RateConstant[0] /= Factor;
               }
          }                            /* end if pKD                          */
     }                                 /* endfor jr                           */

     /* Set parameter for Equil detect                                      */

     if ( EquilDetectOption )
     {
          MinFrequency = 1 - MinEfficiency;
     }                                 /* endif                               */

     OutputCalcStatus( );
     CalcAllRxnProbabilities( );

     TotalProbability = TotalRxnProbability;

     // added 10.06.94 to test for initial TotalProbability == 0.0
     // exit with msg if true

     if ( TotalProbability == 0.0 )
     {
          InitializeScreenDisplay( ElapsedTime, EventCount, TotalNumberOfEvents );
          SetReasonForExit( msimZERO_PROBABILITY );
          exit( msimNO_ERROR );
     }

     // this prevents jumping past the first point in ext temp profile
     // in the event that the initial rates are too small

     if ( TempAndPressOption  & msimPROGR_TEMP )// is this a programmed temperature rxn ?
     {
          if ( ( TotalProbability < DBL_MIN ) ||
                    ( ( ProgTempFile != NULL ) && ( 1.0 / TotalProbability ) >= ProgTempFile[2] )
               )
               JumpStart( );
     }

     return msimNO_ERROR;

}                                      /* end function SetUpCalculation       */



/*--------------------------------------------------------------------------*/
/*Definition of function SetUpUpdateTables                                  */
/*Purpose: to create tables for expediting probability updates              */
/*Parameters: none                                                          */
/*Calls: nothing                                                            */
/*Returns: error code if array allocation is unsuccessful                   */
/*--------------------------------------------------------------------------*/

int SetUpUpdateTables( void )

{                                      /* begin function                      */
     unsigned int Offset, SearchOffset;
     msimBOOL FoundMatch;
     unsigned int j, jr, k1, k2, n1, n2;

     /* allocate space and pointer for UpdateTable[NumberOfRxns*NumberOfRxns+1]*/

     pUT = new unsigned int[ ( NumberOfRxns + 1 ) *NumberOfRxns];

     if ( pUT == NULL )
     {
          RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, NULL, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimMEM_ALLOC_ERROR;
     }
     else

          memset( pUT, '\0', ( ( NumberOfRxns + 1 ) *NumberOfRxns ) *sizeof ( unsigned
                    int ) );
     UpdateTable = pUT;

     /* initialize flag */
     FoundMatch = FALSE;

     /* Set up lookup table for updating probabilities after SelectEvent    */

     jr = j = 0;
     UpdateTableOffset = NumberOfRxns + 1;
     while ( jr < NumberOfRxns )
     {
          j = jr + 1;
          if ( ( pStD + jr ) ->numreactants == 0 )
          {
               jr++;
               j++;
          }
          if ( jr >= NumberOfRxns )
               break;
          Offset = jr * UpdateTableOffset;/* accesses array sections for
                                             each reaction                    */
          k1 = ( ++ ( * ( UpdateTable + Offset ) ) );/* calc and store next open spot
                                                        in table              */
          * ( UpdateTable + Offset + k1 ) = jr;/* saves rxns own index        */
          while ( j < NumberOfRxns )
          {
               if ( ( pStD + j ) ->numreactants == 0 )
                    j++;
               if ( j >= NumberOfRxns )
                    break;
               SearchOffset = j * UpdateTableOffset;/* searches following
                                                       sections for matches   */
               for ( n1 = 0; n1 < ( ( pStD + jr ) ->numreactants ); n1++ )/* search
                                                                 reactant list*/
               {
                    for ( n2 = 0; n2 < ( ( pStD + j ) ->numreactants ); n2++ )
                    {
                         FoundMatch = FALSE;
                         if ( ( ( pStD + jr ) ->ReactantsInStep[n1] ) == ( ( pStD + j ) ->
                                        ReactantsInStep[n2] ) )
                         {
                              FoundMatch = TRUE;
                              k1 = ( ++ ( * ( UpdateTable + Offset ) ) );
                              k2 = ( ++ ( * ( UpdateTable + SearchOffset ) ) );
                              * ( UpdateTable + Offset + k1 ) = j;
                              * ( UpdateTable + SearchOffset + k2 ) = jr;
                         }             /* endif                               */
                         if ( FoundMatch )
                              break;
                    }                  /* endfor n2                           */
                    if ( FoundMatch )
                         break;
               }                       /* endfor n1                           */
               if ( ! FoundMatch )
               {
                    for ( n1 = 0; n1 < ( ( pStD + jr ) ->numproducts ); n1++ )/*
                                                           search product list*/
                    {                  /* of jr rxn                           */
                         for ( n2 = 0; n2 < ( ( pStD + j ) ->numreactants ); n2++ )
                         {
                              FoundMatch = FALSE;
                              if ( ( ( pStD + jr ) ->ProductsInStep[n1] ) == ( ( pStD + j )
                                        ->ReactantsInStep[n2] ) )
                              {
                                   FoundMatch = TRUE;
                                   k1 = ( ++ ( * ( UpdateTable + Offset ) ) );
                                   * ( UpdateTable + Offset + k1 ) = j;
                              }        /* endif                               */
                              if ( FoundMatch )
                                   break;
                         }             /* endfor n2                           */
                         if ( FoundMatch )
                              break;
                    }                  /* endfor n1                           */
                    for ( n1 = 0; n1 < ( ( pStD + j ) ->numproducts ); n1++ )/*
                                                           search product list*/
                    {                  /* of j rxn                            */
                         for ( n2 = 0; n2 < ( ( pStD + jr ) ->numreactants ); n2++ )
                         {
                              FoundMatch = FALSE;
                              if ( ( ( pStD + j ) ->ProductsInStep[n1] ) == ( ( pStD + jr )
                                        ->ReactantsInStep[n2] ) )
                              {
                                   FoundMatch = TRUE;
                                   k2 = ( ++ ( * ( UpdateTable + SearchOffset ) ) );
                                   * ( UpdateTable + SearchOffset + k2 ) = jr;
                              }        /* endif                               */
                              if ( FoundMatch )
                                   break;
                         }             /* endfor n2                           */
                         if ( FoundMatch )
                              break;
                    }                  /* endfor n1                           */
               }                       /* endif not FoundMatch                */
               j++;
          }                            /* endwhile j                          */
          jr++;
     }                                 /* endwhile jr                         */
     return msimNO_ERROR;

}                                      /* end function SetUpUpdateTables      */





/*------------Definition of Function OpenOutputFile-------------------------*/
/*    Purpose: to open file for storage of simulation results               */
/*    Parameters: none                                                      */
/*    Returns: int error code if one occurs                                 */
/*--------------------------------------------------------------------------*/

USHORT FAR OpenOutputFile( PCHAR OutputFileName )
{                                      /* start function                      */

     /* open output file for write-only in binary mode                      */

     if ( NULL == ( OutFile = fopen( OutputFileName, OUTPUT_FORMAT ) ) )
     {
          RecordFatalErrorLocation( msimFILE_OPEN_ERROR, OutputFileName, __FILE__, __TIMESTAMP__, __LINE__ );
          return msimFILE_OPEN_ERROR;
     }                                 /* end if                              */

     /* position at end of file so we can append data                       */

     fseek( OutFile, 0L, SEEK_END );

     return msimNO_ERROR;
}                                      /* end function OpenOutputFile         */

/*+-----------------------------------------------------------------------+
  | Definition of FUNCTION OutputCalcStatus :
  | Purpose: to print to the FILE OutFile periodic updates
  |  of the status  of the calculation & tests for Interrupts.
  | Parameters : none
  | Returns : nothing
  | Note : form of output differs depending on definition of ASCII_OUTPUT
  +-----------------------------------------------------------------------+ */

void OutputCalcStatus( void )

{                                      /* start function OutPutCalcStatus     */

#ifdef   ASCII_OUTPUT
#define  TS_LENGTH                               100
     char               TempString1[TS_LENGTH],TempString2[TS_LENGTH];
     signed int         k,kk;
     short int          skipover;

#endif

#ifdef   ASCII_OUTPUT
     switch (TempAndPressOption)

     {
     case  msimCONST_TP_v :            /* write time only                     */
          fprintf(OutFile, "%e", ElapsedTime);
          skipover = 1;
          break;

          /* write time and temp                                            */

     case  msimVAR_T_CONST_P_v :
     case  msimPROG_T_CONST_P_v :
          fprintf(OutFile, "%e %e", ElapsedTime, T);
          skipover = 2;
          break;

          /* write time and pressure                                        */

     case  msimCONST_T_VAR_P_CONST_V :
          fprintf(OutFile, "%e %e", ElapsedTime, P);
          skipover = 2;
          break;

          /* write time and volume                                          */

     case  msimCONST_TP_VAR_V :
          fprintf(OutFile, "%e %e", ElapsedTime, V);
          skipover = 2;
          break;

          /* write time, temp, pressure                                     */

     case  msimVAR_T_VAR_P_CONST_V :
     case  msimPROG_T_VAR_P_CONST_V :
          fprintf(OutFile, "%e %e %e", ElapsedTime, T, P);
          skipover = 3;
          break;

          /* write time,temp,volume                                         */

     case  msimVAR_T_CONST_P_VAR_V :
     case  msimPROG_T_CONST_P_VAR_V :
          fprintf(OutFile, "%e %e %e", ElapsedTime, T, V);
          skipover = 3;
          break;
     }                                 /* end switch                          */
     TempString1[0] = 0x00;
     msimStringCat(TempString1, " ", sizeof TempString1);
     kk = 1;
     while (kk <= NumberOfSpecies)
     {
     for (k = (1+skipover); k <= NUMBER_PER_LINE; k++)
     {
          sprintf(TempString2, "%10lu", NoPrtcls[kk]);
          msimStringCat(TempString1, TempString2, sizeof TempString1);
          msimStringCat(TempString1, " ", sizeof TempString1);
          kk++;
          if (kk > NumberOfSpecies)
               break;
          skipover = 0;
     }                                 /* end for k                           */
     fputs(TempString1, OutFile);
     fputs("\n", OutFile);
     TempString1[0] = 0x00;
     }                                 /* end while                           */
     return ;

#else

     switch ( TempAndPressOption )
     {
     case msimCONST_TP_v :             /* write time only                     */
          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );

          if ( OutputTempAnyway )
               fwrite( &T, sizeof ( T ), 1, OutFile );

          break;

          /* write time and temp                                            */

     case msimVAR_T_CONST_P_v :
     case msimPROG_T_CONST_P_v :
          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );
          fwrite( &T, sizeof ( T ), 1, OutFile );
          break;

          /* write time and pressure                                        */

     case msimCONST_T_VAR_P_CONST_V :
          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );

          if ( OutputTempAnyway )
               fwrite( &T, sizeof ( T ), 1, OutFile );

          fwrite( &P, sizeof ( P ), 1, OutFile );
          break;

          /* write time and volume                                          */

     case msimCONST_TP_VAR_V :

          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );

          if ( OutputTempAnyway )
               fwrite( &T, sizeof ( T ), 1, OutFile );

          fwrite( &V, sizeof ( V ), 1, OutFile );
          break;

          /* write time, temp, pressure                                     */

     case msimVAR_T_VAR_P_CONST_V :
     case msimPROG_T_VAR_P_CONST_V :
          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );
          fwrite( &T, sizeof ( T ), 1, OutFile );
          fwrite( &P, sizeof ( P ), 1, OutFile );
          break;

          /* write time,temp,volume                                         */

     case msimVAR_T_CONST_P_VAR_V :
     case msimPROG_T_CONST_P_VAR_V :
          fwrite( &ElapsedTime, sizeof ( ElapsedTime ), 1, OutFile );
          fwrite( &T, sizeof ( T ), 1, OutFile );
          fwrite( &V, sizeof ( V ), 1, OutFile );
          break;

     default :
          break;

     }                                 /* end switch                          */

     /* now write the whole array out at once                               */

     fwrite( &NoPrtcls[1], sizeof ( NoPrtcls[1] ), NumberOfSpecies, OutFile );

     /* check for errors and abort w/o updating NumberOfRecordsWritten      */
     /* if an IO error has occurred                                         */

     if ( ferror( OutFile ) )
     {
          RecordFatalErrorLocation( msimWRITE_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     NumberOfRecordsWritten++;
     return;

#endif
}                                      /* end function OutPutCalcStatus       */

/*+---------------------------------------------------------------------------+
  |Definition of Function ZeroProbability :
  |Purpose : to clean things up and to notify user when total probaibility = 0
  |Calls: Nothing
  |Parameters: none ; Returns nothing
 +---------------------------------------------------------------------------+*/

void ZeroProbability( void )
{
     SetReasonForExit( msimZERO_PROBABILITY );
     return;
}

/*+------------------------------------------------------------------------ */
/*   Definition of Function CalcAllRxnProbabilities                         */
/*   Purpose: to establish  probabilities of reactions in mechanism         */
/*   and  TotalRxnProbability. This is the only time all reaction probs are */
/*   calculated for constant T simulations.                                 */
/*    Calls: RxnProbCalc, EnoughReactantsTest                               */
/*    Parameters: none                                                      */
/*     Returns: nothing                                                     */
/*--------------------------------------------------------------------------*/

void CalcAllRxnProbabilities( void )

{                                      /* start of function                   */
     unsigned int jr;
     int OKToContinue;

     for ( jr = 0; jr < NumberOfRxns; jr++ )
     {
          if ( ( pStD + jr ) ->numreactants != 0 )
          {
               OKToContinue = EnoughReactantsTest( jr );
               if ( ! OKToContinue )
               {
                    * ( pRP + jr ) = 0.0;
               }
               else
               {
                    * ( pRP + jr ) = RxnProbCalc( jr );
               }                       /* endif                               */
          }                            /* end if pStD+jr                      */
     }                                 /* end for jr                          */

     /* Calculate total probability                                         */

     for ( jr = 0, TotalRxnProbability = 0.0; jr < NumberOfRxns; jr++ )
     {
          TotalRxnProbability += * ( pRP + jr );
     }                                 /* end for                             */
     return;
}                                      /* end function
                                          CalcAllRxnProbabilities             */

/*---------------------------------------------------------------------------
    Definition of function RxnProbCalc
     Purpose: this is a general function for calculating a rate from
     rate law parameters.
    Parameters: the index of the reaction whose probability is calculated, int
     Returns: the value of the probability, double
------------------------------------------------------------------------------*/

double RxnProbCalc( unsigned int n )

{                                      /* begin function                      */
     double q, zexp;
     unsigned int ReactantCounter;
     unsigned int SpeciesNumber;

     q = ( pKD + n ) ->RateConstant[0];
     switch ( TempAndPressOption )
     {

     case msimCONST_TP_v :
     case msimCONST_TP_VAR_V :
     case msimCONST_T_VAR_P_CONST_V :
          break;

     default :
          if ( ( pKD + n ) ->RateConstant[2] )
          {
               zexp = ( ( pKD + n ) ->RateConstant[2] ) / ( R * T );

               if ( - zexp < MINIMUM_EXPONENT )/* check for underflow         */
                    zexp = - MINIMUM_EXPONENT;

               q *= ( exp( - zexp ) );

          }                            /* endif                               */
          if ( ( pKD + n ) ->RateConstant[1] )
          {
               q *= ( pow( T, ( ( pKD + n ) ->RateConstant[1] ) ) );
          }                            /* endif                               */
          break;
     }                                 /* endswitch                           */
     ReactantCounter = 0;
     while ( ReactantCounter < ( pKD + n ) ->numreactants )
     {
          SpeciesNumber = ( pKD + n ) ->ReactantsInStep[ReactantCounter];
          q *= ( pow(( double ) NoPrtcls[SpeciesNumber], ( pKD + n ) ->ReactantOrder
                    [ReactantCounter] ) );
          ReactantCounter++;
     }                                 /* endwhile ReactantCounter            */
     return q;

}                                      /* end function                        */



/*-----------------------------------------------------------------------------
  Definition of function EnoughReactantsTest
  Purpose: to determine if enough particles of each reactant are available
     to satisfy stoichiometry requirements of a particular reaction
  Parameters: n (= reaction number)
  Returns : integer code 0 or 1
------------------------------------------------------------------------------*/

int EnoughReactantsTest( unsigned int n )
{                                      /* begin function                      */
     unsigned int ReactantCounter, SpeciesNumber;

     ReactantCounter = 0;
     while ( ReactantCounter < ( pStD + n ) ->numreactants )
     {
          SpeciesNumber = ( pStD + n ) ->ReactantsInStep[ReactantCounter];
          if ( NoPrtcls[SpeciesNumber] < ( ( pStD + n ) ->ReactantStoich
                         [ReactantCounter] ) )
          {
               return FALSE;
          }                            /* end if                              */
          ReactantCounter++;
     }                                 /* endwhile ReactantCounter            */
     return TRUE;
}                                      /* end of function                     */

/*-----------------------------------------------------------------------------
  Definition of function UpdateRxnProbabilities
  Purpose: to update the subset of reaction probabilities affected by selection
   of a particular event, and the TotalRxnProbability
   Calls: RxnProbCalc, EnoughReactantsTest
   Parameters: none
   Returns:  nothing
------------------------------------------------------------------------------*/

void UpdateRxnProbabilities( void )

{                                      /* begin function                      */
     unsigned int j, jr;
     int OKToContinue;

     /* save old total probability for programmed T option, system idler    */

     PreviousTotalRxnProbability = TotalRxnProbability;

     switch ( TempAndPressOption )
     {
     case msimCONST_TP_v :
     case msimCONST_T_VAR_P_CONST_V :

          for ( j = 1; j <= ( * ( UpdateTable + SelectedRxnNo * UpdateTableOffset ) );
                    j++ )
          {
               jr = ( * ( UpdateTable + SelectedRxnNo * UpdateTableOffset + j ) );
               OKToContinue = EnoughReactantsTest( jr );
               if ( ! OKToContinue )
               {
                    * ( pRP + jr ) = 0.0;
               }
               else
               {
                    * ( pRP + jr ) = RxnProbCalc( jr );
               }                       /* endif                               */
          }                            /* endfor                              */
          for ( jr = 0, TotalRxnProbability = 0.0; jr < NumberOfRxns; jr++ )
               TotalRxnProbability += * ( pRP + jr );
          break;
     default :
          CalcAllRxnProbabilities( );
          break;
     }                                 /* endswitch                           */
     return;
}                                      /* end function
                                          UpdateRxnProbabilities              */



/*------------------------------------------------------------------------------
Definition of function JumpStart
Purpose: to enter idling routine before starting simulation if a temperature
program is started at a temperature where the total reaction probability is less
than the minimum floating point number.
Parameters: none
Calls: ProgTUpdate
Returns: nothing
--------------------------------------------------------------------------- */

void JumpStart( void )

{
     switch ( TempAndPressOption )
     {
     case msimPROG_T_CONST_P_VAR_V :
     case msimPROG_T_VAR_P_CONST_V :
     case msimPROG_T_CONST_P_v :

          DeltaTime = ElapsedTime = DBL_MAX;
          EventCount = 0;

          /*flag for system reset                                            */

          PreviousElapsedTime = 0.0;

          /*enter idling cycle until temperature is high enough for simulator to
             take over                                                           */

          ProgTUpdate( );
          EventCount = 1;
          break;
     default :
          break;
     }                                 /* endswitch                           */
     return;
}

/*--------------------------------------------------------------------------*/
/*Definition of function SelectEvent                                        */
/* Purpose: to test for equil detect option or existence of equilibrium     */
/*          and use appropriate procedure for selecting reaction            */
/* Calls: SelectNonEquilRxn, SelectAnyRxn                                   */
/* Parameters: none                                                         */
/*  Returns: nothing                                                        */
/*--------------------------------------------------------------------------*/

void SelectEvent( void )

{                                      /* begin function                      */
     if ( ElapsedTimeLimit > 0.0 && ElapsedTime > ElapsedTimeLimit )
     {
          ReactionOver = TRUE;
          SetReasonForExit( msimELAPSED_TIME_LIMIT );
          return;
     }

     if ( FinalTempReached )
     {
          ReactionOver = TRUE;
          SetReasonForExit( msimFINAL_TEMP_REACHED );
          return;
     }                                 /* endif                               */
     if ( EquilDetectOption )
     {
          if ( EquilTestCycleCount > EquilTestCycle )
               CheckForEquilibration( );
     }                                 /* endif                               */
     if ( DominantRxn )
     {
          SelectNonEquilRxn( );

          // if no nonequilibrium rxn to select, do std SelectEvent
          // Flag is DominantRxn = FALSE

          if ( DominantRxn == FALSE )
               SelectAnyRxn( );
     }
     else
     {
          SelectAnyRxn( );
     }                                 /* endif                               */
     return;

}                                      /* end function                        */



/*------------------------------------------------------------------------------
    Definition of function SelectNonEquilRxn
    Purpose:  to freeze equilibrated steps, select improbable event, update stoichiometry
          of selected event, update time. Sets ReactionOver true if the probability
          of all non-equil rxns is zero
    Calls: ZeroProbability, RandomNoGen, UpdateRxnStoichiometry
    Parameters: none
    Returns: status parameter
------------------------------------------------------------------------------*/

void SelectNonEquilRxn( void )
{                                      /* begin function                      */
     double Lambda;
     unsigned int jr, j, i;
     double DominantProbability, ProbSubTotal;

     DominantProbability = 0;

     for ( i = 0; i < NumberOfEquilRxns; i++ )
     {
          j = * ( pEqR + i );
          DominantProbability += ( * ( pRP + j ) + * ( pRP + j + 1 ) );
     }                                 /* endfor                              */

     Lambda = TotalRxnProbability - DominantProbability;

     if ( Lambda <= 0.0 )
     {
          // no events to select - set flag and retunr for std SelectEvent

          DominantRxn = FALSE;
          return;
     }

     /* Lambda is the frequency of occurrence of an event that is NOT the dominant rxn, ie the rate at which an improbable event
occurs. Thus it is just the total probability minus the probabilities of the most likely forward/backward rxn process*/

     for ( jr = 0; jr < NumberOfRxns; jr++ )

          /* temporarily normalize all probabilities of unlikely events to 1.0*/

     {
          * ( pTP + jr ) = ( * ( pRP + jr ) ) / Lambda;
     }

     /* set equil rxns to zero prob so they cannot be selected              */

     for ( i = 0; i < NumberOfEquilRxns; i++ )
     {
          j = ( * ( pEqR + i ) );
          * ( pTP + j ) = 0.0;
          * ( pTP + j + 1 ) = 0.0;
     }                                 /* endfor                              */

     /* here the probability is represented by a random number between 0 and 1*/

     WaitingTime = - log( RandomNum.NextValue( ) ) / Lambda;

     /* time in seconds that has passed before an improbable event occurs   */
     /* now select unlikely event                                           */

     RandomNo = RandomNum.NextValue( );

     for ( ProbSubTotal = 0, jr = 0;; )
     {
          ProbSubTotal += * ( pTP + jr );
          if ( ProbSubTotal >= RandomNo )
          {
               SelectedRxnNo = jr;
               break;
          }
          jr++;
          if ( jr > NumberOfRxns )
          {
               // no events to select - set flag and retunr for std SelectEvent

               DominantRxn = FALSE;
               return;
          }
     }                                 /* end for                             */

#if ( PROFILE & RECORD_EQUIL_TABLE )
     PrfRecordNonEquilRxnSelection( Lambda, DominantProbability );
#endif

     return;

}                                      /* end function SelectNonEquilRxn      */

/*--------------------------------------------------------------------------*/
/*  Definition of function SelectAnyRxn                                     */
/*  Purpose:  to use RandomNumber to Select Event                           */
/*   Calls: ZeroProbability, RandomNoGen,                                   */
/*   Parameters: none                                                       */
/*   Returns: return code                                                   */
/*--------------------------------------------------------------------------*/

void SelectAnyRxn( void )

{                                      /* begin function SelectAnyRxn         */
     double ProbSubtotal;
     unsigned int jr;

     if ( TotalRxnProbability == 0.0 ) /* Return if no reactions can occur    */
     {
          ZeroProbability( );
          ReactionOver = TRUE;
          SetReasonForExit( msimZERO_PROBABILITY );
          return;
     }                                 /* end if                              */

     /* get and normalize random number                                     */

     RandomNo = RandomNum.NextValue( );

     RandomNo *= TotalProbability;

     /* select event                                                        */

     ProbSubtotal = 0.0;
     jr = 0;
     while ( jr < NumberOfRxns )
     {
          ProbSubtotal += * ( pRP + jr );
          if ( ProbSubtotal > RandomNo )
          {
               break;
          }                            /* endif                               */
          jr++;
     }                                 /* endwhile                            */
     if ( jr >= NumberOfRxns )
     {
          ZeroProbability( );
          ReactionOver = TRUE;
          SetReasonForExit( msimZERO_PROBABILITY );
          return;
     }                                 /* end if                              */
     SelectedRxnNo = jr;

     if ( EquilDetectOption == EQUIL_DETECT )
     {
          ( * ( pRE + jr ) ) ++;
     }                                 /* endif                               */
}                                      /* end of function SelectAnyRxn        */

/*--------------------------------------------------------------------------*/
/*  Definition of function UpdateTime                                       */
/*   Purpose : to calculate time step associated with each event selection  */
/*   Parameters : none                                                      */
/*   Returns: nothing                                                       */
/*--------------------------------------------------------------------------*/

void UpdateTime( void )

{                                      /* begin UpdateTime                    */
     if ( DominantRxn )
     {
          DeltaTime = WaitingTime;
          DominantRxn = FALSE;
     }
     else
     {                                 /* using Gillespie's expression        */
          DeltaTime = - log( RandomNum.NextValue( ) ) / TotalProbability;
     }                                 /* endif                               */
     PreviousElapsedTime = ElapsedTime;/* save for ProgT idling routine if
                                          accessed                            */
     ElapsedTime += DeltaTime;

}                                      /* end UpdateTime                      */

/* +-----------------------------------------------------------------------+
   |Definition of function UpdateRxnStoichiometry                          |
   |Purpose : to update the concentrations of all species involved in      |
   | the selected reaction.                                                |
   |Parameters : none
   |Returns: nothing                                                       |
   +-----------------------------------------------------------------------+*/

void UpdateRxnStoichiometry( void )

/* update particle concentrations using reaction stoichiometry              */

{
     unsigned int j, SpeciesNumber;

     for ( j = 0; j < ( pStD + SelectedRxnNo ) ->numreactants; j++ )
     {
          SpeciesNumber = ( pStD + SelectedRxnNo ) ->ReactantsInStep[j];
          * ( pNP + SpeciesNumber ) -= ( pStD + SelectedRxnNo ) ->ReactantStoich[j];
     }
     for ( j = 0; j < ( pStD + SelectedRxnNo ) ->numproducts; j++ )
     {
          SpeciesNumber = ( pStD + SelectedRxnNo ) ->ProductsInStep[j];
          * ( pNP + SpeciesNumber ) += ( pStD + SelectedRxnNo ) ->ProductStoich[j];
     }
     return;

}                                      /* end function
                                          UpdateRxnStoichiometry              */


/*---------------------------------------------------------------------------------
       Definition : function TempPressUpdate
       Purpose: to update T and P for the various options
       Calls: UpdateThermo, AdiabaticTempPressUpdate, UpdateProgT,
               IsothermalPressUpdate, CondensedVolUpdate, GasVolUpdate
                ConcentrationUpdate
       Parameters: none
       Returns: nothing
----------------------------------------------------------------------------------*/

void TempPressUpdate( void )

{                                      /* begin function                      */
     double OldVolume;

     switch ( TempAndPressOption )
     {
     case msimCONST_TP_v :
          break;
     case msimCONST_TP_VAR_V :
          OldVolume = V;
          if ( InitialPhase == msimGAS )
          {
               GasVolUpdate( );
          }
          else
          {
               CondensedVolUpdate( );
          }
          ConcentrationUpdate( OldVolume );
          break;
     case msimCONST_T_VAR_P_CONST_V :
          IsothermalPressUpdate( );
          break;
     case msimVAR_T_CONST_P_v :
          UpdateThermo( );
          break;
     case msimVAR_T_VAR_P_CONST_V :
          UpdateThermo( );
          AdiabaticPressUpdate( );
          break;
     case msimVAR_T_CONST_P_VAR_V :
          OldVolume = V;
          UpdateThermo( );
          if ( InitialPhase == msimGAS )
          {
               GasVolUpdate( );
          }
          else
          {
               CondensedVolUpdate( );
          }
          ConcentrationUpdate( OldVolume );
          break;
     case msimPROG_T_CONST_P_v :
          ProgTUpdate( );
          break;
     case msimPROG_T_VAR_P_CONST_V :
          ProgTUpdate( );
          if ( ReactionOver )
          {
               return;
          }                            /* endif                               */
          IsothermalPressUpdate( );
          break;
     case msimPROG_T_CONST_P_VAR_V :
          OldVolume = V;
          ProgTUpdate( );
          if ( ReactionOver )
          {
               return;
          }                            /* endif                               */
          if ( InitialPhase == msimGAS )
          {
               GasVolUpdate( );
          }
          else
          {
               CondensedVolUpdate( );
          }
          ConcentrationUpdate( OldVolume );
          break;

     default :
          break;

     }                                 /* end switch                          */
     return;
}                                      /* end function                        */

/* +----------------------------------------------------------------------+
   |    DEFINITION : Function UpdateThermo
   |    Purpose: updates temp.  via an iterative procedure
   |    Uses the following global variables : t, TotalHeatCapacity, TotalEnthalpy
   |    MODIFIES globals : t, TotalHeatCapacity, TotalEnthalpy
   |    Parameters : none;  Returns : nothing
   +----------------------------------------------------------------------+ */

void UpdateThermo( void )

{
     double tt, tx, cpy, hy;
     double Tsquare, Tcube;
     unsigned int j;

     tx = T;

     do
     {
#if  defined(__OS2__)
          if ( RescheduleFlag )
#endif
               Reschedule( );

          hy = cpy = 0.0;
          Tsquare = T * T;
          Tcube = T * T * T;
          j = 1;
          while ( j <= NumberOfSpecies )
          {
               if ( * ( pNP + j ) )
               {
                    hy += ( ( * ( pNP + j ) ) * (( ( ( pThD + j ) ->enthalpy ) * 1000 ) + ( ( pThD + j ) ->
                                   coeffA ) + ( ( pThD + j ) ->coeffB ) *T + ( ( pThD + j ) ->coeffC ) *Tsquare +
                              ( ( pThD + j ) ->coeffD ) *Tcube ) );
                    cpy += ( ( * ( pNP + j ) ) * (( ( pThD + j ) ->coeffB ) + ( ( pThD + j ) ->coeffC )
                              *T * 2 + ( ( pThD + j ) ->coeffD ) *Tsquare * 3 ) );
               }                       /* endif                               */
               j++;
          }                            /* endwhile                            */
          tt = tx + ( 2 * (TotalEnthalpy - hy ) ) / ( TotalHeatCapacity + cpy );
          if ( fabs( tt - T ) <= TConvergence )
          {
               T = tt;
               TotalHeatCapacity = cpy;
               TotalEnthalpy = hy;
               break;
          }
          else
          {
               T = ( T + tt ) / 2;
          }
     }
     while ( TRUE );                   /* do until convergence                */
}                                      /* end function UpdateThermo           */

/*---------------------------------------------------------------------------
Description of function ProgTUpdate
Purpose: to calculate the temperature change in the system  after a
time increment, and terminate T program if necessary
Calls: ProgTSystemIdler, TerminateProgT;
Parameters: none
Returns: nothing
------------------------------------------------------------------------------*/

void ProgTUpdate( void )

{                                      /* begin function                      */
     int GoIntoIdler;
     double Slope, InterpVal;

     /*calculate new temperature step from the new time calculated
        in the UpdateTime function                                           */

     OutOfRange = FALSE;
     GoIntoIdler = FALSE;

     switch ( ( USHORT ) ProgTFormat )
     {

     case msimANALYTIC_TPROG :

          DeltaTemp = Tfunction.TRamp * DeltaTime;

          /* now see if new temperature jump is within range                */
          /* test is whether temperature jump is of the right magnitude (independent of*/
          /* sign ) and whether the final temperature of the system has been reached*/

          if ( ( fabs( DeltaTemp ) ) > Tfunction.MaxDeltaTemp )

               /* T step too large                                          */

          {
               ProgTSystemIdler( );
               if ( ReactionOver )
               {
                    return;
               }                       /* endif                               */
          }
          else
          {
               T += DeltaTemp;
               if ( ( Tfunction.Slope == POS ) && ( T >= Tfunction.FinalTemp ) )

                    /* T beyond endpoint  & T monotonically increasing      */

               {
                    OutOfRange = TRUE;
               }
               if ( ( Tfunction.Slope == NEG ) && ( T <= Tfunction.FinalTemp ) )

                    /* T beyond endpoint  & T monotonically decreasing      */

               {
                    OutOfRange = TRUE;
               }
          }                            /* end if fabs Delta Temp              */
          if ( OutOfRange )
          {
               TerminateProgT( );
          }                            /* endif                               */
          break;

     case msimFILE_TPROG :
          NewIndex = ProgTFileIndex;
          if ( ElapsedTime > * (pPTF + NoOfProgTFilePoints - 2 ) )
          {
               OutOfRange = TRUE;
          }                            /* end if                              */
          if ( ! OutOfRange )
          {                            /* update location in ProgT file       */
               if ( ElapsedTime > * (pPTF + NewIndex ) )
                    /* if ElapsedTime is out of current segment in profile */
               {
                    NewIndex += 2;     /* even entries are always time        */
                    if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
                    {
                         OutOfRange = TRUE;
                         break;
                    }                  /* endif                               */
                    /* if ElapsedTime is out of range of next segment too, send */
                    /* simulation into idle mode to follow profile closely */
                    if ( ElapsedTime > * (pPTF + NewIndex ) )
                    {
                         GoIntoIdler = TRUE;
                    }
               }                       /* end if                              */
          }                            /* end if                              */
          if ( ! GoIntoIdler )
          {
               if ( ! OutOfRange )     /* determine temp jump in profile      */
               {
                    if ( ElapsedTime == * ( pPTF + NewIndex ) )
                    {                  /* Temp is one of the file points      */
                         DeltaTemp = * ( pPTF + NewIndex + 1 ) - T;
                    }
                    else
                    {                  /* interpolate temp                    */
                         Slope = ( * ( pPTF + NewIndex + 1 ) - * ( pPTF + NewIndex - 1 ) ) / ( * ( pPTF +
                                   NewIndex ) - * ( pPTF + NewIndex - 2 ) );
                         InterpVal = * ( pPTF + NewIndex + 1 ) - Slope * (* ( pPTF + NewIndex ) -
                              ElapsedTime );
                         DeltaTemp = InterpVal - T;
                    }                  /* endif ElapsedTime                   */
               }
               else
               {                       /* out of range                        */
                    DeltaTemp = Tfunction.FinalTemp - T;
               }                       /* endif not OutOfRange                */
          }
          else
          {     /* set DeltaTemp for this segment, value not important because*/
               /* the GoIntoIdler flag is set */

               DeltaTemp = * ( pPTF + NewIndex + 1 );
          }                            /*  end if not GoIntoIdler             */

          /* now see if new temperature jump is of the right size to continue, and check */
          /* whether flag or JumpStart is trying to send simulation into idler           */

          // ElapsedTime == DBL_MAX if this fcn is called by JumpStart()
          if ( ( fabs( DeltaTemp ) > Tfunction.MaxDeltaTemp ) ||
                    ( ( fabs( DeltaTemp ) <= DBL_MIN ) && ( ElapsedTime == DBL_MAX ) ) ||
                    ( GoIntoIdler )
               )
          {
               NewIndex = ProgTFileIndex;/* reset file index                  */
               ProgTSystemIdler( );
               /* save new place in program file */
               ProgTFileIndex = NewIndex;
               if ( ReactionOver )
               {
                    return;
               }                       /* endif                               */
          }
          else
          {
               ProgTFileIndex = NewIndex;/* save place in program file        */
               T += DeltaTemp;
          }                            /* end if fabs DeltaTemp               */

          /* if T is still out of range, convert to constant T simulation   */

          if ( OutOfRange )
          {
               TerminateProgT( );
          }                            /* endif                               */
          break;

     default :
          break;
     }                                 /* end switch                          */
     return;

}                                      /* end function                        */

/*----------------------------------------------------------------------------
        Function GetSlopeAndMaxDeltaTime
        Purpose: gets maximum time step in current segment of the temperature profile
        Calls: nothing
        Returns: nothing

------------------------------------------------------------------------------*/

void GetSlopeAndMaxDeltaTime( void )
{                                      /* begin function                      */

     double InterpVal, TrialMaxTemp, Slope;

     /* find current slope in profile segment, put in Tfunction.TRamp */

     Slope = ( * ( pPTF + NewIndex + 1 ) - * ( pPTF + NewIndex - 1 ) ) /
     ( * ( pPTF + NewIndex ) - * ( pPTF + NewIndex - 2 ) );
     if ( Slope > 0.0 )
     {
          TrialMaxTemp = T + Tfunction.MaxDeltaTemp;
     }
     else
     {                                 /* Slope < = 0.0                       */
          TrialMaxTemp = T - Tfunction.MaxDeltaTemp;
     }

     /* find Max Delta Time for current segment */
     if ( TrialMaxTemp < * ( pPTF + NewIndex + 1 ) )
     {                              /* max temp jump is within current segment*/
          if ( Slope )
          {
               InterpVal = * ( pPTF + NewIndex - 2 ) + ( TrialMaxTemp - * ( pPTF + NewIndex - 1 ) )
               / Slope;
               Tfunction.MaxDeltaTime = InterpVal - ElapsedTime;
          }
          else
          {/* Slope is 0.0, T will not increase so set time step to max within segment*/
               Tfunction.MaxDeltaTime = * ( pPTF + NewIndex ) - ElapsedTime;
          }                            /* end if Slope                        */
     }
     else
     {      /* out of range of current segment, set to max step within segment*/
          Tfunction.MaxDeltaTime = * ( pPTF + NewIndex ) - ElapsedTime;
     }                                 /* endif TrialMaxTemp                  */

     Tfunction.TRamp = Slope;
}                                      /* end function                        */



/*---------------------------------------------------------------------------
        Function ProgTSystemIdler
        Purpose: steps temperature until reaction probabilities reach a threshold
          value. If the temperature exceeds the final temperature of the T program
          the flag OutOfRange is set.
        Calls: IdlingTimeStep, GetMaxDeltaTime
        Returns: nothing
------------------------------------------------------------------------------*/

void ProgTSystemIdler( void )
{                                      /* begin function                      */
     int IntermediatePrintFlag;
     int CuspFlag;
     short int j;
     unsigned int SpeciesNumber;
     double InterpVal;

     /* reset range flag                                                    */

     OutOfRange = FALSE;
     MeanDeltaTime = DeltaTime;

     /* reset system                                                        */

     ElapsedTime = PreviousElapsedTime;/* roll back most recent time step     */
     if ( EventCount > 0 )
     {

          /* reset stoichiometry if event has been selected                  */
          /* roll back stoich change from most recent selected event             */

          for ( j = 0; j < ( pStD + SelectedRxnNo ) ->numreactants; j++ )
          {
               SpeciesNumber = ( pStD + SelectedRxnNo ) ->ReactantsInStep[j];
               * ( pNP + SpeciesNumber ) += ( pStD + SelectedRxnNo ) ->ReactantStoich[j];
          }                            /* end for                             */
          for ( j = 0; j < ( pStD + SelectedRxnNo ) ->numproducts; j++ )
          {
               SpeciesNumber = ( pStD + SelectedRxnNo ) ->ProductsInStep[j];
               * ( pNP + SpeciesNumber ) -= ( pStD + SelectedRxnNo ) ->ProductStoich[j];
          }                            /* end for                             */
     }                                 /* endif                               */

     /* enter idling cycle                                                  */

     switch ( ( USHORT ) ProgTFormat )
     {
     case msimANALYTIC_TPROG :

          while ( Tfunction.MaxDeltaTime < MeanDeltaTime )/* tests 1/prob     */
          {

#if  defined(__OS2__)
               if ( RescheduleFlag )
#endif
                    Reschedule( );

               /* get new temperature                                       */

               T += IDLING_T_STEP * Tfunction.Slope;
               if ( ( Tfunction.Slope == POS ) && ( T > Tfunction.FinalTemp ) )
               {
                    OutOfRange = TRUE;
                    return;
               }                       /* endif                               */
               if ( ( Tfunction.Slope == NEG ) && ( T < Tfunction.FinalTemp ) )
               {
                    OutOfRange = TRUE;
                    return;
               }                       /* endif                               */
               if ( ! OutOfRange )
               {

                    /*get new probabilities and mean time step              */

                    IdlingTimeStep( );
                    if ( ReactionOver )
                    {
                         return;
                    }                  /* endif                               */

                    /*increment system time to keep simulation on track     */

                    ElapsedTime += Tfunction.IdlingTimeStep;
               }
               else
               {                       /* if OutOfRange                       */
                    return;
               }                       /* endif not OutOfRange                */
          }                            /* end while Tfunction                 */

          /* return to normal simulator functions                           */

          break;

     case msimFILE_TPROG :

          /* find current slope in profile segment */

          GetSlopeAndMaxDeltaTime( );

          /* check that the current time-T point is not close to a profile point - can */
          /* lead to simulator spinning its wheels */

          if ( ( Tfunction.TRamp > 0.0 ) && ( ( * ( pPTF + NewIndex + 1 ) - T ) <= IDLING_T_STEP ) )
          {
               T = * ( pPTF + NewIndex + 1 );
               ElapsedTime = * ( pPTF + NewIndex );
               /* advance to next segment */
               NewIndex += 2;
               if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
               {
                    OutOfRange = TRUE;
                    return;
               }
               GetSlopeAndMaxDeltaTime( );
          }                            /* end if Ramp                         */

          if ( ( Tfunction.TRamp < 0.0 ) && ( ( T - * ( pPTF + NewIndex + 1 ) ) <= IDLING_T_STEP ) )
          {
               T = * ( pPTF + NewIndex + 1 );
               ElapsedTime = * ( pPTF + NewIndex );
               /* advance to next segment */
               NewIndex += 2;
               if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
               {
                    OutOfRange = TRUE;
                    return;
               }
               GetSlopeAndMaxDeltaTime( );
          }                            /* end if Ramp                         */



          /* ok to continue - now enter idling cycle                        */
          /*set flag to print system state each time NewIndex is increased  */
          /*Print state of system before starting                           */

          OutputCalcStatus( );
          IntermediatePrintFlag = FALSE;

          /*initialize flag to make sure that time points are not extrapolated beyond a 2-point segment in */
          /*the profile */

          CuspFlag = FALSE;

          for (;; )
          {

#if  defined(__OS2__)
               if ( RescheduleFlag )
#endif
                    Reschedule( );

               if ( Tfunction.MaxDeltaTime >= MeanDeltaTime )/* tests 1/prob  */
               {
                    break;
               }                       /* end if - breaks out of for loop     */

               /* look at current slope in T program file                       */

               if ( Tfunction.TRamp == 0.0 )/*Slope of current segment        */
               {                       /* increment NewIndex until slope
                                          becomes finite                      */
                    do
                    {
                         NewIndex += 2;
                         IntermediatePrintFlag = TRUE;
                         if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
                         {
                              OutOfRange = TRUE;
                              return;
                         }             /* end if                              */

                         /* update time and temp for new place in profile */
                         ElapsedTime = * ( pPTF + NewIndex - 2 );
                         T = * ( pPTF + NewIndex - 1 );
                         /* get slope etc for new segment */
                         GetSlopeAndMaxDeltaTime( );
                         if ( IntermediatePrintFlag )
                         {
                              OutputCalcStatus( );
                              IntermediatePrintFlag = FALSE;
                         }             /* end if                              */
                         /* check to see if the slope is finite */
                         if ( Tfunction.TRamp )
                         {
                              break;
                         }             /* endif                               */
                    }
                    while ( TRUE );

               }                       /* endif                               */

               /* now evaluate new temp and time steps                      */

               if ( Tfunction.TRamp > 0.0 )
               {
                    T += IDLING_T_STEP;/* increment T                         */

                    /*check to see if the new T is within the bounds of the current time-temperature */
                    /*pairs, use epsilon tolerance to accommodate round-off errors */

                    if ( T >= ( * ( pPTF + NewIndex + 1 ) - EPSILON ) )
                    {/*out of bounds, set temperature to the highest temperature in the current profile*/
                         /*segment, and increase the index to move to the next time-temperature pair and */
                         /*the next segment in the profile */

                         T = * ( pPTF + NewIndex + 1 );
                         CuspFlag = TRUE;
                         NewIndex += 2;
                         IntermediatePrintFlag = TRUE;
                         if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
                         {
                              OutOfRange = TRUE;
                              return;
                         }             /* end if                              */

                         /* get new MaxDeltaTime for the segment */
                         GetSlopeAndMaxDeltaTime( );
                         if ( IntermediatePrintFlag )
                         {
                              OutputCalcStatus( );
                              IntermediatePrintFlag = FALSE;
                         }             /* end if                              */
                    }                  /* end if                              */
               }
               else
               {                       /* Slope (= Tfunction.TRamp) < 0.0     */
                    T -= IDLING_T_STEP;/* decrement T                         */

                    /*check to see if the new T is within the bounds of the current time-temperature */
                    /*pairs, use epsilon tolerance to accommodate round-off errors */

                    if ( T <= ( * ( pPTF + NewIndex + 1 ) + EPSILON ) )
                    {/*out of bounds, set temperature to the lowest temperature in the current profile*/
                         /*segment, and increase the index to move to the next time-temperature pair and */
                         /*the next segment in the profile */

                         T = * ( pPTF + NewIndex + 1 );
                         CuspFlag = TRUE;
                         NewIndex += 2;
                         IntermediatePrintFlag = TRUE;
                         if ( NewIndex > ( NoOfProgTFilePoints - 2 ) )
                         {
                              OutOfRange = TRUE;
                              return;
                         }             /* endif                               */

                         /* get new MaxDeltaTime for the segment */
                         GetSlopeAndMaxDeltaTime( );
                         if ( IntermediatePrintFlag )
                         {
                              OutputCalcStatus( );
                              IntermediatePrintFlag = FALSE;
                         }             /* end if                              */
                    }                  /* end if                              */
               }                       /* endif                               */
               if ( ! OutOfRange )
               {

                    if ( CuspFlag )/*set time step to correspond to the temperature*/
                    {
                         DeltaTime = * ( pPTF + NewIndex - 2 ) - ElapsedTime;
                         CuspFlag = FALSE;
                    }
                    else
                    {
                         /* calculate new DeltaTime by interpolation             */

                         if ( T == * ( pPTF + NewIndex + 1 ) )/* if T is one of points*/
                         {
                              DeltaTime = * ( pPTF + NewIndex ) - ElapsedTime;
                         }
                         else
                         {
                              InterpVal = * ( pPTF + NewIndex ) - ( * ( pPTF + NewIndex + 1 ) - T ) /
                              Tfunction.TRamp;
                              DeltaTime = InterpVal - ElapsedTime;
                         }             /* endif                               */
                    }                  /* end if CuspFlag                     */
                    /* get new probabilities and time step                  */

                    IdlingTimeStep( );
                    if ( ReactionOver )
                    {
                         return;
                    }                  /* endif                               */

                    /* increment system time to keep simulation on track    */

                    ElapsedTime += DeltaTime;
               }
               else
               {                       /* if OutOfRange                       */
                    return;
               }                       /* endif not OutOfRange                */
          }                            /* end for infinite loop               */

          /* return to normal simulator functions                           */

          break;

     default :
          break;
     }                                 /* endswitch                           */
     return;
}                                      /* end function                        */

/*----------------------------------------------------------------------------
   Function IdlingTimeStep
   Purpose: estimates time step by calculating total probability and taking its
   inverse  (mean time step)
   Calls: RxnProbCalc, ZeroProbability
   Parameters: none
   Returns: nothing
-------------------------------------------------------------------------------*/

void IdlingTimeStep( void )

{                                      /* begin function                      */
     unsigned int jr;

     for ( jr = 0, TotalRxnProbability = 0.0; jr < NumberOfRxns; jr++ )
     {
          if ( * ( pRP + jr ) > 0.0 )
          {
               * ( pRP + jr ) = RxnProbCalc( jr );
               TotalRxnProbability += * ( pRP + jr );
          }                            /* endif                               */
     }                                 /* endfor                              */
     if ( TotalRxnProbability == 0.0 )
     {
          ZeroProbability( );
          ReactionOver = TRUE;
          SetReasonForExit( msimZERO_PROBABILITY );
          return;
     }                                 /* endif                               */
     TotalProbability = TotalRxnProbability;
     MeanDeltaTime = 1 / TotalProbability;/* mean value                       */
     return;

}                                      /* end function                        */

/*----------------------------------------------------------------------------
Function TerminateProgT
Purpose: to set up to end the program T function: set flag for analytic
  program, change over to const T simulation for file program
Paramters: none
Calls:  nothing
Returns: nothing
------------------------------------------------------------------------------*/

void TerminateProgT( void )
{                                      /* begin function                      */
     unsigned int jr;
     double Temp, Factor;

     switch ( ( USHORT ) ProgTFormat )
     {
     case msimANALYTIC_TPROG :

          /* set flag to halt simulation                                    */

          FinalTempReached = TRUE;
          break;

     case msimFILE_TPROG :
          if ( OutOfRange )            /* convert over to a const T
                                          simulation                          */
          {
               T = Tfunction.FinalTemp;
               OutputCalcStatus( );    /* record where the program ended      */
               OutputTempAnyway = TRUE;
               switch ( TempAndPressOption )
               {
               case msimPROG_T_CONST_P_v :
                    TempAndPressOption = msimCONST_TP_v;
                    break;
               case msimPROG_T_CONST_P_VAR_V :
                    TempAndPressOption = msimCONST_TP_VAR_V;
                    break;
               case msimPROG_T_VAR_P_CONST_V :
                    TempAndPressOption = msimCONST_T_VAR_P_CONST_V;
                    break;
               default :
                    break;
               }                       /* endswitch                           */

               /* if constT Arrhrate calculate rate constants and store in RateConstant[0]*/

               for ( jr = 0; jr < NumberOfRxns; jr++ )
               {
                    Temp = ( pKD + jr ) ->RateConstant[0];
                    if ( Temp )
                    {
                         if ( ( pKD + jr ) ->RateConstant[1] )
                         {
                              Temp *= pow( T, ( pKD + jr ) ->RateConstant[1] );
                         }
                         if ( ( pKD + jr ) ->RateConstant[2] )
                         {
                              Factor = ( ( pKD + jr ) ->RateConstant[2] ) / ( R * T );

                              if ( - Factor < MINIMUM_EXPONENT )
                                   Factor = - MINIMUM_EXPONENT;

                              Temp *= exp( - Factor );
                         }             /* endif                               */
                         ( pKD + jr ) ->RateConstant[0] = Temp;
                    }                  /* endif Temp                          */
               }                       /* endfor jr                           */
               if ( EquilDetectOption == EQUIL_DETECT_LATER )
               {
                    EquilDetectOption = EQUIL_DETECT;
               }                       /* endif                               */
          }                            /* endif                               */
          break;
     default :
          break;
     }                                 /* endswitch                           */
     return;

}                                      /* end function                        */

/*----------------------------------------------------------------------------
     Description function AdiabaticPressUpdate
     Purpose : does an adiabatic expansion/compression on the system to maintain
        const vol, always does P correction and  T correction
     Parameters: none
     Returns: nothing
------------------------------------------------------------------------------*/

void AdiabaticPressUpdate( void )
{                                      /* begin function                      */
     double jc, VolumeRatio, cp;
     unsigned int j;

     for ( jc = 0.0, j = 1; j <= NumberOfSpecies; j++ )
     {
          jc += * ( pNP + j );
     }                                 /* end for                             */
     VolumeRatio = ( GAS_CONSTANT * jc * T ) / ( P * ConversionFactor );
     cp = TotalHeatCapacity / jc;
     T = T * pow( VolumeRatio, ( 1.98726 / ( cp - 1.98726 ) ) );
     P = P * pow( VolumeRatio, ( cp / ( cp - 1.98726 ) ) );
     return;
}                                      /* end function                        */



/* -------------------------------------------------------------------------
  Definition of function IsothermalPressUpdate
   Purpose: does an isothermal expansion/compression on the system to maintain
        constant vol since the reaction is assumed to be at constant pressure.
        Assumes appropriate heat exchange with system. The new P is calculated
        from the new T, new # moles and the initial volume.
   Parameters: none
   Returns: nothing
---------------------------------------------------------------------------*/

void IsothermalPressUpdate( void )
{                                      /* begin function                      */
     unsigned int j;
     double MolesGas = 0.0;

     for ( j = 1; j <= NumberOfSpecies; j++ )
     {
          MolesGas += * ( pNP + j );
     }                                 /* end for                             */
     P = ( MolesGas * GAS_CONSTANT * T ) / ( ConversionFactor );
     return;

}                                      /* end function                        */



/*---------------------------------------------------------------------
  Definition of function GasVolUpdate
  Purpose: Uses ideal gas law to calculate new volume from
  current state of system
  Calls: nothing
  Returns: nothing
  --------------------------------------------------------------------      */

void GasVolUpdate( void )
{                                      /* begin function                      */
     double MolesGas = 0.0;
     unsigned int j;

     for ( j = 1; j <= NumberOfSpecies; j++ )
     {
          if ( ( ( pSpD + j ) ->Phase ) == InitialPhase )
          {
               MolesGas += * ( pNP + j );
          }
     }
     V = ( MolesGas * GAS_CONSTANT * T ) / ( P * ParticlesPerMol );
     return;

}                                      /* end function                        */



/*----------------------------------------------------------------------
  Definition of function CondensedVolUpdate
  Purpose : to calculate system volume from moles and densities
  when components are liquids or solids
  Calls: nothing
  Returns: nothing
  --------------------------------------------------------------------      */

void CondensedVolUpdate( void )
{                                      /* begin function                      */
     unsigned int j;

     for ( V = 0.0, j = 1; j <= NumberOfSpecies; j++ )
     {
          if ( ( ( pSpD + j ) ->Phase ) == InitialPhase )
          {
               V += ( * ( pNP + j ) / ( ParticlesPerMol * (( pSpD + j ) ->Density ) ) );
          }
     }
     return;

}                                      /* end function                        */



/*--------------------------------------------------------------------
 Definition of function ConcentrationUpdate
 Purpose: to renormalize the rate constants (in particles units) after
 a volume change (i.e. a concentration change)
 Calls: nothing
 Returns: nothing
 ----------------------------------------------------------------------     */

void ConcentrationUpdate( double OldV )
{                                      /* begin function                      */
     unsigned int jr;
     double Factor;

      // added to avoid fp error when no reactant particles are left. OK to
      // set V to an incorrect but nonzero value since immediatlye following
      // this fcn the reaction probabilities will be determined to be == 0.0
      // and the simulation will terminate    7.19.94   

     if ( V == 0.0)    
          V = OldV;

      // end of 7.19.94 fix
                     
     for ( jr = 0; jr < NumberOfRxns; jr++ )
     {
          if ( ( pKD + jr ) ->RateConstant[0] )
          {
               if ( ( pKD + jr ) ->UnitsConversionExponent != 0.0 )
               {
                    Factor = pow(( OldV / V ), ( ( pKD + jr ) ->UnitsConversionExponent )
                    );
                    ( pKD + jr ) ->RateConstant[0] *= Factor;
               }                       /* endif UCE                           */
          }                            /* endif RC[0]                         */
     }                                 /* endfor                              */
     return;

}                                      /* end function                        */




/*-----------------------------------------------------------------------------

    Function CheckForEquilibration
    Purpose: examines event selection record to see if an equilibrium is present
          and saves indices of all maintained equilibria
    Parameters: none
    Returns  : nothing

    Change history: 2/7/96 modified test structure for equilibrium diagnosis
	to remove bias toward multiple equilibria with equal selection
	frequencies (FH)

------------------------------------------------------------------------------*/

void CheckForEquilibration( void )
{
     unsigned int i, j;
     long SelectionFrequency;
     double PairEfficiency, PairFrequency;


#if ( PROFILE & RECORD_EQUIL_TABLE )
     PrfRecordEquilTable( );
#endif

     DominantRxn = FALSE;              /* ensure flag is set                  */

     /* calculate reaction propagation efficiency                           */

     for ( j = 0, i = 0; i < NumberOfRxns; i += 2 )
     {

/* if the difference between these is below the limit then we are spending most*/
	  /* of our time in nonproductive shuttling of this rxn             */

	  if ( * ( pRE + i ) > 0 )
	  {
	       PairEfficiency = fabs((( double ) * (pRE + i ) - ( double ) * (pRE + i + 1 ) ) /
		    ( double ) * (pRE + i ) );
	       if ( PairEfficiency < MinEfficiency )
	       {
		    * ( pEqR + j ) = i;/* saves index of forward rx in
					  equil pair                          */
		    j++;
	       }                       /* end if                              */
	  }                            /* end if *(pRE)                       */
     }                                 /* end for                             */
     NumberOfEquilRxns = j;

/* now test that the equilibrated rxns are dominating */

     if ( NumberOfEquilRxns > 1 )
     {
	  SelectionFrequency = 0;
	  for ( i = 0; i < NumberOfEquilRxns; i++ )
	  {
	       j = ( * ( pEqR + i ) );
	       SelectionFrequency += ( * ( pRE + j ) + * ( pRE + j + 1 ) );
	  }                            /* endfor                              */
     }
     else
     {
	  j = ( * ( pEqR ) );
	  SelectionFrequency = * ( pRE + j ) + * ( pRE + j + 1 );
     }                                 /* endif                               */

     /*  set flag if total selection frequency passes filter   */
     if ( ( SelectionFrequency / ( double ) EquilTestCycle ) > MinFrequency )
     {

	  DominantRxn = TRUE;

     }                                 /* endif SelectionFrequency            */


/* clear accumulation array for new cycle  */
     for ( i = 0; i < NumberOfRxns; i++ )
     {
	  * ( pRE + i ) = 0;
     }
     EquilTestCycleCount = 1;
     return;
}





 

/*--------------------------------------------------------------------------*/
/*      Definition of Function SaveSimulatorState()                         */
/*                                                                          */
/*      This function fills a structure of type STATE_STRUCT with           */
/*      the current values of various global variables, then writes         */
/*      the structure to the output file F. Following that the values       */
/*      in several arrays are also written to the file in a specific        */
/*      order.  The function returns the position in the file where         */
/*      the STATE_STRUCT data starts. This is to be saved in the file       */
/*      header so that we know where to look for the STATE_STRUCT data      */
/*      when a simulation is being resumed.                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void SaveSimulatorState( void )
{
     STATE_STRUCT s;
     unsigned int i;
	 

     /* save current position in file - we need to place it in file header later*/

     if ( -1L == ( ResumeDataOffset = ftell( OutFile ) ) )
     {
          RecordFatalErrorLocation( msimWRITE_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }
	 
#if ( PROFILE & RECORD_SAVED_STATE )

	long prev_offset;
	long cur_offset;
	
	memset( &s, '\xFF', sizeof ( STATE_STRUCT ) );

	
    PrfFile << "Start Record Saved State ot file offset = " <<  setw( NUMERIC_WIDTH ) << ResumeDataOffset << endl;
	
	cur_offset = ftell( OutFile );
	
    PrfFile << tab << "SavedStateStruct start offset = " <<  setw( NUMERIC_WIDTH ) << cur_offset << endl;
    PrfFile << tab << tab << "size of SavedStateStruct  = " << setw( NUMERIC_WIDTH ) <<  sizeof( STATE_STRUCT ) << endl;
	
	prev_offset = cur_offset;
	
#endif

     s.InitialPhase = InitialPhase;
     s.TempAndPressOption = TempAndPressOption;
     s.EquilDetectOption = EquilDetectOption;
     s.ProgTFileIndex = ProgTFileIndex;
     s.NewIndex = NewIndex;
     s.EventCount = EventCount;
     s.EquilTestCycleCount = EquilTestCycleCount;
     s.TotalProbability = TotalProbability;
     s.TotalRxnProbability = TotalRxnProbability;
     s.ParticlesPerMol = ParticlesPerMol;
     s.V = V;
     s.P = P;
     s.T = T;
     s.TotalEnthalpy = TotalEnthalpy;
     s.TotalHeatCapacity = TotalHeatCapacity;
     s.ConversionFactor = ConversionFactor;
     s.MinFrequency = MinFrequency;
     s.PreviousTotalRxnProbability = PreviousTotalRxnProbability;
     s.ElapsedTime = ElapsedTime;
     s.NoOfProgTFilePoints = NoOfProgTFilePoints;
     s.NumberOfRecordsWritten = NumberOfRecordsWritten;
     s.FinalTemp = Tfunction.FinalTemp;
     s.MaxDeltaTemp = Tfunction.MaxDeltaTemp;
     s.MaxDeltaTime = Tfunction.MaxDeltaTime;
     s.IdlingTimeStep = Tfunction.IdlingTimeStep;
     s.TRamp = Tfunction.TRamp;
     s.Slope = Tfunction.Slope;

     RandomNum.GetState( s.RandomNoSeed );

     /* write out the structure s                                           */

     fwrite( &s, sizeof ( s ), 1, OutFile );

	 
#if ( PROFILE & RECORD_SAVED_STATE )

     PrfFile << tab << tab << VALUE( s.InitialPhase );    
     PrfFile << tab << tab << VALUE( s.TempAndPressOption);
     PrfFile << tab << tab << VALUE( s.EquilDetectOption);
     PrfFile << tab << tab << VALUE( s.ProgTFileIndex);
     PrfFile << tab << tab << VALUE( s.NewIndex );
     PrfFile << tab << tab << VALUE( s.EventCount );
     PrfFile << tab << tab << VALUE( s.EquilTestCycleCount );
     PrfFile << tab << tab << VALUE( s.NumberOfRecordsWritten );
     PrfFile << tab << tab << VALUE( s.TotalProbability );
     PrfFile << tab << tab << VALUE( s.TotalRxnProbability );
     PrfFile << tab << tab << VALUE( s.ParticlesPerMol );
     PrfFile << tab << tab << VALUE( s.V );
     PrfFile << tab << tab << VALUE( s.P );
     PrfFile << tab << tab << VALUE( s.T );
     PrfFile << tab << tab << VALUE( s.TotalEnthalpy );
     PrfFile << tab << tab << VALUE( s.TotalHeatCapacity );
     PrfFile << tab << tab << VALUE( s.ConversionFactor );
     PrfFile << tab << tab << VALUE( s.MinFrequency );
     PrfFile << tab << tab << VALUE( s.PreviousTotalRxnProbability );
     PrfFile << tab << tab << VALUE( s.ElapsedTime );
     PrfFile << tab << tab << VALUE( s.NoOfProgTFilePoints );
     PrfFile << tab << tab << VALUE( s.RandomNoSeed[0] );
     PrfFile << tab << tab << VALUE( s.RandomNoSeed[1] );
     PrfFile << tab << tab << VALUE( s.RandomNoSeed[2] );
     PrfFile << tab << tab << VALUE( s.FinalTemp );
     PrfFile << tab << tab << VALUE( s.MaxDeltaTemp );
     PrfFile << tab << tab << VALUE( s.MaxDeltaTime );
     PrfFile << tab << tab << VALUE( s.IdlingTimeStep );
     PrfFile << tab << tab << VALUE( s.TRamp );
     PrfFile << tab << tab << VALUE( s.Slope );


	cur_offset = ftell( OutFile );
	
    PrfFile << tab << tab << "size of STATE_STRUCT in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;


    PrfFile << tab << "NoPrtcls array start offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl;
	
#endif

     /* now write the whole array of concentrations out at once             */

     fwrite( &NoPrtcls[1], sizeof ( NoPrtcls[1] ), NumberOfSpecies, OutFile );

     /* the current rxnprobs                                                */

#if ( PROFILE & RECORD_SAVED_STATE )

	cur_offset = ftell( OutFile );
	
    PrfFile << tab << tab << "size of NoPrtcls in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;

    PrfFile << tab << "RxnProbs array start offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl;
	
#endif

     fwrite( RxnProbs, sizeof ( RxnProbs[0] ), NumberOfRxns, OutFile );

     /* etc                                                                 */

#if ( PROFILE & RECORD_SAVED_STATE )

	cur_offset = ftell( OutFile );
	
    PrfFile << tab << tab << "size of RxnProbs in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;

    PrfFile << tab << "RxnEventCtr array start offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl;
	
#endif

     fwrite( RxnEventCtr, sizeof ( RxnEventCtr[0] ), NumberOfRxns, OutFile );

     // EquilRxns is typed as unsigned ints; upon saving we convert them to ULONG before writing
     // so that all operating systems are saving them as 4 byte entities

#if ( PROFILE & RECORD_SAVED_STATE )

	cur_offset = ftell( OutFile );
	
    PrfFile << tab << tab << "size of RxnEventCtr in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;

    PrfFile << tab << "EquilRxns array start offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl;
	
#endif

     for ( i = 0; i < NumberOfRxns / 2; )
     {
          unsigned long temp_ul;

          temp_ul = ( unsigned long ) EquilRxns[i];
          fwrite( &temp_ul, sizeof temp_ul, 1, OutFile );
          i++;
     }


#if ( PROFILE & RECORD_SAVED_STATE )

	cur_offset = ftell( OutFile );
	
    PrfFile << tab << tab << "size of EquilRxns in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;

    PrfFile << tab << "RateConstant & UnitsConversionExponent arrays start offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl;
	
#endif



     for ( i = 0; i < NumberOfRxns; i++ )
     {
          fwrite( &(( pKD + i ) ->RateConstant[0] ), sizeof ( pKD->RateConstant[0] ), 1,
               OutFile );

          fwrite( &(( pKD + i ) ->UnitsConversionExponent ), sizeof ( pKD->
                    UnitsConversionExponent ), 1, OutFile );
     }

     /* check for errors and abort                                          */
     /* if an IO error has occurred                                         */

     if ( ferror( OutFile ) )
     {
          RecordFatalErrorLocation( msimWRITE_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }
	 
#if ( PROFILE & RECORD_SAVED_STATE )

	cur_offset = ftell( OutFile );

	
    PrfFile << tab << tab << "RateConstant & UnitsConversionExponent in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-prev_offset) << endl;
	
	prev_offset = cur_offset;
	
	
	PrfFile << tab << "Overall size of all state data  in file = " << setw( NUMERIC_WIDTH ) << (cur_offset-ResumeDataOffset) << endl;

    PrfFile << "End Record Saved State at offset = " << setw( NUMERIC_WIDTH ) << cur_offset << endl << endl;
	
	PrfCloseFile( );

#endif
	 
     return;
}




/*--------------------------------------------------------------------------*/
/*      Definition of Function RestoreSimulatorState()                      */
/*                                                                          */
/*      This function sets various global variables by reading their        */
/*      values saved to the output file from a previous run of the          */
/*      simulator. The file position in the output file is then set to      */
/*      the position at then end of the simulation results data.            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void RestoreSimulatorState( void )
{
     STATE_STRUCT s;
     unsigned int i;
     msimINSTANCE init_file_record;

     /* read in header from output file and get STATE_STRUCT offset from it */

     fseek( OutFile, 0L, SEEK_SET );
     fread( &init_file_record, sizeof ( init_file_record ), 1, OutFile );

     /* move file position to strcuture s  and read it in                   */

     if ( 0 != fseek( OutFile, init_file_record.sim_state_data_file_offset.offset, SEEK_SET ) )
     {
          RecordFatalErrorLocation( msimREAD_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     fread( &s, sizeof ( s ), 1, OutFile );

     /* restore variables from the struct we just got                       */

     InitialPhase = s.InitialPhase;
     TempAndPressOption = s.TempAndPressOption;
     EquilDetectOption = s.EquilDetectOption;
     ProgTFileIndex = s.ProgTFileIndex;
     NewIndex = s.NewIndex;
     EventCount = s.EventCount;
     EquilTestCycleCount = s.EquilTestCycleCount;
     TotalProbability = s.TotalProbability;
     TotalRxnProbability = s.TotalRxnProbability;
     ParticlesPerMol = s.ParticlesPerMol;
     V = s.V;
     P = s.P;
     T = s.T;
     TotalEnthalpy = s.TotalEnthalpy;
     TotalHeatCapacity = s.TotalHeatCapacity;
     ConversionFactor = s.ConversionFactor;
     MinFrequency = s.MinFrequency;
     PreviousTotalRxnProbability = s.PreviousTotalRxnProbability;
     ElapsedTime = s.ElapsedTime;
     NumberOfRecordsWritten = s.NumberOfRecordsWritten;
     NoOfProgTFilePoints = s.NoOfProgTFilePoints;
     Tfunction.FinalTemp = s.FinalTemp;
     Tfunction.MaxDeltaTemp = s.MaxDeltaTemp;
     Tfunction.MaxDeltaTime = s.MaxDeltaTime;
     Tfunction.IdlingTimeStep = s.IdlingTimeStep;
     Tfunction.TRamp = s.TRamp;
     Tfunction.Slope = s.Slope;

     RandomNum.SetState( s.RandomNoSeed );

     /* now write the whole array of concentrations out at once             */

     fread( &NoPrtcls[1], sizeof ( NoPrtcls[1] ), NumberOfSpecies, OutFile );

     /* the current rxnprobs                                                */

     fread( RxnProbs, sizeof ( RxnProbs[0] ), NumberOfRxns, OutFile );

     /* etc                                                                 */

     fread( RxnEventCtr, sizeof ( RxnEventCtr[0] ), NumberOfRxns, OutFile );

     // EquilRxns is typed as unsigned ints; upon saving we converted them  to ULONG before writing
     // so that all operating systems are saving them as 4 byte entities
     // we now have to convert them back

     for ( i = 0; i < NumberOfRxns / 2; )
     {
          unsigned long temp_ul;

          fread( &temp_ul, sizeof temp_ul, 1, OutFile );
          EquilRxns[i] = ( unsigned int ) temp_ul;
          i++;
     }

     for ( i = 0; i < NumberOfRxns; i++ )
     {
          fread( &(( pKD + i ) ->RateConstant[0] ), sizeof ( pKD->RateConstant[0] ), 1,
               OutFile );

          fread( &(( pKD + i ) ->UnitsConversionExponent ), sizeof ( pKD->
                    UnitsConversionExponent ), 1, OutFile );
     }

     /* check for errors and abort                                          */
     /* if an IO error has occurred                                         */

     if ( ferror( OutFile ) )
     {
          RecordFatalErrorLocation( msimREAD_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     /* put file position back to end of simulation data                    */

     if ( 0 != fseek( OutFile, init_file_record.sim_state_data_file_offset.offset, SEEK_SET ) )
     {
          RecordFatalErrorLocation( msimREAD_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
          FatalError( );
     }

     /* now call SetUpUpdateTables if necesarry                             */

     if ( TempAndPressOption == msimCONST_TP_v || TempAndPressOption ==
               msimCONST_T_VAR_P_CONST_V || ( ProgTFormat == msimFILE_TPROG &&
                    TempAndPressOption != msimPROG_T_CONST_P_VAR_V ) )

          if ( SetUpUpdateTables( ) != msimNO_ERROR )
          {
               RecordFatalErrorLocation( msimMEM_ALLOC_ERROR, msimNULL_STR, __FILE__, __TIMESTAMP__, __LINE__ );
               FatalError( );
          }

     return;
}

/* +-------------------------------------------------------------------------+
   |Definition of function GracefulExit:                                     |
   |Purpose : cleans up things, closes output file.                          |
   |Not that this is installed as an exit procedure in main() using '        |
   |the function _atexit() and is called automatically upon any              |
   |termination                                                              |
   |Parameters : none; returns : nothing                                     |
   |Note : calculates time required for the current calculation and displays |
   |it                                                                       |
   |                                                                         |
   +-------------------------------------------------------------------------+*/

void FAR GracefulExit( void )

{
     msimINSTANCE init_file_record;
     time_t current_wallclock_time;

     time( &current_wallclock_time );

     StopTime = clock( );

     sprintf( ExecutionTime, "%1.0f", difftime( current_wallclock_time, StartWallClockTime ) );

     /* if the simulation was interupted save its current state             */

     if ( GetReasonForExit( ) == msimUSER_ABORT )
          SaveSimulatorState( );

     /* Here we update the first record in the reaction file so as to       */
     /* specify the total number of output records appended                 */

     if ( OutFile != NULL )
     {

          /* get the existing record                                        */

          fseek( OutFile, 0L, SEEK_SET );
          fread( &init_file_record, sizeof ( init_file_record ), 1, OutFile );

          /* update the data                                                */

          init_file_record.num_simulation_data_records =
          NumberOfRecordsWritten;
          init_file_record.sim_state_data_file_offset.offset = ResumeDataOffset;
          init_file_record.sim_return_code = GetReasonForExit( );
          strcpy( init_file_record.execution_time_str, ExecutionTime );

          /* write it back to the file                                      */

          fseek( OutFile, 0L, SEEK_SET );
          fwrite( &init_file_record, sizeof ( init_file_record ), 1, OutFile );

          if ( ferror( OutFile ) )
          {
               RecordFatalErrorLocation( msimWRITE_ERROR, "<<rxn file>>", __FILE__, __TIMESTAMP__, __LINE__ );
               FatalError( );
          }

          fclose( OutFile );

          DisplaySimulationExitStatus( EventCount, T, ElapsedTimeLimit );

     }                                 /* end if                              */
}



// wdh 10.11.94
// these are for profiling internal behavior of simulation  engine

#if ( PROFILE != RECORD_NOTHING )


void PrfOpenFile( )
{
     struct tm *current_time;
     time_t local_time;
     long int curpos;
     msimRXN rxn;
     msimSPECIES species;
     size_t index;

     time( &local_time );              /* get time in seconds                 */
     current_time = localtime( &local_time );

     PrfFile.open( PROFILE_FILENAME );
     PrfFile.setf( ios::left);
     PrfFile << "===== Open profile =====" << endl;
     PrfFile << "Reaction File : " << PrfRxnFileName << endl;
     PrfFile << "Profile created on " << asctime( current_time ) << endl << endl;

     // allocate memory to store the names of all the species
     PrfSpecNameArray = new  msimNAME_STRING [ NumberOfSpecies ];
     PrfEquationArray =  new msimEQUATION_STRING [ NumberOfRxns ];

    // save current file position in open rxn file since we are about to move around

     curpos = ftell( OutFile );

     // move to start of rxn list
     fseek( OutFile, (long) sizeof(msimINSTANCE), SEEK_SET );

     // read in first rxn record
     fread( &rxn, sizeof rxn, 1, OutFile );
     index = 0;

     strcpy( PrfEquationArray[ index ], rxn.equation );
     index++;

     strcpy( PrfEquationArray[ index ], rxn.equation );
     index++;


     // walk through list saving all equations

      while ( rxn.next != NULL )
      {
          fread( &rxn, sizeof rxn, 1, OutFile );

          strcpy( PrfEquationArray[ index ], rxn.equation );
          index++;

          strcpy( PrfEquationArray[ index ], rxn.equation );
          index++;
      }

     // now we are at the start of the species list

     fread( &species, sizeof species, 1, OutFile );
     index = 0;

     strcpy( PrfSpecNameArray[ species.index-1 ], species.name );
     index++;

     while ( species.next != NULL )
     {
          fread( &species, sizeof species, 1, OutFile );
          strcpy( PrfSpecNameArray[ species.index-1 ], species.name );
          index++;
     }

    // put OutFile Position back where we started from

     fseek( OutFile, curpos, SEEK_SET );
}

void PrfRecordCurrentState( )
{
     PrfFile << "Start Record Current State" << endl;

#if ( PROFILE & RECORD_GLOBALS )
	PrfFile << VALUE( ElapsedTime );
	PrfFile << VALUE( EventCount );
	PrfFile << VALUE( WriteIntervalCount );
	PrfFile << VALUE( NumberOfRecordsWritten );
	PrfFile << VALUE( EquilTestCycleCount );
	PrfFile << VALUE( T );
	PrfFile << VALUE( V );
	PrfFile << VALUE( P );
	PrfFile << VALUE( TotalHeatCapacity );
	PrfFile << VALUE( TotalEnthalpy );
	PrfFile << VALUE( PreviousElapsedTime );
	PrfFile << VALUE( DeltaTime );
	PrfFile << VALUE( MeanDeltaTime );
	PrfFile << VALUE( WaitingTime );
	PrfFile << VALUE( TotalProbability );
	PrfFile << VALUE( TotalRxnProbability );
	PrfFile << VALUE( PreviousTotalRxnProbability );
	PrfFile << VALUE( RandomNo );
#endif

     unsigned int i;

#if ( PROFILE & RECORD_PROBS )
	PrfFile << tab << "Rxn Probabilities :" << endl;
     PrfFile << tab << tab << setw( NUMERIC_WIDTH) <<"FwdProb" << setw( NUMERIC_WIDTH) << "RevProb" << "Equation" << endl;

     for ( i = 0; i < NumberOfRxns; i += 2  )
          PrfFile << tab << tab << setw( NUMERIC_WIDTH ) << *( pRP + i ) << " " << setw( NUMERIC_WIDTH ) << *( pRP + i + 1 )<< " " << PrfEquationArray[ i ] << endl;
#endif

#if ( PROFILE & RECORD_CONCS )
	PrfFile << tab << "Species Particle Counts :" << endl;
     PrfFile << tab << tab <<  setw( NUMERIC_WIDTH ) << "NumParticles" << "SpeciesName" << endl;

     for ( i = 1; i <= NumberOfSpecies; i++ )
          PrfFile << tab << tab << setw( NUMERIC_WIDTH ) << NoPrtcls[i] << " " << PrfSpecNameArray[i-1] << endl;
#endif

     PrfFile << "End Record Current State" << endl << endl;
}


void PrfCloseFile( )
{
     PrfRecordCurrentState( );
     PrfFile << endl<< "===== Close profile =====" << endl;
     PrfFile.close( );

     delete[] PrfSpecNameArray;
     delete[] PrfEquationArray;
}

#if ( PROFILE & RECORD_INIT_STATE )

void PrfRecordInitState( )
{
     PrfFile << "Start Record Initial State" << endl;

#if ( PROFILE & RECORD_GLOBALS )
	PrfFile << VALUE( NumberOfSpecies );
	PrfFile << VALUE( NumberOfRxns );
	PrfFile << VALUE( NoOfSpecWithNonZeroInitConc );
	PrfFile << VALUE( ResumeSimulation );
	PrfFile << VALUE( ParticlesPerMol );
	PrfFile << VALUE( ConversionFactor );
	PrfFile << VALUE( InitialT );
	PrfFile << VALUE( ElapsedTimeLimit );
     PrfFile << VALUE( WriteInterval );
	PrfFile << VALUE( TempAndPressOption );
	PrfFile << VALUE( TConvergence );
	PrfFile << VALUE( InitialPhase );
	PrfFile << VALUE( MinEfficiency );
#endif

#if ( PROFILE & RECORD_CONCS )

     unsigned int i;

	PrfFile << tab << "Species Particle Counts :" << endl;
     PrfFile << tab << tab <<  setw( NUMERIC_WIDTH ) << "NumParticles" << "SpeciesName" << endl;

     for ( i = 1; i <= NumberOfSpecies; i++ )
          PrfFile << tab << tab << setw( NUMERIC_WIDTH ) << NoPrtcls[i] << " " << PrfSpecNameArray[i-1] << endl;
#endif

     PrfFile << "End Record Initial State" << endl << endl;
}

#endif

#if ( PROFILE & RECORD_EQUIL_TABLE )

void PrfRecordNonEquilRxnSelection( double Lambda, double DominantProbability )
{
     PrfFile << "Equilibrium Detected - Start Record NonEquil Rxn Selection" << endl;
	PrfFile << VALUE( DominantRxn );
	PrfFile << VALUE( ElapsedTime );
	PrfFile << VALUE( NumberOfEquilRxns );
	PrfFile << VALUE( WaitingTime );
	PrfFile << VALUE( SelectedRxnNo );
     PrfFile << tab << "Selected Rxn = " << SelectedRxnNo << " ( odd = rev rxn);       " << PrfEquationArray[SelectedRxnNo] << endl ;
	PrfFile << VALUE( TotalRxnProbability );
	PrfFile << VALUE( ElapsedTime );
	PrfFile << VALUE( Lambda );
	PrfFile << VALUE( DominantProbability );

     PrfFile << "End Record NonEquil Rxn Selection" << endl << endl;
}



void PrfRecordEquilTable( )
{
     PrfFile << "Start Record Equil Table" << endl;
	PrfFile << VALUE( ElapsedTime );
	PrfFile << VALUE( EventCount );
	PrfFile << VALUE( WriteIntervalCount );
	PrfFile << VALUE( NumberOfRecordsWritten );
	PrfFile << VALUE( EquilTestCycleCount );

	PrfFile << tab << "Rxn Selection Count Table :" << endl;
     PrfFile << tab << tab << setw( NUMERIC_WIDTH ) << "FwdSelns" << setw( NUMERIC_WIDTH ) <<  "RevSelns" << "Equation" << endl;

     for ( unsigned int i = 0; i < NumberOfRxns; i += 2 )
          PrfFile << tab << tab << setw( NUMERIC_WIDTH ) << * (pRE + i ) << " " << setw( NUMERIC_WIDTH ) << * (pRE + i + 1) << " " << PrfEquationArray[ i ] << endl;

     PrfFile << "End Record Equil Table" << endl << endl;
}

#endif


#endif



