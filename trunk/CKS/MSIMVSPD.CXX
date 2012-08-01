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
/*  FILE NAME : msimvspd.cxx                                                */
/*                                                                          */
/*  This module implements a dialog for verifying species data              */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for multiple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1993                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <stdlib.h>




class VerifySpecDataDlg : public ModalDialog
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
     FixedText EnthalpyLabel;
     Edit EnthalpySLE;
     FixedText EnthalpyUnits;
     FixedText SpecDataALabel;
     Edit ThermCoeffASLE;
     FixedText SpecDataAUnits;
     FixedText SpecDataBLabel;
     Edit ThermCoeffBSLE;
     FixedText SpecDataBUnits;
     FixedText SpecDataCLabel;
     Edit ThermCoeffCSLE;
     FixedText SpecDataCUnits;
     FixedText SpecDataDLabel;
     Edit ThermCoeffDSLE;
     FixedText SpecDataDUnits;
     GroupBox ConcLabelGB;
     GroupBox SpecListGB;
     GroupBox StateGB;
     GroupBox ThermCoeffGB;
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
     msimREAL_STRING_ARRAY PTR tempCoeffs;
     msimBOOL data_altered;
     msimWID owner;
     msimBOOL initialized;

     void RestoreInitialData( );
     void SetSpeciesDataToDefault( );

     void AllocateAndInitializeConcArray( );
     void InitializeEntries( );
     void UpdatePanel( );
     void DispPhysStateMolarDensity( );
     void DispThermCoeff( );
     msimBOOL ValidData( );
     void ReadCurrentEntries( );
     void InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance );
     void RestoreInitialConcData( );

public :

     VerifySpecDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance );
     ~VerifySpecDataDlg( );

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

VerifySpecDataDlg::~VerifySpecDataDlg( )
{
     if ( tempInitConc )
          delete[]tempInitConc;

     if ( tempPhysState )
          delete[]tempPhysState;

     if ( tempDensity )
          delete[]tempDensity;

     if ( tempCoeffs )
          delete[]tempCoeffs;
}



