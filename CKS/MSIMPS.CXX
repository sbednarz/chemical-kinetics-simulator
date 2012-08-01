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
   |  msimps.cxx       :  defines the class HPGLPlot for MSIM               |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   Sept 26 1993                                       |
   |                                                                        |
   |  this module contains the definition of the object class PS_Plot.      |
   |  This class is derived from HPGLPlot and provides for the construction |
   |  of a text file containing all the PosScript commands for drawing the  |
   |  plot of simulation data represented by the structure pointed          |
   |  to by HPGLPlot::pplotspecs                                            |
   |                                                                        |
   |  to use this class, create an instance of PS_Plot after getting a      |
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
#include <time.h>


#if defined(__MAC__) && defined(__PPC__)
#include <sysdep.hxx>
#endif


#define  NEW_X_ORIGIN
#define  NEW_Y_ORIGIN                                0
#define  psXMAX                                  (DRAWING_AREA_WIDTH_IN_INCHES * POINTS_PER_INCH)
#define  psYMAX                                  (DRAWING_AREA_HEIGHT_IN_INCHES * POINTS_PER_INCH)
#define  NEWLINE                                 "\n"
#define psROTATE_90     "90 rotate" NEWLINE
#define psTRANSLATE "%s %s translate" NEWLINE
#define psPORTRAIT_ORIGIN_X     "90"   /* Round( ( 1.25 * POINTS_PER_INCH ) ) */
#define psPORTRAIT_ORIGIN_Y     "684"/* Round( ( (11.0 - 1.50) * POINTS_PER_INCH ) )*/

#define psLANDSCAPE_ORIGIN_X    "108"  /* Round( (  1.50 * POINTS_PER_INCH ) )*/
#define psLANDSCAPE_ORIGIN_Y    "-90"  /* Round( ( -7.25 * POINTS_PER_INCH ) )*/

#define psFIND_FONT   "/%s findfont[%hd 0 0 %hd 0 0] makefont setfont\n"


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

#define  SCALING_FACTOR                          1.00
#define  SEGMENT_SIZE                            10

PSPlot::PSPlot( msimWID pParent, PPLOT_SPECS PlotSpecs, PCHAR Filename ) :
HPGLPlot ( pParent, PlotSpecs, Filename )
{

     /* we use a different scaling system here than in the HPGL file */
     if ( pplot_specs->landscape )
     {
          plot_specs.vertical_win_size = window_height = Round( psYMAX );
          plot_specs.horizontal_win_size = window_width = Round( psXMAX );
     }
     else
     {
          plot_specs.vertical_win_size = window_height = Round( psXMAX );
          plot_specs.horizontal_win_size = window_width = Round( psYMAX );
     }

}

void PSPlot::WriteFooter( )
{
     if ( f )
     {
          fprintf( f, String( ResId( msimPS_END_OF_OUTPUT_STR ) ) );
          if ( ! pplot_specs->encapsulated_ps )
               fprintf( f, String( ResId( msimPS_SHOWPAGE_STR ) ) );
          fprintf( f, String( ResId( msimPS_TRAILER_STR ) ) );
     }

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


void PSPlot::xShow( )
{
     WriteHeader( );
     InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );
     DrawSelectedPlots( );
     WriteFooter( );
}


// write std info to file which sets up page parameters

void PSPlot::WriteHeader( )
{
     struct tm *current_time;
     time_t local_time;

     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     // here we print the prolog to the ps file
     fprintf( f, String( ResId( msimPS_HEADER_STR_1 ) ) );
     if ( pplot_specs->encapsulated_ps )
          fprintf( f, String( ResId( msimPS_HEADER_STR_2 ) ) );

     fprintf( f, String( ResId( msimPS_HEADER_STR_3 ) ) );
     fprintf( f, String( ResId( msimPS_HEADER_STR_4 ) ),
          plot_specs.plot_instance->base_filename );

     time( &local_time );              /* get time in seconds                 */
     current_time = localtime( &local_time );
     fprintf( f, String( ResId( msimPS_HEADER_STR_5 ) ), asctime( current_time ) );

     fprintf( f, String( ResId( msimPS_HEADER_STR_6 ) ), FontName( pplot_specs->typeface ) );

     fprintf( f, String( ResId( msimPS_HEADER_STR_7 ) ) );

     PrintBaseFunctions( );
     PrintOptionalFunctions( );

     // start non-function output

     // more stuff to set global vars
     fprintf( f, String( ResId( msimPS_HEADER_STR_8 ) ) );


     if ( pplot_specs->landscape )
     {
          fprintf( f, String( ResId( msimPS_HEADER_STR_9 ) ),
               psLANDSCAPE_ORIGIN_X, psLANDSCAPE_ORIGIN_Y );
     }
     else
          fprintf( f, String( ResId( msimPS_HEADER_STR_10 ) ),
               psPORTRAIT_ORIGIN_X, psPORTRAIT_ORIGIN_Y );

     fprintf( f, String( ResId( msimPS_HEADER_STR_11 ) ) );

     if ( ferror( f ) )
          io_error = TRUE;

     return;
}

