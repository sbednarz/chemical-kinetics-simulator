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
//  XFRSCHEM.HXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module declares the SchematicWindow class for
//  the VSIM user interface. 
//
//  CREATED : 8.07.98 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#include "vsimtype.hxx"


// ----------------------------------------------------------------------------
//                class SchematicWindow
// ............................................................................
// 
//   used to display is a graphical way the transfer step being edited
//
// ----------------------------------------------------------------------------

class SchematicWindow : public Window
{
protected :
     String     source;
     String     target;
     String     species;

     enum XFER_DIRECTION xfer_direction;

     Point      source_box_pos;
     Point      target_box_pos;
     Point      fwd_arrow_left_end;
     Point      fwd_arrow_right_end;
     Point      rev_arrow_left_end;
     Point      rev_arrow_right_end;

     Size       box_size;
     Size       window_size;

     INT16      char_width;
     INT16      char_height;
     Brush      arrow_fill;

public :

     SchematicWindow( Window* pParent, const ResId& rResId );

     virtual void Paint( const Rectangle& );

     void                  SetSourceName( const String& rName ) { source = rName; }
     void                  SetTargetName( const String& rName ) { target = rName; }
     void                  SetSpeciesName( const String& rName ) { species = rName; }
     void                  SetTransferDirection( const enum XFER_DIRECTION NewState ) { xfer_direction = NewState; }
     enum XFER_DIRECTION   GetTransferDirection( ) const { return xfer_direction; }
};



