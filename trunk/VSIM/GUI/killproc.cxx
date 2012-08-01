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
//  KILLPROC.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChildProcess object. 
//  this version of the file is specific to OS/2
//
//  CREATED : 5.05.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#define INCL_DOSSESMGR
#define INCL_DOSMEMMGR
#define INCL_DOSPROCESS
#define INCL_DOSQUEUES

#include <os2.h>


#include "vsimtype.hxx"


BOOL8 KillProcess( UINT32 SessionID )
{
    return ( 0 == DosStopSession( STOP_SESSION_SPECIFIED, SessionID ) );
}


BOOL8 DoesProcessExist( UINT32 SessionID )
{
     STATUSDATA status_data;

     status_data.Length    = 6;
     status_data.SelectInd = SET_SESSION_UNCHANGED;
     status_data.BondInd   = SET_SESSION_UNCHANGED;;

    return ( 0 == DosSetSession( SessionID, &status_data ) );
}


void  Sleep( UINT32 mSec ) { DosSleep( mSec ); }
