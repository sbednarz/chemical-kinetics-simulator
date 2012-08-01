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
/*  FILE NAME : msimpltd.cxx                                                */
/*                                                                          */
/*  This module  contains the functions for creating and processing the     */
/*  plot setup dialog which is invoked when the user selects ShowResults    */
/*  from the MSIM Main menu                                                 */
/*                                                                          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started August  22 1993                                    */
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
#include <stdlib.h>
#include <limits.h>

#define MAX_PTS_DEFAULT             1000
#define MAX_NO_RECORDS    ( (unsigned int) ( ( msimSIZE_T_MAX / sizeof( msimFLOAT) ) - (msimEXTRA_PLOT_ARRAY_PTS + 1 ) ) )

static msimRC LoadExternalDataFile( PCHAR Filename, PPLOT_SPECS PlotSpecs, msimWID
                   Owner );



/* initializer for PLOT_SPECS struct                                        */

extern const PLOT_SPECS C_PLOT_SPECS =
                        {
                             NULL,     /* plot_instance                       */
                             ( USHORT ) 0,/* USHORT num_species_to_plot,      */
                             {
                                  ( USHORT ) 0, ( USHORT ) 0, ( USHORT ) 0, ( USHORT ) 0, ( USHORT ) 0, ( USHORT ) 0, (
                                       USHORT )
                                  0
                             }
                             ,         /* plotspeciesindex[];                 */
                             ( USHORT ) 0,/* USHORT num_plots,                */
                             0UL,      /* num_data_sets;                      */
                             0UL,      /* num_exp_data_sets;                  */
                             1,        /* USHORT sim_data_interval;           */
                             1,        /* USHORT exp_data_interval;           */
                             DEFAULT_MAX_PLOT_PTS,/* ULONG max_pts_to_plot;   */
                             ( msimBOOL ) FALSE,/* msimBOOL exp_data_ok,      */
                             ( msimBOOL ) FALSE,/* msimBOOL plotconc_v_time,  */
                             ( msimBOOL ) FALSE,/* plottemp_v_time;           */
                             ( msimBOOL ) FALSE,/* msimBOOL plotpress_v_time, */
                             ( msimBOOL ) FALSE,/* plotvol_v_time;            */
                             ( msimBOOL ) FALSE,/* msimBOOL plotconc_v_temp,  */
                             ( msimBOOL ) FALSE,/* msimBOOL plotpress_v_temp, */
                             ( msimBOOL ) FALSE,/* plotvol_v_temp;            */
                             ( msimBOOL ) FALSE,/* plotexpdata                */
                             {
                                  msimNULL_STR,/* msimNAME_STRING plotlabel[];*/
                                  msimNULL_STR, msimNULL_STR, msimNULL_STR, msimNULL_STR,
                                  msimNULL_STR, msimNULL_STR
                             }
                             , NULL,   /* msimFLOAT *Ptime,                   */
                             NULL,     /* *Ptemp,                             */
                             NULL,     /* *Pvol,                              */
                             NULL,     /* *Ppress,                            */
                             NULL,     /* *Pexpdata_x                         */
                             NULL,     /* *Pexpdata_y                         */
                             {         /* msimFLOAT *Pconc[];                 */
                                  NULL, NULL, NULL, NULL, NULL, NULL, NULL
                             }
                             , ( msimBOOL ) FALSE,/* msimBOOL plot_time_x;    */
                             ( msimBOOL ) FALSE,/* msimBOOL plot_temp_x;      */
                             ( USHORT ) 200,/* USHORT vertical_win_size,      */
                             ( USHORT ) 200,/* horizontal_win_size;           */
                             ( msimBOOL ) TRUE,/* msimBOOL vary_color,        */
                             ( msimBOOL ) FALSE,/* vary_linestyle;            */
                             ( msimBOOL ) TRUE,/* vary_markerstyle            */
                             ( msimBOOL ) TRUE,/* show_marker;                */
                             ( msimBOOL ) TRUE,/* show_line;                  */
                             DEFAULT_FONTFAMILY,/* enum FontFamily typeface;  */
                             DEFAULT_POINTSIZE,/* USHORT pointsize;           */
                             msimGRAY_BKGRD,/* current_colorset               */
                             SCREEN_DEFAULT_BKGRD_COLOR,/* enum ColorName background_color;*/
                             SCREEN_DEFAULT_AXIS_COLOR,/* enum ColorName axis_color;*/
                             {
                                  COL_YELLOW, COL_RED, COL_BLUE, COL_GREEN,/* enum ColorName
                                                    line_color[MAX_NO_PLOTS+1]*/
                                  COL_CYAN, COL_LIGHTRED, COL_WHITE, COL_BLACK
                             }
                             ,
                             SCREEN_DEFAULT_PLOT_COLOR,/* enum ColorName
                                                          default_line_color; */
                             SCREEN_DEFAULT_EXP_DATA_COLOR,/* enum ColorName exp_data_color;*/
                             SCREEN_DEFAULT_PEN_WIDTH,/* USHORT axis_line_width;*/
                             DEFAULT_LINE_TYPE,/* enum PenStyle axis_line_style;*/
                             DEFAULT_LINE_TYPE,/* enym PenStyle default_line_style*/
                             {
                                  PEN_SOLID,/* enum PenStyle
                                               line_style[MAX_NO_PLOTS+1];    */
                                  PEN_DASH, PEN_DOT, PEN_DASHDOT, PEN_DASH, PEN_DOT, PEN_DASHDOT,
                                  PEN_NULL
                             }
                             ,
                             DEFAULT_MARKER_STYLE,/* enum MARKER_STYLE
                                                     default_marker_style     */
                             {
                                  MARKER_CIRCLE,/* enum MARKER_STYLE
                                                 marker_style[MAX_NO_PLOTS+1];*/
                                  MARKER_DIAMOND, MARKER_SQUARE, MARKER_CROSS, MARKER_PLUS,
                                  MARKER_SOLIDSQUARE, MARKER_SOLIDDIAMOND, MARKER_STAR8
                             }
                             ,
                             msimPLOTFILE_TEXT,/* default_output_filetype     */
                             ( USHORT ) NO_DATA_TYPE,/* USHORT expdata_x_type;*/
                             ( USHORT ) NO_DATA_TYPE,/* USHORT expdata_y_type;*/
                             ( msimBOOL ) FALSE,/* msimBOOL encapsulated_ps;  */
                             ( msimBOOL ) TRUE,/* msimBOOL landscape;         */
                             ( msimBOOL ) FALSE,/* msimBOOL show_filename;    */
                             ( msimBOOL ) FALSE,/* msimBOOL use_color_in_plot_file;*/
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /* msimAXIS time_axis;                 */
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /* msimAXIS conc_axis;                 */
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /* msimAXIS temp_y_axis;               */
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /* msimAXIS temp_x_axis;               */
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /* msimAXIS press_axis;                */
                             {
                                  FALSE, FALSE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                  0, 0, 0, msimNULL_STR
                             }
                             ,         /*    msimAXIS volume_axis;            */
                             TRUE,     /*    show_grid                        */
                             FALSE     /*   msimBOOL plot_was_visible_before; */

                        };


// destructor

