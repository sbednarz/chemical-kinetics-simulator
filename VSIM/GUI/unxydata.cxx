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
//  UNXYDATA.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the UniformXyDataArray class, to be used to contain an
//  array of X-Y data points in a floating point format. This class accepts
//  FloatArrays of values for rX and rY which may be nonuniformly spaced. It 
//  creates and provides access to modified arrays equivalent to rX and rY 
//  (i.e the data points are superimposable on the curve defined by the set 
//  of rX,rY pairs) but which are uniformly and evenly spaced in the X 
//  dimension within the range of rX. The user may specify the interval 
//  between X values or may specify the total number of points within the 
//  range.
//
//  CREATED : 1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "unxydata.hxx"

#endif


#include <math.h>




// ----------------------------------------------------------------------------
//                          UniformXyDataArray::UniformXyDataArray()
// ............................................................................
// 
//	constructor
//
// ----------------------------------------------------------------------------

UniformXyDataArray::UniformXyDataArray( const FloatArray& rX, const FloatArray& rY, UINT16 NumPoints ) :
rx( rX ),
ry( rY ),
uniform_x(),
uniform_y(),
num_pts( 0 ),
interval( 0.0 )
{
     InitializeArrays( NumPoints );
}







// ----------------------------------------------------------------------------
//                          UniformXyDataArray::UniformXyDataArray()
// ............................................................................
// 
//	constructor
//
// ----------------------------------------------------------------------------

UniformXyDataArray::UniformXyDataArray( const FloatArray& rX, const FloatArray& rY, FLOAT64 Spacing ) :
rx( rX ),
ry( rY ),
uniform_x(),
uniform_y(),
num_pts( 0 ),
interval( 0.0 )
{
     InitializeArrays( Spacing );
}






// ----------------------------------------------------------------------------
//             UniformXyDataArray::ChangeNumPoints(  ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

UINT16  UniformXyDataArray::ChangeNumPoints( UINT16 NewNumPts ) 
{ 
	UINT16 old_val = GetNumPoints();
     InitializeArrays( NewNumPts ); 
     return old_val;
}





// ----------------------------------------------------------------------------
//             UniformXyDataArray::ChangeSpacing(  ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

FLOAT64 UniformXyDataArray::ChangeSpacing( FLOAT64 NewSpacing ) 
{ 
   	FLOAT64 old_val = GetSpacing();
     InitializeArrays( NewSpacing ); 
     return old_val;
} 




// ----------------------------------------------------------------------------
//             UniformXyDataArray::InitializeArrays( UINT16  ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UniformXyDataArray::InitializeArrays( UINT16 NumPoints )
{
	FLOAT64 spacing = rx.GetLimits().Range() / ( (FLOAT64) (NumPoints-1) );
     InitializeArrays( spacing );
}






// ----------------------------------------------------------------------------
//             UniformXyDataArray::InitializeArrays( FLOAT64 ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UniformXyDataArray::InitializeArrays( FLOAT64 Spacing )
{
     interval = Spacing;

	Limits x_limits = rx.GetLimits();

	FLOAT64 range =   x_limits.Range();

	num_pts = ((UINT16) ceil(range/Spacing) ) + 1;  //round up range/spacing, add one for init pt

     uniform_x.Initialize( num_pts );     
     uniform_y.Initialize( num_pts );     

	// get first point 

	uniform_x[0] = rx[0];
	uniform_y[0] = ry[0];

     UINT16 upper_index = 0;

     // derive a value for each of the new x,y points 

	for  ( UINT16 i = 1; i < num_pts; i++ )
	{
          FLOAT64 new_y;
   		FLOAT64 new_x = x_limits.Min() + ( i * Spacing ); 

          if ( new_x > x_limits.Max() ) 
          {
			new_x = x_limits.Max();
			new_y = ry.GetLimits().Max();
          } 
          else 
          {
	     	// find point in rX immediately below new_x
      	
      		while ( new_x > rx[upper_index] )
                     upper_index++;
      
               UINT16 lower_index = upper_index-1;
      
      		// interpolate to get value for new_y;
      
               FLOAT64 slope = (ry[upper_index] - ry[lower_index]) / (rx[upper_index] - rx[lower_index]);

               new_y = ry[lower_index] + ( slope * ( new_x - rx[lower_index] ) );
          } 
		
   		uniform_x[i] = new_x;
   		uniform_y[i] = new_y;
   	}
     
}







#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>


#include "xydata.hxx"

void main()
{
     FloatArray x(100);
     FloatArray y(100);

     for ( UINT16 i = 0 ; i < x.GetArraySize(); i++ )
     {
          x[i] = (FLOAT64) i; 
          y[i] = x[i] * x[i];
     }


	UINT16 points = 7;
     UniformXyDataArray xy( x, y, points );

	XyData data( xy.GetXDataArray(), xy.GetYDataArray() );

     cout << "with default bounding rect " << endl;

	for ( UINT16 j = 0; j < points; j++ )
     {
         Point pt = data.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }

     data.ChangeBoundingRectangle( Rectangle( 0, (short)(99*99)-1, 98, 0)  );

	for ( j = 0; j < points; j++ )
     {
         Point pt = data.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }

     data.ChangeBoundingRectangle( Rectangle( 0, (short)(99*99)-1, 0, 0)  ); 

     cout << "with width=1 bounding rect " << endl;
	for ( j = 0; j < points; j++ )
     {
         Point pt = data.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }

     x.Initialize( 12 );
     y.Initialize( 12 );

     for ( i = 0 ; i < x.GetArraySize(); i++ )
     {
          x[i] = (FLOAT64) i; 
          y[i] = x[i] * x[i];
     }

     points = 150;
     UniformXyDataArray xy2( x, y, points );

	XyData data2( xy2.GetXDataArray(), xy2.GetYDataArray() );

     data2.ChangeBoundingRectangle( Rectangle( 0, (short)(12*12)-1, 11, 0)  );

     cout << endl << "with many interpolated points" << endl;
	for ( j = 0; j < points; j++ )
     {
         Point pt = data2.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }


    points = 10;
    xy2.ChangeNumPoints( points );
    data2.Invalidate();
	for ( j = 0; j < points; j++ )
     {
         Point pt = data2.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }


    xy2.ChangeSpacing( 3.0 );
    data2.Invalidate();
    points = xy2.GetNumPoints();

	for ( j = 0; j < points; j++ )
     {
         Point pt = data2.AsDiscretePoints().GetPoint( j ) ;

          cout << "j =" << j << ", x =" << pt.X() << ", y =" << pt.Y() << endl;
     }

}


#endif



