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
//  ABARDISP.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the AnimatedBarDisplay class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing qunantitative data in a bar graph. The bar 
//  is animated
//
//  CREATED :  1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__ABARDISP_HXX__)
#define __ABARDISP_HXX__


#include "bardsply.hxx"
#include "flotarry.hxx"

class AnimatedBarDisplay : public BarDisplay
{

protected: 
     const FloatArray&   array;
     UINT32              index;
     UINT32              max_index;

public:
					AnimatedBarDisplay( Window* pParent, const String& rName, 
                          const FloatArray& rArray, WinBits WinStyle = BAR_VERTICAL  );

     BOOL8               Next();
     BOOL8               Previous();
     void                First()   { index = 0;         SetCurrentBarValue( array[index] ); }
     void                Last()    { index = max_index; SetCurrentBarValue( array[index] ); } 
};


#endif