MainPlotDialog::~MainPlotDialog( )
{
     msimFreePlotArrayMemory( &PlotSpecs );

     if ( PlotSpecs.Pexpdata_x )
          delete[]PlotSpecs.Pexpdata_x;

     if ( PlotSpecs.Pexpdata_y )
          delete[]PlotSpecs.Pexpdata_y;

}


/* constructor for Main Plot Dialog                                         */

MainPlotDialog::MainPlotDialog( Window * pParent, msimPINSTANCE Instance ) :
ModelessDialog ( pParent, ResId ( msimMAIN_PLOT_PANEL )
),
aPlotConcTimeCB ( this, ResId ( msimPLOT_CONCTIME ) ),
aPlotTempTimeCB ( this, ResId ( msimPLOT_TEMPTIME ) ),
aPlotVolTimeCB ( this, ResId ( msimPLOT_VOLTIME ) ),
aPlotPressTimeCB ( this, ResId ( msimPLOT_PRESSTIME ) ),
aPlotConcTempCB ( this, ResId ( msimPLOT_CONCTEMP ) ),
aPlotVolTempCB ( this, ResId ( msimPLOT_VOLTEMP ) ),
aPlotPressTempCB ( this, ResId ( msimPLOT_PRESSTEMP ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aPlotExtrnDataCB ( this, ResId ( msimPLOT_EXTRNDATA ) ),
aLoadExtrnDataPB ( this, ResId ( msimPLOT_LOADXTRN_BTN ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aDefinedSpeciesMLB ( this, ResId ( msimPLOT_DEFSPC_BOX ) ),
aPlotSpeciesGB ( this, ResId ( msimPLOT_SPECIES_LABEL ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aNumPlotsAvailText ( this, ResId ( msimPLOT_NUM_PTS_AVAIL ) ),
aNumSpeciesSelectedText ( this, ResId ( msimPLOT_NUMSPECIES ) ),
aMaxPointsSLE ( this, ResId ( msimPLOT_MAX_POINTS ) ),
aGroupBox4 ( this, ResId ( 4 ) ),
aPlotCancelBtn ( this, ResId ( msimPLOT_CANCEL_BTN ) ),
aPlotPlotBtn ( this, ResId ( msimPLOT_PLOT_BTN ) ),
aPlotHelpBtn ( this, ResId ( msimPLOT_HELP_BTN ) )
{
     FreeResource( );
     Instance->p_plot_dialog = this;
     plot_frame_window = NULL;

     aPlotConcTimeCB.GrabFocus( );
     SetText( String( Instance->base_filename ) + GetText( ) );

     PlotSpecs = C_PLOT_SPECS;
     PlotSpecs.plot_instance = Instance;
     if ( ! msimINVALID_WINDOW_GEOMETRY( msimAppOptions.plot_dialog_position ) )
          msimRestoreCurrentWindowPosition( this, &msimAppOptions.plot_dialog_position );
     else
          /* if we are using default attachment, cascade from parent              */

          msimCenterDialogWindowOnOwner( this, pParent );

     /* get all the saved atribute data                                */

     PlotSpecs.typeface = msimAppOptions.fonttype;
     PlotSpecs.pointsize = msimAppOptions.fontsize;
     PlotSpecs.vary_color = msimAppOptions.vary_colors;
     PlotSpecs.vary_linestyle = msimAppOptions.vary_linestyle;
     PlotSpecs.vary_markerstyle = msimAppOptions.vary_markerstyle;
     PlotSpecs.show_line = msimAppOptions.show_lines;
     PlotSpecs.show_marker = msimAppOptions.show_markers;
     PlotSpecs.show_filename = msimAppOptions.show_filename;
     PlotSpecs.encapsulated_ps = msimAppOptions.encapsulated_ps;
     PlotSpecs.landscape = msimAppOptions.landscape;
     PlotSpecs.use_color_in_plot_file = ! msimAppOptions.plotfile_conv_color;
     PlotSpecs.default_output_filetype = msimAppOptions.default_plotfile;
     PlotSpecs.current_colorset = msimAppOptions.default_colorset;
     PlotSpecs.show_grid = msimAppOptions.show_grid;

     InitializePlotDataDialog( PlotSpecs.plot_instance );
     Show( );

     aPlotConcTimeCB.ChangeClickHdl( LINK( this, MainPlotDialog, ConcBtnHandler )
     );

     aPlotConcTempCB.ChangeClickHdl( LINK( this, MainPlotDialog, ConcBtnHandler )
     );

     aLoadExtrnDataPB.ChangeClickHdl( LINK( this, MainPlotDialog,
               LoadExternDataHandler ) );

     aPlotCancelBtn.ChangeClickHdl( LINK( this, MainPlotDialog,
               PlotCancelHandler ) );

     aPlotPlotBtn.ChangeClickHdl( LINK( this, MainPlotDialog, PlotPlotHandler ) );

     aDefinedSpeciesMLB.ChangeSelectHdl( LINK( this, MainPlotDialog, LBSelectHdl ) );
}

void MainPlotDialog::LBSelectHdl( MultiListBox PTR )
{
     if ( show_species )
          aNumSpeciesSelectedText.SetText( String( " " ) +
               String( aDefinedSpeciesMLB.GetSelectEntryCount( ) ) + String( " Species Selected" )
          );
     else
          aNumSpeciesSelectedText.SetText( String( msimNULL_STR ) );
}


void MainPlotDialog::Hide( )
{
     if ( plot_frame_window )// does this exist ? if so save visibility state and hide it
     {
          PlotSpecs.plot_was_visible_before = plot_frame_window->IsVisible( );
          plot_frame_window->Hide( );
     }

     ModelessDialog::Hide( );
}

void MainPlotDialog::Show( )
{
     if ( ( plot_frame_window )// does this exist ? if so restore previous state
               && ( PlotSpecs.plot_was_visible_before ) )
          plot_frame_window->Show( );

     ModelessDialog::Show( );
}



void MainPlotDialog::InitializePlotDataDialog( msimPINSTANCE
          Instance )
{

     num_species_selected = def_selection = plot_selection = 0;
     show_species = FALSE;
     extern_filename[0] = '\0';

     if ( Instance->num_simulation_data_records > MAX_PTS_DEFAULT )
          aMaxPointsSLE.SetText( String( MAX_PTS_DEFAULT ) );
     else
          aMaxPointsSLE.SetText( String( Instance->num_simulation_data_records ) );

     aNumPlotsAvailText.SetText( String( Instance->num_simulation_data_records ) +
          String( ResId( msimMAIN_PLOT_DLG_STR1 ) ) );

     if ( Instance->temp_option != msimCONST_TEMP )
          aPlotTempTimeCB.Enable( );
     else
          aPlotTempTimeCB.Disable( );

     /* if variable volume simulation then we want to say                   */
     /* Amount /time and AMount /temp on radiobuttons rather than           */
     /* Conc / time and Conc. temp                                          */

     if ( Instance->volume_option == msimVAR_VOL )
     {
          aPlotVolTimeCB.Enable( );
          aPlotConcTempCB.SetText( ResId( msimMAIN_PLOT_DLG_STR2 )  );
          aPlotConcTimeCB.SetText( ResId( msimMAIN_PLOT_DLG_STR3 ) );
     }
     else
     {
          aPlotVolTimeCB.Disable( );
          aPlotConcTempCB.SetText( ResId( msimMAIN_PLOT_DLG_STR4 ) );
          aPlotConcTimeCB.SetText( ResId( msimMAIN_PLOT_DLG_STR5 ) );
     }

     if ( Instance->variablepress )
          aPlotPressTimeCB.Enable( );
     else
          aPlotPressTimeCB.Disable( );

     if ( Instance->temp_option != msimCONST_TEMP )
          aPlotConcTempCB.Enable( );
     else
          aPlotConcTempCB.Disable( );

     if ( ( Instance->volume_option == msimVAR_VOL ) && ( Instance->temp_option !=
                    msimCONST_TEMP ) )
          aPlotVolTempCB.Enable( );
     else
          aPlotVolTempCB.Disable( );

     if ( Instance->variablepress && Instance->temp_option != msimCONST_TEMP )
          aPlotPressTempCB.Enable( );
     else
          aPlotPressTempCB.Disable( );

     return;
}

void MainPlotDialog::PlotCancelHandler( PushButton * )
{
     Hide( );

     if ( plot_frame_window )          // does this exist ? if so delete
     {
          // restore to std size before saving geometry

          // under OS/2 this works fine, Under Windows Restore does not work
#if defined(__OS2__)
          plot_frame_window->Hide( );
          plot_frame_window->Restore( );
#endif

#if defined(__MSDOS__)

          // do not save size and position if window is currently minimized or maximized

          if ( plot_frame_window->IsMaximized( ) || plot_frame_window->IsMinimized( ) )
              /* NULL */  ;    // do nothing
          else
#endif
          msimSaveCurrentWindowGeometry( plot_frame_window,
               &msimAppOptions.plot_window_position );

          delete plot_frame_window;

          plot_frame_window = NULL;
     }

     msimSaveCurrentWindowGeometry( this, &msimAppOptions.plot_dialog_position );

     msimAppOptions.fonttype = PlotSpecs.typeface;
     msimAppOptions.fontsize = PlotSpecs.pointsize;
     msimAppOptions.vary_colors = PlotSpecs.vary_color;
     msimAppOptions.vary_linestyle = PlotSpecs.vary_linestyle;
     msimAppOptions.vary_markerstyle = PlotSpecs.vary_markerstyle;
     msimAppOptions.show_lines = PlotSpecs.show_line;
     msimAppOptions.show_markers = PlotSpecs.show_marker;
     msimAppOptions.show_filename = PlotSpecs.show_filename;
     msimAppOptions.encapsulated_ps = PlotSpecs.encapsulated_ps;
     msimAppOptions.landscape = PlotSpecs.landscape;
     msimAppOptions.plotfile_conv_color = ! PlotSpecs.use_color_in_plot_file;
     msimAppOptions.default_plotfile = PlotSpecs.default_output_filetype;
     msimAppOptions.default_colorset = PlotSpecs.current_colorset;
     msimAppOptions.show_grid = PlotSpecs.show_grid;

     PlotSpecs.plot_instance->p_plot_dialog = NULL;

     delete this;

     return;
}

void MainPlotDialog::PlotPlotHandler( PushButton * )
{
     if ( GetUserSelections( ) != msimNO_ERROR )
          return;

     // if the window already exists free the memory associated with
     // the plot arrays that were used to construct it
     if ( plot_frame_window )
          msimFreePlotArrayMemory( &PlotSpecs );

     if ( ReadAndStoreSimulationData( ) != msimNO_ERROR )
          return;

     if ( plot_frame_window )
     {
          plot_frame_window->aPlotXYWindow.Invalidate( );
          plot_frame_window->Show( );
     }
     else
     {
          plot_frame_window = new msimPlotFrameWindow(this,
          PlotSpecs.plot_instance );

          if ( plot_frame_window == NULL )
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, this );
          else
          {
               plot_frame_window->Resize( );// moved from ctor
               plot_frame_window->Show( );
          }

     }
}

void MainPlotDialog::LoadExternDataHandler( PushButton * )
{
     if ( msimUSER_ABORT == msimGetFileName( this,
                    msimAppOptions.external_file_template, extern_filename, WB_OPEN,
                    "Load External Data File", msimEXTDATFILE_TYPE, sizeof extern_filename ) )
          return;

     if ( msimFileExists( extern_filename ) )
     {
          SetExtFileButtonText( msimBaseFilename( extern_filename ) );
          aPlotExtrnDataCB.Enable( );
     }
     else
     {
          String message;

          message = String( ResId( msimMAIN_PLOT_DLG_STR6 ) ) + String( extern_filename ) +
           String( ResId( msimMAIN_PLOT_DLG_STR7 ) );

          WarningBox box( this, WB_OK | WB_DEF_OK, message );

          box.Execute( );
     }

     return;

}

void MainPlotDialog::ConcBtnHandler( CheckBox * )
{
     msimBOOL test = ( aPlotConcTempCB.IsChecked( ) ||
          aPlotConcTimeCB.IsChecked( ) );

     if ( test != show_species )       /* if state has changed                */
     {
          InitializeSpeciesSelection( test );
          show_species = ( msimBOOL ) ! show_species;
          num_species_selected = 0;
          LBSelectHdl( &aDefinedSpeciesMLB );

#if defined(__MAC__)
          aDefinedSpeciesMLB.Invalidate();
#endif
     }
}


msimRC MainPlotDialog::ReadMaxPointsLB( )
{
     msimINTEGER_STRING str;
     ULONG tempval;

     // read the entryfield and return is an error

     msimStringCopy( str, aMaxPointsSLE.GetText( ), sizeof str );

     if ( ! msimValidPositiveULongInteger( str, &tempval ) ||
               ( tempval < 2 ) ||
               ( tempval > MAX_NO_RECORDS )
          )
     {

          String message = String( ResId( msimMAIN_PLOT_DLG_STR8 ) );

          message += String( str );

          message += String( ResId( msimMAIN_PLOT_DLG_STR9 ) );
          message += String( (ULONG) (MAX_NO_RECORDS) );

          WarningBox box( this, WB_OK | WB_DEF_OK, message );

          box.Execute( );
          return msimINPUT_ERROR;
     }
     else
     {
          sscanf( str, " %u", &PlotSpecs.max_pts_to_plot );
          return msimNO_ERROR;
     }


}


/*--------------------------------------------------------------------------*/
/*                        GetUserSelections( )                              */
/*..........................................................................*/
/*                                                                          */
/* Function for determining what kinds of plots have been requested         */
/* by the user     in the main plot dialog window.                          */
/* Updates the boolean switches and also updates                            */
/* the num_plots field in the PlotData struct                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC MainPlotDialog::GetUserSelections( )
{
     USHORT i = 0;

     if ( msimNO_ERROR != ReadMaxPointsLB( ) )
          return msimINPUT_ERROR;


     if ( PlotSpecs.plottemp_v_time = aPlotTempTimeCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotvol_v_time = aPlotVolTimeCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotpress_v_time = aPlotPressTimeCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotconc_v_time = aPlotConcTimeCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotvol_v_temp = aPlotVolTempCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotpress_v_temp = aPlotPressTempCB.IsChecked( ) )
          i++;

     if ( PlotSpecs.plotconc_v_temp = aPlotConcTempCB.IsChecked( ) )
          i++;

     /* so much for specifying plots of the simulation data                 */
     /* now see if the external experimental data box is checked            */
     /* if so, then figure out whether this will require another            */
     /* plot to be displayed or whether it can be overlaid on an            */
     /* already specified plot                                              */

     if ( PlotSpecs.plotexpdata = aPlotExtrnDataCB.IsChecked( ) )
     {
          msimRC rc;

          if ( msimNO_ERROR != ( rc = LoadExternalDataFile( extern_filename,
                              &PlotSpecs, this ) )
               )
               return rc;


          /* sum of data types gives a unique value depending on            */
          /* the x and y data types                                         */

          switch ( PlotSpecs.expdata_x_type + PlotSpecs.expdata_y_type )
          {
          case ( CONC_DATA + TIME_DATA ) :

               if ( ! PlotSpecs.plotconc_v_time )
                    i++;
               break;

          case ( CONC_DATA + TEMP_DATA ) :
               if ( ! PlotSpecs.plotconc_v_temp )
                    i++;
               break;

          case ( TEMP_DATA_Y + TIME_DATA ) :
               if ( ! PlotSpecs.plottemp_v_time )
                    i++;
               break;

          case ( PRESS_DATA + TIME_DATA ) :
               if ( ! PlotSpecs.plotpress_v_time )
                    i++;
               break;

          case ( PRESS_DATA + TEMP_DATA ) :
               if ( ! PlotSpecs.plotpress_v_temp )
                    i++;
               break;

          case ( VOL_DATA + TIME_DATA ) :
               if ( ! PlotSpecs.plotvol_v_time )
                    i++;
               break;

          case ( VOL_DATA + TEMP_DATA ) :
               if ( ! PlotSpecs.plotvol_v_temp )
                    i++;
               break;

          }
     }

     if ( i > MAX_NO_VIEWPORTS )
     {
          InfoBox( this, ResId( msimTOO_MANY_PLOTS_MSG ) ) .Execute( );
          return msimINPUT_ERROR;
     }

     if ( i == 0 )
     {
          InfoBox( this, ResId( msimNO_PLOTS_SELECTED_MSG ) ) .Execute( );
          return msimINPUT_ERROR;
     }
     else

          PlotSpecs.num_plots = i;

     PlotSpecs.plot_time_x = ( msimBOOL ) ( PlotSpecs.plottemp_v_time ||
          PlotSpecs.plotconc_v_time || PlotSpecs.plotpress_v_time ||
          PlotSpecs.plotvol_v_time || ( PlotSpecs.plotexpdata &&
               ( PlotSpecs.expdata_x_type == ( USHORT ) TIME_DATA ) ) );

     PlotSpecs.plot_temp_x = ( msimBOOL ) ( PlotSpecs.plotconc_v_temp ||
          PlotSpecs.plotpress_v_temp || PlotSpecs.plotvol_v_temp ||
          ( PlotSpecs.plotexpdata && ( PlotSpecs.expdata_x_type == ( USHORT ) TEMP_DATA
               ) ) );

     USHORT j = 0;

     /* now get the selected plot species if necessary                     */

     if ( PlotSpecs.plotconc_v_time || PlotSpecs.plotconc_v_temp )
     {
          j = aDefinedSpeciesMLB.GetSelectEntryCount( );

          if ( j > MAX_NO_OF_PLOTS )
          {
               InfoBox( this, ResId( msimTOO_MANY_SPECIES_MSG ) ) .Execute( );
               return msimINPUT_ERROR;
          }

          if ( j == 0 )
          {
               InfoBox( this, ResId( msimNO_SPECIES_SELECTED_MSG ) ) .Execute( );
               return msimINPUT_ERROR;
          }

          for ( USHORT k = 0; k < j; k++ )
          {
               msimStringCopy( PlotSpecs.plotlabel[k],
                    aDefinedSpeciesMLB.GetSelectEntry( k ), sizeof PlotSpecs.plotlabel[0] );
               PlotSpecs.plotspeciesindex[k] = msimFindSpeciesIndex
               ( PlotSpecs.plotlabel[k], PlotSpecs.plot_instance );
          }

     }

     PlotSpecs.num_species_to_plot = j;


     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        ReadAndStoreSimulationData( )                     */
/*..........................................................................*/
/*                                                                          */
/* Function for constructing a plot of the simulation data                  */
/* on the screen depending on the  options selected by the                  */
/* user in the msimPlotDialogWindow                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC MainPlotDialog::ReadAndStoreSimulationData( )
{
     msimRC rc;
     size_t array_size;
     ULONG total_num_points = PlotSpecs.plot_instance->num_simulation_data_records;
     msimBOOL load_sim_data = FALSE;

     // here we calculate the interval between simulation data points to read in and
     // plot

     if ( total_num_points <= PlotSpecs.max_pts_to_plot )
          PlotSpecs.sim_data_interval = 1;
     else
     {
          PlotSpecs.sim_data_interval =
          total_num_points / PlotSpecs.max_pts_to_plot;

          if ( total_num_points % PlotSpecs.max_pts_to_plot )// is there a remainder
               PlotSpecs.sim_data_interval++;
     }

     array_size = ( total_num_points / PlotSpecs.sim_data_interval ) + 2;

     /* at this point we know all we need to start allocating memory for the */
     /* data arrays                                                         */
     /* first the array for the elapsed time values                         */

     if ( PlotSpecs.plotconc_v_time || PlotSpecs.plottemp_v_time ||
               PlotSpecs.plotpress_v_time || PlotSpecs.plotvol_v_time )
     {
          PlotSpecs.Ptime = new msimFLOAT[array_size];

          if ( ! PlotSpecs.Ptime )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, this );
               return msimMEM_ALLOC_ERROR;
          }

          load_sim_data = TRUE;
     }
     else

          PlotSpecs.Ptime = NULL;

     /* now allocate memory for temperature array if necessary              */

     if ( PlotSpecs.plottemp_v_time || PlotSpecs.plotconc_v_temp ||
               PlotSpecs.plotpress_v_temp || PlotSpecs.plotvol_v_temp )
     {
          PlotSpecs.Ptemp = new msimFLOAT[array_size];

          if ( ! PlotSpecs.Ptemp )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, this );
               return msimMEM_ALLOC_ERROR;
          }

          load_sim_data = TRUE;
     }
     else

          PlotSpecs.Ptemp = NULL;

     /* do we need an array for pressure data ?                             */

     if ( PlotSpecs.plotpress_v_time || PlotSpecs.plotpress_v_temp )
     {
          PlotSpecs.Ppress = new msimFLOAT[array_size];

          if ( ! PlotSpecs.Ppress )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, this );
               return msimMEM_ALLOC_ERROR;
          }

          load_sim_data = TRUE;
     }
     else

          PlotSpecs.Ppress = NULL;

     /* check if we are going to be plotting volume data                    */

     if ( PlotSpecs.plotvol_v_time || PlotSpecs.plotvol_v_temp )
     {
          PlotSpecs.Pvol = new msimFLOAT[array_size];

          if ( ! PlotSpecs.Pvol )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, this );
               return msimMEM_ALLOC_ERROR;
          }

          load_sim_data = TRUE;
     }
     else

          PlotSpecs.Pvol = NULL;

     /* now allocate memory for concentration arrays                        */

     if ( PlotSpecs.plotconc_v_time || PlotSpecs.plotconc_v_temp )
     {
          USHORT i = 0;

          /* allocated one array for each requested species                 */

          while ( i < PlotSpecs.num_species_to_plot )
          {
               PlotSpecs.Pconc[i] = new msimFLOAT[array_size];

               if ( ! PlotSpecs.Pconc[i] )
               {
                    msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                         __TIMESTAMP__, __LINE__, this );
                    return msimMEM_ALLOC_ERROR;
               }
               i++;
          }

          /* and set the remaining pointers to NULL                         */

          while ( i < MAX_NO_OF_PLOTS )
               PlotSpecs.Pconc[i++] = NULL;

          load_sim_data = TRUE;
     }

     if ( load_sim_data )
          if ( msimNO_ERROR != ( rc = ReadSimulationData( ) ) )
               return rc;

     /* if we are plotting external data from a file then */
     /* adjust the plot limits if necessary */

     if ( PlotSpecs.plotexpdata )
     {
          if ( ! load_sim_data )
               PlotSpecs.time_axis.datamin =
               PlotSpecs.time_axis.datamax =
               PlotSpecs.conc_axis.datamin =
               PlotSpecs.conc_axis.datamax =
               PlotSpecs.temp_y_axis.datamin =
               PlotSpecs.temp_y_axis.datamax =
               PlotSpecs.press_axis.datamin =
               PlotSpecs.press_axis.datamax =
               PlotSpecs.volume_axis.datamin =
               PlotSpecs.volume_axis.datamax =
               PlotSpecs.temp_x_axis.datamin =
               PlotSpecs.temp_x_axis.datamax = 0.0;

          msimAdjustLimits( &PlotSpecs );
     }

     msimCheckForInvalidLimits( &PlotSpecs );

     return msimNO_ERROR;
}

