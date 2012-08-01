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
//  ANIBAREL.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the AnimatedBarGraphElement class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing quantitative data in a bar graph 
//  It includes info about drawing the object on the screen or other
//  output device. It also is associated with an array of floating 
//  points numbers and incorporates a mechanism for moving backward 
//  and forward incrementally through the array while displaying 
//  the value graphically 
//
//  CREATED :  1.31.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__ANIBAREL_HXX__)
#define __ANIBAREL_HXX__


#include "barelmnt.hxx"
#include "flotarry.hxx"

class AnimatedBarGraphElement : public BarGraphElement
{

protected: 
     const FloatArray&   array;
     UINT32              index;
     UINT32              max_index;

public:
                         AnimatedBarGraphElement( OutputDevice* pParent, const String& rName, 
                                                  const FloatArray& rArray );

     BOOL8               Next();
     BOOL8               Previous();
     void                First()   { SetCurrentBarLength( array[0] ); }
     void                Last()    { SetCurrentBarLength( array[max_index] ); } 


     virtual PLOT_OBJECT_TYPE    Type() const { return ANIMATED_BAR_ELEMENT_OBJECT; }
};


#endif

