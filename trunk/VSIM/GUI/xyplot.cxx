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
//  XYPLOT.CXX - a module for use in the VSIM User interface 
// ............................................................................
// 
//  This module defines the XyPlot class for use in displaying simulation
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

#include "xyplot.hxx"
#include "markers.hxx"
#include "linestyl.hxx"
#include "plotattr.hxx"

#endif


// ----------------------------------------------------------------------------
//                             XyPlot::XyPlot( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

XyPlot::XyPlot( OutputDevice* pParent ) :
PlotObject( pParent),
color_scheme( DEFAULT_COLOR_SCHEME ),
x_limits_data_changed( FALSE ),
y_limits_data_changed( FALSE ),
vary_line_style( FALSE ),
vary_color( TRUE ),
show_markers( FALSE ),
show_line( TRUE ),
vary_marker_style( TRUE ),
show_rect( TRUE ),
bounding_rect(),
xydata_list(),
x_axis( GetpParent() ),
y_axis( GetpParent() ),
title( GetpParent() ),
legend( GetpParent() ),
x_data_limits(),
y_data_limits()
{
     legend.VaryLineStyle( vary_line_style );
     legend.VaryColor( vary_color );
     legend.ShowMarkers( show_markers );
     legend.ShowLines( show_line );
     legend.VaryMarkerStyle( vary_marker_style );
}


// ----------------------------------------------------------------------------
//                             XyPlot::~XyPlot( )
// ............................................................................
//
//   destructor
//
// ----------------------------------------------------------------------------

XyPlot::~XyPlot( )
{
     // delete any XyDataPlot object we have created

     if ( xydata_list.First() )
     {
          delete xydata_list.GetCurObject();

          while ( xydata_list.Next() )
               delete xydata_list.GetCurObject();
     }
}





// ----------------------------------------------------------------------------
//                             XyPlot::SetColorScheme( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  XyPlot::SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme )  
{ 
     color_scheme = NewScheme; 
     legend.SetColorScheme( NewScheme );
     VaryColor( vary_color); 
}






// ----------------------------------------------------------------------------
//                             XyPlot::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 XyPlot::EditObject( const Point Position )
{
     if ( title.GetActiveArea().IsInside( Position ) )
     {
          title.HighlightObject( TRUE );
          BOOL8 rc = title.EditObject( Position );
          title.HighlightObject( FALSE );
          return rc;
     }


     if ( x_axis.GetActiveArea().IsInside( Position ) )
     {
          x_axis.HighlightObject( TRUE );
          BOOL8 rc = x_axis.EditObject( Position );
          x_axis.HighlightObject( FALSE );

          if ( rc )
          {
                x_axis.ResizeObject();   // this updates the position of tic marks
                SynchXAxisAndDataPlotLimits();
          }

          return rc;
     }

     if ( y_axis.GetActiveArea().IsInside( Position ) )
     {
          y_axis.HighlightObject( TRUE );
          BOOL8 rc = y_axis.EditObject( Position );
          y_axis.HighlightObject( FALSE );

          if ( rc )
          {
               y_axis.ResizeObject();
               SynchYAxisAndDataPlotLimits();
          }

          return rc;
     }

     // edit self if there are no other hits

     XyDataPlot* p_dataplot = xydata_list.First();
     if ( p_dataplot && p_dataplot->GetActiveArea().IsInside( Position ) )
     {
          p_dataplot->HighlightObject( TRUE );

          BOOL8 rc = EditPlotAttributes( this  );

          p_dataplot->HighlightObject( FALSE );

          return rc;
     }

     return FALSE;
}






// ----------------------------------------------------------------------------
//                             XyPlot::ValidateLimits( )
// ............................................................................
//
//   function to ensure that the plot limits are valid in the sense that they
//   canbe used to create a plot without error. This means that the maximum
//   must be greater than the minimum and X,Y data. We update the internal
//   data members x_data_limits and y_data_limits so that the enclose all the
//   current
//
// ----------------------------------------------------------------------------

