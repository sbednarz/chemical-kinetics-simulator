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

/* []-----------------------------------------------------------------------[]
   |  msimrxde.cxx   :   a module of msim                                   |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : defines a dialog for viewing/editing reactions       |
   |                   the Eqn, rate parameters are set here                |
   |                                                                        |
   |  authors        :   Fumiko Allen and Bill Hinsberg , IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 1993                                           |
   |                                                                        |
   |  Uses the Starview Class libraries to provide multi-platform support   |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <stdlib.h>

class RxnDataEntryDialog : public ModalDialog
{

protected :
     FixedText RxnStepLabel;
     Edit EquationSLE;
     RadioButton ArrhFormRB;
     RadioButton SingleRateRB;
     GroupBox aGroupBox1;
     RadioButton StoichRateLawRB;
     RadioButton SpecRateLawRB;
     PushButton EditRateLawPB;
     GroupBox aGroupBox2;
     FixedText aFixedText1;
     FixedText ReverseLabel;
     Edit FwdAFactorSLE;
     Edit FwdTExpSLE;
     Edit FwdEaSLE;
     Edit RevAFactorSLE;
     Edit RevTExpSLE;
     Edit RevEaSLE;
     GroupBox aGroupBox3;
     OKButton ClosePB;
     PushButton AddRxnPB;
     PushButton DeleteRxnPB;
     PushButton UndoPB;
     HelpButton HelpPB;
     FixedText AFactorLabel;
     FixedText aFixedText4;
     FixedText EaLabel;
     FixedText AFactorUnitsLabel;
     FixedText EaUnitsLabel;
     SymbolButton PrevPB;
     SymbolButton NextPB;

     msimPINSTANCE pSimInstance;
     msimBOOL data_altered;
     msimPRXN rxnptr;
     msimBOOL eqn_altered;
     msimWID owner;

     void Initialize_Entries( );
     void Update_Panel( );

     void ModifiedReactionStep( );
     void ModifiedRateConstant( );

     msimBOOL Store_Data( );
     void SetModifiedFlagHandler( Edit PTR pEdit );

     void SendErrorMessage( SHORT, PCHAR, msimBOOL );

public :
     RxnDataEntryDialog( Window PTR pParent, msimPINSTANCE PSimInstance );

     void PrevHandler( SymbolButton PTR aSymbolButton );
     void NextHandler( SymbolButton PTR aSymbolButton );

     void ReactionStepModifiedHandler( Edit PTR pEdit );

     void RateConstantHandler( RadioButton PTR pRButton );

     void RateLawHandler( RadioButton PTR pRButton );
     void EditRateLawHandler( Edit PTR pEdit );

     void CloseHandler( PushButton PTR pButton );
     void Add_RxnHandler( PushButton PTR pButton );
     void Delete_RxnHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
};


RxnDataEntryDialog::
RxnDataEntryDialog( Window PTR pParent, msimPINSTANCE PSimInstance ) :
ModalDialog ( pParent, ResId ( msimRXN_DATA_ENTRY_PANEL ) ),
RxnStepLabel ( this, ResId ( msimRDE_REACTION_STEP_LABEL ) ),
EquationSLE ( this, ResId ( msimRDE_REACTION ) ),
ArrhFormRB ( this, ResId ( msimRDE_ARRH_FORM ) ),
SingleRateRB ( this, ResId ( msimRDE_SINGLE_RATE ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
StoichRateLawRB ( this, ResId ( msimRDE_STOICH_RATELAW ) ),
SpecRateLawRB ( this, ResId ( msimRDE_SPEC_RATELAW ) ),
EditRateLawPB ( this, ResId ( msimRDE_EDIT_RATELAW ) ),
aFixedText1 ( this, ResId ( 1 ) ),
ReverseLabel ( this, ResId ( msimRDE_REV_LABEL ) ),
FwdAFactorSLE ( this, ResId ( msimRDE_FWD_AFACTOR ) ),
FwdTExpSLE ( this, ResId ( msimRDE_FWD_TEXP ) ),
FwdEaSLE ( this, ResId ( msimRDE_FWD_EA ) ),
RevAFactorSLE ( this, ResId ( msimRDE_REV_A_FACTOR ) ),
RevTExpSLE ( this, ResId ( msimRDE_REV_TEXP ) ),
RevEaSLE ( this, ResId ( msimRDE_REV_EA ) ),
aGroupBox3 ( this, ResId ( 3 ) ),
ClosePB ( this, ResId ( msimRDE_CLOSE ) ),
AddRxnPB ( this, ResId ( msimRDE_ADD_REACTION ) ),
DeleteRxnPB ( this, ResId ( msimRDE_DELETE_REACTION ) ),
UndoPB ( this, ResId ( msimRDE_UNDO ) ),
HelpPB ( this, ResId ( msimRDE_HELP ) ),
AFactorLabel ( this, ResId ( msimRDE_AFACTOR_LABEL ) ),
aFixedText4 ( this, ResId ( 4 ) ),
EaLabel ( this, ResId ( msimRDE_EA_LABEL ) ),
AFactorUnitsLabel ( this, ResId ( msimRDE_AFACTOR_UNITS ) ),
EaUnitsLabel ( this, ResId ( msimRDE_EA_UNITS ) ),
PrevPB ( this, ResId ( msimRDE_PREV_BTN ) ),
NextPB ( this, ResId ( msimRDE_NEXT_BTN ) )
{
     FreeResource( );
     pSimInstance = PSimInstance;
     owner = pParent;

     SetText( GetText( ) + pSimInstance->base_filename );//wdh

     Initialize_Entries( );

     PrevPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, PrevHandler ) );
     NextPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, NextHandler ) );
     EquationSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, ReactionStepModifiedHandler ) );
     ArrhFormRB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateConstantHandler ) );
     SingleRateRB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateConstantHandler ) );
     StoichRateLawRB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateLawHandler ) );
     SpecRateLawRB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, RateLawHandler ) );
     EditRateLawPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, EditRateLawHandler ) );

     FwdAFactorSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     FwdTExpSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     FwdEaSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     RevAFactorSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     RevTExpSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );
     RevEaSLE.ChangeModifyHdl(
          LINK( this, RxnDataEntryDialog, SetModifiedFlagHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, CloseHandler ) );
     AddRxnPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, Add_RxnHandler ) );
     DeleteRxnPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, Delete_RxnHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, RxnDataEntryDialog, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


void RxnDataEntryDialog::Initialize_Entries( )
{
     // Initialize entries in the dialog box

     if ( ! pSimInstance->ptr_to_rxnlist )
     {
          // then we need to allocate memory for the new reaction list

          rxnptr = ( pSimInstance->ptr_to_rxnlist = msimAddReaction( NULL, pSimInstance ) );
     }
     else
     {
          rxnptr = msimGetRxnPtrFromLineNumber( pSimInstance->listbox_selection, pSimInstance->ptr_to_rxnlist );
     }

     // initialize the AFactor and Acitivation Energy unit

     AFactorUnitsLabel.SetText( String( "(" ) +
          String( msimAFactorUnits( 0.0, pSimInstance->conc_units,
                    pSimInstance->volume_units, pSimInstance->time_units ) ) +
          String( ") units" ) );
     EaUnitsLabel.SetText( String( "k" ) +
          String( msimEaUnits( pSimInstance-> energy_units ) ) );

     Update_Panel( );

     if ( msimWords( rxnptr->equation ) )
          data_altered = eqn_altered = FALSE;
     else
          data_altered = eqn_altered = TRUE;// we started w blank eqn from add btn


     return;
}


void RxnDataEntryDialog::Update_Panel( )
{

     RxnStepLabel.SetText( String( "Reaction Step " ) +
          String( pSimInstance->listbox_selection + 1 ) +
          String( " of " ) + String( pSimInstance->numsteps ) );
     EquationSLE.SetText( String( rxnptr->equation ) );
     FwdAFactorSLE.SetText( String( rxnptr->fwdA ) );
     FwdTExpSLE.SetText( String( rxnptr->fwdM ) );
     FwdEaSLE.SetText( String( rxnptr->fwdEa ) );
     ModifiedReactionStep( );

     ArrhFormRB.Check( !( rxnptr->singlerate ) );
     SingleRateRB.Check( rxnptr->singlerate );
     ModifiedRateConstant( );

     StoichRateLawRB.Check( !( rxnptr->not_stoich ) );
     SpecRateLawRB.Check( rxnptr->not_stoich );

     if ( rxnptr->prev )
          PrevPB.Enable( );
     else
          PrevPB.Disable( );

     if ( rxnptr->next )
          NextPB.Enable( );
     else
          NextPB.Disable( );

     // If using a special rate law, then enable the RateLaw Button,
     // otherwise, disable the button

     if ( SpecRateLawRB.IsChecked( ) )
          EditRateLawPB.Enable( );
     else
          EditRateLawPB.Disable( );

     return;
}


void RxnDataEntryDialog::PrevHandler( SymbolButton PTR )
{
     // If the information regarding the previous reaction step is
     // changed, then store all the entries in the data structure.

     // first test to be sure that a previous rxn step exists

     if ( ! rxnptr->prev )
          return;

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          if ( ! Store_Data( ) )
               return;
     }

#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     if ( eqn_altered )
     {
          // wdh added - replace text in main rxn list box if altered
          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( FALSE );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .RemoveEntry( pSimInstance->listbox_selection );
          aMainApp.GetAppWindow( ) ->GetListBox( ) .InsertEntry( String( rxnptr->equation ), pSimInstance->listbox_selection );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( TRUE );// wdh added
     }

#endif

     //  Display the previous reaction step
     rxnptr = rxnptr->prev;

     pSimInstance->listbox_selection--;
     Update_Panel( );

     //  Select line in main rxn list -- wdh added

#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // doesn't fcn on MAC

     aMainApp.GetAppWindow( ) ->GetListBox( ) .SelectEntryPos( pSimInstance->listbox_selection );
#endif

     return;
}


void RxnDataEntryDialog::NextHandler( SymbolButton PTR )
{
     // If the information regarding the previous reaction step is  //
     // changed, then store all the entries in the data structure.  //

     // first test to be sure that a previous rxn step exists

     if ( ! rxnptr->next )
          return;

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          if ( ! Store_Data( ) )
               return;
     }


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     if ( eqn_altered )
     {
          // wdh added - replace text in main rxn list box if altered

          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( FALSE );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .RemoveEntry( pSimInstance->listbox_selection );
          aMainApp.GetAppWindow( ) ->GetListBox( ) .InsertEntry( String( rxnptr->equation ), pSimInstance->listbox_selection );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( TRUE );// wdh added
     }

#endif

     // Display the next reaction step //

     rxnptr = rxnptr->next;

     pSimInstance->listbox_selection++;
     Update_Panel( );


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     //  Select line in main rxn list

     aMainApp.GetAppWindow( ) ->GetListBox( ) .SelectEntryPos( pSimInstance->listbox_selection );

#endif

     return;
}


void RxnDataEntryDialog::ReactionStepModifiedHandler( Edit PTR )
{
     ModifiedReactionStep( );
     data_altered = TRUE;
     eqn_altered = TRUE;
     return;
}


void RxnDataEntryDialog::RateConstantHandler( RadioButton PTR pRButton )
{
     // Update the appropriate information

#if defined(__MAC__)

    // under MAC these RB's are not automatic it seems - probably a SV bug

    ArrhFormRB.Check( pRButton == &ArrhFormRB );
    SingleRateRB.Check( pRButton == &SingleRateRB );

#endif

     ModifiedRateConstant( );
     data_altered = TRUE;
     return;
}


void RxnDataEntryDialog::RateLawHandler( RadioButton PTR )
{
     // If using a special rate law, enable the RateLaw Button and start //
     // the Edit Rate Law dialog box, otherwise, diable the button       //

     msimEQUATION_STRING equation;

     data_altered = TRUE;

     if ( SpecRateLawRB.IsChecked( ) )
     {
          EditRateLawPB.Enable( );

          // start the Edit RateLaw Dialog box //
          msimStringCopy( equation, EquationSLE.GetText( ), sizeof equation );

          msimEditRateLawDialog( this, aMainApp.GetActiveInstance( ),
               rxnptr, equation );

     }
     else
          EditRateLawPB.Disable( );

     return;
}


void RxnDataEntryDialog::EditRateLawHandler( Edit PTR )
{
     // Open the Edit Reaction Law Dialog Box. //

     msimEQUATION_STRING equation;

     msimStringCopy( equation, EquationSLE.GetText( ), sizeof equation );

     msimEditRateLawDialog( this, aMainApp.GetActiveInstance( ),
          rxnptr, equation );

     return;
}



void RxnDataEntryDialog::CloseHandler( PushButton PTR )
{
     msimPRXN error_rxn;
     USHORT error_position;
     msimRC rc;
     msimEQUATION_STRING eqn;


     /* test to see if there is only one */
     /* rxn step defined. If so and it is blank */
     /* then let the user exit the dialog  */
     /* Delete the rxn step and clean things up */

     if ( pSimInstance -> numsteps == 1 )
     {
          /* get and examine the eqn string */
          if ( data_altered )
               if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
                    return;

          msimStringCopy( eqn, EquationSLE.GetText( ), sizeof eqn );

          if ( ! msimWords( eqn ) )
          {
               /* if string is blank or null, */
               /* then clean up and leave */

               /* clean up the Instance */
               /* structure, free memory and bail out */

               msimDeleteReaction( rxnptr, pSimInstance );
               msimClearSpeciesList( pSimInstance->ptr_to_species_list );

               pSimInstance->ptr_to_species_list = NULL;
               pSimInstance->ptr_to_rxnlist = NULL;

               pSimInstance -> numsteps = pSimInstance -> nonzeroconcs =
               pSimInstance -> speciescount = 0;

               aMainApp.GetAppWindow( ) ->GetListBox( ) .Clear( );

               pSimInstance -> data_altered_since_lastsave = TRUE;
               pSimInstance -> specieslist_altered = TRUE;

               EndDialog( );

#if defined(__MAC__)

     // MAC does  not accept updates of main listbox while this dlg is running
      // so we just be sure to do it all now

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_AND_LIST );

#else
               msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
#endif
               return;

          }
     }

     // If the data is altered then update everything and end the dialog box.  //
     // If not then simply end the dialog box.                                 //

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          // Update everything

          if ( ! Store_Data( ) )
               return;
     }

     // wdh added - analyze entire rxn scheme

     if ( msimNO_ERROR != ( rc = msimAnalyzeRxnScheme( pSimInstance, &error_rxn, &error_position, this ) ) )
     {
          // if there is an error found, make the  offending rxn step current so
          // it can be corrected




#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

          // this does not work under MAC

          // first update text in main rxn list
          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( FALSE );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .RemoveEntry( pSimInstance->listbox_selection );
          aMainApp.GetAppWindow( ) ->GetListBox( ) .InsertEntry( String( rxnptr->equation ), pSimInstance->listbox_selection );
          aMainApp.GetAppWindow( ) ->GetListBox( ) .SelectEntryPos( pSimInstance->listbox_selection );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( TRUE );// wdh added


#endif

          /* now make sure the next/prev buttons  */
          /* are set correctly */

          rxnptr = error_rxn;
          pSimInstance->listbox_selection = error_position;

          /* now take care of various details */

          data_altered = FALSE;
          eqn_altered = FALSE;

          Update_Panel( );

          msimDisplayAnalysisError( rc, this );

          return;
     }


     // we get to here if no errors are found during rxn scheme analysis

     EndDialog( );


