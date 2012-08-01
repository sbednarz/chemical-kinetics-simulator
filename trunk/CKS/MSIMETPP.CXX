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
/*  FILE NAME : msimetpp.cxx                                                */
/*                                                                          */
/*  This module implements dialog for selecting and viewing the contents of */
/*  external temperature profile data files                                 */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for mutliple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1993                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"
#include "msimplot.hxx"
#include "msimtprg.hxx"
#include "msimfile.hxx"

#include <stdlib.h>

#define  BORDER      10

// Class derived from Modaldialog class

class SelectExtTProfileDlg : public ModalDialog
{
protected :
     FixedText FileNameLabel;
     PushButton SelectFilePB;
     PushButton CreateNewPB;
     FixedText MLELabel;
     MultiLineEdit FileMLE;
     PushButton RefreshPB;
     FixedText aFixedText2;
     FixedText aFixedText3;
     Edit StepSizeSLE;
     OKButton ClosePB;
     PushButton UndoPB;
     PushButton SaveAsPB;
     HelpButton HelpPB;

     XYWindow aPlotXYWindow;

     GroupBox aGroupBox2;
     GroupBox aGroupBox1;

     TProfile temp_profile;

     msimBOOL data_altered;
     msimBOOL file_data_altered;
     msimFILE_STRING filename;

     msimPINSTANCE psim_instance;

     msimPFLOAT arrayX;
     msimPFLOAT arrayY;

     PPLOT_SPECS pplot_specs;

     void InitializeEntries( );
     msimBOOL InitializePlotWindow( );
     void InitializePlotSpecs( );
     void GetDataFromFile( );
     void SaveToFile( );
     void SetupForNewFile( );
     msimTPRO_RC RefreshPlot( );
     msimTPRO_RC SaveDataIfOK( );
     void ProcessDependingOnRC( msimTPRO_RC Rc );

public :

     SelectExtTProfileDlg( Window PTR pParent, msimPINSTANCE pInstance, PPLOT_SPECS pPlotSpecs );
     ~SelectExtTProfileDlg( );

     void SetModifiedFlagHandler( Edit PTR pEdit );
     void SetFileModifiedFlagHandler( Edit PTR pEdit );
     void SelectFileHandler( PushButton PTR pButton );
     void CreateFileHandler( PushButton PTR );
     void RefreshPlotHandler( PushButton PTR pButton );
     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void SaveAsHandler( PushButton PTR pButton );
};

SelectExtTProfileDlg::~SelectExtTProfileDlg( )
{
     if ( arrayX )
          delete[]arrayX;
     if ( arrayY )
          delete[]arrayY;
}


SelectExtTProfileDlg::SelectExtTProfileDlg( Window PTR pParent,
     msimPINSTANCE pInstance, PPLOT_SPECS pPlotSpecs ) :
ModalDialog ( pParent, ResId ( msimEXT_T_PROFILE_PANEL ) ),
FileNameLabel ( this, ResId ( msimEXT_T_PROFILE_FILENAME ) ),
SelectFilePB ( this, ResId ( msimEXT_T_PROFILE_FILE_BTN ) ),
CreateNewPB ( this, ResId ( msimEXT_T_PROFILE_CREATE ) ),
MLELabel ( this, ResId ( msimVIEW_EDIT_PROF_TIME_LABEL ) ),
FileMLE ( this, ResId ( msimVIEW_EDIT_PROF_FILE ) ),
RefreshPB ( this, ResId ( msimVIEW_EDIT_PROF_REFRESH ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
StepSizeSLE ( this, ResId ( msimEXT_T_PROFILE_STEPSIZE ) ),
ClosePB ( this, ResId ( msimEXT_T_PROFILE_CLOSE ) ),
UndoPB ( this, ResId ( msimEXT_T_PROFILE_UNDO ) ),
SaveAsPB ( this, ResId ( msimVIEW_EDIT_PROF_SAVEAS ) ),
HelpPB ( this, ResId ( msimEXT_T_PROFILE_HELP ) ),
aPlotXYWindow ( this, pPlotSpecs ),
aGroupBox2 ( this, ResId ( 2 ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
temp_profile ( )
{
     FreeResource( );

     pplot_specs = pPlotSpecs;
     psim_instance = pInstance;
     arrayX = arrayY = NULL;

     SetText( GetText( ) + String( psim_instance->base_filename ) );

     if ( ! InitializePlotWindow( ) )
          return;

     InitializeEntries( );

     StepSizeSLE.ChangeModifyHdl(
          LINK( this, SelectExtTProfileDlg, SetModifiedFlagHandler ) );

     FileMLE.ChangeModifyHdl(
          LINK( this, SelectExtTProfileDlg, SetFileModifiedFlagHandler ) );

     SelectFilePB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, SelectFileHandler ) );
     CreateNewPB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, CreateFileHandler ) );

     RefreshPB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, RefreshPlotHandler ) );
     SaveAsPB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, SaveAsHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, SelectExtTProfileDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );


     aPlotXYWindow.Invalidate( );
}



