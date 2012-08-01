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

/* []-----------------------------------------------------------------------[]
   |  msim1.hxx      :   main header file for msim                          |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : contains global declarations for msim                |
   |                   not related to Starview                              |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 11,1993                                        |
   |                                                                        |
   | This is the first  main header file for MSIM. It contains struct,      |
   | constants, #defines and typedefs                                       |
   |                                                                        |
   | CHANGE LOG :  5.30.95 chnaged .cnf, .res file names to CKS.xxx  (wdh)  |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#if !defined(MSIM2_HXX)
#define Window                       void
#define ModelessDialog               void
#endif

#ifndef  MSIM1_HXX
#define  MSIM1_HXX

#include "msim.hrc"
#include "msim.hhc"
#include <stdio.h>

/* global #defines                                                          */

#ifndef  TRUE
#define  TRUE                                    1
#endif

#ifndef  FALSE
#define  FALSE                                   0
#endif

#ifndef  __TIMESTAMP__
#define  __TIMESTAMP__                           __DATE__
#endif

#if      defined(__MSDOS__)
#define  PTR                                     far *

#else
#define  PTR                                     *
#endif


#define  CHAR                                    char
#define  SHORT                                   short
#define  LONG                                    long
#define  VOID                                    void

#if defined(__MAC__)

#define  msimRXNFILE_TEMPLATE                    ":"
#define  msimEXTERN_FILE_TEMPLATE                ":"
#define  msimTEXTOUT_FILE_TEMPLATE               ":"
#define  msimGRAPHOUT_FILE_TEMPLATE              ":"
#define  msimEXT_T_PROFILE_TEMPLATE              ":"

#else

#define  msimRXNFILE_TEMPLATE                    "*.rxn"
#define  msimEXTERN_FILE_TEMPLATE                "*.dat"
#define  msimTEXTOUT_FILE_TEMPLATE               "*.txt"
#define  msimGRAPHOUT_FILE_TEMPLATE              "*.grf"
#define  msimEXT_T_PROFILE_TEMPLATE              "*.prf"

#endif

#define  msimRXNFILE_TYPE                        "RXN "
#define  msimTEXTFILE_TYPE                       "TEXT"
#define  msimTPROFILE_TYPE                       "TEXT"
#define  msimGRAFFILE_TYPE                       "TEXT"
#define  msimEXTDATFILE_TYPE                     "TEXT"
#define  msimCREATOR_NAME                        "MSIM"
#define  msimDEFAULT_CREATOR_NAME                ""


#define msimCOMMENT_CHAR                         '*'
#define  msimOUTPUT_FILENAME                     "runparm.dat"
#define  msimMECHANISM_FLAG                      "MECHANISM:"
#define  msimRESUME_SIM_FLAG                     "RESUME:"
#define  msimEXECUTION_FLAG                      "EXECUTION:"
#define  msimTHERMOCHEM_FLAG                     "THERMOCHEMISTRY:"
#define  msimNULL_STR                            ""
#define  msimBLANK_STR                           " "
#define  msimNULL_EQN                            msimNULL_STR
#define  msimNUMBER_OF_OPTIONS                   4
#define  msimMAX_NUMBER_OF_COMPONENTS            4
#define  msimNUMBER_OF_THERM_COEFFS              5
#define  msimCFG_FILE_FLG                        "MSIM 1.0 cfg"
#define  msimCFG_FILE_FLG_LENGTH                 13

// here we define slight differences in the rxn file ID tag
// for different OS's

#define  msimRXN_FILE_FLG_DOS                    "MSIM 1.0 rxn"
#define  msimRXN_FILE_FLG_MAC                    "MSIM 1.0 Rxn"
#define  msimRXN_FILE_FLG_AIX                    "MSIM 1.0 rXn"
#define  msimRXN_FILE_FLG_PMAC                   "MSIM 1.0 rxN"
#define  msimRXN_FILE_FLG_OS2                    msimRXN_FILE_FLG_DOS


#if defined(__OS2__)
#define  msimRXN_FILE_FLG                        msimRXN_FILE_FLG_OS2
#endif

#if defined(__MSDOS__)
#define  msimRXN_FILE_FLG                        msimRXN_FILE_FLG_DOS
#endif

#if defined(__MAC__)
   #if defined(__PPC__)
      #define  msimRXN_FILE_FLG                        msimRXN_FILE_FLG_PMAC
   #else
      #define  msimRXN_FILE_FLG                        msimRXN_FILE_FLG_MAC
   #endif		  
#endif

#if defined(__AIX__)
#define  msimRXN_FILE_FLG                        msimRXN_FILE_FLG_AIX
#endif



