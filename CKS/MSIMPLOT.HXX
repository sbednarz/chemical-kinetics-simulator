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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimplot.hxx                                                */
/*                                                                          */
/*  contains declarations and defines used in the plotting modules          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started August 22, 1993                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/* to activate the code for drawing exponents as superscripts, *            */
/* define WANT_EXPONENTS                                                    */

// define this to use bitmap technique for drawing markers
// #define USE_BITMAP

/* these are used for Postscript/HPGL file generation                       */

#define  POINTS_PER_INCH                         72.0
#define  CM_PER_INCH                             2.54
#define  DRAWING_AREA_HEIGHT_IN_INCHES           6.0

/* ratio avg char width to avg char height, empirically determined          */

#define  FONT_ASPECT_RATIO                       0.48
#define  DRAWING_AREA_WIDTH_IN_INCHES            8.0

/* these are for output of the plot data as a text file                     */

#define  FORMAT_STR                              "%-12.*s"
#define  NUMERIC_FORMAT_STR                      "%-12.*e"
#define  NUM_DEC_PLACES_IN_TABLE                 4
#define  INVALID_DATA_STR                        "---         "
#define  PRESSURE_STR                            "Press"
#define  TIME_STR                                "Time"
#define  VOLUME_STR                              "Volume"
#define  TEMPERATURE_STR                         "Temp"
#define  TEMP_UNITS_STR                          "deg K"
#define  HEADER_STR_1                            \
             "--- Simulation data for file \"%s\" \n"
#define  HEADER_STR_2                            \
             "--- This table was created on %s\n"
#define  SEPARATOR_CHAR                          '-'

/* these are for string/text manipulation                                   */

#define  EXTERNAL_FILE_STRING                    "External Data: "
#define  TEMPERATURE_UNITS                       "deg K"
#define  EXP_DATA_LABEL                          "Ext Data"

#define  USE_LONG_NAME                           TRUE

/* these define the (arbitrary) scaling of the graphics context             */

#define  GC_XMIN                                 (msimFLOAT) 0.0
#define  GC_YMIN                                 (msimFLOAT) 0.0
#define  GC_XMAX                                 (msimFLOAT) 1.0
#define  GC_YMAX                                 (msimFLOAT) 1.0

/* the following define positioning of x-y plots in a viewport              */
/* these are in multiples of the char height or char width in               */
/* dialog units                                                             */

#define  LEFT_SPACE                              11.0
#define  BOTTOM_SPACE                            4.0
#define  RIGHT_SPACE                             11.0
#define  TOP_SPACE                               2.0

/* these define characteristics of the tic marks                            */
/* the tic lengths are in units of fraction of the char height/width        */

#define  X_TIC_LENGTH                            0.5
#define  Y_TIC_LENGTH                            1.0
enum MARKER_STYLE
{
     MARKER_DEFAULT, MARKER_CROSS, MARKER_PLUS, MARKER_DIAMOND, MARKER_SQUARE,
     MARKER_STAR6, MARKER_STAR8, MARKER_SOLIDDIAMOND, MARKER_SOLIDSQUARE,
     MARKER_DOT, MARKER_CIRCLE
};


/* data definitions of various screen colors                                */

#define  SCREEN_DEFAULT_BKGRD_COLOR              COL_GRAY
#define  SCREEN_DEFAULT_AXIS_COLOR                   COL_BLACK
#define  SCREEN_DEFAULT_PLOT_COLOR                  COL_BLACK
#define  SCREEN_DEFAULT_EXP_DATA_COLOR             COL_BLACK
#define  SCREEN_DEFAULT_PEN_WIDTH                (USHORT) 0
#define  DEFAULT_MARKER_STYLE                    MARKER_STAR6
#define  DEFAULT_EXPDATA_MARKER_STYLE            MARKER_SOLIDDIAMOND
#define  DEFAULT_FONTFAMILY                          FAMILY_MODERN
#define  DEFAULT_POINTSIZE                           10
#define  DEFAULT_MAX_PLOT_PTS                        500
#define  DEFAULT_LINE_TYPE                           PEN_SOLID
#define  DEFAULT_LINEWIDTH                       0

