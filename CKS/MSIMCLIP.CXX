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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimclip.cxx                                                */
/*                                                                          */
/*  This module  contains functions used for creating plots of simulation   */
/*  results . It is a companion file to msimplot.cxx                        */
/*                                                                          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started August 23, 1993                                    */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimplot.hxx"
#include "msimfile.hxx"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>



#if defined(__MAC__) && defined(__PPC__)
#include <sysdep.hxx>
#endif


#define MIN_RANGE    ( (msimFLOAT) 1.0e-5)

static msimBOOL PointWithinBounds( msimFLOAT X, msimFLOAT Y, msimFLOAT XMin,
                     msimFLOAT XMax, msimFLOAT YMin, msimFLOAT
                     YMax );

static msimBOOL AnyConcPointsWithinBounds( msimFLOAT X, msimPFLOAT ConcPtrArray
                     [], USHORT NumSpeciesToPlot,
                     msimFLOAT XMin, msimFLOAT XMax,
                     msimFLOAT MinConc, msimFLOAT MaxConc
                );


msimBOOL VisibleLineSegment( msimFLOAT& X1, msimFLOAT& Y1, msimFLOAT& X2,
                             msimFLOAT& Y2, USHORT AreaCode1, USHORT AreaCode2,
                             msimFLOAT MinX, msimFLOAT MinY, msimFLOAT MaxX,
                             msimFLOAT MaxY);

#define  LEFT_OF_WINDOW                          (1<<1)
#define  RIGHT_OF_WINDOW                         (1<<2)
#define  BELOW_WINDOW                            (1<<3)
#define  ABOVE_WINDOW                            (1<<4)

/*--------------------------------------------------------------------------*/
/*                     msimScaleDataForPlotting()                           */
/*..........................................................................*/
/*                                                                          */
/* this function takes as paramters arrays of x and y values as             */
/* msimFLOATS. The data is first clipped and then                           */
/* is scaled and converted into screen coordinates                          */
/* which can be used to produce the x-y plot of the data on the screen.     */
/* This fcn is based on a Cohen-Sutherland type clipping algorithm          */
/* ref Hearn & Baker, "Computer Graphics", Prentice Hall 1986, pp 128-134   */
/*                                                                          */
/* The screen coordinates are returned in the array of Point                */
/* pointed to by the parameter Points. The value returned by the function   */
/* is the number of valid points found and contained in the array Points    */
/*                                                                          */
/* This function is strictly intended for plotting a curve comprised of     */
/* a series of line segments representing the x,y data. To plot only actual */
/* data points within the plot limit, call the function                     */
/* msimScalePointsForPlotting in this module                                */
/*--------------------------------------------------------------------------*/