void XyPlot::ValidateLimits( Limits& rLimits )
{
     const FLOAT64 MIN_RANGE = 1.0e-5;


     // if range is less than zero than invert values

     if ( rLimits.Range() < 0.0 )
     {
          FLOAT64 tempval = rLimits.Min();

          rLimits.Min() = rLimits.Max();

          rLimits.Max() = tempval;
     }


     // if min and max are the same value then Range == 0.0
     // so we give a bit of range to it all for plotting purposes 

     if ( rLimits.Range() == 0.0 )
     {
          if ( rLimits.Min() == 0.0 )
          {
               rLimits.Max() = 1.0;
          }
          else
          {
               rLimits.Max() = 1.1 * rLimits.Max();
               rLimits.Min() = 0.9 * rLimits.Min();
          }
      }


     if ( ( rLimits.Min() > 0.0 ) &&
          ( rLimits.Range() / rLimits.Min() ) < MIN_RANGE
        )
          rLimits.Max()  = ( 1.0 + MIN_RANGE ) * rLimits.Min();
}





// ----------------------------------------------------------------------------
//                             XyPlot::AddDataSet( )
// ............................................................................
//
//   function to add another X,Y data set to the current plot. returns TRUE if
//   addition is successful, FALSE is failure. In this function we create a
//   new XyDataPlot object to contain the X,Y data. We update the internal
//   data members x_data_limits and y_data_limits so that the enclose all the
//   current
//
// ----------------------------------------------------------------------------

BOOL8  XyPlot::AddDataSet( const FloatArray& rX, const FloatArray& rY, const String& rName )
{
     XyDataPlot* ptr = new XyDataPlot( GetpParent(), rX, rY );

     if ( ptr )
     {
          // successfully made a new object - we now need to adjust limits.
          UINT16 dataset_num = xydata_list.Count();

          if ( 0 == dataset_num )
          {
               // If this is the first XyDataPlot object then its limits are the
               // ones we want

               x_data_limits = rX.GetLimits();
               y_data_limits = rY.GetLimits();
          }
          else
          {
               // otherwise set ?_data_limits to include new limits

               x_data_limits.Union( rX.GetLimits() );
               y_data_limits.Union( rY.GetLimits() );
          }

          ptr->ShowPoints( show_markers );
          ptr->ShowLines( show_line );

          ptr->SetMarkerStyle( vary_marker_style ? GetMarkerStyle( dataset_num ) :
           DEFAULT_MARKER_STYLE );

          ptr->SetColor ( vary_color ?
                          ColorScheme[ color_scheme ].GetDataColor( dataset_num ) :
                          ColorScheme[ color_scheme ].GetDefaultDataColor( )
          );

          ptr->SetLineStyle ( vary_line_style ? GetLineStyle( dataset_num ) :
           DEFAULT_LINE_STYLE );

          // now add new object to list at end 

          xydata_list.Insert( ptr, LIST_APPEND );

          x_limits_data_changed = y_limits_data_changed = TRUE;

          legend.AddElement( rName );

          return TRUE;
     }
     else
          return FALSE;
}








// ----------------------------------------------------------------------------
//                             XyPlot::AddDataSet( )
// ............................................................................
//
//   function rescans all the atached data sets and recalculates the limits
//   from their current values. Useful when data in the data sets has changed
//
// ----------------------------------------------------------------------------

void XyPlot::ResetLimits()
{
     UINT16 num_datasets = xydata_list.Count();

     if ( num_datasets == 0 )
     {
          x_data_limits = y_data_limits = Limits();
     }
     else
     {
          XyDataPlot* ptr = xydata_list.GetObject( 0 );
          ptr->ResetLimits();
          x_data_limits = ptr->GetXDataLimits();
          y_data_limits = ptr->GetYDataLimits();


          for ( UINT16 i = 1; i < num_datasets; i++ ) 
          {
               XyDataPlot* ptr = xydata_list.GetObject( i );
               ptr->ResetLimits();

               x_data_limits.Union( ptr->GetXDataLimits() );
               y_data_limits.Union(  ptr->GetYDataLimits() );
          }
     
     }

     x_limits_data_changed = TRUE;
     y_limits_data_changed = TRUE;
}




