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
//  MARKERS.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This module defines data on markers for displaying simulation
//  results for the VSIM user interface.
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__MARKERS_HXX__)
#define __MARKERS_HXX__

const UINT16 NUM_MARKER_STYLES = 10;

enum MARKER_STYLE
{
     MARKER_CROSS, MARKER_PLUS, MARKER_DIAMOND, MARKER_SQUARE,
     MARKER_STAR6, MARKER_CIRCLE, MARKER_SOLIDDIAMOND, MARKER_SOLIDSQUARE,
     MARKER_SOLIDCIRCLE, MARKER_STAR8
};


const enum MARKER_STYLE DEFAULT_MARKER_STYLE = MARKER_CROSS;


inline MARKER_STYLE GetMarkerStyle( UINT16 I )  { return (enum MARKER_STYLE) (I % NUM_MARKER_STYLES); }

#endif
 
 