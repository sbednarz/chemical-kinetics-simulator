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
//  GLOBCONC.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the ChangeGlobalConcentrationDialog class
//  the VSIM user interface. 
//
//  CREATED : 9.09.98
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if !defined(__GLOBCONC_HXX__)
#define __GLOBCONC_HXX__

#include "rxnschem.hxx"

enum  CGC_RC { CGC_RC_FAIL, CGC_RC_CANCEL, CGC_RC_ACCEPT };

enum CGC_RC ChangeGlobalConcentrationDialog( ReactionScheme* pScheme, Window* pParent );

#endif


