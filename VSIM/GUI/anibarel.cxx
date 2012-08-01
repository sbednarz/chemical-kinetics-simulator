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
//  ANIBAREL.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the AnimatedBarGraphElement class for
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


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "anibarel.hxx"

#endif






// ----------------------------------------------------------------------------
//                 AnimatedBarGraphElement::AnimatedBarGraphElement()
// ............................................................................
//
//
//   constructor
//
// ----------------------------------------------------------------------------

AnimatedBarGraphElement::AnimatedBarGraphElement( OutputDevice* pParent, const String& rName, 
                                                  const FloatArray& rArray ) :
BarGraphElement( pParent, rName ),
array( rArray ),
index( 0 ),
max_index( array.GetArraySize()-1 )
{
	SetMaxBarLength( array.GetMaximumValue() );
	First();
}







// ----------------------------------------------------------------------------
//                 AnimatedBarGraphElement::Next()
// ............................................................................
//
//   updates bargraph element to display next value in associated array 
//   returns TRUE if successfully incremented, FALSE if at end of array
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarGraphElement::Next()
{
     if (index < max_index )
     {
          SetCurrentBarLength( array[++index] );
          return TRUE;
     }
     else
         return FALSE;
}









// ----------------------------------------------------------------------------
//                 AnimatedBarGraphElement::Previous()
// ............................................................................
//
//   updates bargraph element to display previous value in associated array 
//   returns TRUE if successfully decremented, FALSE if at beginning of array
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarGraphElement::Previous()
{
     if (index > 0)
     {
          SetCurrentBarLength( array[--index] );
          return TRUE;
     }
     else
         return FALSE;
}



