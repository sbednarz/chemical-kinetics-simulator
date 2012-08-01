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
/*  FILE NAME : msimplot.cxx                                                */
/*                                                                          */
/*  This module  contains the functions for creating and processing the     */
/*  plot frame window  is invoked when the user selects the 'plot' button   */
/*  from the PlotDialog window                                              */
/*                                                                          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started August  27 1993                                    */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimplot.hxx"
#include "msimfile.hxx"

#include <string.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>

#define  BORDER                10
#define PLOTWIN_WIDTH          6
#define PLOTWIN_HEIGHT         6

#define PLOTWIN_MINWIDTH       4
#define PLOTWIN_MINHEIGHT      4


class PlotSaveDialog : public ModalDialog
{
protected :

     RadioButton aPlotFileRawPSRB;
     RadioButton aPlotFileEncapsPSRB;
     RadioButton aPlotFileHPGL_RB;
     RadioButton aPlotFileTextRB;
     GroupBox aGroupBox1;

     RadioButton aPlotFileLandscapeOrientRB;
     RadioButton aPlotFilePortraitOrientRB;
     FixedBitmap aFixedBitmap1;
     FixedBitmap aFixedBitmap2;
     GroupBox aGroupBox2;
     CheckBox aPlotFileConvColorCB;

     OKButton aPlotFileCancelPB;
     PushButton aPlotFileCreatePB;
     HelpButton aPlotFileHelpButton;

     PPLOT_SPECS pplot_specs;
     msimBOOL orient_state;

public :

     PlotSaveDialog( Window PTR pParent, PPLOT_SPECS PlotSpecs );
     void PlotFileCancelBtnHandler( PushButton PTR );
     void PlotFileCreateBtnHandler( PushButton PTR );
     void OrientCtrlHandler( RadioButton PTR pRButton );
};


class PlotLimitsDialog : public ModalDialog
{

protected :
     FixedText aPlotLimTimeLbl;
     FixedText aPlotLimTempXLbl;
     Edit aPlotLimMinTimeSLE;
     Edit aPlotLimMaxTimeSLE;
     Edit aPlotLimTimeIncrSLE;
     ListBox aPlotLimTimeDecPlLB;
     Edit aPlotLimMinTempXSLE;
     Edit aPlotLimMaxTempXSLE;
     Edit aPlotLimTempXIncrSLE;
     ListBox aPlotLimTempXDecPlLB;
     GroupBox aGroupBox1;

     FixedText aPlotLimConcLbl;
     FixedText aPlotLimTempYLbl;
     FixedText aPlotLimPressLbl;
     FixedText aPlotLimVolLbl;
     Edit aPlotLimMinConcSLE;
     Edit aPlotLimMaxConcSLE;
     Edit aPlotLimConcIncrSLE;
     ListBox aPlotLimConcDecPlLB;
     Edit aPlotLimMinTempYSLE;
     Edit aPlotLimMaxTempYSLE;
     Edit aPlotLimTempYIncrSLE;
     ListBox aPlotLimTempYDecPlLB;
     Edit aPlotLimMinPressSLE;
     Edit aPlotLimMaxPressSLE;
     Edit aPlotLimPressIncrSLE;
     ListBox aPlotLimPressDecPlLB;
     Edit aPlotLimMinVolSLE;
     Edit aPlotLimMaxVolSLE;
     Edit aPlotLimVolIncrSLE;
     ListBox aPlotLimVolDecPlLB;
     GroupBox aGroupBox2;

     FixedText aFixedText1;
     FixedText aFixedText2;
     FixedText aFixedText3;
     FixedText aFixedText4;
     FixedText aFixedText5;
     FixedText aFixedText6;
     FixedText aFixedText7;
     FixedText aFixedText8;

     OKButton aPlotLimClosePB;
     PushButton aPlotLimUndoPB;
     PushButton aPlotLimDefaultsPB;
     PushButton aPlotLimPreviousPB;
     HelpButton aPlotLimHelpPB;

     PPLOT_SPECS pplot_specs;
     USHORT volume_option;
     PLOT_SPECS temp_plot_data;
     static msimAXIS time_axis;
     static msimAXIS conc_axis;
     static msimAXIS temp_y_axis;
     static msimAXIS temp_x_axis;
     static msimAXIS press_axis;
     static msimAXIS volume_axis;
     static msimBOOL PrevPlotLimAvail;
     msimPlotFrameWindow PTR owner;

     msimRC ReadDialog( );
     void InitializeDialog( PPLOT_SPECS pPlotSpecs );
     PCHAR DataTypeString( USHORT DataType );
     PCHAR AxisDirectionString( USHORT DataType );
     void AxisEntryFieldErrorMsg( USHORT Position, USHORT DataType,
          PCHAR Value );
     msimBOOL ValidateLimitsData( msimREAL_STRING Source[], msimFLOAT Limits[], PSHORT
          DecPlaces, USHORT DataType );
     void FillInAxisStruct( msimPAXIS pAxis, msimPFLOAT LimitsVals, USHORT
          NumDecplaces );

public :
     PlotLimitsDialog( msimPlotFrameWindow PTR pParent, PPLOT_SPECS PlotSpecs );
     void PlotLimCloseBtnHandler( PushButton PTR );

     void PlotLimUndoHandler( PushButton PTR );
     void PlotLimDefaultsHandler( PushButton PTR );
     void PlotLimPreviousHandler( PushButton PTR );
};

class SampleWindow : public Window
{
protected :
     Font aLogFont;
     Font aPhysicalFont;

public :

     SampleWindow( Window PTR pParent,
          const ResId& rResId );

     virtual void Paint( const Rectangle& );
     void InitializeFontData( enum FontFamily TypeFace, USHORT PointSize );
     void ActivateNewFont( );
     inline Font& GetLogFont( )
     {
          return aLogFont;
     };
};


SampleWindow::SampleWindow( Window PTR pParent, const ResId& rResId ) :
Window ( pParent, rResId )
{
#if defined(OLD_MAPMODE)
    ChangeMapMode( MapMode( MAP_POINT,
               PixelToLogic( Point( 0, -10 ), MapMode( MAP_POINT ) ),
               Fraction( 1, 1 ), Fraction( -1, 1 ) ) );
#else
     ChangeMapMode( MapMode( MAP_POINT ) );
#endif

     ChangeBackgroundBrush( Brush( COL_WHITE, BRUSH_SOLID ) );
}

void SampleWindow::Paint( const Rectangle& )
{
     SHORT x, y;
     Size aTextSize( GetTextSize( GetText( ) ) );
     Size aWindowSize( GetOutputSize( ) );

     x = aWindowSize.Width( ) / 2 - aTextSize.Width( ) / 2;
     y = aWindowSize.Height( ) / 2 + abs( aTextSize.Height( ) ) / 2;

     DrawText( Point( x, y ), GetText( ) );
}


void SampleWindow::ActivateNewFont( )
{
     ChangeFont( aPhysicalFont );
     Invalidate( );
}


void SampleWindow::InitializeFontData( enum FontFamily TypeFace, USHORT PointSize )
{
     // spec the new desired font

     aLogFont = Font( TypeFace, Size( 0, PointSize ) );

     aLogFont.ChangeTransparent( TRUE );
     aLogFont.ChangeColor( Color( COL_BLACK ) );
     aLogFont.ChangeAlign( ALIGN_BOTTOM );

     /* now get font metrics and store values in private vars                    */

     FontMetric aMetric = GetFontMetric( aLogFont );

     aPhysicalFont = aMetric.GetPhysicalFont( );
}




class PlotAttrDialog : public ModalDialog
{
protected :
     CheckBox aPlotAttrVaryLineColorCB;
     CheckBox aPlotAttrVaryLineStyleCB;
     CheckBox aPlotAttrVaryMarkerStyleCB;
     GroupBox aGroupBox1;

     CheckBox aPlotAttrShowLinesCB;
     CheckBox aPlotAttrShowMarkersCB;
     CheckBox aPlotAttrShowFilenameCB;
     CheckBox aPlotAttrShowGridCB;
     GroupBox aGroupBox2;

     RadioButton aPlotAttrBlackBkgrdRB;
     RadioButton aPlotAttrBlueBkgrdRB;
     RadioButton aPlotAttrGrayBkgrdRB;
     RadioButton aPlotAttrWhiteBkgrdRB;
     FixedBitmap aFixedBitmap1;
     FixedBitmap aFixedBitmap2;
     FixedBitmap aFixedBitmap3;
     FixedBitmap aFixedBitmap4;
     GroupBox aGroupBox3;

