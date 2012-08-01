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

/* []----------------------------------------------------------------------[]
   |  msimxy.cxx       :  defines the class msimXYPlot for MSIM             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   Aug 22 1993                                        |
   |                                                                        |
   |  this module contains the definitiona of the object class msimXYPlot.  |
   |  This provides a device-independent definition of an X-Y Plot of       |
   |  two data arrays. The definition                                       |
   |  Also included are the initialization and shutdown routines for the    |
   |  overall application                                                   |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop
#include "msimplot.hxx"
#include <string.h>

#if defined(__OS2__)
#define GRID_LINESTYLE          PEN_DOT
#endif

#if defined(__MSDOS__)
#define GRID_LINESTYLE          PEN_DOT
#endif

#if defined(__AIX__)
#define GRID_LINESTYLE          PEN_DOT
#endif

#if defined(__MAC__)
#define GRID_LINESTYLE          PEN_DOT
#endif



#define SIM_DATA_INTERVAL       1
#define EXP_DATA_INTERVAL       1


struct COLOR_STRUCT
{
     enum ColorName background_color;
     enum ColorName axis_color;
     enum ColorName default_line_color;
     enum ColorName exp_data_color;
     enum ColorName line_color[MAX_NO_OF_PLOTS + 1];

};


static struct COLOR_STRUCT ColorSet[msimNUMBER_OF_COLORSETS] =
     {
          {
               COL_BLACK, COL_WHITE, COL_YELLOW, COL_YELLOW,// black bkgrd
               {
                    COL_YELLOW, COL_LIGHTRED, COL_LIGHTBLUE, COL_LIGHTGREEN, COL_LIGHTCYAN, COL_LIGHTMAGENTA, COL_WHITE, COL_USER
               }
          },
          {
               COL_WHITE, COL_BLACK, COL_RED, COL_RED,// white bkgrd
               {
                    COL_RED, COL_BLUE, COL_GREEN, COL_CYAN, COL_BROWN, COL_MAGENTA, COL_BLACK, COL_USER
               }
          },
          {
               COL_LIGHTGRAY, COL_BLACK, COL_RED, COL_RED,//  gray bkgrd
               {
                    COL_RED, COL_BLUE, COL_GREEN, COL_CYAN, COL_BROWN, COL_MAGENTA, COL_BLACK, COL_USER
               }
          },
          {
               COL_CYAN, COL_WHITE, COL_YELLOW, COL_YELLOW,// cyan bkgrd
               {
                    COL_YELLOW, COL_LIGHTRED, COL_BLACK, COL_LIGHTGREEN, COL_LIGHTCYAN, COL_LIGHTMAGENTA, COL_WHITE, COL_USER
               }
          },
          {
               COL_BLACK, COL_WHITE, COL_WHITE, COL_WHITE,// white on black
               {
                    COL_WHITE, COL_WHITE, COL_WHITE, COL_WHITE, COL_WHITE, COL_WHITE, COL_WHITE, COL_USER
               }
          },
          {
               COL_WHITE, COL_BLACK, COL_BLACK, COL_BLACK,// black on white
               {
                    COL_BLACK, COL_BLACK, COL_BLACK, COL_BLACK, COL_BLACK, COL_BLACK, COL_BLACK, COL_USER
               }
          },
          {
               COL_WHITE, COL_BLACK, COL_RED, COL_RED,// paper color set
               {
                    COL_RED, COL_BLUE, COL_GREEN, COL_CYAN, COL_BROWN, COL_MAGENTA, COL_BLACK, COL_USER
               }
          }
     };


/* ------- data definitions follow ------                                   */
/* lookup table for viewport coordinates                                    */
/* designed to be referenced as Viewport[tot_num_wins][this_win_num].coord  */



