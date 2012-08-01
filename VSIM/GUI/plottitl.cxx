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
//  PLOTTITL.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the PlotTitle class for use in displaying simulation
//  results in VSIM
//
//  CREATED : 7.14.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "plottitl.hxx"
#include "edittitl.hxx"

#endif




// ----------------------------------------------------------------------------
//                           PlotTitle::PlotTitle( )
// ............................................................................
// 
//   contructor 
//
// ----------------------------------------------------------------------------

PlotTitle::PlotTitle( OutputDevice* pParent ) : 
PlotObject( pParent), 
title(), 
location()
{
}



BOOL8 PlotTitle::EditObject( const Point )
{
     return ( EditTitle( this ) );
}




// ----------------------------------------------------------------------------
//                        XAxis::ResizeObject( )
// ............................................................................
// 
//  this function is called to resize the object. We recalculate all positions
//  in device coordinates
//
// ----------------------------------------------------------------------------

void PlotTitle::ResizeObject( )
{
     location = active_area.TopCenter();
}


void PlotTitle::DrawObject()
{
     Size     text_size = GetpParent()->GetTextSize( title );

     GetpParent()->DrawText( location + Point(text_size.Width()/-2, text_size.Height() ), 
      title );

     if ( highlight )
          HighlightArea();
      
}

