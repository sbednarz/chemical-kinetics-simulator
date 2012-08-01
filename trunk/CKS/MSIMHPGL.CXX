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
   |  msimhpgl.cxx       :  defines the class HPGLPlot for MSIM             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   Aug 22 1993                                        |
   |                                                                        |
   |  this module contains the definition  of the object class HPGLPlot.    |
   |  This class is derived from XYPlot and provides for the construction   |
   |  of a text file containing all the HPGL commands for drawing the       |
   |  plot of simulation data represented by the structure pointed          |
   |  to by HPGLPlot::pplotspecs                                            |
   |                                                                        |
   |  to use this class, create an instance of HPGLPlot after getting a     |
   |  valid filename from the user.  Call FileOperationOK to check that no  |
   |  error has occurred, then call Show() to create the file. After        |
   |  creating the file, call FileOperationOK to check for IO errors.       |
   |  The file is automatically closed after the Show procedure             |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop
#include "msimplot.hxx"
#include "msimstrg.hxx"

#include <stdlib.h>



#if defined(__MAC__) && defined(__PPC__)
#include <sysdep.hxx>
#endif


#define  X_ORIGIN                                0
#define  Y_ORIGIN                                0
#define  hpglXMAX                                1000
#define  hpglYMAX                                1000
#define  NEWLINE                                 "\n"
#define  glTERM                                  ";"
#define  glTERM_NEWLINE                          ";\n"
#define  glLBL_TERM                              "\3"
#define  glINITIALIZE                            "IN;\n"
#define  glSCALE_USER                            "SC %hd %hd %hd %hd;"
#define  glSET_SCALE_PTS                         "IP %.4f %.4f %.4f %.4f;\n"
#define  glSET_WINDOW                            "IW %.4f %.4f %.4f %.4f;"
#define  glSET_SCALE_PTS_DEFLT                   "IP;"
#define  glSET_WINDOW_DEFAULT                    "IW;\n"
#define  glSTD_CHAR_SET                          "CS;"
#define  glDEFINE_LBL_TERM                       "DT\3;"
#define  glSET_ABS_CHAR_SIZE                     "SI %.4f %.4f;\n"
#define  glSELECT_PEN                            "SP %d;"
#define  glROTATE_PLOT_90                        "RO90;\n"
#define  glROTATE_TEXT_90                        "DI 0,1;"
#define  glROTATE_TEXT_0                         "DI 1,0;"
#define  glPEN_UP                                "PU;"
#define  glPEN_UP_AND_MOVE                       "PU %hd %hd;"
#define  glPEN_DOWN_AND_MOVE                     "PD %hd %hd;"
#define  glPEN_DOWN_AND_MOVE_NEWLINE             "PD %hd %hd;\n"
#define glEDGE_RECT_ABS                          "EA %hd %hd;"
#define  glPEN_DOWN                              "PD;"
#define  glPEN_DOWN_NEWLINE                      "PD;\n"
#define  glMOVE_PEN_ABSOLUTE                     "PA %hd %hd"
#define  glXY_COORD_PAIR                         " %hd %hd"
#define  glMOVE_PEN_RELATIVE                     "PR %hd %hd"
#define  glMOVE_PEN_CHAR_UNITS                   "CP %11.4f %11.4f;"
#define  glDRAW_LABEL                            "LB%s\3;"
#define  glSTORE_PEN                             "PU;SP0;\n"
#define  glSET_LINETYPE_SOLID                    "LT;\n"
#define  glSET_LINETYPE_DOT                      "LT 1,0.5;\n"
#define  glSET_LINETYPE_DASH                     "LT 2,1.0;\n"
#define  glSET_LINETYPE_LONGDASH                 "LT 3,2.0;\n"
#define  glSET_LINETYPE_DASHDOT                  "LT 4,2.0;\n"
#define  glSET_LINETYPE_DBLDOT                   "LT 6,2.0;\n"
#define  glSET_LINETYPE_DASHDBLDOT               "LT 5,2.0;\n"
#define  glSET_LINETYPE_ALTERNATE                "LT 0\n;"
#define  glSET_LINETYPE_NULL                     "PU;\n"

