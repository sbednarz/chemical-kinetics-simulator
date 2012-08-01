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
//  STAKPLOT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This files defines the StackedXyPlot class for displaying a
//  waterfall-style x-y plot of simulation results for the VSIM user interface.
//
//  CREATED : 7.20.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "stakplot.hxx"
#include "math.hxx"

#endif




// ----------------------------------------------------------------------------
//                      StackedXyPlot::StackedXyPlot( )
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

StackedXyPlot::StackedXyPlot( OutputDevice* pParent ) :
XyPlot( pParent ),
z_axis( GetpParent() ),
labels_need_refresh( TRUE ),
name_list()
{
     z_axis.DrawBoundaries( show_rect );
}



// ----------------------------------------------------------------------------
//                      MultiXyPlot::~MultiXyPlot( )
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

StackedXyPlot::~StackedXyPlot( )
{
     if ( name_list.First() )
     {
          delete name_list.GetCurObject();
 
          while ( name_list.Next() )
               delete name_list.GetCurObject();
     }
}





// ----------------------------------------------------------------------------
//                             StackedXyPlot::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 StackedXyPlot::EditObject( const Point Position )
{
     BOOL8 rc = XyPlot::EditObject( Position );

     z_axis.DrawBoundaries( show_rect );

     return rc;
}



// ----------------------------------------------------------------------------
//                      StackedXyPlot::RefreshLabels( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StackedXyPlot::RefreshLabels()
{
     UINT16 count = name_list.Count();

     z_axis.SetNumTicMarks( count );

     while ( count-- ) 
        z_axis.SetTicLabel( count, *(name_list.GetObject( count )) );

     labels_need_refresh = FALSE;
}





// ----------------------------------------------------------------------------
//                      StackedXyPlot::AddDataSet( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 StackedXyPlot::AddDataSet( const FloatArray& rX, const FloatArray& rY, 
const String& rName )
{
     if ( XyPlot::AddDataSet(rX, rY, rName ) ) 
     {
          String* ptr = new String( rName );

          if (ptr )
          {
               name_list.Insert( ptr, LIST_APPEND );

               labels_need_refresh = TRUE;

               return TRUE;
          }
               return FALSE;
     }
     else
          return FALSE;
}






// ----------------------------------------------------------------------------
//                             StackedXyPlot::ChangepParent( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

OutputDevice*  StackedXyPlot::ChangepParent( OutputDevice* pNewParent )
{
     OutputDevice* old_pparent = PlotObject::ChangepParent( pNewParent );

     z_axis.ChangepParent( pNewParent );

     // draw xy data plots if any exist start with rearmost and work forward

     UINT16 num_xy_datasets = xydata_list.Count();

     while (num_xy_datasets--)
          xydata_list.GetObject( num_xy_datasets )->ChangepParent( pNewParent );

     // and the rest of the plot components

     x_axis.ChangepParent( pNewParent );
     y_axis.ChangepParent( pNewParent );
     title.ChangepParent( pNewParent );

     return old_pparent;
}





// ----------------------------------------------------------------------------
//                      StackedXyPlot::DrawObject()
// ............................................................................
//
//   called to draw the StackedXyPlot object
//
// ----------------------------------------------------------------------------

void StackedXyPlot::DrawObject( )
{
     if ( labels_need_refresh )
        RefreshLabels();

     // recalc limits info if necessary 

   if ( x_limits_data_changed || y_limits_data_changed )
          ResizeObject();

     // change font temporarily to be transparent 

     Font font = GetpParent()->GetFont();
     font.ChangeTransparent( TRUE );

     Font old_font = GetpParent()->ChangeFont( font );

     z_axis.DrawObject( );

     // draw xy data plots if any exist start with rearmost and work forward

     UINT16 num_xy_datasets = xydata_list.Count();

     while (num_xy_datasets--)
          xydata_list.GetObject( num_xy_datasets )->DrawObject();

     // and the rest of the plot components

     x_axis.DrawObject( );
     y_axis.DrawObject( );
     title.DrawObject( );

     GetpParent()->ChangeFont( old_font );
}




// ----------------------------------------------------------------------------
//                      StackedXyPlot::ResizeObject(_
// ............................................................................
//
//   called to resize the StackedXyPlot object
//
// ----------------------------------------------------------------------------