#if defined(__MAC__)

     // MAC does  not accept updates of main listbox while this dlg is running
      // so we just be sure to do it all now

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_AND_LIST );

#else

     if ( eqn_altered )
          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_AND_LIST );
     else
          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

#endif
     return;
}


void RxnDataEntryDialog::Add_RxnHandler( PushButton PTR )
{
     // If the information regarding the previous reaction step is
     // changed, then store all the entries in the data structure.

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

          if ( ! Store_Data( ) )
               return;
     }


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     if ( eqn_altered )
     {
          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( FALSE );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .RemoveEntry( pSimInstance->listbox_selection );
          aMainApp.GetAppWindow( ) ->GetListBox( ) .InsertEntry( String( rxnptr->equation ), pSimInstance->listbox_selection );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .ChangeUpdateMode( TRUE );
     }

#endif
     // need to allocate memory for the new reaction list

     rxnptr = msimAddReaction( rxnptr, pSimInstance );


     // Update the information on the dialog panel with the new reaction step

     pSimInstance->listbox_selection++;

     Update_Panel( );


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     // this does not work under MAC

     aMainApp.GetAppWindow( ) ->GetListBox( ) .InsertEntry( String( msimBLANK_STR ), pSimInstance->listbox_selection );
     aMainApp.GetAppWindow( ) ->GetListBox( ) .SelectEntryPos( pSimInstance->listbox_selection );

