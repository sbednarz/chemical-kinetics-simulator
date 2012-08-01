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
/*  FILE NAME : msimvvol.cxx                                                */
/*                                                                          */
/*  This module contains a function for checking that, for variable volume  */
/*  simulations, all species with non-zero initial concentrations are in    */
/* the same phase. It also implements a dialog for verifying species data   */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for multiple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1993                                          */
/*                                                                          */
/*  Authors : Bill Hinsberg                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <stdlib.h>





/*--------------------------------------------------------------------------*/
/*                        msimCheckInitialPhase                             */
/*..........................................................................*/
/*                                                                          */
/* this function tests that all species with non-zero initial amounts are   */
/* in the same phase. If they are then TRUE is returned, FALSE otherwise    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL  msimCheckInitialPhase( msimPINSTANCE pInstance)
{
     msimPSPECIES workptr = pInstance->ptr_to_species_list;
     USHORT initial_phase;

     // walk through linked list of species to
     // find first species with non-zero inital amount
      
     while ( !workptr->nonzero )
          workptr = workptr->next;

     initial_phase = workptr->phys_state;

     workptr = workptr->next; 

     while ( workptr )
     {
          if ( workptr->nonzero )
          {
               if (workptr->phys_state != initial_phase )
                    return FALSE;
          }

          workptr = workptr -> next;
     }

     return TRUE;
}




class ChangePhaseDataDlg : public ModalDialog
{

protected :
     FixedText SpeciesDataED1Text;
     FixedText SpeciesNameLabel;
     FixedText FixedText1;
     FixedText FixedText2;
     Edit ConcSLE;
     FixedText ConcUnitsLabel;
     ListBox SpeciesNameLB;
     RadioButton SolidStateRB;
     RadioButton LiquidStateRB;
     RadioButton GasStateRB;
     FixedText DensityLabel;
     Edit DensitySLE;
     FixedText DensityUnitsLabel;
     GroupBox ConcLabelGB;
     GroupBox SpecListGB;
     GroupBox StateGB;
     OKButton ClosePB;
     PushButton CancelPB;
     PushButton DefaultsPB;
     PushButton UndoPB;
     HelpButton HelpPB;

     msimREAL_STRING PTR tempInitConc;
     String conc_amount;

     msimPINSTANCE pSimInstance;
     msimPSPECIES speciesptr;
     USHORT PTR tempPhysState;
     msimREAL_STRING PTR tempDensity;
     msimBOOL data_altered;
     msimWID owner;
     msimBOOL initialized;

     void RestoreInitialData( );
     void SetSpeciesDataToDefault( );

     void AllocateAndInitializeConcArray( );
     void InitializeEntries( );
     void UpdatePanel( );
     void DispPhysStateMolarDensity( );
     msimBOOL ValidData( );
     void ReadCurrentEntries( );
     void InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance );
     void RestoreInitialConcData( );

public :

     ChangePhaseDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance );
     ~ChangePhaseDataDlg( );

     msimBOOL IsInitialized( )
     {
          return initialized;
     };

     void ListboxSelectHandler( ListBox PTR );

     void PhysStateHandler( RadioButton PTR pRadioButton );
     void TextModifiedHandler( Edit PTR );

     void CloseHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );

     void CancelHandler( PushButton PTR pButton );
};



// destructor - deletes allocated memory

ChangePhaseDataDlg::~ChangePhaseDataDlg( )
{
     if ( tempInitConc )
          delete[]tempInitConc;

     if ( tempPhysState )
          delete[]tempPhysState;

     if ( tempDensity )
          delete[]tempDensity;

}



ChangePhaseDataDlg::ChangePhaseDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance ) :
ModalDialog ( pParent, ResId ( msimPHASE_DATA_PANEL ) ),
SpeciesDataED1Text ( this, ResId ( msimPHASE_DATA_ED_1 ) ),
SpeciesNameLabel ( this, ResId ( msimPHASE_DATA_NAME_STR ) ),
FixedText1 ( this, ResId ( 1 ) ),
FixedText2 ( this, ResId ( 2 ) ),
ConcSLE ( this, ResId ( msimPHASE_DATA_CONC ) ),
ConcUnitsLabel ( this, ResId ( msimPHASE_DATA_CONCUNITS ) ),
SpeciesNameLB ( this, ResId ( msimPHASE_DATA_LISTBOX ) ),
SolidStateRB ( this, ResId ( msimPHASE_DATA_SOLID ) ),
LiquidStateRB ( this, ResId ( msimPHASE_DATA_LIQUID ) ),
GasStateRB ( this, ResId ( msimPHASE_DATA_GAS ) ),
DensityLabel ( this, ResId ( msimPHASE_DATA_DENSITY_LABEL ) ),
DensitySLE ( this, ResId ( msimPHASE_DATA_DENSITY ) ),
DensityUnitsLabel ( this, ResId ( msimPHASE_DATA_DENSUNITS ) ),
ConcLabelGB ( this, ResId ( msimPHASE_DATA_CONC_LABEL ) ),
SpecListGB ( this, ResId ( msimPHASE_DATA_LIST ) ),
StateGB ( this, ResId ( msimPHASE_DATA_STATE_LABEL ) ),
ClosePB ( this, ResId ( msimPHASE_DATA_CLOSE ) ),
CancelPB ( this, ResId ( msimPHASE_DATA_CANCEL ) ),
DefaultsPB ( this, ResId ( msimPHASE_DATA_DEFAULTS ) ),
UndoPB ( this, ResId ( msimPHASE_DATA_UNDO ) ),
HelpPB ( this, ResId ( msimPHASE_DATA_HELP ) )
{

     FreeResource( );

     InitSpeciesData( pParent, PSimInstance );
     AllocateAndInitializeConcArray( );

     // Set the title of the dialog

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     InitializeEntries( );
     UpdatePanel( );

     data_altered = FALSE;

     SpeciesNameLB.ChangeSelectHdl(
          LINK( this, ChangePhaseDataDlg, ListboxSelectHandler ) );

     SolidStateRB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, PhysStateHandler ) );
     LiquidStateRB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, PhysStateHandler ) );
     GasStateRB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, PhysStateHandler ) );

     ConcSLE.ChangeModifyHdl(
          LINK( this, ChangePhaseDataDlg, TextModifiedHandler ) );
     DensitySLE.ChangeModifyHdl(
          LINK( this, ChangePhaseDataDlg, TextModifiedHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, CloseHandler ) );

     CancelPB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, CancelHandler ) );

     DefaultsPB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, DefaultsHandler ) );

     UndoPB.ChangeClickHdl(
          LINK( this, ChangePhaseDataDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );
}


void ChangePhaseDataDlg::AllocateAndInitializeConcArray( )
{
     tempInitConc = new msimREAL_STRING[pSimInstance->speciescount];

     if ( ! tempInitConc )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, owner );

          initialized = FALSE;
     }
     else
     {
          msimPSPECIES curspecptr = pSimInstance->ptr_to_species_list;

          USHORT j = 0;

          while ( curspecptr )
          {
               msimStringCopy( tempInitConc[j++], curspecptr->initialconc,
                    sizeof tempInitConc[0] );

               curspecptr = curspecptr->next;
          }
     }
}

void ChangePhaseDataDlg::RestoreInitialConcData( )
{
     msimPSPECIES curspecptr = pSimInstance->ptr_to_species_list;

     USHORT j = 0;

     while ( curspecptr )
     {
          msimStringCopy( curspecptr->initialconc, tempInitConc[j++],
               sizeof curspecptr->initialconc );

          curspecptr = curspecptr->next;
     }
}


void ChangePhaseDataDlg::InitializeEntries( )
{
     // first initialize the species listbox contents

     // turn off update to prevent flicker

     SpeciesNameLB.ChangeUpdateMode( FALSE );
     SpeciesNameLB.Clear( );

     // fill the listbox

     for ( speciesptr = pSimInstance->ptr_to_species_list;
               speciesptr != NULL; speciesptr = speciesptr->next )
     {
          SpeciesNameLB.InsertEntry( String( speciesptr->name ) );
     }

     SpeciesNameLB.SelectEntryPos( 0 );
     SpeciesNameLB.ChangeUpdateMode( TRUE );
     SpeciesNameLB.Update( );

     // set the species pointer to the top of the list

     speciesptr = pSimInstance->ptr_to_species_list;

     // now initialize static text

     // initialize the initial amount units, and
     // the molar density unit 

     ConcUnitsLabel.SetText(
          String( msimConcUnits( pSimInstance->conc_units,
                    pSimInstance->volume_option ) ) );

     DensityUnitsLabel.SetText( String( "moles/" ) +
          String( msimVolumeUnits( pSimInstance->volume_units ) ) );

     return;
}





void ChangePhaseDataDlg::UpdatePanel( )
{
     // display necessary information for the selected species

     SpeciesNameLabel.SetText( String( speciesptr->name ) );

     ConcSLE.SetText( String( speciesptr->initialconc ) );

     DispPhysStateMolarDensity( );

     return;
}



void ChangePhaseDataDlg::DispPhysStateMolarDensity( )
{
     // display the data for the selected species

     SolidStateRB.Check( speciesptr->phys_state == msimSOLID );
     LiquidStateRB.Check( speciesptr->phys_state == msimLIQUID );
     GasStateRB.Check( speciesptr->phys_state == msimGAS );

     DensitySLE.SetText( String( speciesptr->molardensity ) );

     return;
}



void ChangePhaseDataDlg::ReadCurrentEntries( )
{
     msimStringCopy( speciesptr->initialconc, ConcSLE.GetText( ),
          sizeof speciesptr->initialconc );

     // read phys_state and density controls

     if ( SolidStateRB.IsChecked( ) )
          speciesptr->phys_state = msimSOLID;
     else
     {
          if ( LiquidStateRB.IsChecked( ) )
               speciesptr->phys_state = msimLIQUID;
          else
               speciesptr->phys_state = msimGAS;
     }

     msimStringCopy( speciesptr->molardensity, DensitySLE.GetText( ),
          sizeof speciesptr->molardensity );
}


void ChangePhaseDataDlg::ListboxSelectHandler( ListBox PTR )
{
     USHORT i;
     USHORT curposition;

     // check the validity of the data in the entry fields for the
     // previously selected species before changing to new selection


     ReadCurrentEntries( );

     if ( ! ValidData( ) )
          return;

     // obtain the new listbox position and the species pointer //

     curposition = SpeciesNameLB.GetSelectEntryPos( );

     speciesptr = pSimInstance->ptr_to_species_list;

     for ( i = 0; i < curposition; i++ )
          speciesptr = speciesptr->next;

     // update the panel for newly seelcted species //

     UpdatePanel( );

     return;
}



void ChangePhaseDataDlg::PhysStateHandler( RadioButton PTR pRadioButton )
{

#if defined(__MAC__)

     SolidStateRB.Check( pRadioButton == &SolidStateRB );
     LiquidStateRB.Check( pRadioButton == &LiquidStateRB );
     GasStateRB.Check( pRadioButton == &GasStateRB );

#endif

     data_altered = TRUE;

     return;
}


void ChangePhaseDataDlg::TextModifiedHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}


void ChangePhaseDataDlg::CloseHandler( PushButton PTR )
{
     // check the validity of the entry fields

     ReadCurrentEntries( );

     if ( ! ValidData( ) )
          return;

     if ( data_altered )
          pSimInstance->data_altered_since_lastsave = TRUE;

     pSimInstance->specieslist_altered = FALSE;

     pSimInstance->nonzeroconcs =
     msimNumberNonZero( pSimInstance->ptr_to_species_list );

     EndDialog( msimNO_ERROR );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


void ChangePhaseDataDlg::CancelHandler( PushButton PTR )
{
     pSimInstance->specieslist_altered = FALSE;

     EndDialog( msimUSER_ABORT );

     return;
}


void ChangePhaseDataDlg::UndoHandler( PushButton PTR )
{
     RestoreInitialData( );

     RestoreInitialConcData( );

     UpdatePanel( );

     data_altered = FALSE;

     return;
}


void ChangePhaseDataDlg::DefaultsHandler( PushButton PTR )
{
     SetSpeciesDataToDefault( );

     UpdatePanel( );

     return;
}


msimBOOL ChangePhaseDataDlg::ValidData( )
{
     String message;

     // check the info read from the entry fields and
     // now contained in the species struct are valid.

     // first, with the initial concentration

     if ( ! msimValidNonNegativeFloat( speciesptr->initialconc ) )
     {


          message = String( ResId( msimPHASE_DATA_STR1) )
                  + String( speciesptr->initialconc )
                  + String( ResId( msimPHASE_DATA_STR2) )
                  + String( speciesptr->name )
                  + String( ResId( msimPHASE_DATA_STR3) );
          ;

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }


     // Then  the  molar density


     if ( ! msimValidPositiveFloat( speciesptr->molardensity ) )
     {
          message = String( ResId( msimPHASE_DATA_STR4) ) 
                  + String( speciesptr->molardensity )
                  + String( ResId( msimPHASE_DATA_STR5) )
                  + String( speciesptr->name )
                  + String( ResId( msimPHASE_DATA_STR6) );

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

               return FALSE;
     }


     return TRUE;
}


void ChangePhaseDataDlg::RestoreInitialData( )
{
     msimPSPECIES curspecptr;          // used to fill in the temporary arrays
     USHORT j;

          curspecptr = pSimInstance->ptr_to_species_list;

          j = 0;
          while ( curspecptr )
          {
               curspecptr->phys_state = tempPhysState[j];
               msimStringCopy( curspecptr->molardensity, tempDensity[j++], sizeof curspecptr->molardensity );

               curspecptr = curspecptr->next;
          }
}


void ChangePhaseDataDlg::SetSpeciesDataToDefault( )
{
     // set relevant fields to default values

     strcpy( speciesptr->molardensity, msimC_SPECIES.molardensity );
     speciesptr->phys_state = msimC_SPECIES.phys_state;

     data_altered = TRUE;
}





void ChangePhaseDataDlg::InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance )
{
     msimPSPECIES curspecptr;          // used to fill in the temporary arrays
     USHORT j;

     pSimInstance = PSimInstance;
     owner = pParent;

     initialized = TRUE;

     tempPhysState = new USHORT[pSimInstance->speciescount];
     tempDensity = new msimREAL_STRING[pSimInstance->speciescount];

     if ( tempPhysState == NULL || tempDensity == NULL )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, owner );

          initialized = FALSE;
     }
     else
     {
          // initialize the arrays
          // fill in the physical state and molar density information
          // for each species into appropriate temporary arrays

          curspecptr = pSimInstance->ptr_to_species_list;

          j = 0;
          while ( curspecptr )
          {
               tempPhysState[j] = curspecptr->phys_state;
               msimStringCopy( tempDensity[j++], curspecptr->molardensity,
                    sizeof tempDensity[0] );
               curspecptr = curspecptr->next;
          }
     }

}



msimRC msimChangePhaseDataDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     msimRC rc;

     ChangePhaseDataDlg PTR pdlg =
                           new ChangePhaseDataDlg( Owner, pInstance );

     if ( pdlg )
     {
          if ( pdlg->IsInitialized( ) )
               rc = pdlg->Execute( );
          else
               rc = msimMEM_ALLOC_ERROR;

          delete pdlg;
          return rc;
     }
     else
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );
          return msimMEM_ALLOC_ERROR;
     }
}

