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
//  MLTCPLOT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This header file defines the class MultiCompartmentPlotDlg
//
//  CREATED : 8.08.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------





#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "mltcplot.hxx"
#include "mainapp.hxx"
#include "comptobj.hxx"
#include "fileutil.hxx"
#include "xypltfrm.hxx"
#include "wfpltfrm.hxx"
#include "cmpstate.hxx"
#include "results.hxx"
#include "plotattr.hxx"

#endif


#include <limits.h>

#include "mltcplot.hrc"
#include "strings.hrc"
#include "help.hhc"




typedef unsigned long PLOT_TYPE;


const PLOT_TYPE NO_PLOTS       = 0x0000,
                CONC_VS_TIME   = 0x0001 ,
                TEMP_VS_TIME   = 0x0002,
                VOLUME_VS_TIME = 0x0004,
                PRESS_VS_TIME  = 0x0008,
                CONC_VS_TEMP   = 0x0010,
                VOLUME_VS_TEMP = 0x0020,
                PRESS_VS_TEMP  = 0x0040,
                AMOUNT_VS_TIME = 0x0080,
                AMOUNT_VS_TEMP = 0x0100;






// ----------------------------------------------------------------------------
//                         class OneCompartmentPlotDlg
// ............................................................................
//
//   class declaration
//
// ----------------------------------------------------------------------------

class MultiCompartmentPlotDlg : public ModalDialog
{
protected:
     MultiListBox compartment_listbox;
     PushButton   select_all_compts_btn;
     PushButton   deselect_all_compts_btn;
     PushButton   select_prev_compts_btn;
     GroupBox     group_box_1;

     RadioButton  conc_v_time_rb;
     RadioButton  amount_v_time_rb;
     RadioButton  temp_v_time_rb;
     RadioButton  volume_v_time_rb;
     RadioButton  press_v_time_rb;
     RadioButton  conc_v_temp_rb;
     RadioButton  amount_v_temp_rb;
     RadioButton  volume_v_temp_rb;
     RadioButton  press_v_temp_rb;

     GroupBox     group_box_2;

     CheckBox     plot_extern_data_chkbox;
     PushButton   select_extern_file_btn;
     GroupBox     group_box_3;

     MultiListBox species_listbox;
     PushButton   select_all_species_btn;
     PushButton   deselect_all_species_btn;
     PushButton   select_prev_species_btn;
     GroupBox     species_list_group_box;

     FixedText    fixed_text_4;
     FixedText    fixed_text_7;
     FixedText    num_pts_avail_text;
     Edit         max_pts_to_plot_ef;
     GroupBox     group_box_5;

     FixedText    num_selected_species_label;
     FixedText    fixed_text_10;
     FixedText    num_species_selected_text;
     FixedText    num_compts_selected_text;
     GroupBox     group_box_6;

     CancelButton cancel_btn;
     OKButton     plot_btn;
     HelpButton   help_btn;

     SchemeWindow*       p_scheme_window;
     BOOL8               show_species;
     PLOT_TYPE           current_plot_type;
     UINT32              num_pts_to_plot;

     void                InitializeDialog( );
     void                InitializeSpeciesSelection( BOOL8 State );
     void                EnableExternalDataFileControls();
     PLOT_TYPE           GetPlotType( const RadioButton*  pRadioBtn ) const;
     PLOT_TYPE           GetPlotType( const String& rTypeStr ) const;
     void                SaveSelectionsForRecall();
     void                ChangePlotButtonActivation();
     virtual void        CreatePlotWindow( );
     void                InitializePlotLabels( XyPlot* pPlot, PLOT_TYPE PlotType, const String& rCompartmentName );

     void                SetPlotTypeFlag( PLOT_TYPE Type ) 
                         { current_plot_type = Type; }

     void                ClearPlotTypeFlag( PLOT_TYPE Type )
                         { current_plot_type = NO_PLOTS; }


public:
                   MultiCompartmentPlotDlg( SchemeFrameWindow* pSchemeFrameWin );

     void          SpeciesListboxSelectHandler( MultiListBox* );
     void          CompartmentListboxHandler( MultiListBox* );
     void          ConcBtnHandler( RadioButton *pRadioBtn );
     void          RadioBtnHandler( RadioButton* pRadioBtn );
     void          DeselectAllSpeciesHandler( PushButton* );
     void          SelectAllSpeciesHandler( PushButton* );
     void          PreviousSpeciesHandler( PushButton* );
     void          PlotButtonHandler( OKButton* );
     void          SelectExtFileHandler( PushButton* );
     void          DeselectAllComptsHandler( PushButton* );
     void          SelectAllComptsHandler( PushButton* );
     void          PreviousComptsHandler( PushButton* );

};






// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::MultiCompartmentPlotDlg()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