/* plot characteristics. I define a "viewport" to be a                      */
/* subarea of the window that is to be drawn in                             */

#define  MAX_NO_OF_PLOTS                         7
#define  MAX_NO_VIEWPORTS                        4
#define  POLYLINE_LIMIT                          8000
#define  PLOTLIMIT_SCALINGFACTOR                 0.02

/* #defines for limit types - used during plotting                          */

#define  MIN_TIME                                0
#define  MAX_TIME                                1
#define  MIN_CONC                                2
#define  MAX_CONC                                3
#define  MIN_TEMP_Y                              4
#define  MAX_TEMP_Y                              5
#define  MIN_PRESS                               6
#define  MAX_PRESS                               7
#define  MIN_VOLUME                              8
#define  MAX_VOLUME                              9
#define  MIN_TEMP_X                              10
#define  MAX_TEMP_X                              11
#define  NUM_LIMITS                              12

/* #defines for data types - used during plotting of data                   */

#define  NO_DATA_TYPE                            0
#define  CONC_DATA                               (1 << 0)
#define  TEMP_DATA                               (1 << 1)
#define  PRESS_DATA                              (1 << 2)
#define  VOL_DATA                                (1 << 3)
#define  EXP_DATA                                (1 << 4)
#define  TIME_DATA                               (1 << 5)
#define  TEMP_DATA_Y                             (1 << 6)
#define  AMOUNT_DATA                             (1 << 7)


// used in axis routines
#define  MAX                                     80
#define  MAXTICKS                                40
#define  MAX_DEC_PLACES                          5
#define  MAXTICKS_STR                            msimMACRO_AS_STR(MAXTICKS)
#define  MAX_DEC_PLACES_STR                      msimMACRO_AS_STR( MAX_DEC_PLACES)
#define  EPSILON                                 ( (msimFLOAT) MAXTICKS * \
             DBL_EPSILON)
#define  EXPONENT_TAGS                           "eE"


typedef msimFLOAT PLOT_LIMITS[NUM_LIMITS], PTR PPLOT_LIMITS;

typedef USHORT INDEX_ARRAY[MAX_NO_OF_PLOTS];

class msimPlotFrameWindow;

/* this structure specifies the the coordinates of a viewport               */
/* in the GC coordinate system                                              */

struct VP_STRUCT
{
     msimFLOAT lower_left_x, upper_right_y, upper_right_x, lower_left_y
               ;
};

typedef struct VP_STRUCT VIEWPORT, VIEWPORT_ARRAY[MAX_NO_VIEWPORTS], PTR
     PVIEWPORT;

/* for axis plotting routines                                               */

typedef struct AXIS_STRUCT
{
     msimBOOL grid;                    /* whether to draw grid                */
     msimBOOL use_exponential_notation;
     msimFLOAT datamin,                /* minimum array value                 */
               datamax,                /* maximum array value                 */
               axismin,                /* lowest tick mark                    */
               axismax,                /* highest tick mark                   */
               axisrange,              /* = axismax-axismin                   */
               spacing,                /* major tick spacing                  */
               mult,                   /* multiplier times actual tick
                                          values                              */
               offset;                 /* amount to subract from actual
                                          tick values                         */
     USHORT num_ticks;
     USHORT num_dec_places;
     USHORT num_digits;
     CHAR tic_label_format[20];
} msimAXIS, PTR msimPAXIS;

/* this struct encapsulates all the info necesary for creating a plot       */