msimBOOL SelectExtTProfileDlg::InitializePlotWindow( )
{
     SHORT xy_width, xy_height;

     // set initial position of aPlotXYWindow
     // we place its top level with top of the Time label and its left
     // edge with BORDER over from right edge of MLE

     // set window position (upper left corner )

     aPlotXYWindow.ChangePosPixel( Point( FileMLE.GetPosPixel( ) .X( ) +
               FileMLE.GetSizePixel( ) .Width( ) + BORDER,
               MLELabel.GetPosPixel( ) .Y( ) ) );

     // set the window size

     xy_width = aGroupBox2.GetSizePixel( ) .Width( ) - aPlotXYWindow.GetPosPixel( ) .X( ) -
     BORDER;
     xy_height = aGroupBox2.GetPosPixel( ) .Y( ) + aGroupBox2.GetSizePixel( ) .Height( )
          - aPlotXYWindow.GetPosPixel( ) .Y( ) - BORDER;

     aPlotXYWindow.ChangeOutputSizePixel( Size( xy_width, xy_height ) );

#if defined(OLD_MAPMODE)

     // set mapmode to that expected by plotting routines
     /* adjust map mode of aPlotXYWindow to deal with change in size          */
     /* the origin needs to be reset  */

     MapMode aOldMapMode = aPlotXYWindow.GetMapMode( );
     aOldMapMode.ChangeOrigin( PixelToLogic( Point( 0, xy_height * -1 ), aOldMapMode.GetMapUnit( ) ) );
     aPlotXYWindow.ChangeMapMode( aOldMapMode );
#endif

     /* update structure so that we aPlotXYWindow can retrieve size info    */

     pplot_specs->vertical_win_size = abs( aPlotXYWindow.GetOutputSize( ) .Height( ) );//xy_height;
     pplot_specs->horizontal_win_size = abs( aPlotXYWindow.GetOutputSize( ) .Width( ) );//xy_width;

     aPlotXYWindow.ChangeColorSet( pplot_specs->current_colorset );

     aPlotXYWindow.InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );

     aPlotXYWindow.ChangeBackgroundBrush( Brush( pplot_specs->background_color, BRUSH_SOLID ) );

     return TRUE;
}



void SelectExtTProfileDlg::ProcessDependingOnRC( msimTPRO_RC Rc )
{
     if ( Rc != msimTPRO_NO_ERROR )
          temp_profile.ShowErrorMsg( filename, Rc, msimTPRO_EDIT_PROFILE, this );

     if ( Rc & msimTPRO_SEVERE_ERROR )
     {
          // these are serious errors - temp profile data
          // in undefined state so don't even try to show it.
          // Reset vars to default state

          msimStringCopy( filename, msimNULL_STR, sizeof filename );
          FileNameLabel.SetText( String( NO_FILENAME ) );
          temp_profile.InitializeFromString( String( msimNULL_STR ) );

          file_data_altered = FALSE;
     }
     else
     {
          // only data format errors or no errors at all come here

          // set file data altered flag if we have added some text to
          // the buffer to point out an error

          if ( Rc & ( msimTPRO_INVALID_DATA |
                         msimTPRO_TIME_ORDER_ERROR | msimTPRO_INVALID_TEMP_VALUE ) )
               file_data_altered = TRUE;

          // if the error code did not indicate zero data points then proceed
          // with allocatng and filling the arrays

          if ( ! ( Rc & msimTPRO_NO_DATA_PTS ) )
          {
               // first acquire a count of the number of valid data points

               size_t no_XY_pairs = temp_profile.GetNumberOfValidDataPoints( );

               if ( no_XY_pairs > 1 )
               {
                    // if the pointers are previously defined, then deallocate them

                    if ( arrayX != NULL )
                         delete[]arrayX;

                    if ( arrayY != NULL )
                         delete[]arrayY;

                    // now allocate two arrays

                    arrayX = new msimFLOAT[no_XY_pairs];
                    arrayY = new msimFLOAT[no_XY_pairs];

                    if ( arrayX == NULL || arrayY == NULL )
                    {
                         temp_profile.ShowErrorMsg( filename, msimTPRO_MEM_ALLOC_ERROR,
                              msimTPRO_EDIT_PROFILE, this );

                         // reset dlg to default state

                         msimStringCopy( filename, msimNULL_STR, sizeof filename );
                         FileNameLabel.SetText( String( NO_FILENAME ) );
                         temp_profile.InitializeFromString( String( msimNULL_STR ) );

                         file_data_altered = FALSE;

                    }
                    else
                    {
                         if ( msimTPRO_NO_ERROR != ( Rc = temp_profile.GetValidDataInBinary( arrayX, arrayY ) ) )
                              temp_profile.ShowErrorMsg( filename, Rc, msimTPRO_EDIT_PROFILE, this );

                         return;
                    }
               }
          }
     }

}





