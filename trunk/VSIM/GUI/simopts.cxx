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
//  SIMOPTS.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SimulationOptionDlg class
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

#include "simopts.hxx"
#include "mainapp.hxx"

#endif

#include "simopts.hrc"
#include "strings.hrc"





// ----------------------------------------------------------------------------
//                  class SimulationOptionDlg 
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

class SimulationOptionDlg : public ModalDialog
{

protected:
     Edit num_mols_entryfield;
     Edit record_interval_entryfield;
     Edit random_num_seed_entryfield;
     Edit num_events_entryfield;
     Edit elapsed_time_limit_entryfield;
     RadioButton enable_equil_detect_rb;
     RadioButton disable_equil_detect_rb;
     Edit cycle_count_entryfield;
     Edit min_efficiency_entryfield;
     OKButton close_btn;
     PushButton defaults_btn;
     PushButton undo_btn;
     HelpButton help_btn;
     FixedText num_mols_label;
     FixedText record_interval_label;
     FixedText random_num_seed_label;
     FixedText num_events_label;
     FixedText elapsed_time_limit_label;
     FixedText min_efficiency_label;
     FixedText aFixedText8;
     FixedText cycle_count_label;
     FixedText et_limits_units_text;
     FixedText aFixedText12;
     FixedText aFixedText13;
     GroupBox aGroupBox1;
     GroupBox aGroupBox2;
     GroupBox aGroupBox3;

     SchemeFrameWindow*        p_parent;
     BOOL8                data_altered;

     BOOL8 IsDataAltered() const { return data_altered; }
     void  SetDataAltered( const BOOL8 NewState ) { data_altered = NewState; }
     SchemeFrameWindow*   GetpParent() { return p_parent; }

     void          Initialize( );
     void          SetEquilDetectEntryFieldState( BOOL8 IsActive );

     BOOL8         ReadUserInput( );

     void          EntryError( const NumericString& UserEntry, const String& rLabel );

public:
     SimulationOptionDlg( SchemeFrameWindow * pParent );

     void TextModifiedHandler( Edit* );
     void CloseBtnHandler( PushButton* );
     void RadioBtnHandler( RadioButton* );
};






// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::Initialize()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::Initialize( )
{
     ReactionScheme& rscheme = GetpParent()->GetReactionScheme();

     num_mols_entryfield.SetText( rscheme.GetNumMols() );
     record_interval_entryfield.SetText( rscheme.GetRecordInterval() ); 
     random_num_seed_entryfield.SetText( rscheme.GetRandomNumSeed() ); 
     num_events_entryfield.SetText( rscheme.GetMaxNumEvents() ); 
     elapsed_time_limit_entryfield.SetText( rscheme.GetElapsedTimeLimit() ); 
     cycle_count_entryfield.SetText( rscheme.GetEquilCycleCount() ); 
     min_efficiency_entryfield.SetText( rscheme.GetMinEquilEfficiency() ); 

     enable_equil_detect_rb.Check( TRUE == rscheme.IsEquilDetectActive() );  
     disable_equil_detect_rb.Check( FALSE == rscheme.IsEquilDetectActive() );   

     SetEquilDetectEntryFieldState( rscheme.IsEquilDetectActive() );
     et_limits_units_text.SetText( rscheme.GetTimeUnits() );
}






// ----------------------------------------------------------------------------
//              SimulationOptionDlg::SetEquilDetectEntryFieldState()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::SetEquilDetectEntryFieldState( BOOL8 IsActive )
{
     if ( IsActive )
     {
          cycle_count_entryfield.Enable();
          min_efficiency_entryfield.Enable();
     }
     else
     {
          cycle_count_entryfield.Disable();
          min_efficiency_entryfield.Disable();
     }
}




// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::ReadUserInput()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 SimulationOptionDlg::ReadUserInput( )
{
     ReactionScheme& rscheme = GetpParent()->GetReactionScheme();

     NumericString str1 = num_mols_entryfield.GetText( );
     NumericString str2 = record_interval_entryfield.GetText( );
     NumericString str3 = random_num_seed_entryfield.GetText( );
     NumericString str4 = num_events_entryfield.GetText( );
     NumericString str5 = elapsed_time_limit_entryfield.GetText( );
     NumericString str6 = cycle_count_entryfield.GetText( );
     NumericString str7 = min_efficiency_entryfield.GetText( );


     if ( !str1.IsValidUINT32() ) 
     {
          EntryError( str1, num_mols_label.GetText() );
          return FALSE;
     }


     if ( !str2.IsValidUINT32() ) 
     {
          EntryError( str2, record_interval_label.GetText() );
          return FALSE;
     }


     if ( !str3.IsValidPositiveINT16() ) 
     {
          EntryError( str3, random_num_seed_label.GetText() );
          return FALSE;
     }


     if ( !str4.IsValidUINT32() ) 
     {
          EntryError( str4, num_events_label.GetText() );
          return FALSE;
     }


     if ( !str5.IsValidNonNegativeFLOAT64() ) 
     {
          EntryError( str5, elapsed_time_limit_label.GetText() );
          return FALSE;
     }


     if ( !str6.IsValidUINT32() ) 
     {
          EntryError( str6, cycle_count_label.GetText() );
          return FALSE;
     }


     if ( !str7.IsValidPositiveFLOAT64() ) 
     {
          EntryError( str7, min_efficiency_label.GetText() );
          return FALSE;
     }


     rscheme.SetEquilDetectActive( enable_equil_detect_rb.IsChecked() );  

     // if we get to here eveything is ok - save and return 
     
     rscheme.SetNumMols( str1 );
     rscheme.SetRecordInterval( str2 ); 
     rscheme.SetRandomNumSeed( str3 ); 
     rscheme.SetMaxNumEvents( str4 ); 
     rscheme.SetElapsedTimeLimit( str5 ); 
     rscheme.SetEquilCycleCount( str6 ); 
     rscheme.SetMinEquilEfficiency( str7 ); 

     return TRUE;
}






// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::EntryError()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::EntryError( const NumericString& UserEntry, const String& rLabel )
{
     String message = String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR1 ) ) + 
      UserEntry + 
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR2 ) ) +
      rLabel +
      String( ResId( vID_STR_SIMPL_DLG_INPUT_ERR3 ) );

      WarningBox( this, WB_OK | WB_DEF_OK, message ).Execute( );
}





// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::TextModifiedHandler()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::TextModifiedHandler( Edit* )
{
     SetDataAltered( TRUE );
}






// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::CloseBtnHandler()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::CloseBtnHandler( PushButton* )
{
     if ( IsDataAltered() )
     {
          if ( ReadUserInput() )
               EndDialog( SO_RC_ACCEPT );
          else
               return;
     }
     else
          EndDialog( (short) SO_RC_CANCEL );
}







// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::RadioBtnHandler()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void SimulationOptionDlg::RadioBtnHandler( RadioButton* )
{
     SetDataAltered( TRUE );

     SetEquilDetectEntryFieldState( enable_equil_detect_rb.IsChecked() );
}








// ----------------------------------------------------------------------------
//                  SimulationOptionDlg::SimulationOptionDlg()
// ............................................................................
// 
//   constructor
//
// ----------------------------------------------------------------------------

SimulationOptionDlg::SimulationOptionDlg( SchemeFrameWindow * pParent ) :
ModalDialog( pParent, ResId( vID_DLG_SIMULATION_OPTIONS ) ),
num_mols_entryfield( this, ResId( vID_EF_NUM_MOLS ) ),
record_interval_entryfield( this, ResId( vID_EF_RECORD_INTERVAL ) ),
random_num_seed_entryfield( this, ResId( vID_EF_RND_NUM_SEED ) ),
num_events_entryfield( this, ResId( vID_EF_NUM_EVENTS ) ),
elapsed_time_limit_entryfield( this, ResId( vID_EF_ELAPSED_TIME_LIMIT ) ),
enable_equil_detect_rb( this, ResId( vID_RB_ENABLE_EQUIL_DETECT ) ),
disable_equil_detect_rb( this, ResId( vID_RB_DISABLE_EQUIL_DETECT ) ),
cycle_count_entryfield( this, ResId( vID_EF_CYCLE_COUNT ) ),
min_efficiency_entryfield( this, ResId( vID_EF_MIN_EFFICIENCY ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
num_events_label( this, ResId( vID_STEXT_MAX_NUM_EVENTS_LABEL ) ),
num_mols_label( this, ResId( vID_STEXT_NUM_MOLS_LABEL ) ),
record_interval_label( this, ResId( vID_STEXT_RECORD_INTERVAL_LABEL ) ),
random_num_seed_label( this, ResId( vID_STEXT_RANDOM_NUM_SEED_LABEL ) ),
min_efficiency_label( this, ResId( vID_STEXT_SELECTION_FREQ_LABEL ) ),
elapsed_time_limit_label( this, ResId( vID_STEXT_MAX_SIMULATION_TIME_LABEL ) ),
aFixedText8( this, ResId( 8 ) ),
cycle_count_label( this, ResId( vID_STEXT_CYCLE_LENGTH_LABEL ) ),
et_limits_units_text( this, ResId( vID_STEXT_ET_LIMITS_UNITS ) ),
aFixedText12( this, ResId( 12 ) ),
aFixedText13( this, ResId( 13 ) ),
aGroupBox1( this, ResId( 1 ) ),
aGroupBox2( this, ResId( 2 ) ),
aGroupBox3( this, ResId( 3 ) ),
p_parent( pParent),
data_altered( FALSE )
{
     FreeResource();
     Initialize();

     
     num_mols_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     record_interval_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     random_num_seed_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     num_events_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     elapsed_time_limit_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     cycle_count_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  
     min_efficiency_entryfield.ChangeModifyHdl( LINK( this, SimulationOptionDlg, TextModifiedHandler ) );  

     enable_equil_detect_rb.ChangeClickHdl( LINK( this, SimulationOptionDlg, RadioBtnHandler ) ); 
     disable_equil_detect_rb.ChangeClickHdl( LINK( this, SimulationOptionDlg, RadioBtnHandler ) );  

     close_btn.ChangeClickHdl( LINK( this, SimulationOptionDlg, CloseBtnHandler ) );  

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}






// ----------------------------------------------------------------------------
//                  EditSimulationOptions()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

SIMOPT_RC EditSimulationOptions( SchemeFrameWindow* pParent )
{
     SimulationOptionDlg* pdlg = new SimulationOptionDlg( pParent );

     if ( pdlg )
     {
           return (SIMOPT_RC) pdlg->Execute();
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return SO_RC_FAIL;
     }
}



