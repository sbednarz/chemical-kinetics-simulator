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
   |  msimspda.cxx       :  View/Edit Species Data dialog                   |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Fumiko Allen and Bill Hinsberg , IBM Almaden       |
   |                                                                        |
   |  file created   :   Aug  1993                                          |
   |                                                                        |
   |  This module implements a dialog for viewing and editing species data  |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <stdlib.h>




class SpeciesDataDlg : public ModalDialog
{

protected :
     FixedText aFixedText2;
     ListBox SpeciesNameLB;
     FixedText SpeciesLabelText;
     FixedText SpeciesNameLabel;
     RadioButton SolidStateRB;
     RadioButton LiquidStateRB;
     RadioButton GasStateRB;
     FixedText DensityLabel;
     Edit DensitySLE;
     FixedText DensityUnitsLabel;
     FixedText EnthalpyLabel;
     Edit EnthalpySLE;
     FixedText EnthalpyUnitsLabel;
     FixedText CoeffALabel;
     Edit CoeffASLE;
     FixedText CoeffAUnits;
     FixedText CoeffBLabel;
     Edit CoeffBSLE;
     FixedText CoeffBUnits;
     FixedText CoeffCLabel;
     Edit CoeffCSLE;
     FixedText CoeffCUnits;
     FixedText CoeffDLabel;
     Edit CoeffDSLE;
     FixedText CoeffDUnits;
     GroupBox aGroupBox2;
     GroupBox ThermCoeffsGB;
     GroupBox SpeciesStateGB;
     OKButton ClosePB;
     PushButton DefaultsPB;
     PushButton UndoPB;
     HelpButton HelpPB;

     msimPINSTANCE pSimInstance;
     msimPSPECIES speciesptr;
     USHORT PTR tempPhysState;
     msimREAL_STRING PTR tempDensity;
     msimREAL_STRING_ARRAY PTR tempCoeffs;
     msimBOOL data_altered;
     msimWID owner;
     msimBOOL initialized;

     void RestoreInitialData( );
     void SetSpeciesDataToDefault( );


     void InitializeEntries( );
     void UpdatePanel( );
     void DispPhysStateMolarDensity( );
     void DispThermCoeff( );
     msimBOOL ValidData( );
     void ReadCurrentEntries( );
     void InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance );

public :
     SpeciesDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance );
     ~SpeciesDataDlg( );

     void ListboxSelectHandler( ListBox PTR pListBox );

     void PhysStateHandler( RadioButton PTR pRadioButton );

     void TextModifiedHandler( Edit PTR );

     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );

     msimBOOL IsInitialized( )
     {
          return initialized;
     };
};



SpeciesDataDlg::SpeciesDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance ) :
ModalDialog ( pParent, ResId ( msimSPECIESDATA_PANEL ) ),
aFixedText2 ( this, ResId ( 2 ) ),
SpeciesNameLB ( this, ResId ( msimSPECIESDATA_LISTBOX ) ),
SpeciesLabelText ( this, ResId ( msimSPECIESDATA_SPECIES_LABEL ) ),
SpeciesNameLabel ( this, ResId ( msimSPECIESDATA_NAME_STR ) ),
SolidStateRB ( this, ResId ( msimSPECIESDATA_SOLID ) ),
LiquidStateRB ( this, ResId ( msimSPECIESDATA_LIQUID ) ),
GasStateRB ( this, ResId ( msimSPECIESDATA_GAS ) ),
DensityLabel ( this, ResId ( msimSPECIESDATA_DENSITY_LABEL ) ),
DensitySLE ( this, ResId ( msimSPECIESDATA_DENSITY ) ),
DensityUnitsLabel ( this, ResId ( msimSPECIESDATA_DENSUNITS ) ),
EnthalpyLabel ( this, ResId ( msimSPECIESDATA_ENTHALPY_LABEL ) ),
EnthalpySLE ( this, ResId ( msimSPECIESDATA_ENTHALPY ) ),
EnthalpyUnitsLabel ( this, ResId ( msimSPECIESDATA_ENTH_UNITS ) ),
CoeffALabel ( this, ResId ( msimSPECIESDATA_A_LABEL ) ),
CoeffASLE ( this, ResId ( msimSPECIESDATA_A ) ),
CoeffAUnits ( this, ResId ( msimSPECIESDATA_A_UNITS ) ),
CoeffBLabel ( this, ResId ( msimSPECIESDATA_B_LABEL ) ),
CoeffBSLE ( this, ResId ( msimSPECIESDATA_B ) ),
CoeffBUnits ( this, ResId ( msimSPECIESDATA_B_UNITS ) ),
CoeffCLabel ( this, ResId ( msimSPECIESDATA_C_LABEL ) ),
CoeffCSLE ( this, ResId ( msimSPECIESDATA_C ) ),
CoeffCUnits ( this, ResId ( msimSPECIESDATA_C_UNITS ) ),
CoeffDLabel ( this, ResId ( msimSPECIESDATA_D_LABEL ) ),
CoeffDSLE ( this, ResId ( msimSPECIESDATA_D ) ),
CoeffDUnits ( this, ResId ( msimSPECIESDATA_D_UNITS ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
ThermCoeffsGB ( this, ResId ( msimSPECIESDATA_COEFFLABEL ) ),
SpeciesStateGB ( this, ResId ( msimSPECIESDATA_STATE_LABEL ) ),
ClosePB ( this, ResId ( msimSPECIESDATA_CLOSE ) ),
DefaultsPB ( this, ResId ( msimSPECIESDATA_DEFAULTS ) ),
UndoPB ( this, ResId ( msimSPECIESDATA_UNDO ) ),
HelpPB ( this, ResId ( msimSPECIESDATA_HELP ) )
{
     FreeResource( );

     InitSpeciesData( pParent, PSimInstance );

     if ( ! initialized )// go no further if SpeciesData base class had a problem
          return;

     // Set the title of the dialog

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize entries in the dialog box

     InitializeEntries( );
     UpdatePanel( );

     data_altered = FALSE;

     SpeciesNameLB.ChangeSelectHdl(
          LINK( this, SpeciesDataDlg, ListboxSelectHandler ) );

     SolidStateRB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, PhysStateHandler ) );
     LiquidStateRB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, PhysStateHandler ) );
     GasStateRB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, PhysStateHandler ) );

     DensitySLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );

     EnthalpySLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );
     CoeffASLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );
     CoeffBSLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );
     CoeffCSLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );
     CoeffDSLE.ChangeModifyHdl(
          LINK( this, SpeciesDataDlg, TextModifiedHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, CloseHandler ) );
     DefaultsPB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, DefaultsHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, SpeciesDataDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}

