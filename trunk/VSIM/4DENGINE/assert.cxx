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
//  MODULE  : assert.cxx                                                                   
//                                                                                          
//             This header file contains the definitions needed for operation of
//             the ASSERT macro used for debugging                                                   
//                                                                                          
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)                                   
//                                                                                          
//  CHANGE HISTORY :                                                                        
//        created Nov. 15 1994                                                               
//                                                                                          
// ----------------------------------------------------------------------------

#include "vsimtype.hxx"
#include "assert.hxx"



#if defined(__UNIT_TEST__)

#include <stdio.h>
#include <stdlib.h>

void _AssertMsgBox( const char * Filename,  int Line )
{
     fflush( stdout);
     fprintf( stderr, "\nAssertion failed: %s, line %d \n", Filename, Line );
     fflush( stderr );
     abort( );
}

#else

#include <sv.hxx>

void _AssertMsgBox( const char * Filename,  int Line )
{
     GetpApp( )->Abort( String(Filename) + String(":") + String(Line)); 
}

#endif