VerifySpecDataDlg::VerifySpecDataDlg( Window PTR pParent, msimPINSTANCE PSimInstance ) :
ModalDialog ( pParent, ResId ( msimVER_SPECDATA_PANEL ) ),
SpeciesDataED1Text ( this, ResId ( msimVER_SPECDATA_ED_1 ) ),
SpeciesNameLabel ( this, ResId ( msimVER_SPECDATA_NAME_STR ) ),
FixedText1 ( this, ResId ( 1 ) ),
FixedText2 ( this, ResId ( 2 ) ),
ConcSLE ( this, ResId ( msimVER_SPECDATA_CONC ) ),
ConcUnitsLabel ( this, ResId ( msimVER_SPECDATA_CONCUNITS ) ),
SpeciesNameLB ( this, ResId ( msimVER_SPECDATA_LISTBOX ) ),
SolidStateRB ( this, ResId ( msimVER_SPECDATA_SOLID ) ),
LiquidStateRB ( this, ResId ( msimVER_SPECDATA_LIQUID ) ),
GasStateRB ( this, ResId ( msimVER_SPECDATA_GAS ) ),
DensityLabel ( this, ResId ( msimVER_SPECDATA_DENSITY_LABEL ) ),
DensitySLE ( this, ResId ( msimVER_SPECDATA_DENSITY ) ),
DensityUnitsLabel ( this, ResId ( msimVER_SPECDATA_DENSUNITS ) ),
EnthalpyLabel ( this, ResId ( msimVER_SPECDATA_ENTHALPY ) ),
EnthalpySLE ( this, ResId ( msimVER_SPECDATA_ENTHALPY ) ),
EnthalpyUnits ( this, ResId ( msimVER_SPECDATA_ENTH_UNITS ) ),
SpecDataALabel ( this, ResId ( msimVER_SPECDATA_A_LABEL ) ),
ThermCoeffASLE ( this, ResId ( msimVER_SPECDATA_A ) ),
SpecDataAUnits ( this, ResId ( msimVER_SPECDATA_A_UNITS ) ),
SpecDataBLabel ( this, ResId ( msimVER_SPECDATA_B_LABEL ) ),
ThermCoeffBSLE ( this, ResId ( msimVER_SPECDATA_B ) ),
SpecDataBUnits ( this, ResId ( msimVER_SPECDATA_B_UNITS ) ),
SpecDataCLabel ( this, ResId ( msimVER_SPECDATA_C_LABEL ) ),
ThermCoeffCSLE ( this, ResId ( msimVER_SPECDATA_C ) ),
SpecDataCUnits ( this, ResId ( msimVER_SPECDATA_C_UNITS ) ),
SpecDataDLabel ( this, ResId ( msimVER_SPECDATA_D_LABEL ) ),
ThermCoeffDSLE ( this, ResId ( msimVER_SPECDATA_D ) ),
SpecDataDUnits ( this, ResId ( msimVER_SPECDATA_D_UNITS ) ),
ConcLabelGB ( this, ResId ( msimVER_SPECDATA_CONC_LABEL ) ),
SpecListGB ( this, ResId ( msimVER_SPECDATA_LIST ) ),
StateGB ( this, ResId ( msimVER_SPECDATA_STATE_LABEL ) ),
ThermCoeffGB ( this, ResId ( msimVER_SPECDATA_COEFFLABEL ) ),
ClosePB ( this, ResId ( msimVER_SPECDATA_CLOSE ) ),
CancelPB ( this, ResId ( msimVER_SPECDATA_CANCEL ) ),
DefaultsPB ( this, ResId ( msimVER_SPECDATA_DEFAULTS ) ),
UndoPB ( this, ResId ( msimVER_SPECDATA_UNDO ) ),
HelpPB ( this, ResId ( msimVER_SPECDATA_HELP ) )
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
          LINK( this, VerifySpecDataDlg, ListboxSelectHandler ) );

     SolidStateRB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, PhysStateHandler ) );
     LiquidStateRB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, PhysStateHandler ) );
     GasStateRB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, PhysStateHandler ) );

     ConcSLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     DensitySLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     EnthalpySLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     ThermCoeffASLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     ThermCoeffBSLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     ThermCoeffCSLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );
     ThermCoeffDSLE.ChangeModifyHdl(
          LINK( this, VerifySpecDataDlg, TextModifiedHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, CloseHandler ) );

     CancelPB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, CancelHandler ) );

     DefaultsPB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, DefaultsHandler ) );

     UndoPB.ChangeClickHdl(
          LINK( this, VerifySpecDataDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );
}


void VerifySpecDataDlg::AllocateAndInitializeConcArray( )
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

void VerifySpecDataDlg::RestoreInitialConcData( )
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


void VerifySpecDataDlg::InitializeEntries( )
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

     // initialize the initial concentration label and its units,
     // the molar density unit and thermochemical coefficient units

     // save conc_amount in variable to use later in error messages if needed

     if ( pSimInstance->volume_option == msimVAR_VOL )
          conc_amount = String ( ResId( msimVER_SPECDATA_STR19 ) ) ;
     else
          conc_amount = String ( ResId( msimVER_SPECDATA_STR20 ) ) ;

     ConcLabelGB.SetText( conc_amount );

     ConcUnitsLabel.SetText(
          String( msimConcUnits( pSimInstance->conc_units,
                    pSimInstance->volume_option ) ) );

     DensityUnitsLabel.SetText( String( "moles/" ) +
          String( msimVolumeUnits( pSimInstance->volume_units ) ) );

     EnthalpyUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) ) +
          String( "/mol" ) );

     SpecDataAUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) ) +
          String( "/mole" ) );

     SpecDataBUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) ) +
          String( "/mole-deg" ) );

     SpecDataCUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) ) +
          String( "/mole-deg^2" ) );
     SpecDataDUnits.SetText(
          String( msimEnergyUnits( pSimInstance->energy_units ) ) +
          String( "/mole-deg^3" ) );

     // disable the proper panel depending on the volume and temperature
     // options

     if ( pSimInstance->volume_option != msimVAR_VOL )
     {
          StateGB.Disable( );
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
          // put null string in the disabled entry field

          DensitySLE.SetText( msimNULL_STR );
     }

     if ( pSimInstance->temp_option != msimVAR_TEMP )
     {
          ThermCoeffGB.Disable( );
          EnthalpyLabel.Disable( );
          EnthalpySLE.Disable( );
          EnthalpyUnits.Disable( );
          SpecDataALabel.Disable( );
          ThermCoeffASLE.Disable( );
          SpecDataAUnits.Disable( );
          SpecDataBLabel.Disable( );
          ThermCoeffBSLE.Disable( );
          SpecDataBUnits.Disable( );
          SpecDataCLabel.Disable( );
          ThermCoeffCSLE.Disable( );
          SpecDataCUnits.Disable( );
          SpecDataDLabel.Disable( );
          ThermCoeffDSLE.Disable( );
          SpecDataDUnits.Disable( );

#if defined(__MAC__)
          SetInactive( EnthalpyLabel );
          SetInactive( EnthalpyUnits );
          SetInactive( SpecDataALabel );
          SetInactive( SpecDataAUnits );
          SetInactive( SpecDataBLabel );

          SetInactive( SpecDataBUnits );
          SetInactive( SpecDataCLabel );
          SetInactive( SpecDataCUnits );
          SetInactive( SpecDataDLabel );
          SetInactive( SpecDataDUnits );
#endif

          // put null string in the disabled entry fields

          EnthalpySLE.SetText( msimNULL_STR );
          ThermCoeffASLE.SetText( msimNULL_STR );
          ThermCoeffBSLE.SetText( msimNULL_STR );
          ThermCoeffCSLE.SetText( msimNULL_STR );
          ThermCoeffDSLE.SetText( msimNULL_STR );
     }

     return;
}





