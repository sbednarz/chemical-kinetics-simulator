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
//  SNGLPLOT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This header file defines the function CreateSimResultDlg
//
//  CREATED : 8.08.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "snglplot.hxx"
#include "mainapp.hxx"
#include "comptobj.hxx"
#include "fileutil.hxx"
#include "xypltfrm.hxx"
#include "cmpstate.hxx"
#include "results.hxx"
#include "plotattr.hxx"

#endif



#include "snglplot.hrc"
#include "strings.hrc"

const UINT16 NUM_DIFFERENT_PLOT_TYPES = 9;  // equal to num values in PLOT_TYPE type - 1

typedef unsigned long PLOT_TYPE;


const PLOT_TYPE NO_PLOTS       = 0x0000,
                CONC_VS_TIME   = 0x0001,
                TEMP_VS_TIME   = 0x0002,
                VOLUME_VS_TIME = 0x0004,
                PRESS_VS_TIME  = 0x0008,
                CONC_VS_TEMP   = 0x0010,
                VOLUME_VS_TEMP = 0x0020,
                PRESS_VS_TEMP  = 0x0040,
                AMOUNT_VS_TIME = 0x0080,
                AMOUNT_VS_TEMP  = 0x0100;






// ----------------------------------------------------------------------------
//                         class OneCompartmentPlotDlg
// ............................................................................
//
//   class declaration
//
// ----------------------------------------------------------------------------

class OneCompartmentPlotDlg : public ModalDialog
{
protected:
     ListBox      compartment_listbox;
     GroupBox     group_box_1;

     CheckBox     conc_v_time_chkbox;
     CheckBox     amount_v_time_chkbox;
     CheckBox     temp_v_time_chkbox;
     CheckBox     volume_v_time_chkbox;
     CheckBox     press_v_time_chkbox;
     CheckBox     conc_v_temp_chkbox;
     CheckBox     amount_v_temp_chkbox;
     CheckBox     volume_v_temp_chkbox;
     CheckBox     press_v_temp_chkbox;
     GroupBox     group_box_2;

     CheckBox     plot_extern_data_chkbox;
     PushButton   select_extern_file_btn;
     ListBox      extdata_type_lb;
     FixedText    extdata_type_lb_label;
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
     FixedText    num_plots_specified_text;
     GroupBox     group_box_6;

     CancelButton cancel_btn;
     OKButton     plot_btn;
     HelpButton   help_btn;

     SchemeWindow*       p_scheme_window;
     CompartmentObject*  p_compt;
     BOOL8               show_species;
     PLOT_TYPE           current_plot_types;
     UINT32              num_pts_to_plot;

     UINT16              GetNumPlotTypesSelected( ) const;
     void                InitializeDialog( );
     void                CompartmentChangedHandler();
     void                InitializeSpeciesSelection( BOOL8 State );
     void                EnableExternalDataFileControls();
     PLOT_TYPE           GetPlotType( const CheckBox* pCheckBox ) const;
     PLOT_TYPE           GetPlotType( const String& rTypeStr ) const;
     void                SaveSelectionsForRecall();
     void                ChangePlotButtonActivation();
     void                CreatePlotWindow( );
     void                InitializePlotLabels( XyPlot* pPlot, PLOT_TYPE PlotType );

     void                SetPlotTypeFlag( PLOT_TYPE Type ) 
                         { current_plot_types = current_plot_types | Type; }

     void                ClearPlotTypeFlag( PLOT_TYPE Type )
                         {
                              current_plot_types =
                               ( current_plot_types & (~Type) );
                         }

public:
                   OneCompartmentPlotDlg( SchemeFrameWindow* pSchemeFrameWin );

     void          SpeciesListboxSelectHandler( MultiListBox* );
     void          CompartmentListboxHandler( ListBox* );
     void          ConcBtnHandler( CheckBox *pCheckBox );
     void          CheckBoxHandler( CheckBox *pCheckBox );
     void          DeselectAllSpeciesHandler( PushButton* );
     void          SelectAllSpeciesHandler( PushButton* );
     void          PreviousSpeciesHandler( PushButton* );
     void          PlotButtonHandler( OKButton* );
     void          SelectExtFileHandler( PushButton* );
     void          ExtFileCheckBoxHandler( CheckBox * pCheckBox );
     void          ExtDataTypeListboxSelectHandler( ListBox* );


};






// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::OneCompartmentPlotDlg()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

OneCompartmentPlotDlg::OneCompartmentPlotDlg( SchemeFrameWindow* pSchemeFrameWin ) :
ModalDialog( pSchemeFrameWin, ResId( vID_DLG_ONE_COMPT_PLOT_SETUP ) ),
compartment_listbox( this, ResId( vID_LB_COMPT_LIST ) ),
group_box_1( this, ResId( 1 ) ),
conc_v_time_chkbox( this, ResId( vID_CB_CONC_V_TIME ) ),
amount_v_time_chkbox( this, ResId( vID_CB_AMOUNT_V_TIME ) ),
temp_v_time_chkbox( this, ResId( vID_CB_TEMP_V_TIME ) ),
volume_v_time_chkbox( this, ResId( vID_CB_VOLUME_V_TIME ) ),
press_v_time_chkbox( this, ResId( vID_CB_PRESS_V_TIME ) ),
conc_v_temp_chkbox( this, ResId( vID_CB_CONC_V_TEMP ) ),
amount_v_temp_chkbox( this, ResId( vID_CB_AMOUNT_V_TEMP ) ),
volume_v_temp_chkbox( this, ResId( vID_CB_VOLUME_V_TEMP ) ),
press_v_temp_chkbox( this, ResId( vID_CB_PRESS_V_TEMP ) ),
group_box_2( this, ResId( 2 ) ),
plot_extern_data_chkbox( this, ResId( vID_CB_PLOT_EXTRNDATA ) ),
select_extern_file_btn( this, ResId( vID_BTN_SELECT_EXTERN_FILE ) ),
extdata_type_lb( this, ResId( vID_LB_EXTDAT_TYPE ) ),
extdata_type_lb_label( this, ResId( vID_STEXT_EXTDATA_TYPE_LABEL ) ),
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
num_plots_specified_text( this, ResId( vID_STEXT_NUM_PLOTS_SPECIFIED ) ),
group_box_6( this, ResId( 6 ) ),
cancel_btn( this, ResId( vID_BTN_CANCEL ) ),
plot_btn( this, ResId( vID_BTN_PLOT ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
p_scheme_window( pSchemeFrameWin->GetpSchemeWindow() ),
p_compt( NULL ),
show_species( FALSE ),
current_plot_types( NO_PLOTS ),
num_pts_to_plot( 0 )
{
     FreeResource();

     InitializeDialog( );

     compartment_listbox.ChangeSelectHdl( LINK( this, OneCompartmentPlotDlg, CompartmentListboxHandler ) );
     species_listbox.ChangeSelectHdl( LINK( this, OneCompartmentPlotDlg, SpeciesListboxSelectHandler ) );
     extdata_type_lb.ChangeSelectHdl( LINK( this, OneCompartmentPlotDlg, ExtDataTypeListboxSelectHandler ) ); 

     conc_v_time_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, ConcBtnHandler ) );
     conc_v_temp_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, ConcBtnHandler ) );
     amount_v_time_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, ConcBtnHandler ) );
     amount_v_temp_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, ConcBtnHandler ) );

     temp_v_time_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, CheckBoxHandler ) );
     volume_v_time_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, CheckBoxHandler ) );
     press_v_time_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, CheckBoxHandler ) );
     volume_v_temp_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, CheckBoxHandler ) );
     press_v_temp_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, CheckBoxHandler ) );


     select_all_species_btn.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, SelectAllSpeciesHandler ) );
     deselect_all_species_btn.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, DeselectAllSpeciesHandler ) );
     select_prev_species_btn.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, PreviousSpeciesHandler ) );

     plot_btn.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, PlotButtonHandler ) );
     select_extern_file_btn.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, SelectExtFileHandler ) );
     plot_extern_data_chkbox.ChangeClickHdl( LINK( this, OneCompartmentPlotDlg, ExtFileCheckBoxHandler ) ); 

     GetpMainApp()->CascadeWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::SetLabels( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::InitializePlotLabels( XyPlot* pPlot, PLOT_TYPE PlotType )
{
     ReactionScheme& rscheme = p_scheme_window->GetScheme();

     // start with the title above the plot 

     String title = p_scheme_window->GetScheme().GetNickName();  // the file name 

     title += String( ResId( vID_STR_TITLE_SEPARATOR ) );

     title += compartment_listbox.GetSelectEntry();               // the compartment name

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
//                OneCompartmentPlotDlg::CreatePlotWindow( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::CreatePlotWindow( )
{
     XyPlotFrameWindow* p_frame_win = new XyPlotFrameWindow( *p_scheme_window );

     if ( NULL == p_frame_win )
     {
          GetpMainApp()->OutOfMemory();
          return;
     }

     XyPlotWindow* p_win = p_frame_win->GetpPlotWindow();

     // made the new windo OK - now get the simulation results for the
     // compartment we are interested in 

     SimulationResult* p_results = p_scheme_window->GetScheme().GetpResults();

     CompartmentState*   p_cmp_state = p_results->GetpComptState( 
      compartment_listbox.GetSelectEntry() );

     // determine whether external data is to be plotted 
     // and its type 

     PLOT_TYPE ext_data_type = NO_PLOTS;
     XyDataFile& r_ext_data = p_win->GetExternalDataFile();

     if ( plot_extern_data_chkbox.IsChecked() )
     {
          // load the data file 
                                              
          r_ext_data.SetFilename( plot_extern_data_chkbox.GetText() );
          r_ext_data.InitializeFromFile();

          // if successully loaded then GetPlotType

          if ( ! r_ext_data.IsStatusFlagSet( SEVERE_ERROR ) )
          {
               ext_data_type = GetPlotType( extdata_type_lb.GetSelectEntry() ); 

               // will the external data be placed on a plot of 
               // simulation results? if not then add plot just for external data

               if ( !( ext_data_type & current_plot_types) )
               {
                    XyPlot* p_plot = p_win->AddPlot( );


                    InitializePlotLabels( p_plot, ext_data_type );
          
                    if (NULL == p_plot )
                    {
                         GetpMainApp()->OutOfMemory();
                         return;
                    }

                    p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(), 
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );

               }
          }
          else
          {
               String msg = r_ext_data.GetErrorMessage( );
               WarningBox( GetParent(), WB_OK | WB_DEF_OK, msg ).Execute();
          }
     }

     // now we work through all the possible plot type requests 

     // if a specific type is requested then we go ahead and 
     // add it to the plot window 




     // ( 1 ) CONCENTRATION VS TIME -------------------------------------------

     if ( current_plot_types & CONC_VS_TIME )
     {
          // get access to species database - we will need this to get SPECIES_ID value

          SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, CONC_VS_TIME );


          // now add each requested species concentration one by one
          // until all sselected species are dealt with

          UINT16 num_species = species_listbox.GetSelectEntryCount();

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               UINT16 position = species_listbox.GetSelectEntryPos( i );

               SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();

               p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetConcentrations( id ), 
                 species_listbox.GetSelectEntry(i) );
          }

          // now add external data if appropriate

          if ( CONC_VS_TIME == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }





     // ( 1a ) AMOUNT VS TIME -----------------------------------------------

     if ( current_plot_types & AMOUNT_VS_TIME )
     {
          // get access to species database - we will need this to get SPECIES_ID value

          SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, AMOUNT_VS_TIME );


          // now add each requested species concentration one by one
          // until all sselected species are dealt with

          UINT16 num_species = species_listbox.GetSelectEntryCount();

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               UINT16 position = species_listbox.GetSelectEntryPos( i );

               SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();

               p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetAmounts( id ), 
                 species_listbox.GetSelectEntry(i) );
          }

          // now add external data if appropriate

          if ( AMOUNT_VS_TIME == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }





     //  ( 2 ) CONCENTRATION VS TEMP ----------------------------------------------- 

     if ( current_plot_types & CONC_VS_TEMP )
     {
          // get access to species database - we will need this to get SPECIES_ID value

          SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, CONC_VS_TEMP );

          // now add each requested species concentration one by one 
          // until all sselected species are dealt with 

          UINT16 num_species = species_listbox.GetSelectEntryCount();

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               UINT16 position = species_listbox.GetSelectEntryPos( i );

               SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();

               p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetConcentrations( id.AsIndex() ),
                 species_listbox.GetSelectEntry(i) );
          }

          // now add external data if appropriate 

          if ( CONC_VS_TEMP == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }





     //  ( 2a ) AMOUNT VS TEMP ----------------------------------------------- 

     if ( current_plot_types & AMOUNT_VS_TEMP )
     {
          // get access to species database - we will need this to get SPECIES_ID value

          SpeciesDatabase* p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, AMOUNT_VS_TEMP );

          // now add each requested species concentration one by one 
          // until all sselected species are dealt with 

          UINT16 num_species = species_listbox.GetSelectEntryCount();

          for ( UINT16 i = 0; i < num_species; i++ )
          {
               UINT16 position = species_listbox.GetSelectEntryPos( i );

               SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();

               p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetAmounts( id.AsIndex() ),
                 species_listbox.GetSelectEntry(i) );
          }

          // now add external data if appropriate 

          if ( AMOUNT_VS_TEMP == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }

     //  ( 3 ) TEMPERATURE VS TIME

     if ( current_plot_types & TEMP_VS_TIME )
     {
          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, TEMP_VS_TIME );

          p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetTemperatures(), 
           String( ResId( vID_STR_TEMPERATURE ) ) );

          // now add external data if appropriate 

          if ( TEMP_VS_TIME == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }

     //  ( 4 ) VOLUME VS TIME

     if ( current_plot_types & VOLUME_VS_TIME )
     {
          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, VOLUME_VS_TIME );

          p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetVolumes(),
           String( ResId( vID_STR_VOLUME ) ) );

          // now add external data if appropriate 

          if ( VOLUME_VS_TIME == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }

     //  ( 5 ) PRESSURE VS TIME

     if ( current_plot_types & PRESS_VS_TIME )
     {
          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, PRESS_VS_TIME );

          p_plot->AddDataSet( p_results->GetElapsedTimes(), p_cmp_state->GetPressures(),
           String( ResId( vID_STR_PRESSURE ) ) );

          // now add external data if appropriate 

          if ( PRESS_VS_TIME == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }


     //  ( 6 ) VOLUME VS TEMPERATURE

     if ( current_plot_types & VOLUME_VS_TEMP )
     {
          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, VOLUME_VS_TEMP );

          p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetVolumes(),
           String( ResId( vID_STR_VOLUME ) ) );

          // now add external data if appropriate 

          if ( VOLUME_VS_TEMP == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }

     //  ( 7 ) PRESSURE VS TEMPERATURE

     if ( current_plot_types & PRESS_VS_TEMP )
     {
          // add a new plot to XyplotWindow

          XyPlot* p_plot = p_win->AddPlot( );

          if (NULL == p_plot )
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

          InitializePlotLabels( p_plot, PRESS_VS_TEMP );

          p_plot->AddDataSet( p_cmp_state->GetTemperatures(), p_cmp_state->GetPressures(),
           String( ResId( vID_STR_PRESSURE ) ) );

          // now add external data if appropriate 

          if ( PRESS_VS_TEMP == ext_data_type )
               p_plot->AddDataSet( r_ext_data.GetXData(), r_ext_data.GetYData(),
                     String( ResId( vID_STR_EXT_DATA_FILE ) ) );
     }

     p_win->SetMaxNumPlotPts( num_pts_to_plot );
     GetpMainApp()->CascadeWindowOnParent( p_frame_win, this );
     p_frame_win->Show();
}