USHORT msimScaleDataForPlotting( msimPFLOAT X, msimPFLOAT Y, Point Points[],
            USHORT TotalNumPts, USHORT Interval, msimFLOAT
            MinX, msimFLOAT MaxX, msimFLOAT MinY, msimFLOAT
            MaxY, SHORT LoLimX, SHORT HiLimX, SHORT LoLimY,
            SHORT HiLimY, USHORT ArraySize )
{
     msimFLOAT x_scaling_factor, y_scaling_factor;
     USHORT area_code1, area_code2;
     USHORT valid_pts = 0;
     msimPFLOAT p_last_pt_x = X + TotalNumPts - 1;
     msimPFLOAT p_last_pt_y = Y + TotalNumPts - 1;

     /* following are used in the clipping algorithm  */

     msimFLOAT  x1, x2, y1, y2;

     x_scaling_factor = ( HiLimX - LoLimX ) / ( MaxX - MinX );
     y_scaling_factor = ( HiLimY - LoLimY ) / ( MaxY - MinY );

     /* calc init value for area_code1                                      */

     area_code1 = 0;

     if ( *X < MinX )
          area_code1 = LEFT_OF_WINDOW;
     else
          if ( *X > MaxX )

               area_code1 = RIGHT_OF_WINDOW;

     if ( *Y < MinY )
          area_code1 |= BELOW_WINDOW;
     else
          if ( *Y > MaxY )

               area_code1 |= ABOVE_WINDOW;

     /* calc init value for area_code2                                      */

     area_code2 = 0;

     if ( * ( X + Interval ) < MinX )
          area_code2 = LEFT_OF_WINDOW;
     else
          if ( * ( X + Interval ) > MaxX )

               area_code2 = RIGHT_OF_WINDOW;

     if ( * ( Y + Interval ) < MinY )
          area_code2 |= BELOW_WINDOW;
     else
          if ( * ( Y + Interval ) > MaxY )
               area_code2 |= ABOVE_WINDOW;

     /* test for trivial case where both points are within window           */
     /* ie where area_code1 == area_code2 == 0                              */

     if ( ! ( area_code1 | area_code2 ) )
     {

          /* accept the line segment                                        */
          /* calculate the position on the viewport from data               */
          /* for the first point ( X still pts to first pt )                */

          *Points = Point( Round(( *X - MinX ) *x_scaling_factor ) + LoLimX, Round(( *Y -
                         MinY ) *y_scaling_factor ) + LoLimY );
          Points++;

          /* and for the second                                             */

          X += Interval;
          Y += Interval;
          *Points = Point( Round(( *X - MinX ) *x_scaling_factor ) + LoLimX, Round(( *Y -
                         MinY ) *y_scaling_factor ) + LoLimY );
          Points++;

          /* keep track of number of valid points found                     */

          valid_pts = 2;

     }
     else
     {

          /* if not accepted then test for any matching bits                */
          /* if any are found then the line segment connecting the          */
          /* points will not pass through the viewport, so we               */
          /* reject the segment and move onto next segment                  */

          if ( area_code1&area_code2 )
          {
               X += Interval;
               Y += Interval;
          }
          else
          {
               /* if we made it to this part of the function then           */
               /* we need to search for an intersection of the line         */
               /* between the points and the viewport if any                */

               x1 = *X;

               X += Interval;
               x2 = *X;

               y1 = *Y;

               Y += Interval;
               y2 = *Y;

               /* we must test whether there is a portion of  */
               /* the line segment visible */

               if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2,
                              MinX, MinY, MaxX, MaxY) )
               {
                    /* now calculate and save the points in GC coordinates  */

                    *Points = Point( Round(( x1 - MinX ) *x_scaling_factor ) + LoLimX,
                         Round(( y1 - MinY ) *y_scaling_factor ) + LoLimY );
                    Points++;

                    *Points = Point( Round(( x2 - MinX ) *x_scaling_factor ) + LoLimX,
                         Round(( y2 - MinY ) *y_scaling_factor ) + LoLimY );
                    Points++;

                    /* keep track of number of valid points found           */

                    valid_pts = 2;

               }

          }
     }

     /* do the same process through the rest of the points                  */
     /* substract 2 from ArraySize since  we may add two members            */
     /* to the array during the loop and we do not want to over-run         */
     /* the memory allocated for the Points Array                           */
     /* Allocation of Points array should add in some extra points          */
     /* above those absolutely necessary since the clipping process         */
     /* can actually add in some members. In principle the number of        */
     /* valid_pts can exceed the TotalNumPts though this is not             */
     /* likely. It wouls only occur when there are many lines clipped       */
     /* and where only one point was excluded during the clipping action    */
     /* but two point were added at the axis for the line segments going    */
     /* up to , and down from the excluded point                            */
     /* we test for over-running the array size at any rate                 */

     ArraySize -= 2;

     TotalNumPts /= Interval;

     while ( ( X + Interval ) < p_last_pt_x && valid_pts <= ArraySize )
     {

          /* 2nd point will become first point in next line seg             */
          /* so we can save the area_code already calculated                */

          area_code1 = area_code2;

          /* calc init value for area_code2                                 */

          area_code2 = 0;
          X += Interval;
          if ( *X < MinX )
               area_code2 = LEFT_OF_WINDOW;
          else
               if ( *X > MaxX )

                    area_code2 = RIGHT_OF_WINDOW;

          Y += Interval;
          if ( *Y < MinY )
               area_code2 |= BELOW_WINDOW;
          else
               if ( *Y > MaxY )

                    area_code2 |= ABOVE_WINDOW;

          /* test for trivial case where both points are within window      */
          /* ie where area_code1 == area_code2 == 0                         */

          if ( ! ( area_code1 | area_code2 ) )
          {

               /* accept the line segment                                   */
               /* calculate the position on the viewport from data          */

               *Points = Point( Round(( *X - MinX ) *x_scaling_factor ) + LoLimX, Round
                    (( *Y - MinY ) *y_scaling_factor ) + LoLimY );
               Points++;

               /* keep track of number of valid points found                */

               valid_pts++;

               /* move to next element in Points struct array               */

               continue;
          }

          /* if not accepted then test for any matching bits                */
          /* if any are found then the line segment connecting the          */
          /* points will not pass through the viewport, so we               */
          /* reject the segment and move onto next segment                  */

          if ( area_code1&area_code2 )
          {
               continue;
          }

          /* if we made it to this part of the function then                */
          /* we need to search for an intersection of the line              */
          /* between the points and the viewport                            */

          // save the x,y pairs as temporary variables as their values
          // may be modified during the clipping operation contained
          // in fcn VisibleLineSegment

          x1 = * ( X - Interval );
          y1 = * ( Y - Interval );
          y2 = *Y;
          x2 = *X;

          if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2,
                              MinX, MinY, MaxX, MaxY) )
          {
               /* now calculate and save the points in GC coordinates       */

               *Points = Point( Round(( x1 - MinX ) *x_scaling_factor ) + LoLimX, Round
                    (( y1 - MinY ) *y_scaling_factor ) + LoLimY );
               Points++;

               *Points = Point( Round(( x2 - MinX ) *x_scaling_factor ) + LoLimX, Round
                    (( y2 - MinY ) *y_scaling_factor ) + LoLimY );
               Points++;

               /* keep track of number of valid points found                */

               valid_pts += 2;

          }

          continue;

     }                                 /* end while                           */

     // now catch the last point in case we did not get it in the above processing

     if ( valid_pts <= ArraySize )
     {

          /* 2nd point will become first point in next line seg             */
          /* so we can save the area_code already calculated                */

          area_code1 = area_code2;
          /* calc init value for area_code2                                 */

          area_code2 = 0;

          if ( *p_last_pt_x < MinX )
               area_code2 = LEFT_OF_WINDOW;
          else
               if ( *p_last_pt_x > MaxX )

                    area_code2 = RIGHT_OF_WINDOW;

          if ( *p_last_pt_y < MinY )
               area_code2 |= BELOW_WINDOW;
          else
               if ( *p_last_pt_y > MaxY )

                    area_code2 |= ABOVE_WINDOW;

          /* test for trivial case where both points are within window      */
          /* ie where area_code1 == area_code2 == 0                         */

          if ( ! ( area_code1 | area_code2 ) )
          {

               /* accept the line segment                                   */
               /* calculate the position on the viewport from data          */

               *Points = Point( Round(( *p_last_pt_x - MinX ) *x_scaling_factor ) + LoLimX, Round
                    (( *p_last_pt_y - MinY ) *y_scaling_factor ) + LoLimY );

               /* keep track of number of valid points found                */

               valid_pts++;
          }
          else
          {
               /* if not accepted then test for any matching bits                */
               /* if any are found then the line segment connecting the          */
               /* points will not pass through the viewport, so we               */
               /* reject the segment and move onto next segment                  */

               if ( area_code1&area_code2 )
                    ;                  // reject the segment
               else
               {
                    /* if we made it to this part of the function then                */
                    /* we need to search for an intersection of the line              */
                    /* between the points and the viewport                            */
                    x1 = *X;           // use the previous  pt
                    y1 = *Y;
                    y2 = *p_last_pt_y;
                    x2 = *p_last_pt_x;

                    if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2,
                              MinX, MinY, MaxX, MaxY) )
                    {

                         /* now calculate and save the points in GC coordinates       */

                         *Points = Point( Round(( x1 - MinX ) *x_scaling_factor ) + LoLimX, Round
                              (( y1 - MinY ) *y_scaling_factor ) + LoLimY );
                         Points++;

                         *Points = Point( Round(( x2 - MinX ) *x_scaling_factor ) + LoLimX, Round
                              (( y2 - MinY ) *y_scaling_factor ) + LoLimY );

                         /* keep track of number of valid points found                */

                         valid_pts += 2;

                    }
               }                       // end else
          }
     }
     return valid_pts;
}


