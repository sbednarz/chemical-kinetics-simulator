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
//  a bar representing qunantitative data in a bar graph 
//
//  CREATED :  1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "abardisp.hxx"

#endif






// ----------------------------------------------------------------------------
//                 AnimatedBarDisplay::AnimatedBarDisplay()
// ............................................................................
//
//
//   constructor
//
// ----------------------------------------------------------------------------

AnimatedBarDisplay::AnimatedBarDisplay( Window* pParent, const String& rName, 
                                        const FloatArray& rArray, WinBits WinStyle ) :
BarDisplay( pParent, WinStyle, rName ),
array( rArray ),
index( 0 ),
max_index( array.GetArraySize()-1 )
{
	SetMaxBarValue( array.GetMaximumValue() );
	First();
}







// ----------------------------------------------------------------------------
//                 AnimatedBarDisplay::Next()
// ............................................................................
//
//   updates bargraph element to display next value in associated array 
//   returns TRUE if successfully incremented, FALSE if at end of array
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarDisplay::Next()
{
     if (index < max_index )
     {
          SetCurrentBarValue( array[++index] );
          return TRUE;
     }
     else
         return FALSE;
}









// ----------------------------------------------------------------------------
//                 AnimatedBarDisplay::Previous()
// ............................................................................
//
//   updates bargraph element to display previous value in associated array 
//   returns TRUE if successfully decremented, FALSE if at beginning of array
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarDisplay::Previous()
{
     if (index > 0)
     {
          SetCurrentBarValue( array[--index] );
          return TRUE;
     }
     else
         return FALSE;
}



