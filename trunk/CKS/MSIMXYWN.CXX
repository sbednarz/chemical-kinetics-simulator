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

/* []----------------------------------------------------------------------[]
   |  msimxywin.cxx    :  defines the class msimXYPlot for MSIM             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   Aug 22 1993                                        |
   |                                                                        |
   |  this module contains the definitiona of the object class XYWindow     |
   |  which is derived from msimXYPlot. This class provides for the creation|
   |  of x-y plots in a screen window                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop
#include "msimplot.hxx"
#include <stdlib.h>

#define MARKER_WIDTH   6
#define MARKER_HT      6


/* -- constructor for XYWindow: ----------------------------------          */

XYWindow::XYWindow( msimWID pParent, PPLOT_SPECS PlotSpecs ) : XYPlot ( pParent,
     PlotSpecs ),
Window ( pParent, WB_BORDER )
{                                      /* initialize font from plotspecs
                                          data                                */

#if defined(OLD_MAPMODE)
     ChangeMapMode( MapMode( MAP_POINT, PixelToLogic( Point( 0, -10
                    ), MapMode( MAP_POINT ) ), Fraction( 1, 1 ), Fraction( -1, 1 ) ) );
#else
     ChangeMapMode( MapMode( MAP_POINT ) );
#endif

     ChangeColorSet( pplot_specs->current_colorset );

     ChangeFillInBrush( Brush( BRUSH_NULL ) );// suppresses fillin of rectangle
     ChangeBackgroundBrush( Brush( pplot_specs->background_color, BRUSH_SOLID ) );

     Show( );
}

void XYWindow::InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize )
{
     // spec the new desired font

     Font afont = Font( TypeFace, Size( 0, PointSize ) );

     afont.ChangeTransparent( TRUE );
     afont.ChangeColor( Color( pplot_specs->axis_color ) );
     afont.ChangeAlign( ALIGN_BOTTOM );

     // now get actual available font and store value

     FontMetric ametric = GetFontMetric( afont );

     StdFont = ametric.GetPhysicalFont( );


     // get avg char ht and width - we get a better fit under OS/2 using the
     // following
#if defined(__OS2__)
     Font aOldFont = ChangeFont( StdFont );

     Size char_size = GetTextSize( String( "1.2345mmmm" ) );
     char_ht = abs( char_size.Height( ) );
     char_width = abs( char_size.Width( ) / 10 );
#endif


#if defined(__MSDOS__)  || defined(__MAC__) || defined(__AIX__)
   char_ht = abs(StdFont.GetSize().Height());
   char_width = abs(StdFont.GetSize().Width());
#endif

     /* now do the same for rotated font                                     */

     afont.ChangeCharOrientation( 900 );
     afont.ChangeLineOrientation( 900 );

     ametric = GetFontMetric( afont );
     RotFont = ametric.GetPhysicalFont( );

#if defined(__OS2__)
     ChangeFont( RotFont );

     char_size = GetTextSize( String( "1.2345mmmm" ) );
     rot_char_ht = abs( char_size.Height( ) );
     rot_char_width = abs( char_size.Width( ) / 10 );
     true_rotated_font = FALSE;        // temporary - rot fonts not working yet
#endif


#if defined(__MSDOS__)
   true_rotated_font = ((ametric.GetType() == TYPE_VECTOR) || (ametric.GetType() == TYPE_SCALABLE));
   rot_char_ht = abs(RotFont.GetSize().Height());
   rot_char_width = abs(RotFont.GetSize().Width());
#endif

#if defined(__MAC__) || defined(__AIX__)
   true_rotated_font = FALSE;
   rot_char_ht = abs(RotFont.GetSize().Height());
   rot_char_width = abs(RotFont.GetSize().Width());
#endif



}

// --- PaintWindow::Paint() ----------------------------------------

void XYWindow::Paint( const Rectangle& )
{
     DrawSelectedPlots( );
}

// --- PaintWindow::Resize() ---------------------------------------

void XYWindow::Resize( )
{
     window_width = pplot_specs->horizontal_win_size;
     window_height = pplot_specs->vertical_win_size;

     Invalidate( );
}