#endif

     data_altered = TRUE;
     eqn_altered = TRUE;

     return;
}


void RxnDataEntryDialog::Delete_RxnHandler( PushButton PTR )
{

     if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
          return;

     // Set the change_listbox_selection flag to TRUE if we are deleting //
     // the last step in the reaction step scheme.                       //
     msimBOOL change_listbox_selection = FALSE;

     if ( rxnptr->next == NULL )
          change_listbox_selection = TRUE;


     // Delete the current reaction step //
     rxnptr = msimDeleteReaction( rxnptr, pSimInstance );

     // wdh 9.2.94
     // rxn step deletion is irreversible, so set flag in Instance struct
     pSimInstance->data_altered_since_lastsave = TRUE;

     // wdh added
     // If we deleted the only reaction, then end the dialog box //
     if ( rxnptr == NULL )
     {
          EndDialog( );
          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_AND_LIST );
     }
     else
     {
          // Update the dialog panel with the next reaction step //
          if ( change_listbox_selection )
               pSimInstance->listbox_selection = pSimInstance->listbox_selection - 1;


          Update_Panel( );


#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

          // this does not work under MAC

          //  wdh added - updates main window so that it tracks what's going on here
          aMainApp.GetAppWindow( ) ->GetListBox( ) .RemoveEntry( change_listbox_selection ?
                    pSimInstance->listbox_selection + 1 : pSimInstance->listbox_selection );

          aMainApp.GetAppWindow( ) ->GetListBox( ) .SelectEntryPos( pSimInstance->listbox_selection );

#endif

          data_altered = TRUE;
          eqn_altered = TRUE;
     }
     return;
}


