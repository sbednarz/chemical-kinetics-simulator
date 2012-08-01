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
   |  msimappo.cxx   :   a module for MSIM                                  |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this file  contains functions for viewing and      |
   |                     modifying the application options                  |
   |                                                                        |
   |  authors        :   Fumiko Allen and Bill Hinsberg,  IBM Almaden       |
   |                                                                        |
   |  file created   :   August 1993                                        |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"


class AppOptionsDlg : public ModalDialog
{

protected :
     Edit DataDirSLE;
     Edit TextOutputDirSLE;
     Edit GraphicalOutputDirSLE;
     Edit ExternalDataFileDirSLE;
     Edit ExternalTempProDirSLE;
     CheckBox SaveWindowDataCB;
     CheckBox ConfirmFileReplaceCB;
     OKButton ClosePB;
     PushButton UndoPB;
     PushButton DefaultsPB;
     HelpButton aHelpPB;
     FixedText aFixedText1;
     FixedText aFixedText2;
     FixedText aFixedText3;
     FixedText aFixedText4;
     FixedText aFixedText30;
     GroupBox aGroupBox1;

     void InitializeDialog( msimPAPP_OPTIONS pAppOptions );

public :
     AppOptionsDlg( Window PTR pParent );

     void CloseHandler( PushButton PTR );
     void UndoHandler( PushButton PTR );
     void DefaultsHandler( PushButton PTR );
};


AppOptionsDlg::AppOptionsDlg( Window PTR pParent ) :
ModalDialog ( pParent, ResId ( msimAPPOPT_PANEL ) ),
DataDirSLE ( this, ResId ( msimAPPOPT_DATADIR ) ),
TextOutputDirSLE ( this, ResId ( msimAPPOPT_TEXTOUT_DIR ) ),
GraphicalOutputDirSLE ( this, ResId ( msimAPPOPT_GRAPHOUT_DIR ) ),
ExternalDataFileDirSLE ( this, ResId ( msimAPPOPT_EXTRN_FILE_DIR ) ),
ExternalTempProDirSLE ( this, ResId ( msimAPPOPT_TEMP_PRO_DIR ) ),
SaveWindowDataCB ( this, ResId ( msimAPPOPT_PLOTWIN_DATA ) ),
ConfirmFileReplaceCB ( this, ResId ( msimAPPOPT_FILE_CONFIRM ) ),
ClosePB ( this, ResId ( msimAPPOPT_CLOSE ) ),
UndoPB ( this, ResId ( msimAPPOPT_UNDO ) ),
DefaultsPB ( this, ResId ( msimAPPOPT_DEFAULTS ) ),
aHelpPB ( this, ResId ( msimAPPOPT_HELP ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aFixedText4 ( this, ResId ( 4 ) ),
aFixedText30 ( this, ResId ( 30 ) ),
aGroupBox1 ( this, ResId ( 1 ) )
{
     FreeResource( );

     InitializeDialog( &msimAppOptions );

     UndoPB.ChangeClickHdl( LINK( this,
               AppOptionsDlg, UndoHandler ) );
     DefaultsPB.ChangeClickHdl( LINK( this,
               AppOptionsDlg, DefaultsHandler ) );
     ClosePB.ChangeClickHdl( LINK( this,
               AppOptionsDlg, CloseHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


void AppOptionsDlg::InitializeDialog( msimPAPP_OPTIONS pAppOptions )
{
     DataDirSLE.SetText( String( pAppOptions->data_template ) );

     TextOutputDirSLE.SetText( String
          ( pAppOptions->text_output_template ) );

     GraphicalOutputDirSLE.SetText( String
          ( pAppOptions->graph_output_template ) );

     ExternalDataFileDirSLE.SetText( String
          ( pAppOptions->external_file_template ) );

     ExternalTempProDirSLE.SetText( String
          ( pAppOptions->external_t_profile_template ) );

     SaveWindowDataCB.Check
     ( pAppOptions->save_window_positions );

     ConfirmFileReplaceCB.Check
     ( pAppOptions->confirm_on_overwrite );

     return;
}



void AppOptionsDlg::CloseHandler( PushButton PTR )
{

     // Store the contents of the entries in the dialog box into the
     // data structure so that the user
     // can retrieve them at later time.  End the dialog box.

     msimStringCopy( msimAppOptions.data_template, DataDirSLE.GetText( ),
          sizeof msimAppOptions.data_template );

     msimStringCopy( msimAppOptions.text_output_template,
          TextOutputDirSLE.GetText( ), sizeof msimAppOptions.text_output_template );
     msimStringCopy( msimAppOptions.graph_output_template,
          GraphicalOutputDirSLE.GetText( ), sizeof msimAppOptions.graph_output_template );
     msimStringCopy( msimAppOptions.external_file_template,
          ExternalDataFileDirSLE.GetText( ), sizeof msimAppOptions.external_file_template );
     msimStringCopy( msimAppOptions.external_t_profile_template,
          ExternalTempProDirSLE.GetText( ), sizeof msimAppOptions.external_t_profile_template );

     msimAppOptions.save_window_positions = SaveWindowDataCB.IsChecked( );
     msimAppOptions.confirm_on_overwrite = ConfirmFileReplaceCB.IsChecked( );

     EndDialog( );

     return;
}

void AppOptionsDlg::UndoHandler( PushButton PTR )
{
     InitializeDialog( &msimAppOptions );
}

void AppOptionsDlg::DefaultsHandler( PushButton PTR )
{
     //    Obtain the default state.

     InitializeDialog( &msimC_APP_OPTIONS );

     return;
}


void msimAppOptionDialog( msimWID Owner )
{
     //  create the dialog box and execute it

     AppOptionsDlg PTR pdlg = new AppOptionsDlg ( Owner );

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