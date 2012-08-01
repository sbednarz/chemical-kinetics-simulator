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
//  LINETYPE.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines data on linetypes for displaying simulation
//  results for the VSIM user interface.
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "brushes.hxx"

#endif






// ----------------------------------------------------------------------------
//             GetBrushStyle()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

enum BrushStyle GetBrushStyle( UINT16 I )  
{ 
     switch ( I % NUM_BRUSH_STYLES) 
     {

     case 0 :
     default :
        return BRUSH_SOLID;

     case 1 :
        return BRUSH_CROSS;

     case 2 :
        return BRUSH_UPDIAG;

     case 3 :
        return BRUSH_25;

     case 4 :
        return BRUSH_HORZ;

     case 5 :
        return BRUSH_DIAGCROSS;

     case 6 :
        return BRUSH_VERT;
     case 7 :
        return BRUSH_DOWNDIAG;
     } 

}


