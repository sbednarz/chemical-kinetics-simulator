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
//  ABARGRAF.CXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the AnimatedBarGraph class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar graph with two axes. It includes info about drawing the object 
//  on the screen. 
//
//  CREATED : 01.31.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "abargraf.hxx"
#include "brushes.hxx"

#endif







// ----------------------------------------------------------------------------
//             AnimatedBarGraph::AnimatedBarGraph()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

AnimatedBarGraph::AnimatedBarGraph( Window* pParent ) :
Window( pParent ),
y_axis( this ),
p_det_var_display( NULL ),
title( this ),
legend( this ),
element_list(),
timer(),
vary_fill_style( TRUE ),
vary_color( TRUE ),
color_scheme( DEFAULT_COLOR_SCHEME )
{
     timer.ChangeTimeoutHdl( LINK( this, AnimatedBarGraph, TimeOutHandler ) );
}







// ----------------------------------------------------------------------------
//             AnimatedBarGraph::~AnimatedBarGraph()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

AnimatedBarGraph::~AnimatedBarGraph()
{
     // delete any AnimatedBarGraphElement object we have created

     if ( element_list.First() )
     {
          delete element_list.GetCurObject();

          while ( element_list.Next() )
               delete element_list.GetCurObject();
     }

     if (p_det_var_display)
          delete p_det_var_display;
}





// ----------------------------------------------------------------------------
//                       AnimatedBarGraph::AddElement()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarGraph::SetDeterminateVariableBar( const FloatArray& rArray, const String& rName )
{
     // delete existing if allocated 

     if (p_det_var_display)
          delete p_det_var_display;

     p_det_var_display  = new AnimatedBarDisplay( this, rName, rArray, BAR_HORIZONTAL );

     if ( p_det_var_display )
     {
          p_det_var_display->SetBarColor(ColorScheme[ color_scheme ].GetDefaultDataColor( ) );
          p_det_var_display->SetBarFillPattern( DEFAULT_BRUSH_STYLE );

          p_det_var_display->Show();
          return TRUE;
     }
     else
          return FALSE;
}





// ----------------------------------------------------------------------------
//                       AnimatedBarGraph::AddElement()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarGraph::AddElement( const FloatArray& rArray, const String& rName )
{
     AnimatedBarDisplay* ptr  = new AnimatedBarDisplay( this, rName, rArray );

     if (ptr )
     {
          // set color and fill style if active 

          UINT16 dataset_num = element_list.Count();



          ptr->SetBarColor( vary_color ?
                          ColorScheme[ color_scheme ].GetDataColor( dataset_num ) :
                          ColorScheme[ color_scheme ].GetDefaultDataColor( )
          );

          ptr->SetBarFillPattern( vary_fill_style ? GetBrushStyle( dataset_num ) :
           DEFAULT_BRUSH_STYLE );

          ptr->Show();

          element_list.Insert( ptr, LIST_APPEND ); 
          return TRUE;
     }
     else
          return FALSE;
}






// ----------------------------------------------------------------------------
//             AnimatedBarGraph::Paint()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void AnimatedBarGraph::Paint( const Rectangle& )
{
     // drae rest of the plot components

     y_axis.DrawObject( );

}







// ----------------------------------------------------------------------------
//             AnimatedBarGraph::ResizeObject()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void AnimatedBarGraph::Resize( )
{
     const UINT16       LEFT_MARGIN  = 10;
     const UINT16       RIGHT_MARGIN = 10;
     const UINT16       TOP_MARGIN   =  3;
     const UINT16       BOTTOM_MARGIN   =  4;


     Rectangle active_area( Point(), GetOutputSizePixel() );
     // do this rather complex operation to be sure we get
     // actual physical font size

     Size   font_size      = LogicToPixel( GetFont().GetSize() );

     INT16 left_margin     = LEFT_MARGIN * font_size.Width();
     INT16 right_margin    = RIGHT_MARGIN * font_size.Width();
     INT16 top_margin      = TOP_MARGIN * font_size.Height();
     INT16 bottom_margin   = BOTTOM_MARGIN * font_size.Height();

     // parm order for Rectangle creation : left top right bottom

     Rectangle bounding_rect( active_area.Left() + left_margin,
                              active_area.Top() + top_margin,
                              active_area.Right() - right_margin,
                              active_area.Bottom() - bottom_margin );


     Rectangle x_axis_rect( bounding_rect.BottomLeft(),
                             bounding_rect.BottomRight() + Point( 0, bottom_margin ) );


     Rectangle y_axis_rect( bounding_rect.TopLeft() - Point( left_margin, 0 ),
                           bounding_rect.BottomLeft() );

     Rectangle plot_title_rect( bounding_rect.TopLeft() - Point( 0, top_margin),
                               bounding_rect.TopRight() );

     Rectangle plot_legend_rect( bounding_rect.TopRight(),
                                bounding_rect.BottomRight() + Point( right_margin, 0 ) );


     // here we specify the areas for each of the BarGraphElements 
     // we need to evenly subdivide the area defined by bounding_rect 
     // into num_elements equal spaces 


     INT16  width = bounding_rect.GetWidth() / element_list.Count();


     Point origin( bounding_rect.TopLeft() );
     Size  size( width, bounding_rect.GetHeight() );

     if ( element_list.First() )
     {
          element_list.GetCurObject()->SetPosSizePixel( origin, size );
 
          while ( element_list.Next() )
          {
               origin += Point( width, 0 );
               element_list.GetCurObject()->SetPosSizePixel( origin, size );
          }
     }
 
     if ( p_det_var_display )
          p_det_var_display->SetPosSizePixel( x_axis_rect.TopLeft(),x_axis_rect.GetSize() ); 

    y_axis.SetActiveArea( y_axis_rect );
    y_axis.SetTicLength( font_size.Height() / 2 ); 
    y_axis.SetGridLength( bounding_rect.GetWidth() );

    y_axis.ResizeObject();

    title.SetActiveArea( plot_title_rect );
    title.ResizeObject( );

    legend.SetActiveArea( plot_legend_rect ); 
    legend.ResizeObject( );
}
   






// ----------------------------------------------------------------------------
//             AnimatedBarGraph::ShowNextValue()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

BOOL8 AnimatedBarGraph::ShowNextValue()
{
     if ( element_list.First() )
     {
          // if we get FALSE return value then we are at end of arrays

          if (!element_list.GetCurObject()->Next() )
               return FALSE;

          while ( element_list.Next() )
               element_list.GetCurObject()->Next();
     }

     if ( p_det_var_display )
          p_det_var_display->Next();
          
     return TRUE;
}




// ----------------------------------------------------------------------------
//             AnimatedBarGraph::Play()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void  AnimatedBarGraph::Play()
{
     ShowNextValue();

     timer.Start();
}






// ----------------------------------------------------------------------------
//             AnimatedBarGraph::Rewind()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void  AnimatedBarGraph::Rewind()
{
     if ( element_list.First() )
     {
          element_list.GetCurObject()->First();

          while ( element_list.Next() )
               element_list.GetCurObject()->First();
     }

     if ( p_det_var_display )
          p_det_var_display->First();
}








// ----------------------------------------------------------------------------
//             AnimatedBarGraph::TimeOutHandler()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void  AnimatedBarGraph::TimeOutHandler( Timer* )
{
     if ( FALSE == ShowNextValue() )
          Stop();
}


