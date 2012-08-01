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
//  XYDATA.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module file defines the XyData class, to be used contain an array of 
//  X-Y data points in a form suitable for plotting ( this includes conversion 
//  from floating point to integer form, scaling, clipping, etc.
//
//  CREATED : 6.26.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "assert.hxx"
#include "xydata.hxx"
#include "round.hxx"

#endif

#include <limits.h>


const UINT32 MAX_POLYGON_SIZE     = (UINT32) USHRT_MAX;
const UINT16 DEFAULT_MAX_NUM_PTS  =  1000;
const UINT16 MIN_NUM_PTS          = 2;
const UINT16 MAX_NUM_PTS          = USHRT_MAX;

const short DEFAULT_LEFT          = 0;
const short DEFAULT_TOP           = 0;
const short DEFAULT_RIGHT         = 99;
const short DEFAULT_BOTTOM        = 99;

const UINT16 LEFT_OF_WINDOW       = (1<<1);
const UINT16 RIGHT_OF_WINDOW      = (1<<2);
const UINT16 BELOW_WINDOW         = (1<<3);
const UINT16 ABOVE_WINDOW         = (1<<4);




// ----------------------------------------------------------------------------
//                                 XyData::XyData( )
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

XyData::XyData( const FloatArray& rX, const FloatArray& rY ) : 
line_polygon(),
point_polygon(),
max_num_plot_pts( DEFAULT_MAX_NUM_PTS ),
interval( 1 ),
x_scaling_factor( 1.0 ),
y_scaling_factor( 1.0 ),
rx( rX ), 
ry( rY ),
x_limits( rX.GetLimits() ), 
y_limits( rY.GetLimits() ),
init_x_limits( x_limits ),
init_y_limits( y_limits ),
bounding_rect( DEFAULT_LEFT, DEFAULT_TOP, DEFAULT_RIGHT, DEFAULT_BOTTOM ),
line_polygon_invalid( TRUE ),
point_polygon_invalid( TRUE )
{
     ASSERT( rx.GetArraySize() == ry.GetArraySize() );
}





// ----------------------------------------------------------------------------
//                           XyData::SetMaxNumPlotPts()
// ............................................................................
// 
//  reset max num pts allowable - check for invalid values while we are at it
//  returns TRUE if values were valid and were accepted, FALSE if invalid 
//
// ----------------------------------------------------------------------------

BOOL8 XyData::SetMaxNumPlotPts( UINT32 NumPts ) 
{ 
     if ( NumPts >= MIN_NUM_PTS )
     {
          max_num_plot_pts = NumPts; 
          Invalidate();
          return TRUE;
     }
     else
          return FALSE;
}



// ----------------------------------------------------------------------------
//                            XyData::ResetDataLimits()
// ............................................................................
// 
//  resets the x and y data limits to the minimum and maximum of the data arrays
//
// ----------------------------------------------------------------------------

void XyData::ResetDataLimits()
{

     init_x_limits = rx.GetLimits();
     init_y_limits = ry.GetLimits();
     x_limits = init_x_limits;
     y_limits = init_y_limits;

     Invalidate();
}




// ----------------------------------------------------------------------------
//                            XyData::AsPolyLine( )
// ............................................................................
// 
//  returns a Polygon containing the XY data in a form ready for plotting as 
//  as series of line segments to form a polyline. checks for internal 
//  consistency first and rescales internal data if necessary 
//
// ----------------------------------------------------------------------------

const Polygon& XyData::AsPolyLine( )
{
     if ( line_polygon_invalid )
          ScaleDataForLinePlot();

     return line_polygon;
}





// ----------------------------------------------------------------------------
//                            XyData::AsDiscretePoints( )
// ............................................................................
// 
//  returns a Polygon containing the XY data in a form ready for plotting as 
//  as series of individual points unconnected by line segments. Checks for 
//  internal consistency first and rescales internal data if necessary 
//
// ----------------------------------------------------------------------------

const Polygon& XyData::AsDiscretePoints( )
{
     if ( point_polygon_invalid )
          ScaleDataForPointPlot();

     return point_polygon;
}