/*--------------------------------------------------------------------------*/
/*                        msimWritePlotDataToFile( )                        */
/*..........................................................................*/
/*                                                                          */
/* This function takes the defined data plot and prints it to an ASCII      */
/* file instead of the the screen                                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimWritePlotDataToFile( PPLOT_SPECS PlotSpecs, msimPINSTANCE Instance,
            msimWID Owner )
{
     msimSTRING tempstr1, tempstr2, tempstr3;
     FILE *f;
     struct tm          *current_time;
     time_t local_time;
     ULONG i, j;
     USHORT line_length;
     PCHAR ptr1;
     msimPFLOAT ptime, pvol, ppress, ptemp;
     msimPFLOAT pconc[MAX_NO_OF_PLOTS];
     msimBOOL print_current_line;
     msimBOOL printing_time, printing_conc, printing_temp;
     msimBOOL printing_press, printing_vol;
     msimBOOL final_pass = FALSE;

     USHORT data_interval;

#ifdef USE_VAR_DATA_INTERVAL
     if ( PlotSpecs->num_data_sets <= PlotSpecs->max_pts_to_plot )
          data_interval = 1;
     else
     {
          data_interval = PlotSpecs->num_data_sets / PlotSpecs->max_pts_to_plot;
          if ( PlotSpecs->num_data_sets % PlotSpecs->max_pts_to_plot )// is there a remainder
               data_interval++;
     }
#else
     data_interval = 1;
#endif

     /* go ahead and open the file                                          */

     if ( NULL == ( f = fopen( Instance->print_target, "w" ) ) )
     {
          msimFileError( Instance->print_target, Owner, ( USHORT )
               msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     /* get the addresses to the various arrays and save them in            */
     /* local copies so we can modify the pointer values without damage     */

     ptime = PlotSpecs->Ptime;
     pvol = PlotSpecs->Pvol;
     ppress = PlotSpecs->Ppress;
     ptemp = PlotSpecs->Ptemp;

     for ( i = 0; i < MAX_NO_OF_PLOTS; i++ )
          pconc[i] = PlotSpecs->Pconc[i];

     /* set some BOOLEAN flags                                              */

     printing_time = ( PlotSpecs->plotconc_v_time || PlotSpecs->
          plotpress_v_time || PlotSpecs->plotvol_v_time || PlotSpecs->
          plottemp_v_time );
     printing_temp = ( PlotSpecs->plottemp_v_time || PlotSpecs->
          plotpress_v_temp || PlotSpecs->plotconc_v_temp || PlotSpecs->
          plotvol_v_temp );
     printing_vol = ( PlotSpecs->plotvol_v_time || PlotSpecs->plotvol_v_temp );
     printing_press = ( PlotSpecs->plotpress_v_time || PlotSpecs->
          plotpress_v_temp );

     printing_conc = ( PlotSpecs->plotconc_v_time || PlotSpecs->plotconc_v_temp
     );

     /* now construct the header to the file                                */

     fprintf( f, HEADER_STR_1, Instance->filename );
     time( &local_time );              /* get time in seconds                 */
     current_time = localtime( &local_time );
     fprintf( f, HEADER_STR_2, asctime( current_time ) );

     /* now construct the table heading                                     */
     /* this layout assumes an allotment of 12 spaces per column            */
     /* the header has two lines. Both lines are constructed simultaneously */
     /* tempstr1 contains first line, tempstr2 contains the second          */

     msimStringCopy( tempstr1, msimNULL_STR, sizeof tempstr1 );
     msimStringCopy( tempstr2, msimNULL_STR, sizeof tempstr2 );
     if ( printing_time )
     {
          sprintf( tempstr3, FORMAT_STR, strlen( TIME_STR ), TIME_STR );
          msimStringCat( tempstr1, tempstr3, sizeof tempstr1 );
          sprintf( tempstr3, FORMAT_STR, strlen( msimTimeUnits( Instance->
                         time_units ) ), msimTimeUnits( Instance->time_units ) );
          msimStringCat( tempstr2, tempstr3, sizeof tempstr2 );
     }

     /* do we need to print temperature ?                                   */

     if ( printing_temp )
     {
          sprintf( tempstr3, FORMAT_STR, strlen( TEMPERATURE_STR ),
               TEMPERATURE_STR );
          msimStringCat( tempstr1, tempstr3, sizeof tempstr1 );
          sprintf( tempstr3, FORMAT_STR, strlen( TEMP_UNITS_STR ), TEMP_UNITS_STR
          );
          msimStringCat( tempstr2, tempstr3, sizeof tempstr2 );
     }
     if ( printing_press )
     {
          sprintf( tempstr3, FORMAT_STR, strlen( PRESSURE_STR ), PRESSURE_STR );
          msimStringCat( tempstr1, tempstr3, sizeof tempstr1 );
          sprintf( tempstr3, FORMAT_STR, strlen( msimPressureUnits( Instance->
                         pressure_units ) ), msimPressureUnits( Instance->pressure_units ) );
          msimStringCat( tempstr2, tempstr3, sizeof tempstr2 );
     }
     if ( printing_vol )
     {
          sprintf( tempstr3, FORMAT_STR, strlen( VOLUME_STR ), VOLUME_STR );
          msimStringCat( tempstr1, tempstr3, sizeof tempstr1 );

          sprintf( tempstr3, FORMAT_STR, strlen( msimVolumeUnits( Instance->
                         volume_units ) ), msimVolumeUnits( Instance->volume_units ) );
          msimStringCat( tempstr2, tempstr3, sizeof tempstr2 );
     }

     /* do we need to print concentration data ?                            */

     if ( printing_conc )
          for ( i = 0; i < PlotSpecs->num_species_to_plot; i++ )
          {
               sprintf( tempstr3, FORMAT_STR, msimLENGTH_OF_NAME, PlotSpecs->
                    plotlabel[i] );
               msimStringCat( tempstr1, tempstr3, sizeof tempstr1 );
               sprintf( tempstr3, FORMAT_STR, strlen( msimConcUnits( Instance->
                              conc_units, Instance->volume_option ) ), msimConcUnits( Instance
                    ->conc_units, Instance->volume_option ) );
               msimStringCat( tempstr2, tempstr3, sizeof tempstr2 );
          }

     /* out to file                                                         */

     fprintf( f, "%s\n%s\n", tempstr1, tempstr2 );

     /* print a separator whose length is equal to the length of the heading*/

     line_length = ( USHORT ) strlen( tempstr1 );
     ptr1 = tempstr1;
     for ( i = 0; i < line_length; i++ )
          *ptr1++= SEPARATOR_CHAR;

     /* zero-terminate and print                                            */

     *ptr1 = '\0';
     fprintf( f, "%s\n", tempstr1 );

     /* now write out the numeric data in ASCII format                      */
     /* we need to check during this that we only print the                 */
     /* data that was on the plot,paying attention to the limits            */

     for ( j = 0; j < PlotSpecs->num_data_sets; )
     {

          /* initialize                                                     */

          msimStringCopy( tempstr1, msimNULL_STR, sizeof tempstr1 );
          print_current_line = FALSE;

          /* do we need to print an entry for the current time ?            */

          if ( printing_time )
          {

               /* if so, then figure out whether it would actually be plotted*/
               /* if we were creating a plot                                */

               if ( PlotSpecs->plotconc_v_time && AnyConcPointsWithinBounds
                         ( *ptime, pconc, PlotSpecs->num_species_to_plot, PlotSpecs->
                              time_axis.axismin * PlotSpecs->time_axis.mult, PlotSpecs->
                              time_axis.axismax * PlotSpecs->time_axis.mult, PlotSpecs->
                              conc_axis.axismin * PlotSpecs->conc_axis.mult, PlotSpecs->
                              conc_axis.axismax * PlotSpecs->conc_axis.mult ) || PlotSpecs->
                         plotpress_v_time && PointWithinBounds( *ptime, *ppress,
                              PlotSpecs->time_axis.axismin * PlotSpecs->time_axis.mult,
                              PlotSpecs->time_axis.axismax * PlotSpecs->time_axis.mult,
                              PlotSpecs->press_axis.axismin * PlotSpecs->press_axis.mult,
                              PlotSpecs->press_axis.axismax * PlotSpecs->press_axis.mult ) ||
                         PlotSpecs->plotvol_v_time && PointWithinBounds( *ptime, *pvol,
                              PlotSpecs->time_axis.axismin * PlotSpecs->time_axis.mult,
                              PlotSpecs->time_axis.axismax * PlotSpecs->time_axis.mult,
                              PlotSpecs->volume_axis.axismin * PlotSpecs->volume_axis.mult,
                              PlotSpecs->volume_axis.axismax * PlotSpecs->volume_axis.mult )
                         || PlotSpecs->plottemp_v_time && PointWithinBounds( *ptime,
                              *ptemp, PlotSpecs->time_axis.axismin * PlotSpecs->
                              time_axis.mult, PlotSpecs->time_axis.axismax * PlotSpecs->
                              time_axis.mult, PlotSpecs->temp_y_axis.axismin * PlotSpecs->
                              temp_y_axis.mult, PlotSpecs->temp_y_axis.axismax * PlotSpecs->
                              temp_y_axis.mult ) )
               {
                    sprintf( tempstr2, NUMERIC_FORMAT_STR,
                         NUM_DEC_PLACES_IN_TABLE, *ptime );
                    msimStringCat( tempstr1, tempstr2, sizeof tempstr1 );
                    print_current_line = TRUE;/* set flag since we have
                                                 added valid data             */
               }
               else

                    /* otherwise we are not in the plot range - put in a place holder*/

                    msimStringCat( tempstr1, INVALID_DATA_STR, sizeof tempstr1 );
          }

          if ( printing_temp )
          {

               /* if so, then figure out whether it would actually be plotted*/
               /* if we were creating a plot                                */

               if ( PlotSpecs->plottemp_v_time && PointWithinBounds( *ptime,
                              *ptemp, PlotSpecs->time_axis.axismin * PlotSpecs->
                              time_axis.mult, PlotSpecs->time_axis.axismax * PlotSpecs->
                              time_axis.mult, PlotSpecs->temp_y_axis.axismin * PlotSpecs->
                              temp_y_axis.mult, PlotSpecs->temp_y_axis.axismax * PlotSpecs->
                              temp_y_axis.mult ) || PlotSpecs->plotpress_v_temp &&
                         PointWithinBounds( *ptemp, *ppress, PlotSpecs->
                              temp_x_axis.axismin * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              temp_x_axis.axismax * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              press_axis.axismin * PlotSpecs->press_axis.mult, PlotSpecs->
                              press_axis.axismax * PlotSpecs->press_axis.mult ) || PlotSpecs->
                         plotvol_v_temp && PointWithinBounds( *ptemp, *pvol, PlotSpecs->
                              temp_x_axis.axismin * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              temp_x_axis.axismax * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              volume_axis.axismin * PlotSpecs->volume_axis.mult, PlotSpecs->
                              volume_axis.axismax * PlotSpecs->volume_axis.mult ) || PlotSpecs
                         ->plotconc_v_temp && AnyConcPointsWithinBounds( *ptemp, pconc,
                              PlotSpecs->num_species_to_plot, PlotSpecs->temp_x_axis.axismin
                              *PlotSpecs->temp_x_axis.mult, PlotSpecs->temp_x_axis.axismax
                              *PlotSpecs->temp_x_axis.mult, PlotSpecs->conc_axis.axismin
                              *PlotSpecs->conc_axis.mult, PlotSpecs->conc_axis.axismax
                              *PlotSpecs->conc_axis.mult ) )

               {
                    sprintf( tempstr2, NUMERIC_FORMAT_STR,
                         NUM_DEC_PLACES_IN_TABLE, *ptemp );
                    msimStringCat( tempstr1, tempstr2, sizeof tempstr1 );
                    print_current_line = TRUE;/* set flag since we have
                                                 added valid data             */
               }
               else

                    /* otherwise we are not in the plot range - put in a place holder*/

                    msimStringCat( tempstr1, INVALID_DATA_STR, sizeof tempstr1 );
          }
          if ( printing_press )
          {
               if ( PlotSpecs->plotpress_v_time && PointWithinBounds( *ptime,
                              *ppress, PlotSpecs->time_axis.axismin * PlotSpecs->
                              time_axis.mult, PlotSpecs->time_axis.axismax * PlotSpecs->
                              time_axis.mult, PlotSpecs->press_axis.axismin * PlotSpecs->
                              press_axis.mult, PlotSpecs->press_axis.axismax * PlotSpecs->
                              press_axis.mult ) || PlotSpecs->plotpress_v_temp &&
                         PointWithinBounds( *ptemp, *ppress, PlotSpecs->
                              temp_x_axis.axismin * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              temp_x_axis.axismax * PlotSpecs->temp_x_axis.mult, PlotSpecs->
                              press_axis.axismin * PlotSpecs->press_axis.mult, PlotSpecs->
                              press_axis.axismax * PlotSpecs->press_axis.mult ) )
               {
                    sprintf( tempstr2, NUMERIC_FORMAT_STR,
                         NUM_DEC_PLACES_IN_TABLE, *ppress );
                    msimStringCat( tempstr1, tempstr2, sizeof tempstr1 );
                    print_current_line = TRUE;/* set flag since we have
                                                 added valid data             */
               }
               else

                    /* otherwise we are not in the plot range - put in a place holder*/

                    msimStringCat( tempstr1, INVALID_DATA_STR, sizeof tempstr1 );
          }
          if ( printing_vol )
          {
               if ( PlotSpecs->plotvol_v_time && PointWithinBounds( *ptime,
                              *pvol, PlotSpecs->time_axis.axismin * PlotSpecs->time_axis.mult
                              , PlotSpecs->time_axis.axismax * PlotSpecs->time_axis.mult,
                              PlotSpecs->volume_axis.axismin * PlotSpecs->volume_axis.mult,
                              PlotSpecs->volume_axis.axismax * PlotSpecs->volume_axis.mult )
                         || PlotSpecs->plotvol_v_temp && PointWithinBounds( *ptemp,
                              *pvol, PlotSpecs->temp_x_axis.axismin * PlotSpecs->
                              temp_x_axis.mult, PlotSpecs->temp_x_axis.axismax * PlotSpecs->
                              temp_x_axis.mult, PlotSpecs->volume_axis.axismin * PlotSpecs->
                              volume_axis.mult, PlotSpecs->volume_axis.axismax * PlotSpecs->
                              volume_axis.mult ) )
               {
                    sprintf( tempstr2, NUMERIC_FORMAT_STR,
                         NUM_DEC_PLACES_IN_TABLE, *pvol );
                    msimStringCat( tempstr1, tempstr2, sizeof tempstr1 );
                    print_current_line = TRUE;/* set flag since we have
                                                 added valid data             */
               }
               else

                    /* otherwise we are not in the plot range - put in a place holder*/

                    msimStringCat( tempstr1, INVALID_DATA_STR, sizeof tempstr1 );
          }

          /* do we need to print concentration data ?                       */

          if ( printing_conc )
          {
               for ( i = 0; i < PlotSpecs->num_species_to_plot; i++ )
               {
                    if ( PlotSpecs->plotconc_v_time && PointWithinBounds( *ptime
                                   , *pconc[i], PlotSpecs->time_axis.axismin * PlotSpecs->
                                   time_axis.mult, PlotSpecs->time_axis.axismax * PlotSpecs->
                                   time_axis.mult, PlotSpecs->conc_axis.axismin * PlotSpecs->
                                   conc_axis.mult, PlotSpecs->conc_axis.axismax * PlotSpecs->
                                   conc_axis.mult ) || PlotSpecs->plotconc_v_temp &&
                              PointWithinBounds( *ptemp, *pconc[i], PlotSpecs->
                                   temp_x_axis.axismin * PlotSpecs->temp_x_axis.mult,
                                   PlotSpecs->temp_x_axis.axismax * PlotSpecs->
                                   temp_x_axis.mult, PlotSpecs->conc_axis.axismin * PlotSpecs
                                   ->conc_axis.mult, PlotSpecs->conc_axis.axismax * PlotSpecs
                              ->conc_axis.mult ) )
                    {
                         sprintf( tempstr2, NUMERIC_FORMAT_STR,
                              NUM_DEC_PLACES_IN_TABLE, *pconc[i] );
                         msimStringCat( tempstr1, tempstr2, sizeof tempstr1 );
                         print_current_line = TRUE;/* set flag since we
                                                      have added valid data   */
                    }
                    else

                         /* otherwise we are not in the plot range - put in a place holder*/

                         msimStringCat( tempstr1, INVALID_DATA_STR, sizeof tempstr1 );
               }                       /* end for i...                        */
          }
          if ( print_current_line )
               fprintf( f, "%s\n", tempstr1 );

          /* increment pointers before doing the next round                 */

          // will the next pass make us go past num_data_sets ?
          // if so make a pass through loop on last pt

          if ( j + data_interval > PlotSpecs->num_data_sets && ! final_pass )
          {
               final_pass = TRUE;

               j = PlotSpecs->num_data_sets - 1;

               if ( printing_time )
                    ptime = PlotSpecs->Ptime + j;

               if ( printing_temp )
                    ptemp = PlotSpecs->Ptemp + j;

               if ( printing_press )
                    ppress = PlotSpecs->Ppress + j;

               if ( printing_vol )
                    pvol = PlotSpecs->Pvol + j;

               if ( printing_conc )
                    for ( i = 0; i < PlotSpecs->num_species_to_plot; i++ )
                         pconc[i] = PlotSpecs->Pconc[i] + j;

          }
          else
          {

               if ( printing_time )
                    ptime += data_interval;

               if ( printing_temp )
                    ptemp += data_interval;

               if ( printing_press )
                    ppress += data_interval;

               if ( printing_vol )
                    pvol += data_interval;

               if ( printing_conc )
                    for ( i = 0; i < PlotSpecs->num_species_to_plot; i++ )
                         pconc[i] += data_interval;

               j += data_interval;
          }

     }

     /* check for errors and notify if any are found                        */

     if ( ferror( f ) )
     {
          fclose( f );
          return msimWRITE_ERROR;
     }

     fclose( f );
	 
#if defined(__MAC__) && defined(__PPC__)

     Sysdepen::SetFileInfo( String( Instance->print_target ), String( msimTEXTFILE_TYPE),
                  String( msimDEFAULT_CREATOR_NAME) );

#endif


     return msimNO_ERROR;
}

PCHAR msimAxisLabelText( msimFLOAT ScalingFactor, msimPINSTANCE Instance, USHORT
           DataType, msimBOOL TrueRotatedText )
{
     PCHAR datatype;
     PCHAR units;
     msimSTRING tmpstr1, exponent;
     static msimSTRING str;

     // first build the exponent string

     if ( ScalingFactor == 1.0 )
          msimStringCopy( exponent, msimNULL_STR, sizeof exponent );
     else
     {

          /* get the exponent in ASCII format and construct the string      */
          /* to be included in the label - stored in exponent               */

          sprintf( tmpstr1, "%14e", ScalingFactor );
          msimMakeExpStr( tmpstr1, sizeof tmpstr1 );
          sprintf( exponent, "x 1%s ", tmpstr1 );
     }

     // select which array of string ptrs to use depending on whether
     // we have true rotated text or not

     if ( TrueRotatedText )
     {
          switch ( DataType )
          {
          case CONC_DATA :

               datatype = Instance->volume_option == msimVAR_VOL ? "Amount" : "Conc";
               units = msimConcUnits( Instance->conc_units, Instance->volume_option );
               break;

          case TEMP_DATA :
          case TEMP_DATA_Y :

               datatype = "Temp";
               units = TEMPERATURE_UNITS;
               break;

          case PRESS_DATA :
               datatype = "Press";
               units = msimPressureUnits( Instance->pressure_units );
               break;

          case VOL_DATA :
               datatype = "Vol";
               units = msimVolumeUnits( Instance->volume_units );
               break;

          case TIME_DATA :
               datatype = "Time";
               units = msimTimeUnits( Instance->time_units );
               break;

          case NO_DATA_TYPE :
          case EXP_DATA :
          default :
               datatype = msimNULL_STR;
               units = msimNULL_STR;
               break;
          }

          // now piece it all together

          sprintf( str, "%s [%s%s]", datatype, exponent, units );

     }
     else
     {
          switch ( DataType )
          {
          case CONC_DATA :

               datatype = Instance->volume_option == msimVAR_VOL ? "A" : "C";
               units = msimConcUnits( Instance->conc_units, Instance->volume_option );
               break;

          case TEMP_DATA :
          case TEMP_DATA_Y :

               datatype = "T";
               units = TEMPERATURE_UNITS;
               break;

          case PRESS_DATA :
               datatype = "P";
               units = msimPressureUnits( Instance->pressure_units );
               break;

          case VOL_DATA :
               datatype = "V";
               units = msimVolumeUnits( Instance->volume_units );
               break;

          case TIME_DATA :
               datatype = "t";
               units = msimTimeUnits( Instance->time_units );
               break;

          case NO_DATA_TYPE :
          case EXP_DATA :
          default :
               datatype = msimNULL_STR;
               units = msimNULL_STR;
               break;
          }

          // now piece it all together

          sprintf( str, "%s %s%s", datatype, exponent, units );
     }


     return str;
}

/*--------------------------------------------------------------------------*/
/*                    msimCheckForInvalidLimits()                           */
/*..........................................................................*/
/*                                                                          */
/* This function checks to be sure that the min and max limits specified    */
/* are such that the maximum is greater than the minimum. If not so then    */
/* the maxlimit is set to the minimum limit + 1                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void msimCheckForInvalidLimits( PPLOT_SPECS PlotSpecs )
{

     // first check for minima >= to maxima - adjust if necessary
     // then check the span of data

     if ( PlotSpecs->Ptime )
     {
          if ( PlotSpecs->time_axis.datamin >= PlotSpecs->time_axis.datamax )
               PlotSpecs->time_axis.datamax = PlotSpecs->time_axis.datamin + 1.0;

          if ( PlotSpecs->time_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->time_axis.datamax - PlotSpecs->time_axis.datamin ) / PlotSpecs->time_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->time_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->time_axis.datamin;
     }

     if ( PlotSpecs->Ptemp )
     {
          if ( PlotSpecs->temp_y_axis.datamin >= PlotSpecs->temp_y_axis.datamax
               )
               PlotSpecs->temp_y_axis.datamax = PlotSpecs->temp_y_axis.datamin
               +1.0;

          if ( PlotSpecs->temp_y_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->temp_y_axis.datamax - PlotSpecs->temp_y_axis.datamin ) / PlotSpecs->temp_y_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->temp_y_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->temp_y_axis.datamin;
          if ( PlotSpecs->temp_x_axis.datamin >= PlotSpecs->temp_x_axis.datamax
               )
               PlotSpecs->temp_x_axis.datamax = PlotSpecs->temp_x_axis.datamin
               +1.0;

          if ( PlotSpecs->temp_x_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->temp_x_axis.datamax - PlotSpecs->temp_x_axis.datamin ) / PlotSpecs->temp_x_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->temp_x_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->temp_x_axis.datamin;
     }
     if ( PlotSpecs->Ppress )
     {
          if ( PlotSpecs->press_axis.datamin >= PlotSpecs->press_axis.datamax )
               PlotSpecs->press_axis.datamax = PlotSpecs->press_axis.datamin +
               1.0;

          if ( PlotSpecs->press_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->press_axis.datamax - PlotSpecs->press_axis.datamin ) / PlotSpecs->press_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->press_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->press_axis.datamin;
     }

     if ( PlotSpecs->Pvol )
     {
          if ( PlotSpecs->volume_axis.datamin >= PlotSpecs->volume_axis.datamax
               )
               PlotSpecs->volume_axis.datamax = PlotSpecs->volume_axis.datamin
               +1.0;

          if ( PlotSpecs->volume_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->volume_axis.datamax - PlotSpecs->volume_axis.datamin ) / PlotSpecs->volume_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->volume_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->volume_axis.datamin;
     }
     if ( PlotSpecs->plotconc_v_time || PlotSpecs->plotconc_v_temp )
     {
          if ( PlotSpecs->conc_axis.datamin >= PlotSpecs->conc_axis.datamax )
               PlotSpecs->conc_axis.datamax = PlotSpecs->conc_axis.datamin + 1.0;

          if ( PlotSpecs->conc_axis.datamin > 0.0 &&
                    ( ( PlotSpecs->conc_axis.datamax - PlotSpecs->conc_axis.datamin ) / PlotSpecs->conc_axis.datamin )
                    < MIN_RANGE )
               PlotSpecs->conc_axis.datamax = ( 1.0 + MIN_RANGE ) * PlotSpecs->conc_axis.datamin;
     }
     return;
}


/*--------------------------------------------------------------------------*/
/*                              Round()                                     */
/*..........................................................................*/
/*                                                                          */
/* this function returns the rounded value of a double precision            */
/* real number as a  short integer.                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

SHORT Round( msimFLOAT x )
{
     return ( SHORT ) floor( x + 0.5 );
}

/*--------------------------------------------------------------------------*/
/*                        CalcConversionFactor()                            */
/*..........................................................................*/
/*                                                                          */
/* The simulation puts out concentration data in units of numbers of        */
/* particles. We present this graphically in concentrations. This function  */
/* calculates that conversion factor using information given in the         */
/* reaction Instance struct.                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimCalcConversionFactor( msimPINSTANCE Instance )
{
     msimFLOAT sum_of_init_concs, total_num_particles;
     msimPSPECIES ptr;

     sum_of_init_concs = 0.0;

     /* check for null ptr                                                  */

     if ( NULL == ( ptr = Instance->ptr_to_species_list ) )
          return 0.0;
     do
     {
          if ( ptr->nonzero )
               sum_of_init_concs += atof( ptr->initialconc );
          ptr = ptr->next;
     }
     while ( ptr );
     total_num_particles = atof( Instance->optionvalue[msimPARTICLES] );

     return ( msimFLOAT ) ( sum_of_init_concs / total_num_particles );
}

