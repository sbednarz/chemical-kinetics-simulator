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
//  EDITXFER.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the EditTransferObjectDialog class
//  the VSIM user interface. 
//
//  CREATED : 4.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if !defined(__EDITXFER_HXX__)
#define __EDITXFER_HXX__

#include "xferobj.hxx"

enum EDIT_XFER_RC  { EX_RC_FAIL, EX_RC_CANCEL, EX_RC_CLOSE };

enum EDIT_XFER_RC EditTransferStepsDialog( TransferObject* pObject, Window* pParent);


#endif

