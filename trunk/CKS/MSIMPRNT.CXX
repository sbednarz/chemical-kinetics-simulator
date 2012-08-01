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
   |  msimxy.cxx       :  defines the class msimXYPlot for MSIM             |
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

#define MARKER_WIDTH   8
#define MARKER_HT      8

// -- constructor for XYPrint ------------------

XYPrint::XYPrint( msimWID pParent, PPLOT_SPECS PlotSpecs ) : XYPlot ( pParent,
     PlotSpecs ),
Printer ( )
{
     pplot_specs = PlotSpecs;
}


void XYPrint::InitializeGeometricData( )
{
     ChangeMapMode( MapMode( MAP_POINT ) );

     Size output_size = GetOutputSize( );// in points == 1/72 inch

     SHORT horizontal_paper_size = output_size.Width ( );
     SHORT vertical_paper_size = output_size.Height ( );

#if defined(USE_MAPMODE)
     // scale to 90 % of paper size
     ChangeMapMode( MapMode( MAP_POINT, Point( 0, - vertical_paper_size ),
               Fraction( 9, 10 ), Fraction( -9, 10 ) ) );
#else
     // scale to 90 % of paper size
     ChangeMapMode( MapMode( MAP_POINT,
               Point( horizontal_paper_size / 20, vertical_paper_size / 20 ),
               Fraction( 9, 10 ), Fraction( 9, 10 ) ) );
#endif

     pplot_specs->horizontal_win_size = horizontal_paper_size;
     pplot_specs->vertical_win_size = vertical_paper_size;
}

void XYPrint::InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize )
{
     // spec the new desired font
     Font afont = Font( TypeFace, Size( 0, PointSize ) );

     afont.ChangeTransparent( TRUE );
     afont.ChangeColor( Color( pplot_specs->axis_color ) );
     afont.ChangeAlign( ALIGN_BOTTOM );

     /* now get font metrics and store values in private vars                    */

     FontMetric aMetric = GetFontMetric( afont );

     StdFont = aMetric.GetPhysicalFont( );

#if defined(__OS2__)
     Font aOldFont = ChangeFont( StdFont );

     Size char_size = GetTextSize( String( "XXXXXXXXXX" ) );
     char_ht = abs( char_size.Height( ) );
     char_width = abs( char_size.Width( ) / 10 );
#endif

#if defined(__MSDOS__) || defined(__MAC__) || defined(__AIX__)
   char_ht = abs(StdFont.GetSize().Height());
   char_width = abs(StdFont.GetSize().Width());
#endif

     /* now do the same for rotated font                                     */

     afont.ChangeCharOrientation( 900 );
     afont.ChangeLineOrientation( 900 );

     aMetric = GetFontMetric( afont );
     RotFont = aMetric.GetPhysicalFont( );

#if defined(__OS2__)
     ChangeFont( RotFont );

     char_size = GetTextSize( String( "XXXXXXXXXX" ) );
     rot_char_ht = abs( char_size.Height( ) );
     rot_char_width = abs( char_size.Width( ) / 10 );
     true_rotated_font = FALSE;        // temporary - rot fonts not working yet
#endif


#if defined(__MSDOS__)
   true_rotated_font = ((aMetric.GetType() == TYPE_VECTOR) || (aMetric.GetType() == TYPE_SCALABLE));
   rot_char_ht = abs(RotFont.GetSize().Height());
   rot_char_width = abs(RotFont.GetSize().Width());
#endif

#if defined(__MAC__) || defined(__AIX__)
   true_rotated_font = FALSE;
   rot_char_ht = abs(RotFont.GetSize().Height());
   rot_char_width = abs(RotFont.GetSize().Width());
#endif

}



void XYPrint::Paint( const Rectangle& )
{
     DrawSelectedPlots( );
}


void XYPrint::Resize( )
{
}


#ifdef USE_BITMAP

void    XYPrint::DrawMarker(Point& rPoint)
{
     DrawBitmap( rPoint + Point(-MARKER_WIDTH/2,MARKER_HT/2), marker_bitmap);
}


void   XYPrint::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor)
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


void    XYPrint::DrawPolyMarker(const Polygon& rPoly)
{
     USHORT             i = 0;
     USHORT             NumPoints = rPoly.GetSize();
     RasterOp oldro = ChangeRasterOp( ROP_OR) ;

     while (i < NumPoints)
         DrawBitmap( rPoly.GetPoint(i++) + Point(-MARKER_WIDTH/2,MARKER_HT/2), marker_bitmap);

   ChangeRasterOp( oldro);
}
#else

void XYPrint::DrawMarker( const Point& rPoint )
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

void XYPrint::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor )
{
     current_marker_type = MarkerStyle;
     ChangePen( Pen( Color( ThisColor ) ) );

}


void XYPrint::DrawTheMarker( const Point& rPoint )
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

          DrawLine( rPoint + Point( 0, -( MARKER_HT / 2 ) ), rPoint + Point( 0, MARKER_HT / 2 ) );

          DrawLine( rPoint + Point( MARKER_WIDTH / 2 - 1, MARKER_HT / 2 - 1 ), rPoint + Point( -( MARKER_WIDTH / 2 - 1 ), - ( MARKER_HT - 3 ) ) );
          DrawLine( rPoint + Point( -( MARKER_WIDTH / 2 - 1 ), MARKER_HT / 2 - 1 ), rPoint + Point( MARKER_WIDTH / 2 - 1, - ( MARKER_HT - 3 ) ) );

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

          DrawLine( rPoint + Point( - MARKER_WIDTH / 2, 0 ), rPoint + Point( MARKER_WIDTH / 2, 0 ) );
          DrawLine( rPoint + Point( 0, - MARKER_HT / 2 ), rPoint + Point( 0, MARKER_HT / 2 ) );

          break;

     default :
          break;

     }                                 /* endswitch                           */

     return;
}


void XYPrint::DrawPolyMarker( const Polygon& rPoly )
{
     USHORT i = 0;
     USHORT NumPoints = rPoly.GetSize ( );
     Brush aOldBrush;

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

}
#endif

inline void XYPrint::xShow( )
{
};
inline Pen XYPrint::xChangePen( const Pen& rNewPen )
{
     return Printer::ChangePen( rNewPen );
};
inline Font XYPrint::xChangeFont( const Font& rNewFont )
{
     return Printer::ChangeFont( rNewFont );
};
inline void XYPrint::xDrawLine( const Point& rStartPt, const Point&
                 rEndPt )
{
     Printer::DrawLine( rStartPt, rEndPt );
};
inline void XYPrint::xDrawPolyLine( const Polygon& rPoly )
{
     Printer::DrawPolyLine( rPoly );
};

inline void XYPrint::xDrawText( const Point& rStartPt, const String&
                 rStr, USHORT nPos, USHORT nLen )
{
     Printer::DrawText( rStartPt, rStr, nPos, nLen );
};


inline void XYPrint::xDrawRect( const Rectangle& rRect )
{
     Printer::DrawRect( rRect );
};


 