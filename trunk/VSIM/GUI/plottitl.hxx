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
//  PLOTTITL.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This module declares the PlotTitle class for use in displaying simulation
//  results in VSIM
//
//  CREATED : 7.14.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__PLOTTITL_HXX__)
#define __PLOTTITL_HXX__


#include "plotobj.hxx"

class PlotTitle : public PlotObject
{
protected:
     String          title;
     Point           location;
public:

                     PlotTitle( OutputDevice* pParent );

                    ~PlotTitle() {}


     virtual void        DrawObject( );
     virtual void        ResizeObject( );
     virtual BOOL8       EditObject( const Point Positiont );

     virtual PLOT_OBJECT_TYPE    Type() const { return PLOT_TITLE_OBJECT; }

     void            SetTitle( const String& rNewTitle ) { title = rNewTitle; }
     const String&   GetTitle( ) const { return title; }

};

#endif
 
 