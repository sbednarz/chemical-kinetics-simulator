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
/*  FILE NAME : msimmain.cxx                                                */
/*                                                                          */
/*  The main module for the MSIM chemical reaction simulation program       */
/*  Translated to use the Starview Class libraries to provide multi-        */
/*  platform support                                                        */
/*                                                                          */
/*  Version 1.0  started Aug 10, 1993                                       */
/*                                                                          */
/*  CHANGE HISTORY : 5.24.95 modified to use bitmap in about dlg (wdh)      */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimfile.hxx"
#include "msimque.hxx"
#include "msimplot.hxx"

#include <string.h>
#include <stdlib.h>

#define  BORDER                                  10
#define  NUM_RXN_STR                             "Number of Reactions : %u"
#define  NUM_SPECIES_STR                         "Number of Species : %u"
#define  FILENAME_STR                            "Active File (%d) : %s"



// this is to keep track of which window the mouse is over. That way we do not update the
// help status line if the mouse pointer is just moving around on top of the same
// window, which causes the text to flicker. This variable is referenced in the
// MouseMove functions below

static Window PTR current_mouse_pos = NULL;



// local fcn declarations

static BOOL OKToExit( );





// --- declaration of class CreditsDialog ----------------------------------

class CreditsDialog : public ModalDialog
{
protected :
    FixedBitmap aFixedBitmap1;
    OKButton aOKButton1;

public :
     CreditsDialog( Window PTR pParent );
};



CreditsDialog::CreditsDialog( Window PTR pParent ) :
ModalDialog ( pParent, ResId ( msimCREDITS_DLG ) ),
aOKButton1 ( this, ResId ( msimCREDITS_OK ) ),
aFixedBitmap1 ( this, ResId ( 1 ) )
{
     FreeResource( );
     return;
}



// --- declaration of class AboutDialog ----------------------------------

class AboutDialog : public ModalDialog
{
protected :
    FixedBitmap aFixedBitmap1;
    OKButton aOKButton1;
    PushButton CreditsButton;

public :
     AboutDialog( Window PTR pParent );
     void ShowCreditsHandler( PushButton PTR pButton );
};



AboutDialog::AboutDialog( Window PTR pParent ) :
ModalDialog ( pParent, ResId ( msimABOUT_DLG ) ),
aOKButton1 ( this, ResId ( msimABOUT_OK ) ),
CreditsButton( this, ResId( msimABOUT_CREDITS ) ),
aFixedBitmap1 ( this, ResId ( 1 ) )
{
     FreeResource( );

     CreditsButton.ChangeClickHdl( LINK( this, AboutDialog, ShowCreditsHandler ) );

     return;
}


void AboutDialog::ShowCreditsHandler( PushButton PTR pButton )
{
     CreditsDialog dlg( this );

     msimCenterDialogWindowOnOwner( &dlg, this );

     dlg.Execute( );
}






// constructors

msimPushButton::msimPushButton( Window PTR pParent, const ResId& rResId ) :
PushButton ( pParent, rResId)
{
}

msimAddPushButton::msimAddPushButton( Window PTR pParent, const ResId& rResId ) :
msimPushButton ( pParent, rResId)
{
}

msimDeletePushButton::msimDeletePushButton( Window PTR pParent, const ResId& rResId ) :
msimPushButton ( pParent, rResId)
{
}

msimEditPushButton::msimEditPushButton( Window PTR pParent, const ResId& rResId ) :
msimPushButton ( pParent, rResId)
{
}


msimRxnListBox::msimRxnListBox( Window PTR pParent, const ResId& rResId ) :
ListBox ( pParent, rResId)
{
}

void msimRxnListBox::KeyInput( const KeyEvent& rKeyEvt )
{
     aMainApp.GetAppWindow( ) ->HandOffKeyInput( rKeyEvt );
}



MainApp::MainApp( ) : Application ( )
{
}


LogoWindow PTR MainApp::ShowLogo( )
{
     // shows logo window for a timed period
     return new LogoWindow();
}


// --- Destructor MainWindow::MainWindow() ----

MainWindow::~MainWindow( )
{
     if ( rxn_cut_buffer )
          delete rxn_cut_buffer;
}

// --- constructor MainWindow::MainWindow() ----

MainWindow::MainWindow( ) :
WorkWindow ( NULL, WB_APP | WB_STDWORK | WB_SVLOOK ),
aActiveFileName ( this),
aActiveFileStatus ( this),
aNumReactionsStatus ( this),
aNumSpeciesStatus ( this),
aFileStatsGB ( this, ResId ( msimMAINWIN_GROUPBOX ) ),
aAddReactionPB ( this, ResId ( msimMAINWIN_ADD_RXN ) ),
aEditReactionPB ( this, ResId ( msimMAINWIN_EDIT_RXN ) ),
aDeleteReactionPB ( this, ResId ( msimMAINWIN_DELETE_RXN ) ),
aHelpStatus ( this, WB_BORDER | WB_SVLOOK ),
aRxnStepLB ( this, ResId ( msimMAINWIN_RXNLIST ) )
{
     ChangeIcon( ResId( msimMAIN_ICON ) );
     rxn_cut_buffer = NULL;

     /* set main window title                                                */

     SetText( ResId( msimMAIN_WINDOW_TITLE ) );
     SetHelpText( String( ResId( msimMAIN_MENU_HELP_STR ) ) );

     aActiveFileName.Show( );
     aActiveFileStatus.Show( );
     aNumReactionsStatus.Show( );
     aNumSpeciesStatus.Show( );
     aFileStatsGB.Show( );
     aAddReactionPB.Show( );
     aEditReactionPB.Show( );
     aDeleteReactionPB.Show( );
     aHelpStatus.Show( );
     aRxnStepLB.Show( );

     /* add callbacks for pushbuttons                                        */

     aAddReactionPB.ChangeClickHdl( LINK( this, MainWindow, AddRxnStepHandler ) );
     aEditReactionPB.ChangeClickHdl( LINK( this, MainWindow, EditRxnStepHandler ) );
     aDeleteReactionPB.ChangeClickHdl( LINK( this, MainWindow, DeleteRxnStepHandler ) );
     aRxnStepLB.ChangeSelectHdl( LINK( this, MainWindow, ListboxSelectHandler ) );
     aRxnStepLB.ChangeDoubleClickHdl( LINK( this, MainWindow, ListboxDoubleClickHandler ) );

     aHelpStatus.SetText( String( ResId( msimMAIN_MENU_HELP_STR ) ) );
     aHelpStatus.ChangeHelpId( msimMAIN_MENU_HELP_STR );

}

#if defined(__MAC__)