void SelectExtTProfileDlg::GetDataFromFile( )
{
     file_data_altered = FALSE;

     ProcessDependingOnRC( temp_profile.InitializeFromFile( filename ) );

     FileMLE.SetText( temp_profile.GetContents( ) );

     InitializePlotSpecs( );

     aPlotXYWindow.Invalidate( );

     return;
}

void SelectExtTProfileDlg::SetupForNewFile( )
{

     temp_profile.InitializeFromString( String( msimNULL_STR ) );

     FileMLE.SetText( temp_profile.GetContents( ) );
     file_data_altered = FALSE;

     // if arrays are already allocated then delete

     if ( arrayX != NULL )
     {
          delete[]arrayX;
          arrayX = NULL;
     }

     if ( arrayY != NULL )
     {
          delete[]arrayY;
          arrayY = NULL;
     }

     InitializePlotSpecs( );

     aPlotXYWindow.Invalidate( );

     return;
}

void SelectExtTProfileDlg::InitializePlotSpecs( )
{
     // the following variables need to be updated
     // everytime the plot is refreshed

     pplot_specs->max_pts_to_plot =
     pplot_specs->num_data_sets = temp_profile.GetNumberOfValidDataPoints( );

     pplot_specs->Ptime = arrayX;
     pplot_specs->Ptemp = arrayY;
     pplot_specs->time_axis.datamin = temp_profile.GetMinX( );
     pplot_specs->time_axis.datamax = temp_profile.GetMaxX( );
     pplot_specs->temp_y_axis.datamin = temp_profile.GetMinY( );
     pplot_specs->temp_y_axis.datamax = temp_profile.GetMaxY( );

     if ( temp_profile.GetNumberOfValidDataPoints( ) > 1 )
     {
          pplot_specs->plottemp_v_time = TRUE;

          // check for the invalid limits and determine the scaling factor

          msimCheckForInvalidLimits( pplot_specs );

          msimSetPlotAndAxisLimits( pplot_specs, PLOTLIMIT_SCALINGFACTOR );
     }
     else
          pplot_specs->plottemp_v_time = FALSE;

     return;
}

void SelectExtTProfileDlg::InitializeEntries( )
{
     // initialize the panel

     StepSizeSLE.SetText( String( psim_instance->temp_data.max_step_size ) );

     msimStringCopy( filename, psim_instance->temp_profile_data, sizeof filename );

#if defined(__MAC__)
     if ( msimWords( filename ) > 0 )
#else
     if ( msimWords( filename ) == 1 )
#endif
     {
          FileNameLabel.SetText( String( filename ) );

          if ( msimFileExists( filename ) )
               GetDataFromFile( );
          else
          {
               temp_profile.ShowErrorMsg( filename, msimTPRO_FILENAME_ERROR,
                    msimTPRO_EDIT_PROFILE, this );

               SetupForNewFile( );
          }
     }
     else
     {
          FileNameLabel.SetText( String( NO_FILENAME ) );

          msimStringCopy( filename, msimNULL_STR, sizeof filename );

          SetupForNewFile( );
     }

     data_altered = FALSE;

     return;
}

void SelectExtTProfileDlg::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;
     return;
}

void SelectExtTProfileDlg::SetFileModifiedFlagHandler( Edit PTR )
{
     file_data_altered = TRUE;
     return;
}



void SelectExtTProfileDlg::RefreshPlotHandler( PushButton PTR )
{
     RefreshPlot( );
}


msimTPRO_RC SelectExtTProfileDlg::RefreshPlot( )
{
     msimTPRO_RC rc;

     ProcessDependingOnRC( rc = temp_profile.InitializeFromString( FileMLE.GetText( ) ) );

     FileMLE.SetText( temp_profile.GetContents( ) );

     InitializePlotSpecs( );

     aPlotXYWindow.Invalidate( );

     return rc;
}



void SelectExtTProfileDlg::SaveAsHandler( PushButton PTR )
{
     if ( msimUSER_ABORT == msimGetFileName( this,
                    msimAppOptions.external_t_profile_template,
                    filename, WB_SAVEAS,
                    "Enter a new name for the temp. profile file"
                    , msimTPROFILE_TYPE, sizeof filename )
          )
          return;

     // Successfully obtained the profile filename.
     // update the panel with a new profile filename

     FileNameLabel.SetText( String( filename ) );

     RefreshPlot( );
     SaveToFile( );
}

