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
//  PLOTATTR.HXX - a header file of the VSIM User interface
// ............................................................................
//
//  This header file declares the EditPlotAttributes dialog used in the plot routines
//  the VSIM user interface.
//
//  CREATED : 8.23.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#include "vsimtype.hxx"
#include "xyplot.hxx"

#include <limits.h>


const UINT32 MAX_PTS_DEFAULT  = 1000;
const UINT32 MIN_PTS          = 2;
const UINT32 RESERVED_RECORDS = 16;
const UINT32 MAX_PTS          = ( UINT_MAX / sizeof( FLOAT64 ) ) - RESERVED_RECORDS;



BOOL8 EditPlotAttributes( XyPlot* pPlot );
 
 