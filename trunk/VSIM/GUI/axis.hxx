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
//  AXIS.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Axis class for
//  the VSIM user interface. This is the base class from which 
//  XY Axis and BarGraphAxis classes are derived. 
//  It includes info about drawing the object on the screen 
//
//  CREATED : 6.28.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__AXIS_HXX__)
     #define __AXIS_HXX__

#include "vsimtype.hxx"
#include "plotobj.hxx"

class Axis : public PlotObject 
{
protected :
     Point         start_pt;
     Point         end_pt;
     Point         title_position;

     String*       ptic_label_array;
     Point*        ptic_position_array;
     String        axis_title;

     UINT16        num_tic_marks;
     INT16         tic_length_device_coords;

     virtual void   DrawTicMark( UINT16 I ) const = 0;
     virtual void   DrawTitle() const = 0;

public :
                    Axis( OutputDevice* pParent );

     virtual       ~Axis();

     virtual void   DrawObject( );

     void           SetTitle( const String& rTitle ) { axis_title = rTitle; }
     const  String& GetTitle( ) const { return axis_title; }

     void           SetTicLabel( const UINT16 I, const String& rLabel );
     void           SetTicPosition( const UINT16 I, const Point& rPosition );
     BOOL8          SetNumTicMarks( const UINT16 I );

     void           SetTicLength( const INT16 NewLength) { tic_length_device_coords = NewLength; }

};


#endif