#define  msimRXN_FILE_FLG_LENGTH                 13
#define  msimAPP_DESCRIPTION                     "MSIM Chemical Reaction Simulator"
#define  msimBASE_FILENAME_LENGTH                32
#define  msimRESERVED_LENGTH                     (512 - 15 - \
                       msimBASE_FILENAME_LENGTH - (msimLENGTH_OF_REAL_FIELD + 1 ))
#define  msimINIT_PARTICLES                      "10000"
#define  msimINIT_EVENTS                         "4000000000"
#define  msimINIT_INTERVAL                       "75"
#define  msimINIT_TEMP_VAL                       "298.15"
#define  msimMAX_TEMP_VAL                        "500.00"
#define  msimREAL_ONE_STRING                     "1.00"
#define  msimDEFAULT_DENSITY                     msimREAL_ONE_STRING
#define  msimINIT_TOLERANCE                      "0.1"
#define  msimINIT_TEMP_STEP_SIZE                 "10.0"
#define  msimINIT_SEED                           "12947"
#define  msimINIT_CYCLE_COUNT                    "100"
#define  msimINIT_MIN_EFF                        "90.0"
#define  msimSTRING_LENGTH                       256
#define  msimMAX_NO_INSTANCES                    9
#define  msimREAL_ZERO_STRING                    "0.0"
#define  msimREAL_ONE_STR                        "1.00"
#define  msimZERO_TAIL                           ".00"
#define  msimINITIALIZATION_FILE                 "cks.cnf"
#define  msimRESOURCE_FILENAME                   "cks.res"
#define  msimRN_SEED_2                           "30873"
#define  msimRN_SEED_3                           "28541"
#define  msimINIT_FILENAME                       "<none loaded>"
#define  msimDEFAULT_POINT_SIZE                  10
#define  msimEXTRA_PLOT_ARRAY_PTS                16
#define  msimDONE_MSG                            "msimDONE\n"
#define  msimENV_VAR                             "CKSPATH"
#define  msim_OS2_V20_FLAG                       '0'
#define  msim_AIX_V31_FLAG                       '1'
#define  msim_MAC_V71_FLAG                       '2'
#define  msimWID_NONE                            NULL
#define  msimWID_INVALID                         NULL
#define  MAX_FILENAME_LENGTH                     FILENAME_MAX

#if      defined(__OS2__) || defined (__MSDOS__)
#define  msimPATH_SEPARATOR                      '\\'
#define  msimSESSION_ID_TYPE                     ULONG
#define  msimNO_SESSION                          0LU
#define  msimOS_FLAG                             msim_OS2_V20_FLAG
#endif

#ifdef   __AIX__
#define  msimPATH_SEPARATOR                      '/'
#define  msimSESSION_ID_TYPE                     int
#define  msimNO_SESSION                          0L
#define  msimOS_FLAG                             msim_AIX_V31_FLAG
#endif

#ifdef   __MAC__
#define  msimPATH_SEPARATOR                      ':'
#define  msimSESSION_ID_TYPE                     int
#define  msimNO_SESSION                          0L
#define  msimOS_FLAG                             msim_MAC_V71_FLAG
#endif


// this conditional defines the maximum memory block size that
// can be allocated

#if defined(__OS2__)
#define msimSIZE_T_MAX    ( (unsigned int)  UINT_MAX )
#endif

#if defined(__MSDOS__)
#define msimSIZE_T_MAX    ( (unsigned int)  UINT_MAX )
#endif

#if defined(__AIX__)
#define msimSIZE_T_MAX    ( (unsigned int)  UINT_MAX )
#endif

#if defined(__MAC__)
#define msimSIZE_T_MAX    ( (unsigned int)  UINT_MAX )
#endif




/* these are for setting various options                                    */
/* timebase                                                                 */

#define msimNO_CONVERSION                        ( (USHORT) 0xFF )

#define  msimTIME_SECONDS                        0
#define  msimTIME_MINUTES                        1
#define  msimTIME_HOURS                          2

/* conc units                                                               */

#define  msimCONC_MOLES_LITER                    0
#define  msimCONC_MOLES_CM3                      1
#define  msimCONC_MOLES_CM2                      2
#define  msimCONC_MOLES_CM                       3
#define  msimCONC_MOL_LITER                      4
#define  msimCONC_MOL_CM3                        5
#define  msimCONC_MOL_CM2                        6
#define  msimCONC_MOL_CM                         7

/* energy units                                                             */

#define  msimENERGY_CALS                         0
#define  msimENERGY_JOULES                       1

/* pressure_units                                                           */

