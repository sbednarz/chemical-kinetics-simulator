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
//  PROGRATE.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module declares the ProgRateConstDialog class
//
//  CREATED : 2.18.97 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>


#endif


#include "prograte.hrc"
#include "popup.hrc"




// ---------------------------------------------------------------------------
//                  class MiniPlotWindow
// ............................................................................
//
//
//  for displaying an x/y plot of the data tabulated in MLE 
//
// ----------------------------------------------------------------------------

class MiniPlotWindow : public Window
{
protected:
     MultiXyPlot    plot;
     XyPlot*        p_xyplot;

public :

     MiniPlotWindow( Window* pParent, const ReactionScheme& rScheme, const ResId& rResId );

     virtual void Paint( const Rectangle& );
     virtual void    Resize();

     XyPlot&       GetXyPlot() { return *p_xyplot; }
};


// ----------------------------------------------------------------------------
//                         MiniPlotWindow::MiniPlotWindow( )
// ............................................................................
//
//  constructor 
// ----------------------------------------------------------------------------

MiniPlotWindow::MiniPlotWindow( Window* pParent, const ReactionScheme& rScheme, const ResId& rResId ):
Window( pParent, rResId ),
plot( this )
{
     const enum FontFamily font_family = FAMILY_SWISS;
     const enum ColorName  bg_color    = COL_WHITE;

     ChangeFont( GetFontMetric( Font( font_family, Size( 0,10 ) ) ).GetPhysicalFont() );
     ChangeBackgroundBrush( Brush( bg_color, BRUSH_SOLID ) );

     p_xyplot = plot.AddPlot();

     p_xyplot->SetXAxisTitle( String( ResId( vID_STR_RPRO_1 ) ) );
     p_xyplot->SetYAxisTitle( String( ResId( vID_STR_RPRO_2 ) ) );

     p_xyplot->SetXAxisUnits( rScheme.GetTimeUnits() );
     p_xyplot->SetYAxisUnits( String() );

     p_xyplot->SetPlotTitle( String( ResId( vID_STR_RPRO_3 ) ) );

     p_xyplot->ShowMarkers( TRUE );


     plot.SetActiveArea( Rectangle( Point( 0,0 ), GetOutputSize() ) );
     plot.ResizeObject();

}





// ----------------------------------------------------------------------------
//                      MiniPlotWindow::Paint( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MiniPlotWindow::Paint( const Rectangle& )
{
     plot.DrawObject();
}


// ----------------------------------------------------------------------------
//                             MiniPlotWindow::Resize( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MiniPlotWindow::Resize( )
{
     plot.SetActiveArea( Rectangle( Point( 0,0 ), GetOutputSize() ) );
     plot.ResizeObject();

     Invalidate();
}






// ----------------------------------------------------------------------------
//         class ProgRateConstDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ProgRateConstDialog : public ModalDialog
{
protected:

     FixedText      filename_text;
     PushButton     select_profile_btn;
     PushButton     create_profile_btn;
     FixedText      profile_edit_label_text;
     MultiLineEdit  profile_multiline_entryfield;
     PushButton     refresh_btn;
     OKButton       close_btn;
     PushButton     undo_btn;
     PushButton     save_as_btn;
     HelpButton     help_btn;
     GroupBox       group_box_2;
     GroupBox       group_box_1;
     MiniPlotWindow plot_window;

     BOOL8          data_altered;
     BOOL8          file_data_altered;
     String&        r_filename;
     XyDataFile     profile;

     void           SetDataAlteredFlag();
     void           ClearDataAlteredFlag();
     void           Initialize();

     void           EntryError();
     void           NewProfile();
     BOOL8          SaveDataIfOK();
     void           SaveToFile();
     void           GetDataFromFile();

public:

     ProgRateConstDialog( Window* pParent, const ReactionScheme& rScheme, String& rFilename );   

     void CloseBtnHandler( OKButton* );
     void UndoBtnHandler( PushButton* );
     void RefreshBtnHandler( PushButton* );
     void FileTextModifiedHandler( Edit* );
     void SelectFileHandler( PushButton* );
     void CreateBtnHandler( PushButton* );
     void SaveAsBtnHandler( PushButton* );
};




// ----------------------------------------------------------------------------
//         ProgRateConstDialog::ProgRateConstDialog()
// ............................................................................
// 
// constructor 
//
// ----------------------------------------------------------------------------

