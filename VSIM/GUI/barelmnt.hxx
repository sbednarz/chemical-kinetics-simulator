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
//  BARELMNT.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the BarGraphElement class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing qunantitative data in a bar graph 
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED :  1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__BARELMNT_HXX__)
#define __BARELMNT_HXX__

#include "plotobj.hxx"





class BarGraphElement : public PlotObject
{
protected :
     Bar                 bar;
     FLOAT64             bar_width_as_fraction;
     String              name;

public:

					BarGraphElement( OutputDevice* pParent, WinBits WinStyle, const String& rName );

     virtual void        DrawObject( );
     virtual void        ResizeObject( );
     virtual BOOL8       EditObject( const Point Position );



     const String&       GetName() const { return name;}
     void                SetName( const String& rNewName ) { name = rNewName;}

     virtual PLOT_OBJECT_TYPE    Type() const { return BAR_ELEMENT_OBJECT; }
};


#endif
