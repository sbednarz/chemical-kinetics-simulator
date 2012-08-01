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
//  AXIS.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module  defines the Axis class for
//  the VSIM user interface. This is the base class from which 
//  XY Axis and BarGraphAxis classes are derived. 
//  It includes info about drawing the object on the screen 
//
//  CREATED : 6.28.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "axis.hxx"
#include "assert.hxx"

#endif



// ----------------------------------------------------------------------------
//                     Axis::Axis( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

Axis::Axis( OutputDevice* pParent ) : 
PlotObject( pParent ),
start_pt( Point(0,0) ),
end_pt(Point(0,0)),
title_position(Point(0,0)),
ptic_label_array( NULL ),
ptic_position_array( NULL ),
axis_title(),
num_tic_marks(0),
tic_length_device_coords(0)
{
}


// ----------------------------------------------------------------------------
//                     Axis::~Axis( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

Axis::~Axis()
{
     if ( ptic_label_array )
          delete[]  ptic_label_array;

     if ( ptic_position_array )
          delete[]  ptic_position_array;
}




// ----------------------------------------------------------------------------
//                     Axis::DrawObject( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Axis::DrawObject( )
{
     GetpParent()->DrawLine( start_pt, end_pt );
     DrawTitle();

     for ( UINT16 i = 0; i < num_tic_marks; i++ )
          DrawTicMark( i );

     if ( highlight )
          HighlightArea();

}



// ----------------------------------------------------------------------------
//                     Axis::SetTicLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Axis::SetTicLabel( const UINT16 I, const String& rLabel )
{
     ASSERT( ptic_label_array && num_tic_marks );

     ptic_label_array[I] = rLabel;
}




// ----------------------------------------------------------------------------
//                     Axis::SetTicLabel( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Axis::SetTicPosition( const UINT16 I, const Point& rPosition )
{
     ASSERT( ptic_position_array && num_tic_marks );

     ptic_position_array[I] = rPosition;
}



// ----------------------------------------------------------------------------
//                     Axis::SetNumTicMarks( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 Axis::SetNumTicMarks( const UINT16 I )
{                           
     coordinates_invalid = TRUE;

     // delete existing arrays of Strings and Points if they exist

     if ( NULL != ptic_label_array )
          delete[] ptic_label_array;

     if ( NULL != ptic_position_array )
          delete[] ptic_position_array;

     // create new array

     ptic_label_array = new String[I];
     ptic_position_array = new Point[I];

     if ( ( NULL != ptic_label_array ) && 
          ( NULL != ptic_position_array ) 
     )
     {
          num_tic_marks = I;
          return TRUE;
     }
     else
     {
          num_tic_marks = 0;
          return FALSE;
     }
}