void PSPlot::PrintBaseFunctions( )
{

     // the DrawRect Function

     fprintf( f, String( ResId( msimPS_DRAW_RECT_FCN_STR ) ) );

     // the DrawLine function

     fprintf( f, String( ResId( msimPS_DRAW_LINE_FCN_STR ) ) );

#if defined( USE_POLY_FCNS )
     // the DrawPolyLine function - not currently used

     fprintf( f, "%%  function to draw a polyline\n" "%%\n"
          "/pldict 5 dict def\n"
          "/DrawPolyLine\n"
          "{                    %% syntax : [x1 y1...xN yN] N DrawPolyLine\n"
          "     pldict \n"
          "     begin\n"
          "          -1 add 2 mul\n"
          "          /npts exch def    %% get 2*(N-1) for loop\n"
          "          /points exch def\n"
          "          newpath\n"
          "          points 0 get /x1 exch def\n"
          "          points 1 get /y1 exch def\n"
          "          x1 y1 moveto\n"
          "          /i 2 def\n"
          "          2 2 npts \n"
          "          {\n"
          "               points exch get /x1 exch def\n"
          "               i 1 add /i exch def\n"
          "               points i get /y1 exch def\n"
          "               i 1 add /i exch def\n"
          "               x1 y1 lineto\n"
          "          } for\n"
          "          stroke\n"
          "     end\n"
          "} bind def\n"
          "%%\n"
     );

#endif

     // std color, linestyle defs

     fprintf( f, String( ResId( msimPS_STD_DEFNS_STR ) ) );

}

void PSPlot::PrintOptionalFunctions( )
{

     if ( pplot_specs->vary_color && pplot_specs->use_color_in_plot_file )
     {
          // the color definitions

          fprintf( f, String( ResId( msimPS_COLOR_DEFNS_STR ) ) );
     }

     if ( pplot_specs->show_marker )
     {
          // this static string is handled in two parts. Apparently the string is
          // so long that Starview resource comiler cannto correctly handle it under AIX.
          // The symptom is that the app crashes early in the execution, when it
          // first accesses the resource manager (wdh 9.2.94)

          fprintf( f, String( ResId( msimPS_MARKER_FCNS_STR1 ) ) );

          fprintf( f, String( ResId( msimPS_MARKER_FCNS_STR2 ) ) );

#if defined( USE_POLY_FCNS )

    // not currently used
          fprintf( f, "%%\n"
               "/CurrentMarker {Cross} def %% used in PolyMarker\n"
               "%%\n"
               "%% function to draw multiple markers\n"
               "/pmdict 5 dict def\n"
               "/PolyMarker\n"
               "{    %% [x1 y1...xN yN] N PolyMarker - draw a set of markers\n"
               "     pmdict\n"
               "     begin\n"
               "          -1 add 2 mul\n"
               "          /npts exch def    %% get 2*(N-1) for loop\n"
               "          /points exch def\n"
               "          newpath\n"
               "          points 0 get /x1 exch def\n"
               "          points 1 get /y1 exch def\n"
               "          x1 y1 CurrentMarker\n"
               "          /i 2 def\n"
               "          2 2 npts\n"
               "          {\n"
               "               points exch get /x1 exch def\n"
               "               i 1 add /i exch def\n"
               "               points i get /y1 exch def\n"
               "               i 1 add /i exch def\n"
               "               x1 y1 CurrentMarker\n"
               "          } for\n"
               "          stroke\n"
               "     end\n"
               "}\n"
               "bind def\n"
          );
#endif

     }                                 // end if show_marker

     if ( pplot_specs->show_grid || pplot_specs->vary_linestyle )
          fprintf( f, String( ResId( msimPS_GRID_DEFNS_STR ) ) );

     if ( pplot_specs->vary_linestyle )
          fprintf( f, String( ResId( msimPS_LINE_ATTRS_STR ) ) );
}



void PSPlot::SetLineStyle( enum PenStyle RequestedLineType )
{
     static PCHAR linetype_str[] =
             {
                  "Solid\n",           // LINETYPE_NULL
                  "Solid\n",           // LINETYPE_SOLID
                  "Dot\n",             // LINETYPE_DOT
                  "Dash\n",            // LINETYPE_DASH
                  "DashDot\n"          // LINETYPE_DASHDOT
             };

     if ( f )
          fprintf( f, linetype_str[RequestedLineType] );
     else
          io_error = TRUE;

     return;
}