ProgRateConstDialog::ProgRateConstDialog( Window* pParent, const ReactionScheme& rScheme, String& rFilename ) :
ModalDialog( pParent, ResId( vID_DLG_RATE_PROFILE ) ),
filename_text( this, ResId( vID_STEXT_FILENAME ) ), 
select_profile_btn( this, ResId( vID_BTN_SELECT_FILE ) ), 
create_profile_btn( this, ResId( vID_BTN_PROFILE_CREATE ) ), 
profile_edit_label_text( this, ResId( vID_STEXT_EDIT_PROF_LABEL ) ), 
profile_multiline_entryfield( this, ResId( vID_MLE_EDIT_PROFILE ) ), 
refresh_btn( this, ResId( vID_BTN_REFRESH ) ), 
close_btn( this, ResId( vID_BTN_CLOSE ) ), 
undo_btn( this, ResId( vID_BTN_UNDO ) ), 
save_as_btn( this, ResId( vID_BTN_SAVEAS ) ), 
help_btn( this, ResId( vID_BTN_HELP ) ), 
group_box_2( this, ResId( 2 ) ), 
group_box_1( this, ResId( 1 ) ),
plot_window( this, rScheme, ResId( vID_WINDOW_PLOT_WINDOW ) ),
data_altered( FALSE ),
file_data_altered( FALSE ),
r_filename( rFilename ),
profile()
{
     FreeResource( );

     profile.SetFilename( rFilename );
     Initialize();

     GetpMainApp()->CenterWindowOnParent( this, pParent );

     plot_window.GetXyPlot().AddDataSet( profile.GetXData(), profile.GetYData(), String( "k") );
     plot_window.Show();

     select_profile_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, SelectFileHandler ) );
     refresh_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, RefreshBtnHandler ) );
     close_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, CloseBtnHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, UndoBtnHandler ) );
     create_profile_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, CreateBtnHandler ) );
     save_as_btn.ChangeClickHdl( LINK( this, ProgRateConstDialog, SaveAsBtnHandler ) ); 

     profile_multiline_entryfield.ChangeModifyHdl(
          LINK( this, ProgRateConstDialog, FileTextModifiedHandler ) );

     profile.MarkFormatErrors( TRUE );
     profile.AllowZeroYValues( TRUE );
}






// ----------------------------------------------------------------------------
//         ProgRateConstDialog::GetDataFromFile()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::GetDataFromFile( )
{
     if ( profile.InitializeFromFile() )
     {
          if ( profile.IsStatusFlagSet( DATA_FORMAT_ERROR ) )
          {
               EntryError();
          }
          profile_multiline_entryfield.SetText( profile.GetContents() );
          plot_window.GetXyPlot().ResetLimits();
          filename_text.SetText( profile.GetFilename() );

     }
     else
     {
          EntryError();
          NewProfile();
     }

     plot_window.Invalidate( );

}








// ----------------------------------------------------------------------------
//         ProgRateConstDialog::Initialize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::Initialize()
{
     // check for a null filename

     if ( !profile.GetFilename() )
     {
          NewProfile();
     }
     else
     {     
          filename_text.SetText( profile.GetFilename() );

          GetDataFromFile();
     }
}









// ----------------------------------------------------------------------------
//                  ProgRateConstDialog::NewProfile()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::NewProfile()
{
     profile.Reset( );
     filename_text.SetText( profile.GetFilename() );

     profile_multiline_entryfield.SetText( profile.GetContents( ) );

     plot_window.Invalidate( );

     return;
}







// ----------------------------------------------------------------------------
//                  ProgRateConstDialog::SetDataAlteredFlag()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::SetDataAlteredFlag()
{
     if ( !data_altered )
     {
          data_altered = TRUE;
          undo_btn.Enable();
     }
}





// ----------------------------------------------------------------------------
//                  ProgRateConstDialog::ClearDataAlteredFlag()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::ClearDataAlteredFlag()
{
     data_altered = FALSE;
     file_data_altered = FALSE;

     undo_btn.Disable();
}




// ----------------------------------------------------------------------------
//                     ProgRateConstDialog::EntryError( )
// ............................................................................
// 
// popup dialog to inform user of input data error 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::EntryError( )
{
      WarningBox( this, WB_OK | WB_DEF_OK, profile.GetErrorMessage() ).Execute( );
}







// ----------------------------------------------------------------------------
//         ProgRateConstDialog::UndoBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::UndoBtnHandler( PushButton* )
{
     profile.SetFilename( r_filename );
     Initialize();
     ClearDataAlteredFlag( );
}


// ----------------------------------------------------------------------------
//         ProgRateConstDialog::CreateBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::CreateBtnHandler( PushButton* )
{
     NewProfile();
     SetDataAlteredFlag();
}




// ----------------------------------------------------------------------------
//         ProgRateConstDialog::SaveAsBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::SaveAsBtnHandler( PushButton* )
{
     String filename;

     if ( !SaveAsFileDialog( filename, RATE_PROFILE_FILETYPE ) )
          return;

     // Successfully obtained the profile filename.
     // update the panel with a new profile filename

     profile.SetFilename( filename );
     filename_text.SetText( profile.GetFilename() );
     RefreshBtnHandler( &refresh_btn );
     SaveToFile( );
     SetDataAlteredFlag();

     plot_window.Invalidate();
}