     RadioButton aPlotAttrHelveticaRB;
     RadioButton aPlotAttrCourierRB;
     RadioButton aPlotAttrTimesRomanRB;
     ListBox aPlotAttrPointSizeLB;
     FixedText aFixedText1;
     SampleWindow aPlotAttrSampleWindow;
     GroupBox aGroupBox4;

     OKButton aPlotAttrOKBtn;
     PushButton aPlotAttrUndoBtn;
     PushButton aPlotAttrDefaultsBtn;
     HelpButton aPlotAttrHelpBtn;

     PPLOT_SPECS pplot_specs;
     enum FontFamily current_font;
     USHORT current_pointsize;

     void SetListBox( );
     void InitializeDialog( const PLOT_SPECS PTR pPlotSpecs );
     enum FontFamily ReadFontRadioButtons( );
     enum msimCOLORSET_TYPE ReadColorSetRadioButtons( );

public :

     PlotAttrDialog( Window PTR pParent, PPLOT_SPECS pPlotSpecs );

     void PlotAttrCloseHandler( PushButton PTR );

     void PlotAttrUndoHandler( PushButton PTR );
     void PlotAttrDefaultsHandler( PushButton PTR );

     void PlotAttrFontRadioBtnHandler( RadioButton PTR );
     void PlotAttrLBSelectHandler( ListBox PTR );

#if defined(__MAC__)
     void PlotAttrColorRadioBtnHandler( RadioButton PTR pRButton);
#endif

};


PlotAttrDialog::PlotAttrDialog( Window PTR pParent, PPLOT_SPECS pPlotSpecs )
: ModalDialog ( pParent, ResId ( msimPLOT_ATTR_DLG ) ),

aPlotAttrVaryLineColorCB ( this, ResId ( msimPLOT_ATTR_VARY_LINECOLOR ) ),
aPlotAttrVaryLineStyleCB ( this, ResId ( msimPLOT_ATTR_VARY_LINESTYLE ) ),
aPlotAttrVaryMarkerStyleCB ( this, ResId ( msimPLOT_ATTR_VARY_MARKERSTYLE ) ),
aGroupBox1 ( this, ResId ( 1 ) ),

aPlotAttrShowLinesCB ( this, ResId ( msimPLOT_ATTR_SHOW_LINES ) ),
aPlotAttrShowMarkersCB ( this, ResId ( msimPLOT_ATTR_SHOW_MARKERS ) ),
aPlotAttrShowFilenameCB ( this, ResId ( msimPLOT_ATTR_SHOW_FILENAME ) ),
aPlotAttrShowGridCB ( this, ResId ( msimPLOT_ATTR_SHOW_GRID ) ),
aGroupBox2 ( this, ResId ( 2 ) ),

aPlotAttrBlackBkgrdRB ( this, ResId ( msimPLOT_ATTR_BLACK_BKGRD ) ),
aPlotAttrBlueBkgrdRB ( this, ResId ( msimPLOT_ATTR_BLUE_BKGRD ) ),
aPlotAttrGrayBkgrdRB ( this, ResId ( msimPLOT_ATTR_GRAY_BKGRD ) ),
aPlotAttrWhiteBkgrdRB ( this, ResId ( msimPLOT_ATTR_WHITE_BKGRD ) ),
aFixedBitmap1 ( this, ResId ( 1 ) ),
aFixedBitmap2 ( this, ResId ( 2 ) ),
aFixedBitmap3 ( this, ResId ( 3 ) ),
aFixedBitmap4 ( this, ResId ( 4 ) ),
aGroupBox3 ( this, ResId ( 3 ) ),

aPlotAttrHelveticaRB ( this, ResId ( msimPLOT_ATTR_HELVETICA ) ),
aPlotAttrCourierRB ( this, ResId ( msimPLOT_ATTR_COURIER ) ),
aPlotAttrTimesRomanRB ( this, ResId ( msimPLOT_ATTR_TMSRMN ) ),
aPlotAttrPointSizeLB ( this, ResId ( msimPLOT_ATTR_POINTSIZE ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aPlotAttrSampleWindow ( this, ResId ( msimPLOT_ATTR_SAMPLE_TEXT ) ),
aGroupBox4 ( this, ResId ( 4 ) ),

aPlotAttrOKBtn ( this, ResId ( msimPLOT_ATTR_OK ) ),
aPlotAttrUndoBtn ( this, ResId ( msimPLOT_ATTR_UNDO ) ),
aPlotAttrDefaultsBtn ( this, ResId ( msimPLOT_ATTR_DEFAULTS ) ),
aPlotAttrHelpBtn ( this, ResId ( msimPLOT_ATTR_HELP ) )
{
     FreeResource( );

     pplot_specs = pPlotSpecs;

     aPlotAttrOKBtn.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrCloseHandler ) );
     aPlotAttrUndoBtn.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrUndoHandler ) );
     aPlotAttrDefaultsBtn.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrDefaultsHandler ) );
     aPlotAttrCourierRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrFontRadioBtnHandler ) );
     aPlotAttrHelveticaRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrFontRadioBtnHandler ) );
     aPlotAttrTimesRomanRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrFontRadioBtnHandler ) );
     aPlotAttrPointSizeLB.ChangeSelectHdl( LINK( this, PlotAttrDialog, PlotAttrLBSelectHandler ) );

#if defined(__MAC__)

     aPlotAttrBlackBkgrdRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrColorRadioBtnHandler ) );
     aPlotAttrBlueBkgrdRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrColorRadioBtnHandler ) );
     aPlotAttrGrayBkgrdRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrColorRadioBtnHandler ) );
     aPlotAttrWhiteBkgrdRB.ChangeClickHdl( LINK( this, PlotAttrDialog, PlotAttrColorRadioBtnHandler ) );

#endif

     current_font = pplot_specs->typeface;
     current_pointsize = pplot_specs->pointsize;

     aPlotAttrSampleWindow.InitializeFontData( current_font, current_pointsize );

     aPlotAttrSampleWindow.ActivateNewFont( );

     InitializeDialog( pplot_specs );

     aPlotAttrPointSizeLB.SelectEntry( String( pplot_specs->pointsize ) );

     msimCascadeWindowOnOwner( this, pParent );
}





void PlotAttrDialog::SetListBox( )
{
     String aOldSize = aPlotAttrPointSizeLB.GetSelectEntry( );

     aPlotAttrPointSizeLB.ChangeUpdateMode( FALSE );
     aPlotAttrPointSizeLB.Clear( );
     SHORT nHeight = GetDevFontSize ( aPlotAttrSampleWindow.GetLogFont ( ), 0 ) .Height( );

     if ( ! nHeight )
     {
          for ( int i = 1; i < 128; i++ )
               aPlotAttrPointSizeLB.InsertEntry( i );
     }
     else
     {
          USHORT n = GetDevFontSizeCount ( aPlotAttrSampleWindow.GetLogFont ( ) );
          for ( USHORT i = 0; i < n; i++ )
               aPlotAttrPointSizeLB.InsertEntry( GetDevFontSize( aPlotAttrSampleWindow.GetLogFont( ),
                         i ) .Height( ) );
     }
     aPlotAttrPointSizeLB.SelectEntry( aOldSize );
     aPlotAttrPointSizeLB.ChangeUpdateMode( TRUE );
     aPlotAttrPointSizeLB.Invalidate( );
}



void PlotAttrDialog::PlotAttrLBSelectHandler( ListBox PTR pListBox )
{
     current_pointsize = pListBox->GetSelectEntry( );

     aPlotAttrSampleWindow.InitializeFontData( current_font, current_pointsize );

     aPlotAttrSampleWindow.ActivateNewFont( );

     aPlotAttrSampleWindow.Invalidate( );
}

#if defined(__MAC__)

void PlotAttrDialog::PlotAttrColorRadioBtnHandler( RadioButton PTR pRButton)
{
     aPlotAttrBlackBkgrdRB.Check( pRButton == &aPlotAttrBlackBkgrdRB );
     aPlotAttrBlueBkgrdRB.Check( pRButton == &aPlotAttrBlueBkgrdRB );
     aPlotAttrGrayBkgrdRB.Check( pRButton == &aPlotAttrGrayBkgrdRB );
     aPlotAttrWhiteBkgrdRB.Check( pRButton == &aPlotAttrWhiteBkgrdRB );
}

#endif