typedef struct
{
     msimPINSTANCE plot_instance;
     USHORT num_species_to_plot;
     INDEX_ARRAY plotspeciesindex;
     USHORT num_plots;
     ULONG num_data_sets;
     ULONG num_exp_data_sets;
     USHORT sim_data_interval;
     USHORT exp_data_interval;
     size_t max_pts_to_plot;
     msimBOOL exp_data_ok;
     msimBOOL plotconc_v_time;
     msimBOOL plottemp_v_time;
     msimBOOL plotpress_v_time;
     msimBOOL plotvol_v_time;
     msimBOOL plotconc_v_temp;
     msimBOOL plotpress_v_temp;
     msimBOOL plotvol_v_temp;
     msimBOOL plotexpdata;
     msimNAME_STRING plotlabel[MAX_NO_OF_PLOTS];
     msimPFLOAT Ptime;
     msimPFLOAT Ptemp;
     msimPFLOAT Pvol;
     msimPFLOAT Ppress;
     msimPFLOAT Pexpdata_x;
     msimPFLOAT Pexpdata_y;
     msimPFLOAT Pconc[MAX_NO_OF_PLOTS];
     msimBOOL plot_time_x;
     msimBOOL plot_temp_x;
     SHORT vertical_win_size;
     SHORT horizontal_win_size;
     msimBOOL vary_color;
     msimBOOL vary_linestyle;
     msimBOOL vary_markerstyle;
     msimBOOL show_marker;
     msimBOOL show_line;
     enum FontFamily typeface;
     USHORT pointsize;
     enum msimCOLORSET_TYPE current_colorset;
     enum ColorName background_color;
     enum ColorName axis_color;
     enum ColorName line_color[MAX_NO_OF_PLOTS + 1];
     enum ColorName default_line_color;
     enum ColorName exp_data_color;
     USHORT axis_line_width;
     enum PenStyle axis_line_style;
     enum PenStyle default_line_style;
     enum PenStyle line_style[MAX_NO_OF_PLOTS + 1];
     enum MARKER_STYLE default_marker_style;
     enum MARKER_STYLE marker_style[MAX_NO_OF_PLOTS + 1];
     enum msimPLOT_FILE_TYPE default_output_filetype;
     USHORT expdata_x_type;
     USHORT expdata_y_type;
     msimBOOL encapsulated_ps;
     msimBOOL landscape;
     msimBOOL show_filename;
     msimBOOL use_color_in_plot_file;
     msimAXIS time_axis;
     msimAXIS conc_axis;
     msimAXIS temp_y_axis;
     msimAXIS temp_x_axis;
     msimAXIS press_axis;
     msimAXIS volume_axis;
     msimBOOL show_grid;
     msimBOOL plot_was_visible_before;
} PLOT_SPECS, PTR PPLOT_SPECS;


extern const PLOT_SPECS C_PLOT_SPECS;



class MainPlotDialog : public ModelessDialog
{
protected :
     CheckBox aPlotConcTimeCB;
     CheckBox aPlotTempTimeCB;
     CheckBox aPlotVolTimeCB;
     CheckBox aPlotPressTimeCB;
     CheckBox aPlotConcTempCB;
     CheckBox aPlotVolTempCB;
     CheckBox aPlotPressTempCB;
     GroupBox aGroupBox1;

     CheckBox aPlotExtrnDataCB;
     PushButton aLoadExtrnDataPB;
     GroupBox aGroupBox2;

     MultiListBox aDefinedSpeciesMLB;
     GroupBox aPlotSpeciesGB;

     FixedText aFixedText2;
     FixedText aFixedText3;
     FixedText aNumPlotsAvailText;
     FixedText aNumSpeciesSelectedText;
     Edit aMaxPointsSLE;
     GroupBox aGroupBox4;

     OKButton aPlotCancelBtn;
     PushButton aPlotPlotBtn;
     HelpButton aPlotHelpBtn;

     msimNAME_STRING def_name, plot_name;
     USHORT def_selection, plot_selection;
     USHORT num_species_selected;
     msimBOOL show_species;
     msimFILE_STRING extern_filename;

     void SetExtFileButtonText( PCHAR Filename );
     void InitializePlotDataDialog( msimPINSTANCE Instance );
     msimRC GetUserSelections( );
     msimRC ReadAndStoreSimulationData( );
     msimRC ReadSimulationData( );
     void InitializeSpeciesSelection( msimBOOL State );

public :
     PLOT_SPECS PlotSpecs;

     msimPlotFrameWindow PTR plot_frame_window;

     MainPlotDialog( Window * pParent, msimPINSTANCE Instance );
     ~MainPlotDialog( );