#ifdef USE_BITMAP
// the following implements the drawing of point markers by first
// drawing the marker on a virtual device, then copying the bitmap
// formed into the plot window
// overlay of points seems to make the drawing messy looking
// since the background is not transparent

void    XYWindow::DrawMarker(Point& rPoint)
{
     DrawBitmap( rPoint + Point(-MARKER_WIDTH/2,MARKER_HT/2), marker_bitmap);
}


void   XYWindow::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor)
{
    VirtualDevice      vd;

    vd.ChangeBackgroundBrush (Brush( pplot_specs->background_color, BRUSH_SOLID));
    vd.ChangePen(Pen(Color(ThisColor)));

    vd.SetOutputSizePixel( Size( MARKER_WIDTH, MARKER_HT));

     // draw the figure here

     switch (MarkerStyle)
     {
     case  MARKER_CROSS :

          vd.DrawLine( Point( 0,0), Point( MARKER_WIDTH-1, MARKER_HT-1) );
          vd.DrawLine( Point( 0, MARKER_HT-1), Point( MARKER_WIDTH-1, 0) );

          break;

     case  MARKER_CIRCLE :

          vd.ChangeFillInBrush( Brush( BRUSH_NULL) );  // suppresses fillin of rectangle
          vd.DrawEllipse( Rectangle( Point( 0,0), Point( MARKER_WIDTH-1, MARKER_HT-1)) );

          break;

     case  MARKER_STAR6 :

          vd.DrawLine( Point( MARKER_WIDTH/2-1, 0), Point( MARKER_WIDTH/2-1, MARKER_HT-1) );
          vd.DrawLine( Point( 0, 1), Point( MARKER_WIDTH, MARKER_HT-2) );
          vd.DrawLine( Point( MARKER_WIDTH, 1), Point( 0, MARKER_HT-2) );

          break;

     case  MARKER_STAR8 :

          vd.DrawLine( Point( 0,0), Point( MARKER_WIDTH-1, MARKER_HT-1) );
          vd.DrawLine( Point( 0, MARKER_HT-1), Point( MARKER_WIDTH-1, 0) );
          vd.DrawLine( Point( MARKER_WIDTH/2-1, 0), Point( MARKER_WIDTH/2-1, MARKER_HT-1) );
          vd.DrawLine( Point( 0, MARKER_HT/2-1), Point( MARKER_WIDTH-1, MARKER_HT/2-1) );

          break ;

     case  MARKER_SQUARE :

          vd.ChangeFillInBrush( Brush( BRUSH_NULL) );  // suppresses fillin of rectangle
          vd.DrawRect( Rectangle( Point( 0,0), Size(MARKER_WIDTH, MARKER_HT ) ));

          break ;

     case  MARKER_DIAMOND :
          {
               Point pt_array[4];

               pt_array[0] =  Point(MARKER_WIDTH/2-1, 0);
               pt_array[1] =  Point(MARKER_WIDTH-1, MARKER_HT/2-1);
               pt_array[2] =  Point(MARKER_WIDTH/2-1, MARKER_HT-1);
               pt_array[3] =  Point(0, MARKER_HT/2-1);

               vd.ChangeFillInBrush( Brush( BRUSH_NULL) );  // suppresses fillin of rectangle
               vd.DrawPolygon( Polygon( 4, pt_array));

               break;
          }

     case  MARKER_SOLIDDIAMOND :
           {
               Point pt_array[4];
                pt_array[0] =  Point(MARKER_WIDTH/2-1, 0);
                pt_array[1] =  Point(MARKER_WIDTH-1, MARKER_HT/2-1);
                pt_array[2] =  Point(MARKER_WIDTH/2-1, MARKER_HT-1);
                pt_array[3] =  Point(0, MARKER_HT/2-1);

               vd.ChangeFillInBrush( Brush( ThisColor, BRUSH_SOLID) );
               vd.DrawPolygon( Polygon( 4, pt_array));

               break;
          }

     case  MARKER_SOLIDSQUARE :

          vd.ChangeFillInBrush( Brush( ThisColor, BRUSH_SOLID) );
          vd.DrawRect( Rectangle( Point( 0,0), Size(MARKER_WIDTH, MARKER_HT ) ));

          break ;

     case  MARKER_PLUS :

          vd.DrawLine( Point( MARKER_WIDTH/2-1, 0), Point( MARKER_WIDTH/2-1, MARKER_HT-1) );
          vd.DrawLine( Point( 0, MARKER_HT/2-1), Point( MARKER_WIDTH-1, MARKER_HT/2-1) );

          break;

     }                                 /* endswitch                           */

    marker_bitmap = vd.GetBitmap(  Point( 0,0), Size( MARKER_WIDTH, MARKER_HT ));

    return;
}