void PSPlot::SetColor( enum ColorName RequestedLineColor )
{
     static PCHAR linecolor_str[] =
             {
                  "Black\n",           // black
                  "Blue\n",            // blue
                  "Green\n",           // green
                  "Cyan\n",            // cyan
                  "Red\n",             // red
                  "Black\n",           // magenta
                  "Yellow\n",          // brown
                  "Black\n",           // gray
                  "Black\n",           // lightgray
                  "Blue\n",            // light blue
                  "Green\n",           // light green
                  "Cyan\n",            // light cyan
                  "Red\n",             // light red
                  "Black\n",           // light magenta
                  "Yellow\n",          // yellow
                  "White\n"            // white
             }
     ;

     if ( RequestedLineColor > COL_WHITE || ! pplot_specs->vary_color
               || ! pplot_specs->use_color_in_plot_file )
          RequestedLineColor = COL_BLACK;

     if ( f )
          fprintf( f, linecolor_str[RequestedLineColor] );
     else
          io_error = TRUE;

     return;
}

void PSPlot::SetLineOrientation( USHORT Angle )
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

               fprintf( f, "RotFont setfont\n" );
               break;

          default :
               fprintf( f, "StdFont setfont\n" );
               break;
          }
     }
     return;
}

Pen PSPlot::xChangePen( const Pen& rNewPen )
{
     SetColor( rNewPen.GetColor( ) .GetColorName( ) );
     SetLineStyle( rNewPen.GetStyle( ) );
     return ( rNewPen );
}

Font PSPlot::xChangeFont( const Font& rNewFont )
{
     SetLineOrientation( rNewFont.GetLineOrientation( ) );
     return rNewFont;
}

void PSPlot::xDrawLine( const Point& rStartPt, const Point&
          rEndPt )
{
     if ( ! f )
          io_error = TRUE;
     else

          fprintf( f, "%hd %hd %hd %hd DrawLine\n", rStartPt.X( ), rStartPt.Y( ),
               rEndPt.X( ), rEndPt.Y( )
          );

     return;
}

void PSPlot::xDrawRect( const Rectangle& rRect )
{
     if ( ! f )
          io_error = TRUE;
     else
     {
          fprintf( f, "%hd %hd %hd %hd DrawRectangle\n",
               rRect.Left( ), rRect.Bottom( ),
               rRect.Right( ), rRect.Top( )
          );
     }
     return;
}


#if defined( USE_POLY_FCNS )
void PSPlot::xDrawPolyLine( const Polygon& rPoly )
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

     fprintf( f, "[" );

     num_full_segments = Numpts / SEGMENT_SIZE;

     num_in_last_segment = Numpts % SEGMENT_SIZE;

     while ( num_full_segments-- )
     {

          for ( i = 0; i < SEGMENT_SIZE; i++, j++ )
               fprintf( f, "%hd %hd ", rPoly[j].X( ), rPoly[j].Y( ) );

          fprintf( f, "\n" );
     }

     if ( num_in_last_segment > 0 )
          for ( i = 0; i < num_in_last_segment; i++, j++ )
               fprintf( f, "%hd %hd ", rPoly[j].X( ), rPoly[j].Y( ) );

     fprintf( f, "] %hd DrawPolyLine\n", Numpts );

     return;

}
#else
// simpler means of drawing polyline, less memory intensive for PS device
// since we do not have a large array in the stack
void PSPlot::xDrawPolyLine( const Polygon& rPoly )
{

     USHORT Numpts = rPoly.GetSize ( );
     USHORT i = 1;

     while ( i < Numpts )
     {
          xDrawLine( Point( rPoly[i - 1].X( ), rPoly[i - 1].Y( ) ), Point( rPoly[i].X( ), rPoly[i].Y( ) ) );
          i++;
     }

     return;

}
#endif

void PSPlot::xDrawText( const Point& rStartPt, const String& rStr, USHORT, USHORT )
{

     msimSTRING str;

     msimStringCopy( str, rStr, sizeof str );
     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, "%hd %hd moveto (%s) show\n", rStartPt.X( ), rStartPt.Y( ), str );

     return;

}

void PSPlot::SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor )
{
     current_marker_type = MarkerStyle;
     SetColor( ThisColor );
}



void PSPlot::DrawMarker( const Point& rPoint )
{
     if ( ! f )
     {
          io_error = TRUE;
          return;
     }

     fprintf( f, "%hd %hd %s\n", rPoint.X( ), rPoint.Y( ), MarkerName( current_marker_type ) );

     return;
}