#define  msimPRESS_ATM                           0
#define  msimPRESS_TORR                          1
#define  msimPRESS_PASCALS                       2

/* volume_units                                                            */

#define  msimVOL_LITERS                          0
#define  msimVOL_CC                              1
#define  msimVOL_CM                              2

/* physical state                                                           */

#define  msimSOLID                               0
#define  msimLIQUID                              1
#define  msimGAS                                 2

/* temp progr data format                                                   */

#define  msimANALYTIC_TPROG                      0
#define  msimFILE_TPROG                          1

/* the following are shifted left to facilitate                             */
/* testing for valid combinations of temp, pressure and                     */
/* volume options. The test will involve summing the values of              */
/* the variablepres, temp_option and volume option fields of the            */
/* reaction instance. The sum will end up with its bits set as              */
/* follows :

                            +-+----vol option: 10=dont care, 01=var, 00=const
                                           | |
                            | |
                            | | +-+--temp option: 10 = progr, 01=var, 00=const
                            | | | |
                            | | | |
                            | | | | +--variable pressure : 1 = yes 0 = no
                            | | | | |
      F E D C B A 0 9 8 7 6 5 4 3 2 1                                       */
/* temperature options                                                      */

#define  msimCONST_TEMP                          (USHORT) (0 << 1)
#define  msimVAR_TEMP                            (USHORT) (1 << 1)
#define  msimPROGR_TEMP                          (USHORT) (2 << 1)

/* volume options                                                           */

#define  msimCONST_VOL                           (USHORT) (0 << 3)
#define  msimVAR_VOL                             (USHORT) (1 << 3)
#define  msimDONT_CARE_VOL                       (USHORT) (2 << 3)

/* define values for the 'simulation options' which tell the simulation     */
/* engine how temperature pressure and volume vary                          */
/* small v at end of constant signifies 'dont care' about volume            */
/*                                 vol. optn           temp. option   Var press?*/

#define  msimVAR_T_CONST_P_VAR_V                 (msimVAR_VOL + msimVAR_TEMP +\
                       FALSE)
#define  msimVAR_T_VAR_P_CONST_V                 (msimCONST_VOL + msimVAR_TEMP\
     + TRUE )
#define  msimVAR_T_CONST_P_v                     (msimDONT_CARE_VOL + \
                       msimVAR_TEMP + FALSE)
#define  msimPROG_T_CONST_P_VAR_V                (msimVAR_VOL + msimPROGR_TEMP\
     + FALSE)
#define  msimPROG_T_VAR_P_CONST_V                (msimCONST_VOL + \
                       msimPROGR_TEMP + TRUE )
#define  msimPROG_T_CONST_P_v                    (msimDONT_CARE_VOL + \
                       msimPROGR_TEMP + FALSE)
#define  msimCONST_TP_VAR_V                      (msimVAR_VOL + msimCONST_TEMP\
     + FALSE)
#define  msimCONST_T_VAR_P_CONST_V               (msimCONST_VOL + \
                       msimCONST_TEMP + TRUE )
#define  msimCONST_TP_v                          (msimDONT_CARE_VOL + \
                       msimCONST_TEMP + FALSE)

/* #defines for errors/conditions occurring during analysis of the reaction */
/* scheme, during file I/O, during memory allocation, etc.                  */

#define  msimNO_ERROR                            0
#define  msimEQN_SYNTAX                          1
#define  msimSPECIES_ERROR                       2
#define  msimSTOICH_ERROR                        3
#define  msimTOO_MANY_COMPS                      4
#define  msimAPPEARS_TWICE                       5
#define  msimNO_EQUATION                         6
#define  msimSPECIES_NAME_TOO_LONG               7
#define  msimFILENAME_ERROR                      8
#define  msimUSER_ABORT                          9
#define  msimWRITE_ERROR                         10
#define  msimREAD_ERROR                          11
#define  msimNOT_MSIM_FILE                       12
#define  msimFILE_OPEN_ERROR                     13
#define  msimMEM_ALLOC_ERROR                     14
#define  msimMEM_FREE_ERROR                      15
#define  msimMEM_OTHER_ERROR                     16
#define  msimENTRYFIELD_ERROR                    17
#define  msimINPUT_ERROR                         18
#define  msimZERO_PROBABILITY                    19
#define  msimRECORD_COUNT                        20
#define  msimDATA_FILE_ERROR                     21
#define  msimQUEUE_ERROR                         22
#define  msimSESSION_ERROR                       23
#define  msimSEG_VIOLATION                       24
#define  msimMATH_ERROR                          25
#define  msimILLEGAL_INSTR                       26
#define  msimVALUE_RANGE_ERROR                   27
#define  msimFILE_CLOSE_ERROR                    28
#define  msimPROG_T_RANGE_ERROR                  29
#define  msimSIM_EVENT_LIMIT                     30
#define  msimFINAL_TEMP_REACHED                  31
#define  msimCHILD_ABORT                         32
#define  msimELAPSED_TIME_LIMIT                  33
#define  msimMISSING_APP_FILE                    34
#define  msimFILE_ALREADY_LOADED                 35
#define  msimCOMMANDLINE_ERROR                   36
#define  msimNOT_PARENT_OF_CHILD                 37
#define  msimILLEGAL_VALUE                       43
#define  msimFILE_ACCESS_ERROR                   44
#define  msimSIM_RECORD_LIMIT                    45
#define  msimWAITING_IN_QUEUE                    46
#define  msimSIMULATION_RUNNING                  47
#define  msimSEEK_ERROR                          48
#define  msimIPC_ERROR                           49
#define  msimFOREIGN_MSIM_FILE                   50
#define  msimOTHER                               51

