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
//  XYAXIS.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the XyAxis class for
//  the VSIM user interface. This is the base class from which
//  XAxis and YAxis classes are derived.
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED : 6.28.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "xyaxis.hxx"
#include "math.hxx"
#include "editaxis.hxx"

#endif

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "strings.hrc"


const FLOAT64 EPSILON = (FLOAT64) MAXTICS * DBL_EPSILON;




// ----------------------------------------------------------------------------
//                                XyAxis::XyAxis( )
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

XyAxis::XyAxis( OutputDevice* pParent, const Limits& DataLimits ) :
Axis( pParent ),
units(),
scale_factor_str(),
scaling_head( ResId( vID_STR_AXIS_SCALING_HEAD ) ),
scaling_tail( ResId( vID_STR_AXIS_SCALING_TAIL ) ),
data_limits( DataLimits ),
axis_limits(),
tic_value_step_size( 0.0 ),
scaling_multiplier( 0.0 ),
show_grid( FALSE ),
labels_invalid( TRUE ),
show_units( TRUE ),
precision( 0 ),
total_num_digits( 0 ),
grid_length_device_coords( 0 )
{
     ScaleDataLimits();
     AutoSetAxisParameters( );
}




// ----------------------------------------------------------------------------
//                                XyAxis::XyAxis( )
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

XyAxis::XyAxis( OutputDevice* pParent ) :
Axis( pParent ),
units(),
scale_factor_str(),
data_limits(),
axis_limits(),
tic_value_step_size( 0.0 ),
scaling_multiplier( 0.0 ),
show_grid( FALSE ),
labels_invalid( TRUE ),
show_units( TRUE ),
precision( 0 ),
total_num_digits( 0 )
{
}




// ----------------------------------------------------------------------------
//                                XyAxis::EditObject( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 XyAxis::EditObject( const Point )
{
     return EditAxis( this );
}



// ----------------------------------------------------------------------------
//                 XyAxis::CalcActualNumTicksAndStartingPt( )
//.............................................................................
//                                                                             
// calculates  the number of tick marks given axis minimum and maximum         
// values. returns the actual number of tick marks as return value and         
// sets *pStartOfTicks to the appropriate value                                
//                                                                             
// ----------------------------------------------------------------------------

UINT16 XyAxis::CalcActualNumTicksAndStartingPt( FLOAT64 AxisMin, FLOAT64 AxisMax,
                   FLOAT64* pStartOfTicks, FLOAT64 Divisor )
{
     FLOAT64 min_val;
     FLOAT64 max_val;
     FLOAT64 mark;
     FLOAT64 diff;

     // start in the middle. work towards ends of range to arrive at min_val and
     // max_val values

     max_val = floor( ( ( AxisMax + AxisMin ) / 2.0 ) / Divisor ) * Divisor;

     // first move down in value  past minimum value of axis 

     while ( max_val > AxisMin )       
          max_val -= Divisor;

     // then work up in value to find maximum ticmark 

     while ( max_val < AxisMax )       
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
          min_val += Divisor;          // actual leftmost tic mark
     }

     // now count how many tick marks we have given the limits and the initial tick mark position

     mark = min_val;
     UINT16 num_ticks = 0;

     while ( ( mark <= max_val ) && ( num_ticks < MAXTICS ) )
     {
          mark += Divisor;
          num_ticks++;
     }

     // return the calculated values

     *pStartOfTicks = min_val;
     return num_ticks;
}







// ----------------------------------------------------------------------------
//                       XyAxis::CalcOptimumSpacing( )
// ............................................................................
//
//  given a maximum allowable number of marks along an axis, the minimum value
//  of the axis and the maximum value, this fcn returns the optimum value for the
//  divisor to be used in the function TrialAxisCalc. Basically we start with a
// trial value for the divisor and keep reducing it's magnitude in orderly steps
// of a factor of 2.0 or 2.5 until the axis defined by AxisMin AxisMax can
// be divided into a number of segments of size divisor without exceeding MaxMarks
//
// ----------------------------------------------------------------------------