#if defined( USE_POLY_FCNS )
void PSPlot::DrawPolyMarker( const Polygon& rPoly )
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


     fprintf( f, "/CurrentMarker {%s} def\n" "[", MarkerName( current_marker_type ) );

     num_full_segments = NumPoints / SEGMENT_SIZE;

     num_in_last_segment = NumPoints % SEGMENT_SIZE;

     while ( num_full_segments-- )
     {
          for ( i = 0; i < SEGMENT_SIZE; i++, j++ )
               fprintf( f, "%hd %hd ", rPoly.GetPoint( j ) .X( ), rPoly.GetPoint( j ) .Y( ) );

          fprintf( f, "\n");
     }

     if ( num_in_last_segment > 0 )
     {
          for ( i = 0; i < num_in_last_segment; i++, j++ )
          {
               fprintf( f, "%hd %hd ", rPoly.GetPoint( j ) .X( ), rPoly.GetPoint( j ) .Y( ) );
          }
     }
     fprintf( f, "] %hd PolyMarker\n", NumPoints );

     return;

}
#else
void PSPlot::DrawPolyMarker( const Polygon& rPoly )
{
     USHORT Numpts = rPoly.GetSize ( );
     USHORT i = 0;

     while ( i < Numpts )
     {
          DrawMarker( Point( rPoly[i].X( ), rPoly[i].Y( ) ) );
          i++;
     }

     return;

}
#endif

void PSPlot::InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize )
{
     // we ignore Typeface since HPGL does not give you any choice
     msimFLOAT fchar_ht = PointSize * (CM_PER_INCH / POINTS_PER_INCH )
     *SCALING_FACTOR;

     msimFLOAT fchar_width = FONT_ASPECT_RATIO * fchar_ht;


     // std font defs

     if ( f )
          fprintf( f, String( ResId( msimPS_FONT_DEFNS_STR ) ),
               FontName( TypeFace ), PointSize, FontName( TypeFace ), PointSize
          );

     else
          io_error = TRUE;

     /* now set globals                                                     */

     if ( pplot_specs->landscape )
     {
          char_ht = Round(( fchar_ht * INCHES_PER_CM * psYMAX ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          char_width = Round(( fchar_width * INCHES_PER_CM * psXMAX ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          rot_char_ht = Round(( fchar_ht * INCHES_PER_CM * psXMAX ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          rot_char_width = Round(( fchar_width * INCHES_PER_CM * psYMAX ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

     }
     else
     {
          char_ht = Round(( fchar_ht * INCHES_PER_CM * psXMAX ) /
               DRAWING_AREA_WIDTH_IN_INCHES );

          char_width = Round(( fchar_width * INCHES_PER_CM * psYMAX ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          rot_char_ht = Round(( fchar_ht * INCHES_PER_CM * psYMAX ) /
               DRAWING_AREA_HEIGHT_IN_INCHES );

          rot_char_width = Round(( fchar_width * INCHES_PER_CM * psXMAX ) /
               DRAWING_AREA_WIDTH_IN_INCHES );
     }

     // we use Font objects to track properties of fonts during output

     StdFont = Font( pplot_specs->typeface, Size( 0, pplot_specs->pointsize ) );

     RotFont = StdFont;

     RotFont.ChangeCharOrientation( 900 );
     RotFont.ChangeLineOrientation( 900 );

     return;
}



PCHAR PSPlot::FontName( enum FontFamily TypeFace )
{
     static PCHAR tr = "Times-Roman";
     static PCHAR c = "Courier";
     static PCHAR h = "Helvetica";

     switch ( TypeFace )
     {
     case FAMILY_ROMAN :
          return tr;

     case FAMILY_MODERN :
     default :
          return c;

     case FAMILY_SWISS :
          return h;

     }
}


PCHAR PSPlot::MarkerName( enum MARKER_STYLE MarkerStyle )
{
     static PCHAR cross = "Cross";
     static PCHAR circle = "Circle";
     static PCHAR star6 = "Star6";
     static PCHAR star8 = "Star8";
     static PCHAR square = "Square";
     static PCHAR diamond = "Diamond";
     static PCHAR soliddiamond = "SolidDiamond";
     static PCHAR solidsquare = "SolidSquare";
     static PCHAR plus = "Plus";

     switch ( MarkerStyle )
     {
     case MARKER_CROSS :
     default :

          return cross;

     case MARKER_CIRCLE :

          return circle;

     case MARKER_STAR6 :

          return star6;

     case MARKER_STAR8 :

          return star8;

     case MARKER_SQUARE :

          return square;

     case MARKER_DIAMOND :

          return diamond;

     case MARKER_SOLIDDIAMOND :

          return soliddiamond;

     case MARKER_SOLIDSQUARE :
          return solidsquare;

     case MARKER_PLUS :

          return plus;

     }                                 /* endswitch                           */
}

 