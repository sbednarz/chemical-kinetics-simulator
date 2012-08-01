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
/*  FILE NAME : msimaxis.cxx                                                */
/*                                                                          */
/*  This module  contains functions for dealing with the construction and   */
/*  display of x-y axes fo the plotting routines. Some                      */
/*  functions are inspired by  similar fcns found in the dbxyplot           */
/*  package in Devbase by J. Cooper of IBM YKT.                             */
/*                                                                          */
/*  started August 23, 1993                                                 */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */            
/*                                                                          */
/* CHANGE HISTORY :                                                         */
/*  5.24.95 fixed Selker Bug where under certain conditions the spacing     */
/*          between tic marks was so small that the value presented in the  */
/*          plot limits dialog was rounded to zero                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimplot.hxx"

#include <stdlib.h>
#include <string.h>
#include <math.h>

const msimFLOAT MINIMUM_SPACING = 1.00e-5;        // wdh add line 5.24.95

/* static fcns declared here                                                */

static USHORT TrialAxisCalc( msimFLOAT AxisMin, msimFLOAT AxisMax, USHORT MaxMarks, msimPFLOAT pStartOfTicks,
                   msimPFLOAT pEndOfTicks, msimPFLOAT pSpacing, msimPFLOAT pMultiplier );


static USHORT CalcOptimumNumTicks( msimPFLOAT StartOfTicks, msimPFLOAT EndOfTicks,
                   msimFLOAT AxisMin, msimFLOAT AxisMax, msimPFLOAT
                   pSpacing, msimPFLOAT pMultiplier );


static void AutoSetAxisParams( msimPAXIS pAxis, msimFLOAT Bigger );


static USHORT CalcActualNumTicksAndStartingPt( msimFLOAT AxisMin, msimFLOAT AxisMax, msimPFLOAT pStartOfTicks,
                   msimFLOAT Divisor );

static msimFLOAT CalcOptimumSpacing( USHORT MaxMarks, msimFLOAT AxisMin, msimFLOAT AxisMax );

/*--------------------------------------------------------------------------*/
/* msimMakeExp(char *buf) - constructs string with exponential only         */
/* from full string representation of fp number                             */
/*--------------------------------------------------------------------------*/

void msimMakeExpStr( PCHAR buf, size_t BufSize )
{
     size_t k;
     msimBOOL signflag;
     char PTR p, expon[80];

     if ( strlen( buf ) > 0 )
     {
          k = strcspn( buf, EXPONENT_TAGS );
          if ( k > 0 )
          {
               p = buf + k + 1;        /* remove 1.0e                         */
               if ( p[0] == '-' )
                    signflag = TRUE;
               else
                    signflag = FALSE;
               p++;                    /* remove + or -                       */
               while ( *p == '0' )
                    p++;               /* remove leading zeroes               */
               msimStringCopy( expon, p, sizeof expon );
               msimStringCopy( buf, "e", BufSize );
               if ( signflag )
                    msimStringCat( buf, "-", BufSize );/* put sign back in    */
               msimStringCat( buf, expon, BufSize );
          }
     }
}


// maximum and minimum number of tick marks for AutoSetAxisParams fcn

#define MAX_AUTO_TICKMARKS      8
#define MIN_AUTO_TICKMARKS      3


/*--------------------------------------------------------------------------*/
/*              AutoSetAxisParams( )                                        */
/* .........................................................................*/
/*                                                                          */
/* Loads the axis structures with tick mark min, max and spacing info       */
/* using automatically determined values based on the data ranges           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void AutoSetAxisParams( msimPAXIS pAxis, msimFLOAT Bigger )
{
     msimFLOAT tickstart;
     msimFLOAT tickend;
     msimFLOAT mult;

     // did caller ask for scaling of plot ?? if so adjust limits

     if ( Bigger != 0.0 )
     {
          msimFLOAT scale = ( pAxis->datamax - pAxis->datamin ) *Bigger;

          pAxis->datamin -= scale;

          if ( pAxis->datamin < 0.0 )  /* guarantee that Min is
                                          non-negative                        */
               pAxis->datamin = 0.0;

          pAxis->datamax += scale;
     }

     // have the optimum number of ticks calculated based on the
     // datamin and datamax values. The tick spacing, the positions of the
     // starting and ending tick marks and the scaling multiplier are
     // also calculated in this call 

     pAxis->num_ticks = CalcOptimumNumTicks( &tickstart, &tickend,
          pAxis->datamin, pAxis->datamax, & ( pAxis->spacing ), &mult );

     // set the axis minimum to the lesser of data minimum or the tickstart
     // this ensures that they will be visible

     pAxis->axismin = fabs((( tickstart * mult ) > pAxis->datamin ) ? pAxis->datamin / mult
               : tickstart );

     // similarly for the axis maximum -- set to a value that ensures all the
     // data and all the tick marks will be visible

     pAxis->axismax = ( ( tickend * mult ) < pAxis->datamax ) ? pAxis->datamax / mult :
          tickend;

     pAxis->axisrange = pAxis->axismax - pAxis->axismin;
     pAxis->mult = mult;

     /* calc format string data                                             */

     msimCalcNumFmt ( pAxis->tic_label_format, & ( pAxis->num_digits ),
          & ( pAxis->num_dec_places ), pAxis->axismax, pAxis->spacing,
          sizeof pAxis->tic_label_format );

     return;
}