FLOAT64 XyAxis::CalcOptimumSpacing( UINT16 MaxMarks, FLOAT64 AxisMin, FLOAT64 AxisMax )
{

     BOOL8 first_pass = TRUE;

     UINT16 num_marks = 0;

     FLOAT64 divisor      = 5000.00;              // initial trial value for spacing
     FLOAT64 next_divisor = 2.5;                  // initial next reduction factor for divisor
     FLOAT64 width        = AxisMax - AxisMin;
     

     while ( num_marks < MaxMarks )
     {
          divisor /= next_divisor;     /* calculate next spacing              */

          num_marks = ( UINT16 ) ( width / divisor );

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







// ----------------------------------------------------------------------------
//                     XyAxis::CalcMultiplierFactor()
// ............................................................................
// 
// this returns a multiplier factor 1.00eX such that (Value / multiplier)
// is between 0 and 10. If Value is less than 5000.0 or greater than 0.1   
// then the value returned is = 1.00e0                
//
// ----------------------------------------------------------------------------

FLOAT64 XyAxis::CalcMultiplierFactor( FLOAT64 Value )
{
     FLOAT64 multiplier = 1.0;

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






/*--------------------------------------------------------------------------*/
/*                     function TrialAxisCalc(  )                           */
/*..........................................................................*/
/*                                                                          */
/* This routine returns the total number of tic marks actually calculated   */
/* as a USHORT  and returns a set of values via pointers specifying the     */
/* range of the tickmarks and their spacing                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

UINT16 XyAxis::TrialAxisCalc( FLOAT64 AxisMin, FLOAT64 AxisMax, UINT16 MaxMarks, 
 FLOAT64* pStartOfTicks, FLOAT64* pEndOfTicks, FLOAT64* pSpacing, FLOAT64* pMultiplier )
{
     const FLOAT64 MINIMUM_SPACING = 1.00e-5;        // for selker bug 

     UINT16 num_ticks;

     FLOAT64 spacing;
     FLOAT64 first_tick_position;
     FLOAT64 multiplier;


     // first calc value for multiplier and scale axis limits appropriately

     multiplier = CalcMultiplierFactor( AxisMax );

     AxisMax /= multiplier;
     AxisMin /= multiplier;

     // determine what the optimum spacing (i.e the interval between tickmarks) is

     spacing = CalcOptimumSpacing( MaxMarks, AxisMin, AxisMax);  

     // this block deals with Selker bug
      
     if ( spacing < MINIMUM_SPACING )
     {
          AxisMax = AxisMin + ( (AxisMax-AxisMin)*( MINIMUM_SPACING / spacing ) );
          spacing = MINIMUM_SPACING;
     }

     // determine the actual number of tick marks 

     num_ticks = CalcActualNumTicksAndStartingPt( AxisMin, AxisMax, &first_tick_position, spacing );

     // now calc where the value for the last tick mark

     FLOAT64 last_tick_position = first_tick_position;

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
//                           XyAxis::CalcOptimumNumTicks()
// ............................................................................
// 
//  this routine attempts to determine an optimum number of Tick marks for  
//  a plot axis given the data limits of the axis. Returns the number of tics
//
// ----------------------------------------------------------------------------

UINT16 XyAxis::CalcOptimumNumTicks( FLOAT64* pStartOfTicks, FLOAT64* pEndOfTicks,
            FLOAT64 AxisMin, FLOAT64 AxisMax, FLOAT64* pSpacing, FLOAT64* pMultiplier )
{
     const UINT16  MAX_AUTO_TICKMARKS = 8;
     const UINT16  MIN_AUTO_TICKMARKS = 3;

     UINT16 max_allowed_num_ticks = 4;             // initial trial value to start

     // maximize the number of marks w/o exceeding a resoanble practical limit

     UINT16 num_ticks_found = 0;

     while ( num_ticks_found < MAX_AUTO_TICKMARKS )
     {
          num_ticks_found = TrialAxisCalc( AxisMin, AxisMax, max_allowed_num_ticks, pStartOfTicks, pEndOfTicks,
               pSpacing, pMultiplier );

          max_allowed_num_ticks *= 2;              /* in case we need to try again        */

          if ( max_allowed_num_ticks > MAXTICS )
               max_allowed_num_ticks = MAXTICS;
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










// ----------------------------------------------------------------------------
//                             XyAxis::AutoSetAxisParameters()
// ............................................................................
// 
// Loads the various internal data member with tic mark and spacing info    
// using automatically determined values based on the data_limits    
//
// ----------------------------------------------------------------------------

void XyAxis::AutoSetAxisParameters()
{
     FLOAT64 tickstart;
     FLOAT64 tickend;
     FLOAT64 mult;

     UINT16  number_of_tic_marks;

     // have the optimum number of ticks calculated based on the
     // datamin and datamax values. The tick spacing, the positions of the
     // starting and ending tick marks and the scaling multiplier are
     // also calculated in this call 

     number_of_tic_marks  = CalcOptimumNumTicks( &tickstart, &tickend,
          data_limits.Min(), data_limits.Max(), &tic_value_step_size, &mult );

     // set the axis minimum to the lesser of data minimum or the tickstart
     // this ensures that they will be visible

     axis_limits.Min() = fabs((( tickstart * mult ) > data_limits.Min() ) ? data_limits.Min() / mult
               : tickstart );

     // similarly for the axis maximum -- set to a value that ensures all the
     // data and all the tick marks will be visible

     axis_limits.Max() = ( ( tickend * mult ) < data_limits.Max() ) ? data_limits.Max() / mult :
          tickend;

     scaling_multiplier = mult;

     // calc format string data

     CalcTicMarkFormat ( &total_num_digits,
          &precision, axis_limits.Max(), tic_value_step_size );

#if defined(__UNIT_TEST__)

     cout << "AutoSetAxisParameters..." << endl;
     cout << "data limits = " << data_limits.Min() << " " << data_limits.Max() << endl;
     cout << "axis limits = " << data_limits.Min() << " " << data_limits.Max() << endl;
     cout << "tic_value_step_size = " << tic_value_step_size << endl;
     cout << "scaling_multiplier = " << scaling_multiplier << endl;
     cout << "precision = " << precision << endl;
     cout << "total_num_digits = " << total_num_digits << endl;

#endif

     SetNumTicMarks( number_of_tic_marks );
     ConstructTicMarkLabels();
}


// ----------------------------------------------------------------------------
//                             XyAxis::CalcTicMarkFormat( )
// ............................................................................
// 
// calculates the total number of digits and the number of digits right of the    
// decimal place in the tic label 
//
// ----------------------------------------------------------------------------

void XyAxis::CalcTicMarkFormat( UINT16* NumDigits, UINT16* NumDecPlaces, 
 FLOAT64 Max, FLOAT64 Spacing )
{
     //base this in absolute values 

     if ( Max < 0.0 )
          Max = - Max;

     // decide on format for output strings                            
     // first the number of digits                                     
     // use this for values greater than 10,000                        

     if ( Max >= 10000.0 )
          *NumDigits = 0;
     else

          // use this for values between 1000 and 10,000    

          if ( ( 10000.0 > Max ) && ( Max >= 1000.0 ) )
               *NumDigits = 5;
          else

               // use this for values between 100 and 1000 

               if ( ( 1000.0 > Max ) && ( Max >= 100.0 ) )
                    *NumDigits = 4;
               else

                    // use this for values between 10 and 100

                    if ( ( 100.0 > Max ) && ( Max >= 10.0 ) )
                         *NumDigits = 3;

                    else

                         // use this for values between 1 and 10 

                         if ( ( 10.0 > Max ) && ( Max >= 1.0 ) )
                              *NumDigits = 2;
                         else

                              // use this for values less than 1.0       

                              if ( ( 1.0 > Max ) && ( Max >= 0.01 ) )
                                   *NumDigits = 1;
                              else

                                   if ( ( Max > 0.0 ) && ( Max < 0.01 ) )
                                        *NumDigits = 0;

                                   else

                                        if ( Max == 0.0 )
                                             *NumDigits = 1;

     // now calc the number of decimal places based on spacing 

     if ( Spacing >= 5.0 )
          *NumDecPlaces = 0;           // default large   
     else
          if ( ( 5.0 > Spacing ) && ( Spacing >= 0.5 ) )
               *NumDecPlaces = 1;

          else

               if ( ( 0.5 > Spacing ) && ( Spacing >= 0.05 ) )
                    *NumDecPlaces = 2;
               else
                    if ( ( 0.05 > Spacing ) && ( Spacing >= 0.005 ) )
                         *NumDecPlaces = 3;
                    else
                         if ( ( 0.005 > Spacing ) && ( Spacing >= 0.0005 ) )
                              *NumDecPlaces = 4;
                         else
                              *NumDecPlaces = 5;


     if ( *NumDigits != 0 )
     {
          *NumDigits += *NumDecPlaces;
     }
     return;

}


// ----------------------------------------------------------------------------
//                       XyAxis::ConstructTicMarkLabels()
// ............................................................................
// 
//  this function uses the internal state of the XyAxis object to 
//  construct text label, one for each tic mark
//
// ----------------------------------------------------------------------------

void XyAxis::ConstructTicMarkLabels()
{
     const UINT32   MAX_TIC_LABEL_LENGTH = 32;
     const char*    TIC_LABEL_FORMAT_STR_1 = "%4e";   
     const char*    TIC_LABEL_FORMAT_STR_2 = "%*.*f"; 

     char           buffer[ MAX_TIC_LABEL_LENGTH ];
     FLOAT64        next_tic;
     String         str;

     if ( 0 == total_num_digits  )
     {
          for ( UINT16 i = 0; i < num_tic_marks; i++ )
          {
               next_tic = axis_limits.Min() + ( i * tic_value_step_size );

               sprintf( buffer, TIC_LABEL_FORMAT_STR_1, next_tic );

               str = buffer;

               str.EraseLeadingChars();

               SetTicLabel( i, str );


#if defined(__UNIT_TEST__)

               cout << "tic mark = " << i  << ", text = " << buffer << endl;

#endif

         }
    }
    else
    {
          for ( UINT16 i = 0; i < num_tic_marks; i++ )
          {
               next_tic = axis_limits.Min() + ( i * tic_value_step_size );

               sprintf( buffer, TIC_LABEL_FORMAT_STR_2, total_num_digits, 
                precision, next_tic );

               str = buffer;

               str.EraseLeadingChars();


               SetTicLabel( i, str );

#if defined(__UNIT_TEST__)

               cout << "tic mark = " << i  << ", text = " << buffer << endl;

#endif

         }
    }
     MakeExponentStr();
     labels_invalid = FALSE;
}


// ----------------------------------------------------------------------------
//                                XyAxis::ScaleDataLimits()
// ............................................................................
// 
//  this function scales the data minimum and maximum so that the 
//  data minimum and maximum contained internally (i.e. those used to 
//  AutoSet ) are slightly smaller/larger respectively than the
//  actual passed data limits. This will ensure that the AutoSet axis 
//  displays all the data 
//
// ----------------------------------------------------------------------------

void XyAxis::ScaleDataLimits()
{
     const FLOAT64  SCALING_FACTOR = 0.02;

     FLOAT64 scale = ( data_limits.Range()) * SCALING_FACTOR;

     FLOAT64 tempmin = data_limits.Min() - scale;

     if ( tempmin < 0.0 )  // guarantee that Min() is non-negative
          tempmin = 0.0;

     data_limits.Min() = tempmin;
     data_limits.Max() = data_limits.Max() + scale;
}



// ----------------------------------------------------------------------------
//                        XyAxis::SetDataLimits( )
// ............................................................................
// 
//  this function resets and internally rescales the data min/max 
//
// ----------------------------------------------------------------------------

void XyAxis::SetDataLimits( const Limits& rNewLimits ) 
{ 
     data_limits = rNewLimits; 
     ScaleDataLimits();
     AutoSetAxisParameters( );
}






// ----------------------------------------------------------------------------
//                        XyAxis::SetAxisParameters()
// ............................................................................
// 
//  this function sets the axis minimum, maximum and tic interval to those 
//  specified in the parameters. Function returns an AXIS_RC enum =
//  AXIS_RC_VALID_DATA if the parameters are valid and the Axis Parameters
//  were changed, otherwise the returned AXIS_RC indicates why the data was 
//  invalid. If invalid then the internal axis data are not changed 
//
// ----------------------------------------------------------------------------

enum AXIS_RC XyAxis::SetAxisParameters( const Limits& rNewLimits, 
 const FLOAT64 NewTicInterval, const UINT16 Precision )
{
     // is max less than or equal to min? if so it's an error  

     if ( rNewLimits.Range() <= 0.0 )
          return AXIS_RC_MINMAX_ERROR;

     // now make sure that the interval is greater than 0.0  and that it is 
     // less than Range of Limits

     if ( NewTicInterval <= 0.0 || 
          NewTicInterval > rNewLimits.Range() 
        )
          return AXIS_RC_TIC_RANGE_ERROR;

     // now check whether too many tic marks have been requested 
     // calculate the requested number of tics

     // note that simple dividing axisRange by new tic interval 
     // does not seem go give correct answer for 2.000/1.000 so 
     // EPSILON added to ensure that roundoff error in the floating  pt calculation
     // does not give an incorrect result

     UINT16 requested_num_tics = (UINT16) ( ( rNewLimits.Range() / NewTicInterval ) + EPSILON + 1.0 );

     if (  requested_num_tics > MAXTICS )
          return AXIS_RC_TOO_MANY_TICS;

     // if we made it here then parameters are valid = update internal state of XyAxis

     // fill in several fields 

     scaling_multiplier = CalcMultiplierFactor( rNewLimits.Max() );

     axis_limits.Min() = rNewLimits.Min() / scaling_multiplier;
     axis_limits.Max() = rNewLimits.Max() / scaling_multiplier;
     tic_value_step_size = NewTicInterval / scaling_multiplier;

     // we now go ahead and calculate the numeric format for the tic
     // mark labels using our standard method. We then modify the values 
     // produced for that function call so that the number of decimal places
     // requested in the paramter Precision is in effect


     CalcTicMarkFormat( &total_num_digits, &precision, axis_limits.Max(), 
      tic_value_step_size );

     // adjust for num_dec_places contribution to num_digits before 

     total_num_digits -= precision;

     // do the over-ride with newly specified number of dec places

     precision = Precision;

     /* and adjust num_digits again                                         */

     total_num_digits += Precision;

     // now we are ready to construct labels and return with success rc 

     SetNumTicMarks( requested_num_tics );

     ConstructTicMarkLabels();

     return AXIS_RC_VALID_DATA;
}



// ----------------------------------------------------------------------------
//                        XyAxis::MakeExponentStr( )
// ............................................................................
// 
// this function constructs a String with exponential only chars only 
// from a the floating point number scaling_multiplier. The result is saved 
// in the data member scale_factor_str
//
// ----------------------------------------------------------------------------

void XyAxis::MakeExponentStr()
{
     // if scaling_multiplier == unity then set result equal to null string

     if ( scaling_multiplier == 1.0 )
          scale_factor_str = "";
     else
     {
          const UINT32   BUFFER_LENGTH = 32;
          const char *   EXPONENT_TAGS = "eE";
     
          char buffer[ BUFFER_LENGTH ];
     
          // make the char buffer 

          sprintf( buffer, "%14e", scaling_multiplier );

          if ( strlen( buffer ) > 0 )
          {
               UINT32 k = strcspn( buffer, EXPONENT_TAGS );
               BOOL8  signflag;

               if ( k > 0 )
               {
                    char * p = buffer + k + 1;        // remove ?.?e 

                    signflag = ( p[0] == '-' );

                    p++;                    // remove + or -   

                    while ( *p == '0' )
                         p++;               /* remove leading zeroes               */


                    scale_factor_str = "x 1e";

                    if ( signflag )
                         scale_factor_str += "-";

                    scale_factor_str += p;
               }
          }
     }
}






// ------    functions for XAxis follow   ------



// ----------------------------------------------------------------------------
//                        XAxis::ResizeObject( )
// ............................................................................
// 
//  this function is called to resize the object. We recalulcate all positions
//  in device coordinates
//
// ----------------------------------------------------------------------------

void XAxis::ResizeObject( )
{
     Size char_size = GetpParent()->GetFont().GetSize();

     start_pt = active_area.TopLeft();
     end_pt = active_area.TopRight();

     // center the title in the active area, up 1 character height from bottom

     title_position = active_area.BottomCenter() - Point( 0, 2 * char_size.Height() );

     const UINT16 length = active_area.GetWidth() - 1;
     const FLOAT64 range = axis_limits.Range(); 

#if defined(__UNIT_TEST__)

     cout << "\nResizeObject XAxis..." << endl;
     cout << "start pt = " << start_pt.X() << ", " << start_pt.Y() << endl;
     cout << "length = " << length << endl;
     cout << "range = " << range << endl;

#endif


     for ( UINT16 i = 0; i < num_tic_marks; i++ )
     {
          INT16 offset = Round( ((i * tic_value_step_size )/range) * length ) ;

          SetTicPosition( i, start_pt + Point( offset, 0 ) );

#if defined(__UNIT_TEST__)

     cout << "i, offset = " << i << ", " << offset << endl;

#endif
     }

     coordinates_invalid = FALSE;

}



// ----------------------------------------------------------------------------
//                        XAxis::DrawTicMark( )
// ............................................................................
//
//  low-level routine to draw tic marks and their labels
//
// ----------------------------------------------------------------------------

void XAxis::DrawTicMark( UINT16 I ) const
{
     Point position = ptic_position_array[I];

     INT16 length =  show_grid ? -grid_length_device_coords :
      -tic_length_device_coords;

      GetpParent()->DrawLine( position,
           position + Point( 0, length ) );

     // change position for label

     Size     text_size = GetpParent()->GetTextSize( ptic_label_array[I] );

     position = position + Point( -text_size.Width()/2, text_size.Height() / 2 );

     GetpParent()->DrawText( position,  ptic_label_array[I] );

}


// ----------------------------------------------------------------------------
//                        XAxis::DrawTitle( )
// ............................................................................
// 
//  low-level routine to draw title near the axis
//
// ----------------------------------------------------------------------------

void XAxis::DrawTitle()  const
{
     String str( axis_title ); 

     if ( show_units &&
        ( scale_factor_str.Len() || units.Len() )
        )
          str += scaling_head + scale_factor_str + 
                 String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) + 
                 units + scaling_tail;

     Size     text_size = GetpParent()->GetTextSize( str );

     GetpParent()->DrawText( title_position - Point(text_size.Width()/2, 0 ),
      str );
}


// ------    functions for YAxis follow   ------



// ----------------------------------------------------------------------------
//                        YAxis::ResizeObject( )
// ............................................................................
// 
//  this function is called to resize the object 
//
// ----------------------------------------------------------------------------

void YAxis::ResizeObject(  )
{
     Size char_size = GetpParent()->GetFont().GetSize();

     start_pt = active_area.BottomRight();
     end_pt = active_area.TopRight();

     // center the title in the active area, up 1 character height from bottom

     title_position = active_area.LeftCenter() + Point( (3 * char_size.Width())/2, 0 );

     const UINT16 length = active_area.GetHeight() -1;
     const FLOAT64 range = axis_limits.Range(); 

#if defined(__UNIT_TEST__)

     cout << "\nResizeObject YAxis..." << endl;
     cout << "start pt = " << start_pt.X() << ", " << start_pt.Y() << endl;
     cout << "length = " << length << endl;
     cout << "range = " << range << endl;

#endif


     for ( UINT16 i = 0; i < num_tic_marks; i++ )
     {
          INT16 offset = Round( ((i * tic_value_step_size )/range) * length ) ;

          SetTicPosition( i, start_pt - Point( 0, offset ) );

#if defined(__UNIT_TEST__)

     cout << "i, offset = " << i << ", " << offset << endl;

#endif

     }
     coordinates_invalid = FALSE;
}




// ----------------------------------------------------------------------------
//                        YAxis::DrawTicMark( )
// ............................................................................
// 
//  low-level routine to draw tic marks and their labels
//
// ----------------------------------------------------------------------------

void YAxis::DrawTicMark( UINT16 I ) const
{
     Point position = ptic_position_array[I];

     INT16 length =  show_grid ? grid_length_device_coords :
      tic_length_device_coords;

     GetpParent()->DrawLine( position,
      position + Point( length, 0 ) );

     // change position for label

     Size text_size = GetpParent()->GetTextSize( ptic_label_array[I] );
     Size char_size = GetpParent()->GetFont().GetSize();

     position = position - Point( text_size.Width() + char_size.Width(), text_size.Height() / 2 );

     GetpParent()->DrawText( position,  ptic_label_array[I] );
}






// ----------------------------------------------------------------------------
//                        YAxis::DrawTitle( )
// ............................................................................
// 
//  low-level routine to draw title near the axis
//
// ----------------------------------------------------------------------------

void YAxis::DrawTitle()  const
{
     String str( axis_title ); 

     if ( show_units &&
        ( scale_factor_str.Len() || units.Len() )
        )
          str += scaling_head + scale_factor_str + 
                 String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) + 
                 units + scaling_tail;


     // font is not actually composed of rotated characters -
     // start at the top and work down vertically

     UINT16 len = str.Len();

     Point location;

     for ( UINT16 i = 0; i < len; i++ )
     {
          Size char_size = GetpParent()->GetTextSize( str, i, 1 );

          location = title_position - Point( char_size.Width()/2, 
           ( (len/2) - i ) * char_size.Height() );

          GetpParent()->DrawText( location, str, i, 1 );
     }
}



#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>




void main()
{
     OutputDevice* ptr = NULL;

     XAxis x_axis(  ptr );
     YAxis y_axis(  ptr );
     Rectangle rect1 ( 0, 0, 100, 100 );
     Rectangle rect2 ( 0, 0, 100, 100 );

     x_axis.SetActiveArea( rect1 );
     y_axis.SetActiveArea( rect2 );

     x_axis.SetCharSize( Size( 10,10 ) );
     y_axis.SetCharSize( Size( 10,10 ) );

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "5.900e-6"
           << endl;
     cout << "Expected axis values: min = " << "0.0" 
          << ", max = "                     << "7.0"
          << ", interval = "                << "1.0"
          << ", scale mult = "               << "1e-6"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 5.900e-6 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

      x_axis.ResizeObject();

     if ( AXIS_RC_VALID_DATA == 
     y_axis.SetAxisParameters( Limits( 1.0e-3, 2.0e-3), 1.0e-4, 5 ) )
     {
          cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
                  ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
          cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;
     
     }
     else
     {
          cout << "Y-Axis parameters reset failed" << endl;
     }

      y_axis.ResizeObject();


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "4.400e-1"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "0.50"
          << ", interval = "                << "0.10"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 4.400e-1 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "1.0"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "1.25"
          << ", interval = "                << "0.25"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 1.000 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "15.189"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "17.5"
          << ", interval = "                << "2.5"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 15.189 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "450"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "500"
          << ", interval = "                << "100"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 450 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "8.0e-2"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "10.0"
          << ", interval = "                << "2.5"
          << ", scale mult = "               << "1.00e-2"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 8.0e-2 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "9.3767e-4"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "10.0"
          << ", interval = "                << "2.5"
          << ", scale mult = "               << "1.00e-4"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 9.3767e-4 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "9.3767e-4"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "10.0"
          << ", interval = "                << "2.5"
          << ", scale mult = "               << "1.00e-4"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 9.3767e-4 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "0.1368"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "0.150"
          << ", interval = "                << "0.025"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 0.1368 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "4.4309e-4"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "5.0"
          << ", interval = "                << "1.0"
          << ", scale mult = "               << "1e-4"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 4.4309e-4 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "500" 
           << ", max = "                    << "1372.3"
           << endl;
     cout << "Expected axis values: min = " << "250"
          << ", max = "                     << "1500"
          << ", interval = "                << "250"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 500, 1372.3 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "4.1025" 
           << ", max = "                    << "18.376"
           << endl;
     cout << "Expected axis values: min = " << "2.5"
          << ", max = "                     << "20.0"
          << ", interval = "                << "2.5"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 4.1025, 18.376 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.00" 
           << ", max = "                    << "25.33"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "28.0"
          << ", interval = "                << "4.0"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 25.33 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0" 
           << ", max = "                    << "2.70e-4"
           << endl;
     cout << "Expected axis values: min = " << "0.00"
          << ", max = "                     << "2.80"
          << ", interval = "                << "0.40"
          << ", scale mult = "               << "1e-4"
          << endl;
     x_axis.SetDataLimits( Limits( 0, 2.70e-4 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "300" 
           << ", max = "                    << "500"
           << endl;
     cout << "Expected axis values: min = " << "280"
          << ", max = "                     << "520"
          << ", interval = "                << "40"
          << ", scale mult = "               << "1"
          << endl;
     x_axis.SetDataLimits( Limits( 300, 500  ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "2.1497e-5" 
           << ", max = "                    << "4.5825e-5"
           << endl;
     cout << "Expected axis values: min = " << "2.00"
          << ", max = "                     << "4.80"
          << ", interval = "                << "0.40"
          << ", scale mult = "               << "1e-5"
          << endl;
     x_axis.SetDataLimits( Limits( 2.1497e-5,  4.5825e-5 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "9.9782e16"
           << endl;
     cout << "Expected axis values: min = " << "0"
          << ", max = "                     << "1.25"
          << ", interval = "                << "0.25"
          << ", scale mult = "               << "1e17"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 9.9782e16 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "2.171e-6"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "2.40"
          << ", interval = "                << "0.40"
          << ", scale mult = "               << "1e-6"
          << endl;
     x_axis.SetDataLimits( Limits( 0.0, 2.171e-6 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nData limits : min =  "      << "0.0" 
           << ", max = "                    << "2.56e-3"
           << endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "2.80"
          << ", interval = "                << "0.40"
          << ", scale mult = "               << "1e-3"
          << endl;
     x_axis.SetDataLimits( Limits( 0, 2.56e-3 ) );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     enum AXIS_RC rc;



     cout << "\nResetting Axis parameters to Limits( 1.0e-3, 2.0e-3), 1.0e-4, 5 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 1.0e-3, 2.0e-3), 1.0e-4, 5 );

     if ( AXIS_RC_VALID_DATA == rc )
     {
          cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
                  ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
          cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;
     
     }
     else
     {
          cout << "Axis paramters reset failed; rc = " << (int) rc << endl;
     }



     cout << "\nResetting Axis parameters to Limits( 0.0, 8.25e-3), 1.425e-3, 3 " <<  endl;

     rc = x_axis.SetAxisParameters( Limits( 0.0e-3, 8.25e-3), 1.425e-3, 3 );

     if ( AXIS_RC_VALID_DATA == rc )
     {
          cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
                  ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
          cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;
     }
     else
     {
          cout << "Axis paramters reset failed; rc = " << (int) rc << endl;
     }

     cout << "\nResetting Axis parameters to defaults" <<  endl;
     cout << "Expected axis values: min = " << "0.0"
          << ", max = "                     << "2.80"
          << ", interval = "                << "0.40"
          << ", scale mult = "               << "1e-3"
          << endl;
     x_axis.ResetAxisParameters( );
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "\nTESTING ERROR DETECTION" <<  endl;

     cout << "Resetting Axis parameters to Limits(  8.25e-3, 0.0 ), 1.425e-3, 3 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 8.25e-3, 0.0 ), 1.425e-3, 3 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;

     cout << "Resetting Axis parameters to Limits(  0, 8.25e-3 ), -1.425e-3, 3 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 0, 8.25e-3 ), -1.425e-3, 3 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;

     cout << "Resetting Axis parameters to Limits(  0, 8.25e-3 ), 1.425e-2, 3 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 0, 8.25e-3 ), 1.425e-2, 3 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;

     cout << "Resetting Axis parameters to Limits(  0, 80 ), 2.0, 1 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 0, 80 ), 2.0, 1 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;

     cout << "Resetting Axis parameters to Limits(  0, 80 ), 2.5, 2 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 0, 80 ), 2.5, 2 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;

     cout << "Resetting Axis parameters to Limits(  0, 80 ), 25, 0 " <<  endl;
     rc = x_axis.SetAxisParameters( Limits( 0, 80 ), 25, 0 );
     cout << "Axis parameter reset  rc = " << (int) rc << endl;
     cout << "Axis Limits : min = " << x_axis.GetAxisLimits().Min() << 
             ", max = " << x_axis.GetAxisLimits().Max() <<  endl;
     cout << "Tic interval = " << x_axis.GetTicInterval() <<  endl;


}



#endif