void RxnDataEntryDialog::UndoHandler( PushButton PTR )
{
     // Undo all the changes made //
     Update_Panel( );
     if ( msimWords( rxnptr->equation ) )
          data_altered = eqn_altered = FALSE;
     else
          data_altered = eqn_altered = TRUE;// we started w blank eqn from add btn


     return;
}


void RxnDataEntryDialog::ModifiedReactionStep( )
{

     // Read the reaction step and determine if the reaction is reversible //
     // or not.  If it is then need to enable the reverse information      //
     // entries.  Otherwise, make suure they are disabled.                 //
     // Also set text in the appropriate dialog entries.                   //

     msimEQUATION_STRING reactionstep;

     msimStringCopy( reactionstep, EquationSLE.GetText( ), sizeof reactionstep );

     rxnptr->reversible = msimSeeIfReversible( reactionstep );

     if ( rxnptr->reversible )
     {
          ReverseLabel.Enable( );
          RevAFactorSLE.Enable( );
          if ( ! SingleRateRB.IsChecked( ) )
          {
               RevTExpSLE.Enable( );
               RevEaSLE.Enable( );
          }
#if defined(__MAC__)
          SetActive( ReverseLabel);
#endif
     }
     else
     {
          ReverseLabel.Disable( );
          RevAFactorSLE.Disable( );
          RevTExpSLE.Disable( );
          RevEaSLE.Disable( );

#if defined(__MAC__)
          SetInactive( ReverseLabel);
#endif
     }

     RevAFactorSLE.SetText( String( rxnptr->revA ) );
     RevTExpSLE.SetText( String( rxnptr->revM ) );
     RevEaSLE.SetText( String( rxnptr->revEa ) );

     return;
}