MultiCompartmentPlotDlg::MultiCompartmentPlotDlg( SchemeFrameWindow* pSchemeFrameWin ) :
ModalDialog( pSchemeFrameWin, ResId( vID_DLG_MULT_COMPT_PLOT_SETUP ) ),
compartment_listbox( this, ResId( vID_LB_COMPT_LIST ) ),
select_all_compts_btn( this, ResId( vID_BTN_SELECT_ALL_COMPTS ) ), 
deselect_all_compts_btn( this, ResId( vID_BTN_DESELECT_ALL_COMPTS ) ), 
select_prev_compts_btn( this, ResId( vID_BTN_PREV_COMPTS_SELECT ) ), 
group_box_1( this, ResId( 1 ) ),
conc_v_time_rb( this, ResId( vID_RB_CONC_V_TIME ) ),
amount_v_time_rb( this, ResId( vID_RB_AMOUNT_V_TIME ) ), 
temp_v_time_rb( this, ResId( vID_RB_TEMP_V_TIME ) ),
volume_v_time_rb( this, ResId( vID_RB_VOLUME_V_TIME ) ),
press_v_time_rb( this, ResId( vID_RB_PRESS_V_TIME ) ),
conc_v_temp_rb( this, ResId( vID_RB_CONC_V_TEMP ) ),
amount_v_temp_rb( this, ResId( vID_RB_AMOUNT_V_TEMP ) ), 
volume_v_temp_rb( this, ResId( vID_RB_VOLUME_V_TEMP ) ),
press_v_temp_rb( this, ResId( vID_RB_PRESS_V_TEMP ) ),
group_box_2( this, ResId( 2 ) ),
plot_extern_data_chkbox( this, ResId( vID_CB_PLOT_EXTRNDATA ) ),
select_extern_file_btn( this, ResId( vID_BTN_SELECT_EXTERN_FILE ) ),
group_box_3( this, ResId( 3 ) ),
species_listbox( this, ResId( vID_LB_SPECIES_LIST ) ),
select_all_species_btn( this, ResId( vID_BTN_SELECT_ALL_SPECIES ) ),
deselect_all_species_btn( this, ResId( vID_BTN_DESELECT_ALL_SPECIES ) ),
select_prev_species_btn( this, ResId( vID_BTN_PREV_SPECIES_SELECT ) ),
species_list_group_box( this, ResId( vID_GB_SPECIES_LABEL ) ),
fixed_text_4( this, ResId( 4 ) ),
fixed_text_7( this, ResId( 7 ) ),
num_pts_avail_text( this, ResId( vID_STEXT_NUM_POINTS_AVAIL ) ),
max_pts_to_plot_ef( this, ResId( vID_EF_MAX_POINTS_TO_PLOT ) ),
group_box_5( this, ResId( 5 ) ),
num_selected_species_label( this, ResId( vID_STEXT_NUM_SPECIES_SELECTED_LABEL ) ),
fixed_text_10( this, ResId( 10 ) ),
num_species_selected_text( this, ResId( vID_STEXT_NUM_SPECIES_SELECTED ) ),
num_compts_selected_text( this, ResId( vID_STEXT_NUM_COMPTS_SELECTED ) ),
group_box_6( this, ResId( 6 ) ),
cancel_btn( this, ResId( vID_BTN_CANCEL ) ),
plot_btn( this, ResId( vID_BTN_PLOT ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
p_scheme_window( pSchemeFrameWin->GetpSchemeWindow() ),
show_species( FALSE ),
current_plot_type( NO_PLOTS ),
num_pts_to_plot( 0 )
{
     FreeResource();

     InitializeDialog( );

     compartment_listbox.ChangeSelectHdl( LINK( this, MultiCompartmentPlotDlg, CompartmentListboxHandler ) );
     species_listbox.ChangeSelectHdl( LINK( this, MultiCompartmentPlotDlg, SpeciesListboxSelectHandler ) );

     conc_v_time_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, ConcBtnHandler ) );
     conc_v_temp_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, ConcBtnHandler ) );
     amount_v_time_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, ConcBtnHandler ) );
     amount_v_temp_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, ConcBtnHandler ) );

     temp_v_time_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, RadioBtnHandler ) );
     volume_v_time_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, RadioBtnHandler ) );
     press_v_time_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, RadioBtnHandler ) );
     volume_v_temp_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, RadioBtnHandler ) );
     press_v_temp_rb.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, RadioBtnHandler ) );

     select_all_species_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, SelectAllSpeciesHandler ) );
     deselect_all_species_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, DeselectAllSpeciesHandler ) );
     select_prev_species_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, PreviousSpeciesHandler ) );

     select_all_compts_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, SelectAllComptsHandler ) );
     deselect_all_compts_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, DeselectAllComptsHandler ) );
     select_prev_compts_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, PreviousComptsHandler ) );

     plot_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, PlotButtonHandler ) );
     select_extern_file_btn.ChangeClickHdl( LINK( this, MultiCompartmentPlotDlg, SelectExtFileHandler ) );

     GetpMainApp()->CascadeWindowOnParent( this, GetParent() );
}



// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::PreviousComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::PreviousComptsHandler( PushButton* )
{
     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     UINT16 num_saved_compt_names = 0;

     num_saved_compt_names = config.ReadKey( String( ResId( vID_STR_NUM_PREV_COMPT_NAMES )));


     if ( num_saved_compt_names )
     {
          compartment_listbox.SetNoSelection();

          String name;

          for ( USHORT i = 1; i <= num_saved_compt_names; i++ )
          {
               name = config.ReadKey( String( ResId( vID_STR_PREV_COMPT_NAME_STUB )) +
                               String( i ) );

                if ( name.Len() )
                    compartment_listbox.SelectEntry( name );
          }
     }

     CompartmentListboxHandler( &compartment_listbox );
}



// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::SelectAllComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::SelectAllComptsHandler( PushButton* )
{
     UINT16 num_entries = compartment_listbox.GetEntryCount();

     while (num_entries--)
          compartment_listbox.SelectEntryPos( num_entries );

     CompartmentListboxHandler( &compartment_listbox );
}




// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::DeselectAllComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::DeselectAllComptsHandler( PushButton* )
{
     compartment_listbox.SetNoSelection( );
     CompartmentListboxHandler( &compartment_listbox );
}








// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::SetLabels( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::InitializePlotLabels( XyPlot* pPlot, PLOT_TYPE PlotType,
                                                    const String& rCompartmentName )
{
     ReactionScheme& rscheme = p_scheme_window->GetScheme();

     // start with the title above the plot 

     String title = p_scheme_window->GetScheme().GetNickName();  // the file name 

     title += String( ResId( vID_STR_TITLE_SEPARATOR ) );

//     title += compartment_listbox.GetSelectEntry();               // the compartment name
     title += rCompartmentName;

     title += String( ResId( vID_STR_TITLE_SEPARATOR ) );

     // the Y axis labels 

     switch ( PlotType )
     {
     case  CONC_VS_TIME :
     case  CONC_VS_TEMP :

          pPlot->SetYAxisTitle( String( ResId( vID_STR_CONCENTRATION ) ) );
          pPlot->SetYAxisUnits( rscheme.GetConcentrationUnits() );
          title += String( ResId( vID_STR_CONCENTRATION ) ); 

          break;

     case PRESS_VS_TIME:
     case PRESS_VS_TEMP:

          pPlot->SetYAxisTitle( String( ResId( vID_STR_PRESSURE ) ) );
          pPlot->SetYAxisUnits( rscheme.GetPressureUnits() );
          title += String( ResId( vID_STR_PRESSURE ) ); 

          break;

     case VOLUME_VS_TIME :
     case VOLUME_VS_TEMP :

          pPlot->SetYAxisTitle( String( ResId( vID_STR_VOLUME ) ) );
          pPlot->SetYAxisUnits( rscheme.GetVolumeUnits() );
          title += String( ResId( vID_STR_VOLUME ) ); 

          break;

     case TEMP_VS_TIME :

          pPlot->SetYAxisTitle( String( ResId( vID_STR_TEMPERATURE ) ) );
          pPlot->SetYAxisUnits( rscheme.GetTemperatureUnits() );
          title += String( ResId( vID_STR_TEMPERATURE ) ); 

          break;

     case AMOUNT_VS_TIME :
     case AMOUNT_VS_TEMP :

          pPlot->SetYAxisTitle( String( ResId( vID_STR_AMOUNT ) ) );
          pPlot->SetYAxisUnits( rscheme.GetAmountUnits() );
          title += String( ResId( vID_STR_AMOUNT ) ); 

          break;
     }


    title += String( ResId( vID_STR_VERSUS ) ); 

     // the X labels

     switch ( PlotType )
     {
     case CONC_VS_TIME :
     case PRESS_VS_TIME:
     case VOLUME_VS_TIME :
     case TEMP_VS_TIME :
     case AMOUNT_VS_TIME :

          pPlot->SetXAxisTitle( String( ResId( vID_STR_TIME ) ) );
          pPlot->SetXAxisUnits( rscheme.GetTimeUnits() );
          title += String( ResId( vID_STR_TIME ) ); 

          break;

     case VOLUME_VS_TEMP :
     case CONC_VS_TEMP :
     case PRESS_VS_TEMP:
     case AMOUNT_VS_TEMP :

          pPlot->SetXAxisTitle( String( ResId( vID_STR_TEMPERATURE ) ) );
          pPlot->SetXAxisUnits( rscheme.GetTemperatureUnits() );
          title += String( ResId( vID_STR_TEMPERATURE ) ); 

          break;
     }


     pPlot->SetPlotTitle( title );
}










// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::CreatePlotWindow( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::CreatePlotWindow( )
{
     XyPlotFrameWindow* p_frame_win = new XyPlotFrameWindow( *p_scheme_window );

     if ( NULL == p_frame_win )
     {
          GetpMainApp()->OutOfMemory();
          return;
     }

     XyPlotWindow* p_win = p_frame_win->GetpPlotWindow();


     // made the new window OK - now get the simulation results for the
     // compartments we are interested in 

     SimulationResult* p_results = p_scheme_window->GetScheme().GetpResults();


     UINT16 num_plots = compartment_listbox.GetSelectEntryCount();

     for (UINT16 i=0; i < num_plots; i++ )
     {
          // get the results for the current compartment 

          CompartmentState*   p_cmp_state = p_results->GetpComptState( 
           compartment_listbox.GetSelectEntry( i ) );

          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, current_plot_type, compartment_listbox.GetSelectEntry( i ) );

          switch ( current_plot_type )
          {

               case CONC_VS_TIME :
               case CONC_VS_TEMP :
               {
                    // get access to species database - we will need this to get SPECIES_ID value
          
                    SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();
     
                    const FloatArray& rX =  ( CONC_VS_TIME == current_plot_type ) ? 
                     p_results->GetElapsedTimes() : p_cmp_state->GetTemperatures();
          
                    // now add each requested species concentration one by one
                    // until all sselected species are dealt with
          
                    UINT16 num_species = species_listbox.GetSelectEntryCount();
          
                    for ( UINT16 i = 0; i < num_species; i++ )
                    {
                         UINT16 position = species_listbox.GetSelectEntryPos( i );
          
                         SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
          
                         p_plot->AddDataSet( rX, p_cmp_state->GetConcentrations( id ),
                          species_listbox.GetSelectEntry(i) );

                    }
          
                    break;
     
               }

               case AMOUNT_VS_TIME :
               case AMOUNT_VS_TEMP :
               {
                    // get access to species database - we will need this to get SPECIES_ID value
          
                    SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();
     
                    const FloatArray& rX =  ( AMOUNT_VS_TIME == current_plot_type ) ? 
                     p_results->GetElapsedTimes() : p_cmp_state->GetTemperatures();
          
                    // now add each requested species concentration one by one
                    // until all sselected species are dealt with
          
                    UINT16 num_species = species_listbox.GetSelectEntryCount();
          
                    for ( UINT16 i = 0; i < num_species; i++ )
                    {
                         UINT16 position = species_listbox.GetSelectEntryPos( i );
          
                         SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
          
                         p_plot->AddDataSet( rX, p_cmp_state->GetAmounts( id ),
                          species_listbox.GetSelectEntry(i) );

                    }
          
                    break;
     
               }
     
               case TEMP_VS_TIME :

                    p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetTemperatures(),
                     String( ResId( vID_STR_TEMPERATURE ) ) );
                    break;

               case VOLUME_VS_TIME :

                    p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetVolumes(), 
                     String( ResId( vID_STR_VOLUME ) ) );
                    break;

               case PRESS_VS_TIME : 

                    p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetPressures(), 
                     String( ResId( vID_STR_PRESSURE ) ) );
                    break;

               case VOLUME_VS_TEMP :

                    p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetVolumes() ,
                     String( ResId( vID_STR_VOLUME ) ) );
                    break;

               case PRESS_VS_TEMP :

                    p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetPressures(),
                     String( ResId( vID_STR_PRESSURE ) ) );
                    break;
          }
     }

     p_win->SetMaxNumPlotPts( num_pts_to_plot );

     GetpMainApp()->CascadeWindowOnParent( p_frame_win, this );
     p_frame_win->Show();
}





// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::SelectExtFileHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::SelectExtFileHandler( PushButton* )
{
     String filename;

     if ( OpenFileDialog( filename, TEXT_FILETYPE) )
     {
          plot_extern_data_chkbox.SetText( filename );
          EnableExternalDataFileControls();
     }
}





// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::ChangePlotButtonActivation( )
// ............................................................................
//
//  Only activates 'Create Plot' buttons when appropariate selections have been
//  made by the user 
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::ChangePlotButtonActivation()
{
     // are any compartments selected 

     if ( compartment_listbox.GetSelectEntryCount() )
     {
          // if so, are any requesting plotting of concentrations ?

          if ( current_plot_type & ( CONC_VS_TIME | CONC_VS_TEMP | AMOUNT_VS_TIME | AMOUNT_VS_TEMP ) )
          {
               // if so, are any species selected 

               if ( species_listbox.GetSelectEntryCount() )
               {
                      plot_btn.Enable();
               }
               else
               {
                      plot_btn.Disable();
               }

          }
          else
               plot_btn.Enable();
     }
     else
          plot_btn.Disable();
}










// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::SaveSelectionsForRecall()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::SaveSelectionsForRecall()
{
     // write stuff to config file to save for later recall

     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     // save the names of selected compartments for later recall 

     UINT16 num_saved_compt_names = 0;

     num_saved_compt_names = config.ReadKey( String( ResId( vID_STR_NUM_PREV_COMPT_NAMES )));

     for ( USHORT i = 1; i <= num_saved_compt_names; i++ )
          config.DeleteKey( String( ResId( vID_STR_PREV_COMPT_NAME_STUB )) +
                          String( i ) );

     UINT16 num_compts_to_save = compartment_listbox.GetSelectEntryCount();

     if ( num_compts_to_save )
     {
          config.WriteKey( String( ResId( vID_STR_NUM_PREV_COMPT_NAMES )),
           num_compts_to_save );

          for ( i = 0; i < num_compts_to_save; i++ )
               config.WriteKey(
                String( ResId( vID_STR_PREV_COMPT_NAME_STUB )) + String( i+1 ),
                compartment_listbox.GetSelectEntry( i )
                );
     }


     // save the names of the selected species for later recall

     if ( species_listbox.IsEnabled() )
     {
          // only save info if currently in use

          UINT16 num_saved_species_names = 0;

          num_saved_species_names = config.ReadKey( String( ResId( vID_STR_NUM_PREV_SPECIES_NAMES )));

          for ( USHORT i = 1; i <= num_saved_species_names; i++ )
               config.DeleteKey( String( ResId( vID_STR_PREV_SPECIES_NAME_STUB )) +
                               String( i ) );

          UINT16 num_species_to_save = species_listbox.GetSelectEntryCount();

          if ( num_species_to_save )
          {
               config.WriteKey( String( ResId( vID_STR_NUM_PREV_SPECIES_NAMES )),
                num_species_to_save );

               for ( i = 0; i < num_species_to_save; i++ )
                    config.WriteKey(
                     String( ResId( vID_STR_PREV_SPECIES_NAME_STUB )) + String( i+1 ),
                     species_listbox.GetSelectEntry( i )
                     );
          }

     }

     // now write out the external data file name and type if in use

     if ( plot_extern_data_chkbox.IsChecked() )
     {
          config.WriteKey( String( ResId( vID_STR_EXT_DATA_FILE ) ), plot_extern_data_chkbox.GetText() );
     }
}







// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::PlotButtonHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::PlotButtonHandler( OKButton* )
{
     // first check user input in specifying number of points to plot 

     NumericString numstr( max_pts_to_plot_ef.GetText() );
     UINT32        value;

     if ( numstr.IsValidUINT32( &value ) && 
          ( value >= MIN_PTS ) &&
          ( value <= MAX_PTS )
        )
     {
          num_pts_to_plot = value;

          SaveSelectionsForRecall();

          CreatePlotWindow( );

          EndDialog();
     }
     else
     {
          String message = String( ResId( vID_STR_MAX_PLOT_PTS_INPUT_ERR1 ) ) + 
           numstr + 
           String( ResId( vID_STR_MAX_PLOT_PTS_INPUT_ERR2 ) );

          WarningBox( GetParent(), WB_OK | WB_DEF_OK, message ).Execute( );

          return;
     }

}








// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::PreviousSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::PreviousSpeciesHandler( PushButton* )
{
     // read in previous file names if any
     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     UINT16 num_saved_species_names = 0;

     num_saved_species_names = config.ReadKey( String( ResId( vID_STR_NUM_PREV_SPECIES_NAMES )));


     if ( num_saved_species_names )
     {
          species_listbox.SetNoSelection();

          String name;

          for ( USHORT i = 1; i <= num_saved_species_names; i++ )
          {
               name = config.ReadKey( String( ResId( vID_STR_PREV_SPECIES_NAME_STUB )) +
                               String( i ) );

                if ( name.Len() )
                    species_listbox.SelectEntry( name );
          }
     }

     SpeciesListboxSelectHandler( &species_listbox );
}





// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::SelectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::SelectAllSpeciesHandler( PushButton* )
{
     UINT16 num_entries = species_listbox.GetEntryCount();

     while (num_entries--)
          species_listbox.SelectEntryPos( num_entries );

     SpeciesListboxSelectHandler( &species_listbox );
}



// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::DeselectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::DeselectAllSpeciesHandler( PushButton* )
{
     species_listbox.SetNoSelection( );
     SpeciesListboxSelectHandler( &species_listbox );
}





// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::SpeciesListboxSelectHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::SpeciesListboxSelectHandler( MultiListBox* )
{
     UINT16 num_species_selected = species_listbox.GetSelectEntryCount( );

     num_species_selected_text.SetText( num_species_selected );

     // enable/disable pushbuttons as required

     if ( num_species_selected )
         deselect_all_species_btn.Enable();
     else
         deselect_all_species_btn.Disable();

     if ( num_species_selected < p_scheme_window->GetScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() &&
          show_species )
         select_all_species_btn.Enable();
     else
         select_all_species_btn.Disable();

     ChangePlotButtonActivation( );
}






// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::ConcBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::ConcBtnHandler( RadioButton *pRadioBtn )
{
     BOOL8 test = ( conc_v_temp_rb.IsChecked( ) ||
                    conc_v_time_rb.IsChecked( ) || 
                    amount_v_temp_rb.IsChecked( ) ||
                    amount_v_time_rb.IsChecked( ) 
                  );

     if ( test != show_species )       /* if state has changed                */
     {
          InitializeSpeciesSelection( test );
          show_species = !show_species;
          SpeciesListboxSelectHandler( &species_listbox );
     }

     RadioBtnHandler( pRadioBtn );
}








// ----------------------------------------------------------------------------
//           MultiCompartmentPlotDlg::GetPlotType( const CheckBox* pCheckBox )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

PLOT_TYPE MultiCompartmentPlotDlg::GetPlotType( const RadioButton* pRadioBtn ) const
{
     if ( pRadioBtn  == &conc_v_time_rb )
          return CONC_VS_TIME;

     if ( pRadioBtn  == &amount_v_time_rb )
          return AMOUNT_VS_TIME;

     if ( pRadioBtn  == &temp_v_time_rb )
          return TEMP_VS_TIME;

     if ( pRadioBtn  == &volume_v_time_rb )
          return VOLUME_VS_TIME;

     if ( pRadioBtn  == &press_v_time_rb )
          return PRESS_VS_TIME;

     if ( pRadioBtn  == &conc_v_temp_rb )
          return CONC_VS_TEMP;

     if ( pRadioBtn  == &amount_v_temp_rb )
          return AMOUNT_VS_TEMP;

     if ( pRadioBtn  == &volume_v_temp_rb )
          return VOLUME_VS_TEMP;

     if ( pRadioBtn  == &press_v_temp_rb )
          return PRESS_VS_TEMP;

     // abort if we get here 

     ASSERT( 0 );
}









// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::CheckBoxHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::RadioBtnHandler( RadioButton * pRadioBtn )
{
     // figure out plot type and set flag in current_plot_type

     SetPlotTypeFlag( GetPlotType( pRadioBtn ) );

     ChangePlotButtonActivation( );
}






// ----------------------------------------------------------------------------
//              MultiCompartmentPlotDlg::InitializeSpeciesSelection()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::InitializeSpeciesSelection( BOOL8 State )
{

     // fill the species selection list box if selected

     if ( State )
     {
          species_list_group_box.Enable( );
          select_all_species_btn.Enable( );
          select_prev_species_btn.Enable();
          num_selected_species_label.Enable();
          num_species_selected_text.Enable();

          deselect_all_species_btn.Disable();

          species_listbox.Enable( );

          // turn off update to prevent flicker

          species_listbox.ChangeUpdateMode( FALSE );

          species_listbox.Clear( );

          // fill the listbox with species names

          SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

          UINT16 num_species = p_spec_db->GetNumberOfSpecies();

          for ( UINT16 i = 0 ; i< num_species; i++ )
          {
               species_listbox.InsertEntry(
                p_spec_db->GetSpeciesDataByPosition( i ).GetName()
                );
          }

          species_listbox.SetNoSelection( );  // de-select any
          species_listbox.ChangeUpdateMode( TRUE );
          species_listbox.Update( );
     }
     else
     {

          /* clear and disable stuff it it is not available                   */

          species_listbox.Clear( );
          species_listbox.Disable( );
          num_selected_species_label.Disable();
          num_species_selected_text.Disable();

          species_list_group_box.Disable( );
          select_all_species_btn.Disable( );
          select_prev_species_btn.Disable();
          deselect_all_species_btn.Disable();

     }
}






// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::CompartmentListboxHandler( )
// ............................................................................
//
//  called whenever a new Compartment is selected in compartment listbox
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::CompartmentListboxHandler( MultiListBox* )
{
     UINT16 count = compartment_listbox.GetSelectEntryCount();

     if( count )
         deselect_all_compts_btn.Enable();
     else
         deselect_all_compts_btn.Disable();

     if ( count < compartment_listbox.GetEntryCount() )
         select_all_compts_btn.Enable();
     else
         select_all_compts_btn.Disable();

     num_compts_selected_text.SetText( count );

     ChangePlotButtonActivation();
}









// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::InitializeDialog( )
{
     // initialize the "select compartment" listbox

     UINT16 num_compts  = p_scheme_window->GetScheme().GetNumCompartments( );


     for ( UINT16 i = 0; i < num_compts; i++ )
          compartment_listbox.InsertEntry(
           p_scheme_window->GetScheme().GetCompartmentObjectPtrFromIndex( i )->GetName()
          );

     // initialize radiobutton selection

     conc_v_time_rb.Check( TRUE );
     ConcBtnHandler( &conc_v_time_rb );


     // if we have saved a non-null ext data file name, then use it to initialize
     // the external data file chekcbox text

     Config config;

     // deal with setting in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     String tempstr = config.ReadKey( String( ResId( vID_STR_EXT_DATA_FILE ) ) );
         String teststr = String( ResId( vID_STR_NO_EXT_DATA_FILE ) ) ;

     if ( tempstr.Len() && tempstr != teststr )
     {
          plot_extern_data_chkbox.SetText( tempstr );
          EnableExternalDataFileControls();
     }
     else
          plot_extern_data_chkbox.SetText( String( ResId( vID_STR_NO_EXT_DATA_FILE ) ) );

     // set some text

      num_pts_avail_text.SetText( String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );

     // set to minimum of available points in simulation of MAX_PTS_DEFAULT

      max_pts_to_plot_ef.SetText(
       String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() > MAX_PTS_DEFAULT ?
       MAX_PTS_DEFAULT : p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );
}






// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::EnableExternalDataFileControls()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MultiCompartmentPlotDlg::EnableExternalDataFileControls()
{
     plot_extern_data_chkbox.Enable( );
}




// ----------------------------------------------------------------------------
//                         class WaterfallPlotDialog
// ............................................................................
//
//   class declaration
//
// ----------------------------------------------------------------------------

class WaterfallPlotDlg: public MultiCompartmentPlotDlg
{
protected: 

     virtual void        CreatePlotWindow( );
     void                InitializePlotLabels( StackedXyPlot& rPlot, PLOT_TYPE PlotType );

public:
                   WaterfallPlotDlg( SchemeFrameWindow* pSchemeFrameWin );
};






// ----------------------------------------------------------------------------
//                    WaterfallPlotDlg::WaterfallPlotDlg()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

WaterfallPlotDlg::WaterfallPlotDlg( SchemeFrameWindow* pSchemeFrameWin ) :
MultiCompartmentPlotDlg( pSchemeFrameWin )
{
     SetText( String( ResId( vID_STR_WATERFALL_DLG_TITLE ) ));
     ChangeHelpId( HLP_WF_PLOT );
}







// ----------------------------------------------------------------------------
//                MultiCompartmentPlotDlg::SetLabels( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WaterfallPlotDlg::InitializePlotLabels( StackedXyPlot& rPlot, PLOT_TYPE PlotType )
{
     ReactionScheme& rscheme = p_scheme_window->GetScheme();

     // start with the title above the plot 

     String title = rscheme.GetNickName();  // the file name 

     title += String( ResId( vID_STR_TITLE_SEPARATOR ) );

     // the Y axis labels 

     switch ( PlotType )
     {
     case  CONC_VS_TIME :
     case  CONC_VS_TEMP :

          rPlot.SetYAxisTitle( String( ResId( vID_STR_CONCENTRATION ) ) );
          rPlot.SetYAxisUnits( rscheme.GetConcentrationUnits() );
          title += String( ResId( vID_STR_CONCENTRATION ) ); 

          break;

     case PRESS_VS_TIME:
     case PRESS_VS_TEMP:

          rPlot.SetYAxisTitle( String( ResId( vID_STR_PRESSURE ) ) );
          rPlot.SetYAxisUnits( rscheme.GetPressureUnits() );
          title += String( ResId( vID_STR_PRESSURE ) ); 

          break;

     case VOLUME_VS_TIME :
     case VOLUME_VS_TEMP :

          rPlot.SetYAxisTitle( String( ResId( vID_STR_VOLUME ) ) );
          rPlot.SetYAxisUnits( rscheme.GetVolumeUnits() );
          title += String( ResId( vID_STR_VOLUME ) ); 

          break;

     case TEMP_VS_TIME :

          rPlot.SetYAxisTitle( String( ResId( vID_STR_TEMPERATURE ) ) );
          rPlot.SetYAxisUnits( rscheme.GetTemperatureUnits() );
          title += String( ResId( vID_STR_TEMPERATURE ) ); 

          break;

     case AMOUNT_VS_TIME :
     case AMOUNT_VS_TEMP :

          rPlot.SetYAxisTitle( String( ResId( vID_STR_AMOUNT ) ) );
          rPlot.SetYAxisUnits( rscheme.GetAmountUnits() );
          title += String( ResId( vID_STR_AMOUNT ) ); 

          break;
     }


    title += String( ResId( vID_STR_VERSUS ) ); 

     // the X labels

     switch ( PlotType )
     {
     case CONC_VS_TIME :
     case PRESS_VS_TIME:
     case VOLUME_VS_TIME :
     case TEMP_VS_TIME :
     case AMOUNT_VS_TIME :

          rPlot.SetXAxisTitle( String( ResId( vID_STR_TIME ) ) );
          rPlot.SetXAxisUnits( rscheme.GetTimeUnits() );
          title += String( ResId( vID_STR_TIME ) ); 

          break;

     case VOLUME_VS_TEMP :
     case CONC_VS_TEMP :
     case PRESS_VS_TEMP:
     case AMOUNT_VS_TEMP :

          rPlot.SetXAxisTitle( String( ResId( vID_STR_TEMPERATURE ) ) );
          rPlot.SetXAxisUnits( rscheme.GetTemperatureUnits() );
          title += String( ResId( vID_STR_TEMPERATURE ) ); 

          break;
     }


     rPlot.SetPlotTitle( title );
}











