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
//  NOTEBOOK.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the Notebook edit dlg
//
//  CREATED : 5.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------
 


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "schemwin.hxx"
#include "notebook.hxx"
#include "mainapp.hxx"

#endif


#include "notebook.hrc"




// ----------------------------------------------------------------------------
//             NotebookDialog::NotebookDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
NotebookDialog::NotebookDialog( SchemeFrameWindow * pParent )
: ModelessDialog( pParent, ResId(vID_DLG_NOTEBOOK) ),
text_mle( this, ResId( vID_MLE_NOTEBOOK ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
clear_btn( this, ResId( vID_BTN_CLEAR ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
p_parent( pParent ),
data_altered( FALSE )
{
    FreeResource();
    SetText( GetText() + pParent->GetReactionScheme().GetNickName() );

    text_mle.SetText( GetpParent()->GetReactionScheme().GetNotebookText() );

     close_btn.ChangeClickHdl( LINK( this, NotebookDialog, CloseBtnHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, NotebookDialog, UndoBtnHandler ) );
     clear_btn.ChangeClickHdl( LINK( this, NotebookDialog, ClearBtnHandler ) );

     text_mle.ChangeModifyHdl( LINK( this, NotebookDialog, TextModifiedHandler ) );
     
     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );

}







// ----------------------------------------------------------------------------
//             NotebookDialog::CloseDialog( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
void NotebookDialog::CloseDialog(  )
{
     if ( IsDataAltered() )
         GetpParent()->GetReactionScheme().SetNotebookText( text_mle.GetText( ) );


     GetpParent()->SetpNotebookDlg( 0 );
     delete this;
}




// ----------------------------------------------------------------------------
//             NotebookDialog::CloseBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
void NotebookDialog::CloseBtnHandler( OKButton* )
{
     CloseDialog( );
}






// ----------------------------------------------------------------------------
//             NotebookDialog::ClearBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
void NotebookDialog::ClearBtnHandler( PushButton* )
{
     text_mle.SetText( String() );
     SetDataAltered( TRUE );
     undo_btn.Enable();
}





// ----------------------------------------------------------------------------
//             NotebookDialog::UndoBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
void NotebookDialog::UndoBtnHandler( PushButton* )
{
     text_mle.SetText( GetpParent()->GetReactionScheme().GetNotebookText() );
     SetDataAltered( FALSE );
     undo_btn.Disable();
}






// ----------------------------------------------------------------------------
//             NotebookDialog::TextModifiedHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
void NotebookDialog::TextModifiedHandler( Edit* )
{
     if ( !IsDataAltered() ) 
     {
          SetDataAltered( TRUE );
          undo_btn.Enable();
     } 
}





// ----------------------------------------------------------------------------
//             EditNotebook( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------
 
NotebookDialog* EditNotebook( SchemeFrameWindow* pParent )
{
     // first check whether parent already has a notebook dlg
     // if so then show the exitsing dialog and return 

     NotebookDialog* pdlg = pParent->GetpNotebookDlg();

     if ( pdlg )
     {
          pdlg->Show( );
          pdlg->ToTop( );
     }
     else
     {
          // otherwise attempt to create new dialog 

          NotebookDialog* pdlg = new NotebookDialog( pParent );

          if ( pdlg )
                pdlg->Show();
          else
               GetpMainApp()->OutOfMemory();
     }

     return pdlg;
}