// ----------------------------------------------------------------------------
//         ProgRateConstDialog::SaveToFile( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::SaveToFile( )
{
     TextOutputStream outfile( profile.GetFilename() );

     if ( outfile.IsOpen() )
     {
          profile.WriteAllDataToTextStream( outfile );

          if (FALSE == outfile.FileError() )
          {
               file_data_altered = FALSE;
          }
          else
          {
               GetpMainApp()->FileWriteError( profile.GetFilename() );
          }
     }
     else
     {
          GetpMainApp()->FileOpenError( profile.GetFilename() );
     }

}










// ----------------------------------------------------------------------------
//         ProgRateConstDialog::RefreshBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::RefreshBtnHandler( PushButton* )
{
     if ( profile.InitializeFromString( profile_multiline_entryfield.GetText() ) )
     {
          if ( profile.IsStatusFlagSet( DATA_FORMAT_ERROR ) )
          {
               profile_multiline_entryfield.SetText( profile.GetContents() );
               EntryError();
          }
          plot_window.GetXyPlot().ResetLimits();
          plot_window.Invalidate();

     }
}







// ----------------------------------------------------------------------------
//         ProgRateConstDialog::SelectFileHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::SelectFileHandler( PushButton* )
{
     String filename;

     if ( file_data_altered )
          if ( FALSE == SaveDataIfOK( ) )
               return;

     if ( !OpenFileDialog( filename, RATE_PROFILE_FILETYPE ) )
          return;

     // Successfully obtained the profile filename
     // update the panel with a new profile filename

     profile.SetFilename( filename );
     GetDataFromFile();

     SetDataAlteredFlag();

     return;
}








// ----------------------------------------------------------------------------
//                    ProgRateConstDialog::SaveDataIfOK( )
// ............................................................................
// 
// return FALSE if user wants to cancel, TRUE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 ProgRateConstDialog::SaveDataIfOK( )
{
     // do this if we have a null filename for profile 

     if ( !profile.GetFilename() )
     {
          switch ( InfoBox( this, ResId( vID_POPUP_SAVE_UNNAMED_PROFILE ) ).Execute() )
          {

          case RET_CANCEL :

               return FALSE;

               break;


          case RET_NO :

               return TRUE;
               break;
     
          case RET_YES :

               {
                    String filename;
                
                    if ( !SaveAsFileDialog( filename, RATE_PROFILE_FILETYPE ) )
                          return FALSE;
                
                    // Successfully obtained the profile filename.
                    // update the panel with a new profile filename
                
                    profile.SetFilename( filename );
                    filename_text.SetText( profile.GetFilename() );
                    RefreshBtnHandler( &refresh_btn );

                    SaveToFile( );
                    return TRUE;
               }
          }

     }
     else
     {
          String message = String( ResId( vID_STR_RPRO_4 ) ) +
                           DirEntry( profile.GetFilename() ).GetName() +
                           String( ResId( vID_STR_RPRO_5 ) );
     
          WarningBox box( this, WB_YES_NO_CANCEL | WB_DEF_NO, message );
     
          switch ( box.Execute( ) )
          {
          case RET_YES :
     
               if ( FileExists( profile.GetFilename() ) )
                    if ( ! GetpMainApp()->OKtoOverwriteFile( profile.GetFilename() ) )
                         return FALSE;
     
     
               RefreshBtnHandler( &refresh_btn );
               SaveToFile( );
     
               return TRUE;
     
          case RET_NO :
     
               return TRUE;
     
          case RET_CANCEL :
     
               return FALSE;
          }
     }


}





// ----------------------------------------------------------------------------
//         ProgRateConstDialog::TextModifiedHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::FileTextModifiedHandler( Edit* )
{
     if ( !file_data_altered )
     {
          file_data_altered = TRUE;
          undo_btn.Enable();
     }
}






// ----------------------------------------------------------------------------
//         ProgRateConstDialog::CloseBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ProgRateConstDialog::CloseBtnHandler( OKButton* )
{
     if ( !file_data_altered && !data_altered)
     {
          EndDialog( (short) FALSE ); 
     }
     else
     {
          // if we get here then something has been modified by the user

          if ( file_data_altered )
          {
               if ( FALSE == SaveDataIfOK( ) )
                    return;
          }
     
     
          if ( data_altered )
          {
               r_filename = profile.GetFilename();
          }

          EndDialog( (short) TRUE ); 
     }
}








// ----------------------------------------------------------------------------
//                 EditProgRateConstant()
// ............................................................................
//
//
// ----------------------------------------------------------------------------


BOOL8 EditProgRateConstant( Window* pParent, const ReactionScheme& rScheme, 
 String& rProfileFilename )
{

     ProgRateConstDialog* pdlg = new ProgRateConstDialog( pParent, rScheme, rProfileFilename );

     if ( pdlg )
     {
          BOOL8 rc = pdlg->Execute();
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}