/*--------------------------------------------------------------------------*/
/*                          msimFreePlotArrayMemory()                       */
/*..........................................................................*/
/*                                                                          */
/* this function scans through  the array pointer in the PLOT_SPECS         */
/* struct and if any of the pointers is != NULL then frees the memory       */
/* pointed to by that pointer. The value of the pointer is then set to      */
/* NULL                                                                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimFreePlotArrayMemory( PPLOT_SPECS PlotSpecs )
{
     USHORT i = 0;

     if ( PlotSpecs->Ptime )
     {
          delete[]PlotSpecs->Ptime;
          PlotSpecs->Ptime = NULL;
     }
     if ( PlotSpecs->Ptemp )
     {
          delete[]PlotSpecs->Ptemp;
          PlotSpecs->Ptemp = NULL;
     }
     if ( PlotSpecs->Ppress )
     {
          delete[]PlotSpecs->Ppress;
          PlotSpecs->Ppress = NULL;
     }
     if ( PlotSpecs->Pvol )
     {
          delete[]PlotSpecs->Pvol;
          PlotSpecs->Pvol = NULL;
     }

     /* now free memory for concentration arrays                            */

     while ( i < MAX_NO_OF_PLOTS )
     {
          if ( PlotSpecs->Pconc[i] )
          {
               delete[]PlotSpecs->Pconc[i];
               PlotSpecs->Pconc[i] = NULL;
          }
          i++;
     }
     return;
}