void VerifySpecDataDlg::UpdatePanel( )
{
     // display necessary information for the selected species

     SpeciesNameLabel.SetText( String( speciesptr->name ) );

     ConcSLE.SetText( String( speciesptr->initialconc ) );

     if ( pSimInstance->volume_option == msimVAR_VOL )
          DispPhysStateMolarDensity( );

     if ( pSimInstance->temp_option == msimVAR_TEMP )
          DispThermCoeff( );

     return;
}



void VerifySpecDataDlg::DispPhysStateMolarDensity( )
{
     // display the data for the selected species

     SolidStateRB.Check( speciesptr->phys_state == msimSOLID );
     LiquidStateRB.Check( speciesptr->phys_state == msimLIQUID );
     GasStateRB.Check( speciesptr->phys_state == msimGAS );

     DensitySLE.SetText( String( speciesptr->molardensity ) );

     return;
}


void VerifySpecDataDlg::DispThermCoeff( )
{
     // display the data for the selected species

     EnthalpySLE.SetText( speciesptr->thermcoeff[0] );
     ThermCoeffASLE.SetText( speciesptr->thermcoeff[1] );
     ThermCoeffBSLE.SetText( speciesptr->thermcoeff[2] );
     ThermCoeffCSLE.SetText( speciesptr->thermcoeff[3] );
     ThermCoeffDSLE.SetText( speciesptr->thermcoeff[4] );

     return;
}


void VerifySpecDataDlg::ReadCurrentEntries( )
{
     msimStringCopy( speciesptr->initialconc, ConcSLE.GetText( ),
          sizeof speciesptr->initialconc );

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
          msimStringCopy( speciesptr->thermcoeff[1], ThermCoeffASLE.GetText( ), sizeof speciesptr->thermcoeff[1] );
          msimStringCopy( speciesptr->thermcoeff[2], ThermCoeffBSLE.GetText( ), sizeof speciesptr->thermcoeff[2] );
          msimStringCopy( speciesptr->thermcoeff[3], ThermCoeffCSLE.GetText( ), sizeof speciesptr->thermcoeff[3] );
          msimStringCopy( speciesptr->thermcoeff[4], ThermCoeffDSLE.GetText( ), sizeof speciesptr->thermcoeff[4] );
     }
}


void VerifySpecDataDlg::ListboxSelectHandler( ListBox PTR )
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



void VerifySpecDataDlg::PhysStateHandler( RadioButton PTR pRadioButton )
{

#if defined(__MAC__)

     SolidStateRB.Check( pRadioButton == &SolidStateRB );
     LiquidStateRB.Check( pRadioButton == &LiquidStateRB );
     GasStateRB.Check( pRadioButton == &GasStateRB );

#endif

     data_altered = TRUE;

     return;
}


void VerifySpecDataDlg::TextModifiedHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}


void VerifySpecDataDlg::CloseHandler( PushButton PTR )
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


void VerifySpecDataDlg::CancelHandler( PushButton PTR )
{
     pSimInstance->specieslist_altered = FALSE;

     EndDialog( msimUSER_ABORT );

     return;
}