// ----------------------------------------------------------------------------
//              OneCompartmentPlotDlg::SelectExtFileHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::SelectExtFileHandler( PushButton* )
{
     String filename;

     if ( OpenFileDialog( filename, TEXT_FILETYPE) )
     {
          plot_extern_data_chkbox.SetText( filename );
          EnableExternalDataFileControls();
     }
}





// ----------------------------------------------------------------------------
//              OneCompartmentPlotDlg::ChangePlotButtonActivation( )
// ............................................................................
//
//  Only activates 'Create Plot' buttons when appropariate selections have been
//  made by the user 
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::ChangePlotButtonActivation()
{
     // are any plots selected

     if ( GetNumPlotTypesSelected( ) )
     {
          // if so, are any requesting plotting of concentrations ?

          if ( current_plot_types & ( CONC_VS_TIME | CONC_VS_TEMP | AMOUNT_VS_TIME | AMOUNT_VS_TEMP ) )
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
//              OneCompartmentPlotDlg::ExtFileCheckBoxHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::ExtFileCheckBoxHandler( CheckBox* )
{
     num_plots_specified_text.SetText( GetNumPlotTypesSelected() );

     ChangePlotButtonActivation( );
}





// ----------------------------------------------------------------------------
//             OneCompartmentPlotDlg::ExtDataTypeListboxSelectHandler( )
// ............................................................................
//
//  called whenever a new Data type is selected in ext file data type listbox
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::ExtDataTypeListboxSelectHandler( ListBox* )
{
     num_plots_specified_text.SetText( GetNumPlotTypesSelected() );
}







// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::GetNumPlotTypesSelected( )
// ............................................................................
//
//   scan through 'current_plot_types counting number of 1 bits
//
// ----------------------------------------------------------------------------

UINT16 OneCompartmentPlotDlg::GetNumPlotTypesSelected( ) const
{

     PLOT_TYPE plot_types = current_plot_types;

     // if external data file selection chekcbox is checked, include that in calculation

     if ( plot_extern_data_chkbox.IsChecked() )
          plot_types |= GetPlotType( extdata_type_lb.GetSelectEntry() );


     UINT16 num_types_found = 0;

     // test LSB and then do a shift to get to next bit to LSB position

     for (UINT16 i=0; i < NUM_DIFFERENT_PLOT_TYPES; i++, plot_types >>= 1 )
     {
          if ( plot_types & 0x0001 )
               num_types_found++;
     }
     return num_types_found;
}





// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::SaveSelectionsForRecall()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::SaveSelectionsForRecall()
{
     // write stuff to config file to save for later recall

     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

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
          config.WriteKey( String( ResId( vID_STR_EXT_DATA_FILE_TYPE ) ), extdata_type_lb.GetSelectEntry() );
     }
}







// ----------------------------------------------------------------------------
//              OneCompartmentPlotDlg::PlotButtonHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::PlotButtonHandler( OKButton* )
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
//              OneCompartmentPlotDlg::PreviousSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::PreviousSpeciesHandler( PushButton* )
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
//              OneCompartmentPlotDlg::SelectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::SelectAllSpeciesHandler( PushButton* )
{
     UINT16 num_entries = species_listbox.GetEntryCount();

     while (num_entries--)
          species_listbox.SelectEntryPos( num_entries );

     SpeciesListboxSelectHandler( &species_listbox );
}



// ----------------------------------------------------------------------------
//              OneCompartmentPlotDlg::DeselectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::DeselectAllSpeciesHandler( PushButton* )
{
     species_listbox.SetNoSelection( );
     SpeciesListboxSelectHandler( &species_listbox );
}





// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::SpeciesListboxSelectHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::SpeciesListboxSelectHandler( MultiListBox* )
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
//                OneCompartmentPlotDlg::ConcBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::ConcBtnHandler( CheckBox *pCheckBox )
{
     BOOL8 test = ( conc_v_temp_chkbox.IsChecked( ) ||
                    conc_v_time_chkbox.IsChecked( ) ||
                    amount_v_temp_chkbox.IsChecked( ) ||
                    amount_v_time_chkbox.IsChecked( )
                  );

     if ( test != show_species )       /* if state has changed                */
     {
          InitializeSpeciesSelection( test );
          show_species = !show_species;
          SpeciesListboxSelectHandler( &species_listbox );
     }

     CheckBoxHandler( pCheckBox );
}








// ----------------------------------------------------------------------------
//           OneCompartmentPlotDlg::GetPlotType( const CheckBox* pCheckBox )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

PLOT_TYPE OneCompartmentPlotDlg::GetPlotType( const CheckBox* pCheckBox ) const
{
     if ( pCheckBox  == &conc_v_time_chkbox )
          return CONC_VS_TIME;

     if ( pCheckBox  == &amount_v_time_chkbox )
          return AMOUNT_VS_TIME;

     if ( pCheckBox  == &temp_v_time_chkbox )
          return TEMP_VS_TIME;

     if ( pCheckBox  == &volume_v_time_chkbox )
          return VOLUME_VS_TIME;

     if ( pCheckBox  == &press_v_time_chkbox )
          return PRESS_VS_TIME;

     if ( pCheckBox  == &conc_v_temp_chkbox )
          return CONC_VS_TEMP;

     if ( pCheckBox  == &amount_v_temp_chkbox )
          return AMOUNT_VS_TEMP;

     if ( pCheckBox  == &volume_v_temp_chkbox )
          return VOLUME_VS_TEMP;

     if ( pCheckBox  == &press_v_temp_chkbox )
          return PRESS_VS_TEMP;

     // abort if we get here 

     ASSERT( 0 );
}






// ----------------------------------------------------------------------------
//           OneCompartmentPlotDlg::GetPlotType( const String& rTypeStr )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

PLOT_TYPE OneCompartmentPlotDlg::GetPlotType( const String& rTypeStr ) const
{
     String test1( ResId( vID_STR_AMOUNT_TIME ) ) ;

     if ( rTypeStr == test1 ) 
          return AMOUNT_VS_TIME;

     test1 = String( ResId( vID_STR_CONC_TIME ) ) ;
         
         if ( rTypeStr == test1 )
          return CONC_VS_TIME;

                  
     test1 = String( ResId( vID_STR_AMOUNT_TEMP ) );        

     if ( rTypeStr == test1 ) 
          return AMOUNT_VS_TEMP;

     test1 = String( ResId( vID_STR_CONC_TEMP   ) );

     if ( rTypeStr == test1 ) 
          return CONC_VS_TEMP;

                  
     test1 = String( ResId( vID_STR_VOLUME_TIME ) );              

     if ( rTypeStr == test1 ) 
          return VOLUME_VS_TIME;


     test1 = String( ResId( vID_STR_VOLUME_TEMP ) ) ;
         
     if ( rTypeStr == test1 ) 
          return VOLUME_VS_TEMP;


     test1 = String( ResId( vID_STR_PRESS_TIME ) );
         
     if ( rTypeStr == test1 ) 
              return PRESS_VS_TIME;


     test1 = String( ResId( vID_STR_PRESS_TEMP ) ) ;
         
     if ( rTypeStr == test1 ) 
          return PRESS_VS_TEMP;


     test1 = String( ResId( vID_STR_TEMP_TIME ) ) ;
     if ( rTypeStr == test1 ) 
          return TEMP_VS_TIME;

     // abort if we get here 

     ASSERT( 0 );
}




// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::CheckBoxHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::CheckBoxHandler( CheckBox * pCheckBox )
{
     // figure out plot type and set flag in current_plot_types

     const PLOT_TYPE type = GetPlotType( pCheckBox );

     if (pCheckBox->IsChecked() )
          SetPlotTypeFlag( type );
     else
          ClearPlotTypeFlag( type );

     num_plots_specified_text.SetText( GetNumPlotTypesSelected() );

     ChangePlotButtonActivation( );
}






// ----------------------------------------------------------------------------
//              OneCompartmentPlotDlg::InitializeSpeciesSelection()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::InitializeSpeciesSelection( BOOL8 State )
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
//                OneCompartmentPlotDlg::CompartmentListboxHandler( )
// ............................................................................
//
//  called whenever a new Compartment is selected in compartment listbox
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::CompartmentListboxHandler( ListBox* )
{
     p_compt = p_scheme_window->GetScheme().GetCompartmentObjectPtrFromName(
      compartment_listbox.GetSelectEntry() );

//     CompartmentChangedHandler( );

//     ChangePlotButtonActivation();

}



// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::CompartmentChangedHandler()
// ............................................................................
//
//  update checkboxes to account for characteristics of selected compartment
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::CompartmentChangedHandler()
{
     // reset all checkboxes

     conc_v_time_chkbox.Check( FALSE );
     amount_v_time_chkbox.Check( FALSE );
     temp_v_time_chkbox.Check( FALSE );
     volume_v_time_chkbox.Check( FALSE );
     press_v_time_chkbox.Check( FALSE );
     conc_v_temp_chkbox.Check( FALSE );
     amount_v_temp_chkbox.Check( FALSE );
     volume_v_temp_chkbox.Check( FALSE );
     press_v_temp_chkbox.Check( FALSE );

     InitializeSpeciesSelection( FALSE );
     num_species_selected_text.SetText( 0  );
     show_species = FALSE;

     current_plot_types = NO_PLOTS;
     num_plots_specified_text.SetText( GetNumPlotTypesSelected() );

#if 0 
     enum TEMPERATURE_OPTION temp_option = p_compt->GetTemperatureOption ( );
     enum VOLUME_OPTION      volume_option = p_compt->GetVolumeOption();
     enum PRESSURE_OPTION    press_option = p_compt->GetPressureOption();

     if ( volume_option == VARIABLE_VOLUME )
     {
          volume_v_time_chkbox.Enable( );
          conc_v_temp_chkbox.SetText( ResId( vID_STR_AMOUNT_TEMP_2 )  );
          conc_v_time_chkbox.SetText( ResId( vID_STR_AMOUNT_TIME_2 ) );
     }
     else
     {
          volume_v_time_chkbox.Disable( );
          conc_v_temp_chkbox.SetText( ResId( vID_STR_CONC_TEMP_2 )  );
          conc_v_time_chkbox.SetText( ResId( vID_STR_CONC_TIME_2 ) );
     }

     if ( press_option == VARIABLE_PRESSURE )
          press_v_time_chkbox.Enable( );
     else
          press_v_time_chkbox.Disable( );

     if ( temp_option != CONSTANT_TEMP )
     {
          conc_v_temp_chkbox.Enable( );
          temp_v_time_chkbox.Enable( );
     }
     else
     {
          conc_v_temp_chkbox.Disable( );
          temp_v_time_chkbox.Disable( );
     }


     if ( ( volume_option == VARIABLE_VOLUME ) && ( temp_option != CONSTANT_TEMP ) )
          volume_v_temp_chkbox.Enable( );
     else
          volume_v_temp_chkbox.Disable( );

     if ( ( press_option == VARIABLE_PRESSURE ) &&
          ( temp_option != CONSTANT_TEMP )
         )
          press_v_temp_chkbox.Enable( );
     else
          press_v_temp_chkbox.Disable( );

#endif

}







// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::InitializeDialog( )
{
     // initialize the "select compartment" listbox

     UINT16 num_compts  = p_scheme_window->GetScheme().GetNumCompartments( );


     for ( UINT16 i = 0; i < num_compts; i++ )
          compartment_listbox.InsertEntry(
           p_scheme_window->GetScheme().GetCompartmentObjectPtrFromIndex( i )->GetName()
          );

     p_compt = p_scheme_window->GetScheme().GetCompartmentObjectPtrFromIndex( 0 );
     compartment_listbox.SelectEntryPos( 0, TRUE );

     CompartmentChangedHandler( );

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

          // select data type listbox entry based on the saved data type

          tempstr = config.ReadKey( String( ResId( vID_STR_EXT_DATA_FILE_TYPE ) ) );

          if (LISTBOX_ENTRY_NOTFOUND != extdata_type_lb.GetEntryPos( tempstr ) )
               extdata_type_lb.SelectEntry( tempstr );
          else
               extdata_type_lb.SelectEntryPos( 0 );
     }
     else
          plot_extern_data_chkbox.SetText( String( ResId( vID_STR_NO_EXT_DATA_FILE ) ) );

     // set some text

      num_pts_avail_text.SetText( String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );

     // set to minimum of available points in simulation of MAX_PTS_DEFAULT

      max_pts_to_plot_ef.SetText(
       String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() > MAX_PTS_DEFAULT ?
       MAX_PTS_DEFAULT : p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );

     // now initialize the external data listbox entries 

     extdata_type_lb.ChangeUpdateMode( FALSE ); // turn off update to avoid flicker

     extdata_type_lb.Clear();

     extdata_type_lb.InsertEntry( String( ResId( vID_STR_CONC_TIME ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_AMOUNT_TIME ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_TEMP_TIME ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_VOLUME_TIME ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_VOLUME_TEMP ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_CONC_TEMP ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_AMOUNT_TEMP ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_PRESS_TIME ) ) );
     extdata_type_lb.InsertEntry( String( ResId( vID_STR_PRESS_TEMP ) ) );

     extdata_type_lb.SelectEntryPos( 0, TRUE );

     extdata_type_lb.ChangeUpdateMode( TRUE );
     extdata_type_lb.Update( );
}






// ----------------------------------------------------------------------------
//                OneCompartmentPlotDlg::EnableExternalDataFileControls()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void OneCompartmentPlotDlg::EnableExternalDataFileControls()
{
     plot_extern_data_chkbox.Enable( );
     extdata_type_lb.Enable( );
     extdata_type_lb_label.Enable( );
}





// ----------------------------------------------------------------------------
//                ShowOneComptPlotDialog( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowOneComptPlotDialog( SchemeFrameWindow* pSchemeFrameWin )
{
     OneCompartmentPlotDlg* pdlg = new OneCompartmentPlotDlg( pSchemeFrameWin );

     if ( pdlg )
     {
          pdlg->Execute();
          delete pdlg;
     }
     else
          GetpMainApp()->OutOfMemory();

}