/*--------------------------------------------------------------------------*/
/*                         CalcNumFmt( )                                    */
/*..........................................................................*/
/*                                                                          */
/* This routine calculates the correct format string for a value as large   */
/* as max.  It retuns the number of decimal places and builds a printf      */
/* format spec w/o the num dec places hard coded (ie it uses *). The        */
/* fmt string is placed in FmtStr                                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimCalcNumFmt ( PCHAR FmtStr, PUSHORT NumDigits, PUSHORT NumDecPlaces,
     msimFLOAT Max, msimFLOAT Spacing, size_t StringSize )
{
     /* base this in absolute values                                        */

     if ( Max < 0.0 )
          Max = - Max;

     /* decide on format for output strings                                 */
     /* first the number of digits                                          */
     /* use this for values greater than 10,000                             */

     if ( Max >= 10000.0 )
          *NumDigits = 0;
     else

          /* use this for values between 1000 and 10,000                         */

          if ( ( 10000.0 > Max ) && ( Max >= 1000.0 ) )
               *NumDigits = 5;
          else

               /* use this for values between 100 and 1000                            */

               if ( ( 1000.0 > Max ) && ( Max >= 100.0 ) )
                    *NumDigits = 4;
               else

                    /* use this for values between 10 and 100                              */

                    if ( ( 100.0 > Max ) && ( Max >= 10.0 ) )
                         *NumDigits = 3;

                    else

                         /* use this for values between 1 and 10                                */

                         if ( ( 10.0 > Max ) && ( Max >= 1.0 ) )
                              *NumDigits = 2;
                         else

                              /* use this for values less than 1.0                                   */

                              if ( ( 1.0 > Max ) && ( Max >= 0.01 ) )
                                   *NumDigits = 1;
                              else

                                   if ( ( Max > 0.0 ) && ( Max < 0.01 ) )
                                        *NumDigits = 0;

                                   else

                                        if ( Max == 0.0 )
                                             *NumDigits = 1;

     /* now calc the number of decimal places based on spacing              */

     if ( Spacing >= 5.0 )
          *NumDecPlaces = 0;           /* default large                       */
     else
          if ( ( 5.0 > Spacing ) && ( Spacing >= 0.5 ) )
               *NumDecPlaces = 1;

          else

               if ( ( 0.5 > Spacing ) && ( Spacing >= 0.05 ) )
                    *NumDecPlaces = 2;
               else
                    if ( ( 0.05 > Spacing ) && ( Spacing >= 0.005 ) )/* chg wdh*/
                         *NumDecPlaces = 3;
                    else
                         if ( ( 0.005 > Spacing ) && ( Spacing >= 0.0005 ) )
                              *NumDecPlaces = 4;
                         else
                              *NumDecPlaces = 5;
     if ( *NumDigits == 0 )
          msimStringCopy( FmtStr, "%4e", StringSize );
     else
     {
          *NumDigits += *NumDecPlaces;
          msimStringCopy( FmtStr, "%*.*f", StringSize );
     }
     return;
}

/*--------------------------------------------------------------------------*/
/*                      CalcActualNumTicksAndStartingPt( )                  */
/*..........................................................................*/
/*                                                                          */
/* calculates  the number of tick marks given axis minimum and maximum      */
/* values. returns the actual number of tick marks as return value and      */
/* sets *pStartOfTicks to the appropriate value                             */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

