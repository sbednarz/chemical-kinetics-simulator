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
//  LEGEND.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module defines the Legendtclass for displaying legends on plots 
//  created in VSIM
//
//  CREATED : 1.19.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "legend.hxx"
#include "colors.hxx"
#include "linestyl.hxx"

#endif






// ----------------------------------------------------------------------------
//                           Legend::Legend( )
// ............................................................................
// 
//   contructor 
//
// ----------------------------------------------------------------------------

Legend::Legend( OutputDevice* pParent ) :
PlotObject( pParent ),
element_list(),
color_scheme( DEFAULT_COLOR_SCHEME ),
vary_line_style( FALSE ),
vary_color( TRUE ),
show_markers( FALSE ),
show_line( TRUE ),
vary_marker_style( TRUE )
{
}





// ----------------------------------------------------------------------------
//                           Legend::Legend( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

Legend::~Legend() 
{
     // delete any LegendElement object we have created

     if ( element_list.First() )
     {
          delete element_list.GetCurObject();

          while ( element_list.Next() )
               delete element_list.GetCurObject();
     }
}





// ----------------------------------------------------------------------------
//                           Legend::ChangepParent()
// ............................................................................
// 
//   
//
// ----------------------------------------------------------------------------

OutputDevice*  Legend::ChangepParent( OutputDevice* pNewParent )
{

     if ( element_list.First() )
     {
          element_list.GetCurObject()->ChangepParent( pNewParent );

          while ( element_list.Next() )
               element_list.GetCurObject()->ChangepParent( pNewParent );


     }

     return PlotObject::ChangepParent( pNewParent );

}





// ----------------------------------------------------------------------------
//                           Legend::DrawObject()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::DrawObject( )
{
     // draw elements if any exist

     if ( element_list.First() )
     {
          // change font temporarily to be transparent
     
          Font font = GetpParent()->GetFont();
          font.ChangeTransparent( TRUE );
     
          Font old_font = GetpParent()->ChangeFont( font );
     

          element_list.GetCurObject()->DrawObject();

          while ( element_list.Next() )
               element_list.GetCurObject()->DrawObject();

          if ( highlight )
               HighlightArea();

          GetpParent()->ChangeFont( old_font );

     }
}





// ----------------------------------------------------------------------------
//                           Legend::ResizeObject()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::ResizeObject( )
{
     if ( element_list.First() )
     {
          const UINT16 ELEMENT_HT = 2; // in multiples of char ht

          const Point OFFSET( 0, GetpParent()->GetFont().GetSize().Height() * ELEMENT_HT  );

          Rectangle rect = Rectangle( active_area.TopLeft(),
                                      active_area.TopRight() +  OFFSET );


          element_list.GetCurObject()->SetActiveArea( rect );
          element_list.GetCurObject()->ResizeObject();
 
          while ( element_list.Next() )
          {
               rect.ChangePos( rect.TopLeft() + OFFSET );
               element_list.GetCurObject()->SetActiveArea( rect );
               element_list.GetCurObject()->ResizeObject();
          }
     }
 

}




// ----------------------------------------------------------------------------
//                           Legend::EditObject()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8  Legend::EditObject( const Point Position )
{
     return FALSE;
}






// ----------------------------------------------------------------------------
//                           Legend::AddElement()
// ............................................................................
// 
//   function to add another element to the current legend. returns TRUE if
//   addition is successful, FALSE is failure. In this function we create a
//   new LegendElement object to contain the legend data. 
//
// ----------------------------------------------------------------------------

BOOL8  Legend::AddElement( const String& rText )
{
     LegendElement* ptr = new LegendElement( GetpParent(), rText );

     if ( ptr )
     {
          UINT16 element_num = element_list.Count();

          ptr->ShowPoints( show_markers );
          ptr->ShowLines( show_line );

          ptr->SetMarkerStyle( vary_marker_style ? GetMarkerStyle( element_num ) :
           DEFAULT_MARKER_STYLE );

          ptr->SetColor ( vary_color ?
                          ColorScheme[ color_scheme ].GetDataColor( element_num ) :
                          ColorScheme[ color_scheme ].GetDefaultDataColor( )
          );

          ptr->SetLineStyle ( vary_line_style ? GetLineStyle( element_num ) :
           DEFAULT_LINE_STYLE );

          // now add new object to list at end 

          element_list.Insert( ptr, LIST_APPEND );

          return TRUE;
     }
     else
          return FALSE;
}









// ----------------------------------------------------------------------------
//                           Legend::VaryLineStyle()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::VaryLineStyle( const BOOL8 State )
{
{
     vary_line_style = State; 

     UINT16 num_elements = element_list.Count();

     for ( UINT16 i = 0; i < num_elements; i++ ) 
     {
          element_list.GetObject( i )->SetLineStyle ( 
           vary_line_style ? GetLineStyle( i ) : DEFAULT_LINE_STYLE );
     }
}
}




// ----------------------------------------------------------------------------
//                           Legend::VaryColor()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::VaryColor( const BOOL8 State )
{
     vary_color = State; 

     UINT16 num_elements = element_list.Count();

     for ( UINT16 i = 0; i < num_elements; i++ )
     {
          element_list.GetObject( i )->SetColor ( vary_color ?
                ColorScheme[ color_scheme ].GetDataColor( i ) :
                ColorScheme[ color_scheme ].GetDefaultDataColor( )
          );

     }
}




// ----------------------------------------------------------------------------
//                           Legend::ShowMarkers()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::ShowMarkers( const BOOL8 State )
{
     show_markers = State;

     UINT16 num_elements = element_list.Count();

     for ( UINT16 i = 0; i < num_elements; i++ ) 
          element_list.GetObject( i )->ShowPoints( show_markers );
}




// ----------------------------------------------------------------------------
//                           Legend::ShowLines()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::ShowLines( const BOOL8 State )
{
     show_line = State;

     UINT16 num_elements = element_list.Count();

     for ( UINT16 i = 0; i < num_elements; i++ )
          element_list.GetObject( i )->ShowLines ( show_line );
}



// ----------------------------------------------------------------------------
//                           Legend::VaryMarkerStyle()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void Legend::VaryMarkerStyle( const BOOL8 State )
{
     vary_marker_style = State; 

     UINT16 num_elements = element_list.Count();

     for ( UINT16 i = 0; i < num_elements; i++ )
     {
          element_list.GetObject( i )->SetMarkerStyle ( 
           vary_marker_style ? GetMarkerStyle( i ) :
           DEFAULT_MARKER_STYLE );
     }
}

