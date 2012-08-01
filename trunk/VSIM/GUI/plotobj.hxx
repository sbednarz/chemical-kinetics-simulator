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
//  PLOTOBJ.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the PlotObject class for
//  the VSIM user interface. This is the base class from which
//  Axis. PlotData and other classes are derived. 
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED : 6.28.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__PLOTOBJ_HXX__)
#define __PLOTOBJ_HXX__

#include <sv.hxx>

#include "vsimtype.hxx"


enum PLOT_OBJECT_TYPE { XY_AXIS_OBJECT, XY_DATA_OBJECT, PLOT_TITLE_OBJECT,
                        XY_PLOT_OBJECT, MULT_XY_PLOT_OBJECT, 
                        STACKED_XY_PLOT_OBJECT, Z_AXIS_OBJECT, 
                        BAR_GRAPH_OBJECT, ANIMATED_BAR_GRAPH_OBJECT, 
                        BAR_ELEMENT_OBJECT, ANIMATED_BAR_ELEMENT_OBJECT,
                        LEGEND_ELEMENT_OBJECT, LEGEND_OBJECT };


class PlotObject : public LinkHdl 
{
protected:
     OutputDevice*       pparent;
     Rectangle           active_area;
     BOOL8               coordinates_invalid;
     BOOL8               highlight;

     void                HighlightArea();


public:
                         PlotObject( OutputDevice* pParent ) :
                         LinkHdl(),
                         pparent( pParent ),
                         active_area(),
                         coordinates_invalid( TRUE ),
                         highlight( FALSE )
                         {}

     virtual            ~PlotObject() {};

     OutputDevice*       GetpParent( ) const { return pparent; }

     virtual OutputDevice*  ChangepParent( OutputDevice* pNewParent )
                         {
                              OutputDevice* old  = pparent;
                              pparent = pNewParent;
                              return old;
                         }

     void                SetActiveArea( const Rectangle& rRect) { active_area = rRect; coordinates_invalid = TRUE; }
     const Rectangle&    GetActiveArea( ) const  { return active_area; }

     virtual void        DrawObject( ) = 0;
     virtual void        ResizeObject( ) = 0;
     virtual BOOL8       EditObject( const Point Position ) = 0;

     virtual PLOT_OBJECT_TYPE    Type() const = 0;

     void                 HighlightObject( BOOL8 State );
};

#endif
