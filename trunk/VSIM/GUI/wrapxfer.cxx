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
//  WRAPXFERCXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the WrappedTransferObject class for
//  the VSIM user interface. 
//
//  CREATED : 11.10.98
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "wrapxfer.hxx"
#include "comptobj.hxx"
#include "vsimtype.hxx"
#include "schemwin.hxx"
#include "idtypes.hxx"
#include "mainapp.hxx"
#include "xobjdlg.hxx"

#endif


// ----------------------------------------------------------------------------
//                 WrappedTransferObject::WrappedTransferObject( )
// ............................................................................
//
//  constructor for WrappedTransferObject
//
// ----------------------------------------------------------------------------

WrappedTransferObject::WrappedTransferObject( const String& rName, SchemeWindow* pParent ) :
TransferObject( rName, pParent ),
phantom_compt_rect(),
wrap_direction( UNSPECIFIED_WRAP )
{
} 


// ----------------------------------------------------------------------------
//                    WrappedTransferObject::~WrappedTransferObject( )
// ............................................................................
// 
//  destructor for WrappedTransferObject. 
//  A virtual function 
//
// ----------------------------------------------------------------------------

WrappedTransferObject::~WrappedTransferObject( )
{
}


// ----------------------------------------------------------------------------
//                    WrappedTransferObject::DrawObject( )
// ............................................................................
//
//  standard draw procedure
//
// ----------------------------------------------------------------------------

void WrappedTransferObject::DrawObject( )
{
     arrow.DrawObject();
     DrawActiveArea( );
     DrawPhantomCompartment();
}





// ----------------------------------------------------------------------------
//                    WrappedTransferObject::DrawPhantomCompartment()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WrappedTransferObject::DrawPhantomCompartment() 
{
     //  draw the phantom compartment as transparent with a dashed line

     Pen old_pen;
     Brush old_brush;

     Pen pen = GetpOutputDevice()->GetPen();
     pen.ChangeStyle( PEN_DASH );

     old_pen = GetpOutputDevice()->ChangePen( pen );
     old_brush =  GetpOutputDevice()->ChangeFillInBrush( Brush( BRUSH_NULL) );

     GetpOutputDevice()->DrawRect( phantom_compt_rect );

     GetpOutputDevice()->ChangePen( old_pen );
     GetpOutputDevice()->ChangeFillInBrush( old_brush );
}









// ----------------------------------------------------------------------------
//               WrappedTransferObject::DetermineWrapDirection()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
enum WRAP_DIRECTION WrappedTransferObject::DetermineWrapDirection()
{
     Rectangle source_rect( GetpSourceCompartment()->GetActiveArea() );
     Rectangle target_rect( GetpTargetCompartment()->GetActiveArea() );


     // if both top and left coords match then target must be behind source
     if ( source_rect.Top() == target_rect.Top()  &&
          source_rect.Left() == target_rect.Left() 
        )

     {
          return DEPTH_WRAP;
     }

     // if only top matches then target must be to left of source
     if ( source_rect.Top() == target_rect.Top() )
     {
          return HORIZONTAL_WRAP;
     }

     // if only left matches then target must be above source
     if ( source_rect.Left() == target_rect.Left() )
     {
          return VERTICAL_WRAP;
     }

     return UNSPECIFIED_WRAP;
}







// ----------------------------------------------------------------------------
//          WrappedTransferObject::PositionPhantomCompartment()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
void WrappedTransferObject::PositionPhantomCompartment()
{
     Rectangle source_rect( GetpSourceCompartment()->GetActiveArea() );

     // start with them equal and reposition depending on 
     // wrap_direction 

     phantom_compt_rect = source_rect;

     Rectangle xfer_rect( GetActiveArea() );

     const INT16  x_increment = phantom_compt_rect.GetWidth() + xfer_rect.GetWidth() + ( 2 * GetpParent()->GetDefaultCharWidth() );
     const INT16  y_increment = phantom_compt_rect.GetHeight() + xfer_rect.GetHeight() + ( 2 * GetpParent()->GetDefaultCharHeight() ); 

     switch ( wrap_direction )
     {
          case UNSPECIFIED_WRAP :
          default :
               break;


          case HORIZONTAL_WRAP :

               phantom_compt_rect.ChangePos( phantom_compt_rect.TopLeft() + Point( x_increment, 0 ) );
               break;


          case VERTICAL_WRAP : 

               phantom_compt_rect.ChangePos( phantom_compt_rect.TopLeft() + Point( 0, y_increment ) );
               break;
     }
}




// ----------------------------------------------------------------------------
//                    WrappedTransferObject::Reposition( )
// ............................................................................
// 
//  this function uses the knowledge of the positioning of the 
//  source and target conpartments to determined whethe the wrapped transfer
//  object is wrapped in the vertical or horinzontal direction.
//  we then recalculate the geometry of everything based on this 
//  We also recalculate the position of the 
//  active area of this object, i.e., the box where the title is displayed
//  that box is supposed to be centered on the line connecting soutce and 
//  target compartments. The default StarView coordinate system 
//  orientation in the design ( upper left = (0,0) )
//
// ----------------------------------------------------------------------------

void WrappedTransferObject::Reposition( )
{
     if ( UNSPECIFIED_WRAP == wrap_direction)
     {
          wrap_direction = DetermineWrapDirection();
     }

     if ( UNSPECIFIED_WRAP == wrap_direction)
     {
          ASSERT( 0 );   // logic error is still no direction at this point - abort 
     }

     PositionPhantomCompartment();

     // reposition the arrow so that it points from source compartment to
     // target compartment   

     // in the case of wrapped compartments, the target will always 
     // be the phantom compartment

     arrow.Reposition( GetpSourceCompartment()->GetActiveArea( ),
                        phantom_compt_rect );

     // now move the active area so that its center is over the center of 
     // the newly calculated line

     short x = ( arrow.SourcePoint().X() + arrow.TargetPoint().X())/2;
     short y = (arrow.SourcePoint().Y() + arrow.TargetPoint().Y())/2;


     Point new_origin( x - active_area.GetWidth()/2, y - active_area.GetHeight() /2 );

     active_area.ChangePos( new_origin );
}