static USHORT CalcActualNumTicksAndStartingPt( msimFLOAT AxisMin, msimFLOAT AxisMax,
                   msimPFLOAT pStartOfTicks, msimFLOAT Divisor )
{
     msimFLOAT min_val;
     msimFLOAT max_val;
     msimFLOAT mark;
     msimFLOAT diff;

     // start in the middle. work towards ends of range to arrive at min_val and
     // max_val values

     max_val = floor( ( ( AxisMax + AxisMin ) / 2.0 ) / Divisor ) * Divisor;

     while ( max_val > AxisMin )       /* first move down in value  past minimum value of axis   */
          max_val -= Divisor;

     while ( max_val < AxisMax )       /* then work up invalue to  find maximum ticmark     */
          max_val += Divisor;

     // not sure what these next two lines are doing, but they are necessary or else the
     // number of tick marks is miscalculated

     diff = fabs(( max_val - Divisor ) - AxisMax ); 

     if ( ( ( diff / Divisor ) < 0.01 ) && ( (max_val - Divisor) > AxisMax ) ) // new form 7.22.94
          max_val -= Divisor;            


     // we now do an analogous calculation to derive the min_val value

     min_val = max_val;                // start at the maximum 

     while ( min_val > AxisMin )       /* then work downin value to find minimum ticmark */
          min_val -= Divisor;

     // not sure what these next two lines are doing, but they are necessary or else the
     // number of tick marks is miscalculated

     diff = fabs(( min_val + Divisor ) - AxisMin );

 
     if ( ( diff / Divisor ) < 0.01 )
     {
          min_val += Divisor;          /* actual leftmost tic mark            */
     }

     /* now count how many tick marks we have given the limits and the initial tick mark position */

     mark = min_val;
     USHORT num_ticks = 0;

     while ( ( mark <= max_val ) && ( num_ticks < MAXTICKS ) )
     {
          mark += Divisor;
          num_ticks++;
     }

     // return the calculated values

     *pStartOfTicks = min_val;
     return num_ticks;
}



/*--------------------------------------------------------------------------*/
/*                     function TrialAxisCalc(  )                           */
/*..........................................................................*/
/*                                                                          */
/* This routine returns the total number of tic marks actually calculated   */
/* as a USHORT  and returns a set of values via pointers specifying the     */
/* range of the tickmarks and their spacing                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

static USHORT TrialAxisCalc( msimFLOAT AxisMin, msimFLOAT AxisMax, USHORT MaxMarks, msimPFLOAT pStartOfTicks,
                   msimPFLOAT pEndOfTicks, msimPFLOAT pSpacing, msimPFLOAT pMultiplier )
{
     USHORT num_ticks;

     msimFLOAT spacing;
     msimFLOAT first_tick_position;
     msimFLOAT multiplier;


     // first calc value for multiplier and scale axis limits appropriately

     multiplier = msimCalcMultiplierFactor( AxisMax );

     AxisMax /= multiplier;
     AxisMin /= multiplier;

     // determine what the optimum spacing (i.e the interval between tickmarks) is

     spacing = CalcOptimumSpacing( MaxMarks, AxisMin, AxisMax);  

     // wdh start add block 5.24.95 --- this block added to deal with Selker bug
      
     if ( spacing < MINIMUM_SPACING )
     {
          AxisMax = AxisMin + ( (AxisMax-AxisMin)*( MINIMUM_SPACING / spacing ) );
          spacing = MINIMUM_SPACING;
     }

     // wdh end aadd block 5.24.95
     // determine the actual number of tick marks 

     num_ticks = CalcActualNumTicksAndStartingPt( AxisMin, AxisMax, &first_tick_position, spacing );

     // now calc where the value for the last tick mark

     msimFLOAT last_tick_position = first_tick_position;

     while ( last_tick_position < AxisMax )
          last_tick_position += spacing;

     // all done - return values 

     *pMultiplier   = multiplier;
     *pSpacing      = spacing;
     *pStartOfTicks = first_tick_position;
     *pEndOfTicks   = last_tick_position;

     return num_ticks;
}


// ----------------------------------------------------------------------------
//                       function CalcOptimumSpacing( )
// ----------------------------------------------------------------------------
//
//  given a maximum allowable number of marks along an axis, the minimum value
//  of the axis and the maximum value, this fcn returns the optimum value for the
//  divisor to be used in the function TrialAxisCalc. Basically we start with a
// trial value for the divisor and keep reducing it's magnitude in orderly steps
// of a factor of 2.0 or 2.5 until the axis defined by AxisMin AxisMax can
// be divided into a number of segments of size divisor without exceeding MaxMarks
//
// ----------------------------------------------------------------------------

msimFLOAT CalcOptimumSpacing( USHORT MaxMarks, msimFLOAT AxisMin, msimFLOAT AxisMax )
{

     msimBOOL first_pass = TRUE;

     USHORT num_marks = 0;

     msimFLOAT divisor      = 5000.00;              // initial trial value for spacing
     msimFLOAT next_divisor = 2.5;                  // initial next reduction factor for divisor
     msimFLOAT width        = AxisMax - AxisMin;
     

     while ( num_marks < MaxMarks )
     {
          divisor /= next_divisor;     /* calculate next spacing              */

          num_marks = ( USHORT ) ( width / divisor );

          if ( next_divisor == 2.5 )
               next_divisor = 2.0;// set next reduction factor: alternate 2.5, 2.0, 2.5 . . .
          else
               if ( first_pass )
                    first_pass = FALSE;
               else
               {
                    next_divisor = 2.5;
                    first_pass = TRUE;
               }
     }

     // reset divisor, next_divisor to the value previous to failing above test

     if ( next_divisor == 2.5 )
          next_divisor = 2.0;
     else
          if ( ! first_pass )
               next_divisor = 2.5;

     divisor *= next_divisor;          /* reset to next larger div            */

     return divisor;
}