void SelectExtTProfileDlg::SaveToFile( )
{

     msimTPRO_RC rc = temp_profile.WriteContentsToFile ( filename );

     if ( rc == msimTPRO_NO_ERROR )
          file_data_altered = FALSE;
     else
     {
          temp_profile.ShowErrorMsg( filename, rc, msimTPRO_EDIT_PROFILE, this );
     }
}


void SelectExtTProfileDlg::CreateFileHandler( PushButton PTR )
{
     if ( file_data_altered )
          if ( msimUSER_ABORT == SaveDataIfOK( ) )
               return;

     // get a file name

     if ( msimUSER_ABORT == msimGetFileName( this,
                    msimAppOptions.external_t_profile_template,
                    filename, WB_SAVEAS,
                    "Enter a name for the new temp. profile file",
                    msimTPROFILE_TYPE,
                    sizeof filename
               )
          )
          return;

     // Successfully obtained the profile filename.
     // update the panel with a new profile filename

     FileNameLabel.SetText( String( filename ) );

     SetupForNewFile( );

     data_altered = TRUE;

     return;
}



void SelectExtTProfileDlg::SelectFileHandler( PushButton PTR )
{
     if ( file_data_altered )
          if ( msimUSER_ABORT == SaveDataIfOK( ) )
               return;

     // get a file name
     if ( msimUSER_ABORT == msimGetFileName( this,
                    msimAppOptions.external_t_profile_template,
                    filename, WB_OPEN, "Select a temperature profile file",
                    msimTPROFILE_TYPE, sizeof filename )
          )
          return;

     // make sure the file chosen exists

     if ( ! msimFileExists( filename ) )
     {
          temp_profile.ShowErrorMsg( filename, msimTPRO_FILENAME_ERROR,
               msimTPRO_EDIT_PROFILE, this );

          return;
     }

     // Successfully obtained the profile filename
     // update the panel with a new profile filename

     FileNameLabel.SetText( filename );

     GetDataFromFile( );

     data_altered = TRUE;

     return;
}



void SelectExtTProfileDlg::CloseHandler( PushButton PTR )
{
     msimREAL_STRING maxstepsize;
     String message;

     if ( file_data_altered )
          if ( msimUSER_ABORT == SaveDataIfOK( ) )
               return;


     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, psim_instance ) )
               return;

          msimStringCopy( maxstepsize, StepSizeSLE.GetText( ), sizeof maxstepsize );

          // make sure the numeric value is a valid entry

          if ( ! msimValidFloat( maxstepsize ) )
          {
               msimEntryFieldErrorMsg( this );
               return;
          }

          psim_instance->data_altered_since_lastsave = TRUE;


          msimStringCopy( psim_instance->temp_profile_data, filename,
               sizeof psim_instance->temp_profile_data );
          msimStringCopy( psim_instance->temp_data.max_step_size, maxstepsize,
               sizeof psim_instance->temp_data.max_step_size );
     }

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


msimRC SelectExtTProfileDlg::SaveDataIfOK( )
{
     String msg( ResId( msimTPRO_STR1 ) );

     msg += String( msimBaseFilename( filename ) );
     msg += String( ResId( msimTPRO_STR2 ) );


     WarningBox box( this, WB_YES_NO_CANCEL | WB_DEF_NO, msg );

     switch ( box.Execute( ) )
     {
     case RET_YES :

          // make sure that the current data has been validated and plotted
          if ( msimNO_ERROR != RefreshPlot( ) )
               return msimUSER_ABORT;

          if ( msimFileExists( filename ) )
               if ( ! msimOKtoOverwrite( filename, this ) )
                    return msimUSER_ABORT;


          SaveToFile( );

          return msimNO_ERROR;

     case RET_NO :

          return msimNO_ERROR;

     case RET_CANCEL :
          return msimUSER_ABORT;
     }
}


void SelectExtTProfileDlg::UndoHandler( PushButton PTR )
{
     InitializeEntries( );

     return;
}


void msimExternalTempProgDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     PLOT_SPECS plot_specs;

     plot_specs = C_PLOT_SPECS;
     plot_specs.plot_instance = pInstance;
     plot_specs.num_plots = 1;
     plot_specs.plot_time_x = TRUE;
     plot_specs.show_marker = TRUE;
     plot_specs.show_line = TRUE;
     plot_specs.show_grid = TRUE;
     plot_specs.typeface = msimAppOptions.fonttype;

#ifdef USE_MAINPLOT_FONTSIZE
     plot_specs.pointsize = msimAppOptions.fontsize;
#endif

     plot_specs.current_colorset = msimAppOptions.default_colorset;


     SelectExtTProfileDlg PTR pdlg =
                              new SelectExtTProfileDlg( Owner, pInstance, &plot_specs );

     if ( pdlg )
     {
          pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}

