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
//  HEADER  : constants.hxx
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

const INT32  SIMDATA_FILE_OFFSET_POSITION = 20;
const INT32  SIMSTATUS_CODE_FILE_OFFSET_POSITION = 32;
 

#endif

