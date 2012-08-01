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
//  USERPREF.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the EditPreferencesDialog function
//
//  CREATED : 10.23.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "userpref.hxx"
#include "mainapp.hxx"

#endif


#include "userpref.hrc"
#include "help.hhc"








// ----------------------------------------------------------------------------
//                            class UserPrefsDialog 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class UserPrefsDialog : public ModalDialog
{
protected:
     Edit            rxn_file_dir;
     Edit            text_file_dir; 
     Edit            any_file_dir; 
     Edit            rate_prog_file_dir;
     Edit            temp_prog_file_dir;
     CheckBox        save_win_data_cb;
     CheckBox        confirm_overwrite_cb;
     OKButton        close_btn;
     PushButton      undo_btn;
     PushButton      defaults_btn;
     HelpButton      help_btn;
     FixedText       text1;
     FixedText       text2;
     FixedText       text3;
     FixedText       text4;
     FixedText       text5;
     GroupBox        box1;
     GroupBox        box3;

     void            InitializeDialog();
public:
     UserPrefsDialog( Window * pParent );

     void         CloseHandler( OKButton * );
     void         UndoHandler( PushButton * );
     void         DefaultsHandler( PushButton * );

};








// ----------------------------------------------------------------------------
//                  UserPrefsDialog::UserPrefsDialog()
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

UserPrefsDialog::UserPrefsDialog( Window * pParent ) :
ModalDialog( pParent, ResId(USER_PREFS_DLG)  ),
rxn_file_dir( this, ResId( vID_RXN_FILE_DIR ) ),
text_file_dir( this, ResId( vID_TEXT_FILE_DIR ) ),
rate_prog_file_dir( this, ResId( vID_RPROG_FILE_DIR ) ),
temp_prog_file_dir( this, ResId( vID_TPROG_FILE_DIR ) ),
any_file_dir( this, ResId( vID_ANY_FILE_DIR ) ),
save_win_data_cb( this, ResId( vID_CB_SAVE_WINDOW_DATA ) ),
confirm_overwrite_cb( this, ResId( vID_CB_CONFIRM_FILE_OVERWRITE ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
text1( this, ResId( 1 ) ),
text2( this, ResId( 2 ) ),
text3( this, ResId( 3 ) ),
text4( this, ResId( 4 ) ),
text5( this, ResId( 5 ) ),
box1( this, ResId( 1 ) ),
box3( this, ResId( 3 ) )
{
     FreeResource();

     close_btn.ChangeClickHdl( LINK( this, UserPrefsDialog, CloseHandler ) ); 
     undo_btn.ChangeClickHdl( LINK( this, UserPrefsDialog, UndoHandler ) );  
     defaults_btn.ChangeClickHdl( LINK( this, UserPrefsDialog, DefaultsHandler ) );  

     InitializeDialog();

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}





// ----------------------------------------------------------------------------
//                  UserPrefsDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UserPrefsDialog::CloseHandler( OKButton * )
{
     GetpMainApp()->SetDefaultRxnFilePath( rxn_file_dir.GetText() );
     GetpMainApp()->SetDefaultTxtFilePath( text_file_dir.GetText() ); 
     GetpMainApp()->SetDefaultAnyFilePath( any_file_dir.GetText() );
     GetpMainApp()->SetDefaultRatFilePath( rate_prog_file_dir.GetText() );
     GetpMainApp()->SetDefaultPrfFilePath( temp_prog_file_dir.GetText() );

     GetpMainApp()->SetSaveWindowPositionData( save_win_data_cb.IsChecked() );
     GetpMainApp()->SetConfirmBeforeOverwrite( confirm_overwrite_cb.IsChecked() );

     EndDialog();
}





// ----------------------------------------------------------------------------
//                  UserPrefsDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UserPrefsDialog::InitializeDialog( )
{
     rxn_file_dir.SetText( GetpMainApp()->GetDefaultRxnFilePath() );
     text_file_dir.SetText(GetpMainApp()->GetDefaultTxtFilePath()); 
     any_file_dir.SetText(GetpMainApp()->GetDefaultAnyFilePath());
     rate_prog_file_dir.SetText(GetpMainApp()->GetDefaultRatFilePath()); 
     temp_prog_file_dir.SetText(GetpMainApp()->GetDefaultPrfFilePath()); 

     save_win_data_cb.Check( GetpMainApp()->SaveWindowPositionData() );
     confirm_overwrite_cb.Check( GetpMainApp()->ConfirmBeforeOverwrite() );
}




// ----------------------------------------------------------------------------
//                  UserPrefsDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UserPrefsDialog::UndoHandler( PushButton * )
{
     InitializeDialog();
}







// ----------------------------------------------------------------------------
//                  UserPrefsDialog::DefaultsHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void UserPrefsDialog::DefaultsHandler( PushButton * )
{
     String str;
     DirEntry base_app( DirEntry( GetpMainApp()->GetAppFileName() ).GetPath() );
     
     // do rxn file path first 

     DirEntry base_path( base_app.GetPath() );
     
     base_path += DirEntry( String( ResId( vID_STR_DEFAULT_RXN_SUBDIR ) ) );
     
     if ( base_path.Exists() )
          str = base_path.GetFull();
     else
          str = base_app.GetPath().GetFull();

     rxn_file_dir.SetText( str );

     // now repeat to make txt file path 

     base_path = base_app.GetPath();
     
     base_path += DirEntry( String( ResId( vID_STR_DEFAULT_TXT_SUBDIR ) ) );
     
     if ( base_path.Exists() )
          str = base_path.GetFull();
     else
          str = base_app.GetPath().GetFull();

     text_file_dir.SetText(str); 

     // now repeat to make prf file path 

     base_path = base_app.GetPath();
     
     base_path += DirEntry( String( ResId( vID_STR_DEFAULT_PRF_SUBDIR ) ) );
     
     if ( base_path.Exists() )
          str = base_path.GetFull();
     else
          str = base_app.GetPath().GetFull();

     temp_prog_file_dir.SetText(str); 

     // now repeat to make rat file path 

     base_path = base_app.GetPath();
     
     base_path += DirEntry( String( ResId( vID_STR_DEFAULT_RAT_SUBDIR ) ) );
     
     if ( base_path.Exists() )
          str = base_path.GetFull();
     else
          str = base_app.GetPath().GetFull();

     rate_prog_file_dir.SetText(str); 

     // now the any file path 

     base_path = base_app.GetPath();
     
     base_path += DirEntry( String( ResId( vID_STR_DEFAULT_ANY_SUBDIR ) ) );
     
     if ( base_path.Exists() )
          str = base_path.GetFull();
     else
          str = base_app.GetPath().GetFull();
     any_file_dir.SetText( str );

     // finally the check boxes 

     save_win_data_cb.Check( TRUE );
     confirm_overwrite_cb.Check( TRUE );
}









// ----------------------------------------------------------------------------
//                               EditPreferencesDialog( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void EditPreferencesDialog( Window* pParent )
{
     UserPrefsDialog* pdlg = new UserPrefsDialog( pParent );

     if ( pdlg )
     {
          pdlg->Execute();
          delete pdlg;
          return;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return;
     }
}