// ----------------------------------------------------------------------------
//                             XyPlot::VaryLineStyle()
// ............................................................................
// 
//   change internal state of XyPlot object, then scan through and change
//   the states of any xyplotdata object within
//
// ----------------------------------------------------------------------------

void XyPlot::VaryLineStyle( const BOOL8 State ) 
{
     vary_line_style = State; 

     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ ) 
     {
          xydata_list.GetObject( i )->SetLineStyle ( 
           vary_line_style ? GetLineStyle( i ) : DEFAULT_LINE_STYLE );
     }

     legend.VaryLineStyle( State );
}








// ----------------------------------------------------------------------------
//                             XyPlot::GetMaxNumPlotPts()
// ............................................................................
// 
//   
//
// ----------------------------------------------------------------------------

UINT32  XyPlot::GetMaxNumPlotPts()
{
     return ( xydata_list.Count() > 0 ) ? xydata_list.GetObject( 0 )->GetMaxNumPlotPts() : 0;
}


// ----------------------------------------------------------------------------
//                             XyPlot::GetNumberOfDataPairs()
// ............................................................................
// 
//   
//
// ----------------------------------------------------------------------------

UINT32  XyPlot::GetNumberOfDataPairs()
{
     return ( xydata_list.Count() > 0 ) ? xydata_list.GetObject( 0 )->GetNumberOfDataPairs() : 0;
}




// ----------------------------------------------------------------------------
//                             XyPlot::SetMaxNumPlotPts()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8  XyPlot::SetMaxNumPlotPts( UINT32 NumPts ) 
{ 
     BOOL8 success = TRUE;
     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ ) 
     {
          if ( FALSE == xydata_list.GetObject( i )->SetMaxNumPlotPts( NumPts ) )
               success = FALSE;
     }

     return success;
}


// ----------------------------------------------------------------------------
//                             XyPlot::VaryColor( )
// ............................................................................
//
//   change internal state of XyPlot object, then scan through and change 
//   the states of any xyplotdata object within
//
// ----------------------------------------------------------------------------

void XyPlot::VaryColor( const BOOL8 State ) 
{
     vary_color = State; 

     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ )
     {
          xydata_list.GetObject( i )->SetColor ( vary_color ?
                ColorScheme[ color_scheme ].GetDataColor( i ) :
                ColorScheme[ color_scheme ].GetDefaultDataColor( )
          );

     }
     legend.VaryColor( State );
}


// ----------------------------------------------------------------------------
//                             XyPlot::ShowMarkers( )
// ............................................................................
// 
//   change internal state of XyPlot object, then scan through and change 
//   the states of any xyplotdata object within
//
// ----------------------------------------------------------------------------

void XyPlot::ShowMarkers( const BOOL8 State )
{ 
     show_markers = State;

     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ ) 
          xydata_list.GetObject( i )->ShowPoints( show_markers );

     legend.ShowMarkers( State );
}




// ----------------------------------------------------------------------------
//                             XyPlot::VaryMarkerStyle( )
// ............................................................................
//
//   change internal state of XyPlot object, then scan through and change 
//   the states of any xyplotdata object within
//
// ----------------------------------------------------------------------------

void XyPlot::VaryMarkerStyle( const BOOL8 State ) 
{ 
     vary_marker_style = State; 

     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ )
     {
          xydata_list.GetObject( i )->SetMarkerStyle ( 
           vary_marker_style ? GetMarkerStyle( i ) :
           DEFAULT_MARKER_STYLE );
     }

     legend.VaryMarkerStyle( State );
}