static VIEWPORT_ARRAY Viewport[MAX_NO_VIEWPORTS] =
{

     {                                 /* for single plot in window           */
          {
               0.0, 0.0, GC_XMAX, GC_YMAX
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
     }
     ,
     {                                 /* for two plots in window one above
                                          the other                           */
          {
               0.0, 0.0, GC_XMAX, GC_YMAX / 2.0
          }
          ,
          {
               0.0, GC_YMAX / 2.0, GC_XMAX, GC_YMAX
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
     }
     ,
     {                                 /* for three plots                     */
          {
               0.0, 0.0, GC_XMAX / 2.0, GC_YMAX / 2.0
          }
          ,
          {
               0.0, GC_YMAX / 2.0, GC_XMAX / 2.0, GC_YMAX
          }
          ,
          {
               GC_XMAX / 2.0, 0.0, GC_XMAX, GC_YMAX / 2.0
          }
          ,
          {
               0.0, 0.0, 0.0, 0.0
          }
     }
     ,
     {                                 /* for four plots arranged in a 2x2
                                          pattern                             */
          {
               0.0, 0.0, GC_XMAX / 2.0, GC_YMAX / 2.0
          }
          ,
          {
               0.0, GC_YMAX / 2.0, GC_XMAX / 2.0, GC_YMAX
          }
          ,
          {
               GC_XMAX / 2.0, 0.0, GC_XMAX, GC_YMAX / 2.0
          }
          ,
          {
               GC_XMAX / 2.0, GC_YMAX / 2.0, GC_XMAX, GC_YMAX
          }
     }
};

XYPlot::XYPlot( msimWID pParent, PPLOT_SPECS pPlotSpecs )

{
     pplot_specs = pPlotSpecs;
     owner = pParent;
     true_rotated_font = TRUE;
}

void XYPlot::CalcSubWindowSizeInDeviceCoords( USHORT NumPlots, USHORT ThisPlotNum )
{
     VIEWPORT win_coords;

/* get the coordinates of the subwindow we are drawing in                   */

     win_coords = Viewport[ --NumPlots][ --ThisPlotNum];

     window_width = pplot_specs->horizontal_win_size;
     window_height = pplot_specs->vertical_win_size;


     lolim_x = Round(( win_coords.lower_left_x * window_width ) + LEFT_SPACE * char_width );
     hilim_x = Round(( win_coords.upper_right_x * window_width ) - RIGHT_SPACE * char_width );

#if defined(USE_MAPMODE)

    lolim_y = Round(( win_coords.lower_left_y * window_height ) + BOTTOM_SPACE * char_ht );
    hilim_y = Round(( win_coords.upper_right_y * window_height ) - TOP_SPACE * char_ht );
#else
     lolim_y = Round(( win_coords.lower_left_y * window_height ) - BOTTOM_SPACE * char_ht );
     hilim_y = Round(( win_coords.upper_right_y * window_height ) + TOP_SPACE * char_ht );
#endif
}


void XYPlot::DrawSelectedPlots( )
{
     USHORT plot_num = 0;              /* keeps track of how many             */
     msimPINSTANCE instance = pplot_specs->plot_instance;
     USHORT i = 0;

     /* plot any concentration plots requested                         */

     if ( pplot_specs->plotconc_v_time || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == CONC_DATA ) &&
                    ( pplot_specs->expdata_x_type == TIME_DATA ) ) )
     {
          plot_num++;

          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotconc_v_time )
          {

               for ( i = 0; i < pplot_specs->num_species_to_plot; i++ )
               {
                    PlotTheData( pplot_specs->time_axis.axismin
                         *pplot_specs->time_axis.mult,
                         pplot_specs->time_axis.axismax
                         *pplot_specs->time_axis.mult,
                         pplot_specs->conc_axis.axismin
                         *pplot_specs->conc_axis.mult,
                         pplot_specs->conc_axis.axismax
                         *pplot_specs->conc_axis.mult,
                         pplot_specs->Ptime,/* xdata                          */
                         pplot_specs->Pconc[i],/* y data                      */
                         pplot_specs->num_data_sets,/* num pts                */
                         SIM_DATA_INTERVAL, pplot_specs->vary_color ?/* color */
                              pplot_specs->line_color[i] :
                              pplot_specs->default_line_color,
                              pplot_specs->vary_linestyle ?/* linestyle       */
                                   pplot_specs->line_style[i] :
                                   pplot_specs->default_line_style,
                                   pplot_specs->vary_markerstyle ? pplot_specs->marker_style[i
                                        ] : pplot_specs->default_marker_style,
                                   pplot_specs->show_line, pplot_specs->show_marker );

                    WriteSpeciesLabel( pplot_specs->plotlabel[i],/* the
                                                            test to be written*/
                         i,            /* which label it is                   */
                         pplot_specs->vary_color ?/* color                    */
                              pplot_specs->line_color[i] :
                              pplot_specs->default_line_color,
                              pplot_specs->vary_linestyle ?/* linestyle       */
                                   pplot_specs->line_style[i] :
                                   pplot_specs->default_line_style,
                                   pplot_specs->vary_markerstyle ? pplot_specs->marker_style[i
                                        ] : pplot_specs->default_marker_style,
                                   pplot_specs->show_line, pplot_specs->show_marker );

               }
          }

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         ( USHORT ) CONC_DATA ) && ( pplot_specs->expdata_x_type == ( USHORT )
                         TIME_DATA ) )
          {
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->conc_axis.axismin
                    *pplot_specs->conc_axis.mult, pplot_specs->conc_axis.axismax

                    *pplot_specs->conc_axis.mult, pplot_specs->Pexpdata_x,/* x
                                                                          data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    i,                 /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    ( msimBOOL ) FALSE,/* do not show line for ext data       */
                    ( msimBOOL ) TRUE );
          }

          DrawAxes(
               TIME_DATA, CONC_DATA,   /* type of data                        */
               instance,               /* to get units data                   */
               &pplot_specs->time_axis, &pplot_specs->conc_axis );

     }

     /* do a temperature plot if requested                                  */

     if ( pplot_specs->plottemp_v_time || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == TEMP_DATA_Y ) &&
                    ( pplot_specs->expdata_x_type == TIME_DATA ) ) )
     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plottemp_v_time )
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->temp_y_axis.axismin
                    *pplot_specs->temp_y_axis.mult,
                    pplot_specs->temp_y_axis.axismax
                    *pplot_specs->temp_y_axis.mult, pplot_specs->Ptime,/* x
                                                                          data*/
                    pplot_specs->Ptemp,/* y data                              */
                    pplot_specs->num_data_sets,/* num pts                     */
                    SIM_DATA_INTERVAL, pplot_specs->default_line_color,/*
                                                                         color*/
                    pplot_specs->default_line_style,/* linestyle              */
                    pplot_specs->default_marker_style,
                    pplot_specs->show_line, pplot_specs->show_marker );
          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         TEMP_DATA_Y ) && ( pplot_specs->expdata_x_type == TIME_DATA ) )
          {
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->temp_y_axis.axismin
                    *pplot_specs->temp_y_axis.mult,
                    pplot_specs->temp_y_axis.axismax

                    *pplot_specs->temp_y_axis.mult, pplot_specs->Pexpdata_x,/*
                                                                        x data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               TIME_DATA, TEMP_DATA,   /* type of data                        */
               instance,               /* to get units data                   */
               &pplot_specs->time_axis, &pplot_specs->temp_y_axis );
     }

     if ( pplot_specs->plotpress_v_time || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == PRESS_DATA ) &&
                    ( pplot_specs->expdata_x_type == TIME_DATA ) ) )
     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotpress_v_time )
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->press_axis.axismin
                    *pplot_specs->press_axis.mult, pplot_specs->press_axis.axismax

                    *pplot_specs->press_axis.mult, pplot_specs->Ptime,/* x
                                                                         data */
                    pplot_specs->Ppress,/* y data                             */
                    pplot_specs->num_data_sets,/* num pts                     */
                    SIM_DATA_INTERVAL, pplot_specs->default_line_color,/*
                                                                         color*/
                    pplot_specs->default_line_style,/* linestyle              */
                    pplot_specs->default_marker_style,
                    pplot_specs->show_line, pplot_specs->show_marker );

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         PRESS_DATA ) && ( pplot_specs->expdata_x_type == TIME_DATA ) )
          {
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->press_axis.axismin
                    *pplot_specs->press_axis.mult, pplot_specs->press_axis.axismax

                    *pplot_specs->press_axis.mult, pplot_specs->Pexpdata_x,/*
                                                                        x data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               ( USHORT ) TIME_DATA, ( USHORT ) PRESS_DATA,/* type of data    */
               instance,               /* to get units data                   */
               &pplot_specs->time_axis, &pplot_specs->press_axis );
     }

     if ( pplot_specs->plotvol_v_time || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == VOL_DATA ) &&
                    ( pplot_specs->expdata_x_type == TIME_DATA ) ) )
     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotvol_v_time )
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->volume_axis.axismin
                    *pplot_specs->volume_axis.mult,
                    pplot_specs->volume_axis.axismax

                    *pplot_specs->volume_axis.mult, pplot_specs->Ptime,/* x
                                                                          data*/
                    pplot_specs->Pvol, /* y data                              */
                    pplot_specs->num_data_sets,/* num pts                     */
                    SIM_DATA_INTERVAL, pplot_specs->default_line_color,/*
                                                                         color*/
                    pplot_specs->default_line_style,/* linestyle              */
                    pplot_specs->default_marker_style,
                    pplot_specs->show_line, pplot_specs->show_marker );

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         VOL_DATA ) && ( pplot_specs->expdata_x_type == TIME_DATA ) )
          {
               PlotTheData( pplot_specs->time_axis.axismin
                    *pplot_specs->time_axis.mult, pplot_specs->time_axis.axismax
                    *pplot_specs->time_axis.mult, pplot_specs->volume_axis.axismin
                    *pplot_specs->volume_axis.mult,
                    pplot_specs->volume_axis.axismax

                    *pplot_specs->volume_axis.mult, pplot_specs->Pexpdata_x,/*
                                                                        x data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               ( USHORT ) TIME_DATA, ( USHORT ) VOL_DATA,/* type of data      */
               instance,               /* to get units data                   */
               &pplot_specs->time_axis, &pplot_specs->volume_axis );
     }

     /* now do the same if any temperature plots are requested              */

     if ( pplot_specs->plotconc_v_temp || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == CONC_DATA ) &&
                    ( pplot_specs->expdata_x_type == TEMP_DATA ) ) )
     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotconc_v_temp )
          {
               USHORT i;

               for ( i = 0; i < pplot_specs->num_species_to_plot; i++ )
               {
                    PlotTheData( pplot_specs->temp_x_axis.axismin
                         *pplot_specs->temp_x_axis.mult,
                         pplot_specs->temp_x_axis.axismax
                         *pplot_specs->temp_x_axis.mult,
                         pplot_specs->conc_axis.axismin
                         *pplot_specs->conc_axis.mult,
                         pplot_specs->conc_axis.axismax

                         *pplot_specs->conc_axis.mult, pplot_specs->Ptemp,/* x
                                                                          data*/
                         pplot_specs->Pconc[i],/* y data                      */
                         pplot_specs->num_data_sets,/* num pts                */
                         SIM_DATA_INTERVAL, pplot_specs->vary_color ?/* color */
                              pplot_specs->line_color[i] :
                              pplot_specs->default_line_color,
                              pplot_specs->vary_linestyle ?/* linestyle       */
                                   pplot_specs->line_style[i] :
                                   pplot_specs->default_line_style,
                                   pplot_specs->vary_markerstyle ? pplot_specs->marker_style[i
                                        ] : pplot_specs->default_marker_style,
                                   pplot_specs->show_line, pplot_specs->show_marker );

                    WriteSpeciesLabel( pplot_specs->plotlabel[i],/* the
                                                            test to be written*/
                         i,            /* which label it is                   */
                         pplot_specs->vary_color ?/* color                    */
                              pplot_specs->line_color[i] :
                              pplot_specs->default_line_color,
                              pplot_specs->vary_linestyle ?/* linestyle       */
                                   pplot_specs->line_style[i] :
                                   pplot_specs->default_line_style,
                                   pplot_specs->vary_markerstyle ? pplot_specs->marker_style[i
                                        ] : pplot_specs->default_marker_style,
                                   pplot_specs->show_line, pplot_specs->show_marker );
               }
          }

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         CONC_DATA ) && ( pplot_specs->expdata_x_type == TEMP_DATA ) )
          {
               PlotTheData( pplot_specs->temp_x_axis.axismin
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->temp_x_axis.axismax
                    *pplot_specs->temp_x_axis.mult, pplot_specs->conc_axis.axismin
                    *pplot_specs->conc_axis.mult, pplot_specs->conc_axis.axismax

                    *pplot_specs->conc_axis.mult, pplot_specs->Pexpdata_x,/* x
                                                                          data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,/* linestyle              */
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               ( USHORT ) TEMP_DATA, ( USHORT ) CONC_DATA,/* type of data     */
               instance,               /* to get units data                   */
               &pplot_specs->temp_x_axis, &pplot_specs->conc_axis );
     }

     if ( pplot_specs->plotpress_v_temp || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == PRESS_DATA ) &&
                    ( pplot_specs->expdata_x_type == TEMP_DATA ) ) )

     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotpress_v_temp )
               PlotTheData( pplot_specs->temp_x_axis.axismin
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->temp_x_axis.axismax
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->press_axis.axismin * pplot_specs->press_axis.mult,
                    pplot_specs->press_axis.axismax * pplot_specs->press_axis.mult,

                    pplot_specs->Ptemp,/* x data                              */
                    pplot_specs->Ppress,/* y data                             */
                    pplot_specs->num_data_sets,/* num pts                     */
                    SIM_DATA_INTERVAL, pplot_specs->default_line_color,/*
                                                                         color*/
                    pplot_specs->default_line_style,
                    pplot_specs->default_marker_style,
                    pplot_specs->show_line, pplot_specs->show_marker );

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         PRESS_DATA ) && ( pplot_specs->expdata_x_type == TEMP_DATA ) )
          {
               PlotTheData( pplot_specs->temp_x_axis.axismin
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->temp_x_axis.axismax
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->press_axis.axismin * pplot_specs->press_axis.mult,
                    pplot_specs->press_axis.axismax * pplot_specs->press_axis.mult,

                    pplot_specs->Pexpdata_x,/* x data                         */
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               ( USHORT ) TEMP_DATA, ( USHORT ) PRESS_DATA,/* type of data    */
               instance,               /* to get units data                   */
               &pplot_specs->temp_x_axis, &pplot_specs->press_axis );
     }

     if ( pplot_specs->plotvol_v_temp || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == VOL_DATA ) &&
                    ( pplot_specs->expdata_x_type == TEMP_DATA ) ) )

     {
          plot_num++;
          CalcSubWindowSizeInDeviceCoords( pplot_specs->num_plots, plot_num );

          if ( pplot_specs->plotvol_v_temp )
               PlotTheData( pplot_specs->temp_x_axis.axismin
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->temp_x_axis.axismax
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->volume_axis.axismin
                    *pplot_specs->volume_axis.mult,
                    pplot_specs->volume_axis.axismax
                    *pplot_specs->volume_axis.mult, pplot_specs->Ptemp,/* x
                                                                          data*/
                    pplot_specs->Pvol, /* y data                              */
                    pplot_specs->num_data_sets,/* num pts                     */
                    SIM_DATA_INTERVAL, pplot_specs->default_line_color,/*
                                                                         color*/
                    pplot_specs->default_line_style,
                    pplot_specs->default_marker_style,
                    pplot_specs->show_line, pplot_specs->show_marker );

          if ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type ==
                         VOL_DATA ) && ( pplot_specs->expdata_x_type == TEMP_DATA ) )
          {
               PlotTheData( pplot_specs->temp_x_axis.axismin
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->temp_x_axis.axismax
                    *pplot_specs->temp_x_axis.mult,
                    pplot_specs->volume_axis.axismin
                    *pplot_specs->volume_axis.mult,
                    pplot_specs->volume_axis.axismax

                    *pplot_specs->volume_axis.mult, pplot_specs->Pexpdata_x,/*
                                                                        x data*/
                    pplot_specs->Pexpdata_y,/* y data                         */
                    pplot_specs->num_exp_data_sets,/* num pts                 */
                    EXP_DATA_INTERVAL, pplot_specs->exp_data_color,
                    pplot_specs->default_line_style,
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );

               WriteSpeciesLabel( EXP_DATA_LABEL,/* the text to be written    */
                    ( USHORT ) 0,      /* which label it is                   */
                    pplot_specs->exp_data_color,/* color                      */
                    pplot_specs->default_line_style,
                    DEFAULT_EXPDATA_MARKER_STYLE,
                    FALSE,             /* do not show line for ext data       */
                    TRUE );
          }

          DrawAxes(
               ( USHORT ) TEMP_DATA, ( USHORT ) VOL_DATA,/* type of data      */
               instance,               /* to get units data                   */
               &pplot_specs->temp_x_axis, &pplot_specs->volume_axis );
     }

     if ( pplot_specs->show_filename )
          WriteFilename( instance->base_filename );

     return;
}

