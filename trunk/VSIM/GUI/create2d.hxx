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
//  CREATE2D.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Create2DComptScheme function 
//  used to create a new reaction scheme containing a two-dimensional
//  array of compartments 
//
//  CREATED : 6.11.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__CREATE2D_HXX__)
     #define __CREATE2D_HXX__

class Window;
class SchemeFrameWindow;

enum Create2D_RC { C2D_RC_FAIL, C2D_RC_CANCEL, C2D_RC_ACCEPT };

enum Create2D_RC Create2DComptScheme( Window* pParent, SchemeFrameWindow* pSchemeFrameWin );


#endif