// ----------------------------------------------------------------------------
//                     XyData::ChangeBoundingRectangle( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

Rectangle XyData::ChangeBoundingRectangle( const Rectangle& rRect )
{
     // the algorithm is set up for 0,0 to be in lower left corner of 
     // rectangle, opposite from what StarView uses. we swap Top and Bottom
     // values in the rects when we receive it for internal use, and swap them 
     // again when we return it through this function 

     // copy the current bounding_rect
     Rectangle old_rect = bounding_rect;

     // swap the top and bottom 

     INT16 top = old_rect.Top();
     INT16 bottom = old_rect.Bottom();

     old_rect.Top() = bottom;
     old_rect.Bottom() = top;

     // old_rect is ready to return - now get new rectangle

     bounding_rect = rRect;

     top = bounding_rect.Top();
     bottom = bounding_rect.Bottom();

     bounding_rect.Top() = bottom;
     bounding_rect.Bottom() = top;

     Invalidate();

     return old_rect;
}





// ----------------------------------------------------------------------------
//                     XyData::AreaCode( )
// ............................................................................
// 
//  calculates a coded value indicating where the x/y data point of the 
//  specified index lies relative to the defined x/y limits
//
// ----------------------------------------------------------------------------

UINT16 XyData::AreaCode( const UINT32 Index ) const 
{
     UINT16 code = 0;

     if ( rx[Index ] < x_limits.Min() )
          code = LEFT_OF_WINDOW;
     else
          if ( rx[Index] > x_limits.Max() )
               code = RIGHT_OF_WINDOW;

     if ( ry[ Index] < y_limits.Min() )
          code |= BELOW_WINDOW;
     else
          if ( ry[Index] > y_limits.Max() )

               code |= ABOVE_WINDOW;

     return code;
}




// ----------------------------------------------------------------------------
//                     XyData::AreaCode( )
// ............................................................................
// 
//  calculates a coded value indicating where the x/y data point of the 
//  specified index lies relative to the defined x/y limits
//
// ----------------------------------------------------------------------------

UINT16 XyData::AreaCode( const FLOAT64 X, const FLOAT64 Y ) const 
{
     UINT16 code = 0;

     if ( X < x_limits.Min() )
          code = LEFT_OF_WINDOW;
     else
          if ( X > x_limits.Max() )
               code = RIGHT_OF_WINDOW;

     if ( Y < y_limits.Min() )
          code |= BELOW_WINDOW;
     else
          if ( Y > y_limits.Max() )

               code |= ABOVE_WINDOW;

     return code;
}








// ----------------------------------------------------------------------------
//                     XyData::ScaleData( )
// ............................................................................
// 
//  calculates a Point that lies within the bounding rectangle , scaled as 
//  appropriate. The input is an index to a set of x/y data 
//
// ----------------------------------------------------------------------------

Point XyData::ScaleData( UINT32 Index ) const 
{
     return Point( Round(( rx[ Index ] - x_limits.Min() ) * x_scaling_factor ) + bounding_rect.Left(), 
                   Round(( ry[ Index ] - y_limits.Min() ) *y_scaling_factor ) + bounding_rect.Top() );
}





// ----------------------------------------------------------------------------
//                     XyData::ScaleData( )
// ............................................................................
// 
//  calculates a Point that lies within the bounding rectangle , scaled as 
//  appropriate. The input is a pair of FLOAT64 values representing X and Y  
//
// ----------------------------------------------------------------------------

Point XyData::ScaleData( FLOAT64 X, FLOAT64 Y ) const 
{
     return Point( Round(( X - x_limits.Min() ) * x_scaling_factor ) + bounding_rect.Left(), 
                   Round(( Y - y_limits.Min() ) *y_scaling_factor ) + bounding_rect.Top() );
}





// ----------------------------------------------------------------------------
//                     XyData::ScaleDataForLinePlot( )
// ............................................................................
// 
// this updates the internal data so that the line_polygon
// are correctly scaled for the current min/max limits and bounding rectangle
//
// This fcn is based on a Cohen-Sutherland type clipping algorithm         
// ref Hearn & Baker, "Computer Graphics", Prentice Hall 1986, pp 128-134  
//
// ----------------------------------------------------------------------------