void PlotAttrDialog::InitializeDialog( const PLOT_SPECS PTR pPlotSpecs )
{
     SetListBox( );
     aPlotAttrSampleWindow.Invalidate( );

     // do the color radio buttons

     switch ( pPlotSpecs->current_colorset )
     {

     default :
     case msimBLACK_BKGRD :
          aPlotAttrBlackBkgrdRB.Check( );
          break;

     case msimWHITE_BKGRD :
          aPlotAttrWhiteBkgrdRB.Check( );
          break;

     case msimBLUE_BKGRD :
          aPlotAttrBlueBkgrdRB.Check( );
          break;

     case msimGRAY_BKGRD :
          aPlotAttrGrayBkgrdRB.Check( );
          break;

     }
     // do the CheckBoxes

     aPlotAttrVaryLineColorCB.Check( pPlotSpecs->vary_color );
     aPlotAttrVaryLineStyleCB.Check( pPlotSpecs->vary_linestyle );
     aPlotAttrVaryMarkerStyleCB.Check( pPlotSpecs->vary_markerstyle );
     aPlotAttrShowLinesCB.Check( pPlotSpecs->show_line );
     aPlotAttrShowMarkersCB.Check( pPlotSpecs->show_marker );
     aPlotAttrShowFilenameCB.Check( pPlotSpecs->show_filename );
     aPlotAttrShowGridCB.Check( pPlotSpecs->show_grid );

     // the following was moved here from top of this fcn so that
         // this weird problem with the RB fonts did not occur on the MAC

     switch ( aPlotAttrSampleWindow.GetLogFont( ) .GetFamily( ) )
     {
     case FAMILY_SWISS :
     default :
          aPlotAttrHelveticaRB.Check( );
          break;

     case FAMILY_ROMAN :
          aPlotAttrTimesRomanRB.Check( );
          break;

     case FAMILY_MODERN :
          aPlotAttrCourierRB.Check( );
          break;
     }

}



void PlotAttrDialog::PlotAttrDefaultsHandler( PushButton PTR )
{
     current_font = C_PLOT_SPECS.typeface;
     current_pointsize = C_PLOT_SPECS.pointsize;

     aPlotAttrSampleWindow.InitializeFontData( current_font, current_pointsize );

     aPlotAttrSampleWindow.ActivateNewFont( );

     InitializeDialog( &C_PLOT_SPECS );

     aPlotAttrPointSizeLB.SelectEntry( String( current_pointsize ) );
}



void PlotAttrDialog::PlotAttrUndoHandler( PushButton PTR )
{
     current_font = pplot_specs->typeface;
     current_pointsize = pplot_specs->pointsize;

     aPlotAttrSampleWindow.InitializeFontData( current_font, current_pointsize );

     aPlotAttrSampleWindow.ActivateNewFont( );

     InitializeDialog( pplot_specs );

     aPlotAttrPointSizeLB.SelectEntry( String( current_pointsize ) );

}



void PlotAttrDialog::PlotAttrFontRadioBtnHandler( RadioButton PTR )
{
     current_font = ReadFontRadioButtons( );

     aPlotAttrSampleWindow.InitializeFontData( current_font, current_pointsize );

     SetListBox( );

     PlotAttrLBSelectHandler( &aPlotAttrPointSizeLB );

     aPlotAttrSampleWindow.ActivateNewFont( );

     aPlotAttrSampleWindow.Invalidate( );
}

enum msimCOLORSET_TYPE PlotAttrDialog::ReadColorSetRadioButtons( )
{
     if ( aPlotAttrBlueBkgrdRB.IsChecked( ) )
          return msimBLUE_BKGRD;
     else
          if ( aPlotAttrGrayBkgrdRB.IsChecked( ) )
               return msimGRAY_BKGRD;
          else
               if ( aPlotAttrWhiteBkgrdRB.IsChecked( ) )
                    return msimWHITE_BKGRD;
               else
                    return msimBLACK_BKGRD;
}



enum FontFamily PlotAttrDialog::ReadFontRadioButtons( )
{
     if ( aPlotAttrTimesRomanRB.IsChecked( ) )
          return FAMILY_ROMAN;
     else
          if ( aPlotAttrCourierRB.IsChecked( ) )
               return FAMILY_MODERN;
          else
               return FAMILY_SWISS;
}

void PlotAttrDialog::PlotAttrCloseHandler( PushButton PTR )
{
     pplot_specs->typeface = current_font;
     pplot_specs->pointsize = current_pointsize;
     pplot_specs->current_colorset = ReadColorSetRadioButtons( );

     pplot_specs->vary_color = aPlotAttrVaryLineColorCB.IsChecked( );
     pplot_specs->vary_linestyle = aPlotAttrVaryLineStyleCB.IsChecked( );
     pplot_specs->show_filename = aPlotAttrShowFilenameCB.IsChecked( );
     pplot_specs->vary_markerstyle = aPlotAttrVaryMarkerStyleCB.IsChecked( );
     pplot_specs->show_line = aPlotAttrShowLinesCB.IsChecked( );
     pplot_specs->show_marker = aPlotAttrShowMarkersCB.IsChecked( );
     pplot_specs->show_grid = aPlotAttrShowGridCB.IsChecked( );

     EndDialog( );
}


/* member fcns for msimPlotFrameWindow                                      */

void msimPlotFrameWindow::Resize( )
{

     // calculate the new window size

     SHORT xy_width = GetOutputSizePixel ( ) .Width ( ) - aPlotXYWindow.GetPosPixel ( ) .X
     ( ) - BORDER;
     SHORT xy_height = GetOutputSizePixel ( ) .Height ( ) - aPlotXYWindow.GetPosPixel ( ) .Y ( ) -
     BORDER;

     /* simply scale the aPlotXYWindow size with dialog size - the button,s size and position  */
     /* is fixed                                                             */

     aPlotXYWindow.ChangeOutputSizePixel( Size( xy_width, xy_height ) );

#if defined(OLD_MAPMODE)
     /* adjust map mode of aPlotXYWindow to deal with change in size          */
     /* the origin needs to be reset  */

     MapMode aOldMapMode = aPlotXYWindow.GetMapMode( );
     aOldMapMode.ChangeOrigin( PixelToLogic( Point( 0, xy_height * -1 ), aOldMapMode.GetMapUnit( ) ) );
     aPlotXYWindow.ChangeMapMode( aOldMapMode );
#endif
     /* update structure so that we aPlotXYWindow can retrieve size info    */

     Owner->PlotSpecs.vertical_win_size = abs( aPlotXYWindow.GetOutputSize( ) .Height( ) );//xy_height;
     Owner->PlotSpecs.horizontal_win_size = abs( aPlotXYWindow.GetOutputSize( ) .Width( ) );//xy_width;

     Invalidate( );

#if defined(__AIX__)

    // do nothing .. causes multiple redraws of plot under AIX

#else

     aPlotXYWindow.Invalidate( );

#endif

}

void msimPlotFrameWindow::KeyInput( const KeyEvent& rKeyEvt )
{
     // don't process input if window is not visible

     if ( ! IsVisible( ) )
          return;

     if ( rKeyEvt.GetKeyCode( ) .IsMod2( ) )
     {
          // go here if the Alt key (WIN, OS/2) is pressed

          switch ( rKeyEvt.GetKeyCode( ) .GetCode( ) )
          {
          case KEY_C :

               Hide( );
               break;

          case KEY_X :

               PlotLimitsBtnHandler( NULL );
               break;

          case KEY_A :

               PlotAttrsBtnHandler( NULL );
               break;

          case KEY_S :

               PlotSaveBtnHandler( NULL );
               break;

          case KEY_P :

               PlotPrintBtnHandler( NULL );
               break;

          case KEY_H :

               HelpHandler( NULL );
               break;

          default :

               WorkWindow::KeyInput( rKeyEvt );
               break;
          }

     }
     else

          // otherwise pass call to base class function

          WorkWindow::KeyInput( rKeyEvt );

}




void msimPlotFrameWindow::PlotPrintBtnHandler( PushButton PTR )
{
     // open the printer information dialog

     msimPrinterDialog( this, instance, & ( Owner->PlotSpecs ) );
}

void msimPlotFrameWindow::HelpHandler( PushButton PTR )
{
     pApp->GetHelp( ) ->Start(( ULONG ) msimPLOT_HELP );
}


msimPlotFrameWindow::msimPlotFrameWindow( MainPlotDialog PTR pParent,
     msimPINSTANCE Instance ) :