void MainPlotDialog::InitializeSpeciesSelection( msimBOOL State )
{

     /* fill the species selection list box                                 */

     if ( State )
     {
          aDefinedSpeciesMLB.Enable( );

          aPlotSpeciesGB.Enable( );

          aDefinedSpeciesMLB.ChangeUpdateMode( FALSE );/* turn off update to
                                                          prevent flicker     */
          aDefinedSpeciesMLB.Clear( );

          /* fill the listbox                                         */

          for ( msimPSPECIES species_ptr =
                    PlotSpecs.plot_instance->ptr_to_species_list; species_ptr != NULL;
                    species_ptr = species_ptr->next )
          {
               aDefinedSpeciesMLB.InsertEntry( String( species_ptr->name ) );
          }

          aDefinedSpeciesMLB.SetNoSelection( );/* de-select any               */
          aDefinedSpeciesMLB.ChangeUpdateMode( TRUE );
          aDefinedSpeciesMLB.Update( );

          /* and set it and associsated group box active                     */

     }
     else
     {

          /* clear and disable stuff it it is not available                   */

          aDefinedSpeciesMLB.Clear( );
          aDefinedSpeciesMLB.Disable( );

          aPlotSpeciesGB.Disable( );
     }

     return;
}

/*--------------------------------------------------------------------------*/
/*                    LoadExternalDataFile()                            */
/*..........................................................................*/
/*                                                                          */
/* Function that loads a test file containing two columns of numeric data   */
/* The  first line must contain two words ( upper or lower of mixed case )  */
/* The first word must be "TIME" or "TEMPERATURE"  and the second word      */
/* must be either "TEMPERATURE", "CONCENTRATION", "PRESSURE" or "VOLUME".   */
/* The data is read in, validated and converted to binary format and made   */
/* available for plotting by the plot routines                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC LoadExternalDataFile( PCHAR Filename, PPLOT_SPECS PlotSpecs, msimWID
            Owner )
{
     msimSTRING string, word;
     ULONG num_points = 0;
     ULONG current_point;
     ULONG next_rec_to_store;
     USHORT interval;
     FILE *f;
     msimPFLOAT px, py;
     size_t array_size;
     msimFLOAT x, y;
     msimBOOL user_specified_data_types = FALSE;

     PlotSpecs->exp_data_ok = FALSE;

     /* first check whether memory is already allocated for external        */
     /* data. If so discard it                                              */

     if ( PlotSpecs->Pexpdata_x )
     {
          delete[]PlotSpecs->Pexpdata_x;
          PlotSpecs->Pexpdata_x = NULL;
     }
     if ( PlotSpecs->Pexpdata_y )
     {
          delete[]PlotSpecs->Pexpdata_y;
          PlotSpecs->Pexpdata_y = NULL;
     }

     /* first go through the file and get the data types                    */

     if ( NULL == ( f = fopen( Filename, "r" ) ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     /* get the first line                                                  */

     if ( NULL == fgets( string, sizeof ( string ), f ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          return msimREAD_ERROR;
     }

     // if first char == msimCOMMENT_CHAR   replace with blank

     if ( *string == msimCOMMENT_CHAR )
          *string = ' ';

     /* get first word in line                                              */

     msimUpCaseString( msimSubWord( string, ( USHORT ) 1 ), word );

     PlotSpecs->expdata_x_type = NO_DATA_TYPE;
     PlotSpecs->expdata_y_type = NO_DATA_TYPE;

     /* if the first word is "TIME"                                         */

     if ( ! strcmp( word, "TIME" ) )
          PlotSpecs->expdata_x_type = TIME_DATA;
     if ( ! strcmp( word, "TEMPERATURE" ) )
	  PlotSpecs->expdata_x_type = TEMP_DATA;

     /* get second word in line                                             */

     msimUpCaseString( msimSubWord( string, ( USHORT ) 2 ), word );

     if ( ! strcmp( word, "CONCENTRATION" ) )
	  PlotSpecs->expdata_y_type = CONC_DATA;

     if ( ! strcmp( word, "AMOUNT" ) )
	  PlotSpecs->expdata_y_type = CONC_DATA;

     if ( ! strcmp( word, "TEMPERATURE" ) )
     {
	  // can't have temperature vs temperature

	  if ( PlotSpecs->expdata_x_type != TEMP_DATA )
	       PlotSpecs->expdata_y_type = TEMP_DATA_Y;
     }

     if ( ! strcmp( word, "PRESSURE" ) )
          PlotSpecs->expdata_y_type = PRESS_DATA;

     if ( ! strcmp( word, "VOLUME" ) )
          PlotSpecs->expdata_y_type = VOL_DATA;

     /* by this point we should know both data types - if not               */
     /* then invoke dialog to allowd user to specify                        */

     if ( ( PlotSpecs->expdata_x_type == NO_DATA_TYPE ) || ( PlotSpecs->
                    expdata_y_type == NO_DATA_TYPE ) )
     {
	  msimRC  rc = msimSelectExternalDataTypesDlg( Owner,
	                PlotSpecs->plot_instance->volume_option,
                	PlotSpecs->expdata_x_type, PlotSpecs->expdata_y_type, Filename );

	  if ( rc == msimUSER_ABORT)
       {
            fclose( f );
	       return msimDATA_FILE_ERROR;
       }
       else
            user_specified_data_types = TRUE;

     }

     /* now count number of lines in file and allocate memory for two       */
     /* arrays of that size - don't bother checking for invalid data yet    */

     // go back to begining of file if first line did not contain valid data type specification,
     // since it probably contains numeric data instead

     if ( user_specified_data_types )
          rewind( f );

     while ( ! feof( f ) )
     {
          fgets( string, sizeof ( string ), f );
          if ( ! feof( f ) )
          {

               /* if we successfully read and converted two values          */

               if ( 2 == sscanf( string, "%lf %lf%s", &x, &y, word ) )
                    num_points++;
          }
     }

     if ( ferror( f ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          return msimREAD_ERROR;
     }

     // at this point we know the number of data points --

     if ( num_points == 0 )
     {
          String msg = String( ResId( msimLOAD_EXTDATA_STR1 ) ) +
                       String( Filename ) +
                       String( ResId( msimLOAD_EXTDATA_STR2 ) );

          InfoBox( Owner, msg ).Execute( );
          fclose( f );
          return msimDATA_FILE_ERROR;
     }

     if ( num_points <= PlotSpecs->max_pts_to_plot )
          interval = 1;
     else
     {
          interval = num_points / PlotSpecs->max_pts_to_plot;
          if ( num_points % PlotSpecs->max_pts_to_plot )
               interval++;
     }

     // note that array_size will not exceed the max allowed value for size_t
     // since it is calculated from max_pts_tp_plot which itself is tested
     // to be less than MAX_NO_RECORDS defined at top of this file 

     array_size = ( num_points / interval ) + 2;

     px = PlotSpecs->Pexpdata_x = new msimFLOAT[array_size];
     py = PlotSpecs->Pexpdata_y = new msimFLOAT[array_size];

     if ( ! px || ! py )
     {
          fclose( f );
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, Owner );
          return msimMEM_ALLOC_ERROR;
     }

     /* now go back to start of file, read and discard first line,          */
     /* and read in and convert the numeric data in the file                */

     rewind( f );
     current_point = 0;
     PlotSpecs->num_exp_data_sets = 0;

     next_rec_to_store = 1;

     // now read in the data and attempt to convert to binary format

     while ( ! feof( f ) )
     {
          fgets( string, sizeof ( string ), f );
          if ( ! feof( f ) )
          {

               /* if we successfully read and converted two values          */

               if ( 2 == sscanf( string, "%lf %lf%s", px, py, word ) )
               {
                    current_point++;
                    if ( current_point == next_rec_to_store )
                    {
                         px++;
                         py++;
                         PlotSpecs->num_exp_data_sets++;

                         next_rec_to_store += interval;
                         if ( next_rec_to_store > num_points )
                              next_rec_to_store = num_points;
                    }
               }
          }
     }
     if ( ferror( f ) )
     {
          msimFileError( Filename, Owner, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          return msimREAD_ERROR;
     }

     fclose( f );


     PlotSpecs->exp_data_ok = TRUE;

     return msimNO_ERROR;

}

/*--------------------------------------------------------------------------*/
/*                    SetExtFileButtonText()                                */
/*..........................................................................*/
/*                                                                          */
/* Function that sets the text of a button to show a filename               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void MainPlotDialog::SetExtFileButtonText( PCHAR Filename )
{
     String tmp = EXTERNAL_FILE_STRING;

     tmp += Filename;

     aPlotExtrnDataCB.SetText( tmp );
     return;
}

/*--------------------------------------------------------------------------*/
/*                    msimPlotDataDialog( )                                 */
/*..........................................................................*/
/*                                                                          */
/* creates the plot data setup dialog -- called from main menu loop         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

VOID msimPlotDataDialog( msimWID Owner, msimPINSTANCE Instance )
{

     /* if no data records are present in the file, return immediately      */

     if ( ! Instance->num_simulation_data_records )
     {
          InfoBox( Owner, ResId( msimNO_SIMULATION_MSG ) ) .Execute( );
          return;
     }

     if ( ! Instance->p_plot_dialog )
     {
          Instance->p_plot_dialog = new MainPlotDialog(Owner, Instance );

          if ( ! Instance->p_plot_dialog )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );
               return;
          }
     }
     else

          /* if the window already exists, show it         */

          Instance->p_plot_dialog->Show( );
     Instance->p_plot_dialog->ToTop( );


     return;
}

