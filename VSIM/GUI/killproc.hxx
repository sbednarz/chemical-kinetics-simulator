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
//  KILLPROC.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the ChildProcess object. 
//  this version of the file is specific to OS/2
//
//  CREATED : 1.29.96 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#include "vsimtype.hxx"


BOOL8 KillProcess( UINT32 SessionID );
BOOL8 DoesProcessExist( UINT32 SessionID );

void  Sleep( UINT32 MSec );

