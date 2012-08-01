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
/*  FILE NAME : msimnb.cxx                                                  */
/*                                                                          */
/*  This module implements a dialog for viewing and editing a user          */
/*  notebook for entering arbitrary text associated with a rxn file         */
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

class NotebookDlg : public ModelessDialog
{
protected :

     MultiLineEdit NotebookMLE;
     OKButton ClosePB;
     PushButton UndoPB;
     PushButton ClearPB;
     HelpButton HelpPB;

     void ReadMLE( );

     msimPINSTANCE pSimInstance;
     msimWID owner;
     USHORT notebook_altered;

public :
     NotebookDlg( Window PTR pParent, msimPINSTANCE PSimInstance, const ResId& rResId );

     void CloseHandler( PushButton PTR pButton );
     void ClearHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void SetModiedFlagHandler( Edit PTR pEdit );

     friend VOID msimReadNotebookMLE( msimPINSTANCE Instance );
};



// Constructor of NotebookDlg
NotebookDlg::NotebookDlg( Window PTR pParent,
     msimPINSTANCE
     PSimInstance,
     const ResId& rResId ) :
ModelessDialog ( NULL, rResId),
NotebookMLE ( this, ResId ( msimNOTEBOOK_ENTRY ) ),
ClosePB ( this, ResId ( msimNOTEBOOK_CLOSE ) ),
UndoPB ( this, ResId ( msimNOTEBOOK_UNDO ) ),
ClearPB ( this, ResId ( msimNOTEBOOK_CLEAR ) ),
HelpPB ( this, ResId ( msimNOTEBOOK_HELP ) )
{
     FreeResource( );
     pSimInstance = PSimInstance;
     owner = pParent;
     notebook_altered = FALSE;

     // Set the title of this dialog

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize the notebook

     NotebookMLE.SetText( String
          ( pSimInstance->ptr_to_notebook_text ) );

     // Set up the handlers of various tasks

     ClosePB.ChangeClickHdl( LINK( this,
               NotebookDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl( LINK( this,
               NotebookDlg, UndoHandler ) );
     ClearPB.ChangeClickHdl( LINK( this,
               NotebookDlg, ClearHandler ) );

     NotebookMLE.ChangeModifyHdl( LINK( this,
               NotebookDlg, SetModiedFlagHandler ) );
     msimCascadeWindowOnOwner( this, pParent );

     ClosePB.GrabFocus( );

     Show( );
}




// this fcn reworked 2/28/94 to fix bug in notebook text - not terminated correctly upon
// modification so that stary chars are picked up, or chars are dropped (WDH)

void NotebookDlg::ReadMLE( )
{
     USHORT textsize;
     PCHAR temp_ptr;

     if ( notebook_altered )
     {
          pSimInstance->notebook_altered = TRUE;

          // obtain the size of the text in the editor

          textsize = NotebookMLE.GetText( ) .Len( );

          if ( textsize >= msimNOTEBOOK_SIZE )
               textsize = msimNOTEBOOK_SIZE - 1;

          if ( textsize )
          {
               // if we have text, then allocate new memory based on the new size of text
               // be sure to add one for the terminating zero

               temp_ptr = new CHAR[textsize + 1];

               if ( ! temp_ptr )
               {
                    msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                         __TIMESTAMP__, __LINE__, this );

                    return;
               }

               // free memory used for the old notebook buffer
               // update the contents of the notebook and the
               // data structure

               // the size we Allocated includes room for terminating zero
               // so allow for that in the size parameter of msimStringCopy

               msimStringCopy( temp_ptr, NotebookMLE.GetText( ), textsize + 1 );

               if ( pSimInstance->ptr_to_notebook_text )
                    delete pSimInstance->ptr_to_notebook_text;

               pSimInstance->ptr_to_notebook_text = temp_ptr;

               // note that value of notebook_size does not include trailing zero

               pSimInstance->notebook_size = textsize;

          }
          else
          {
               // the editor is empty, free all the memory allocated and
               // update the data structure acordingly.

               if ( pSimInstance->ptr_to_notebook_text )
                    delete pSimInstance->ptr_to_notebook_text;

               pSimInstance->ptr_to_notebook_text = NULL;
               pSimInstance->notebook_size = 0;
          }

     }

}




void NotebookDlg::CloseHandler( PushButton PTR )
{
     // If nothing has been changed simply end the dialog, if the notebook has
     // been changed then make the necessary changes

     Hide( );

     ReadMLE( );

     if ( notebook_altered )
          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     // end the dialog box.

     pSimInstance->p_notebook_window = msimWID_INVALID;
     delete this;

     return;
}

void NotebookDlg::UndoHandler( PushButton PTR )
{
     // Undo all the changes made to the note book

     NotebookMLE.SetText( String
          ( pSimInstance->ptr_to_notebook_text ) );
     notebook_altered = FALSE;
     return;
}

void NotebookDlg::ClearHandler( PushButton PTR )
{
     // Clear the Note book editing area

     NotebookMLE.SetText( String( msimNULL_STR ) );
     notebook_altered = TRUE;
     return;
}


void NotebookDlg::SetModiedFlagHandler( Edit PTR )
{
     notebook_altered = TRUE;
     return;
}

/*--------------------------------------------------------------------------*/
/*                    msimNotebookDialog( )                                 */
/*..........................................................................*/
/*                                                                          */
/* creates the notebook window        -- called from main menu loop         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

VOID msimNotebookDialog( msimWID Owner, msimPINSTANCE Instance )
{

     if ( ! Instance->p_notebook_window )
     {
          Instance->p_notebook_window = new
          NotebookDlg ( Owner,
          Instance, ResId( msimNOTEBOOK_PANEL ) );

          // successful creation of object ??

          if ( ! Instance->p_notebook_window )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );
               return;
          }
     }
     else
          // if the window already exists, show it in normal size

          Instance->p_notebook_window->Show( );
     Instance->p_notebook_window->ToTop( );

     return;
}




VOID msimReadNotebookMLE( msimPINSTANCE Instance )
{
     // note that this fcn does a cast of p_notebook_window PTR

     if ( Instance->p_notebook_window != msimWID_INVALID )
          ( ( NotebookDlg PTR ) ( Instance->p_notebook_window ) ) ->ReadMLE( );
}




VOID msimCloseNotebookWindow( msimPINSTANCE Instance )
{
     // note that this fcn does a cast of p_notebook_window PTR

     if ( Instance->p_notebook_window != msimWID_INVALID )
          ( ( NotebookDlg PTR ) ( Instance->p_notebook_window ) ) ->CloseHandler( NULL );
}

