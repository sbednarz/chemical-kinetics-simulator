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
//  HEADER  : assert.hxx                                                                   
//                                                                                          
//             This header file contains the declarations for the ASSERT                
//             macro used for debugging                                                   
//                                                                                          
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)                                   
//                                                                                          
//  CHANGE HISTORY :                                                                        
//        created Nov. 8 1994                                                               
//                                                                                          
// ----------------------------------------------------------------------------


#if !defined(ASSERT_HXX)
#define ASSERT_HXX


// define an ASSERT macro for use with Starview

#if defined(__DEBUG__)

void _AssertMsgBox( const char * Filename,  int Line );  // fcn prototype - in msimutil.cxx

#define ASSERT(statement)                     \
     if (statement)                               \
          ;                                       \
     else _AssertMsgBox( __FILE__, __LINE__ )

#else

// define a NULL macro
#define ASSERT(statement)

#endif



#endif


