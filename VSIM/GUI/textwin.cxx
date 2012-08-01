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
//  TEXTWIN.CXX - a module  for the VSIM User interface 
// ............................................................................
// 
//  This module declares a TextWindow, specialized for presenting 
//  simulation results in text format
//
//  CREATED : 8.20.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "textwin.hxx"
#include "assert.hxx"

#endif


const enum ColorName  BACKGROUND_COLOR   = COL_WHITE;
const enum FontFamily TEXTWIN_FONTFAMILY = FAMILY_MODERN;
const INT16           FONT_HEIGHT_PTS    = 10;




// ----------------------------------------------------------------------------
//                         TextWindow::TextWindow()
// ............................................................................
//
//
//   constructor 
//
// ----------------------------------------------------------------------------

TextWindow::TextWindow( Window* pParent, LineList* pLineList ) :
DisplayWindow( pParent ), 
pline_list( pLineList ),
char_ht(0),
char_width(0),
increment(),
max_line_length( pLineList->GetMaxLineLength() ), 
total_num_lines( pLineList->GetNumLines() ), 
topmost_line_in_window(0), 
leftmost_char_in_window(0), 
bottom_line_in_window(0),
window_ht_lines(0), 
window_width_chars(0), 
page_ht_lines(0), 
page_width_chars(0)
{
     Color  color( BACKGROUND_COLOR );
     Brush  brush( color );

     ChangeBackgroundBrush( brush );

     // set font for window 

     Font font( TEXTWIN_FONTFAMILY, Size( 0, FONT_HEIGHT_PTS ) ); 

     // resolve logical to physical font - needed for correct spacing/sizing

     FontMetric metric = GetFontMetric( font );

     font =  metric.GetPhysicalFont( );

     font.ChangeFillColor( color );

     ChangeFont( font );
}




// ----------------------------------------------------------------------------
//                  TextWindow::ChangeScale( )
// ............................................................................
//
//  called when the user changes the scale of the TextWindow 
//
// ----------------------------------------------------------------------------

Fraction TextWindow::ChangeScale( const Fraction& rNewScale )
{
    Fraction old_scale = ScalableWindow::ChangeScale( rNewScale );

    Resize();
    Invalidate();
    Update();

    return old_scale;
}










// ----------------------------------------------------------------------------
//                  TextWindow::Scroll( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextWindow::Scroll( INT16 DeltaX, INT16 DeltaY )
{
    Rectangle rect( Point(), GetOutputSize() );
    BOOL8     do_scroll = FALSE;


    if ( DeltaY != 0 )
    {
         if ( DeltaY < 0 )
         {
              if ( -DeltaY < window_ht_lines )
              {
                   do_scroll = TRUE;
                   rect.Bottom()= rect.Bottom() + ( DeltaY * char_ht );
              }

              MoveUp( -DeltaY );
         }
         else
         {
              if ( DeltaY < window_ht_lines )
              {
                   do_scroll = TRUE;
                   rect.Top()= rect.Top() + ( DeltaY * char_ht );
              }

              MoveDown( DeltaY );
         }
    }
    else
    {
         if ( DeltaX != 0 )
         {
              if ( DeltaX < 0 )
              {
                   if ( -DeltaX < window_width_chars )
                   {
                        do_scroll = TRUE;
                        rect.Right()= rect.Right() + ( DeltaX * char_width );
                   }
     
                   MoveLeft( -DeltaX );
              }
              else
              {
                   if ( DeltaX < window_width_chars )
                   {
                        do_scroll = TRUE;
                        rect.Left()= rect.Left() + ( DeltaX * char_width );
                   }
     
                   MoveRight( DeltaX );
              }
         }
         else
         {
              // we come here if delta x and delta y = 0 

              return;
         }
    }



     // NOTE : the following line is included temporarily until 
     //        I get the scrolling working correctly : WDH  3 Sep 1996 

    do_scroll = FALSE;  // force invalidation


    if ( do_scroll )
    {
          Window::Scroll( -DeltaX * char_width, -DeltaY * char_ht, rect );
    }
    else
    {
         Invalidate();
    }

    Update();
}






// ----------------------------------------------------------------------------
//                         TextWindow::HorizontalDrag()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::HorizontalDrag( INT16 nDelta )
{
     INT16 delta = (INT16) ( (INT32) nDelta * (UINT32) max_line_length) / MAX_RANGE;

     Scroll( delta, 0 );
}




// ----------------------------------------------------------------------------
//                         TextWindow::VerticalDrag()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::VerticalDrag( INT16 nDelta )
{
     INT16 delta = (INT16) ( (INT32) nDelta * total_num_lines) / MAX_RANGE;

     Scroll( 0, delta );
}