void XyData::ScaleDataForLinePlot()
{
     ASSERT ( !bounding_rect.IsEmpty() && ( x_limits.Range() > 0.0 ) && ( y_limits.Range() > 0.0 ) );

     UINT16 area_code1;
     UINT16 area_code2;

     // set polygon size to maximum - we will reset to actual size later 

     line_polygon.ChangeSize( MAX_POLYGON_SIZE );

     // We substract 2 from polygon_size since  we may add two members 
     // to the array during the main loop and not want to over-run 
     // Point array internal to the Polygon 

     const UINT32 POLYGON_SIZE = MAX_POLYGON_SIZE - 2;

     UINT32 current_data_member = 0;       // index into the rx, ry data arrays 

     UINT16 polygon_posn = 0;              // index into line_polygon 

     if ( rx.GetArraySize() > 1)
     {
          const  UINT32 LAST_DATA_ELEMENT =  rx.GetArraySize() - 1;   
     
          const UINT32 INTERVAL = GetStepInterval( );
     
          CalcScalingFactors();
     
          // calc init value for area_codes
     
          area_code1 = AreaCode( current_data_member );
          area_code2 = AreaCode( current_data_member + INTERVAL );
     
          // test for trivial case where both points are within window
          // ie where area_code1 == area_code2 == 0
     
          if ( ! ( area_code1 | area_code2 ) )
          {
               // accept the line segment                                        
               // calculate the position on the viewport from data               
               // for the first point
     
               line_polygon[polygon_posn++] = ScaleData( current_data_member );
     
               // and for the second 
     
               current_data_member += INTERVAL;
     
               line_polygon[polygon_posn++] = ScaleData( current_data_member );
     
          }
          else
          {
     
               // If not initially accepted. then test for any matching bits in area codes.
               // If any are found then the line segment connecting the points will not pass 
               // through the viewport, so we can reject the segment and move on
     
               if ( area_code1 & area_code2 )
               {
                    current_data_member += INTERVAL;
               }
               else
               {
                    // If we enter this block then there is a possibility that a segment of the 
                    // line connecting the first two points is visible. We need to search for 
                    // an intersection of the line between the points and the viewport if any 
     
                    FLOAT64 x1 = rx[ current_data_member];
                    FLOAT64 y1 = ry[ current_data_member]; 
     
                    current_data_member += INTERVAL;
     
                    FLOAT64 x2 = rx[ current_data_member]; 
                    FLOAT64 y2 = ry[ current_data_member]; 
     
                    // Test whether there is a portion of the line segment visible 
     
                    if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2 ) )
                    {
                         // now calculate and save the points
     
                         line_polygon[polygon_posn++] = ScaleData( x1, y1 ); 
     
     
                         line_polygon[polygon_posn++] = ScaleData( x2, y2 ); 
                    }
     
               }
          }
     
          // Repeat the same sequence of tests for the rest of the points.
          // Note that the clipping process can actually place more points in the
          // polygon than there are data elements in the x/y FloatArrays. This is not
          // likely, however, and would only occur when there are many lines clipped
          // and where only one x/y data member was excluded during the clipping action
          // but two points were added to the Polygon to represent where the line 
          // crosses the axis. We test for over-running the array size at any rate                 */
     
          // index next member in data arrays
     
          current_data_member += INTERVAL;
     
          for ( ; 
                ( current_data_member <= LAST_DATA_ELEMENT ) && ( polygon_posn <= POLYGON_SIZE ); 
                current_data_member += INTERVAL 
              )
          {
               // 2nd point will become first point in next line seg   
     
               area_code1 = area_code2;
     
               /* calc init value for area_code2                                 */
     
               area_code2 = AreaCode( current_data_member );
     
               // test for trivial case where both points are within window 
               // ie where area_code1 == area_code2 == 0
     
               if ( ! ( area_code1 | area_code2 ) )
               {
     
                    // accept the line segment
     
                    line_polygon[polygon_posn++] = ScaleData( current_data_member );
     
                    // and move on to next x/y data element
     
                    continue;
               }
     
               // If not initially accepted. then test for any matching bits in area codes.
               // If any are found then the line segment connecting the points will not pass 
               // through the viewport, so we can reject the segment and move on
     
               if ( area_code1 & area_code2 )
               {
                    continue;
               }
     
               // If we made it here then there is a possibility that a segment of the 
               // line connecting the first two points is visible. We need to search for 
               // an intersection of the line between the points and the viewport if any 
     
               // save the x,y pairs as temporary variables as their values
               // may be modified during the clipping operation contained
               // in fcn VisibleLineSegment
     
               FLOAT64 x1 = rx[ current_data_member - INTERVAL];
               FLOAT64 y1 = ry[ current_data_member - INTERVAL]; 
     
               FLOAT64 x2 = rx[ current_data_member]; 
               FLOAT64 y2 = ry[ current_data_member]; 
     
               // Test whether there is a portion of the line segment visible 
     
               if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2 ) )
               {
                    // now calculate and save the points
     
                    line_polygon[polygon_posn++] = ScaleData( x1, y1 ); 
     
                    line_polygon[polygon_posn++] = ScaleData( x2, y2 ); 
               }
     
               continue;
     
          }    // end while 
     
     
          // now catch the last point in case we did not get it in the above processing
     
          current_data_member -= INTERVAL;
     
          if ( ( current_data_member < LAST_DATA_ELEMENT ) &&  ( polygon_posn <= POLYGON_SIZE ) )
          {
     
               // previous point will become first point in next line seg       
     
               area_code1 = area_code2;
     
               // calc area_code for last data element 
     
               area_code2 = AreaCode( LAST_DATA_ELEMENT );
     
               // test for trivial case where both points are within window
               // ie where area_code1 == area_code2 == 0 
     
               if ( ! ( area_code1 | area_code2 ) )
               {
                    // accept the line segment                                   
     
                    line_polygon[polygon_posn++] = ScaleData( LAST_DATA_ELEMENT );
               }
               else
               {
                    // if not accepted then test for any matching bits      
                    // if any are found then the line segment connecting the
                    // points will not pass through the viewport, so we     
                    // reject the segment and move onto next segment        
     
                    if ( area_code1 & area_code2 )
                         ;                  // reject the segment
                    else
                    {
                         // if we made it to this part of the function then   
                         // we need to search for an intersection of the line 
                         // between the points and the viewport               
                         
                         FLOAT64 x1 = rx[ current_data_member ]; // use previous data element 
                         FLOAT64 y1 = ry[ current_data_member ]; 
                         FLOAT64 y2 = rx[ LAST_DATA_ELEMENT ];  
                         FLOAT64 x2 = ry[ LAST_DATA_ELEMENT ];  
     
                         if (  VisibleLineSegment( x1, y1, x2, y2, area_code1, area_code2 ) ) 
                         {
                              line_polygon[polygon_posn++] = ScaleData( x1, y1 ); 
     
                              line_polygon[polygon_posn++] = ScaleData( x2, y2 ); 
     
                         }
                    }                       // end else
               }
          }
     
     }

     // reset polygon size to the actual size used 

     line_polygon.ChangeSize( polygon_posn );

     // line_polygon state is now consistent - reset flag 

     line_polygon_invalid = FALSE;
}