void    XYWindow::DrawPolyMarker(const Polygon& rPoly)
{
     USHORT             i = 0;
     USHORT             NumPoints = rPoly.GetSize();
     RasterOp oldro = ChangeRasterOp( ROP_OR) ;

     while (i < NumPoints)
         DrawBitmap( rPoly.GetPoint(i++) + Point(-MARKER_WIDTH/2,MARKER_HT/2), marker_bitmap);

   ChangeRasterOp( oldro);
}


#else
// This is a more straihgtforward implementation of the marker drawing routines.
// Here we explicitly call the drawing operations to draw each marker.
// Slower than the bitmap approach above but seems to look better
// since it does not lead to blanked-out areas in the vicinity of the marker.

void XYWindow::DrawMarker( const Point& rPoint )
{
     Brush aOldBrush;

     if ( current_marker_type == MARKER_SOLIDDIAMOND || current_marker_type == MARKER_SOLIDSQUARE )
     {
          aOldBrush = GetFillInBrush( );
          ChangeFillInBrush( Brush( GetPen( ) .GetColor( ), BRUSH_SOLID ) );
          DrawTheMarker( rPoint );
          ChangeFillInBrush( aOldBrush );
     }
     else
          DrawTheMarker( rPoint );
}

void XYWindow::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor )
{
     current_marker_type = MarkerStyle;
     ChangePen( Pen( Color( ThisColor ) ) );

}


void XYWindow::DrawTheMarker( const Point& rPoint )
{
     // draw the figure here
     switch ( current_marker_type )
     {
     case MARKER_CROSS :
          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), rPoint + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) );
          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, MARKER_HT / 2 ), rPoint + Point( MARKER_WIDTH / 2, - MARKER_HT / 2 ) );

          break;

     case MARKER_CIRCLE :

          DrawEllipse( Rectangle( rPoint + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), rPoint + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) ) );

          break;

     case MARKER_STAR6 :

#if ( MARKER_HT == 8 )
          //vertical
          DrawLine( rPoint - Point( 0, MARKER_HT / 2 - 1 ),
               rPoint + Point( 0, MARKER_HT / 2 - 1 )
          );

          // ll to ur
          DrawLine( rPoint - Point( MARKER_WIDTH / 2 - 2, MARKER_HT / 2 - 2 ),
               rPoint + Point( MARKER_WIDTH / 2 - 2, MARKER_HT / 2 - 2 )
          );

          // lr to ul
          DrawLine( rPoint - Point( -( MARKER_WIDTH / 2 - 2 ), MARKER_HT / 2 - 2 ),
               rPoint + Point( -( MARKER_WIDTH / 2 - 2 ), MARKER_HT / 2 - 2 )
          );

#endif
#if ( MARKER_HT == 6 )
          //vertical
          DrawLine( rPoint - Point( 0, MARKER_HT / 2 ),
               rPoint + Point( 0, MARKER_HT / 2 )
          );

          // ll to ur
          DrawLine( rPoint - Point( MARKER_WIDTH / 2, MARKER_HT / 2 ),
               rPoint + Point( MARKER_WIDTH / 2, MARKER_HT / 2 )
          );

          // lr to ul
          DrawLine( rPoint - Point( -( MARKER_WIDTH / 2 ), MARKER_HT / 2 ),
               rPoint + Point( -( MARKER_WIDTH / 2 ), MARKER_HT / 2 )
          );