void XYPlot::DrawAxes( USHORT
          XDataType, USHORT YDataType, msimPINSTANCE
          Instance, msimPAXIS XAxisData, msimPAXIS
          YAxisData )
{
     Point start_pt, end_pt;

     SHORT x_tic_length, y_tic_length;
     SHORT x_axis_length, y_axis_length;
     USHORT i;
     msimREAL_STRING mantissa;
     PCHAR label;
     SHORT x_axis_label_pos, y_axis_label_pos;
     msimFLOAT temp_result;
     SHORT nominal_x_tic_length, nominal_y_tic_length;


     xChangePen( Pen( Color( pplot_specs->axis_color ),
               pplot_specs->axis_line_width, pplot_specs->axis_line_style ) );

     xDrawRect( Rectangle( Point( lolim_x, lolim_y ), Point( hilim_x, hilim_y ) ) );

     /* now draw tic marks and numeric labels on the tic marks              */
     /* tic_length is proportional to font size                             */
     /* independent of size of viewport                                     */

     x_axis_length = hilim_x - lolim_x;
     y_axis_length = lolim_y - hilim_y;
     nominal_x_tic_length = Round( char_ht * X_TIC_LENGTH );
     nominal_y_tic_length = Round( char_width * Y_TIC_LENGTH );


     /* now draw the tic marks, construct and write the labels              */

     if ( pplot_specs->show_grid )
     {
          xChangePen( Pen( Color( pplot_specs->axis_color ),
                    pplot_specs->axis_line_width, GRID_LINESTYLE ) );
          x_tic_length = y_axis_length;
          y_tic_length = x_axis_length;
     }
     else
     {
          x_tic_length = nominal_x_tic_length;
          y_tic_length = nominal_y_tic_length;
     }

     /* first the x axis                                                    */

     xChangeFont( StdFont );

     for ( i = 0; i < XAxisData->num_ticks; i++ )
     {
          temp_result = i * XAxisData->spacing;

          sprintf( mantissa, XAxisData->tic_label_format, XAxisData->num_digits
               , XAxisData->num_dec_places, XAxisData->axismin + temp_result );

          /* draw the x tic marks                                           */

          start_pt = Point( lolim_x + Round(( temp_result / XAxisData->axisrange )
                    *x_axis_length ), lolim_y );
          end_pt = start_pt - Point( 0, x_tic_length );

          xDrawLine( start_pt, end_pt );

          start_pt += Point( 0, nominal_x_tic_length );

          x_axis_label_pos = WriteXTicLabel( start_pt, mantissa );
     }

     /* now draw the y tic marks                                       */

     for ( i = 0; i < YAxisData->num_ticks; i++ )
     {
          temp_result = i * YAxisData->spacing;

          sprintf( mantissa, YAxisData->tic_label_format, YAxisData->num_digits
               , YAxisData->num_dec_places, YAxisData->axismin + temp_result );

          start_pt = Point( lolim_x, lolim_y - Round(( temp_result / YAxisData->axisrange )
                    *y_axis_length ) );

          end_pt = start_pt + Point( y_tic_length, 0 );

          xDrawLine( start_pt, end_pt );

          start_pt -= Point( nominal_y_tic_length, 0 );

          y_axis_label_pos = WriteYTicLabel( start_pt, mantissa );
     }

     /* now draw the labels, x label first                                  */
     /* get the text depending on the data type                             */

     label = msimAxisLabelText( XAxisData->mult, Instance, XDataType, USE_LONG_NAME );

     /* now center the text below the plot and                              */
     /* set the bottom edge of the label down from the bottom of the        */
     /* x tic mark labels                                                   */

     start_pt = Point( lolim_x + ( x_axis_length - ( ( ( SHORT ) char_width ) * (( SHORT ) strlen( label ) ) ) ) / 2,
          x_axis_label_pos + nominal_x_tic_length + ( ( SHORT ) char_ht ) );

     xDrawText( start_pt, label );

     /* now the y label                                                     */
     /* get the text depending on the data type                             */

     label = msimAxisLabelText( YAxisData->mult, Instance, YDataType, true_rotated_font );

     xChangeFont( RotFont );

     if ( true_rotated_font )
     {
          /* now center the text along the left side of the plot                 */
          /* set the rightmost edge of the label over from the leftmost edge     */
          /* of the y tic mark labels                                            */
          /* note that rect.hilim_x is now the left-most point, rect.lolim_x is the        */
          /* rigthmost - this is opposite to the convention used for the         */
          /* x axis label                                                        */

          start_pt = Point( y_axis_label_pos - ( ( SHORT ) rot_char_ht ), lolim_y - ( y_axis_length -
                    ( ( ( SHORT ) strlen( label ) ) * ( ( SHORT ) rot_char_width ) ) ) / 2 );

          xDrawText( start_pt, label );

     }
     else
     {
          USHORT len = strlen ( label );
          // font is not actually composed of rotated characters -
          // start of the top and workd down vertically

          start_pt = Point( y_axis_label_pos - ( 2 * (( SHORT ) char_width ) ), hilim_y + ( y_axis_length -
                    ( ( ( SHORT ) strlen( label ) ) * ( ( SHORT ) char_ht ) ) ) / 2 );

          for ( USHORT j = 0; j < len; j++ )
          {
               xDrawText( start_pt, label, j, 1 );
               start_pt += Point( 0, char_ht );
          }

     }


     return;
}

