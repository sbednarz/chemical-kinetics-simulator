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
//  PLOTOBJ.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the PlotObject class for
//  the VSIM user interface. This is the base class from which
//  Axis. PlotData and other classes are derived.
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED : 8.23.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "plotobj.hxx"

#endif





// ----------------------------------------------------------------------------
//                 PlotObject::HighlightArea()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void PlotObject::HighlightArea()
{
     enum RasterOp old_op = GetpParent()->ChangeRasterOp( ROP_INVERT );

     Brush brush = GetpParent()->GetFillInBrush( );

     enum BrushStyle old_style = brush.ChangeStyle( BRUSH_UPDIAG );
     brush.ChangeTransparent( TRUE );

     GetpParent()->ChangeFillInBrush( brush );

     GetpParent()->DrawRect( active_area );

     GetpParent()->ChangeRasterOp( old_op );
     brush.ChangeStyle( old_style );
}





// ----------------------------------------------------------------------------
//                 PlotObject::HighlightObject()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void PlotObject::HighlightObject( BOOL8 State )
{
     if ( highlight )
          HighlightArea(); // undoes highlight to put bakc to std state

     highlight = State;
     DrawObject();
}


