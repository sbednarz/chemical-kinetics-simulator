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
//  HEADER  : constant.hxx
//
//             This header file contains declarations and definitions of constants
//             for use throughout the vsim program files
//
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)
//
//  CHANGE HISTORY :
//        created Nov. 18 1994
//
// ----------------------------------------------------------------------------


#if !defined(VSIMDEFS_HXX)
#define VSIMDEFS_HXX

#include "vsimtype.hxx"


const UINT16  MAX_NUMBER_OF_COMPONENTS = 4;
const UINT16 MAX_LENGTH_OF_NAME = 8;

// these are 'magic numbers' - file offset for a specific piece of data that
// both the user interface and the simulation engine will access

// position where we can read the scheme type 
const INT32  SIMSCHEME_TYPE_POSITION            = 16;

// position where we can write the scheme window gemotry information
const INT32  SIMFRAME_WIN_GEOMETRY_POSITION     = 18;

// position where we can read/write the number of data points
const INT32  SIMNUM_POINTS_FILE_POSITION        = 34;

// position where we can read/write the value of the file offset
// where simulation results start
const INT32  SIMDATA_FILE_OFFSET_POSITION       = 38;

// position where we can read/write the value of the file offset
// where the simulation state is saved when a simulation is interrupted
const INT32  SIMSTATE_DATA_FILE_OFFSET_POSITION = 42;

// offset where we can read/write the simulation status code
const INT32  SIMSTATUS_CODE_FILE_POSITION       = 46;

// offset where we can read/write the overall execution time since scheme was last modified
const INT32 SIMCUMULATIVE_SIMULATION_TIME_POSITION = 48;

// offset where we can read/write the execution time of the current simulation step
const INT32 SIMSTEP_SIMULATION_TIME_POSITION    = 52;

// offset where we can read/write the timestamp of the most recent simulation
const INT32 SIMLAST_SIMULATION_TIMESTAMP_POSITION  = 56;


// offset where we can read/write the timestamp of notebook modifications
const INT32 SIMLAST_NOTEBOOK_TIMESTAMP_POSITION    = 64;

// offset where we can read/write the notebook contents 
const INT32 SIM_NOTEBOOK_TEXT_POSITION     = 72;

#endif