// ----------------------------------------------------------------------------
//                             XyPlot::ShowLines( )
// ............................................................................
// 
//   change internal state of XyPlot object, then scan through and change 
//   the states of any xyplotdata object within
//
// ----------------------------------------------------------------------------

void XyPlot::ShowLines( const BOOL8 State )
{
   show_line = State;

     UINT16 num_datasets = xydata_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ )
          xydata_list.GetObject( i )->ShowLines ( show_line );

     legend.ShowLines( State );
}







// ----------------------------------------------------------------------------
//                             XyPlot::ChangepParent( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

OutputDevice*  XyPlot::ChangepParent( OutputDevice* pNewParent )
{
     OutputDevice* old_pparent = PlotObject::ChangepParent( pNewParent );

     if ( xydata_list.First() )
     {
          xydata_list.GetCurObject()->ChangepParent( pNewParent );

          while ( xydata_list.Next() )
               xydata_list.GetCurObject()->ChangepParent( pNewParent );
     }

     // and the rest of the plot components

     x_axis.ChangepParent( pNewParent );
     y_axis.ChangepParent( pNewParent );
     title.ChangepParent( pNewParent );
     legend.ChangepParent( pNewParent );

     return old_pparent;
}


// ----------------------------------------------------------------------------
//                             XyPlot::DrawObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  XyPlot::DrawObject( )
{
     if ( 0 == GetNumberOfDataPairs() )
          return;

     // recalc limits info if necessary

     if ( x_limits_data_changed || y_limits_data_changed )
     {
          ResizeObject();
      }

     // change font temporarily to be transparent

     Font font = GetpParent()->GetFont();
     font.ChangeTransparent( TRUE );

     Font old_font = GetpParent()->ChangeFont( font );

     // draw xy data plots if any exist

     if ( xydata_list.First() )
     {
          xydata_list.GetCurObject()->DrawObject();

          while ( xydata_list.Next() )
               xydata_list.GetCurObject()->DrawObject();
     }

     // and the rest of the plot components

     x_axis.DrawObject( );
     y_axis.DrawObject( );
     title.DrawObject( );
     legend.DrawObject( );

     if ( highlight )
          HighlightArea();

     if ( show_rect )
     {
          const UINT16 RECT_WIDTH = 3;
          Pen pen = GetpParent()->GetPen();
          pen.ChangeWidth( RECT_WIDTH );

          Pen old_pen = GetpParent()->ChangePen( pen );

          Polygon  bounding_polygon( bounding_rect );
          bounding_polygon.ChangeSize( 5 );
          bounding_polygon.ChangePoint( bounding_polygon.GetPoint( 0 ), 4 );
          GetpParent()->DrawPolyLine( bounding_polygon );

          GetpParent()->ChangePen( old_pen );
     }

     GetpParent()->ChangeFont( old_font );
}