WorkWindow ( pParent, ResId ( msimPLOT_WINDOW ) ),
aPlotCancelBtn ( this, ResId ( msimPLOT_EXIT ) ),
aPlotLimitsBtn ( this, ResId ( msimPLOT_LIMITS ) ),
aChangeAttrsBtn ( this, ResId ( msimPLOT_CHANGE_ATTRS ) ),
aPlotSaveBtn ( this, ResId ( msimPLOT_SAVE_TO_FILE ) ),
aPlotPrintBtn ( this, ResId ( msimPLOT_PRINT ) ),
aPlotHelpBtn ( this, ResId ( msimPLOT_HELP ) ),
aPlotXYWindow ( this, & ( pParent->PlotSpecs ) )
{
     FreeResource( );

     Owner = pParent;
     pplot_specs = & ( Owner->PlotSpecs );
     instance = Instance;

     if ( ! msimINVALID_WINDOW_GEOMETRY( msimAppOptions.plot_window_position ) )
          msimRestoreCurrentWindowGeometry( this,
               &msimAppOptions.plot_window_position );
     else
     {
          /* if we are using default attachment, cascade from parent              */

          ChangeSizePixel( Size(( PLOTWIN_WIDTH * System::
                         GetScreenSizePixel( ) .Width( ) ) / 10,( PLOTWIN_HEIGHT * System
                         :: GetScreenSizePixel( ) .Height( ) ) / 10 ) );

          msimCascadeWindowOnOwner( this, pParent );
     }

     ChangeMinOutputSizePixel( Size(
               ( PLOTWIN_MINWIDTH * System::GetScreenSizePixel( ) .Width( ) ) / 10,
               ( PLOTWIN_MINHEIGHT * System::GetScreenSizePixel( ) .Height( ) ) / 10
          )
     );

     /* set main window title                                                */

     SetText( GetText( ) + String( Instance->base_filename ) );


     /* add callbacks for pushbuttons                                        */

     aPlotCancelBtn.ChangeClickHdl( LINK( this, msimPlotFrameWindow,
               PlotCancelBtnHandler ) );
     aPlotLimitsBtn.ChangeClickHdl( LINK( this, msimPlotFrameWindow,
               PlotLimitsBtnHandler ) );
     aChangeAttrsBtn.ChangeClickHdl( LINK( this, msimPlotFrameWindow,
               PlotAttrsBtnHandler ) );
     aPlotSaveBtn.ChangeClickHdl( LINK( this, msimPlotFrameWindow,
               PlotSaveBtnHandler ) );
     aPlotPrintBtn.ChangeClickHdl( LINK( this, msimPlotFrameWindow,
               PlotPrintBtnHandler ) );


     /* set initial position of aPlotXYWindow                               */
     /* we place its top level with top of Close button and BORDER over from right*/
     /* edge of CLose button                                                */

     aPlotXYWindow.ChangePosPixel( Point( aPlotCancelBtn.GetPosPixel( ) .X( ) +
               aPlotCancelBtn.GetSizePixel( ) .Width( ) + BORDER, aPlotCancelBtn.GetPosPixel
               ( ) .Y( ) ) );

     aPlotXYWindow.InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );

     aPlotCancelBtn.Show( );
     aPlotLimitsBtn.Show( );
     aChangeAttrsBtn.Show( );
     aPlotSaveBtn.Show( );
     aPlotPrintBtn.Show( );
     aPlotHelpBtn.Show( );
     aPlotXYWindow.Show( );
}

void msimPlotFrameWindow::PlotCancelBtnHandler( PushButton PTR )
{
     Hide( );
     Owner->ToTop( );
}

void msimPlotFrameWindow::PlotAttrsBtnHandler( PushButton PTR )
{
     PlotAttrDialog PTR pdlg = new PlotAttrDialog ( this, pplot_specs );

     if ( pdlg != NULL )
     {
          pdlg->Execute( );
          delete pdlg;

          // update stuff so that PLot Window exhibits the selected attributes

          aPlotXYWindow.ChangeColorSet( pplot_specs->current_colorset );

          aPlotXYWindow.InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );

          aPlotXYWindow.ChangeBackgroundBrush( Brush( pplot_specs->background_color, BRUSH_SOLID ) );

          aPlotXYWindow.Invalidate( );
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, this );

}


void msimPlotFrameWindow::PlotLimitsBtnHandler( PushButton PTR )
{
     PlotLimitsDialog dlg( this, pplot_specs );

     msimCascadeWindowOnOwner( &dlg, this );

     dlg.Execute( );
}



void msimPlotFrameWindow::PlotSaveBtnHandler( PushButton PTR )
{
     PlotSaveDialog dlg( this, pplot_specs );

     msimCascadeWindowOnOwner( &dlg, this );

     dlg.Execute( );
}

PlotSaveDialog::PlotSaveDialog( Window PTR pParent, PPLOT_SPECS PlotSpecs ) :
ModalDialog ( pParent, ResId ( msimPLOT_SAVE_DLG ) ),
aPlotFileRawPSRB ( this, ResId ( msimPLOT_FILE_PS_RAW ) ),
aPlotFileEncapsPSRB ( this, ResId ( msimPLOT_FILE_PS_ENCAPS ) ),
aPlotFileHPGL_RB ( this, ResId ( msimPLOT_FILE_HPGL ) ),
aPlotFileTextRB ( this, ResId ( msimPLOT_FILE_TEXT ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aPlotFileLandscapeOrientRB ( this, ResId ( msimPLOT_ORIENT_LANDSCAPE ) ),
aPlotFilePortraitOrientRB ( this, ResId ( msimPLOT_ORIENT_PORTRAIT ) ),
aFixedBitmap1 ( this, ResId ( 1 ) ),
aFixedBitmap2 ( this, ResId ( 2 ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aPlotFileConvColorCB ( this, ResId ( msimPLOT_FILE_COLOR_CONVERT ) ),
aPlotFileCancelPB ( this, ResId ( msimPLOT_FILE_CANCEL ) ),
aPlotFileCreatePB ( this, ResId ( msimPLOT_CREATE_FILE ) ),
aPlotFileHelpButton ( this, ResId ( msimPLOT_FILE_HELP ) )
{
     FreeResource( );
     aPlotFileCancelPB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               PlotFileCancelBtnHandler ) );
     aPlotFileCreatePB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               PlotFileCreateBtnHandler ) );

     aPlotFileRawPSRB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               OrientCtrlHandler ) );
     aPlotFileEncapsPSRB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               OrientCtrlHandler ) );
     aPlotFileHPGL_RB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               OrientCtrlHandler ) );
     aPlotFileTextRB.ChangeClickHdl( LINK( this, PlotSaveDialog,
               OrientCtrlHandler ) );

     pplot_specs = PlotSpecs;
     orient_state = TRUE;

     // initialize controls

     if ( pplot_specs->landscape )
          aPlotFileLandscapeOrientRB.Check( );
     else
          aPlotFilePortraitOrientRB.Check( );

     aPlotFileConvColorCB.Check( ! pplot_specs->use_color_in_plot_file );


     switch ( pplot_specs->default_output_filetype )
     {
     case msimPLOTFILE_TEXT :
     case msimPLOTFILE_UNKNOWN :
     default :

          aPlotFileTextRB.Check( );
          aPlotFileLandscapeOrientRB.Disable( );
          aPlotFilePortraitOrientRB.Disable( );
          aPlotFileConvColorCB.Disable( );
          aGroupBox2.Disable( );;
          orient_state = FALSE;
          break;

     case msimPLOTFILE_PS :

          aPlotFileRawPSRB.Check( );
          break;


     case msimPLOTFILE_EPS :

          aPlotFileEncapsPSRB.Check( );
          break;

     case msimPLOTFILE_HPGL :

          aPlotFileHPGL_RB.Check( );
          break;

     }                                 // end switch
}

void PlotSaveDialog::OrientCtrlHandler( RadioButton PTR pRButton )
{

#if defined(__MAC__)

     aPlotFileRawPSRB.Check( pRButton == &aPlotFileRawPSRB );
     aPlotFileEncapsPSRB.Check( pRButton == &aPlotFileEncapsPSRB );
     aPlotFileHPGL_RB.Check( pRButton == &aPlotFileHPGL_RB );
     aPlotFileTextRB.Check( pRButton == &aPlotFileTextRB );

#endif

     if ( orient_state == aPlotFileTextRB.IsChecked( ) )
     {
          if ( orient_state )
          {
               orient_state = FALSE;
               aPlotFileLandscapeOrientRB.Disable( );
               aPlotFilePortraitOrientRB.Disable( );
               aPlotFileConvColorCB.Disable( );
               aGroupBox2.Disable( );;
          }
          else
          {
               orient_state = TRUE;
               aPlotFileLandscapeOrientRB.Enable( );
               aPlotFilePortraitOrientRB.Enable( );
               aPlotFileConvColorCB.Enable( );
               aGroupBox2.Enable( );;
          }

     }
}