void RxnDataEntryDialog::ModifiedRateConstant( )
{

     // If Single rate is requested then hide the information regarding //
     // with Temperature expponents and Activation Energies.  Otherwise //
     // make sure these values are visible and accessible to users.     //

#if defined(__MAC__)

     if ( SingleRateRB.IsChecked( ) )
     {
              String empty("");

          aFixedText4.SetText( empty);
          EaLabel.SetText( empty);
                  EaUnitsLabel.SetText( empty);
          AFactorLabel.SetText( String( "Rate Constant " ) );

          FwdTExpSLE.Disable( );
          FwdEaSLE.Disable( );
          RevTExpSLE.Disable( );
          RevEaSLE.Disable( );

              SetInactive(FwdTExpSLE );
          SetInactive(FwdEaSLE );
          SetInactive(RevTExpSLE );
          SetInactive(RevEaSLE );
    }
     else
     {
          aFixedText4.SetText( String("Temperature Exponent " ) );
          EaLabel.SetText( String("Activation Energy " ) );
          EaUnitsLabel.SetText( String( "k" ) +
           String( msimEaUnits( pSimInstance-> energy_units ) ) );
          AFactorLabel.SetText( String( "A Factor " ) );

          FwdTExpSLE.Enable( );
          FwdEaSLE.Enable( );

              SetActive(FwdTExpSLE );
          SetActive(FwdEaSLE );

                  if (rxnptr->reversible)
                  {
               RevTExpSLE.Enable( );
               RevEaSLE.Enable( );

                       SetActive(RevTExpSLE );
               SetActive(RevEaSLE );
              }
     }

#else

     if ( SingleRateRB.IsChecked( ) )
     {
          aFixedText4.Hide( );
          EaLabel.Hide( );
          EaUnitsLabel.Hide( );
          AFactorLabel.SetText( String( "Rate Constant " ) );

          FwdTExpSLE.Hide( );
          FwdEaSLE.Hide( );
          RevTExpSLE.Hide( );
          RevEaSLE.Hide( );
     }
     else
     {
          aFixedText4.Show( );
          EaLabel.Show( );
          EaUnitsLabel.Show( );
          AFactorLabel.SetText( String( "A Factor " ) );

          FwdTExpSLE.Show( );
          FwdEaSLE.Show( );
          RevTExpSLE.Show( );
          RevEaSLE.Show( );

          if ( rxnptr->reversible )
          {
               RevTExpSLE.Enable( );
               RevEaSLE.Enable( );
          }
     }

#endif
     return;
}