     void ConcBtnHandler( CheckBox * );
     void LoadExternDataHandler( PushButton * );
     void PlotCancelHandler( PushButton * );
     void PlotPlotHandler( PushButton * );
     void LBSelectHdl( MultiListBox PTR );
     msimRC ReadMaxPointsLB( );
     void Hide( );
     void Show( );

};


class XYPlot
{
protected :
     SHORT window_width;
     SHORT window_height;
     PPLOT_SPECS pplot_specs;
     SHORT char_ht;
     SHORT char_width;
     SHORT rot_char_ht, rot_char_width;
     SHORT lolim_x, hilim_x, lolim_y, hilim_y;
     msimWID owner;
     msimBOOL true_rotated_font;


     Font RotFont;
     Font StdFont;

public :
     virtual void xShow( ) = 0;
     virtual Pen xChangePen( const Pen& rNewPen ) = 0;
     virtual Font xChangeFont( const Font& rNewFont ) = 0;
     virtual void xDrawLine( const Point& rStartPt, const Point&
          rEndPt ) = 0;
     virtual void xDrawPolyLine( const Polygon& rPoly ) = 0;
     virtual void xDrawText( const Point& rStartPt, const String&
          rStr, USHORT nPos = 0, USHORT nLen = STRING_LEN ) = 0;
     virtual void xDrawRect( const Rectangle& rRect ) = 0;

     virtual void DrawMarker( const Point& rPoint ) = 0;
     virtual void DrawPolyMarker( const Polygon& rPoly ) = 0;
     virtual void SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor ) = 0;


     void DrawSelectedPlots( );

     void CalcSubWindowSizeInDeviceCoords( USHORT NumPlots, USHORT ThisPlotNum );

     void DrawAxes( USHORT
          XDataType, USHORT YDataType, msimPINSTANCE
          Instance, msimPAXIS XAxisData, msimPAXIS
          YAxisData );

     void WriteSpeciesLabel( PCHAR Text, USHORT LabelNum, enum
          ColorName PlotColor, enum PenStyle
          LineStyle, enum MARKER_STYLE
          MarkerStyle,
          msimBOOL
          ShowLines, msimBOOL ShowMarkers );
     void WriteFilename( PCHAR Filename );

     void PlotTheData( msimFLOAT MinX, msimFLOAT MaxX,
          msimFLOAT MinY, msimFLOAT MaxY,
          msimPFLOAT Xdata, msimPFLOAT
          Ydata, USHORT NumPoints, USHORT
          Interval, enum ColorName
          PlotColor, enum PenStyle
          LineStyle, enum MARKER_STYLE
          MarkerStyle,
          msimBOOL
          ShowLines, msimBOOL ShowMarkers );
     SHORT WriteXTicLabel( Point& pt, PCHAR mantissa );
     SHORT WriteYTicLabel( Point& pt, PCHAR mantissa );

     XYPlot( msimWID pParent, PPLOT_SPECS pPlotSpecs );

     virtual void InitializeFontData( enum FontFamily TypeFace, USHORT
          PointSize ) = 0;
     enum msimCOLORSET_TYPE ChangeColorSet( enum msimCOLORSET_TYPE NewColorSet );
     Font& GetStdFont( )
     {
          return StdFont;
     };
     Font& GetRotFont( )
     {
          return RotFont;
     };

};


class XYWindow : public XYPlot, public Window
{
protected :

#if defined(USE_BITMAP)
     Bitmap marker_bitmap;
#endif

     void DrawTheMarker( const Point& rPoint );

     enum MARKER_STYLE current_marker_type;

public :
     XYWindow( msimWID pParent, PPLOT_SPECS PlotSpecs );

     virtual void Paint( const Rectangle& );
     virtual void Resize( );
     virtual void InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize );

     virtual void DrawMarker( const Point& rPoint );
     virtual void DrawPolyMarker( const Polygon& rPoly );
     virtual void SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor );

     virtual void xShow( );
     virtual Pen xChangePen( const Pen& rNewPen );
     virtual Font xChangeFont( const Font& rNewFont );
     virtual void xDrawLine( const Point& rStartPt, const Point&
          rEndPt );
     virtual void xDrawPolyLine( const Polygon& rPoly );

     virtual void xDrawText( const Point& rStartPt, const String&
          rStr, USHORT nPos = 0, USHORT nLen = STRING_LEN );

     virtual void xDrawRect( const Rectangle& rRect );

};