// this creates and initializes temporary storage for the species data
// so that we can undo ALL changes if requested

void SpeciesDataDlg::InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance )
{
     msimPSPECIES curspecptr;          // used to fill in the temporary arrays
     USHORT j;

     pSimInstance = PSimInstance;
     owner = pParent;

     initialized = TRUE;

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
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
     else
     {
          tempPhysState = NULL;
          tempDensity = NULL;
     }


     if ( pSimInstance->temp_option == msimVAR_TEMP )
     {
          tempCoeffs = new msimREAL_STRING_ARRAY[pSimInstance->speciescount];

          if ( tempCoeffs == NULL )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, owner );

               initialized = FALSE;
          }
          else
          {
               // fill in the thermochemical coefficents information for
               // each species into appropriate temporary arrays

               curspecptr = pSimInstance->ptr_to_species_list;

               j = 0;
               while ( curspecptr )
               {
                    for ( USHORT i = 0; i < 5; i++ )
                         msimStringCopy( tempCoeffs[j][i], curspecptr->thermcoeff[i],
                              sizeof tempCoeffs[0][0] );

                    j++;
                    curspecptr = curspecptr->next;
               }

          }
     }
     else tempCoeffs = NULL;
}


void SpeciesDataDlg::RestoreInitialData( )
{
     msimPSPECIES curspecptr;          // used to fill in the temporary arrays
     USHORT j;

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
          curspecptr = pSimInstance->ptr_to_species_list;

          j = 0;
          while ( curspecptr )
          {
               curspecptr->phys_state = tempPhysState[j];
               msimStringCopy( curspecptr->molardensity, tempDensity[j++],
                    sizeof curspecptr->molardensity );

               curspecptr = curspecptr->next;
          }
     }

     if ( pSimInstance->temp_option == msimVAR_TEMP )
     {
          curspecptr = pSimInstance->ptr_to_species_list;

          j = 0;
          while ( curspecptr )
          {
               for ( USHORT i = 0; i < 5; i++ )
                    msimStringCopy( curspecptr->thermcoeff[i], tempCoeffs[j][i],
                         sizeof curspecptr->thermcoeff[0] );

               j++;
               curspecptr = curspecptr->next;
          }

     }
}


void SpeciesDataDlg::SetSpeciesDataToDefault( )
{
     // set relevant fields to default values

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
          msimStringCopy( speciesptr->molardensity, msimC_SPECIES.molardensity,
               sizeof speciesptr->molardensity );
          speciesptr->phys_state = msimC_SPECIES.phys_state;
     }

     if ( pSimInstance->temp_option == msimVAR_TEMP )
          for ( USHORT i = 0; i < msimNUMBER_OF_THERM_COEFFS; i++ )
               msimStringCopy( speciesptr->thermcoeff[i],
                    msimC_SPECIES.thermcoeff[i], sizeof speciesptr->thermcoeff[0] );

     data_altered = TRUE;
}