//---------------------------------------------------------------------------
//                        XyData::VisibleLineSegment( )
//...........................................................................
//                                                                          
// this function returns TRUE if the line segment defined by X1, X2 and     
// Y1, Y2 contains a segment visible within the x/y limitsrt bounded by         
// x_limits and y_limits. Otherwise it returns FALSE. If the result is     
// TRUE then X1, Y1, X2, Y2 contain the points which define the visible     
// segment                                                                  
//                                                                          
//---------------------------------------------------------------------------

BOOL8 XyData::VisibleLineSegment( FLOAT64& rX1, FLOAT64& rY1, FLOAT64& rX2, FLOAT64& rY2, 
                          UINT16 AreaCode1, UINT16 AreaCode2 )
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

               rY1 += ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (x_limits.Min() - rX1) );
               rX1 = x_limits.Min();

               AreaCode1 = AreaCode( rX1, rY1 );
               continue;
          }

          if (AreaCode1 & RIGHT_OF_WINDOW )
          {
               // the line crosses the right border of the viewport;
               // calculate the intersection with the right border

               rY1 += ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (x_limits.Max() - rX1) );
               rX1 = x_limits.Max();

               AreaCode1 = AreaCode( rX1, rY1 );
               continue;
          }

          if (AreaCode1 & ABOVE_WINDOW )
          {
               // the line crosses the top border of the vieport;
               // calculate the intersection with the top border

               rX1 += ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (y_limits.Max() - rY1) );
               rY1 = y_limits.Max();

               AreaCode1 = AreaCode( rX1, rY1 );
               continue;
          }

          if (AreaCode1 & BELOW_WINDOW )
          {
               // the line crosses the bottom border of the vieport;
               // calculate the intersection with the bottom border

               rX1 += ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (y_limits.Min() - rY1) );
               rY1 = y_limits.Min();

               AreaCode1 = AreaCode( rX1, rY1 );
          }
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

               rY2 = rY1 + ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (x_limits.Min() - rX1) );
               rX2 = x_limits.Min();

               AreaCode2 = AreaCode( rX2, rY2 );
               continue;
          }

          if (AreaCode2 & RIGHT_OF_WINDOW )
          {
               // the line crosses the right border of the vieport;
               // calculate the intersection with the right border

               rY2 = rY1 + ( ( ( rY2 - rY1 ) / ( rX2 - rX1 ) ) * (x_limits.Max() - rX1) );
               rX2 = x_limits.Max();

               AreaCode2 = AreaCode( rX2, rY2 );
               continue;
          }

          if (AreaCode2 & ABOVE_WINDOW )
          {
               // the line crosses the top border of the vieport;
               // calculate the intersection with the top border

               rX2 = rX1 + ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (y_limits.Max() - rY1) );
               rY2 = y_limits.Max();

               AreaCode2 = AreaCode( rX2, rY2 );
               continue;
          }

          if (AreaCode2 & BELOW_WINDOW )
          {
               // the line crosses the bottom border of the vieport;
               // calculate the intersection with the bottom border

               rX2 = rX1 + ( ( ( rX2 - rX1 ) / ( rY2 - rY1 ) ) * (y_limits.Min() - rY1) );
               rY2 = y_limits.Min();

               AreaCode2 = AreaCode( rX2, rY2 );
          }
     }

     // we have successfully pushed the points  and a visible line segment remains 

     return TRUE;  
}