void     EnableAppMenu( BOOL State )
{
     MenuBar PTR pmenu = aMainApp.GetAppMenu ( );

     pmenu->EnableItem( msimFILE_MENU, State );
     pmenu->EnableItem( msimDATA_ENTRY_MENU, State );
     pmenu->EnableItem( msimSELECT_MENU, State );
     pmenu->EnableItem( msimSIMULATION_MENU, State );
     pmenu->EnableItem( msimSHOW_RESULTS_MENU, State );
     pmenu->EnableItem( msimHELP_MENU, State );
}

#endif


void MainWindow::KeyInput( const KeyEvent& rKeyEvt )
{

     if ( ! rKeyEvt.GetKeyCode( ) .IsMod2( ) )
     {

          // the Alt key is not pressed

          if ( aRxnStepLB.IsEnabled( ) )
          {
               switch ( rKeyEvt.GetKeyCode( ) .GetCode( ) )
               {
               case KEY_UP :

                    if ( aMainApp.GetActiveInstance( ) ->listbox_selection > 0 )
                    {
                         aRxnStepLB.SelectEntryPos(
                              -- ( aMainApp.GetActiveInstance( ) ->listbox_selection )
                         );
                    }

                    break;

               case KEY_DOWN :

                    if ( aMainApp.GetActiveInstance( ) ->listbox_selection <
                              ( aMainApp.GetActiveInstance( ) ->numsteps - 1 )
                         )
                    {
                         aRxnStepLB.SelectEntryPos(
                              ++ ( aMainApp.GetActiveInstance( ) ->listbox_selection )
                         );
                    }

                    break;

               case KEY_RETURN :

                    ListboxDoubleClickHandler( NULL );

                    break;

               default :

                    WorkWindow::KeyInput( rKeyEvt );

                    break;
               }
          }
          else
               WorkWindow::KeyInput( rKeyEvt );
     }
     else
     {
          // the Alt key is pressed

          switch ( rKeyEvt.GetKeyCode( ) .GetCode( ) )
          {
          case KEY_A :

               if ( aAddReactionPB.IsEnabled( ) )
                    AddRxnStepHandler( NULL );
               break;

          case KEY_I :

               if ( aEditReactionPB.IsEnabled( ) )
                    msimRxnDataEntryDialog( this, aMainApp.GetActiveInstance( ) );
               break;

          case KEY_D :

               if ( aDeleteReactionPB.IsEnabled( ) )
                    DeleteRxnStepHandler( NULL );
               break;

          default :
               WorkWindow::KeyInput( rKeyEvt );

               break;
          }
     }

     return;
}



void MainWindow::AddRxnStepHandler( PushButton PTR )
{
     if ( ! msimOKToInvalidateRxnData( this, aMainApp.GetActiveInstance( ) ) )
          return;

     msimPRXN rxnptr = msimGetRxnPtrFromLineNumber
     ( aMainApp.GetActiveInstance( ) ->listbox_selection,
          aMainApp.GetActiveInstance( ) ->ptr_to_rxnlist );

     if ( NULL != AddReactionToListBox( rxnptr, aMainApp.GetActiveInstance( ) ) )
          msimRxnDataEntryDialog( this, aMainApp.GetActiveInstance( ) );

     return;
}



void MainWindow::ListboxDoubleClickHandler( ListBox PTR )
{
     if ( aMainApp.GetActiveInstance( ) )
          msimRxnDataEntryDialog( this, aMainApp.GetActiveInstance( ) );

     return;
}



void MainWindow::ListboxSelectHandler( ListBox PTR pListBox )
{
     aMainApp.GetActiveInstance( ) ->listbox_selection = pListBox->GetSelectEntryPos
     ( );

     return;
}



void MainWindow::EditRxnStepHandler( PushButton PTR )
{
     msimRxnDataEntryDialog( this, aMainApp.GetActiveInstance( ) );

     return;

}




void MainWindow::CutSelectedRxnStep( )
{
     if ( ! msimOKToInvalidateRxnData( this, aMainApp.GetActiveInstance( ) ) )
          return;

     ProcessSelectedRxnStep( );

     DeleteRxnStepHandler( NULL );

     return;
}


void MainWindow::CopySelectedRxnStep( )
{
     ProcessSelectedRxnStep( );

     UpdateMainWinData( FALSE );       // activates menu item if necessary

     return;
}


void MainWindow::PasteRxnStep( )
{
     msimPINSTANCE Instance;
     msimPRXN rxnptr, newrxnptr, tmp;
     USHORT error_position;


     if ( ! rxn_cut_buffer )
          return;

     Instance = aMainApp.GetActiveInstance( );

     if ( ! msimOKToInvalidateRxnData( this, Instance ) )
          return;

     // obtain the selected reaction steps

     rxnptr = msimGetRxnPtrFromLineNumber
     ( Instance->listbox_selection, Instance->ptr_to_rxnlist );

     // allocate memory for the new reaction step

     if ( NULL == ( newrxnptr =
                    aMainApp.GetAppWindow( ) ->AddReactionToListBox( rxnptr, Instance ) ) )
          return;

     // copy all the information regarding the reaction step //
     // from a temporary buffer to the permanent structure   //

     TransferSelectedRxnData( newrxnptr, rxn_cut_buffer );

     // analyze reaction scheme                                     //

     if ( msimNO_ERROR != msimAnalyzeRxnScheme( Instance, &tmp, &error_position, this ) )
     {
          // select the erroneous eqn in main rxn list              //

          aRxnStepLB.SelectEntryPos( error_position );

          // go into edit mode on the offending equation            //
          aMainApp.PostUserEvent( msimUSR_EVT_EDIT_RXN, NULL );
     }

     // update the dialog panel //



     UpdateMainWinData( TRUE );

#if defined (__MAC__)
     aRxnStepLB.Invalidate( ); // immed update fix problem w/MAC
#endif

     return;
}


void MainWindow::ProcessSelectedRxnStep( )
{
     msimPRXN rxnptr;

     // allocate memory for rxn buffer if rxn_cut_buffer == NULL //

     if ( ! rxn_cut_buffer )
          rxn_cut_buffer = new msimRXN;

     if ( ! rxn_cut_buffer )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, this );
          return;
     }
     else
          // set ptrs to NULL to be safe
          rxn_cut_buffer->next = rxn_cut_buffer->prev = NULL;

     // obtain ptr to the selected reaction step

     rxnptr = msimGetRxnPtrFromLineNumber
     ( aMainApp.GetActiveInstance( ) ->listbox_selection,
          aMainApp.GetActiveInstance( ) ->ptr_to_rxnlist );

     // copy selected rxn data to rxn_cut_buffer

     TransferSelectedRxnData( rxn_cut_buffer, rxnptr );

     return;
}