// initializes the dialog box - called once per invocation

void SpeciesDataDlg::InitializeEntries( )
{
     // first initialize the species name listbox
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


     // reset the species pointer to the top of the list and highlight
     // first entry.

     speciesptr = pSimInstance->ptr_to_species_list;

     // initialize the molar density unit and thermochemical coefficient units

     DensityUnitsLabel.SetText( String( "moles/" ) +
          String( msimVolumeUnits( pSimInstance->volume_units ) ) );

     EnthalpyUnitsLabel.SetText( String( "k" ) +
          String( msimEnergyUnits( pSimInstance->energy_units ) )
          + String( "/mole" ) );

     CoeffAUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) )
          + String( "/mole" ) );

     CoeffBUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) )
          + String( "/mole-deg" ) );

     CoeffCUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) )
          + String( "/mole-deg^2" ) );

     CoeffDUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) )
          + String( "/mole-deg^3" ) );


     // disable the proper panel depending on the volume and temperature
     // options

     if ( pSimInstance->volume_option != msimVAR_VOL )
     {
          SpeciesStateGB.Disable( );
          SolidStateRB.Disable( );
          LiquidStateRB.Disable( );
          GasStateRB.Disable( );
          DensityLabel.Disable( );
          DensitySLE.Disable( );
          DensityUnitsLabel.Disable( );

#if defined(__MAC__)
          SetInactive( DensityLabel );
          SetInactive( DensityUnitsLabel );
#endif
          // put null string in the disabled entry fields

          DensitySLE.SetText( msimNULL_STR );
     }

     if ( pSimInstance->temp_option != msimVAR_TEMP )
     {
          ThermCoeffsGB.Disable( );
          EnthalpyLabel.Disable( );
          EnthalpySLE.Disable( );
          EnthalpyUnitsLabel.Disable( );
          CoeffALabel.Disable( );
          CoeffASLE.Disable( );
          CoeffAUnits.Disable( );
          CoeffBLabel.Disable( );
          CoeffBSLE.Disable( );
          CoeffBUnits.Disable( );
          CoeffCLabel.Disable( );
          CoeffCSLE.Disable( );
          CoeffCUnits.Disable( );
          CoeffDLabel.Disable( );
          CoeffDSLE.Disable( );
          CoeffDUnits.Disable( );

#if defined(__MAC__)
          SetInactive( EnthalpyLabel );
          SetInactive( EnthalpyUnitsLabel );
          SetInactive( CoeffALabel );
          SetInactive( CoeffAUnits );
          SetInactive( CoeffBLabel );

          SetInactive( CoeffBUnits );
          SetInactive( CoeffCLabel );
          SetInactive( CoeffCUnits );
          SetInactive( CoeffDLabel );
          SetInactive( CoeffDUnits );
#endif

          // put null string in the disabled entry fields

          EnthalpySLE.SetText( msimNULL_STR );
          CoeffASLE.SetText( msimNULL_STR );
          CoeffBSLE.SetText( msimNULL_STR );
          CoeffCSLE.SetText( msimNULL_STR );
          CoeffDSLE.SetText( msimNULL_STR );
     }

     return;
}


void SpeciesDataDlg::UpdatePanel( )
{

     // display necessary information for the selected species

     SpeciesNameLabel.SetText( speciesptr->name );

     if ( pSimInstance->volume_option == msimVAR_VOL )
          DispPhysStateMolarDensity( );

     if ( pSimInstance->temp_option == msimVAR_TEMP )
          DispThermCoeff( );

     return;
}


void SpeciesDataDlg::DispPhysStateMolarDensity( )
{
     // display the data for the selected species

     SolidStateRB.Check( msimSOLID == speciesptr->phys_state );
     LiquidStateRB.Check( msimLIQUID == speciesptr->phys_state );
     GasStateRB.Check( msimGAS == speciesptr->phys_state );

     DensitySLE.SetText( String( speciesptr->molardensity ) );

     return;
}


void SpeciesDataDlg::DispThermCoeff( )
{
     // display the data for the selected species

     EnthalpySLE.SetText( speciesptr->thermcoeff[0] );
     CoeffASLE.SetText( speciesptr->thermcoeff[1] );
     CoeffBSLE.SetText( speciesptr->thermcoeff[2] );
     CoeffCSLE.SetText( speciesptr->thermcoeff[3] );
     CoeffDSLE.SetText( speciesptr->thermcoeff[4] );

     return;
}