void PlotSaveDialog::PlotFileCreateBtnHandler( PushButton PTR )
{
     msimFILE_STRING filename;
     msimPLOT_FILE_TYPE filetype;
     msimBOOL file_error = FALSE;
     msimRC rc = msimNO_ERROR;
     msimSTRING title;
     PCHAR filetype_str[5] =
      {
           "Text", "Std. PostScript", "Encaps. PostScript", "HPGL", "Unknown"
      };

     // figure out what the user selection is
     // first the type of output file

     msimStringCopy( filename, msimNULL_STR, sizeof filename );
     if ( aPlotFileRawPSRB.IsChecked( ) )
          filetype = msimPLOTFILE_PS;
     else
          if ( aPlotFileEncapsPSRB.IsChecked( ) )
               filetype = msimPLOTFILE_EPS;
          else
               if ( aPlotFileHPGL_RB.IsChecked( ) )
                    filetype = msimPLOTFILE_HPGL;
               else
                    filetype = msimPLOTFILE_TEXT;

     sprintf( title, "Select Name for %s Output File", filetype_str[filetype] );

     // next get file name from user and make sure its OK to overwrite existing file

     if ( msimUSER_ABORT == msimGetFileName( this,
                    filetype == msimPLOTFILE_TEXT ? msimAppOptions.text_output_template :
                         msimAppOptions.graph_output_template,
                         filename, WB_SAVEAS, title, msimGRAFFILE_TYPE, sizeof filename ) )
          return;

     // save selected file type in plot_specs for use next time

     pplot_specs->default_output_filetype = filetype;


     aMainApp.Wait( TRUE );

     if ( filetype != msimPLOTFILE_TEXT )
     {
          pplot_specs->landscape = aPlotFileLandscapeOrientRB.IsChecked( );
          pplot_specs->use_color_in_plot_file = ! aPlotFileConvColorCB.IsChecked( );
     }

     switch ( filetype )
     {
     case msimPLOTFILE_TEXT :
     default :
          msimStringCopy( pplot_specs->plot_instance->print_target, filename,
               sizeof pplot_specs->plot_instance->print_target );
          file_error = ( msimNO_ERROR != ( rc = msimWritePlotDataToFile( pplot_specs,
                         pplot_specs->plot_instance, this ) ) );

          break;

     case msimPLOTFILE_HPGL :
     {
          msimStringCopy( pplot_specs->plot_instance->plot_target, filename,
               sizeof pplot_specs->plot_instance->plot_target );

          HPGLPlot aHPGLPlot( this, pplot_specs, filename );

          if ( aHPGLPlot.FileOperationOK( ) )
          {
               aHPGLPlot.xShow( );
               if ( file_error = ( ! aHPGLPlot.FileOperationOK( ) ) )
                    rc = msimWRITE_ERROR;

          }
          else
          {
               file_error = TRUE;
               rc = msimFILE_OPEN_ERROR;
          }



          break;
     }

     case msimPLOTFILE_PS :
     case msimPLOTFILE_EPS :
     {
          msimStringCopy( pplot_specs->plot_instance->plot_target, filename,
               sizeof pplot_specs->plot_instance->plot_target );
          pplot_specs->encapsulated_ps = ( filetype == msimPLOTFILE_EPS );

          PSPlot aPSPlot( this, pplot_specs, filename );

          if ( aPSPlot.FileOperationOK( ) )
               aPSPlot.xShow( );

          file_error = ( ! aPSPlot.FileOperationOK( ) );

          break;
     }

     }                                 // end switch

     aMainApp.Wait( FALSE );

     if ( file_error )
          msimFileError( filename, this, rc );
}

void PlotSaveDialog::PlotFileCancelBtnHandler( PushButton PTR )
{
     EndDialog( );
}

/* contructors follow                                                       */