void XYPlot::WriteFilename( PCHAR Filename )
{
     Point start_pt;

     /* center the text at the top of the window  and                       */
     /* set the top edge of the label char height down from                 */
     /* top of window                                                       */

     xChangeFont( StdFont );

     start_pt = Point(( window_width - char_width * strlen( Filename ) ) / 2,
          ( ( 30 * char_ht ) / 20 ) );

     xDrawText( start_pt, String( Filename ) );

     return;
}

void XYPlot::PlotTheData( msimFLOAT MinX, msimFLOAT MaxX, msimFLOAT
          MinY, msimFLOAT MaxY, msimPFLOAT Xdata,
          msimPFLOAT Ydata, USHORT NumPoints,
          USHORT Interval, enum ColorName
          PlotColor, enum PenStyle LineStyle, enum
          MARKER_STYLE MarkerStyle,
          msimBOOL
          ShowLines, msimBOOL ShowMarkers )
{

     Point PTR plot_array;

     /* first allocate memory for the array of points                       */

     plot_array = new Point[ ( NumPoints / Interval + 1 ) + msimEXTRA_PLOT_ARRAY_PTS];

     if ( ! plot_array )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, owner );
          return;
     }

     /* now set graphics parameters as requested                            */

     /* we use this to indicate that points rather than lines are to be draw*/

     if ( ShowMarkers )
     {
          USHORT points_to_plot =
               msimScalePointsForPlotting( Xdata, Ydata, plot_array, NumPoints,
               Interval, MinX, MaxX, MinY, MaxY, lolim_x, hilim_x, lolim_y, hilim_y,
               NumPoints + msimEXTRA_PLOT_ARRAY_PTS );

          SetMarker( MarkerStyle, PlotColor );

          if ( points_to_plot <= POLYLINE_LIMIT )
               DrawPolyMarker( Polygon( points_to_plot, plot_array ) );
          else
          {
               USHORT j;
               Point pt;
               Point PTR ptr;

               ptr = plot_array;

               for ( j = 0; j < ( points_to_plot ); j++ )
               {
                    pt = *ptr++;
                    DrawMarker( pt );
               }
          }
     }

     if ( ShowLines )
     {
          USHORT points_to_plot = msimScaleDataForPlotting( Xdata, Ydata, plot_array, NumPoints,
               Interval, MinX, MaxX, MinY, MaxY, lolim_x, hilim_x, lolim_y, hilim_y,
               NumPoints + msimEXTRA_PLOT_ARRAY_PTS );

          xChangePen( Pen( Color( PlotColor ), DEFAULT_LINEWIDTH, LineStyle ) );

          if ( NumPoints <= POLYLINE_LIMIT )
               xDrawPolyLine( Polygon( points_to_plot, plot_array ) );
          else
          {
               USHORT j;
               Point src, dest;
               Point PTR ptr;

               ptr = plot_array;

               for ( j = 0; j < ( points_to_plot - 1 ); j++ )
               {
                    src = *ptr;
                    ptr++;
                    dest = *ptr;

                    xDrawLine( src, dest );
               }
          }
     }

     delete[]plot_array;
     return;

}

