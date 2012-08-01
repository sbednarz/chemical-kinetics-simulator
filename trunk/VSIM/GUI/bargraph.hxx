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
//  BARGRAPH.HXX - a header file for the VSIM User interface
// ............................................................................
//
//  This header file declares the BarGraph class for
//  the VSIM user interface. This class encapsultaes the definition of
//  a bar graphy with two axes
//  It includes info about drawing the object on the screen or other
//  output device
//
//  CREATED : 10.22.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__BARGRAPH_HXX__)
#define __BARGRAPH_HXX__




DECLARE_LIST( ElementList, BarGraphElement* )


class BarGraph : public PlotObject
{
protected :

     YAxis               y_axis;
     BarGraphAxis        x_axis;
     ElementList         element_list;


public :

                         BarGraph( OutputDevice* pParent );
     virtual            ~BarGraph( );

     BOOL8               AddElement( const FloatArray& rX, const FloatArray& rY );

     virtual void        DrawObject( );
     virtual void        ResizeObject( );
     virtual BOOL8       EditObject( const Point Position );

     virtual PLOT_OBJECT_TYPE    Type() const { return BAR_GRAPH_OBJECT; }


#endif