/* we assume that pens are different colors in the following order          */
/*                                                                          */
/*   pen 1   black                                                          */
/*       2   yellow                                                         */
/*       3   red                                                            */
/*       4   blue                                                           */
/*       5   green                                                          */
/*       6   cyan                                                           */
/*       7   : there is no pen 7, we use pen 1 again                        */

#define  glSELECT_PEN_BLACK                      "SP1;\n"
#define  glSELECT_PEN_YELLOW                     "SP2;\n"
#define  glSELECT_PEN_RED                        "SP3;\n"
#define  glSELECT_PEN_BLUE                       "SP4;\n"
#define  glSELECT_PEN_GREEN                      "SP5;\n"
#define  glSELECT_PEN_CYAN                       "SP6;\n"
#define  glSELECT_PEN_PINK                       "SP1;\n"
#define  glSELECT_PEN_WHITE                      "PU;\n"

/* define plot positioning in inches, assuming 8/12 x 11" paper             */
/* which actually has available uses 7.8 x 10.15 inches                     */

#define  LEFT_BLANKSPACE                         ( ( 10.15 - \
             DRAWING_AREA_WIDTH_IN_INCHES) / 2.0)
#define  BOTTOM_BLANKSPACE                       ( ( 7.8 - \
             DRAWING_AREA_HEIGHT_IN_INCHES) / 2.0 )
#define  PLOTTER_UNITS_PER_INCH                  1016
#define  INCHES_PER_CM                           (1.0 / 2.54)

/* the fixes relation between how HPGL and points systems calc font sizes   */
/* a 9 pt font should be 9/72 == 1/8 " tall overall including descenders    */
/* HPGL sets font size such that the height of a capital letter is 1/8 "    */
/* so this SCALING_FACTOR takes into account 0.25 of char cell for descenders*/

#define glHT_SCALING_FACTOR             2.00
#define glWIDTH_SCALING_FACTOR          1.50

#define  SCALING_FACTOR                          0.75
#define  P1X                                     LEFT_BLANKSPACE * \
             PLOTTER_UNITS_PER_INCH
#define  P1Y                                     (BOTTOM_BLANKSPACE + \
             DRAWING_AREA_HEIGHT_IN_INCHES)* PLOTTER_UNITS_PER_INCH
#define  P2X                                     (LEFT_BLANKSPACE + \
             DRAWING_AREA_WIDTH_IN_INCHES)* PLOTTER_UNITS_PER_INCH
#define  P2Y                                     BOTTOM_BLANKSPACE * \
             PLOTTER_UNITS_PER_INCH
#define  P1X_ROT                                 LEFT_BLANKSPACE * \
             PLOTTER_UNITS_PER_INCH
#define  P2Y_ROT                                 BOTTOM_BLANKSPACE * \
             PLOTTER_UNITS_PER_INCH
#define  P2X_ROT                                 (LEFT_BLANKSPACE + \
             DRAWING_AREA_HEIGHT_IN_INCHES)* PLOTTER_UNITS_PER_INCH
#define  P1Y_ROT                                 (BOTTOM_BLANKSPACE + \
             DRAWING_AREA_WIDTH_IN_INCHES)* PLOTTER_UNITS_PER_INCH
#define  SEGMENT_SIZE                            10



HPGLPlot::HPGLPlot( msimWID pParent, PPLOT_SPECS PlotSpecs, PCHAR Filename ) :
XYPlot ( pParent, PlotSpecs)
{
     // initialize vars

     io_error = FALSE;
     filename = Filename;

     // make local copy of plotspecs data since we will prob modify it

     plot_specs = *PlotSpecs;
     pplot_specs = &plot_specs;

     ChangeColorSet( msimPAPER_SET );

     plot_specs.vertical_win_size = window_height = hpglYMAX;
     plot_specs.horizontal_win_size = window_width = hpglXMAX;

     OpenTargetFile( );

}


msimBOOL HPGLPlot::FileOperationOK( )
{
     return ! io_error;
};


// write std info to file which sets up page parameters