void MainWindow::TransferSelectedRxnData( msimPRXN ToRxnptr, msimPRXN FromRxnptr )
{
     msimPRXN save_next, save_prev;

     // copy selected rxn data to ToRxnptr

     save_next = ToRxnptr->next;       // preserve values in the event we
     save_prev = ToRxnptr->prev;     // are copying to a member in a linked list

     *ToRxnptr = *FromRxnptr;

     ToRxnptr->next = save_next;       // restore ptrs
     ToRxnptr->prev = save_prev;

     return;
}



void MainWindow::DeleteRxnStepHandler( PushButton PTR )
{
     if ( ! msimOKToInvalidateRxnData( this, aMainApp.GetActiveInstance( ) ) )
          return;

     msimPRXN rxnptr = msimGetRxnPtrFromLineNumber
     ( aMainApp.GetActiveInstance( ) ->listbox_selection,
          aMainApp.GetActiveInstance( ) ->ptr_to_rxnlist );

     aMainApp.GetAppWindow( ) ->DeleteReactionFromListBox( rxnptr,
          aMainApp.GetActiveInstance( ) ->listbox_selection, aMainApp.GetActiveInstance( ) );

     return;
}


void msimPushButton::MouseMove( const MouseEvent & )
{

     if ( current_mouse_pos == this )
          return;

     if ( IsEnabled( ) )
          aMainApp.GetAppWindow( ) ->GetStatus( ) .SetText( GetHelpText( ) );
     else
          aMainApp.GetAppWindow( ) ->GetStatus( ) .SetText( aMainApp.GetAppWindow( ) ->GetHelpText( ) );

     current_mouse_pos = this;
}



void MainWindow::MouseMove( const MouseEvent & )
{
     if ( current_mouse_pos == this )
          return;

     aHelpStatus.SetText( GetHelpText( ) );

     current_mouse_pos = this;
}



void msimRxnListBox::MouseMove( const MouseEvent & )
{
      if ( current_mouse_pos == this )
          return;

    if ( IsEnabled( ) )
          aMainApp.GetAppWindow( ) ->GetStatus( ) .SetText( GetHelpText( ) );
     else
          aMainApp.GetAppWindow( ) ->GetStatus( ) .SetText( aMainApp.GetAppWindow( ) ->GetHelpText( ) );

     current_mouse_pos = this;
}



BOOL MainWindow::Close( )
{
     // give user a chance to abort
     if ( OKToExit( ) )
     {
          aMainApp.Quit( );
          return TRUE;
     }
     else
          return FALSE;
}



void MainWindow::Resize( )
{

     /* get the current window size                                          */

     SHORT window_width = GetOutputSize ( ) .Width ( );
     SHORT window_height = GetOutputSize ( ) .Height ( );

     Size pb_size = aAddReactionPB.GetSizePixel( );
     SHORT button_height = pb_size.Height ( );
     SHORT button_width = pb_size.Width ( );

     SHORT char_ht = ( 11 * aMainApp.GetAppFont( ) .GetSize( ) .Height( ) ) / 10;

#if defined (__MAC__)

     // fix for problem with MAC getting char ht. Since button_ht is currently
      // set to 12 font units ht, and there 1 font units == 1/8 actual char ht
      // then the value calculated below gives the same result
     // char_ht == button_height * 8/12 * 11/10

      // 104 rather then 88 as multiplier

     if ( char_ht == 0 )
           char_ht = ( button_height *104 ) / 120;

#endif

     SHORT x, y, width, height;
     SHORT lb_y;

     /* now lay out all the controls based on that                           */

     x = BORDER;
     y = BORDER;

     aFileStatsGB.ChangePosPixel( Point( x, y ) );

     y = ( 2 * BORDER + char_ht );
     x = 2 * BORDER;

     width = window_width - 4 * BORDER;
     height = char_ht;

     // static text is positioned relative to parent groupbox

     aActiveFileName.ChangeSizePixel( Size( width, height ) );
     aActiveFileName.ChangePosPixel( Point( x, y ) );

     y += ( ( 30 * char_ht ) / 20 );
     aActiveFileStatus.ChangeSizePixel( Size( width, height ) );
     aActiveFileStatus.ChangePosPixel( Point( x, y ) );

     width = window_width / 2 - ( 50 * BORDER ) / 20;

     y += ( ( 30 * char_ht ) / 20 );

     aNumReactionsStatus.ChangeSizePixel( Size( width, height ) );
     aNumReactionsStatus.ChangePosPixel( Point( x, y ) );

     x = ( window_width ) / 2 - ( 10 * BORDER ) / 20;
     aNumSpeciesStatus.ChangeSizePixel( Size( width, height ) );
     aNumSpeciesStatus.ChangePosPixel( Point( x, y ) );

     y += ( ( 30 * char_ht ) / 20 );

     aFileStatsGB.ChangeSizePixel( Size( window_width - 2 * BORDER, y ) );

     /* we let the listbox adjust larger or smaller as the window is         */
     /* resized                                                              */
     /* we set the  listbox position now, the dimensions later               */

     lb_y = y + 2 * BORDER;

     aRxnStepLB.ChangePosPixel( Point( BORDER, lb_y ) );

     /* now work up from bottom starting with                                */
     /* the help status bar: at bottom of window and across whole window width*/

     y = window_height - 2 * char_ht;
     aHelpStatus.ChangeSizePixel( Size( window_width, 30 * char_ht / 20 ) );
     aHelpStatus.ChangePosPixel( Point( 0, y ) );

     /* the pushbutton positions  y = same for all                           */
     /* x varies with button - we do not change button size at all           */

     y -= ( button_height + BORDER );

     x = ( window_width - ( 3 * button_width ) - ( 4 * BORDER ) ) / 2;
     aAddReactionPB.ChangePosPixel( Point( x, y ) );

     x += ( button_width + ( 2 * BORDER ) );
     aEditReactionPB.ChangePosPixel( Point( x, y ) );

     x += ( button_width + ( 2 * BORDER ) );
     aDeleteReactionPB.ChangePosPixel( Point( x, y ) );

     /* now set the size of the listbox                                      */

     aRxnStepLB.ChangeSizePixel( Size( window_width - 2 * BORDER, y - BORDER - lb_y ) );

     Invalidate( );
}




// ---------- MainWindow::SelectMenuHdl() -----------
//
// The main loop for the Mainwindow pulldown menu.
// we come here whenever a menu item is selected