msimBOOL RxnDataEntryDialog::Store_Data( )
{
     // Check for the null reaction equation and for the validity of
     // the entry fields.

     USHORT numreactants, numproducts;
     msimRC rc;
     msimNAME_ARRAY reactants, products;
     msimEXPONENT_ARRAY r_def_exponents, p_def_exponents;
     msimEQUATION_STRING equation;
     msimREAL_STRING fwdA, fwdM, fwdEa, revA, revM, revEa;
     msimBOOL singlerate;
     String message;

     // If the equation entry is blank then don't check for the validity //
     // of the rest of the input but just return after the error message.//

     msimStringCopy( equation, EquationSLE.GetText( ), sizeof equation );

     if ( msimNO_ERROR != ( rc = msimAnalyzeEqn( equation, reactants,
                         r_def_exponents,
                         &numreactants,
                         products, p_def_exponents,
                         &numproducts ) ) )
     {
          msimDisplayAnalysisError( rc, this );
          return FALSE;
     }

     // The rest of the input should be converted into the numeric data //
     // without error.  The rateconstant/A Factor must be a valid float //
     // greater than 0.0                                               //

     // read the numerical value to the temporary variables and set the  //
     // single rate flag to proper value.                                //

     msimStringCopy( fwdA, FwdAFactorSLE.GetText( ), sizeof fwdA );
     msimStringCopy( fwdM, FwdTExpSLE.GetText( ), sizeof fwdM );
     msimStringCopy( fwdEa, FwdEaSLE.GetText( ), sizeof fwdEa );
     msimStringCopy( revA, RevAFactorSLE.GetText( ), sizeof revA );
     msimStringCopy( revM, RevTExpSLE.GetText( ), sizeof revM );
     msimStringCopy( revEa, RevEaSLE.GetText( ), sizeof revEa );
     singlerate = SingleRateRB.IsChecked( );

     if ( ! msimValidPositiveFloat( fwdA ) )
     {
          SendErrorMessage( msimRDE_FWD_AFACTOR, fwdA, singlerate );
          return FALSE;
     }

     if ( ! singlerate )
     {
          if ( ! msimValidFloat( fwdM ) )
          {
               SendErrorMessage( msimRDE_FWD_TEXP, fwdM, singlerate );
               return FALSE;
          }
          if ( ! msimValidFloat( fwdEa ) )
          {
               SendErrorMessage( msimRDE_FWD_EA, fwdEa, singlerate );
               return FALSE;
          }
     }

     // if the equation is reversible repeat the above process for the //
     // numerical values for the reversible information                //
     if ( rxnptr->reversible )
     {
          if ( ! msimValidPositiveFloat( revA ) )
          {
               SendErrorMessage( msimRDE_REV_A_FACTOR, revA, singlerate );
               return FALSE;
          }

          if ( ! singlerate )
          {
               if ( ! msimValidFloat( revM ) )
               {
                    SendErrorMessage( msimRDE_REV_TEXP, revM, singlerate );
                    return FALSE;
               }
               if ( ! msimValidFloat( revEa ) )
               {
                    SendErrorMessage( msimRDE_REV_EA, revEa, singlerate );
                    return FALSE;
               }
          }

     }
     // If all the entry fields are valid, then store the data.    //
     if ( eqn_altered )
          pSimInstance->specieslist_altered = TRUE;

     pSimInstance->data_altered_since_lastsave = TRUE;

     msimStringCopy( rxnptr->equation, equation, sizeof rxnptr->equation );
     rxnptr->singlerate = singlerate;
     msimStringCopy( rxnptr->fwdA, fwdA, sizeof rxnptr->fwdA );
     msimStringCopy( rxnptr->fwdM, fwdM, sizeof rxnptr->fwdM );
     msimStringCopy( rxnptr->fwdEa, fwdEa, sizeof rxnptr->fwdEa );
     msimStringCopy( rxnptr->revA, revA, sizeof rxnptr->revA );
     msimStringCopy( rxnptr->revM, revM, sizeof rxnptr->revM );
     msimStringCopy( rxnptr->revEa, revEa, sizeof rxnptr->revEa );

     rxnptr->not_stoich = SpecRateLawRB.IsChecked( );

     return TRUE;
}