void SpeciesDataDlg::ReadCurrentEntries( )
{
     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
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

     if ( pSimInstance->temp_option == msimVAR_TEMP )
     {
          // read the thermal coefficients

          msimStringCopy( speciesptr->thermcoeff[0], EnthalpySLE.GetText( ), sizeof speciesptr->thermcoeff[0] );
          msimStringCopy( speciesptr->thermcoeff[1], CoeffASLE.GetText( ), sizeof speciesptr->thermcoeff[1] );
          msimStringCopy( speciesptr->thermcoeff[2], CoeffBSLE.GetText( ), sizeof speciesptr->thermcoeff[2] );
          msimStringCopy( speciesptr->thermcoeff[3], CoeffCSLE.GetText( ), sizeof speciesptr->thermcoeff[3] );
          msimStringCopy( speciesptr->thermcoeff[4], CoeffDSLE.GetText( ), sizeof speciesptr->thermcoeff[4] );
     }
}


void SpeciesDataDlg::ListboxSelectHandler( ListBox PTR )
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


void SpeciesDataDlg::PhysStateHandler( RadioButton PTR pRadioButton )
{
#if defined(__MAC__)

     SolidStateRB.Check( pRadioButton == &SolidStateRB );
     LiquidStateRB.Check( pRadioButton == &LiquidStateRB );
     GasStateRB.Check( pRadioButton == &GasStateRB );
#endif

     data_altered = TRUE;
}



void SpeciesDataDlg::TextModifiedHandler( Edit PTR )
{
     data_altered = TRUE;
}


void SpeciesDataDlg::CloseHandler( PushButton PTR )
{
     // check the validity of the entry fields

     if ( data_altered )
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

     ReadCurrentEntries( );

     if ( ! ValidData( ) )
          return;

     if ( data_altered )
          pSimInstance->data_altered_since_lastsave = TRUE;


     pSimInstance->specieslist_altered = FALSE;

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


void SpeciesDataDlg::UndoHandler( PushButton PTR )
{
     RestoreInitialData( );

     UpdatePanel( );

     data_altered = FALSE;

     return;
}


void SpeciesDataDlg::DefaultsHandler( PushButton PTR )
{
     SetSpeciesDataToDefault( );

     UpdatePanel( );

     return;
}


msimBOOL SpeciesDataDlg::ValidData( )
{
     USHORT i;
     String message;
     PCHAR fields[] =
      {
           "the Enthalpy for",
           "Thermochem Coeff A for",
           "Thermochem Coeff B for",
           "Thermochem Coeff C for",
           "Thermochem Coeff D for"
      };


     // check the info read from the entry fields and
     // now contained in the species struct are valid.
     // First with the physical state and molar density

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
          if ( ! msimValidPositiveFloat( speciesptr->molardensity ) )
          {
               message = "The value [ " + String( speciesptr->molardensity ) +
               String( "] entered as the Molar Density for species \"" ) +
               String( speciesptr->name ) +
               String( "\" is invalid. The entry must be "
                    "a numeric value greater then zero." );

               WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

               return FALSE;
          }
     }

     // now check the validity of thermochemical coefficients //

     if ( pSimInstance->temp_option == msimVAR_TEMP )
     {
          for ( i = 0; i < msimNUMBER_OF_THERM_COEFFS; i++ )
          {
               if ( ! msimValidFloat( speciesptr->thermcoeff[i] ) )
               {
                    message = String( "The value [ " ) +
                    String( speciesptr->thermcoeff[i] ) +
                    String( "] entered as " ) + String( fields[i] ) +
                    String( " species \"" ) + String( speciesptr->name ) +
                    String( "\" is not valid numeric input." );

                    WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

                    return FALSE;
               }
          }

          // double check that the heat capacity ( coeff B) is != 0.0

          if ( ! msimValidNonZeroFloat( speciesptr->thermcoeff[2] ) )
          {
               message = String( "The Heat Capacity (Thermochem Coeff B) for species \"" ) +
               String( speciesptr->name ) +
               String( "\" cannot be equal to zero." );

               WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

               return FALSE;
          }
     }

     return TRUE;
}

SpeciesDataDlg::~SpeciesDataDlg( )
{
     if ( tempPhysState )
          delete[]tempPhysState;

     if ( tempDensity )
          delete[]tempDensity;

     if ( tempCoeffs )
          delete[]tempCoeffs;
}

VOID msimSpeciesDataDialog( msimWID Owner, msimPINSTANCE pInstance )
{
     SpeciesDataDlg PTR pdlg =
                        new SpeciesDataDlg( Owner, pInstance );

     if ( pdlg )
     {
          if ( pdlg->IsInitialized( ) )
               pdlg->Execute( );

          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;

}