long MainWindow::SelectMenuHdl( Menu PTR pMenu )
{
     USHORT nItemId = pMenu->GetCurItemId ( );

     switch ( nItemId )
     {
     case msimRXNSCHEME_1 :
     case msimRXNSCHEME_2 :
     case msimRXNSCHEME_3 :
     case msimRXNSCHEME_4 :
     case msimRXNSCHEME_5 :
     case msimRXNSCHEME_6 :
     case msimRXNSCHEME_7 :
     case msimRXNSCHEME_8 :
     case msimRXNSCHEME_9 :

          aMainApp.ActivateRxnInstance( nItemId - 1 );
          break;

     case msimCUT_RXN_MENU_ITEM :

          CutSelectedRxnStep( );
          break;

     case msimCOPY_RXN_MENU_ITEM :

          CopySelectedRxnStep( );
          break;

     case msimPASTE_RXN_MENU_ITEM :

          PasteRxnStep( );
          break;

     case msimCLOSE_MENU_ITEM :

          if ( aMainApp.GetActiveInstance( ) )
          {
               msimSimQueue.RemoveFromQueue( aMainApp.GetActiveInstance( ) );

               if ( msimClearMemory( aMainApp.GetActiveInstance( ), this, TRUE ) )
               {
                    delete aMainApp.GetActiveInstance( );
                    aMainApp.SetActiveInstance( NULL );
                    UpdateMainWinData( TRUE );
                    aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ),
                         NULL );

               }
          }
          break;

     case msimSAVE_MENU_ITEM :

          if ( aMainApp.GetActiveInstance( ) ->data_altered_since_lastsave )
               msimSaveRxnScheme( aMainApp.GetActiveInstance( ), this, msimCHECK_FOR_OVERWRITE );

          // if notebook is open read it - this sets notebook_altered flag
          // if text has been altered

          if ( aMainApp.GetActiveInstance( ) ->p_notebook_window != msimWID_INVALID )
               msimReadNotebookMLE( aMainApp.GetActiveInstance( ) );

          if ( aMainApp.GetActiveInstance( ) ->notebook_altered )
               msimSaveNotebookText( aMainApp.GetActiveInstance( ), this );

          UpdateMainWinData( FALSE );

          break;

     case msimSAVEAS_MENU_ITEM :

          msimSaveRxnSchemeAs( aMainApp.GetActiveInstance( ), this, msimSHOW_WARNING );
          aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ),
               aMainApp.GetActiveInstance( ) ->filename );

          break;

     case msimPRINT_MENU_ITEM :

          msimCreateTextSummary( aMainApp.GetActiveInstance( ), this );

          break;

     case msimGRAPHICAL_DISPLAY_MENU_ITEM :

          msimPlotDataDialog( NULL, aMainApp.GetActiveInstance( ) );

          break;

     case msimAPP_OPTION_MENU_ITEM :

          msimAppOptionDialog( this );
          break;

     case msimSIMULATION_OPTION_MENU_ITEM :

          msimSimulationOptionDialog( this, aMainApp.GetActiveInstance( ) );
          break;


     case msimNOTEBOOK_MENU_ITEM :

          msimNotebookDialog( this, aMainApp.GetActiveInstance( ) );

          break;

     case msimREACTION_SCHEME_MENU_ITEM :

          msimRxnDataEntryDialog( this, aMainApp.GetActiveInstance( ) );

          break;

     case msimRXN_CONDNS_MENU_ITEM :

          msimSetRxnConditions( this, aMainApp.GetActiveInstance( ) );
          break;

     case msimOVERVIEW :
     case msimTUTORIAL :
     case msimMAIN_WINDOW :
     case msimMAIN_MENU :

          pApp->GetHelp( ) ->Start(( ULONG ) nItemId );

          break;



     case msimHELP_ON_HELP :

          pApp->GetHelp( ) ->Start( HELP_HELPONHELP );

          break;

     case msimHELPINDEX_MENU_ITEM :

#if defined(__MAC__)
          pApp->GetHelp( ) ->Start( HELP_INDEX );
#else
          pApp->GetHelp( ) ->Start( msimSUBJECTS );
#endif
          break;

     case msimSPECIES_DATA_MENU_ITEM :

          msimSpeciesDataDialog( this, aMainApp.GetActiveInstance( ) );

          break;


     case msimHALT_SIMULATION_MENU_ITEM :

          msimAbortSimulation( this, aMainApp.GetActiveInstance( ) );
          break;

     case msimABOUT_MENU_ITEM :
     {
          AboutDialog aAboutDlg( this );

          msimCenterDialogWindowOnOwner( &aAboutDlg, this );

          aHelpStatus.SetText( String( ResId( msimABOUT_HELP_STR ) ) );
          aAboutDlg.Execute( );
          aHelpStatus.SetText( String( ResId( msimMAIN_MENU_HELP_STR ) ) );
     }

          break;

     case msimSTART_SIMULATION_MENU_ITEM :

          msimStartSimulation( this, aMainApp.GetActiveInstance( ), FALSE );

          break;

     case msimRESUME_SIMULATION_MENU_ITEM :

          msimStartSimulation( this, aMainApp.GetActiveInstance( ), TRUE );

          break;

     case msimVIEW_SIMQUEUE_MENU_ITEM :

          msimQueueDialog( this );
          break;

     case msimOPEN_MENU_ITEM :

          OpenRxnFile( msimNULL_STR );
          break;



     case msimCREATE_MENU_ITEM :
     {
          msimFILE_STRING filename;
          msimRC rc;
          msimBOOL file_already_loaded = FALSE;

          // attempt switch to unused Instance
          // if no empty one to switch to and a simulation of the current
          // active scheme is running then display error msg and break;

          if ( aMainApp.GetNextEmptyRxnInstance( ) == msimNO_EMPTY_INSTANCES &&
                    aMainApp.GetActiveInstance( ) ->session_id != msimNO_SESSION )
          {
               InfoBox( this, ResId( msimCANT_CLOSE_ACTIVE_SIM_MSG ) ) .Execute( );
               break;
          }

          /* get an empty rxn struct                                   */

          if ( aMainApp.GetActiveInstance( ) )
          {
               if ( ! msimClearMemory( aMainApp.GetActiveInstance( ), this, TRUE ) )
                    break;
               UpdateMainWinData( TRUE );
               aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ),
                    aMainApp.GetActiveInstance( ) ->filename );

          }
          else
               aMainApp.SetActiveInstance( msimCreateRxnInstance( this ) );

          /* get new file name                                         */

          rc = msimGetFileName( this,
               msimAppOptions.data_template, filename, WB_SAVEAS,
               "Create New Reaction Scheme", msimRXNFILE_TYPE, sizeof filename );

          if ( msimUSER_ABORT != rc )
          {
               file_already_loaded = msimFileAlreadyLoaded( this, filename );

               if ( ! file_already_loaded )
               {
                    msimInitRxnInstance( aMainApp.GetActiveInstance( ), this,
                         filename );
                    aMainApp.GetActiveInstance( ) ->listbox_selection = 0;
                    aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ),
                         aMainApp.GetActiveInstance( ) ->filename );
               }
          }

          if ( ( msimUSER_ABORT == rc ) || file_already_loaded )
          {
               delete(aMainApp.GetActiveInstance( ) );

               aMainApp.SetActiveInstance( NULL );

               aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ), NULL );
          }

          UpdateMainWinData( TRUE );

          break;
     }

     case msimEXIT_MENU_ITEM :

          if ( OKToExit( ) )
               aMainApp.Quit( );
          break;

     default :

          break;
     }

     return TRUE;
}