void StackedXyPlot::ResizeObject( )
{
     const UINT16       LEFT_MARGIN  = 10;
     const UINT16       RIGHT_MARGIN = 10;
     const UINT16       TOP_MARGIN   =  3;
     const UINT16       BOTTOM_MARGIN   =  4;

     if ( labels_need_refresh )
        RefreshLabels();

     if ( x_limits_data_changed )
          RecalcXAxisLimits();

     if ( y_limits_data_changed )
          RecalcYAxisLimits();

     Size   font_size      = GetpParent()->GetFont().GetSize();

     INT16 left_margin     = LEFT_MARGIN * font_size.Width();
     INT16 right_margin    = RIGHT_MARGIN * font_size.Width(); 
     INT16 top_margin      = TOP_MARGIN * font_size.Height();
     INT16 bottom_margin   = BOTTOM_MARGIN * font_size.Height(); 

    // parm order for Rectangle creation : left top right bottom 

     Rectangle data_plot_rect( active_area.Left() + left_margin, 
                               active_area.Top() + top_margin, 
                                active_area.Right() - right_margin,
                                active_area.Bottom() - bottom_margin );
 
     Rectangle plot_title_rect( data_plot_rect.TopLeft() - Point( 0, top_margin),
                                data_plot_rect.TopRight() );                       
 
     Rectangle plot_legend_rect( data_plot_rect.TopRight(),                                                        
                                 data_plot_rect.BottomRight() + Point( right_margin, 0 ) );
 
     title.SetActiveArea( plot_title_rect );
     title.ResizeObject( );

     const FLOAT64 reduction_factor = 0.70;

     // now change size of data_plot_rect and reposition so that bottomleft is same 
     // as before scaling 

     Size dataplot_overall_size = data_plot_rect.GetSize();

     Point position = data_plot_rect.BottomLeft( );

     // scale it 
     data_plot_rect.ChangeSize( Size( Round( reduction_factor * dataplot_overall_size.Width() ) , 
                                      Round( reduction_factor * dataplot_overall_size.Height() )
                                    ) 
                              );

     // and move 
     position -= Point( 0, data_plot_rect.GetHeight()-1 );
     data_plot_rect.ChangePos( position );

     // now position x, y axes before we mess with things more 

     Rectangle x_axis_rect( data_plot_rect.BottomLeft(),              
                              data_plot_rect.BottomRight() + Point( 0, bottom_margin ) );
 
 
     Rectangle y_axis_rect( data_plot_rect.TopLeft() - Point( left_margin, 0 ), 
                            data_plot_rect.BottomLeft() );
 
     x_axis.SetActiveArea( x_axis_rect );
     x_axis.SetTicLength( font_size.Height() / 2 );
     x_axis.SetGridLength( data_plot_rect.GetHeight() );
 
     x_axis.ResizeObject();
 
     y_axis.SetActiveArea( y_axis_rect );
     y_axis.SetTicLength( font_size.Height() / 2 ); 
     y_axis.SetGridLength( data_plot_rect.GetWidth() );
 
     y_axis.ResizeObject();
 
     // now set up to position each x,y dataset in a different place 

     Point step_point;

     const UINT16 num_xy_datasets = xydata_list.Count();

     if (num_xy_datasets > 1)
          step_point = Point( -(dataplot_overall_size.Width() - data_plot_rect.GetSize().Width() ) / (num_xy_datasets-1),
                              (dataplot_overall_size.Height() - data_plot_rect.GetSize().Height() ) / (num_xy_datasets-1)
                            );

     for ( UINT16 i = 0; i < num_xy_datasets; i++ )
     {
          XyDataPlot* ptr = xydata_list.GetObject( i );
          ptr->SetActiveArea( data_plot_rect ); 
          ptr->ResizeObject();

          position -= step_point;
          data_plot_rect.ChangePos( position );
     }

          position += step_point;

     // Rect constructor Left Top Right BOttom 

     z_axis.SetActiveArea( Rectangle( x_axis_rect.Left(), 
                                      position.Y() + data_plot_rect.GetHeight(), 
                                      position.X(), 
                                      x_axis_rect.Top() 
                                    ) 
                         );

     z_axis.SetXTicLength( x_axis_rect.GetWidth() );
     z_axis.SetYTicLength( y_axis_rect.GetHeight() );

     z_axis.ResizeObject();
}



// ----------------------------------------------------------------------------
//                      StackedXyPlot::SetColorScheme( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void  StackedXyPlot::SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme )
{
     XyPlot::SetColorScheme( NewScheme );

     Pen pen = GetpParent()->GetPen();

     pen.ChangeColor( Color( ColorScheme[ NewScheme ].GetAxisColor() ) );
     GetpParent()->ChangePen( pen );

     Font font = GetpParent()->GetFont();

     font.ChangeColor( Color( ColorScheme[ NewScheme ].GetTextColor() ) );
     GetpParent()->ChangeFont( font );

     Brush brush = GetpParent()->GetBackgroundBrush();

     brush.ChangeColor( Color( ColorScheme[ NewScheme ].GetBackgroundColor() ) );
     GetpParent()->ChangeBackgroundBrush( brush );


}


