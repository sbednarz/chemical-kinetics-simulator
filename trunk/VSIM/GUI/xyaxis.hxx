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
//  XYAXIS.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the XyAxis class for
//  the VSIM user interface. This is the base class from which 
//  XAxis and YAxis classes are derived. 
//  It includes info about drawing the object on the screen or other 
//  output device
//
//  CREATED : 6.28.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__XYAXIS_HXX__)
#define __XYAXIS_HXX__

#include <float.h>

#include "axis.hxx"
#include "limits.hxx"
#include "vsimtype.hxx"



enum AXIS_RC { AXIS_RC_VALID_DATA, AXIS_RC_MINMAX_ERROR, AXIS_RC_TIC_RANGE_ERROR,
               AXIS_RC_TOO_MANY_TICS };




class XyAxis : public Axis
{
protected:
     String        units;
     String        scale_factor_str;
     const String  scaling_head;
     const String  scaling_tail;

     Limits        data_limits;
     Limits        axis_limits;

     FLOAT64       tic_value_step_size;
     FLOAT64       scaling_multiplier;

     BOOL8         show_grid;
     BOOL8         labels_invalid;
     BOOL8         show_units;

     UINT16        precision;   // number of digits to right of decimal place in tic mark label
     UINT16        total_num_digits;

     INT16         grid_length_device_coords;

     void          ScaleDataLimits();
     void          AutoSetAxisParameters();
     void          ConstructTicMarkLabels();
     void          MakeExponentStr();

     UINT16        CalcActualNumTicksAndStartingPt( FLOAT64 AxisMin, FLOAT64 AxisMax,
                    FLOAT64* pStartOfTicks, FLOAT64 Divisor );

     FLOAT64       CalcOptimumSpacing( UINT16 MaxMarks, FLOAT64 AxisMin, FLOAT64 AxisMax );

     FLOAT64       CalcMultiplierFactor( FLOAT64 Value );

     UINT16        TrialAxisCalc( FLOAT64 AxisMin, FLOAT64 AxisMax, UINT16 MaxMarks, 
                    FLOAT64* pStartOfTicks, FLOAT64* pEndOfTicks, FLOAT64* pSpacing, 
                    FLOAT64* pMultiplier );

     UINT16        CalcOptimumNumTicks( FLOAT64* pStartOfTicks, FLOAT64* pEndOfTicks,
                    FLOAT64 AxisMin, FLOAT64 AxisMax, FLOAT64* pSpacing, 
                    FLOAT64* pMultiplier );

     void         CalcTicMarkFormat( UINT16* NumDigits, UINT16* NumDecPlaces,
                   FLOAT64 Max, FLOAT64 Spacing );
public:
                                 XyAxis( OutputDevice* pParent, const Limits& DataLimits );
                                 XyAxis( OutputDevice* pParent );
     virtual                    ~XyAxis() {};

     virtual PLOT_OBJECT_TYPE    Type() const { return XY_AXIS_OBJECT; }

     virtual BOOL8         EditObject( const Point Position );

     void                  SetDataLimits( const Limits& rNewLimits );

     void                  SetUnits( const String& rStr ) {units = rStr; }

     const Limits          GetAxisLimits() const
                           {
                                return Limits( axis_limits.Min() * scaling_multiplier,
                                               axis_limits.Max() * scaling_multiplier
                                             );
                           }


     FLOAT64               GetTicInterval()  const { return tic_value_step_size * scaling_multiplier; }
     UINT16                GetTicLabelPrecision()    const { return precision; }
     UINT16                GetTicLabelTotalNumDigits() const { return total_num_digits; }

     enum AXIS_RC          SetAxisParameters( const Limits& rNewLimits,
                            const FLOAT64 NewTicInterval, const UINT16 Precision );

     void                  ResetAxisParameters( ) { AutoSetAxisParameters( ); }

     void                  ShowGrid( BOOL8 State ) { show_grid = State; }
     BOOL8&                ShowGrid( ) { return show_grid; }

     void                  ShowUnits( BOOL8 State ) { show_units = State; }
     BOOL8&                ShowUnits( ) { return show_units; }

     void                  SetGridLength( INT16 NewLength ) { grid_length_device_coords = NewLength; }
};





class XAxis : public XyAxis
{     
protected :

     virtual void                DrawTicMark( UINT16 I ) const;
     virtual void                DrawTitle() const;

public:
                                 XAxis( OutputDevice* pParent, const Limits& DataLimits ) :
                                  XyAxis( pParent, DataLimits ) {} ;

                                 XAxis( OutputDevice* pParent ) :
                                  XyAxis( pParent ) {} ;

     virtual                     ~XAxis() {};

     virtual void                ResizeObject( );
};



class YAxis : public XyAxis
{     
protected :
     virtual void                DrawTicMark( UINT16 I ) const;
     virtual void                DrawTitle() const;

public:
                                 YAxis( OutputDevice* pParent, const Limits& DataLimits ) :
                                  XyAxis( pParent, DataLimits ) {} ;

                                 YAxis( OutputDevice* pParent ) :
                                  XyAxis( pParent ) {} ;

     virtual                     ~YAxis() {};

     virtual void                ResizeObject( );
};



const UINT16 MAXTICS  = 40;   // maximum allowable number of tic marks


#endif
 
 