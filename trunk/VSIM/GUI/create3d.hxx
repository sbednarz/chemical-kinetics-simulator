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
//  CREATE3D.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Create3DComptScheme function 
//  used to create a new reaction scheme containing a three-dimensional
//  array of compartments 
//
//  CREATED : 6.11.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__CREATE3D_HXX__)
     #define __CREATE3D_HXX__

class Window;
class SchemeFrameWindow;

enum Create3D_RC { C3D_RC_FAIL, C3D_RC_CANCEL, C3D_RC_ACCEPT };

enum Create3D_RC Create3DComptScheme( Window* pParent, SchemeFrameWindow* pSchemeFrameWin );


#endif