#define  msimT_PROFILE_FILE_OPEN_ERROR           (~(0UL))
#define  msimNO_EMPTY_INSTANCES                  ( (USHORT) 0xFF )

/* #defines for setting the simulation options                              */

#define  msimPARTICLES                           0
#define  msimEVENTS                              1
#define  msimINTERVAL                            2
#define  msimSEED                                3

/* defines for string comparison                                            */

#define  msimCASE_INSENSITIVE                    0
#define  msimCASE_SENSITIVE                      1

/* defines for user Events */

#define msimUSR_EVT_EDIT_RXN                     1
#define msimUSR_EVT_UPDATE_WIN_AND_LIST          2
#define msimUSR_EVT_UPDATE_WIN_ONLY              3

/* global typedefs                                                          */

enum msimPLOT_FILE_TYPE
{
     msimPLOTFILE_TEXT, msimPLOTFILE_PS, msimPLOTFILE_EPS, msimPLOTFILE_HPGL, msimPLOTFILE_UNKNOWN
};

#define msimNUMBER_OF_COLORSETS        7

enum msimCOLORSET_TYPE
{
     msimBLACK_BKGRD, msimWHITE_BKGRD, msimGRAY_BKGRD, msimBLUE_BKGRD, msimWHITE_ON_BLACK,
     msimBLACK_ON_WHITE, msimPAPER_SET
};

typedef unsigned char BYTE;

typedef unsigned char UCHAR;
typedef unsigned short USHORT, PTR PUSHORT;
typedef unsigned long ULONG, PTR PULONG;
typedef float FLOAT, PTR PFLOAT;
typedef double DOUBLE, PTR PDOUBLE;
typedef short PTR PSHORT;

typedef double msimFLOAT, PTR msimPFLOAT;
typedef char msimSTRING[msimSTRING_LENGTH + 1];
typedef char msimFILE_STRING[msimLENGTH_OF_FILENAME];
typedef char msimNAME_STRING[msimLENGTH_OF_NAME + 1];
typedef char msimREAL_STRING[msimLENGTH_OF_REAL_FIELD + 1];
typedef char msimINTEGER_STRING[msimLENGTH_OF_INTEGER_FIELD + 1];
typedef char msimEQUATION_STRING[msimLENGTH_OF_EQUATION + 1];
typedef USHORT msimRC;
typedef ULONG msimBOOL;
typedef Window PTR msimWID;
typedef CHAR PTR PCHAR;

typedef msimINTEGER_STRING msimOPTION_VALUE_ARRAY[msimNUMBER_OF_OPTIONS];
typedef msimREAL_STRING msimEXPONENT_ARRAY[msimMAX_NUMBER_OF_COMPONENTS];
typedef msimNAME_STRING msimNAME_ARRAY[msimMAX_NUMBER_OF_COMPONENTS];
typedef CHAR msimREAL_STRING_ARRAY[msimNUMBER_OF_THERM_COEFFS][msimLENGTH_OF_REAL_FIELD + 1];

// typedef struct APP_OPTION_STRUCT msimAPP_OPTIONS, PTR msimPAPP_OPTIONS;

struct WIN_GEOM_STRUCT
{
     SHORT x, y, height, width;
};

typedef struct WIN_GEOM_STRUCT msimWIN_GEOMETRY, PTR msimPWIN_GEOMETRY;

struct COMPONENT_STRUCT
{
     USHORT speciesindex;
     msimINTEGER_STRING stoichcoeff;
};

typedef struct COMPONENT_STRUCT msimCOMPONENT;