class HPGLPlot : public XYPlot
{
protected :
     BOOL io_error;              // for tracking whether file error has occurred
     PCHAR filename;
     FILE PTR f;
     PLOT_SPECS plot_specs;       // loca copy to modify w/o affecting plot data

     enum MARKER_STYLE current_marker_type;

public :

     HPGLPlot( msimWID pParent, PPLOT_SPECS PlotSpecs, PCHAR Filename );

     // redefined fcns from base class OutputDevice

     virtual void xShow( );
     virtual Pen xChangePen( const Pen& rNewPen );
     virtual Font xChangeFont( const Font& rNewFont );
     virtual void xDrawLine( const Point& rStartPt, const Point&
          rEndPt );
     virtual void xDrawPolyLine( const Polygon& rPoly );
     virtual void xDrawText( const Point& rStartPt, const String&
          rStr, USHORT nPos = 0, USHORT nLen = STRING_LEN );

     // functions redefined from  base class XYPlot

     virtual void DrawMarker( const Point& rPoint );
     virtual void DrawPolyMarker( const Polygon& rPoly );
     virtual void SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor );
     virtual void InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize );


     virtual void xDrawRect( const Rectangle& rRect );

     // new functions added to HPGLPlot

     void WriteHeader( );
     msimBOOL FileOperationOK( );
     void OpenTargetFile( );
     void WriteFooter( );

protected :
     void SetLineStyle( enum PenStyle RequestedLineType );
     void SetColor( enum ColorName RequestedLineColor );
     void SetLineOrientation( USHORT Angle );

};

class PSPlot : public HPGLPlot
{
public :

     PSPlot( msimWID pParent, PPLOT_SPECS PlotSpecs, PCHAR Filename );

     // redefined fcns from base class OutputDevice

     virtual void xShow( );
     virtual Pen xChangePen( const Pen& rNewPen );
     virtual Font xChangeFont( const Font& rNewFont );
     virtual void xDrawLine( const Point& rStartPt, const Point&
          rEndPt );
     virtual void xDrawPolyLine( const Polygon& rPoly );
     virtual void xDrawText( const Point& rStartPt, const String&
          rStr, USHORT nPos = 0, USHORT nLen = STRING_LEN );

     // functions redefined from  base class XYPlot

     virtual void DrawMarker( const Point& rPoint );
     virtual void DrawPolyMarker( const Polygon& rPoly );
     virtual void SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor );

     virtual void InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize );

     virtual void xDrawRect( const Rectangle& rRect );

protected :

     // new functions added to PSPlot

     void PrintBaseFunctions( );
     void PrintOptionalFunctions( );
     PCHAR FontName( enum FontFamily TypeFace );
     PCHAR MarkerName( enum MARKER_STYLE MarkerStyle );

     // redefined from base class

     void WriteHeader( );
     void WriteFooter( );
     void SetLineStyle( enum PenStyle RequestedLineType );
     void SetColor( enum ColorName RequestedLineColor );
     void SetLineOrientation( USHORT Angle );

};


class msimPlotFrameWindow : public WorkWindow
{
protected :
     CancelButton aPlotCancelBtn;
     PushButton aPlotLimitsBtn;
     PushButton aChangeAttrsBtn;
     PushButton aPlotSaveBtn;
     PushButton aPlotPrintBtn;
     HelpButton aPlotHelpBtn;
     PPLOT_SPECS pplot_specs;
     virtual void KeyInput( const KeyEvent& rKEvent );

     msimPINSTANCE instance;

public :
     XYWindow aPlotXYWindow;

     MainPlotDialog PTR Owner;

     msimPlotFrameWindow( MainPlotDialog PTR pParent, msimPINSTANCE Instance );

     void PlotInvalidate( )
     {
          aPlotXYWindow.Invalidate( );
     };