#endif

          break;

     case MARKER_STAR8 :

          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), rPoint + Point( MARKER_WIDTH / 2, MARKER_HT / 2 ) );
          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, MARKER_HT / 2 ), rPoint + Point( MARKER_WIDTH / 2, - MARKER_HT / 2 ) );

          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, 0 ), rPoint + Point( MARKER_WIDTH / 2, 0 ) );
          DrawLine( rPoint + Point( 0, - MARKER_HT / 2 ), rPoint + Point( 0, MARKER_HT / 2 ) );

          break;

     case MARKER_SQUARE :

          DrawRect( Rectangle( rPoint + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Size( MARKER_WIDTH, MARKER_HT ) ) );

          break;

     case MARKER_DIAMOND :
     {
          Point pt_array[4];

          pt_array[0] = rPoint + Point( - MARKER_WIDTH / 2, 0 );
          pt_array[1] = rPoint + Point( 0, - MARKER_HT / 2 );
          pt_array[2] = rPoint + Point( MARKER_WIDTH / 2, 0 );
          pt_array[3] = rPoint + Point( 0, MARKER_HT / 2 );

          DrawPolygon( Polygon( 4, pt_array ) );

          break;
     }

     case MARKER_SOLIDDIAMOND :
     {
          Point pt_array[4];

          pt_array[0] = rPoint + Point( - MARKER_WIDTH / 2, 0 );
          pt_array[1] = rPoint + Point( 0, - MARKER_HT / 2 );
          pt_array[2] = rPoint + Point( MARKER_WIDTH / 2, 0 );
          pt_array[3] = rPoint + Point( 0, MARKER_HT / 2 );


          DrawPolygon( Polygon( 4, pt_array ) );

          break;
     }

     case MARKER_SOLIDSQUARE :

          DrawRect( Rectangle( rPoint + Point( - MARKER_WIDTH / 2, - MARKER_HT / 2 ), Size( MARKER_WIDTH, MARKER_HT ) ) );

          break;

     case MARKER_PLUS :
     default :
          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, 0 ), rPoint + Point( MARKER_WIDTH / 2, 0 ) );
          DrawLine( rPoint + Point( 0, - MARKER_HT / 2 ), rPoint + Point( 0, MARKER_HT / 2 ) );

          break;
     }                                 /* endswitch                           */

     return;
}


void XYWindow::DrawPolyMarker( const Polygon& rPoly )
{
     USHORT i = 0;
     USHORT NumPoints = rPoly.GetSize ( );
     Brush aOldBrush;
//     RasterOp oldro = ChangeRasterOp( ROP_XOR) ;

     if ( current_marker_type == MARKER_SOLIDDIAMOND || current_marker_type == MARKER_SOLIDSQUARE )
     {
          aOldBrush = GetFillInBrush( );
          ChangeFillInBrush( Brush( GetPen( ) .GetColor( ), BRUSH_SOLID ) );
          while ( i < NumPoints )
               DrawTheMarker( rPoly.GetPoint( i++ ) );

          ChangeFillInBrush( aOldBrush );

     }
     else
          while ( i < NumPoints )
               DrawTheMarker( rPoly.GetPoint( i++ ) );

//   ChangeRasterOp( oldro);
}
#endif

inline void XYWindow::xShow( )
{
     Window::Show( );
};
inline Pen XYWindow::xChangePen( const Pen& rNewPen )
{
     return Window::ChangePen( rNewPen );
};

inline Font XYWindow::xChangeFont( const Font& rNewFont )
{
     return Window::ChangeFont( rNewFont );
};
inline void XYWindow::xDrawLine( const Point& rStartPt, const Point&
                 rEndPt )
{
     Window::DrawLine( rStartPt, rEndPt );
};
inline void XYWindow::xDrawPolyLine( const Polygon& rPoly )
{
     DrawPolyLine( rPoly );
};

inline void XYWindow::xDrawText( const Point& rStartPt, const String&
                 rStr, USHORT nPos, USHORT nLen )
{
     Window::DrawText( rStartPt, rStr, nPos, nLen );
};


// under Win 3.1 screen rectangle is off by one pixel in horizontal lines
// - this somehow fixes it !!

#if defined(__MSDOS__)
inline     void   XYWindow::xDrawRect( const Rectangle& rRect)
                    { Window::DrawRect( Rectangle( rRect.BottomLeft() + Point(0, 0),
                                  rRect.TopRight() + Point(0, 0) )); };
#else
inline void XYWindow::xDrawRect( const Rectangle& rRect )
{
     Window::DrawRect( rRect );
};
#endif


