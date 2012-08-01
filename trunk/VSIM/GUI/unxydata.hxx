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
//  UNXYDATA.HXX - a header file for the VSIM User interface 
// ............................................................................
//
//  This header defines the UniformXyDataArray class, to be used to contain an
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


#if !defined(__UNXYDATA_HXX__)
#define __UNXYDATA_HXX__


#include "flotarry.hxx"


const UINT16 DEFAULT_NUM_POINTS = 16;

class UniformXyDataArray
{
protected:	
	const FloatArray& rx;
	const FloatArray& ry;

	FloatArray        uniform_x;
	FloatArray        uniform_y;

	UINT16            num_pts;
     FLOAT64           interval;

     void              InitializeArrays( FLOAT64 Spacing );
     void              InitializeArrays( UINT16  NumPoints );

public:
			
                       UniformXyDataArray( const FloatArray& rX, const FloatArray& rY, UINT16 NumPoints = DEFAULT_NUM_POINTS );
                       UniformXyDataArray( const FloatArray& rX, const FloatArray& rY, FLOAT64 Spacing );

	UINT16            GetNumPoints() const { return num_pts; }
	FLOAT64           GetSpacing() const { return interval; }
	UINT16            ChangeNumPoints( UINT16 NewNumPts );
	FLOAT64           ChangeSpacing( FLOAT64 NewSpacing );

	const FloatArray& GetXDataArray( ) const { return uniform_x; }
	const FloatArray& GetYDataArray( ) const { return uniform_y; }
};

#endif