void HPGLPlot::WriteHeader( )
{
     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, glINITIALIZE );

     if ( pplot_specs->landscape == FALSE )
     {
          fprintf( f, glROTATE_PLOT_90 );

          fprintf( f, glSET_SCALE_PTS, P1X_ROT, P1Y_ROT, P2X_ROT,
               P2Y_ROT );

          fprintf( f, glSET_WINDOW_DEFAULT );
     }
     else
     {
          fprintf( f, glSET_SCALE_PTS, P1X, P1Y, P2X, P2Y );

          fprintf( f, glSET_WINDOW_DEFAULT );
     }

     fprintf( f, glSTD_CHAR_SET );
     fprintf( f, glDEFINE_LBL_TERM );
     fprintf( f, glSCALE_USER, X_ORIGIN, window_width, Y_ORIGIN, window_height );

     fprintf( f, NEWLINE );

     if ( ferror( f ) )
          io_error = TRUE;

     return;
}

void HPGLPlot::WriteFooter( )
{
     if ( f )
          fprintf( f, glSTORE_PEN );

     if ( ferror( f ) )
          io_error = TRUE;

     if ( 0 != fclose( f ) )
          io_error = TRUE;

#if defined(__MAC__) && defined(__PPC__)

     Sysdepen::SetFileInfo( String( filename), String( msimTEXTFILE_TYPE),
                  String( msimDEFAULT_CREATOR_NAME) );

#endif

     return;

}

void HPGLPlot::OpenTargetFile( )
{
     if ( NULL == ( f = fopen( filename, "wt" ) ) )
          io_error = TRUE;

     return;
}

void HPGLPlot::xShow( )
{
     WriteHeader( );
     InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );
     DrawSelectedPlots( );
     WriteFooter( );
}

void HPGLPlot::SetLineStyle( enum PenStyle RequestedLineType )
{
     static PCHAR linetype_str[] =
             {
                  glSET_LINETYPE_NULL, glSET_LINETYPE_SOLID, glSET_LINETYPE_DOT,
                  glSET_LINETYPE_DASH, glSET_LINETYPE_DASHDOT
             }
     ;

     if ( f )
          fprintf( f, linetype_str[RequestedLineType] );
     else
          io_error = TRUE;

     return;

}

void HPGLPlot::SetColor( enum ColorName RequestedLineColor )
{
     static PCHAR linecolor_str[] =
             {
                  glSELECT_PEN_BLACK,
                  glSELECT_PEN_BLUE,
                  glSELECT_PEN_GREEN,
                  glSELECT_PEN_CYAN,
                  glSELECT_PEN_RED,
                  glSELECT_PEN_BLACK,  // magenta
                  glSELECT_PEN_YELLOW, // brown
                  glSELECT_PEN_BLACK,  // gray
                  glSELECT_PEN_BLACK,  // lightgray
                  glSELECT_PEN_BLUE,   // light blue
                  glSELECT_PEN_GREEN,  // light green
                  glSELECT_PEN_CYAN,   // light cyan
                  glSELECT_PEN_RED,    // light red
                  glSELECT_PEN_BLACK,  // light magenta
                  glSELECT_PEN_YELLOW, // yellow
                  glSELECT_PEN_WHITE   // white
             }
     ;

     if ( RequestedLineColor > COL_WHITE )
          RequestedLineColor = COL_BLACK;

     if ( f )
          fprintf( f, linecolor_str[RequestedLineColor] );
     else
          io_error = TRUE;

     return;
}


void HPGLPlot::SetLineOrientation( USHORT Angle )
{
     // note angle is in 1/10 deg units, ie 90 deg == 900
     if ( ! f )
          io_error = TRUE;
     else
     {
          switch ( Angle )
          {
               // add other cases later if needed
          case 900 :

               fprintf( f, glROTATE_TEXT_90 );
               break;

          default :
               fprintf( f, glROTATE_TEXT_0 );
               break;
          }
     }
     return;
}

Pen HPGLPlot::xChangePen( const Pen& rNewPen )
{
     SetColor( rNewPen.GetColor( ) .GetColorName( ) );
     SetLineStyle( rNewPen.GetStyle( ) );
     return ( rNewPen );
}

Font HPGLPlot::xChangeFont( const Font& rNewFont )
{
     SetLineOrientation( rNewFont.GetLineOrientation( ) );
     return rNewFont;
}

