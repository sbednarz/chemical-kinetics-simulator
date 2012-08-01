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
//  EDITSPDB.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the class EditSpeciesDatabaseDlg 
//
//  CREATED : 5.11.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------





#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "editspdb.hxx"
#include "mainapp.hxx"
#include "schemfrm.hxx"

#endif


#include "editspdb.hrc"
#include "strings.hrc"





// ----------------------------------------------------------------------------
//                      class EditSpeciesDatabaseDlg 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class EditSpeciesDatabaseDlg : public ModalDialog
{

protected:
     ListBox species_names_listbox;
     FixedText listbox_label_text;
     FixedText aFixedText2;
     RadioButton solid_phase_rb;
     RadioButton liquid_phase_rb;
     RadioButton gas_phase_rb;
     FixedText density_label_text;
     Edit density_entryfield;
     FixedText density_units_text;
     FixedText enthalpy_label_text;
     Edit enthalpy_entryfield;
     FixedText enthalpy_units_text;
     FixedText a_label_text;
     Edit a_entryfield;
     FixedText a_units_text;
     FixedText b_label_text;
     Edit b_entryfield;
     FixedText b_units_text;
     FixedText c_label_text;
     Edit c_entryfield;
     FixedText c_units_text;
     FixedText d_label_text;
     Edit d_entryfield;
     FixedText d_units_text;
     OKButton close_btn;
     PushButton defaults_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText active_species_name_text;
     GroupBox therm_coeff_gb;
     GroupBox aGroupBox2;
     GroupBox phys_state_gb;

     SchemeFrameWindow*        p_parent;
     BOOL8                data_altered;
     BOOL8                current_data_altered;
     SpeciesDatabase*     p_database;
     SpeciesDatabaseEntry current_entry;

     void                 ClearDataAlteredFlag( );
     void                 SetDataAlteredFlag( );

     SchemeFrameWindow*        GetpParent() { return p_parent; }
     SpeciesDatabase*     GetpDatabase() const { return p_database; }

     void          Initialize( );
     void          SetPhaseData( );
     void          SetThermalCoeffs();
     void          InitializeListbox( );
     void          InitializeSpeciesData( );
     void          SetUnitsText();

     BOOL8         ReadUserInput( );
     BOOL8         ReadPhaseData( );
     BOOL8         ReadThermalCoeffs( );

     void          EntryError( const NumericString& UserEntry, const String& rLabel );

public:

     EditSpeciesDatabaseDlg( SchemeFrameWindow* pParent );

     void TextModifiedHandler( Edit* );
     void ListboxSelectHandler( ListBox* );
     void CloseBtnHandler( OKButton* );
     void UndoBtnHandler( PushButton* );
     void DefaultsBtnHandler( PushButton* );
     void RadioBtnHandler( RadioButton* );
};





// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::EditSpeciesDatabaseDlg( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