/*--------------------------------------------------------------------------*/
/*                              CalcOptimumNumTicks( )                      */
/*..........................................................................*/
/*                                                                          */
/*  this routine attempts to determine an optimum number of Tick marks for  */
/*  a plot axis given the limts of the axis AxisMin and AxisMax. Returns    */
/*  the total                                                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

USHORT CalcOptimumNumTicks( msimPFLOAT pStartOfTicks, msimPFLOAT pEndOfTicks,
            msimFLOAT AxisMin, msimFLOAT AxisMax, msimPFLOAT
            pSpacing, msimPFLOAT pMultiplier )
{
     USHORT max_allowed_num_ticks = 4;             // initial trial value to start

     // maximize the number of marks w/o exceeding a resoanble practical limit

     USHORT num_ticks_found = 0;

     while ( num_ticks_found < MAX_AUTO_TICKMARKS )
     {
          num_ticks_found = TrialAxisCalc( AxisMin, AxisMax, max_allowed_num_ticks, pStartOfTicks, pEndOfTicks,
               pSpacing, pMultiplier );

          max_allowed_num_ticks *= 2;              /* in case we need to try again        */

          if ( max_allowed_num_ticks > MAXTICKS )
               max_allowed_num_ticks = MAXTICKS;
     }

     // if we overshot, back up until we are below MAX_AUTO_TICKMARKS

     if ( num_ticks_found > MAX_AUTO_TICKMARKS )
     {
          max_allowed_num_ticks /= 2;

          while ( num_ticks_found > MAX_AUTO_TICKMARKS )
          {
               num_ticks_found = TrialAxisCalc( AxisMin, AxisMax, max_allowed_num_ticks, pStartOfTicks, pEndOfTicks,
                    pSpacing, pMultiplier );

               max_allowed_num_ticks /= 2;         /* in case we need to try again        */
          }
     }

     // too few marks now ?

     if ( num_ticks_found <= MIN_AUTO_TICKMARKS )
     {
          max_allowed_num_ticks *= 4;
          num_ticks_found = TrialAxisCalc( AxisMin, AxisMax, max_allowed_num_ticks, pStartOfTicks, pEndOfTicks,
               pSpacing, pMultiplier );
     }

     return num_ticks_found;
}