// ----------------------------------------------------------------------------
//                     XyData::ScaleDataForPointPlot( )
// ............................................................................
// 
// this updates the internal data so that the point_polygon
// are correctly scaled for the current min/max limits and bounding rectangle
//
// We assume in the coordinate calculation that 0,0 is at the left top of the 
// bounding rectangle
//
// ----------------------------------------------------------------------------

void XyData::ScaleDataForPointPlot()
{
     ASSERT ( !bounding_rect.IsEmpty() && ( x_limits.Range() > 0.0 ) && ( y_limits.Range() > 0.0 ) );

     UINT32 polygon_posn      = 0;       // index position in point_polygon
     UINT32 current_data_member = 0;       // index into the rx, ry data arrays 

     // we use this  to tell when we are at the end of the array 

     const  UINT32 LAST_DATA_ARRAY_MEMBER =  rx.GetArraySize() - 1;   

     // INTERVAL is the delta index between data points - if we are not 
     // using every data point then this will be greater than 1 

     const UINT32 INTERVAL = GetStepInterval( );

     CalcScalingFactors();

     // set point_polygon to its maximum size in the event we use all the 
     // array elements - we will shrink it later if appropriate 

     point_polygon.ChangeSize( MAX_POLYGON_SIZE );

     // use this to flag special handling of last data point 

     BOOL8    doing_last_pt = FALSE;

     // loop through data, scale and clip as requested by bounding_rect and max/min x, y limits

     while ( ( current_data_member <= LAST_DATA_ARRAY_MEMBER ) &&
             ( polygon_posn <= MAX_POLYGON_SIZE ) 
           )
     {
          // if within the limits then add to the point polygon

          if ( x_limits.Inside( rx[current_data_member] ) && 
               y_limits.Inside( ry[current_data_member] ) 
             )
          {
               point_polygon[ polygon_posn ] = ScaleData( current_data_member );

                polygon_posn++;
          }


          if ( doing_last_pt )
          {
               // we are done - break out of loop 

               break;
          }
          else
          {
               // otherwise point index at the next member of the data array

               current_data_member += INTERVAL;

               // if that points at the end of data or past the end of the 
               // data, then take ethe last point 

               if ( current_data_member >= LAST_DATA_ARRAY_MEMBER )
               {
                    current_data_member = LAST_DATA_ARRAY_MEMBER;
                    doing_last_pt = TRUE;
               }
          }

     }

     // reset polygon size to the actual size used 

     point_polygon.ChangeSize( polygon_posn );

     // point_polygon state is now consistent - reset flag 

     point_polygon_invalid = FALSE;
}



