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
//  SIMOPTS.HXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SimulationOptionDlg class
//
//  CREATED : 5.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__SIMOPTS_HXX__)
#define __SIMOPTS_HXX__

class SchemeFrameWindow;

enum SIMOPT_RC { SO_RC_FAIL, SO_RC_ACCEPT, SO_RC_CANCEL };

SIMOPT_RC EditSimulationOptions( SchemeFrameWindow* pParent );

#endif