PlotLimitsDialog::PlotLimitsDialog( msimPlotFrameWindow PTR pParent, PPLOT_SPECS PlotSpecs )
: ModalDialog ( pParent, ResId ( msimPLOTLIM_DIALOG ) ),
aPlotLimTimeLbl ( this, ResId ( msimPLOTLIM_TIME_LBL ) ),
aPlotLimTempXLbl ( this, ResId ( msimPLOTLIM_TEMP_X_LBL ) ),
aPlotLimMinTimeSLE ( this, ResId ( msimPLOTLIM_MINTIME ) ),
aPlotLimMaxTimeSLE ( this, ResId ( msimPLOTLIM_MAXTIME ) ),
aPlotLimTimeIncrSLE ( this, ResId ( msimPLOTLIM_TIME_INCR ) ),
aPlotLimTimeDecPlLB ( this, ResId ( msimPLOTLIM_TIME_DECPL ) ),
aPlotLimMinTempXSLE ( this, ResId ( msimPLOTLIM_MINTEMP_X ) ),
aPlotLimMaxTempXSLE ( this, ResId ( msimPLOTLIM_MAXTEMP_X ) ),
aPlotLimTempXIncrSLE ( this, ResId ( msimPLOTLIM_TEMP_X_INCR ) ),
aPlotLimTempXDecPlLB ( this, ResId ( msimPLOTLIM_TEMP_X_DECPL ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aPlotLimConcLbl ( this, ResId ( msimPLOTLIM_CONC_LBL ) ),
aPlotLimTempYLbl ( this, ResId ( msimPLOTLIM_TEMP_Y_LBL ) ),
aPlotLimPressLbl ( this, ResId ( msimPLOTLIM_PRESS_LBL ) ),
aPlotLimVolLbl ( this, ResId ( msimPLOTLIM_VOL_LBL ) ),
aPlotLimMinConcSLE ( this, ResId ( msimPLOTLIM_MINCONC ) ),
aPlotLimMaxConcSLE ( this, ResId ( msimPLOTLIM_MAXCONC ) ),
aPlotLimConcIncrSLE ( this, ResId ( msimPLOTLIM_CONC_INCR ) ),
aPlotLimConcDecPlLB ( this, ResId ( msimPLOTLIM_CONC_DECPL ) ),
aPlotLimMinTempYSLE ( this, ResId ( msimPLOTLIM_MINTEMP_Y ) ),
aPlotLimMaxTempYSLE ( this, ResId ( msimPLOTLIM_MAXTEMP_Y ) ),
aPlotLimTempYIncrSLE ( this, ResId ( msimPLOTLIM_TEMP_Y_INCR ) ),
aPlotLimTempYDecPlLB ( this, ResId ( msimPLOTLIM_TEMP_Y_DECPL ) ),
aPlotLimMinPressSLE ( this, ResId ( msimPLOTLIM_MINPRESS ) ),
aPlotLimMaxPressSLE ( this, ResId ( msimPLOTLIM_MAXPRESS ) ),
aPlotLimPressIncrSLE ( this, ResId ( msimPLOTLIM_PRESS_INCR ) ),
aPlotLimPressDecPlLB ( this, ResId ( msimPLOTLIM_PRESS_DECPL ) ),
aPlotLimMinVolSLE ( this, ResId ( msimPLOTLIM_MINVOL ) ),
aPlotLimMaxVolSLE ( this, ResId ( msimPLOTLIM_MAXVOL ) ),
aPlotLimVolIncrSLE ( this, ResId ( msimPLOTLIM_VOLUME_INCR ) ),
aPlotLimVolDecPlLB ( this, ResId ( msimPLOTLIM_VOL_DECPL ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aFixedText4 ( this, ResId ( 4 ) ),
aFixedText5 ( this, ResId ( 5 ) ),
aFixedText6 ( this, ResId ( 6 ) ),
aFixedText7 ( this, ResId ( 7 ) ),
aFixedText8 ( this, ResId ( 8 ) ),
aPlotLimClosePB ( this, ResId ( msimPLOTLIM_CLOSE ) ),
aPlotLimUndoPB ( this, ResId ( msimPLOTLIM_UNDO ) ),
aPlotLimDefaultsPB ( this, ResId ( msimPLOTLIM_DEFAULTS ) ),
aPlotLimPreviousPB ( this, ResId ( msimPLOTLIM_PREVIOUS ) ),
aPlotLimHelpPB ( this, ResId ( msimPLOTLIM_HELP ) )
{
     FreeResource( );

     owner = pParent;
     pplot_specs = PlotSpecs;
     volume_option = pplot_specs->plot_instance->volume_option;

#if defined(__MSDOS__) || defined(__OS2__)
     owner->Owner->Disable( );
#endif

     aPlotLimConcLbl.SetText( String( volume_option == msimVAR_VOL ? "Amount :" : "Concentration :" ) );

     aPlotLimClosePB.ChangeClickHdl( LINK( this, PlotLimitsDialog,
               PlotLimCloseBtnHandler ) );
     aPlotLimUndoPB.ChangeClickHdl( LINK( this, PlotLimitsDialog,
               PlotLimUndoHandler ) );
     aPlotLimDefaultsPB.ChangeClickHdl( LINK( this, PlotLimitsDialog,
               PlotLimDefaultsHandler ) );
     aPlotLimPreviousPB.ChangeClickHdl( LINK( this, PlotLimitsDialog,
               PlotLimPreviousHandler ) );

     InitializeDialog( pplot_specs );
}


void PlotLimitsDialog::InitializeDialog( PPLOT_SPECS pPlotSpecs )
{
     msimREAL_STRING min_str, max_str;
     msimREAL_STRING interval_str;
     msimBOOL test;

     /* first the time */
     if ( pPlotSpecs -> plot_time_x )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->time_axis.axismin,
               pPlotSpecs->time_axis.mult,
               pPlotSpecs->time_axis.num_digits,
               pPlotSpecs->time_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->time_axis.axismax,
               pPlotSpecs->time_axis.mult,
               pPlotSpecs->time_axis.num_digits,
               pPlotSpecs->time_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->time_axis.spacing,
               pPlotSpecs->time_axis.mult,
               pPlotSpecs->time_axis.num_digits,
               pPlotSpecs->time_axis.num_dec_places );

          aPlotLimMinTimeSLE.SetText( String( min_str ) );
          aPlotLimMaxTimeSLE.SetText( String( max_str ) );
          aPlotLimTimeIncrSLE.SetText( String( interval_str ) );
          aPlotLimTimeDecPlLB.SelectEntry( String( pPlotSpecs->time_axis.num_dec_places ) );

          aPlotLimMinTimeSLE.Enable( );
          aPlotLimMaxTimeSLE.Enable( );
          aPlotLimTimeIncrSLE.Enable( );
          aPlotLimTimeDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinTimeSLE.Disable( );
          aPlotLimMaxTimeSLE.Disable( );
          aPlotLimTimeIncrSLE.Disable( );
          aPlotLimTimeDecPlLB.Disable( );
     }
     /* then the temp if it is being used as an x axis (independent variable) */

     /* temp_x label */

     if ( pPlotSpecs -> plot_temp_x )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->temp_x_axis.axismin,
               pPlotSpecs->temp_x_axis.mult,
               pPlotSpecs->temp_x_axis.num_digits,
               pPlotSpecs->temp_x_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->temp_x_axis.axismax,
               pPlotSpecs->temp_x_axis.mult,
               pPlotSpecs->temp_x_axis.num_digits,
               pPlotSpecs->temp_x_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->temp_x_axis.spacing,
               pPlotSpecs->temp_x_axis.mult,
               pPlotSpecs->temp_x_axis.num_digits,
               pPlotSpecs->temp_x_axis.num_dec_places );

          aPlotLimMinTempXSLE.SetText( String( min_str ) );
          aPlotLimMaxTempXSLE.SetText( String( max_str ) );
          aPlotLimTempXIncrSLE.SetText( String( interval_str ) );
          aPlotLimTempXDecPlLB.SelectEntry( String( pPlotSpecs->temp_x_axis.num_dec_places ) );

          aPlotLimMinTempXSLE.Enable( );
          aPlotLimMaxTempXSLE.Enable( );
          aPlotLimTempXIncrSLE.Enable( );
          aPlotLimTempXDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinTempXSLE.Disable( );
          aPlotLimMaxTempXSLE.Disable( );
          aPlotLimTempXIncrSLE.Disable( );
          aPlotLimTempXDecPlLB.Disable( );
     }
     /* do the same for conc data */
     test = ( msimBOOL ) ( pPlotSpecs -> plotconc_v_time || pPlotSpecs -> plotconc_v_temp ||
          ( pPlotSpecs -> plotexpdata &&
               ( pPlotSpecs -> expdata_y_type == ( USHORT ) CONC_DATA ) ) );


     if ( test )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->conc_axis.axismin,
               pPlotSpecs->conc_axis.mult,
               pPlotSpecs->conc_axis.num_digits,
               pPlotSpecs->conc_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->conc_axis.axismax,
               pPlotSpecs->conc_axis.mult,
               pPlotSpecs->conc_axis.num_digits,
               pPlotSpecs->conc_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->conc_axis.spacing,
               pPlotSpecs->conc_axis.mult,
               pPlotSpecs->conc_axis.num_digits,
               pPlotSpecs->conc_axis.num_dec_places );

          aPlotLimMinConcSLE.SetText( String( min_str ) );
          aPlotLimMaxConcSLE.SetText( String( max_str ) );
          aPlotLimConcIncrSLE.SetText( String( interval_str ) );
          aPlotLimConcDecPlLB.SelectEntry( String( pPlotSpecs->conc_axis.num_dec_places ) );

          aPlotLimMinConcSLE.Enable( );
          aPlotLimMaxConcSLE.Enable( );
          aPlotLimConcIncrSLE.Enable( );
          aPlotLimConcDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinConcSLE.Disable( );
          aPlotLimMaxConcSLE.Disable( );
          aPlotLimConcIncrSLE.Disable( );
          aPlotLimConcDecPlLB.Disable( );
     }
     /* do the same for press data */

     test = ( msimBOOL ) ( pPlotSpecs -> plotpress_v_time || pPlotSpecs -> plotpress_v_temp ||
          ( pPlotSpecs -> plotexpdata &&
               ( pPlotSpecs -> expdata_y_type == ( USHORT ) PRESS_DATA ) ) );
     if ( test )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->press_axis.axismin,
               pPlotSpecs->press_axis.mult,
               pPlotSpecs->press_axis.num_digits,
               pPlotSpecs->press_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->press_axis.axismax,
               pPlotSpecs->press_axis.mult,
               pPlotSpecs->press_axis.num_digits,
               pPlotSpecs->press_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->press_axis.spacing,
               pPlotSpecs->press_axis.mult,
               pPlotSpecs->press_axis.num_digits,
               pPlotSpecs->press_axis.num_dec_places );

          aPlotLimMinPressSLE.SetText( String( min_str ) );
          aPlotLimMaxPressSLE.SetText( String( max_str ) );
          aPlotLimPressIncrSLE.SetText( String( interval_str ) );
          aPlotLimPressDecPlLB.SelectEntry( String( pPlotSpecs->press_axis.num_dec_places ) );

          aPlotLimMinPressSLE.Enable( );
          aPlotLimMaxPressSLE.Enable( );
          aPlotLimPressIncrSLE.Enable( );
          aPlotLimPressDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinPressSLE.Disable( );
          aPlotLimMaxPressSLE.Disable( );
          aPlotLimPressIncrSLE.Disable( );
          aPlotLimPressDecPlLB.Disable( );
     }


     /* do the same for temperature data */

     test = ( msimBOOL ) ( pPlotSpecs -> plottemp_v_time ||
          ( pPlotSpecs -> plotexpdata &&
               ( pPlotSpecs -> expdata_y_type == ( USHORT ) TEMP_DATA_Y ) ) );

     if ( test )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->temp_y_axis.axismin,
               pPlotSpecs->temp_y_axis.mult,
               pPlotSpecs->temp_y_axis.num_digits,
               pPlotSpecs->temp_y_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->temp_y_axis.axismax,
               pPlotSpecs->temp_y_axis.mult,
               pPlotSpecs->temp_y_axis.num_digits,
               pPlotSpecs->temp_y_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->temp_y_axis.spacing,
               pPlotSpecs->temp_y_axis.mult,
               pPlotSpecs->temp_y_axis.num_digits,
               pPlotSpecs->temp_y_axis.num_dec_places );

          aPlotLimMinTempYSLE.SetText( String( min_str ) );
          aPlotLimMaxTempYSLE.SetText( String( max_str ) );
          aPlotLimTempYIncrSLE.SetText( String( interval_str ) );
          aPlotLimTempYDecPlLB.SelectEntry( String( pPlotSpecs->temp_y_axis.num_dec_places ) );

          aPlotLimMinTempYSLE.Enable( );
          aPlotLimMaxTempYSLE.Enable( );
          aPlotLimTempYIncrSLE.Enable( );
          aPlotLimTempYDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinTempYSLE.Disable( );
          aPlotLimMaxTempYSLE.Disable( );
          aPlotLimTempYIncrSLE.Disable( );
          aPlotLimTempYDecPlLB.Disable( );
     }


     /* do the same for volume data */

     test = ( msimBOOL ) ( pPlotSpecs -> plotvol_v_time || pPlotSpecs -> plotvol_v_temp ||
          ( pPlotSpecs -> plotexpdata &&
               ( pPlotSpecs -> expdata_y_type == ( USHORT ) VOL_DATA ) ) );

     if ( test )
     {
          msimConstructNumericLimitString( min_str,
               pPlotSpecs->volume_axis.axismin,
               pPlotSpecs->volume_axis.mult,
               pPlotSpecs->volume_axis.num_digits,
               pPlotSpecs->volume_axis.num_dec_places );

          msimConstructNumericLimitString( max_str,
               pPlotSpecs->volume_axis.axismax,
               pPlotSpecs->volume_axis.mult,
               pPlotSpecs->volume_axis.num_digits,
               pPlotSpecs->volume_axis.num_dec_places );

          msimConstructNumericLimitString( interval_str,
               pPlotSpecs->volume_axis.spacing,
               pPlotSpecs->volume_axis.mult,
               pPlotSpecs->volume_axis.num_digits,
               pPlotSpecs->volume_axis.num_dec_places );

          aPlotLimMinVolSLE.SetText( String( min_str ) );
          aPlotLimMaxVolSLE.SetText( String( max_str ) );
          aPlotLimVolIncrSLE.SetText( String( interval_str ) );
          aPlotLimVolDecPlLB.SelectEntry( String( pPlotSpecs->volume_axis.num_dec_places ) );

          aPlotLimMinVolSLE.Enable( );
          aPlotLimMaxVolSLE.Enable( );
          aPlotLimVolIncrSLE.Enable( );
          aPlotLimVolDecPlLB.Enable( );
     }
     else
     {
          aPlotLimMinVolSLE.Disable( );
          aPlotLimMaxVolSLE.Disable( );
          aPlotLimVolIncrSLE.Disable( );
          aPlotLimVolDecPlLB.Disable( );
     }


     /* activate/deacticate "Previous" button depending on availability of */
     /* retained data */

     PrevPlotLimAvail ? aPlotLimPreviousPB.Enable( ) : aPlotLimPreviousPB.Disable ( );