EditSpeciesDatabaseDlg::EditSpeciesDatabaseDlg( SchemeFrameWindow * pParent ) :
ModalDialog( pParent, ResId( vID_DLG_SPECIES_DATA ) ),
species_names_listbox( this, ResId( vID_LB_SPECIES_NAMES ) ),
listbox_label_text( this, ResId( vID_STEXT_SPECIES_LABEL ) ),
aFixedText2( this, ResId( 2 ) ),
solid_phase_rb( this, ResId( vID_RB_PHASE_SOLID ) ),
liquid_phase_rb( this, ResId( vID_RB_PHASE_LIQUID ) ),
gas_phase_rb( this, ResId( vID_RB_PHASE_GAS ) ),
density_label_text( this, ResId( vID_STEXT_DENSITY_LABEL ) ),
density_entryfield( this, ResId( vID_EF_DENSITY ) ),
density_units_text( this, ResId( vID_STEXT_DENSITY_UNITS ) ),
enthalpy_label_text( this, ResId( vID_STEXT_ENTHALPY_LABEL ) ),
enthalpy_entryfield( this, ResId( vID_EF_ENTHALPY ) ),
enthalpy_units_text( this, ResId( vID_STEXT_ENTHALPY_UNITS ) ),
a_label_text( this, ResId( vID_STEXT_A_LABEL ) ),
a_entryfield( this, ResId( vID_EF_A ) ),
a_units_text( this, ResId( vID_STEXT_A_UNITS ) ),
b_label_text( this, ResId( vID_STEXT_B_LABEL ) ),
b_entryfield( this, ResId( vID_EF_B ) ),
b_units_text( this, ResId( vID_STEXT_B_UNITS ) ),
c_label_text( this, ResId( vID_STEXT_C_LABEL ) ),
c_entryfield( this, ResId( vID_EF_C ) ),
c_units_text( this, ResId( vID_STEXT_C_UNITS ) ),
d_label_text( this, ResId( vID_STEXT_D_LABEL ) ),
d_entryfield( this, ResId( vID_EF_D ) ),
d_units_text( this, ResId( vID_STEXT_D_UNITS ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
active_species_name_text( this, ResId( vID_STEXT_SPECIES_NAME ) ),
therm_coeff_gb( this, ResId( vID_GB_COEFF_LABEL ) ),
aGroupBox2( this, ResId( 2 ) ),
phys_state_gb( this, ResId( vID_GB_STATE_LABEL ) ),
p_parent( pParent),
data_altered( FALSE ),
current_data_altered( FALSE ),
p_database( GetpParent()->GetReactionScheme().GetpSpeciesDatabase() ),
current_entry( )
{
     // use assignment operator which does not access certain object data members

     current_entry = GetpDatabase()->GetSpeciesDataByPosition( 0 ); 

     FreeResource();
     SetUnitsText();
     Initialize();

     density_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) ); 
     enthalpy_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) );  
     a_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) );  
     b_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) );  
     c_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) );  
     d_entryfield.ChangeModifyHdl( LINK( this, EditSpeciesDatabaseDlg, TextModifiedHandler ) );  

     species_names_listbox.ChangeSelectHdl( LINK( this, EditSpeciesDatabaseDlg, ListboxSelectHandler ) );

     close_btn.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,CloseBtnHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,UndoBtnHandler ) );
     defaults_btn.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,DefaultsBtnHandler ) );

     solid_phase_rb.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,RadioBtnHandler ) ); 
     liquid_phase_rb.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,RadioBtnHandler ) ); 
     gas_phase_rb.ChangeClickHdl( LINK( this, EditSpeciesDatabaseDlg,RadioBtnHandler ) );
     
     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::SetUnitsText()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::SetUnitsText( )
{
     ReactionScheme& rscheme = GetpParent()->GetReactionScheme();

     density_units_text.SetText( rscheme.GetConcentrationUnits() );

     enthalpy_units_text.SetText( rscheme.GetLargeEnergyUnits() + 
                                  String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                  rscheme.GetAmountUnits() );

     a_units_text.SetText( rscheme.GetSmallEnergyUnits() + 
                                  String( ResId( vID_STR_SYMBOL_SLASH ) ) +
                                  rscheme.GetAmountUnits() );

     b_units_text.SetText( a_units_text.GetText() + 
                          String( ResId( vID_STR_SYMBOL_DASH ) ) +
                                  rscheme.GetTemperatureUnits() );

     c_units_text.SetText( b_units_text.GetText() + 
                          String( ResId( vID_STR_SYMBOL_EXPONENT_2 ) )
     );                                        

     d_units_text.SetText( b_units_text.GetText() + 
                          String( ResId( vID_STR_SYMBOL_EXPONENT_3 ) )
     );                                        

}





// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::ClearDataAlteredFlag( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::ClearDataAlteredFlag( )
{
     current_data_altered = FALSE;
     undo_btn.Disable( );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::DefaultsBtnHandler( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::DefaultsBtnHandler( PushButton* )
{

     current_entry = SpeciesDatabaseEntry();
     InitializeSpeciesData();
     SetDataAlteredFlag( );
}




// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::SetDataAlteredFlag( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  EditSpeciesDatabaseDlg::SetDataAlteredFlag( )
{
     current_data_altered = TRUE;
     undo_btn.Enable( );
}




// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::RadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  EditSpeciesDatabaseDlg::RadioBtnHandler( RadioButton* )
{
     if ( !current_data_altered ) 
          SetDataAlteredFlag( );
}





// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::UndoBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  EditSpeciesDatabaseDlg::UndoBtnHandler( PushButton* )
{
     current_entry = GetpDatabase()->GetSpeciesDataByPosition( 
      species_names_listbox.GetSelectEntryPos() );

     InitializeSpeciesData();
}



// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::CloseBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  EditSpeciesDatabaseDlg::CloseBtnHandler( OKButton* )
{
     if ( current_data_altered )
          if ( !ReadUserInput() )
               return;

     EndDialog( data_altered ? ED_RC_ACCEPT : ED_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::ReadUserInput( )
// ............................................................................
// 
// for reading controls - returns TRUE is no syntax errors, 
// FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 EditSpeciesDatabaseDlg::ReadUserInput( )
{
     if ( ReadPhaseData() && ReadThermalCoeffs() )
     {
          GetpDatabase()->SetSpeciesDataByName( 
           active_species_name_text.GetText(), current_entry );

          data_altered = TRUE;

          return TRUE; 
     }
     else
          return FALSE;
}







// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::EntryError( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::EntryError( const NumericString& UserEntry, 
 const String& rLabel )
{
     String message = String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR1 ) ) + 
      UserEntry + 
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR2 ) ) +
      rLabel +
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR3 ) );

      WarningBox( this, WB_OK | WB_DEF_OK, message ).Execute( );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::ReadPhaseData( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 EditSpeciesDatabaseDlg::ReadPhaseData( )
{
     NumericString str = density_entryfield.GetText( );

     if ( !str.IsValidPositiveFLOAT64( ) ) 
     {
          EntryError( str, density_label_text.GetText() );
          return FALSE;
     }
     else
          current_entry.SetMolarDensity( str );


     if ( solid_phase_rb.IsChecked() )
          current_entry.SetPhysicalState( SOLID_STATE );
     else
         if ( liquid_phase_rb.IsChecked() )
               current_entry.SetPhysicalState( LIQUID_STATE ); 
         else
              if ( gas_phase_rb.IsChecked() )
                    current_entry.SetPhysicalState( GAS_STATE ); 

     return TRUE;
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::ReadThermalCoeffs( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 EditSpeciesDatabaseDlg::ReadThermalCoeffs( )
{
     NumericString str1 = enthalpy_entryfield.GetText( ); 
     NumericString str2 = a_entryfield.GetText( );
     NumericString str3 = b_entryfield.GetText( ); 
     NumericString str4 = c_entryfield.GetText( ); 
     NumericString str5 = d_entryfield.GetText( ); 


    if ( !str1.IsValidFLOAT64() ) 
    {
          EntryError( str1, enthalpy_label_text.GetText() );
          return FALSE;
    } 

    if ( !str2.IsValidFLOAT64() ) 
    {
          EntryError( str2, a_label_text.GetText() );
          return FALSE;
    } 

    if ( !str3.IsValidNonZeroFLOAT64() ) 
    {
          EntryError( str3, b_label_text.GetText() );
          return FALSE;
    } 

    if ( !str4.IsValidFLOAT64() ) 
    {
          EntryError( str4, c_label_text.GetText() );
          return FALSE;
    } 

    if ( !str5.IsValidFLOAT64() ) 
    {
          EntryError( str5, d_label_text.GetText() );
          return FALSE;
    } 

     // if we got here then evrything is OK 

    current_entry.SetEnthalpy( str1 ); 
    current_entry.SetThermCoeffA( str2 );
    current_entry.SetThermCoeffB( str3 ); 
    current_entry.SetThermCoeffC( str4 ); 
    current_entry.SetThermCoeffD( str5 ); 

    return TRUE;
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::ListboxSelectHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::ListboxSelectHandler( ListBox* )
{
     if ( current_data_altered && !ReadUserInput() )
               return;

     // get new entry data 

      active_species_name_text.SetText( species_names_listbox.GetSelectEntry() );

      current_entry = GetpDatabase()->GetSpeciesDataByPosition( 
       species_names_listbox.GetSelectEntryPos() );

      InitializeSpeciesData();
}





// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::InitializeSpeciesData( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::InitializeSpeciesData( )
{
     // eventually need to query whether therm coeffs and phase data 
     // are active or not 

     SetPhaseData();
     SetThermalCoeffs();

     ClearDataAlteredFlag( );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::TextModifiedHandler( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::TextModifiedHandler( Edit* )
{
     if ( !current_data_altered ) 
          SetDataAlteredFlag( );
}

  



// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::SetPhaseData( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::SetPhaseData( )
{
     solid_phase_rb.Check( SOLID_STATE == current_entry.GetPhysicalState() );
     liquid_phase_rb.Check( LIQUID_STATE == current_entry.GetPhysicalState() ); 
     gas_phase_rb.Check( GAS_STATE == current_entry.GetPhysicalState() ); 

     density_entryfield.SetText( current_entry.GetMolarDensity() );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::SetThermalCoeffs( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::SetThermalCoeffs()
{
     enthalpy_entryfield.SetText( current_entry.GetEnthalpy() ); 
     a_entryfield.SetText( current_entry.GetThermCoeffA() );
     b_entryfield.SetText( current_entry.GetThermCoeffB() ); 
     c_entryfield.SetText( current_entry.GetThermCoeffC() ); 
     d_entryfield.SetText( current_entry.GetThermCoeffD() ); 
}
     




// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::InitializeListbox( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::InitializeListbox( )
{
     species_names_listbox.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     species_names_listbox.Clear( );

     // initialize from Reaction Scheme SpeciesDatabase

     // first get the Species database so we can retrieve the species names 

     SpeciesDatabase* p_db = GetpParent()->GetReactionScheme().GetpSpeciesDatabase();

     UINT16 num_species = p_db->GetNumberOfSpecies( );

     for ( UINT16 i = 0; i < num_species; i++ )
     {
          const SpeciesDatabaseEntry& r_entry = p_db->GetSpeciesDataByPosition( i );
          species_names_listbox.InsertEntry( r_entry.GetName( ) );
     }

     species_names_listbox.SelectEntryPos( 0 );

     species_names_listbox.ChangeUpdateMode( TRUE );
     species_names_listbox.Update( );

     active_species_name_text.SetText( species_names_listbox.GetSelectEntry() );
}






// ----------------------------------------------------------------------------
//                EditSpeciesDatabaseDlg::Initialize( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void EditSpeciesDatabaseDlg::Initialize( )
{
     // first initialize listbox with species names 

     InitializeListbox();
     InitializeSpeciesData();
}







// ----------------------------------------------------------------------------
//                              EditSpeciesDatabase( ) 
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

EditSDbRC EditSpeciesDatabase( SchemeFrameWindow* pParent )
{
     EditSpeciesDatabaseDlg* pdlg = new EditSpeciesDatabaseDlg( pParent );

     if ( pdlg )
     {
           return (EditSDbRC) pdlg->Execute();
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return ED_RC_FAIL;
     }
}