void HPGLPlot::xDrawRect( const Rectangle& rRect )
{
     // here we use line primitives rather than EA instruction since not
     // all HPGL devices seem to have this instruction ??

     if ( ! f )
          io_error = TRUE;
     else
     {
          fprintf( f, glPEN_UP_AND_MOVE, rRect.Left( ), rRect.Bottom( ) );
          fprintf( f, glPEN_DOWN_AND_MOVE, rRect.Right( ), rRect.Bottom( ) );
          fprintf( f, glMOVE_PEN_ABSOLUTE, rRect.Right( ), rRect.Top( ) );
          fprintf( f, glMOVE_PEN_ABSOLUTE, rRect.Left( ), rRect.Top( ) );
          fprintf( f, glPEN_DOWN_AND_MOVE_NEWLINE, rRect.Left( ), rRect.Bottom( ) );
     }
     return;
}

void HPGLPlot::xDrawLine( const Point& rStartPt, const Point&
          rEndPt )
{
     if ( ! f )
          io_error = TRUE;
     else
     {

          /* first move to the Start Point                                  */

          fprintf( f, glPEN_UP_AND_MOVE, rStartPt.X( ), rStartPt.Y( ) );
          fprintf( f, glPEN_DOWN_AND_MOVE_NEWLINE, rEndPt.X( ), rEndPt.Y( ) );
     }
     return;
}

void HPGLPlot::xDrawPolyLine( const Polygon& rPoly )
{
     USHORT num_full_segments;
     USHORT num_in_last_segment;
     USHORT i;
     USHORT j = 0;

     USHORT Numpts = rPoly.GetSize ( );

     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, glPEN_UP_AND_MOVE, rPoly[j].X( ), rPoly[j].Y( ) );

     num_full_segments = Numpts / SEGMENT_SIZE;

     num_in_last_segment = Numpts % SEGMENT_SIZE;

     fprintf( f, glPEN_DOWN_NEWLINE );

     while ( num_full_segments-- )
     {
          fprintf( f, glMOVE_PEN_ABSOLUTE, rPoly[j].X( ), rPoly[j].Y( ) );
          j++;

          for ( i = 1; i < SEGMENT_SIZE; i++ )
          {
               fprintf( f, glXY_COORD_PAIR, rPoly[j].X( ), rPoly[j].Y( ) );
               j++;
          }

          fprintf( f, NEWLINE );
     }

     if ( num_in_last_segment > 0 )
     {
          fprintf( f, glMOVE_PEN_ABSOLUTE, rPoly[j].X( ), rPoly[j].Y( ) );
          j++;

          for ( i = 1; i < num_in_last_segment; i++ )
          {
               fprintf( f, glXY_COORD_PAIR, rPoly[j].X( ), rPoly[j].Y( ) );
               j++;
          }

          fprintf( f, glTERM_NEWLINE );
     }

     return;

}


void HPGLPlot::xDrawText( const Point& rStartPt, const String& rStr, USHORT, USHORT )
{

     msimSTRING str;

     msimStringCopy( str, rStr, sizeof str );
     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, glPEN_UP_AND_MOVE,
          rStartPt.X( ),
          rStartPt.Y( ) - Round( char_ht / ( 2.0 * glHT_SCALING_FACTOR ) )
     );

     fprintf( f, glDRAW_LABEL, str );

     fprintf( f, NEWLINE );

     return;

}


void HPGLPlot::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor )
{
     current_marker_type = MarkerStyle;
     SetColor( ThisColor );
}