#if defined(__MAC__)

     aGroupBox1.Enable( );
     aGroupBox2.Enable( );

#endif

     return;
}


/*--------------------------------------------------------------------------*/
/*                     ReadPlotLimitsDialog()                               */
/*..........................................................................*/
/*                                                                          */
/* Function for reading the plot limits dialog and validating the data      */
/* after retrieving it all.                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
msimRC PlotLimitsDialog::ReadDialog ( )
{
     USHORT i, j, k;
     msimREAL_STRING limits_str[2 * NUM_LIMITS];/* stor all str data here     */
     msimFLOAT limits_vals[3 * (NUM_LIMITS / 2 )];/* stor num limits
                                                     data here                */
     SHORT decplace_vals[NUM_LIMITS / 2];
     msimBOOL valid;

     msimBOOL check_conc, check_temp_x, check_temp_y, check_press,
              check_volume, check_time;

     /* first we retrieve all the numeric data in string                    */
     /* format from the dialog entryfields                                  */
     /* first the time                                                      */

     i = 0;

     if ( pplot_specs->plot_time_x )
     {
          msimStringCopy( limits_str[i], aPlotLimMinTimeSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxTimeSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTimeIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTimeDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_time = TRUE;
     }
     else
          check_time = FALSE;

     if ( pplot_specs->plot_temp_x )
     {
          msimStringCopy( limits_str[i], aPlotLimMinTempXSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxTempXSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTempXIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTempXDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_temp_x = TRUE;
     }
     else
          check_temp_x = FALSE;

     /* do the same for conc data                                           */

     if ( pplot_specs->plotconc_v_time || pplot_specs->plotconc_v_temp ||
               ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type == CONC_DATA ) ) )
     {
          msimStringCopy( limits_str[i], aPlotLimMinConcSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxConcSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimConcIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimConcDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_conc = TRUE;
     }
     else
          check_conc = FALSE;

     /* do the same for press data                                          */

     if ( pplot_specs->plotpress_v_time || pplot_specs->plotpress_v_temp ||
               ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type == PRESS_DATA ) ) )
     {
          msimStringCopy( limits_str[i], aPlotLimMinPressSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxPressSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimPressIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimPressDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_press = TRUE;
     }
     else
          check_press = FALSE;

     /* do the same for temperature data                                    */

     if ( pplot_specs->plottemp_v_time || ( pplot_specs->plotexpdata &&
                    ( pplot_specs->expdata_y_type == TEMP_DATA_Y ) ) )
     {
          msimStringCopy( limits_str[i], aPlotLimMinTempYSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxTempYSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTempYIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimTempYDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_temp_y = TRUE;
     }
     else
          check_temp_y = FALSE;

     /* do the same for volume data                                         */

     if ( pplot_specs->plotvol_v_time || pplot_specs->plotvol_v_temp ||
               ( pplot_specs->plotexpdata && ( pplot_specs->expdata_y_type == VOL_DATA ) ) )
     {
          msimStringCopy( limits_str[i], aPlotLimMinVolSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimMaxVolSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimVolIncrSLE.GetText( ), sizeof limits_str[0] );
          i++;

          msimStringCopy( limits_str[i], aPlotLimVolDecPlLB.GetSelectEntry( ), sizeof limits_str[0] );
          i++;
          check_volume = TRUE;
     }
     else
          check_volume = FALSE;

     /* now work through all the string data, check for invalid             */
     /* input, and store in the limits_vals and decplac_vals arrays         */
     /* here we work through the data in exactly the same order as in       */
     /* reading the entryfields so we know which array member is which      */

     valid = TRUE;
     i = j = k = 0;
     if ( check_time )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
               & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               TIME_DATA );
          i += 4;
          j += 3;
          k++;
     }

     if ( check_temp_x )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
              & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               TEMP_DATA );

          i += 4;
          j += 3;
          k++;
     }

     /* do the same for conc data                                           */

     if ( check_conc )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
               & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               volume_option == msimVAR_VOL ?
                    AMOUNT_DATA : CONC_DATA );

          i += 4;
          j += 3;
          k++;
     }
     if ( check_press )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
               & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               PRESS_DATA );

          i += 4;
          j += 3;
          k++;
     }
     if ( check_temp_y )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
               & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               TEMP_DATA_Y );

          i += 4;
          j += 3;
          k++;
     }
     if ( check_volume )
     {
          valid = valid && ValidateLimitsData(
#if defined(__MAC__)
#if defined(__PPC__)
#else
             (msimREAL_STRING[])
#endif			 
#endif
               & ( limits_str[i] ),
               & ( limits_vals[j] ),
               & ( decplace_vals[k] ),
               VOL_DATA );

     }
     if ( ! valid )
          return msimENTRYFIELD_ERROR;

     /* if we got this far the input data is ok - fill in the pplot_specs struct*/

     j = k = 0;
     if ( check_time )
     {
          FillInAxisStruct( &( pplot_specs->time_axis ), limits_vals + j,
               decplace_vals[k] );
          j += 3;
          k++;
     }
     if ( check_temp_x )
     {
          FillInAxisStruct( &( pplot_specs->temp_x_axis ), limits_vals + j,
               decplace_vals[k] );
          j += 3;
          k++;
     }
     if ( check_conc )
     {
          FillInAxisStruct( &( pplot_specs->conc_axis ), limits_vals + j,
               decplace_vals[k] );
          j += 3;
          k++;
     }
     if ( check_press )
     {
          FillInAxisStruct( &( pplot_specs->press_axis ), limits_vals + j,
               decplace_vals[k] );
          j += 3;
          k++;
     }
     if ( check_temp_y )
     {
          FillInAxisStruct( &( pplot_specs->temp_y_axis ), limits_vals + j,
               decplace_vals[k] );
          j += 3;
          k++;
     }
     if ( check_volume )
     {
          FillInAxisStruct( &( pplot_specs->volume_axis ), limits_vals + j,
               decplace_vals[k] );
     }
     return msimNO_ERROR;
}


void PlotLimitsDialog::PlotLimCloseBtnHandler( PushButton PTR )
{
     if ( msimNO_ERROR == ReadDialog( ) )
     {

          /* save current plot limits for possibility of later recall by user */

          time_axis = pplot_specs->time_axis;
          conc_axis = pplot_specs->conc_axis;
          temp_y_axis = pplot_specs->temp_y_axis;
          temp_x_axis = pplot_specs->temp_x_axis;
          press_axis = pplot_specs->press_axis;
          volume_axis = pplot_specs->volume_axis;

          PrevPlotLimAvail = TRUE;

          /* destroy dialog and redraw plot to update with new limits */
          owner->PlotInvalidate( );

          /* enable main plot dialog again */
          owner->Owner->Enable( );

          EndDialog( );
     }
}

void PlotLimitsDialog::PlotLimUndoHandler( PushButton PTR )
{
     InitializeDialog( pplot_specs );
}