// ----------------------------------------------------------------------------
//                             XyPlot::ResizeObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyPlot::ResizeObject( )
{
     const UINT16       LEFT_MARGIN  = 8;
     const UINT16       RIGHT_MARGIN = 8;
     const UINT16       TOP_MARGIN   =  3;
     const UINT16       BOTTOM_MARGIN   =  4;

     if ( 0 != GetNumberOfDataPairs() )
     {
          if ( x_limits_data_changed )
               RecalcXAxisLimits();
     
          if ( y_limits_data_changed )
               RecalcYAxisLimits();
     }

     // do this rather complex operation to be sure we get
     // actual physcial font size

     Size   font_size      = GetpParent()->GetFont().GetSize();

     INT16 left_margin     = LEFT_MARGIN * font_size.Width();
     INT16 right_margin    = RIGHT_MARGIN * font_size.Width();
     INT16 top_margin      = TOP_MARGIN * font_size.Height();
     INT16 bottom_margin   = BOTTOM_MARGIN * font_size.Height();

    // parm order for Rectangle creation : left top right bottom

    bounding_rect = Rectangle( active_area.Left() + left_margin,
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



     if ( xydata_list.First() )
     {
          xydata_list.GetCurObject()->SetActiveArea( bounding_rect );
          xydata_list.GetCurObject()->ResizeObject();
 
          while ( xydata_list.Next() )
          {
               xydata_list.GetCurObject()->SetActiveArea( bounding_rect );
               xydata_list.GetCurObject()->ResizeObject();
          }
     }
 

    x_axis.SetActiveArea( x_axis_rect );
    x_axis.SetTicLength( font_size.Height() / 2 );
    x_axis.SetGridLength( bounding_rect.GetHeight() );

    x_axis.ResizeObject();

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
//                       XyPlot::SynchXAxisAndDataPlotLimits()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyPlot::SynchXAxisAndDataPlotLimits()
{
     if ( xydata_list.First() )
     {
          xydata_list.GetCurObject()->SetXDataLimits( x_axis.GetAxisLimits() );

          while ( xydata_list.Next() )
               xydata_list.GetCurObject()->SetXDataLimits( x_axis.GetAxisLimits() );
     }
}




// ----------------------------------------------------------------------------
//                       XyPlot::SynchYAxisAndDataPlotLimits()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyPlot::SynchYAxisAndDataPlotLimits()
{
     if ( xydata_list.First() )
     {
          xydata_list.GetCurObject()->SetYDataLimits( y_axis.GetAxisLimits() );

          while ( xydata_list.Next() )
               xydata_list.GetCurObject()->SetYDataLimits( y_axis.GetAxisLimits() );
     }
}






// ----------------------------------------------------------------------------
//                             XyPlot::RecalcXAxisLimits( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------


void XyPlot::RecalcXAxisLimits()
{
     ValidateLimits( x_data_limits );

     x_axis.SetDataLimits( x_data_limits );
     SynchXAxisAndDataPlotLimits();
     x_limits_data_changed = FALSE;
}


// ----------------------------------------------------------------------------
//                             XyPlot::RecalcYAxisLimits( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyPlot::RecalcYAxisLimits()
{
     ValidateLimits( y_data_limits );

     y_axis.SetDataLimits( y_data_limits );
     SynchYAxisAndDataPlotLimits();
     y_limits_data_changed = FALSE;
}





// ----------------------------------------------------------------------------
//                          XyPlot::SetXAxisParameters( )
// ............................................................................
//
//  sets X axis parameters to user-selected values 
//
// ----------------------------------------------------------------------------

enum AXIS_RC XyPlot::SetXAxisParameters( const Limits& rNewLimits, 
 const FLOAT64 NewTicInterval, const UINT16 Precision ) 
{ 
     enum AXIS_RC rc = x_axis.SetAxisParameters( rNewLimits, NewTicInterval, Precision ); 

     // if no error in new axis parameters, then make sure  xydataplot objects have same
     // limits

     if ( AXIS_RC_VALID_DATA == rc )
     {
          SynchXAxisAndDataPlotLimits();
          x_limits_data_changed = FALSE;
     }

     return rc;
}




// ----------------------------------------------------------------------------
//                          XyPlot::SetYAxisParameters( )
// ............................................................................
//
//  sets X axis parameters to user-selected values
//
// ----------------------------------------------------------------------------

enum AXIS_RC XyPlot::SetYAxisParameters( const Limits& rNewLimits,
 const FLOAT64 NewTicInterval, const UINT16 Precision )
{
     enum AXIS_RC rc = y_axis.SetAxisParameters( rNewLimits, NewTicInterval, Precision );

     // if no error in new axis parameters, then make sure  xydataplot objects have same
     // limits

     if ( AXIS_RC_VALID_DATA == rc )
     {
          SynchYAxisAndDataPlotLimits();

          y_limits_data_changed = FALSE;
     }

     return rc;
}



