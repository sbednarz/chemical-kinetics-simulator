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
//  EDITTITL.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This modules defines the EditTitle dialog used in the plot routines
//  the VSIM user interface.
//
//  CREATED : 8.21.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "edittitl.hxx"
#include "plottitl.hxx"
#include "mainapp.hxx"

#endif

#include "edittitl.hrc"

// ----------------------------------------------------------------------------
//                 class EditTitleDialog
// ............................................................................
//
//   class declaration
//
// ----------------------------------------------------------------------------

class EditTitleDialog : public ModalDialog
{
protected:
     OKButton     close_btn;
     HelpButton   help_btn;
     PushButton   undo_btn;
     Edit         title_ef;
     GroupBox     group_box;

     PlotTitle*   ptitle_obj;
     BOOL8        modified;

public:
                   EditTitleDialog(  PlotTitle* pTitleObject );

     void          UndoHandler( PushButton* );
     void          CloseHandler( OKButton* );
     void          SetModifiedFlagHandler( Edit* );

};





// ----------------------------------------------------------------------------
//                 EditTitleDialog::EditTitleDialog()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

EditTitleDialog::EditTitleDialog( PlotTitle * pTitleObject ) :
ModalDialog( (Window *) pTitleObject->GetpParent(), ResId( v_ID_DLG_EDIT_TITLE  ) ),
ptitle_obj( pTitleObject ),
modified( FALSE ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
title_ef( this, ResId( vID_EF_TITLE_TEXT ) ),
group_box( this, ResId( 1 ) )
{
     FreeResource();

     title_ef.SetText( ptitle_obj->GetTitle() );

     title_ef.ChangeModifyHdl( LINK( this, EditTitleDialog, SetModifiedFlagHandler ) );

     close_btn.ChangeClickHdl( LINK( this, EditTitleDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, EditTitleDialog, UndoHandler ) );

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );

}




// ----------------------------------------------------------------------------
//                 EditTitleDialog::UndoHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditTitleDialog::UndoHandler( PushButton* )
{
     modified = FALSE;
     title_ef.SetText( ptitle_obj->GetTitle() );
     undo_btn.Disable();
}






// ----------------------------------------------------------------------------
//                 EditTitleDialog::CloseHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditTitleDialog::CloseHandler( OKButton* )
{
     if ( modified )
     {
          ptitle_obj->SetTitle( title_ef.GetText() );
          EndDialog( TRUE );
     }
     else
          EndDialog( FALSE );
}






// ----------------------------------------------------------------------------
//                 EditTitleDialog::SetModifiedFlagHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditTitleDialog::SetModifiedFlagHandler( Edit* )
{
     if ( ! modified )
     {
          modified = TRUE;
          undo_btn.Enable();
     }
}





BOOL8 EditTitle( PlotTitle* pTitleObject )
{

     EditTitleDialog* pdlg = new EditTitleDialog( pTitleObject );

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



