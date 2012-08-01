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
//  ABARGRAF.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the AnimatedBarGraph class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar graph with two axes. It includes info about drawing the object 
//  on the screen. 
//
//  CREATED : 01.31.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__ABARGRAF_HXX__)
#define __ABARGRAF_HXX__

#include "abardisp.hxx"
#include "xyaxis.hxx"
#include "plottitl.hxx"
#include "legend.hxx"
#include "colors.hxx"


DECLARE_LIST( BarElementList, AnimatedBarDisplay* )


class AnimatedBarGraph : public Window
{
protected :

     YAxis               y_axis;
     AnimatedBarDisplay* p_det_var_display;
     PlotTitle           title;
     Legend              legend;
     BarElementList      element_list;
     AutoTimer           timer;
     BOOL8               vary_fill_style;
     BOOL8               vary_color;
     enum  COLOR_SCHEME_TYPE color_scheme;

     BOOL8               ShowNextValue();

     virtual void        Resize();

public :

                         AnimatedBarGraph( Window* pParent );
     virtual            ~AnimatedBarGraph( );

     BOOL8               AddElement( const FloatArray& rArray, const String& rName );
     BOOL8               SetDeterminateVariableBar( const FloatArray& rArray, const String& rName );

     virtual void        Paint( const Rectangle& );
   
     void                SetStepTime( ULONG MilliSecs ) { timer.ChangeTimeout( MilliSecs ); }

     void                Play();
     void                Stop() { timer.Stop(); } 
     void                Rewind();

     void                TimeOutHandler( Timer* );


};


#endif