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
//  RXNCONDN.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the ReactionCondition Dialog invokation function
//
//  CREATED : 5.11.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__RXNCONDN_HXX__)
#define __RXNCONDN_HXX__

class CompartmentDialog;
class ReactionScheme;
class Window;


enum RxnCondnRC { RC_RC_FAIL, RC_RC_CANCEL, RC_RC_ACCEPT };

RxnCondnRC EditReactionConditions( CompartmentDialog* pParent );

RxnCondnRC EditReactionConditions3D( CompartmentDialog* pParent );


RxnCondnRC EditGlobalSchemeTemperature( ReactionScheme* pScheme, Window* pParent );

#endif