struct RXN_STRUCT
{
     msimEQUATION_STRING equation;
     USHORT numreactants, numproducts;
     msimCOMPONENT reactantlist[msimMAX_NUMBER_OF_COMPONENTS],
                   productlist[msimMAX_NUMBER_OF_COMPONENTS];
     struct RXN_STRUCT PTR next, PTR prev;
     msimBOOL reversible, not_stoich, singlerate;
     msimREAL_STRING fwdA, fwdM, fwdEa,/* for the rate k=A x T**M x
                                          e**(-Ea/RT)                         */
                     revA, revM, revEa;
     msimEXPONENT_ARRAY fwdexponent, revexponent;
};

typedef struct RXN_STRUCT msimRXN, PTR msimPRXN;

struct SPECIES_STRUCT
{
     msimNAME_STRING name;
     USHORT index;
     msimBOOL nonzero;
     msimREAL_STRING thermcoeff[msimNUMBER_OF_THERM_COEFFS];
     msimREAL_STRING initialconc;
     msimREAL_STRING molardensity;
     USHORT phys_state;
     struct SPECIES_STRUCT PTR next, PTR prev;
};

typedef struct SPECIES_STRUCT msimSPECIES, PTR msimPSPECIES;

struct TEMPERATURE_STRUCT
{
     msimREAL_STRING const_temp;
     msimREAL_STRING convergence_std;
     msimREAL_STRING progr_coeffA;
     msimREAL_STRING prog_initial_temp;
     msimREAL_STRING initial_temp;
     msimREAL_STRING reserved;
     msimREAL_STRING max_step_size;
     msimREAL_STRING max_temp;         /* used by temp programming            */
};

typedef struct TEMPERATURE_STRUCT msimTEMP_COEFFS, PTR msimPTEMP_COEFFS;

struct EQUIL_OPTS_STRUCT
{
     msimINTEGER_STRING cycle_length;
     msimREAL_STRING min_eff;
};

typedef struct EQUIL_OPTS_STRUCT msimEQUIL_OPTS, PTR msimPEQUIL_OPTS;


// this union definition attempts to deal with the fact that
// under OS/2 Cset/2, fpos_t is two 4-byte long ints
// under Borland C for OS/2 or Windows fpos_t is one 4-byte long int

union FILE_POS_UNION
{
     LONG array[2];
     fpos_t fp;
     LONG offset;
};

typedef union FILE_POS_UNION msimFPOS_T, PTR msimPFPOS_T;

typedef struct
{
     CHAR msimID[msimRXN_FILE_FLG_LENGTH];
     msimFPOS_T sim_data_file_offset;
     ULONG notebook_size;
     msimPRXN ptr_to_rxnlist;
     msimPSPECIES ptr_to_species_list;
     PCHAR ptr_to_notebook_text;
     USHORT listbox_selection;
     USHORT numsteps, nonzeroconcs, speciescount;
     msimBOOL variablepress, specieslist_altered;
     msimFPOS_T sim_state_data_file_offset;/* for resuming simulation         */
     msimBOOL data_altered_since_lastsave;
     msimBOOL enable_equil_detect;
     USHORT temp_prog_data_format;
     ULONG reserved4bytes;
     ULONG num_simulation_data_records;
     USHORT volume_option;
     USHORT temp_option;
     msimOPTION_VALUE_ARRAY optionvalue;
     ModelessDialog PTR p_plot_dialog;
     ModelessDialog PTR p_notebook_window;
     msimRC sim_return_code;           /* records simulation state            */
     USHORT notebook_altered;
     CHAR operating_sys_flag;
     msimFILE_STRING filename, print_target, plot_target;
     USHORT time_units, energy_units, conc_units, pressure_units,
            volume_units;
     msimTEMP_COEFFS temp_data;
     msimSESSION_ID_TYPE session_id;   /* Session ID of simulation            */
     msimEQUIL_OPTS equil_data;
     msimFILE_STRING temp_profile_data;
     msimREAL_STRING elapsed_time_limit;
     CHAR reserved15bytes[15];
     msimREAL_STRING execution_time_str;
     CHAR base_filename[msimBASE_FILENAME_LENGTH];
     CHAR reserved_for_future[msimRESERVED_LENGTH];
} msimINSTANCE, PTR msimPINSTANCE;

// define an ASSERT macro for use with Starview

#if defined(DEBUG)

void msimAssertMsg( PCHAR Filename,  int Line );  // fcn prototype - in msimutil.cxx

#define msimASSERT(statement)                     \
     if (statement)                               \
          ;                                       \
     else msimAssertMsg( __FILE__, __LINE__ )

#else

// define a NULL macro
#define msimASSERT(statement)

#endif




#endif