/*--------------------------------------------------------------------------*/
/*                        ReadSimulationData()                              */
/*..........................................................................*/
/*                                                                          */
/* Function for getitng the simulation results data in from the rxn file    */
/* and into arrays for creatng the plots. The function is passed a pointer  */
/* to the PLOT_SPECS struct which contains pointers to the arrays           */
/* allocated in memory for storing the data. Returns an error code if       */
/* everything does not go well in retrieving the data                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC MainPlotDialog::ReadSimulationData( )
{
     FILE *f;
     USHORT tp_option;
     ULONG record_count = 0;
     msimPFLOAT ptime, pvol, ppress, ptemp;
     msimPFLOAT pconc[MAX_NO_OF_PLOTS];
     USHORT i;
     ULONG *all_concs;
     msimFLOAT floatbuf;
     msimFLOAT conc_conversion_factor; /* for converting from
                                          particles to correct conc units     */
     msimFLOAT press_conversion_factor;/* for converting press
                                          units                               */
     msimFLOAT vol_conversion_factor;  /* for converting volume
                                          units                               */
     msimFLOAT time_conversion_factor; /* for converting time units           */
     PLOT_LIMITS datalimit;
     msimPINSTANCE instance = PlotSpecs.plot_instance;

     ULONG next_rec_to_store = 1;

     all_concs = new ULONG[instance->speciescount + 1];

     if ( NULL == all_concs )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, this );
          return msimMEM_ALLOC_ERROR;
     }

     aMainApp.Wait( TRUE );

     /* go ahead and try to open the file                                   */

     if ( NULL == ( f = fopen( PlotSpecs.plot_instance->filename, "rb" ) ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( instance->filename, this, ( USHORT ) msimFILE_OPEN_ERROR )
          ;
          delete[]all_concs;
          return msimFILE_OPEN_ERROR;
     }

     /* calc value for tp_option - will be used in determining the          */
     /* file format and how to read in the data                             */

     tp_option = msimValidTPVCombo( instance->variablepress,
          instance->temp_option, instance->volume_option );

     /* get the adresses to the various arrays and save them in             */
     /* local copies so we can modify the pointer values without damage     */

     ptime = PlotSpecs.Ptime;
     pvol = PlotSpecs.Pvol;
     ppress = PlotSpecs.Ppress;
     ptemp = PlotSpecs.Ptemp;
     for ( i = 0; i < MAX_NO_OF_PLOTS; i++ )
          pconc[i] = PlotSpecs.Pconc[i];

     conc_conversion_factor = msimCalcConversionFactor( instance );
     press_conversion_factor = msimConvFactorPressFromAtm
     ( instance->pressure_units );
     vol_conversion_factor = msimConvFactorVolumeFromLiters
     ( instance->volume_units );

     time_conversion_factor = msimConvFactorTimeFromSec( instance->time_units );

     /* initialize all limits to zero                                       */

     for ( i = 0; i < NUM_LIMITS; i++ )
          datalimit[i] = 0.0;

     /* now move to the start of the data as specified by the               */
     /* Instance->sim_data_file_offset                                      */

     PlotSpecs.num_data_sets = 0;

     if ( 0 != fseek( f, instance->sim_data_file_offset.offset, SEEK_SET ) )
     {
          aMainApp.Wait( FALSE );
          fclose( f );
          msimFileError( instance->filename, this, ( USHORT ) msimSEEK_ERROR );
          delete[]all_concs;
          return msimSEEK_ERROR;
     }
     /* read the data in staring at the first record                        */
     /* note that all the floating point data was saved in double precision */
     /* if we successfully read the first record                            */

     if ( fread( &floatbuf, sizeof ( msimFLOAT ), 1, f ) )
     {

          record_count++;
          if ( PlotSpecs.Ptime )
          {
               * ( ptime++ ) = time_conversion_factor * floatbuf;
               datalimit[MIN_TIME] = time_conversion_factor * floatbuf;
          }
          switch ( tp_option )
          {
          case msimCONST_TP_v :        /* read time only                      */

               break;
          case msimVAR_T_CONST_P_v :   /* read time and temp                  */
          case msimPROG_T_CONST_P_v :
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp )
               {
                    * ( ptemp++ ) = floatbuf;
                    datalimit[MIN_TEMP_Y] = datalimit[MAX_TEMP_Y] = floatbuf;
               }
               break;

          case msimCONST_T_VAR_P_CONST_V :/* read time and pressure           */
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ppress )
               {
                    * ( ppress++ ) = press_conversion_factor * floatbuf;

                    datalimit[MIN_PRESS] = datalimit[MAX_PRESS] =
                    press_conversion_factor * floatbuf;
               }
               break;

          case msimCONST_TP_VAR_V :    /* read time and volume                */
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Pvol )
               {
                    * ( pvol++ ) = vol_conversion_factor * floatbuf;
                    datalimit[MIN_VOLUME] = datalimit[MAX_VOLUME] =
                    vol_conversion_factor * floatbuf;
               }
               break;

          case msimVAR_T_VAR_P_CONST_V :/* read time, temp, pressure          */
          case msimPROG_T_VAR_P_CONST_V :

               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp )
               {
                    * ( ptemp++ ) = floatbuf;
                    datalimit[MIN_TEMP_Y] = datalimit[MAX_TEMP_Y] = floatbuf;
               }
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );

               if ( PlotSpecs.Ppress )
               {
                    * ( ppress++ ) = press_conversion_factor * floatbuf;
                    datalimit[MIN_PRESS] = datalimit[MAX_PRESS] =
                    press_conversion_factor * floatbuf;
               }
               break;

          case msimVAR_T_CONST_P_VAR_V :/* read time,temp,volume              */
          case msimPROG_T_CONST_P_VAR_V :

               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp )
               {
                    * ( ptemp++ ) = floatbuf;
                    datalimit[MIN_TEMP_Y] = datalimit[MAX_TEMP_Y] = floatbuf;
               }
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Pvol )
               {
                    * ( pvol++ ) = vol_conversion_factor * floatbuf;

                    datalimit[MIN_VOLUME] = datalimit[MAX_VOLUME] =
                    vol_conversion_factor * floatbuf;
               }

               break;
          }                            /* end switch                          */

          /* now read in the whole conc array out at once                   */

          fread( &( all_concs[1] ), sizeof ( ULONG ), instance->speciescount, f );

          /* and then pick out the data for the selected species of interest*/

          if ( PlotSpecs.plotconc_v_time || PlotSpecs.plotconc_v_temp )
          {

               /* copy data to be plotted into arrays                       */

               for ( i = 0; i < PlotSpecs.num_species_to_plot; i++ )
                    * ( pconc[i] ) ++= conc_conversion_factor * all_concs
                    [PlotSpecs.plotspeciesindex[i]];

               /* then go through the data to find min amd max              */

               datalimit[MIN_CONC] = datalimit[MAX_CONC] = * ( PlotSpecs.Pconc[0] );

               for ( i = 1; i < PlotSpecs.num_species_to_plot; i++ )
               {
                    if ( * ( PlotSpecs.Pconc[i] ) > datalimit[MAX_CONC] )
                         datalimit[MAX_CONC] = * ( PlotSpecs.Pconc[i] );
                    if ( * ( PlotSpecs.Pconc[i] ) < datalimit[MIN_CONC] )
                         datalimit[MIN_CONC] = * ( PlotSpecs.Pconc[i] );
               }
          }

     }                                 /* end if                              */

     PlotSpecs.num_data_sets = 1;

     /* while there are still data records to read, do this loop            */
     /* fread returns zero if not  successful in reading at least one item  */

     while ( record_count < instance->num_simulation_data_records )
     {
          if ( record_count == next_rec_to_store )
          {
               next_rec_to_store += PlotSpecs.sim_data_interval;

               if ( next_rec_to_store > instance->num_simulation_data_records )
                    next_rec_to_store = instance->num_simulation_data_records;
          }

          fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
          record_count++;

          if ( PlotSpecs.Ptime && record_count == next_rec_to_store )
               * ( ptime++ ) = time_conversion_factor * floatbuf;

          switch ( tp_option )
          {

          case msimCONST_TP_v :        /* read time only                      */
          default :
               break;
          case msimVAR_T_CONST_P_v :   /* read time and temp                  */
          case msimPROG_T_CONST_P_v :
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp && ( record_count == next_rec_to_store ) )
               {
                    * ( ptemp++ ) = floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_TEMP_Y] > floatbuf )
                         datalimit[MIN_TEMP_Y] = floatbuf;

                    if ( datalimit[MAX_TEMP_Y] < floatbuf )
                         datalimit[MAX_TEMP_Y] = floatbuf;
               }
               break;

          case msimCONST_T_VAR_P_CONST_V :/* read time and pressure           */
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ppress && ( record_count == next_rec_to_store ) )
               {
                    * ( ppress++ ) = press_conversion_factor * floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_PRESS] > floatbuf )
                         datalimit[MIN_PRESS] = floatbuf;
                    if ( datalimit[MAX_PRESS] < floatbuf )
                         datalimit[MAX_PRESS] = floatbuf;
               }
               break;
          case msimCONST_TP_VAR_V :    /* read time and volume                */
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Pvol && ( record_count == next_rec_to_store ) )
               {
                    * ( pvol++ ) = vol_conversion_factor * floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_VOLUME] > floatbuf )
                         datalimit[MIN_VOLUME] = floatbuf;
                    if ( datalimit[MAX_VOLUME] < floatbuf )
                         datalimit[MAX_VOLUME] = floatbuf;
               }
               break;

          case msimVAR_T_VAR_P_CONST_V :/* read time, temp, pressure          */
          case msimPROG_T_VAR_P_CONST_V :

               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp && ( record_count == next_rec_to_store ) )
               {
                    * ( ptemp++ ) = floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_TEMP_Y] > floatbuf )
                         datalimit[MIN_TEMP_Y] = floatbuf;

                    if ( datalimit[MAX_TEMP_Y] < floatbuf )
                         datalimit[MAX_TEMP_Y] = floatbuf;
               }

               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );

               if ( PlotSpecs.Ppress && ( record_count == next_rec_to_store ) )
               {
                    * ( ppress++ ) = press_conversion_factor * floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_PRESS] > floatbuf )
                         datalimit[MIN_PRESS] = floatbuf;
                    if ( datalimit[MAX_PRESS] < floatbuf )
                         datalimit[MAX_PRESS] = floatbuf;
               }
               break;

          case msimVAR_T_CONST_P_VAR_V :/* read time,temp,volume              */
          case msimPROG_T_CONST_P_VAR_V :
               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );
               if ( PlotSpecs.Ptemp && ( record_count == next_rec_to_store ) )
               {
                    * ( ptemp++ ) = floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_TEMP_Y] > floatbuf )
                         datalimit[MIN_TEMP_Y] = floatbuf;

                    if ( datalimit[MAX_TEMP_Y] < floatbuf )
                         datalimit[MAX_TEMP_Y] = floatbuf;
               }

               fread( &floatbuf, sizeof ( msimFLOAT ), 1, f );

               if ( PlotSpecs.Pvol && ( record_count == next_rec_to_store ) )
               {
                    * ( pvol++ ) = vol_conversion_factor * floatbuf;

                    /* now adjust min/max values if necessary               */

                    if ( datalimit[MIN_VOLUME] > floatbuf )
                         datalimit[MIN_VOLUME] = floatbuf;

                    if ( datalimit[MAX_VOLUME] < floatbuf )
                         datalimit[MAX_VOLUME] = floatbuf;
               }

               break;
          }                            /* end switch                          */

          /* now read in the whole conc array out at once                   */

          fread( &( all_concs[1] ), sizeof ( ULONG ), instance->speciescount, f );

          /* and then pick out the data for the selected species of interest*/

          if ( ( PlotSpecs.plotconc_v_time || PlotSpecs.plotconc_v_temp )
                    && ( record_count == next_rec_to_store )
               )
          {
               for ( i = 0; i < PlotSpecs.num_species_to_plot; i++ )
               {
                    * ( pconc[i] ) = conc_conversion_factor * all_concs
                    [PlotSpecs.plotspeciesindex[i]];

                    if ( * ( pconc[i] ) > datalimit[MAX_CONC] )
                         datalimit[MAX_CONC] = * ( pconc[i] );

                    if ( * ( pconc[i] ) < datalimit[MIN_CONC] )
                         datalimit[MIN_CONC] = * ( pconc[i] );

                    ( pconc[i] ) ++;
               }
          }

          if ( record_count == next_rec_to_store )// keep track of how many pts we have
               PlotSpecs.num_data_sets++;


     }                                 /* end while not eof                   */

     if ( ferror( f ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( instance->filename, this, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          delete[]all_concs;
          return msimREAD_ERROR;
     }

     fclose( f );

     if ( PlotSpecs.Ptime )
          datalimit[MAX_TIME] = * ( --ptime );

     if ( PlotSpecs.Ptemp && PlotSpecs.plot_temp_x )
     {
          datalimit[MIN_TEMP_X] = datalimit[MIN_TEMP_Y];
          datalimit[MAX_TEMP_X] = datalimit[MAX_TEMP_Y];
     }

     aMainApp.Wait( FALSE );

     /* test to make sure the record counts agree - if not there was        */
     /* an error someplace                                                  */

     if ( record_count != instance->num_simulation_data_records )
     {

          String message;
          message =  String( ResId( msimMAIN_PLOT_DLG_STR10 ) ) +
          String( instance->num_simulation_data_records ) +
          String( ResId( msimMAIN_PLOT_DLG_STR11 ) ) +
          String( record_count ) +
          String( ResId( msimMAIN_PLOT_DLG_STR12 ) );

          ErrorBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          delete[]all_concs;
          return msimRECORD_COUNT;
     }

     /* now calc the Tickmarks using devbase fcn for all data sets          */
     /* to be plotted                                                       */
     /* We adjust all limits so that they are slightly larger/smalller than */
     /* actual data limits. By doing so, any rounding error in the converting*/
     /* strings in the SetPlotLimits Dialog should not cause extreme points */
     /* to not be plotted. PLOTLIMIT_SCALINGFACTOR sets the amount          */
     /* first move min/max data to permanent structs in pPlotSpecs          */

     PlotSpecs.time_axis.datamin = datalimit[MIN_TIME];

     PlotSpecs.time_axis.datamax = datalimit[MAX_TIME];

     // 5.11.94 added this in an attempt to make it so that, if the simulation
     // stopped because of reaching the user-set elapsed time limit, the default
     // max value on the time axis is equal to that limit

     if ( msimELAPSED_TIME_LIMIT == instance->sim_return_code )
     {
          msimFLOAT et_limit = 0.0;

          if ( msimValidFloat( instance->elapsed_time_limit, &et_limit )
                    && ( et_limit != 0.0 )
               )
               PlotSpecs.time_axis.datamax = et_limit;
     }

     PlotSpecs.conc_axis.datamin = datalimit[MIN_CONC];
     PlotSpecs.conc_axis.datamax = datalimit[MAX_CONC];
     PlotSpecs.temp_y_axis.datamin = datalimit[MIN_TEMP_Y];
     PlotSpecs.temp_y_axis.datamax = datalimit[MAX_TEMP_Y];
     PlotSpecs.press_axis.datamin = datalimit[MIN_PRESS];
     PlotSpecs.press_axis.datamax = datalimit[MAX_PRESS];
     PlotSpecs.volume_axis.datamin = datalimit[MIN_VOLUME];
     PlotSpecs.volume_axis.datamax = datalimit[MAX_VOLUME];
     PlotSpecs.temp_x_axis.datamin = datalimit[MIN_TEMP_X];
     PlotSpecs.temp_x_axis.datamax = datalimit[MAX_TEMP_X];

     msimCheckForInvalidLimits( &PlotSpecs );
     msimSetPlotAndAxisLimits( &PlotSpecs, PLOTLIMIT_SCALINGFACTOR );

     delete[]all_concs;

     return msimNO_ERROR;
}

void msimClosePlotDialogWindow( msimPINSTANCE Instance )
{
     if ( Instance->p_plot_dialog != msimWID_INVALID )
          ( ( MainPlotDialog PTR ) ( Instance->p_plot_dialog ) ) ->PlotCancelHandler( NULL );
}

void msimShowPlotDialogWindow( msimPINSTANCE Instance )
{
     if ( Instance->p_plot_dialog != msimWID_INVALID )
          ( ( MainPlotDialog PTR ) ( Instance->p_plot_dialog ) ) ->Show( );
}

void msimHidePlotDialogWindow( msimPINSTANCE Instance )
{
     if ( Instance->p_plot_dialog != msimWID_INVALID )
          ( ( MainPlotDialog PTR ) ( Instance->p_plot_dialog ) ) ->Hide( );
}


