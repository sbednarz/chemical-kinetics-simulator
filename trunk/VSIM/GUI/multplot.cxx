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
//  MULTPLOT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This files defines the MultiXyPlot class for displaying
//  multiple x-y plots of simulation results for the VSIM user interface.
//
//  CREATED : 7.19.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------





#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "multplot.hxx"

#endif






// ----------------------------------------------------------------------------
//                      MultiXyPlot::MultiXyPlot( )
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

MultiXyPlot::MultiXyPlot( OutputDevice* pParent ) :
PlotObject( pParent),
plot_list(),
color_scheme( DEFAULT_COLOR_SCHEME )
{
}



// ----------------------------------------------------------------------------
//                      MultiXyPlot::~MultiXyPlot( )
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

MultiXyPlot::~MultiXyPlot( ) 
{
     // delete any XyPlot objects we have created

     if ( plot_list.First() )
     {
          delete plot_list.GetCurObject();
 
          while ( plot_list.Next() )
               delete plot_list.GetCurObject();
     }
}




// ----------------------------------------------------------------------------
//                          MultiXyPlot::SetMaxNumPlotPts()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 MultiXyPlot::SetMaxNumPlotPts( UINT32 NumPts )
{ 
     BOOL8 success = TRUE;
     UINT16 num_datasets = plot_list.Count();

     for ( UINT16 i = 0; i < num_datasets; i++ ) 
     {
          if ( FALSE == plot_list.GetObject( i )->SetMaxNumPlotPts( NumPts ) )
               success = FALSE;
     }

     return success;
}






// ----------------------------------------------------------------------------
//                      MultiXyPlot::AddPlot( )
// ............................................................................
//
//   called to add a plot to the MultiPlot object. returns the ptr to 
//   the newly created object, NULL if failure 
//
// ----------------------------------------------------------------------------

XyPlot* MultiXyPlot::AddPlot( )
{
     XyPlot* ptr = new XyPlot( GetpParent() );

     if ( ptr )
     {
          ptr->SetColorScheme( color_scheme );
          plot_list.Insert( ptr, LIST_APPEND );

          coordinates_invalid = TRUE;
     }

     return ptr;
}







// ----------------------------------------------------------------------------
//                      MultiXyPlot::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 MultiXyPlot::EditObject( const Point Position )
{
     BOOL8 rc = FALSE;

     ULONG num_plots = plot_list.Count();

     for ( ULONG i = 0; i < num_plots; i++ )
     {
          XyPlot* pplot =  plot_list.GetObject( i );

          if ( pplot->GetActiveArea().IsInside( Position ) )
          {
               rc = pplot->EditObject( Position );
               break;
          }
     }

     return rc;
}




// ----------------------------------------------------------------------------
//                      MultiXyPlot::DrawObject( )
// ............................................................................
//
//   called to draw the MultiPlot object
//
// ----------------------------------------------------------------------------

void MultiXyPlot::DrawObject( )
{
   if ( coordinates_invalid )
      ResizeObject();

     if ( plot_list.First() )
     {
          plot_list.GetCurObject()->DrawObject();

          while ( plot_list.Next() )
               plot_list.GetCurObject()->DrawObject();
     }
}




// ----------------------------------------------------------------------------
//                      MultiXyPlot::ChangepParent( )
// ............................................................................
//
//   called to change parent - used eg for printing
//
// ----------------------------------------------------------------------------

OutputDevice*  MultiXyPlot::ChangepParent( OutputDevice* pNewParent )
{
     OutputDevice* old_pparent = PlotObject::ChangepParent( pNewParent );

     if ( plot_list.First() )
     {
          plot_list.GetCurObject()->ChangepParent( pNewParent );

          while ( plot_list.Next() )
               plot_list.GetCurObject()->ChangepParent( pNewParent );
     }

     return old_pparent;
}




// ----------------------------------------------------------------------------
//                      MultiXyPlot::ResizeObject( )
// ............................................................................
//
//   called to resize the MultiPlot object
//
// ----------------------------------------------------------------------------

void MultiXyPlot::ResizeObject( )
{
     const UINT16  num_plots = (UINT16) plot_list.Count();

     const Size    output_size = active_area.GetSize();

     const UINT16 NUM_COLUMNS = (num_plots > 1) ? 2 : 1;

     // calculate the number of rows

     UINT16 num_rows = num_plots / NUM_COLUMNS;

     // add a less than full row if needed

     if ( num_plots % NUM_COLUMNS )
          num_rows++;

     // now calculate the single_plot_size; ( width, height )

     if ( num_rows )
     {
          const INT16 x_step = output_size.Width() / NUM_COLUMNS;

          // special case for 2 plots since they look awkward otherwise
          // (too tall)

          const INT16 y_step   = ( 2 == num_plots ) ? output_size.Height() / 2 : output_size.Height() / num_rows;

          const Size single_plot_size = Size( x_step, y_step );

          const INT16 max_x = x_step * NUM_COLUMNS;

          INT16 x_offset = active_area.Left();
          INT16 y_offset = active_area.Top();

          if ( 2 == num_plots )
               y_offset += (y_step /2);

          for ( UINT16 i = 0; i < num_plots; i++ )
          {
               XyPlot* ptr = plot_list.GetObject( i );

               ptr->SetActiveArea( Rectangle( Point( x_offset, y_offset ), single_plot_size) );
               ptr->ResizeObject( );

               // increment position

               x_offset += x_step;

               if (x_offset >= max_x)
               {
                    x_offset = 0;
                    y_offset += y_step;
               }
          }

     }
     coordinates_invalid = FALSE;
}





// ----------------------------------------------------------------------------
//                      MultiXyPlot::SetColorScheme( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void  MultiXyPlot::SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme )
{
     Pen pen = GetpParent()->GetPen();

     pen.ChangeColor( Color( ColorScheme[ NewScheme ].GetAxisColor() ) );
     GetpParent()->ChangePen( pen );

     Font font = GetpParent()->GetFont();

     font.ChangeColor( Color( ColorScheme[ NewScheme ].GetTextColor() ) );
     GetpParent()->ChangeFont( font );

     Brush brush = GetpParent()->GetBackgroundBrush();

     brush.ChangeColor( Color( ColorScheme[ NewScheme ].GetBackgroundColor() ) );
     GetpParent()->ChangeBackgroundBrush( brush );


     if ( plot_list.First() )
     {
          plot_list.GetCurObject()->SetColorScheme( NewScheme );

          while ( plot_list.Next() )
               plot_list.GetCurObject()->SetColorScheme( NewScheme );
     }

     color_scheme = NewScheme;
}