// ----------------------------------------------------------------------------
//                WaterfallPlotDlg::CreatePlotWindow( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WaterfallPlotDlg::CreatePlotWindow( )
{
     WaterfallPlotFrameWindow* p_frame_win = new WaterfallPlotFrameWindow( *p_scheme_window );

     if ( NULL == p_frame_win )
     {
          GetpMainApp()->OutOfMemory();
          return;
     }

     WaterfallPlotWindow* p_win = p_frame_win->GetpPlotWindow();

     StackedXyPlot&  r_plot = p_win->GetPlot();


     // made the new window OK - now get the simulation results for the
     // compartments we are interested in 

     SimulationResult* p_results = p_scheme_window->GetScheme().GetpResults();


     UINT16 num_data_sets = compartment_listbox.GetSelectEntryCount();


     InitializePlotLabels( r_plot, current_plot_type );

     for (UINT16 i=0; i < num_data_sets; i++ )
     {
          // get the results for the current compartment 

          CompartmentState*   p_cmp_state = p_results->GetpComptState( 
           compartment_listbox.GetSelectEntry( i ) );

          String compt_name = compartment_listbox.GetSelectEntry( i );


          switch ( current_plot_type )
          {

               case CONC_VS_TIME :
               case CONC_VS_TEMP :
               {
                    // get access to species database - we will need this to get SPECIES_ID value
          
                    SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();
     
                    const FloatArray& rX =  ( CONC_VS_TIME == current_plot_type ) ? 
                     p_results->GetElapsedTimes() : p_cmp_state->GetTemperatures();
          
                    // now add each requested species concentration one by one
                    // until all sselected species are dealt with
          
                    UINT16 num_species = species_listbox.GetSelectEntryCount();
          
                    for ( UINT16 i = 0; i < num_species; i++ )
                    {
                         UINT16 position = species_listbox.GetSelectEntryPos( i );
          
                         SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
          
                         r_plot.AddDataSet( rX, p_cmp_state->GetConcentrations( id ), 
                          compt_name + 
                          String( ResId( vID_STR_TITLE_SEPARATOR ) ) +
                          species_listbox.GetSelectEntry(i) );

                    }
          
                    break;
     
               }

               case AMOUNT_VS_TIME :
               case AMOUNT_VS_TEMP :
               {
                    // get access to species database - we will need this to get SPECIES_ID value
          
                    SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();
     
                    const FloatArray& rX =  ( AMOUNT_VS_TIME == current_plot_type ) ? 
                     p_results->GetElapsedTimes() : p_cmp_state->GetTemperatures();
          
                    // now add each requested species concentration one by one
                    // until all sselected species are dealt with
          
                    UINT16 num_species = species_listbox.GetSelectEntryCount();
          
                    for ( UINT16 i = 0; i < num_species; i++ )
                    {
                         UINT16 position = species_listbox.GetSelectEntryPos( i );
          
                         SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
          
                         r_plot.AddDataSet( rX, p_cmp_state->GetAmounts( id ),
                          compt_name + 
                          String( ResId( vID_STR_TITLE_SEPARATOR ) ) +
                          species_listbox.GetSelectEntry(i) );

                    }
          
                    break;
     
               }
     
               case TEMP_VS_TIME :

                    r_plot.AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetTemperatures(),
                      compt_name );
                    break;

               case VOLUME_VS_TIME :

                    r_plot.AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetVolumes(), 
                     compt_name );
                    break;

               case PRESS_VS_TIME : 

                    r_plot.AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetPressures(), 
                     compt_name );
                    break;

               case VOLUME_VS_TEMP :

                    r_plot.AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetVolumes() ,
                     compt_name );
                    break;

               case PRESS_VS_TEMP :

                    r_plot.AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetPressures(),
                     compt_name );
                    break;
          }
     }

     p_win->SetMaxNumPlotPts( num_pts_to_plot );

     GetpMainApp()->CascadeWindowOnParent( p_frame_win, this );
     p_frame_win->Show();
}









// ----------------------------------------------------------------------------
//                ShowMultComptPlotDialog( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowMultComptPlotDialog( SchemeFrameWindow* pSchemeFrameWin )
{
     MultiCompartmentPlotDlg* pdlg = new MultiCompartmentPlotDlg( pSchemeFrameWin );

     if ( pdlg )
     {
          pdlg->Execute();
          delete pdlg;
     }
     else
          GetpMainApp()->OutOfMemory();

}







// ----------------------------------------------------------------------------
//                    ShowWaterfallPlotDialog( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowWaterfallPlotDialog( SchemeFrameWindow* pSchemeFrameWin )
{
     WaterfallPlotDlg* pdlg = new WaterfallPlotDlg( pSchemeFrameWin );

     if ( pdlg )
     {
          pdlg->Execute();
          delete pdlg;
     }
     else
          GetpMainApp()->OutOfMemory();

}