/*--------------------------------------------------------------------------*/
/*                          PointWithinBounds()                             */
/*..........................................................................*/
/*                                                                          */
/* this function returns TRUE if *X is within the range given by            */
/* XMin and XMax, and *Y is within the range given by YMin and YMax         */
/* returns FALSE otherwise                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL PointWithinBounds( msimFLOAT X, msimFLOAT Y, msimFLOAT XMin, msimFLOAT
              XMax, msimFLOAT YMin, msimFLOAT YMax )
{
     return ( msimBOOL ) ( X >= XMin && X <= XMax && Y >= YMin && Y <= YMax );
}

/*--------------------------------------------------------------------------*/
/*                AnyConcPointsWithinBounds()                               */
/*..........................................................................*/
/*                                                                          */
/* this function returns TRUE if any of the pairs of *X and *ConcPtrArray[i]*/
/* are within the range given by                                            */
/* XMin and XMax, and MinConc and MaxConc, respectively                     */
/* returns FALSE otherwise                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL AnyConcPointsWithinBounds( msimFLOAT X, msimPFLOAT ConcPtrArray[],
              USHORT NumSpeciesToPlot, msimFLOAT XMin,
              msimFLOAT XMax, msimFLOAT MinConc, msimFLOAT
              MaxConc )
{
     msimBOOL within_range = FALSE;
     USHORT i;

     for ( i = 0; i < NumSpeciesToPlot; i++ )
     {
          within_range = within_range || ( msimBOOL ) ( X >= XMin && X <= XMax &&
               *ConcPtrArray[i] >= MinConc && *ConcPtrArray[i] <= MaxConc );
     }
     return within_range;
}


/*--------------------------------------------------------------------------*/
/*                          VisibleLineSegment()                            */
/*..........................................................................*/
/*                                                                          */
/* this function returns TRUE if the line segment defined by X1, X2 and     */
/* Y1, Y2 contains a segment visible within the viewport bounded by         */
/* MinX, Miny, MaxX, MaxY. Otherwise it returns FALSE. If the result is     */
/* TRUE then X1, Y1, X2, Y2 contain the points which define the visible     */
/* segment                                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL VisibleLineSegment( msimFLOAT& rX1, msimFLOAT& rY1, msimFLOAT& rX2,
                             msimFLOAT& rY2, USHORT AreaCode1, USHORT AreaCode2,
                             msimFLOAT MinX, msimFLOAT MinY, msimFLOAT MaxX,
                             msimFLOAT MaxY)
{
     // first get Point 1 on screen

     while (AreaCode1)
     {
          if (AreaCode1 & AreaCode2 )   // will not be visible; return
               return FALSE;

          if (AreaCode1 & LEFT_OF_WINDOW )
          {
               // the line crosses the left border of the vieport;
               // calculate the intersection with the left border

               rY1 += ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (MinX - rX1) );
               rX1 = MinX;

               goto RecalcCode1;
          }

          if (AreaCode1 & RIGHT_OF_WINDOW )
          {
               // the line crosses the right border of the vieport;
               // calculate the intersection with the right border

               rY1 += ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (MaxX - rX1) );
               rX1 = MaxX;

               goto RecalcCode1;
          }

          if (AreaCode1 & ABOVE_WINDOW )
          {
               // the line crosses the top border of the vieport;
               // calculate the intersection with the top border

               rX1 += ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (MaxY - rY1) );
               rY1 = MaxY;

               goto RecalcCode1;
          }

          if (AreaCode1 & BELOW_WINDOW )
          {
               // the line crosses the bottom border of the vieport;
               // calculate the intersection with the bottom border

               rX1 += ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (MinY - rY1) );
               rY1 = MinY;

          }

RecalcCode1:

     AreaCode1 = 0;

     if ( rX1 < MinX )
          AreaCode1 = LEFT_OF_WINDOW;
     else
          if ( rX1 > MaxX )

               AreaCode1 = RIGHT_OF_WINDOW;

     if ( rY1 < MinY )
          AreaCode1 |= BELOW_WINDOW;
     else
          if ( rY1 > MaxY )

               AreaCode1 |= ABOVE_WINDOW;

     }

     // now get Point 2 on screen

     while (AreaCode2)
     {
          if (AreaCode1 & AreaCode2 )   // will not be visible; return
               return FALSE;

          if (AreaCode2 & LEFT_OF_WINDOW )
          {
               // the line crosses the left border of the vieport;
               // calculate the intersection with the left border

               rY2 = rY1 + ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (MinX - rX1) );
               rX2 = MinX;

               goto RecalcCode2;
          }

          if (AreaCode2 & RIGHT_OF_WINDOW )
          {
               // the line crosses the right border of the vieport;
               // calculate the intersection with the right border

               rY2 = rY1 + ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (MaxX - rX1) );
               rX2 = MaxX;

               goto RecalcCode2;
          }

          if (AreaCode2 & ABOVE_WINDOW )
          {
               // the line crosses the top border of the vieport;
               // calculate the intersection with the top border

               rX2 = rX1 + ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (MaxY - rY1) );
               rY2 = MaxY;

               goto RecalcCode2;
          }

          if (AreaCode2 & BELOW_WINDOW )
          {
               // the line crosses the bottom border of the vieport;
               // calculate the intersection with the bottom border

               rX2 = rX1 + ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (MinY - rY1) );
               rY2 = MinY;

          }

RecalcCode2:

     AreaCode2 = 0;

     if ( rX2 < MinX )
          AreaCode2 = LEFT_OF_WINDOW;
     else
          if ( rX2 > MaxX )

               AreaCode2 = RIGHT_OF_WINDOW;

     if ( rY2 < MinY )
          AreaCode2 |= BELOW_WINDOW;
     else
          if ( rY2 > MaxY )

               AreaCode2 |= ABOVE_WINDOW;

     }

     return TRUE;  // we have successfully pushed the points  and a visible line segment remains
}





/*--------------------------------------------------------------------------*/
/*                     msimScalePointsForPlotting( )                        */
/*..........................................................................*/
/*                                                                          */
/* this function takes as paramters arrays of x and y values as             */
/* msimFLOATS. If a givien point is within the MinX MinY, MaxX, MaxY        */
/* limits then it is accepted and added to the Points Array after scaling   */
/* to screen coordinates                                                     */
/*                                                                          */
/* The screen coordinates are returned in the array of Point                */
/* pointed to by the parameter Points. The value returned by the function   */
/* is the number of valid points found and contained in the array Points    */
/*                                                                          */
/*--------------------------------------------------------------------------*/