// return TRUE is succesfully opened file

msimBOOL MainWindow::OpenRxnFile( PCHAR FileName )
{
     msimBOOL new_instance;

     // attempt switch to unused Instance
     // if no empty one to switch to and a simulation of the current
     // active scheme is running then display error msg and break;

     if ( ( aMainApp.GetNextEmptyRxnInstance( ) == msimNO_EMPTY_INSTANCES ) &&
               ( aMainApp.GetActiveInstance( ) ->session_id != msimNO_SESSION ) )
     {
          InfoBox( this, ResId( msimCANT_CLOSE_ACTIVE_SIM_MSG ) ) .Execute( );
          return FALSE;
     }


     if ( ! aMainApp.GetActiveInstance( ) )
     {
          // a NULL reaction scheme - allocate memory and initialize
          aMainApp.SetActiveInstance( msimCreateRxnInstance( this ) );
          new_instance = TRUE;
     }
     else
          new_instance = FALSE;

     if ( aMainApp.GetActiveInstance( ) )
     {
          msimRC rc;

          // if we now have a non-null ptr to rxn scheme

          rc = msimLoadRxnScheme( aMainApp.GetActiveInstance( ), FileName, this );

          if ( ( msimNO_ERROR != rc && msimUSER_ABORT != rc ) ||
                    ( msimUSER_ABORT == rc && new_instance ) )
          {
               /* loading was not completed successully - clear out instance                */

               if ( msimClearMemory( aMainApp.GetActiveInstance( ), this, FALSE ) )
               {

                    delete aMainApp.GetActiveInstance( );
                    aMainApp.SetActiveInstance( NULL );
                    UpdateMainWinData( TRUE );
                    aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ), NULL );
               }
               return FALSE;
          }
          else
          {
               aMainApp.UpdateRxnSchemeListMenu( aMainApp.GetActiveInstanceIndex( ), aMainApp.GetActiveInstance( ) ->filename );
               UpdateMainWinData( TRUE );
               return TRUE;
          }
     }

}




long MainWindow::HighlightMenuHdl( Menu PTR pMenu )
{
     String aText = pMenu->GetHelpText( pMenu->GetCurItemId( ) );

     if ( ! aText )
          aHelpStatus.SetText( GetHelpText( ) );
     else
          aHelpStatus.SetText( String( aText ) );

     current_mouse_pos = NULL;
     return TRUE;
}




void MainApp::AppEvent( const ApplicationEvent& rAppEvent )
{
#if defined(__MAC__)

     msimFILE_STRING filename;
	 
 #if defined(__PPC__)
 
 	// deal with notification from engine. In this case the 
	// name of the file to 'open' is the cks gui app 
	// i.e., this application 
	
	 if ( rAppEvent.IsOpenEvent() && 
	      rAppEvent.GetData()  == GetAppFileName( )
		)
	{	
		ChildDied();
		return;
	}	
		
 #endif

     if ( rAppEvent.IsOpenEvent()  || rAppEvent.IsPrintEvent() )
      {
           for (USHORT i = 0; i < rAppEvent.GetParamCount(); i++ )
            {
                 strncpy( filename, rAppEvent.GetParam( i ), sizeof filename );

                 if ( GetAppWindow( )->OpenRxnFile( filename) && rAppEvent.IsPrintEvent() )
               msimCreateTextSummary( GetActiveInstance( ), GetAppWindow( ) );
             }
            return;
      }
     else
#endif
     msimUpdateAfterSimulation( GetAppWindow( ), rAppEvent );
}



void MainApp::UserEvent( ULONG nEvent, void PTR )
{
     switch ( nEvent )
     {

     case msimUSR_EVT_EDIT_RXN :

          msimRxnDataEntryDialog( GetAppWindow( ), aMainApp.GetActiveInstance( ) );

          break;

     case msimUSR_EVT_UPDATE_WIN_AND_LIST :
     case msimUSR_EVT_UPDATE_WIN_ONLY :

          GetAppWindow( ) ->UpdateMainWinData(( nEvent == msimUSR_EVT_UPDATE_WIN_AND_LIST ) );
          break;

     default :
          break;
     }
}



BOOL MainApp::QueryExit( )
{
     return OKToExit( );
}



// ------ definition of MainApp::Main() -------

void MainApp::Main( int argc, char PTR argv[] )

