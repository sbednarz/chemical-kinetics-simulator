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
//  ARROW.HXX - a header file  of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Arrow class for drawing arrow figures in 
//  the VSIM user interface. 
//
//  CREATED : 10.18.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__ARROW_HXX__)
#define __ARROW_HXX__

#include <sv.hxx>

#include "vsimtype.hxx"




class Arrow
{
protected:
  
     OutputDevice*         poutput_device;
     Point                 source_point;
     Point                 target_point;
     Brush                 arrow_fill;
     INT16                 arrow_length;
     INT16                 arrow_halfwidth;
     BOOL8                 show_arrowhead;
     Polygon               polygon;


     Point                 MovePoint( const Rectangle& rRect, const Point& rPoint );
     void                  DrawArrowHead( );
     void                  CalcPolygon();
  
     OutputDevice*         GetpOutputDevice() const { return poutput_device; }
  
public:
                           Arrow( OutputDevice* pOutputDevice );

     Arrow&                operator = ( const Arrow& rObject );
  
     void                  DrawObject();
     void                  Reposition( const Rectangle& rSourceRect, const Rectangle& rTargetRect );

     Point                 SourcePoint( ) const { return source_point; };
     Point                 TargetPoint( ) const { return target_point; };

     void                  SizeArrow( INT16 rSize );
  
     void                  SetpOutputDevice( OutputDevice* pNewDevice );

     BOOL8                 ShowArrowHead() const { return show_arrowhead; }
     void                  ShowArrowHead( BOOL8 NewState ) { show_arrowhead = NewState; }
};  

#endif
