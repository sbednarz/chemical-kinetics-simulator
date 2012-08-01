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
//  EDITSDIF.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the EditStandardDiffusionStepDialog class
//  the VSIM user interface. 
//
//  CREATED : 8.13.98
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if !defined(__EDITSDIF_HXX__)
#define __EDITSDIF_HXX__

#include "xferobj.hxx"

enum EDIT_SDIF_RC  { ESD_RC_FAIL, ESD_RC_CANCEL, ESD_RC_CLOSE };

enum EDIT_SDIF_RC EditStandardDiffusionStep( TransferObject* pParentObject, TransferStep* pStep);


#endif