void RxnDataEntryDialog::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;
     return;
}


void RxnDataEntryDialog::SendErrorMessage( SHORT FieldID, PCHAR UserEntry, msimBOOL SingleRate )
{
     // Sends the error message to the user //
     String message;
     PCHAR field;

     switch ( FieldID )
     {
     case msimRDE_FWD_AFACTOR :
          field = SingleRate ? "Rate Constant" : "Forward A Factor";
          break;
     case msimRDE_FWD_TEXP :
          field = "Forward Temperature Exponent";
          break;
     case msimRDE_FWD_EA :
          field = "Forward Activation Energy";
          break;
     case msimRDE_REV_A_FACTOR :
          field = SingleRate ? "Rate Constant" : "Reverse A Factor";
          break;
     case msimRDE_REV_TEXP :
          field = "Reverse Temperature Exponent";
          break;
     case msimRDE_REV_EA :
          field = "Reverse Activation Energy";
          break;
     default :
          field = msimNULL_STR;
          break;
     }

     message = "The value [" +
     String( UserEntry ) +
     "] entered for the " +
     String( field ) +
     " is out of range or does not represent a numeric value. " +
     String( msimNULL_STR ) +
     "Correct the error and repeat your last operation.";
     WarningBox box( this, WB_OK | WB_DEF_OK, message );
     box.Execute( );
     return;
}

/*--------------------------------------------------------------------------*/
/*                      DisplayAnalysisError()                              */
/*..........................................................................*/
/*                                                                          */
/* Function for informing the user that an error was detected during        */
/*  analysis of the rxn scheme                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void msimDisplayAnalysisError( msimRC rc, msimWID Owner )
{
     String message( ResId( msimANAL_ERROR_STR1 ) );


     message += String( msimAnalysisError( rc ) );

     message += String( ResId( msimANAL_ERROR_STR2 ) );

     WarningBox( Owner, WB_OK | WB_DEF_OK, message ) .Execute( );
}


void msimRxnDataEntryDialog( msimWID Owner, msimPINSTANCE pInstance )
{

#if defined(__MAC__)
     aMainApp.GetAppWindow( )->GetListBox( ).SetNoSelection( );   // to prevent multiple selections
#endif

     RxnDataEntryDialog PTR pdlg =
                            new RxnDataEntryDialog(Owner, pInstance );

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