{
     USHORT rc;

     /* initialization                                                      */

     ActiveInstanceIndex = 0;
     ActiveInstance = NULL;
     OKToStartSimulation = TRUE;

     EnableSVLook( );

     SetLogoWindow( ShowLogo( ) );

     /* find the resource file - not needed under OS/2 or Windows   */

#ifdef msimAPP_USES_RESFILE

     if (!msimLocateAppFile(msimRESOURCE_FILENAME, ResourceFileName, sizeof ResourceFilename ))
     {
          String             message = "Unable to locate resource file "
           msimRESOURCE_FILENAME"..."
           " Exiting application. Be sure that the environment variable "
           msimENV_VAR" points to the location of the "msimAPP_NAME
           " program files.";

          ErrorBox           box(NULL, WB_OK|WB_DEF_OK, message);

         while ( GetLogoWindow( ) )
               Reschedule( );


          Abort(message);
     }

#endif

     // instantiate the main window

     MainWindow aAppWindow;

     pAppWindow = &aAppWindow;

     // load the menu bar resource

     MenuBar aAppMenu( ResId( msimMAIN_MENU_ ) );

     ChangeAppMenu( &aAppMenu );

#if  defined(__MAC__)
     EnableAppMenu( FALSE );
#endif

     aAppMenu.PushHighlightHdl( LINK( &aAppWindow, MainWindow, HighlightMenuHdl ) );
     aAppMenu.PushSelectHdl( LINK( &aAppWindow, MainWindow, SelectMenuHdl ) );


     // set the application name

     ChangeAppName( String( msimBaseFilename( argv[0] ) ) );

     // enable and initialize help system

     Help aHelp;

     ChangeHelp( &aHelp );
     EnableHelp( HELPMODE_ALL );

     /* initialize application as necessary, quit if unsuccessful           */

     if ( msimNO_ERROR != ( rc = msimInitializeApp( argc, argv, Instance,
                         GetAppWindow( ) ) ) )
     {
          while ( GetLogoWindow( ) )
               Reschedule( );

          String msg = String( ResId( msimINIT_ERROR_STRING ) );

          msg += rc;
          Abort( msg );
     }

     // set window size using info in msimAppOptions
     // msimAppOptions was initliaze in msimInitializeApp

     aAppWindow.SetPosSizePixel( Point( msimAppOptions.main_window_position.x,
               msimAppOptions.main_window_position.y ), Size
          ( msimAppOptions.main_window_position.width,
               msimAppOptions.main_window_position.height ) );

     // then call the resize fcn to recalc all geometries and positioning on controls

     aAppWindow.Resize( );

     ActiveInstance = Instance[0];


     // wait for logo window to close if it's still around

     while ( GetLogoWindow( ) )
          Reschedule( );

#if  defined(__MAC__)
     EnableAppMenu( TRUE );
#endif

     aAppWindow.UpdateMainWinData( TRUE );// moved for MAC - no enabled menu items until last minute. Was before prec loop
     aAppWindow.Show( );


     // start the event/msg processing
     Execute( );

     // after user has requested exit of application

     if ( ! aAppWindow.IsMinimized( ) && ! aAppWindow.IsMaximized( ) )
          msimSaveCurrentWindowGeometry( GetAppWindow( ),
               & ( msimAppOptions.main_window_position ) );

     if ( msimNO_ERROR != ( rc = msimCloseApp( Instance, GetAppWindow( ) ) ) )
     {
          String msg = String( ResId( msimSHUTDOWN_ERR_STR ) );

          msg += rc;
          Abort( msg );
     }

}



void MainWindow::UpdateMainWinData( msimBOOL RefreshRxnBuffer )