/*--------------------------------------------------------------------------*/
/*                        WriteSpeciesLabel()                               */
/*..........................................................................*/
/*                                                                          */
/* this function draws the name of a species as specified in Text, using    */
/* Color. Then a line is dranwn under the name, using Color and Linestyle.  */
/* This fcn is intended to be used to labal an x-y plot of concentration    */
/* vs time/temperature so that the different traces on theplot can be       */
/* distinguished. The legened are drawn on the right side of the plot       */
/* with each entry in the legend below the one before it. The position      */
/* in this ordering is determined by LabelNum, with increasing values       */
/* leading to a lower position. The function optionally draws the lines,    */
/* this is a flag that the data is externally loaded data which is          */
/* markers, or both points and markers depending on the setting of the      */
/* switches ShowLines and ShowMarkers                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void XYPlot::WriteSpeciesLabel( PCHAR Text, USHORT LabelNum, enum
          ColorName PlotColor, enum
          PenStyle LineStyle, enum
          MARKER_STYLE MarkerStyle,
          msimBOOL ShowLines, msimBOOL
          ShowMarkers )
{
     Pen line_attributes;
     Point start, end, str_pos, middle;

     /* set graphics parameters as requested                                */
     xChangeFont( StdFont );

     /* 2 spaces over from plot                                              */
     /* and move downward 2 lines from previous entry in legend             */

     str_pos = Point( hilim_x + ( ( 30 * char_width ) / 20 ), hilim_y + ( char_ht / 2 ) + ( LabelNum * 3 * char_ht ) );

     start = str_pos + Point( 0, char_ht );

     end = start + Point(( msimLENGTH_OF_NAME * char_width ), 0 );

     if ( ShowMarkers )
     {
          SetMarker( MarkerStyle, PlotColor );

          middle = Point(( start.X( ) + end.X( ) ) / 2, start.Y( ) );

          DrawMarker( start );
          DrawMarker( middle );
          DrawMarker( end );
     }


     if ( ShowLines )
     {
          xChangePen( Pen( Color( PlotColor ), SCREEN_DEFAULT_PEN_WIDTH, LineStyle ) );
          xDrawLine( start, end );
     }

     xDrawText( str_pos, String( Text ) );

     return;
}

