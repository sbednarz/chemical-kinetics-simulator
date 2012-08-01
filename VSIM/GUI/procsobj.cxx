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
//  PROCSOBJ.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ProcessObject class for
//  the VSIM user interface. This is the base class from which 
//  the Compartment and TransferPath class are derived. 
//  It includes info about drawing the object on the screen 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "procsobj.hxx"
#include "schemwin.hxx"
#include "mainapp.hxx"

#endif


#include <stdlib.h>




// define colors and brush styles


const enum ColorName  PO_HIGHLIGHT_TEXT_COLOR = COL_LIGHTRED;

const enum ColorName  PO_SELECT_BG_COLOR      = COL_CYAN;
const enum ColorName  PO_SELECT_BG_FILL_COLOR = COL_WHITE;

const enum ColorName  PO_OPEN_BG_COLOR        = COL_BLACK;
const enum ColorName  PO_OPEN_BG_FILL_COLOR   = COL_WHITE;

const enum BrushStyle PO_OPEN_BG_BRUSH        = BRUSH_UPDIAG;
const enum BrushStyle PO_SELECT_BG_BRUSH      = BRUSH_25;






// ----------------------------------------------------------------------------
//                    ProcessObject::ProcessObject( )
// ............................................................................
//
//  constructor for ProcessObject
//
// ----------------------------------------------------------------------------

ProcessObject::ProcessObject( const String& rName, SchemeWindow* pParent ) :
title( rName ),
state( PO_NORMAL ),
highlight( FALSE ),
is_modified( FALSE ),
active_area( ),
pparent( pParent ),
poutput_device( pParent ),
std_font( pParent->GetFont() ),
highlight_font( std_font ),
std_fill( ),
select_fill( Color( PO_SELECT_BG_COLOR ), Color( PO_SELECT_BG_FILL_COLOR), PO_SELECT_BG_BRUSH  ),
open_fill( Color( PO_OPEN_BG_COLOR ), Color( PO_OPEN_BG_FILL_COLOR), PO_OPEN_BG_BRUSH  )
{
     highlight_font.ChangeColor( Color( PO_HIGHLIGHT_TEXT_COLOR ) );
     highlight_font.ChangeFillColor( Color( PO_NORMAL_BG_FILL_COLOR ) );

     char_width = GetpParent()->GetDefaultCharWidth();
     char_height = GetpParent()->GetDefaultCharHeight();
}





// ----------------------------------------------------------------------------
//                    ProcessObject::ProcessObject( )
// ............................................................................
//
//  copy constructor for ProcessObject
//
// ----------------------------------------------------------------------------

ProcessObject::ProcessObject( const ProcessObject& rObject ) :
title( rObject.title ),
state( rObject.state ),
highlight( rObject.highlight ),
is_modified( rObject.is_modified ),
active_area( rObject.active_area ),
pparent( rObject.pparent ),
poutput_device( rObject.poutput_device ),
std_font( rObject.std_font ),
highlight_font( rObject.highlight_font ),
std_fill(rObject.std_fill ),
select_fill( rObject.select_fill  ),
open_fill( rObject.open_fill  ),
char_width( rObject.char_width ),
char_height( rObject.char_height )
{
}



// ----------------------------------------------------------------------------
//                    ProcessObject::~ProcessObject( )
// ............................................................................
//
//  destructor for ProcessObject
//
// ----------------------------------------------------------------------------

ProcessObject::~ProcessObject(  )
{
}






// ----------------------------------------------------------------------------
//                    ProcessObject::operator =
// ............................................................................
//
//  assignment operator         
//
// ----------------------------------------------------------------------------

ProcessObject& ProcessObject::operator = ( const ProcessObject& rObject )
{
     // avoid assignment to self 

     if ( &rObject == this )
          return *this;

     title          = rObject.title;          
     state          = rObject.state;          
     highlight      = rObject.highlight;      
     is_modified    = rObject.is_modified;    
     active_area    = rObject.active_area;    
     pparent        = rObject.pparent;        
     poutput_device = rObject.poutput_device; 
     std_font       = rObject.std_font;       
     highlight_font = rObject.highlight_font; 
     std_fill       = rObject.std_fill;       
     select_fill    = rObject.select_fill;    
     open_fill      = rObject.open_fill;      
     char_width     = rObject.char_width;     
     char_height    = rObject.char_height;    

     return *this;
}


// ----------------------------------------------------------------------------
//                    ProcessObject::SetpParent()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void ProcessObject::SetpParent(  SchemeWindow* pParent ) 
{ 
     pparent = pParent; 

     char_width = GetpParent()->GetDefaultCharWidth();
     char_height = GetpParent()->GetDefaultCharHeight();
}



// ----------------------------------------------------------------------------
//                    ProcessObject::Highlight( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ProcessObject::Highlight( BOOL NewState )
{
     highlight = NewState;
     DrawTitle( );
}




// ----------------------------------------------------------------------------
//                    ProcessObject::DrawTitle( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ProcessObject::DrawTitle( )
{
     Font old_font;

     if ( highlight )
          old_font = GetpOutputDevice()->ChangeFont( highlight_font );
     else
          old_font = GetpOutputDevice()->ChangeFont( std_font );

     Size aTextSize( GetpOutputDevice()->GetTextSize( title ) );

     GetpOutputDevice()->DrawText( active_area.Center() -
      Point(aTextSize.Width( ) / 2, abs( aTextSize.Height( ) ) / 2 ), title); 

     GetpOutputDevice()->ChangeFont( old_font );
}






// ----------------------------------------------------------------------------
//                    ProcessObject::SelectBrush( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ProcessObject::SelectBrush()
{
     switch ( state )
     {
          case PO_NORMAL :
               GetpOutputDevice()->ChangeFillInBrush( std_fill );
               break;

          case PO_SELECT :
               GetpOutputDevice()->ChangeFillInBrush( select_fill );
               break;

          case PO_OPEN :
               GetpOutputDevice()->ChangeFillInBrush( open_fill );
               break;
     }
}




// ----------------------------------------------------------------------------
//                    ProcessObject::ChangeState( )
// ............................................................................
// 
//  resets colors and resets state - calls Draw to change screen appearance
//
// ----------------------------------------------------------------------------

enum ProcObjState ProcessObject::ChangeState( enum ProcObjState NewState )
{
     enum ProcObjState old_state = state;
 
     state = NewState;

     DrawActiveArea( );

     return old_state;
}