USHORT msimScalePointsForPlotting( msimPFLOAT X, msimPFLOAT Y, Point Points[],
            USHORT TotalNumPts, USHORT Interval, msimFLOAT
            MinX, msimFLOAT MaxX, msimFLOAT MinY, msimFLOAT
            MaxY, SHORT LoLimX, SHORT HiLimX, SHORT LoLimY,
            SHORT HiLimY, USHORT ArraySize )
{
     msimFLOAT x_scaling_factor, y_scaling_factor;
     USHORT valid_pts = 0;
     msimPFLOAT p_last_pt_x = X + TotalNumPts - 1;


     x_scaling_factor = ( HiLimX - LoLimX ) / ( MaxX - MinX );
     y_scaling_factor = ( HiLimY - LoLimY ) / ( MaxY - MinY );

     while( ( X  <= p_last_pt_x ) && ( valid_pts <= ArraySize ) )
     {
          if ( *X >= MinX && *X <= MaxX && *Y >= MinY && *Y <= MaxY )
          {
               *Points = Point( Round(( *X - MinX ) *x_scaling_factor ) + LoLimX, Round(( *Y -
                         MinY ) *y_scaling_factor ) + LoLimY );
               Points++;
               valid_pts++;
          }

          X += Interval;
          Y += Interval;
     }

      return valid_pts;
}


