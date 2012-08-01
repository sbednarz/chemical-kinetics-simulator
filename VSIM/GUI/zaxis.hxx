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
//  ZAXIS.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the ZAxis class for
//  the VSIM user interface. ZAxis is used to display a line and the names
//  of individual layers in a StackedXyPlot object
//
//  CREATED : 7.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__ZAXIS_HXX__)
     #define __ZAXIS_HXX__

#include "axis.hxx"

class ZAxis : public Axis
{
protected :
     INT16          ytic_length_device_coords;
     BOOL8          draw_boundaries;
     Polygon        bottom_face;
     Polygon        left_face;

     virtual void   DrawTicMark( UINT16 I ) const;
     virtual void   DrawTitle() const;

public :
                    ZAxis( OutputDevice* pParent );

     virtual       ~ZAxis();

     virtual void   ResizeObject( );
     virtual void   DrawObject( );
     virtual BOOL8  EditObject( const Point Position );

     virtual PLOT_OBJECT_TYPE    Type() const { return Z_AXIS_OBJECT; }

     void          SetXTicLength( const INT16 NewLength ) { tic_length_device_coords = NewLength; }
     void          SetYTicLength( const INT16 NewLength ) { ytic_length_device_coords = NewLength; }

     void          DrawBoundaries( BOOL8 State ) { draw_boundaries = State; }
     BOOL8&        DrawBoundaries( ) { return draw_boundaries; }
};


#endif