void PlotLimitsDialog::PlotLimDefaultsHandler( PushButton PTR )
{
     /* make a temp copy of plotSpecs */
     /* so we can reset entry fields w/o */
     /* modifying original data */

     temp_plot_data = *pplot_specs;

     /* use a scaling factor of0.0 since we  */
     /* alreadyscaled the data once when we loaded */
     /* it from file  */

     msimSetPlotAndAxisLimits( &temp_plot_data, 0.0 );

     InitializeDialog( &temp_plot_data );
}

void PlotLimitsDialog::PlotLimPreviousHandler( PushButton PTR )
{
     if ( PrevPlotLimAvail )
     {
          /* make a copy of current PlotSpecs */

          temp_plot_data = *pplot_specs;

          /* alter the axis structs so that they */
          /* contain data from previous invocation */

          temp_plot_data.time_axis = time_axis;
          temp_plot_data.conc_axis = conc_axis;
          temp_plot_data.temp_y_axis = temp_y_axis;
          temp_plot_data.temp_x_axis = temp_x_axis;
          temp_plot_data.press_axis = press_axis;
          temp_plot_data.volume_axis = volume_axis;

          /* update the Dialog */
          InitializeDialog( &temp_plot_data );
     }
}


PCHAR PlotLimitsDialog::DataTypeString( USHORT DataType )
{
     static PCHAR str[] =
             {
                  "Time", "Temperature", "Concentration", "Pressure", "Volume", "Amount",
                  "Unknown"
             }
     ;
     switch ( DataType )
     {
     case TIME_DATA :
          return str[0];
     case TEMP_DATA :
     case TEMP_DATA_Y :
          return str[1];
     case CONC_DATA :
          return str[2];
     case PRESS_DATA :
          return str[3];
     case VOL_DATA :
          return str[4];

     case AMOUNT_DATA :
          return str[5];
     default :
          return str[6];
     }
}


PCHAR PlotLimitsDialog::AxisDirectionString( USHORT DataType )
{
     static PCHAR str[] =
             {
                  "Horizontal", "Vertical"
             };

     return ( DataType == TIME_DATA || DataType == TEMP_DATA ) ? str[0] : str[1];
}

void PlotLimitsDialog::AxisEntryFieldErrorMsg( USHORT Position, USHORT DataType,
          PCHAR Value )
{
     PCHAR label[] =
      {
           "Minimum Value", "Maximum Value", "Tic Mark Interval",
           "Number of Dec. Places"
      };

     String message;

     message = "The value [ " + String( Value ) + "] entered as the " + label[Position
     ] + " for the " + AxisDirectionString( DataType ) + " " + DataTypeString( DataType )
          + " axis is out-of-range or does not represent a numeric value.";

     WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );
     return;
}

/*--------------------------------------------------------------------------*/
/*                 ValidateLimitsData( )                                    */
/*..........................................................................*/
/*                                                                          */
/* This routine does several actions:                                       */
/*                                                                          */
/* 1. checks that the min, max, interval and num_dec_places inputs convert  */
/*    to valid numbers and are within the coreect range of values. The      */
/*    special requirements for ranges are :                                 */
/*    a. Min ( = Limits[0]) must be greater than or equal to zero           */
/*    b. Max ( = Limits[1]) must be greater than Min                        */
/*    c. interval (Limits[2]) must be greater than 0.0 and less than        */
/*       (MAx-Min)                                                          */
/*    d. (Max-Min)/interval must be less than or equal to MAXTICKS          */
/*    e. num_dec_places must be >= 0 and <= MAX_DEC_PLACES                  */
/*                                                                          */
/* 2. The functions stores the numeric data in the array Limits and in the  */
/*    location DecPlaces after validation                                   */
/*                                                                          */
/* 3. The function displays a dialog box if invalid data is detected        */
/*                                                                          */
/* 4. The function returns TRUE if all data is valid, FALSE otherwise       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL PlotLimitsDialog::ValidateLimitsData( msimREAL_STRING Source[], msimFLOAT Limits[], PSHORT
              DecPlaces, USHORT DataType )
{

     /* first check conversion of min, max, interval                        */

     USHORT i;

     for ( i = 0; i < 3; i++ )
     {
          if ( ! msimValidNonNegativeFloat( Source[i], &Limits[i] ) )
          {
               AxisEntryFieldErrorMsg( i, DataType, Source[i] );
               return FALSE;
          }

     }

     /* convert number of decimal places string into numeric form  */

     *DecPlaces = (SHORT) ( atoi( Source[3] ) );

     /* at this point all the data has been confirmed to be in valid        */
     /* numeric format. Now let's check more specific relations between     */
     /* these numbers                                                       */
     /* first, is max less than or equal to min? if so it's an error        */

     if ( Limits[1] <= Limits[0] )
     {
          String message;

          message = "The Maximum Value for the " + String( AxisDirectionString
               ( DataType ) ) + " " + String( DataTypeString( DataType ) ) +
          " is less than or equal "
          "to the Minimum Value. Adjust the entries.";

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );
          return FALSE;
     }

     /* now make sure that the interval is greater than 0.0                 */
     /* and that it is less than Max-Min)                                   */

     if ( Limits[2] <= 0.0 || Limits[2] > ( Limits[1] - Limits[0] ) )
     {
          String message;

          message = "The Tic Mark Interval for the " + String
          ( AxisDirectionString( DataType ) ) + " " + String( DataTypeString( DataType )
          ) + " axis must be greater than zero " + "and cannot be greater than "
          "(Max value-Min Value). Change the current entry.";

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );
          return FALSE;
     }

     /* now check whether user has requested too many tic marks             */
     /* if( (Max - Min) /interval )>  MAXTICKS ==> error                     */

     if ( ( USHORT ) ( ( ( Limits[1] - Limits[0] ) / Limits[2] ) + EPSILON + 1.0 ) > ( USHORT )
               MAXTICKS )
     {
          String message;

          message = String( "The Tic Mark Interval for the " ) + String
          ( AxisDirectionString( DataType ) ) + String( " " ) + String( DataTypeString( DataType )
          ) + String( " axis is too small. A maximum of " ) + String( MAXTICKS_STR ) +
          String( " tic marks can be drawn. Correct the entries " );

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );
          return FALSE;
     }

     /* if we made it here everything is OK                                  */

     return TRUE;
}


void PlotLimitsDialog::FillInAxisStruct( msimPAXIS pAxis, msimPFLOAT LimitsVals, USHORT
          NumDecplaces )
{
     msimFLOAT r;

     /* first calculate the multiplier from axis_max value                  */
     /* entered by the user                                                 */

     pAxis->mult = msimCalcMultiplierFactor( *( LimitsVals + 1 ) );

     /* fill in several fields using mult                                   */

     pAxis->axismin = *LimitsVals++ / pAxis->mult;
     pAxis->axismax = *LimitsVals++ / pAxis->mult;
     pAxis->spacing = *LimitsVals++ / pAxis->mult;

     pAxis->axisrange = pAxis->axismax - pAxis->axismin;

     /* calc num_ticks - simple dividing axisrange by spacing as in
        pAxis->num_ticks = (USHORT) floor( pAxis->axisrange / pAxis->spacing)+1;
        does not seem go give correct answer for 2.000/1.000 ??               */
     /* EPSILON added to ensure that roundoff error in flt pt calc           */
     /* does not cause wrong result                                          */

     r = ( pAxis->axisrange / pAxis->spacing ) + EPSILON;
     pAxis->num_ticks = ( ( USHORT ) r ) + 1;

     /* determine the numeric format from the data at hand.                 */
     /* this sets the num+digits and tic_label_format fields                */
     /* Note that we are going to override the value returned for           */
     /* num_dec_places                                                      */

     msimCalcNumFmt( pAxis->tic_label_format, & ( pAxis->num_digits ),
          & ( pAxis->num_dec_places ), pAxis->axismax, pAxis->spacing,
          sizeof pAxis->tic_label_format );

     /* adjust for num_dec_places contribution to num_digits before         */
     /* we lose the value                                                   */

     pAxis->num_digits -= pAxis->num_dec_places;

     /* do the over-ride with user-selected num of dec places               */

     pAxis->num_dec_places = NumDecplaces;

     /* and adjust num_digits again                                         */

     pAxis->num_digits += pAxis->num_dec_places;
     return;
}


msimBOOL PlotLimitsDialog::PrevPlotLimAvail = FALSE;
msimAXIS PlotLimitsDialog::time_axis;
msimAXIS PlotLimitsDialog::conc_axis;
msimAXIS PlotLimitsDialog::temp_y_axis;
msimAXIS PlotLimitsDialog::temp_x_axis;
msimAXIS PlotLimitsDialog::press_axis;
msimAXIS PlotLimitsDialog::volume_axis;