VOID msimSetPlotAndAxisLimits( PPLOT_SPECS PlotSpecs, msimFLOAT ScalingFactor )
{
     if ( PlotSpecs->Ptime )
          AutoSetAxisParams( &( PlotSpecs->time_axis ), ScalingFactor );

     if ( PlotSpecs->Ptemp )
          AutoSetAxisParams( &( PlotSpecs->temp_y_axis ), ScalingFactor );

     if ( PlotSpecs->Ppress )
          AutoSetAxisParams( &( PlotSpecs->press_axis ), ScalingFactor );

     if ( PlotSpecs->Pvol )
          AutoSetAxisParams( &( PlotSpecs->volume_axis ), ScalingFactor );

     if ( PlotSpecs->plotconc_v_time || PlotSpecs->plotconc_v_temp )
          AutoSetAxisParams( &( PlotSpecs->conc_axis ), ScalingFactor );

     /* if we are going to be plotting temperature on the x axis            */
     /* then copy the tickmark info                                         */

     if ( PlotSpecs->Ptemp && PlotSpecs->plot_temp_x )
          PlotSpecs->temp_x_axis = PlotSpecs->temp_y_axis;

     return;
}

void msimAdjustLimits( PPLOT_SPECS PlotSpecs )
{
     ULONG i;
     msimPFLOAT px, py;
     msimPAXIS x_plot_data, y_plot_data;

     /* set the indexes into the datalimit array depending on the           */
     /* data type of the external data                                      */

     switch ( PlotSpecs->expdata_x_type )
     {
     case TIME_DATA :
          x_plot_data = & ( PlotSpecs->time_axis );
          break;
     case TEMP_DATA :
          x_plot_data = & ( PlotSpecs->temp_x_axis );
          break;
     default :
          return;
     }
     switch ( PlotSpecs->expdata_y_type )
     {
     case VOL_DATA :
          y_plot_data = & ( PlotSpecs->volume_axis );
          break;
     case TEMP_DATA :
     case TEMP_DATA_Y :
          y_plot_data = & ( PlotSpecs->temp_y_axis );
          break;
     case CONC_DATA :
          y_plot_data = & ( PlotSpecs->conc_axis );
          break;
     case PRESS_DATA :
          y_plot_data = & ( PlotSpecs->press_axis );
          break;
     default :
          return;
     }

     /* then scan through the external data and check to see if             */
     /* it exceeds the existing data limits - if so adjust the              */
     /* data limits                                                         */

     px = PlotSpecs->Pexpdata_x;
     py = PlotSpecs->Pexpdata_y;
     for ( i = 0; i < PlotSpecs->num_exp_data_sets; i++ )
     {
          if ( x_plot_data->datamin > *px )
               x_plot_data->datamin = *px;

          if ( x_plot_data->datamax < *px )
               x_plot_data->datamax = *px;

          if ( y_plot_data->datamin > *py )
               y_plot_data->datamin = *py;

          if ( y_plot_data->datamax < *py )
               y_plot_data->datamax = *py;

          px++;
          py++;
     }

     /* now update the axis and data limit data                             */

     AutoSetAxisParams( x_plot_data, PLOTLIMIT_SCALINGFACTOR );

     AutoSetAxisParams( y_plot_data, PLOTLIMIT_SCALINGFACTOR );

     return;
}

void msimConstructNumericLimitString( PCHAR Target, msimFLOAT Mantissa, msimFLOAT
          ScalingFactor, USHORT NumDigits, USHORT
          NumDecplaces )
{
     msimSTRING exponent;

     /* if value is zero or Scaling factor = unity                          */
     /* then skip addn of exponent                                          */

     if ( ScalingFactor != 1.0 && Mantissa != 0.0 )
     {
          sprintf( exponent, "%14e", ScalingFactor );
          msimMakeExpStr( exponent, sizeof exponent );
     }
     else
          msimStringCopy( exponent, msimNULL_STR, sizeof exponent );
     sprintf( Target, "%*.*f%s", NumDigits, NumDecplaces, Mantissa, exponent );

     /* remove leading spaces if any                                        */

     msimStrip( Target, 'L' );
     return;
}

/* this returns a multiplier factor 1.00eX such that                        */
/* Value / multiplier is between 0 and 10                                   */
/* If Value is less than 5000.0 or greater than 0.1                         */
/* then the value returned is = 1.00e0                                      */

msimFLOAT msimCalcMultiplierFactor( msimFLOAT Value )
{
     msimFLOAT multiplier = 1.0;

     if ( Value > 5000.0 )
     {
          while ( Value > 10.0 )
          {
               Value /= 10.0;
               multiplier *= 10.0;
          }
     }
     else
          if ( Value < 0.1 )
          {
               while ( Value < 1.0 )
               {
                    Value *= 10.0;
                    multiplier /= 10.0;
               }
          }
          else
               multiplier = 1.0;

     return multiplier;
}