     virtual void Resize( );
     void PlotLimitsBtnHandler( PushButton PTR );
     void PlotCancelBtnHandler( PushButton PTR );
     void PlotAttrsBtnHandler( PushButton PTR );
     void PlotSaveBtnHandler( PushButton PTR );
     void PlotPrintBtnHandler( PushButton PTR );
     void HelpHandler( PushButton PTR );
};

/* plot function declarations  generic to all plot classes                  */

USHORT msimScaleDataForPlotting( msimPFLOAT X, msimPFLOAT Y, Point Points[],
            USHORT TotalNumPts, USHORT Interval, msimFLOAT
            MinX, msimFLOAT MaxX, msimFLOAT MinY, msimFLOAT
            MaxY, SHORT LoLimX, SHORT HiLimX, SHORT LoLimY,
            SHORT HiLimY, USHORT ArraySize );

USHORT msimScalePointsForPlotting( msimPFLOAT X, msimPFLOAT Y, Point Points[],
            USHORT TotalNumPts, USHORT Interval, msimFLOAT
            MinX, msimFLOAT MaxX, msimFLOAT MinY, msimFLOAT
            MaxY, SHORT LoLimX, SHORT HiLimX, SHORT LoLimY,
            SHORT HiLimY, USHORT ArraySize );


msimRC msimWritePlotDataToFile( PPLOT_SPECS PlotSpecs, msimPINSTANCE Instance,
            msimWID Owner );

PCHAR msimAxisLabelText( msimFLOAT ScalingFactor, msimPINSTANCE Instance, USHORT
           DataType, msimBOOL TrueRotatedFont );

void msimCheckForInvalidLimits( PPLOT_SPECS PlotSpecs );
void msimAdjustLimits( PPLOT_SPECS PlotSpecs );
SHORT Round( msimFLOAT x );
msimFLOAT msimCalcConversionFactor( msimPINSTANCE Instance );
void msimFreePlotArrayMemory( PPLOT_SPECS PlotSpecs );

VOID msimSetPlotAndAxisLimits( PPLOT_SPECS PlotSpecs, msimFLOAT ScalingFactor );

void msimCalcNumFmt( PCHAR FmtStr, PUSHORT NumDigits, PUSHORT NumDecPlaces,
           msimFLOAT Max, msimFLOAT Spacing, size_t StringSize );

void msimConstructNumericLimitString( PCHAR Target, msimFLOAT Mantissa, msimFLOAT
          ScalingFactor, USHORT NumDigits, USHORT
          NumDecplaces );

void msimMakeExpStr( PCHAR buf, size_t BufSize );
msimFLOAT msimCalcMultiplierFactor( msimFLOAT Value );

VOID msimPlotDataDialog( msimWID Owner, msimPINSTANCE Instance );


// defined in msimsetp.cxx

void msimPrinterDialog( msimWID Owner, msimPINSTANCE pInstance, PPLOT_SPECS pPlotSpecs );


/* --- class XYPrint -------------------------------------------           */

class XYPrint : public XYPlot, public Printer
{
protected :
#if defined(USE_BITMAP)
     Bitmap marker_bitmap;
#endif

     void DrawTheMarker( const Point& rPoint );

     enum MARKER_STYLE current_marker_type;

public :
     XYPrint( msimWID pParent, PPLOT_SPECS PlotSpecs );

     void InitializeGeometricData( );


     virtual void Paint( const Rectangle& );
     virtual void Resize( );
     virtual void InitializeFontData( enum FontFamily TypeFace,
          USHORT PointSize );

     virtual void DrawMarker( const Point& rPoint );
     virtual void DrawPolyMarker( const Polygon& rPoly );
     virtual void SetMarker( enum MARKER_STYLE MarkerStyle, enum ColorName ThisColor );

     virtual void xShow( );
     virtual Pen xChangePen( const Pen& rNewPen );
     virtual Font xChangeFont( const Font& rNewFont );
     virtual void xDrawLine( const Point& rStartPt, const Point&
          rEndPt );
     virtual void xDrawPolyLine( const Polygon& rPoly );

     virtual void xDrawText( const Point& rStartPt, const String&
          rStr, USHORT nPos = 0, USHORT nLen = STRING_LEN );

     virtual void xDrawRect( const Rectangle& rRect );

};


