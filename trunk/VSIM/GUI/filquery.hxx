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
//  FILQUERY.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file declares the InvokeFillQueryDialog function
//
//  CREATED : 10.23.96 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__FILQUERY_HXX__)
     #define __FILQUERY_HXX__

class SchemeFrameWindow;
class CompartmentObject;

enum FILL_OPERATION { FO_FAIL, FO_CANCEL, FO_USE_CLIP_CONTENTS, FO_USE_EXISTING_CONTENTS };

enum FILL_OPERATION InvokeFillQueryDialog( SchemeFrameWindow* pParent, 
                                           const CompartmentObject* const pClipObject );

#endif
