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
// VSIM - a Visual Kinetics Simulator
// ............................................................................
// 
//  MODULE  : vsimtype.hxx
//
//             This header file contains type definitions for 
//             use throughout the vsim program files
//
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)
// 
//  CHANGE HISTORY :
//        created Nov. 8 1994
//
// ----------------------------------------------------------------------------


#if !defined(VSIMTYPE_HXX)
#define  VSIMTYPE_HXX        

// defines 

#define PTR              *

#if !defined(FALSE)
#define FALSE            0
#endif

#if !defined(TRUE)
#define TRUE             1
#endif


// numeric types 

typedef unsigned char UCHAR8, PTR PUCHAR8;
typedef signed char CHAR8, PTR PCHAR8;
typedef unsigned short UINT16, PTR PUINT16;
typedef unsigned long UINT32, PTR PUINT32;
typedef short INT16, PTR PINT16;
typedef long  INT32, PTR PINT32;
typedef float FLOAT32, PTR PFLOAT32;
typedef double FLOAT64, PTR PFLOAT64;
typedef unsigned char BOOL8; 
typedef unsigned long LargeUnsignedInt;
typedef long LargeSignedInt;
typedef unsigned long TP_OPTS;  // for temperature /pressure option flags 


enum RATE_CONST_FORMAT { TEMP_INDEPENDENT, TEMP_DEPENDENT };



enum SIM_STATUS_CODE {  SIM_NEW,
                        SIM_DATA_MODIFIED, 
                        SIM_DATA_UNMODIFIED,
                        SIM_READY_TO_RUN, 
                        SIM_WAITING_IN_QUEUE,
                        SIM_RUNNING, 
                        SIM_TERMINATE_ZERO_PROBABILITY, 
                        SIM_TERMINATE_EVENT_LIMIT,
                        SIM_TERMINATE_RECORD_LIMIT,
                        SIM_TERMINATE_ELAPSED_TIME_LIMIT,
                        SIM_TERMINATE_FINAL_TEMP_REACHED,
                        SIM_TERMINATE_USER_ABORT,
                        SIM_TERMINATE_CHILD_ABORT,
                        SIM_TERMINATE_FILE_ERROR,
                        SIM_TERMINATE_SEG_VIOLATION,
                        SIM_TERMINATE_MATH_ERROR,
                        SIM_TERMINATE_ILLEGAL_INSTR,
                        SIM_TERMINATE_INPUT_ERROR,
                        SIM_TERMINATE_MEM_ALLOC_ERROR
                     };


enum ELAPSED_TIME_UNITS { TIME_UNITS_SEC, TIME_UNITS_MINUTE, TIME_UNITS_HOUR, 
                          TIME_UNITS_DAY };

enum TEMPERATURE_OPTION { CONSTANT_TEMP, VARIABLE_TEMP, ANALYTIC_PROGR_TEMP, 
                          EXT_PROFILE_PROGR_TEMP };

enum VOLUME_OPTION { CONSTANT_VOLUME, VARIABLE_VOLUME, NOT_TRACKED_VOLUME };


enum PRESSURE_OPTION { CONSTANT_PRESSURE,  VARIABLE_PRESSURE };

#endif