// ----------------------------------------------------------------------------
//                     XyData::GetStepInterval( )
// ............................................................................
// 
//  calculates the step interval between x/y data points given the maximum
//  number of data points and the size of the x/y data FloatArrays
//
// ----------------------------------------------------------------------------

UINT32 XyData::GetStepInterval( )
{
     UINT32 total_num_points = GetNumXYDataPoints();

     if ( total_num_points <= max_num_plot_pts )
          return 1;
     else
     {
          UINT32 interval = total_num_points / max_num_plot_pts;

          // is there a remainder add one since we will always include the final point

          if ( total_num_points % max_num_plot_pts ) 
               interval++;

          return interval;
     }
}


void XyData::CalcScalingFactors()
{
     x_scaling_factor = bounding_rect.GetWidth() / GetXDataLimits().Range();
     y_scaling_factor = bounding_rect.GetHeight() / GetYDataLimits().Range();
}






#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>

void main()
{
     FloatArray x(100);
     FloatArray y(100);

     for ( UINT16 i = 0 ; i < x.GetArraySize(); i++ )
     {
          x[i] = (FLOAT64) i; 
          y[i] = x[i] * x[i];
     }

     XyData xy( x, y );

     Polygon poly( xy.AsPolyLine() );


     cout << "default parameters" << endl;
     cout << "polyline size =" << poly.GetSize() << endl;


     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

     cout << "setting max num plot pts to 11 " << endl;
     xy.SetMaxNumPlotPts( 11 );

     poly = Polygon( xy.AsPolyLine() );

     cout << "polyline size =" << poly.GetSize() << endl;
     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

     xy.SetMaxNumPlotPts( 2000 );

     poly = Polygon( xy.AsDiscretePoints() );


     cout << "\npolypoints size =" << poly.GetSize() << endl;


     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

     cout << "\n\nchanged bounding rect" << endl;
     xy.ChangeBoundingRectangle( Rectangle( 0, 0, 999, 999 ) );

     poly = Polygon( xy.AsPolyLine() );

     cout << "polyline size =" << poly.GetSize() << endl;


     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

     cout << "\n\nchanged y data limits" << endl;
     xy.SetYDataLimits( Limits( 0, 5 ) );
     poly = Polygon( xy.AsPolyLine() );

     cout << "polyline size =" << poly.GetSize() << endl;


     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

     poly = Polygon( xy.AsDiscretePoints() );


     cout << "polypoints size =" << poly.GetSize() << endl;


     for ( i = 0 ; i < poly.GetSize(); i++ )
     {
          cout << "i =" << i << ", x =" << poly[i].X() << ", y =" << poly[i].Y() << endl;
     }

}



#endif