/*--------------------------------------------------------------------------*/
/*                        WriteXTicLabel()                                  */
/*..........................................................................*/
/*                                                                          */
/* this function draws a text string near and below an x tic mark           */
/* whose start position is passed as start_pt. the text string is centered  */
/* below the point and the upper edge of string touches the point           */
/* returns the farthest-down extent of the label. If the Tic makr being     */
/* labeled is the last one then we cat the exponent of the ScalingFactor    */
/* to the string                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

SHORT XYPlot::WriteXTicLabel( Point& pt, PCHAR mantissa )
{
     // strip any leading blanks
     while ( *mantissa == ' ' )
          mantissa++;

     pt -= Point(( strlen( mantissa ) *char_width ) / 2, - char_ht );

     xDrawText( pt, String( mantissa ) );

     return pt.Y( );
}

/*--------------------------------------------------------------------------*/
/*                        WriteYTicLabel()                                  */
/*..........................................................................*/
/*                                                                          */
/* this function draws a text string to the left of a y tic mark            */
/* whose start position is passed as start_pt. the text string is centered  */
/* to the left of the point and the right edge of string touches the point  */
/* returns the farthest-left extent of the label                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

SHORT XYPlot::WriteYTicLabel( Point& pt, PCHAR mantissa )
{
     // strip any leading blanks
//     while( *mantissa == ' ')
//          mantissa++;

     pt -= Point( strlen( mantissa ) *char_width, ( -10 * char_ht ) / 20 );

     xDrawText( pt, String( mantissa ) );

     return pt.X( );
}



enum msimCOLORSET_TYPE XYPlot::ChangeColorSet( enum msimCOLORSET_TYPE NewColorSet )
{
     enum msimCOLORSET_TYPE old_colorset = pplot_specs->current_colorset;
     pplot_specs->background_color = ColorSet[NewColorSet].background_color;
     pplot_specs->axis_color = ColorSet[NewColorSet].axis_color;
     pplot_specs->default_line_color = ColorSet[NewColorSet].default_line_color;
     pplot_specs->exp_data_color = ColorSet[NewColorSet].exp_data_color;

     for ( USHORT i = 0; i <= MAX_NO_OF_PLOTS; i++ )
          pplot_specs->line_color[i] = ColorSet[NewColorSet].line_color[i];

     pplot_specs->current_colorset = NewColorSet;

     return old_colorset;
}


