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
//  CHILDAPP.HXX - a header of the VSIM User interface 
// ............................................................................
// 
//  This header declares the ChildApplication object. 
//  this version of the file is specific to OS/2
//
//  CREATED : 3.09.97 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------






DECLARE_LIST( ParameterList, String* )





// ----------------------------------------------------------------------------
//             macro RAISE_ERROR_FLAG
// ............................................................................
// 
// define a macro to report an exception condition to user
//
//
// ----------------------------------------------------------------------------

#define RAISE_ERROR_FLAG( error_type ) ChildApplication::ReportFatalError( __FILE__, __LINE__, __TIME__, error_type )