// ----------------------------------------------------------------------------
//                         TextWindow::MoveDown()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::MoveDown( INT16 DeltaY )
{
     if ( bottom_line_in_window == (total_num_lines-1 ) )
          return;
     else
     {
          topmost_line_in_window += DeltaY;
     
          if ( topmost_line_in_window >= total_num_lines)
               topmost_line_in_window = total_num_lines-1;

          bottom_line_in_window = topmost_line_in_window + window_ht_lines;
     
          if (bottom_line_in_window >= total_num_lines)
               bottom_line_in_window = total_num_lines-1;
     
     }
}






// ----------------------------------------------------------------------------
//                         TextWindow::MoveUp()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::MoveUp( INT16 DeltaY )
{
     if ( topmost_line_in_window == 0 )
          return;
     else
     {
          if (topmost_line_in_window >= DeltaY )
               topmost_line_in_window -= DeltaY;
          else
               topmost_line_in_window = 0;
     
          bottom_line_in_window = topmost_line_in_window + window_ht_lines;
     
          if (bottom_line_in_window >= total_num_lines)
               bottom_line_in_window = total_num_lines-1;
     
     }
}






// ----------------------------------------------------------------------------
//                         TextWindow::MoveLeft()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::MoveLeft( INT16 DeltaX )
{
     if ( leftmost_char_in_window == 0 )
          return;
     else
     {
          if (leftmost_char_in_window >= DeltaX )
               leftmost_char_in_window -= DeltaX;
          else
               leftmost_char_in_window = 0;
     }
}





// ----------------------------------------------------------------------------
//                         TextWindow::MoveRight()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::MoveRight( INT16 DeltaX )
{
     if ( leftmost_char_in_window == (max_line_length + EXTRA_CHARS_LENGTH - window_width_chars ) )
          return;
     else
     {
          leftmost_char_in_window += DeltaX;
     
          if ( leftmost_char_in_window >= max_line_length)
               leftmost_char_in_window = max_line_length-1;
     }
}





// ----------------------------------------------------------------------------
//                         TextWindow::Paint()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::Paint( const Rectangle& )
{
     DrawObject( this );
}





// ----------------------------------------------------------------------------
//                         TextWindow::Resize()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------
void TextWindow::Resize()
{
     Size text_size = GetTextSize( String( "MMMMMMMMMM" ) );

     char_ht    = text_size.Height();
     char_width = text_size.Width()/10; 

     increment = Point( 0, char_ht );


     Size window_size = GetOutputSize();

     INT16 win_ht_log    = window_size.Height();
     INT16 win_width_log = window_size.Width();

     // we make each "page" equal to 1/2 a window

     page_ht_lines       = (win_ht_log/2) / char_ht;
     page_width_chars    = (win_width_log/2) / char_width; 

     // if == 0 then we are minimized - just return 

     if ( (page_ht_lines == 0 ) || (page_width_chars == 0 ) )
          return;

     window_ht_lines     = win_ht_log / char_ht; 
     window_width_chars  = win_width_log / char_width;

     bottom_line_in_window = topmost_line_in_window + window_ht_lines;

     if (bottom_line_in_window > total_num_lines)
          bottom_line_in_window = total_num_lines-1;

     (( TextFrameWindow*) GetParent() )->RecalcScrollbarSettings();
}







// ----------------------------------------------------------------------------
//                         TextWindow::Paint()
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::DrawObject( OutputDevice* pDevice )
{
     Point position( 0, 0 );

     for ( UINT32 i = topmost_line_in_window; i <= bottom_line_in_window; i++ )
     {
          pDevice->DrawText( position, 
                             *( pline_list->GetLineNum(i) ), 
                             leftmost_char_in_window, 
                             window_width_chars );

          position += increment;
     }
}


// ----------------------------------------------------------------------------
//                         TextWindow::PrintPlot( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void TextWindow::PrintPlot( Printer* pPrinter )
{
#if 0
        // set up for directing output to printer
        // preserve current state so that we can restore when done

        OutputDevice* old_parent = plot.ChangepParent( pPrinter );
        Rectangle     old_area   = plot.GetActiveArea( );

        Rectangle new_area = Rectangle( Point(), pPrinter->PixelToLogic( pPrinter->GetOutputSizePixel() ) );

        MapMode map_mode  = pPrinter->GetMapMode();

        // calc offset so that plot is centered after
        // we shrink to 80% of page size

        Point offset = pPrinter->PixelToLogic( pPrinter->GetPageOffsetPixel() );

        offset = offset + Point( new_area.GetWidth() /10, new_area.GetHeight() /10 );

        map_mode.ChangeOrigin( offset );

        const Fraction scaling_factor( 8, 10 );

        Fraction scale = map_mode.GetScaleX();
        scale *= scaling_factor;
        map_mode.ChangeScaleX( scale );

        scale = map_mode.GetScaleY();
        scale *= scaling_factor;
        map_mode.ChangeScaleY( scale );

        pPrinter->ChangeMapMode( map_mode );

        plot.SetActiveArea(  new_area );

        DrawObject();

        // restore previous state

        plot.ChangepParent( old_parent );
        plot.SetActiveArea( old_area );
#endif
}


