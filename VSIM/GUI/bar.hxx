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
//  BAR.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the Bar class for
//  the VSIM user interface. This class encapsulates the definition of
//  a bar representing qunantitative data to be used in a bar graph 
//
//  CREATED :  2.2.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__BAR_HXX__)
#define __BAR_HXX__

#include <sv.hxx>
#include "vsimtype.hxx"


#define BAR_HORIZONTAL  ((WinBits)0x0001)
#define BAR_VERTICAL    ((WinBits)0x0002)

class Bar : public Control
{
protected:

     FLOAT64             max_bar_length_flt;
     FLOAT64             bar_length_flt;

     INT16               bar_width;
     INT16               max_bar_length;

     Rectangle           filled_bar;
     Rectangle           unfilled_bar;

     Brush               fill_brush;
     Brush               frame_brush;
     Brush               background_brush;

     Pen                 fill_pen;
     Pen                 frame_pen;
     Pen                 unfill_pen;
     BOOL8               vertical;

     INT16               spacer;

     void                CalcBarGeometries();
     virtual void        Resize();

public:
                         Bar( Window* pParent, WinBits WinStyle );

     void                SetBarColor( enum ColorName NewColor );
     virtual void        Paint( const Rectangle& );

     void                SetBarFillPattern( enum BrushStyle NewPattern ) { fill_brush.ChangeStyle( NewPattern ); }
     void                SetBackgroundBrush( const Brush& rBrush ) {background_brush = rBrush; }

	void                SetMaxBarValue( FLOAT64 MaxLength );
	void                SetCurrentBarValue( FLOAT64 Length );
};


#endif
