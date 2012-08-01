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
//  ZAXIS.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This header file defines the ZAxis class for
//  the VSIM user interface. ZAxis is used to display a line and the names
//  of individual layers in a StackedXyPlot object
//
//  CREATED : 7.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "zaxis.hxx"
#include "math.hxx"

#endif



// ----------------------------------------------------------------------------
//                             ZAxis::ZAxis( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

ZAxis::ZAxis( OutputDevice* pParent ) :
Axis( pParent ),
ytic_length_device_coords( 0 ),
draw_boundaries( FALSE ),
bottom_face( 4 ),
left_face( 4 )
{
}





// ----------------------------------------------------------------------------
//                             ZAxis::~ZAxis( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

ZAxis::~ZAxis()
{
}




// ----------------------------------------------------------------------------
//                             ZAxis::DrawTitle()
// ............................................................................
// 
//   does nothing
//
// ----------------------------------------------------------------------------
void ZAxis::DrawTitle() const
{
}





BOOL8 ZAxis::EditObject( const Point Position )
{
     return TRUE;
}





// ----------------------------------------------------------------------------
//                             ZAxis::DrawTicMark()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ZAxis::DrawTicMark( UINT16 I ) const
{
     // change position for label 

//     Size text_size = GetpParent()->GetTextSize( ptic_label_array[I] );
     Size char_size = GetpParent()->GetFont().GetSize();

     // draw tic marks in x-axis and y-axis directions

     Point position = ptic_position_array[I];

     GetpParent()->DrawLine( position, position + Point( tic_length_device_coords, 0 ) );
     GetpParent()->DrawLine( position, position - Point( 0, ytic_length_device_coords ) );

     GetpParent()->DrawText( position + Point( tic_length_device_coords + char_size.Width(), -char_size.Height()/2 ), 
                            ptic_label_array[I] );
}






// ----------------------------------------------------------------------------
//                             ZAxis::DrawObject()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ZAxis::DrawObject( )
{
     const enum BrushStyle SIDE_FILL_STYLE = BRUSH_25;
     const enum PenStyle FRAME_STYLE = PEN_NULL;

     Pen pen = GetpParent()->GetPen();
     Pen old_pen = pen;

     if ( draw_boundaries )
     {
          pen.ChangeStyle( FRAME_STYLE  );
     
          GetpParent()->ChangePen( pen );
     
          Brush   brush = GetpParent()->GetFillInBrush();
     
          brush.ChangeStyle( SIDE_FILL_STYLE );
          brush.ChangeColor( GetpParent()->GetPen().GetColor() );
          brush.ChangeTransparent( TRUE );
     
          Brush old_brush = GetpParent()->ChangeFillInBrush( brush );
     

          GetpParent()->DrawPolygon( bottom_face );
          GetpParent()->DrawPolygon( left_face );

          GetpParent()->ChangeFillInBrush( old_brush );

          GetpParent()->ChangePen( old_pen );
     }

     GetpParent()->DrawLine( start_pt, end_pt );

     for ( UINT16 i = 0; i < num_tic_marks; i++ )
          DrawTicMark( i );

     if ( draw_boundaries )
     {
          const enum PenStyle BOX_STYLE = PEN_DOT;

          // draw line along upper left edge of cube to define face of 3-d plot
     
          Point offset = Point( 0, -ytic_length_device_coords );
          GetpParent()->DrawLine( start_pt + offset, end_pt + offset );
     
          // draw line along lower right edge of cube
     
          offset = Point( tic_length_device_coords, 0 );
          GetpParent()->DrawLine( start_pt + offset, end_pt + offset );

          // change to dotted line for the rest of the boundaries

          pen.ChangeStyle( BOX_STYLE  );
          GetpParent()->ChangePen( pen );

          // draw line along upper right edge of cube

          offset = Point( tic_length_device_coords, -ytic_length_device_coords );
          GetpParent()->DrawLine( start_pt + offset, end_pt + offset );

          // draw line along upper front and rear edges of cube

          offset = Point( tic_length_device_coords, 0 );

          Point  origin = Point(  start_pt + Point( 0, -ytic_length_device_coords ) );

          GetpParent()->DrawLine( origin, origin + offset );

          origin = Point(  end_pt + Point( 0, -ytic_length_device_coords ) );
          GetpParent()->DrawLine( origin, origin + offset );

          // draw line along right front and right rear edges of cube


          origin = Point(  start_pt + Point( tic_length_device_coords, 0 )  );
          offset = Point( 0, -ytic_length_device_coords );

          GetpParent()->DrawLine( origin, origin + offset );

          origin = Point(  end_pt + Point( tic_length_device_coords, 0 )  );
          GetpParent()->DrawLine( origin, origin + offset );

     }

     GetpParent()->ChangePen( old_pen );
}




// ----------------------------------------------------------------------------
//                             XyPlot::XyPlot( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

void ZAxis::ResizeObject( )
{
     start_pt = active_area.BottomLeft();
     end_pt   = active_area.TopRight();

     FLOAT64 x_length;
     FLOAT64 y_length;

     if ( num_tic_marks > 1)
     {
          x_length = ( (FLOAT64) (end_pt.X() - start_pt.X() ) ) / (num_tic_marks-1); 
          y_length = ( (FLOAT64) (end_pt.Y() - start_pt.Y() ) ) / (num_tic_marks-1); 
     }
     else
     {
          x_length = 0.0;
          y_length = 0.0;
     }

     Point position;

     for ( UINT16 i = 0; i < num_tic_marks; i++ )
     {

          position = start_pt + Point( Round( i * x_length) , Round(i * y_length ) );

          SetTicPosition( i, position );
     }

      bottom_face[0] = start_pt;
      bottom_face[1] = start_pt + Point( tic_length_device_coords, 0 );
      bottom_face[2] = end_pt + Point( tic_length_device_coords, 0 ); 
      bottom_face[3] = end_pt;
      left_face[0]   = start_pt;
      left_face[1]   = start_pt + Point( 0, -ytic_length_device_coords );
      left_face[2]   = end_pt + Point( 0, -ytic_length_device_coords ); 
      left_face[3]   = end_pt;
}



