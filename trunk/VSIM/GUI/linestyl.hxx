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
//  LINETYPE.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This files declares data on linetypes for displaying simulation
//  results for the VSIM user interface.
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__LINESTYL_HXX__)
     #define __LINESTYL_HXX__

#include <sv.hxx>

const UINT16 NUM_LINE_STYLES = 4;

const enum PenStyle  DEFAULT_LINE_STYLE   = PEN_SOLID;

enum PenStyle GetLineStyle( UINT16 I );

#endif