void HPGLPlot::DrawMarker( const Point& rPoint )
{
     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, glPEN_UP_AND_MOVE, rPoint.X( ), rPoint.Y( ) );

     switch ( current_marker_type )
     {
     case MARKER_CROSS :

          fprintf( f, "UC 3 3 99 -6 -6 -99 0 6 99 6 -6;\n" );
          return;

     case MARKER_CIRCLE :

          fprintf( f, "UC 3 -1 99 0 2 -2 2 -2 0 -2 -2 0 -2 2 -2 2 0 2 2;\n" );
          return;

     case MARKER_STAR6 :

          fprintf( f, "UC 3 2 99 -6 -4 -99 0 4 99 6 -4 -99 -3 -1 99 0 6;\n" );
          return;

     case MARKER_STAR8 :

          fprintf( f, "UC 4 0 99 -8 0 -99 1 3 99 6 -6 -99 -3 -1 99 0 8 -99 3 -1 99 -6 -6;\n" );
          return;

     case MARKER_SQUARE :

          fprintf( f, "UC 3 3 99 -6 0 0 -6 6 0 0 6;\n" );
          return;

     case MARKER_DIAMOND :

          fprintf( f, "UC 4 0 99 -4 4 -4 -4 4 -4 4 4;\n" );
          return;

     case MARKER_SOLIDDIAMOND :
          fprintf( f, "UC 4 0 99 -4 4 -4 -4 4 -4 4 4 -1 1 0 -2 -1 3 0 -4 -1 5 0 -6 -1 7 0 -8 -1 7 0 -6 -1 5 0 -4 -1 3 0 -2;\n" );

          return;

     case MARKER_SOLIDSQUARE :
          fprintf( f, "UC 3 3 99 -6 0 0 -6 6 0 0 6 -1 -6 0 6 -1 -6 0 6 -1 -6 0 6 -1 -6 0 6 -1 -6 0 6 -1 -6;\n" );
          return;

     case MARKER_PLUS :

          fprintf( f, "UC 4 0 99 -8 0 -99 4 4 99 0 -8;\n" );
          return;

     case MARKER_DEFAULT :
     case MARKER_DOT :
          return;
     }                                 /* endswitch                           */
     return;

}

void HPGLPlot::DrawPolyMarker( const Polygon& rPoly )
{
     USHORT num_full_segments;
     USHORT num_in_last_segment;
     USHORT i;
     USHORT j = 0;

     USHORT NumPoints = rPoly.GetSize ( );

     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, glPEN_UP_AND_MOVE, rPoly[j].X( ), rPoly[j].Y( ) );

     num_full_segments = NumPoints / SEGMENT_SIZE;

     num_in_last_segment = NumPoints % SEGMENT_SIZE;

     fprintf( f, glPEN_DOWN );

     while ( num_full_segments-- )
     {
          for ( i = 0; i < SEGMENT_SIZE; i++ )
               DrawMarker( rPoly.GetPoint( j++ ) );
     }

     if ( num_in_last_segment > 0 )
     {
          for ( i = 0; i < num_in_last_segment; i++ )
          {
               DrawMarker( rPoly.GetPoint( j++ ) );
          }
     }

     return;

}

void HPGLPlot::InitializeFontData( enum FontFamily,
          USHORT PointSize )
{
     // we ignore Typeface since HPGL does not give you any choice

     msimFLOAT fchar_ht = PointSize * (CM_PER_INCH / POINTS_PER_INCH )
     *SCALING_FACTOR;

     msimFLOAT fchar_width = FONT_ASPECT_RATIO * fchar_ht;

     if ( f )
          fprintf( f, glSET_ABS_CHAR_SIZE, fchar_width, fchar_ht );
     else
          io_error = TRUE;

     /* now set globals                                                     */

     if ( pplot_specs->landscape )
     {
          char_ht = Round(( fchar_ht * INCHES_PER_CM * hpglYMAX * glHT_SCALING_FACTOR ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          char_width = Round(( fchar_width * INCHES_PER_CM * hpglXMAX * glWIDTH_SCALING_FACTOR ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          rot_char_ht = Round(( fchar_ht * INCHES_PER_CM * hpglXMAX * glHT_SCALING_FACTOR ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          rot_char_width = Round(( fchar_width * INCHES_PER_CM * hpglYMAX * glWIDTH_SCALING_FACTOR ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

     }
     else
     {
          char_ht = Round(( fchar_ht * INCHES_PER_CM * hpglXMAX * glHT_SCALING_FACTOR ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          char_width = Round(( fchar_width * INCHES_PER_CM * hpglYMAX * glWIDTH_SCALING_FACTOR ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          rot_char_ht = Round(( fchar_ht * INCHES_PER_CM * hpglYMAX * glHT_SCALING_FACTOR ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          rot_char_width = Round(( fchar_width * INCHES_PER_CM * hpglXMAX * glWIDTH_SCALING_FACTOR ) /
               DRAWING_AREA_WIDTH_IN_INCHES );
     }

     // we use Font objects to tracl properties of fonts during output
     StdFont = Font( pplot_specs->typeface, Size( 0, pplot_specs->pointsize ) );

     RotFont = StdFont;

     RotFont.ChangeCharOrientation( 900 );
     RotFont.ChangeLineOrientation( 900 );

     return;
}