{
     msimSTRING tmp1, tmp2, tmp3, tmp4;
     msimPINSTANCE temp_ptr;
     PCHAR status_ptr = tmp4;
     PopupMenu PTR ppopup;

     MenuBar PTR pmenu = aMainApp.GetAppMenu ( );

     msimPINSTANCE Instance = aMainApp.GetActiveInstance( );

     /* if Instance is a NULL ptr do not allow any data editing to occur    */

     temp_ptr = Instance;

     if ( ! temp_ptr )
     {
          pmenu->EnableItem( msimDATA_ENTRY_MENU, FALSE );
          Instance = &msimC_INSTANCE;
     }
     else
          pmenu->EnableItem( msimDATA_ENTRY_MENU,
               ( BOOL ) ( ( Instance->session_id == msimNO_SESSION ) &&
                    ( Instance->sim_return_code != msimWAITING_IN_QUEUE ) ) );

     /* activate/deactivate menu items as appropriate                       */

     // the file pulldown menu...
     ppopup = pmenu->GetPopupMenu( msimFILE_MENU );

     // enable Save if  we have notes or rxns  and the data has been altered
     // but we are not simulating this scheme right now

     ppopup->EnableItem( msimSAVE_MENU_ITEM, ( BOOL )
          ( TRUE == ( ( ( Instance->ptr_to_rxnlist != NULL ) ||
                         ( Instance->ptr_to_notebook_text != NULL ) ) &&
                    ( Instance->data_altered_since_lastsave ||
                         Instance->notebook_altered ) ) && ( Instance->session_id == msimNO_SESSION )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE )
          ) );

     ppopup->EnableItem( msimSAVEAS_MENU_ITEM,
          ( BOOL ) (
               ( Instance->sim_return_code != msimWAITING_IN_QUEUE ) &&
               ( Instance->session_id == msimNO_SESSION ) &&
               ( ( Instance->ptr_to_rxnlist != NULL ) ||
                    ( Instance->ptr_to_notebook_text != NULL )
               )
          )
     );

     ppopup->EnableItem( msimCLOSE_MENU_ITEM, ( BOOL
          ) ( ( temp_ptr != NULL ) && ( Instance->session_id == msimNO_SESSION )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE ) ) );

     ppopup->EnableItem( msimPRINT_MENU_ITEM, ( BOOL
          ) ( ( Instance->ptr_to_rxnlist != NULL ) || ( Instance->ptr_to_notebook_text !=
                    NULL ) ) );


     // the data entry pulldown menu
     ppopup = pmenu->GetPopupMenu( msimDATA_ENTRY_MENU );

     ppopup->EnableItem( msimSPECIES_DATA_MENU_ITEM, ( BOOL ) ( ( Instance->ptr_to_species_list !=
                    NULL ) && ( ( Instance->temp_option == msimVAR_TEMP ) ||
                    ( Instance->volume_option == msimVAR_VOL ) ) ) );

     ppopup->EnableItem
     ( msimCUT_RXN_MENU_ITEM, ( BOOL ) ( Instance->ptr_to_rxnlist != NULL ) );

     ppopup->EnableItem
     ( msimCOPY_RXN_MENU_ITEM, ( BOOL ) ( Instance->ptr_to_rxnlist != NULL ) );

     ppopup->EnableItem
     ( msimPASTE_RXN_MENU_ITEM, ( BOOL ) ( ( Instance != NULL )
               && ( rxn_cut_buffer != NULL ) ) );

     ppopup->EnableItem
     ( msimRXN_CONDNS_MENU_ITEM, ( BOOL ) ( Instance->ptr_to_species_list != NULL ) );

     ppopup = pmenu->GetPopupMenu( msimSIMULATION_MENU );
     ppopup->EnableItem
     ( msimSIMULATION_OPTION_MENU_ITEM, ( BOOL ) ( ( Instance->session_id ==
                    msimNO_SESSION ) && ( Instance->ptr_to_species_list != NULL )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE ) ) );

     ppopup->EnableItem
     ( msimSTART_SIMULATION_MENU_ITEM, ( BOOL ) ( ( Instance->ptr_to_rxnlist != NULL )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE )
               && ( Instance->session_id == msimNO_SESSION ) ) );

     ppopup->EnableItem
     ( msimRESUME_SIMULATION_MENU_ITEM, ( BOOL )
          ( ( Instance->num_simulation_data_records > 0LU ) &&
               ! ( Instance->data_altered_since_lastsave ) && ( Instance->sim_return_code
                    == msimUSER_ABORT ) && ( Instance->session_id == msimNO_SESSION )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE )
          ) );

     ppopup->EnableItem
     ( msimHALT_SIMULATION_MENU_ITEM, ( BOOL ) ( Instance->session_id !=
               msimNO_SESSION ) );

     pmenu->EnableItem( msimSHOW_RESULTS_MENU, ( BOOL )
          ( ! Instance->data_altered_since_lastsave &&
               ( Instance->num_simulation_data_records != 0 ) &&
               ( Instance->session_id == msimNO_SESSION )
               && ( Instance->sim_return_code != msimWAITING_IN_QUEUE )
          ) );

     /* update text on the main window                                      */

     sprintf( tmp1, FILENAME_STR, aMainApp.GetActiveInstanceIndex( ) + 1,
          Instance->base_filename );
     aActiveFileName.SetText( String( tmp1 ) );

     sprintf( tmp2, NUM_RXN_STR, Instance->numsteps );
     aNumReactionsStatus.SetText( String( tmp2 ) );

     sprintf( tmp3, NUM_SPECIES_STR, Instance->speciescount );
     aNumSpeciesStatus.SetText( String( tmp3 ) );

     if ( ! Instance->ptr_to_rxnlist )
          status_ptr = "Status : no reaction data defined";
     else
          if ( Instance->sim_return_code == msimWAITING_IN_QUEUE )
               status_ptr = "Status : waiting in simulation queue";
          else
               if ( Instance->session_id != msimNO_SESSION )

                    status_ptr = "Status : simulation running";
               else
                    if ( Instance->data_altered_since_lastsave )

                         status_ptr = "Status : reaction data modified";
                    else
                         if ( Instance->num_simulation_data_records )
                         {

                              /* go here if we have some data points                               */

                              switch ( Instance->sim_return_code )
                              {
                              case msimZERO_PROBABILITY :

                                   sprintf( tmp4, "Status : simul'n complete in %s sec - %lu pts"
                                        , Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimSIM_EVENT_LIMIT :

                                   sprintf( tmp4, "Status : event limit reached in %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimSIM_RECORD_LIMIT :

                                   sprintf( tmp4, "Status : max num records reached in %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimELAPSED_TIME_LIMIT :

                                   sprintf( tmp4,
                                        "Status : elapsed time limit reached in %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimFINAL_TEMP_REACHED :

                                   sprintf( tmp4, "Status : final temp. reached in %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimUSER_ABORT :

                                   sprintf( tmp4,
                                        "Status : simul'n interrupted by user after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimCHILD_ABORT :

                                   sprintf( tmp4,
                                        "Status : simul'n aborted by signal after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimFILE_OPEN_ERROR :
                              case msimWRITE_ERROR :
                              case msimREAD_ERROR :

                                   sprintf( tmp4,
                                        "Status : simul'n aborted by file error after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimSEG_VIOLATION :

                                   sprintf( tmp4,
                                        "Status : simul'n aborted by mem error after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimMATH_ERROR :

                                   sprintf( tmp4,
                                        "Status : simul'n aborted by math error after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              case msimILLEGAL_INSTR :

                                   sprintf( tmp4,
                                        "Status : Simul, aborted by illegal instr after %s sec - %lu pts",
                                        Instance->execution_time_str, Instance->num_simulation_data_records );

                                   break;

                              default :

                                   sprintf( tmp4, "Status : unknown (code = %hu) - %lu data points"
                                        , Instance->sim_return_code,
                                        Instance->num_simulation_data_records );
                                   break;
                              }        /* end switch                          */
                         }
                         else
                         {

                              /* go here if we do not have simulation data points               */
                              if ( Instance->sim_return_code == msimCHILD_ABORT )
                                   status_ptr = "Status : simulation aborted by signal - 0 data points";
                              else
                                   status_ptr = "Status : reaction scheme unmodified";
                         }

     /* if we did not reassing val to status_ptr then it still              */
     /* points at temp4 buffer                                              */

     aActiveFileStatus.SetText( String( status_ptr ) );

     if ( RefreshRxnBuffer )
     {

          if ( Instance->ptr_to_rxnlist )
          {
               aRxnStepLB.ChangeUpdateMode( FALSE );/* turn off update to
                                                       prevent flicker        */
               aRxnStepLB.Clear( );

               /* fill the listbox                                         */

               for ( msimPRXN rxnptr = Instance->ptr_to_rxnlist; rxnptr != NULL
                         ; rxnptr = rxnptr->next )
               {
                    aRxnStepLB.InsertEntry( String( rxnptr->equation ) );
               }


               aRxnStepLB.SelectEntryPos( Instance->listbox_selection );
               aRxnStepLB.ChangeUpdateMode( TRUE );
               aRxnStepLB.Update( );
          }
          else
               aRxnStepLB.Clear( );

     }
     else
     {
          if ( ! temp_ptr )
          {
               aRxnStepLB.Clear( );
          }
     }

     /* turn on/off pushbuttons and listbox                                 */

     if ( temp_ptr != NULL && Instance->session_id == msimNO_SESSION
               && Instance->sim_return_code != msimWAITING_IN_QUEUE
          )
          aAddReactionPB.Enable( );
     else
          aAddReactionPB.Disable( );

     if ( Instance->ptr_to_rxnlist != NULL &&
               Instance->session_id == msimNO_SESSION &&
               Instance->sim_return_code != msimWAITING_IN_QUEUE
          )
          aEditReactionPB.Enable( );
     else
          aEditReactionPB.Disable( );

     if ( Instance->ptr_to_rxnlist != NULL &&
               Instance->session_id == msimNO_SESSION &&
               Instance->sim_return_code != msimWAITING_IN_QUEUE
          ) aDeleteReactionPB.Enable( );
     else
          aDeleteReactionPB.Disable( );

     if ( Instance->ptr_to_rxnlist != NULL &&
               Instance->session_id == msimNO_SESSION &&
               Instance->sim_return_code != msimWAITING_IN_QUEUE
          ) aRxnStepLB.Enable( );
     else
          aRxnStepLB.Disable( );

#if defined(__MAC__)
     aRxnStepLB.Invalidate();
	 
	 #if defined(__PPC__)
               aRxnStepLB.SelectEntryPos( Instance->listbox_selection );
	 #endif
#endif

     // Windows needs this, OS/2 does not

#if      defined(__MSDOS__)
     Invalidate();
#endif
     return;
}


/*--------------------------------------------------------------------------*/
/*                        GetNextEmptyRxnInstance()                         */
/*..........................................................................*/
/*                                                                          */
/* scane through the array of rxn instances looking for one set to NULL     */
/* If one is found, then its index is returned, else the value              */
/* msimNO_EMPTY_INSTANCES is returned                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
USHORT MainApp::GetNextEmptyRxnInstance( )
{
     for ( USHORT i = 0; i < msimMAX_NO_INSTANCES; i++ )
     {
          if ( ! Instance[i] )
          {
               ActivateRxnInstance( i );
               return i;               /* find first NULL instance            */
          }
     }
     return msimNO_EMPTY_INSTANCES;
}



// always uses the currently active rxn instance

void MainApp::UpdateRxnSchemeListMenu( USHORT Index, PCHAR FileName )
{
     String str;
     if ( ! FileName )
          FileName = ( PCHAR ) & ( msimC_INSTANCE.filename );

     str = String( "~" ) + String( Index + 1 ) + String( " : " ) + String( FileName );

     GetAppMenu( ) ->
     GetPopupMenu( msimSELECT_MENU ) ->SetItemText( Index + 1, str );
}


void MainApp::ActivateRxnInstance( USHORT NewIndex )
{

     // do nothing if invalid parameter or no change in active instance
     if ( NewIndex >= msimMAX_NO_INSTANCES || ActiveInstanceIndex == NewIndex )
          return;

     ActiveInstanceIndex = NewIndex;   // update member variable

     /* if currently active instance is non-NULL, then hide the plot and notebook*/
     /* windows is they are visible                                         */

     if ( ActiveInstance )
     {
          if ( ActiveInstance->p_notebook_window != msimWID_INVALID )
               ActiveInstance->p_notebook_window->Hide( );

          if ( ActiveInstance->p_plot_dialog != msimWID_INVALID )
               msimHidePlotDialogWindow( ActiveInstance );
     }

     /* if this is an existing instance then build the main window          */

     ActiveInstance = Instance[ActiveInstanceIndex];
     GetAppWindow( ) ->UpdateMainWinData( TRUE );

     /* show notebook and plot windows if the newly activated instance      */
     /* already had them open                                               */

     if ( ActiveInstance )
     {
          if ( ActiveInstance->p_notebook_window != msimWID_INVALID )
          {
               ActiveInstance->p_notebook_window->Show( );
               ActiveInstance->p_notebook_window->ToTop( );
          }

          if ( ActiveInstance->p_plot_dialog != msimWID_INVALID )
          {
               msimShowPlotDialogWindow( ActiveInstance );
               ActiveInstance->p_plot_dialog->ToTop( );
          }
     }

     return;
}

/*--------------------------------------------------------------------------*/
/*                        DeleteReactionFromListBox()                       */
/*..........................................................................*/
/*                                                                          */
/* removes a reaction struct from the linked list of reactions in Instance, */
/* updates the listbox accordingly. Returns the ptr to the current          */
/* selected   rxn struct in the listbox, returns NULL and cleans up         */
/* memory and the Instance struct if that is the case                       */
/* Assumes that the RxnPtr passed to it has already been tested to ensure   */
/* an non-NULL value                                                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPRXN MainWindow::DeleteReactionFromListBox( msimPRXN RxnPtr,
              USHORT Position,
              msimPINSTANCE
              Instance )
{
     msimPRXN tmp;
     USHORT error_position;
     msimBOOL last_posn;

     last_posn = ( RxnPtr->next == NULL );

     if ( NULL == ( tmp = msimDeleteReaction( RxnPtr, Instance ) ) )
     {

          /* do this if we removed the only rxn                             */
          /* in scheme -- we  clean up the  Instance                        */
          /* data structure                                                 */

          msimClearSpeciesList( Instance->ptr_to_species_list );

          Instance->ptr_to_species_list = NULL;
          Instance->ptr_to_rxnlist = NULL;
          Instance->numsteps = 0;
          Instance->nonzeroconcs = 0;
          Instance->speciescount = 0;
          Instance->data_altered_since_lastsave = TRUE;
          Instance->specieslist_altered = TRUE;

          UpdateMainWinData( TRUE );

          return NULL;
     }
     else
     {

          /* now remove the deleted reaction from rxn list                  */

          aRxnStepLB.RemoveEntry( Position );

          /* decrement the position if we just removed the last             */
          /* line in the listbox                                            */

          if ( last_posn )
               Position--;

          /* and select the new current reaction                            */

          aRxnStepLB.SelectEntryPos( Position );

          Instance->listbox_selection = Position;

     }

     Instance->data_altered_since_lastsave = TRUE;
     Instance->specieslist_altered = TRUE;

     /* analyze rxn scheme - should not be any errors but check for         */
     /* them anyway                                                         */
     /* if there is an error then go into edit mode on the offending        */
     /* equation                                                            */

     if ( msimNO_ERROR != msimAnalyzeRxnScheme( Instance, &tmp, &error_position,
                    this ) )
     {

          /* select the erroneous eqn in main rxn list                      */

          aRxnStepLB.SelectEntryPos( error_position );

          /* and post message to simulate having                            */

          aMainApp.PostUserEvent( msimUSR_EVT_EDIT_RXN, NULL );
     }

     UpdateMainWinData( FALSE );

     return tmp;
}

/*--------------------------------------------------------------------------*/
/*                        AddReactionToListBox()                            */
/*..........................................................................*/
/*                                                                          */
/* Adds    a reaction struct to   the linked list of reactions in Instance  */
/* updates the listbox accordingly. Returns the ptr to the newly created    */
/*  rxn struct                                                              */
/* Returns NULL if the memory  allocation fails                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPRXN MainWindow::AddReactionToListBox( msimPRXN RxnPtr,
              msimPINSTANCE Instance )
{
     msimPRXN tmp;

     if ( NULL == ( tmp = msimAddReaction( RxnPtr, Instance ) ) )
          return NULL;

     if ( RxnPtr )
          Instance->listbox_selection++;

     /* add new item with blank text line in rxn list                       */

     aRxnStepLB.InsertEntry( String( msimBLANK_STR ), Instance->listbox_selection
     );

     /* and make it the selected reaction                                   */

     aRxnStepLB.SelectEntryPos( Instance->listbox_selection );

     Instance->data_altered_since_lastsave = TRUE;
     Instance->specieslist_altered = TRUE;

     return tmp;
}

static BOOL OKToExit( )
{
     QueryBox box( aMainApp.GetAppWindow( ), ResId( msimOK_TO_QUIT ) );

#if defined(__AIX__)
    // don't do this - corrupts the box layout somehow (wdh 9.2.94)
#else
     msimCenterDialogWindowOnOwner( &box, aMainApp.GetAppWindow( ) );
#endif

     if ( RET_YES == box.Execute( ) )
     {
          msimSimQueue.ClearQueue( );

          // check for active simulation
          if ( msimCheckForActiveSimulation( aMainApp.GetInstanceArray( ), aMainApp.GetAppWindow( ) ) )
               return FALSE;
          else
               return TRUE;
     }
     else
          return FALSE;
}



// --- instantiate aMainApp --------------------------------------------------

MainApp aMainApp;