void VerifySpecDataDlg::UndoHandler( PushButton PTR )
{
     RestoreInitialData( );

     RestoreInitialConcData( );

     UpdatePanel( );

     data_altered = FALSE;

     return;
}


void VerifySpecDataDlg::DefaultsHandler( PushButton PTR )
{
     SetSpeciesDataToDefault( );

     UpdatePanel( );

     return;
}


msimBOOL VerifySpecDataDlg::ValidData( )
{
     USHORT i;
     String message;

     // check the info read from the entry fields and
     // now contained in the species struct are valid.

     // first, with the initial concentration

     if ( ! msimValidNonNegativeFloat( speciesptr->initialconc ) )
     {
          message = String( ResId( msimVER_SPECDATA_STR1) )
                  + String( speciesptr->initialconc )
                  + String( ResId( msimVER_SPECDATA_STR2) )  
                  + conc_amount
                  + String( ResId( msimVER_SPECDATA_STR3) )   
                  + String( speciesptr->name )
                  + String( ResId( msimVER_SPECDATA_STR4) );
        

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }


     // Then with the physical state and molar density

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
          if ( ! msimValidPositiveFloat( speciesptr->molardensity ) )
          {
               message = String( ResId( msimVER_SPECDATA_STR5 ) )
                       + String( speciesptr->molardensity )
                       + String( ResId( msimVER_SPECDATA_STR6 ) )
                       + String( speciesptr->name )
                       + String( ResId( msimVER_SPECDATA_STR7) );

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
                    String fields[5];

                    fields[0] = String( ResId( msimVER_SPECDATA_STR12) ); 
                    fields[1] = String( ResId( msimVER_SPECDATA_STR13) ); 
                    fields[2] = String( ResId( msimVER_SPECDATA_STR14) ); 
                    fields[3] = String( ResId( msimVER_SPECDATA_STR15) ); 
                    fields[4] = String( ResId( msimVER_SPECDATA_STR16) ); 

                    message = String( ResId( msimVER_SPECDATA_STR8 ) ) 
                            + String( speciesptr->thermcoeff[i] )
                            + String( ResId( msimVER_SPECDATA_STR9 ) ) 
                            + fields[i] 
                            + String( ResId( msimVER_SPECDATA_STR10) ) 
                            + String( speciesptr->name )
                            + String( ResId( msimVER_SPECDATA_STR11) ); 

                    WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

                    return FALSE;
               }
          }

          // double check that the heat capacity ( coeff B) is != 0.0

          if ( ! msimValidNonZeroFloat( speciesptr->thermcoeff[2] ) )
          {
               message = String( ResId( msimVER_SPECDATA_STR17) )
                       + String( speciesptr->name )
                       + String( ResId( msimVER_SPECDATA_STR18));

               WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

               return FALSE;
          }
     }

     return TRUE;
}


void VerifySpecDataDlg::RestoreInitialData( )
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
               msimStringCopy( curspecptr->molardensity, tempDensity[j++], sizeof curspecptr->molardensity );

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


void VerifySpecDataDlg::SetSpeciesDataToDefault( )
{
     // set relevant fields to default values

     if ( pSimInstance->volume_option == msimVAR_VOL )
     {
          strcpy( speciesptr->molardensity, msimC_SPECIES.molardensity );
          speciesptr->phys_state = msimC_SPECIES.phys_state;
     }

     if ( pSimInstance->temp_option == msimVAR_TEMP )
          for ( USHORT i = 0; i < msimNUMBER_OF_THERM_COEFFS; i++ )
               strcpy( speciesptr->thermcoeff[i], msimC_SPECIES.thermcoeff[i] );

     data_altered = TRUE;
}





void VerifySpecDataDlg::InitSpeciesData( Window PTR pParent, msimPINSTANCE PSimInstance )
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
                         msimStringCopy( tempCoeffs[j][i],
                              curspecptr->thermcoeff[i], sizeof tempCoeffs[0][0] );

                    j++;
                    curspecptr = curspecptr->next;
               }

          }
     }
     else tempCoeffs = NULL;
}



msimRC msimVerifySpeciesDataDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     msimRC rc;

     VerifySpecDataDlg PTR pdlg =
                           new VerifySpecDataDlg( Owner, pInstance );

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