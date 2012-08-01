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
//  TEXTRSLT.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module file defines the class TextResultsDialog
//
//  CREATED : 8.16.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



//    the general structure of the text resulrs display buffer is as follows
//
//
//    left edge of table
//    |
//    -- header line 1
//    -- header line 2
//    <blank line>
//                             Compt 1 name                                                   Compt 2 name                                    
//                -------------------------------------------------------------  -------------------------------------------------------------
//                       Amounts              Concs                                     Amounts              Concs                            
//                ------------------------ ------------------------              ------------------------ ------------------------            
//      Time        SpeciesA    SpeciesB     SpeciesA    SpeciesB       Temp       SpeciesA    SpeciesB     SpeciesA    SpeciesB       Temp   
//      (sec)        (moles)     (moles)     (mole/l)    (mole/l)      (deg K)      (moles)     (moles)     (mole/l)    (mole/l)      (deg K) 
//     --------   ----------   ----------   ---------    --------      --------
//        0            0          1           6             7            500
//        1            0          1           6             6            501
//        2            0          1           6             5            504
//        3            0          1           6             4
//
//
//
//
//





#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "textrslt.hxx"
#include "mainapp.hxx"
#include "comptobj.hxx"
#include "fileutil.hxx"
#include "cmpstate.hxx"
#include "results.hxx"
#include "txtstrem.hxx"

#endif


#include <limits.h>

#include "textrslt.hrc"
#include "strings.hrc"
#include "help.hhc"




const UINT16 NUMERIC_FIELD_WIDTH     = 11;
const UINT16 COLUMN_SPACER_WIDTH     = 2;
const UINT16 HEADER_WIDTH            = 80;
const UINT16 NUM_DIGITS_RIGHT_OF_DEC_PT = 5;

const char   UNDERLINE_CHAR = '-';
const char   SPACE_CHAR     = ' ';
const char   HEADER_DELIMITER_CHAR = '-';


typedef unsigned long DATA_TYPE;


const DATA_TYPE NO_DATA      = 0x0000,
                CONC_DATA    = 0x0001,
                TEMP_DATA    = 0x0002,
                VOLUME_DATA  = 0x0004,
                PRESS_DATA   = 0x0008,
                AMOUNT_DATA  = 0x0010;





// ----------------------------------------------------------------------------
//                         class OneCompartmentPlotDlg
// ............................................................................
//
//   class declaration
//
// ----------------------------------------------------------------------------

class TextResultsDialog : public ModalDialog
{
protected:
     MultiListBox compartment_listbox;
     PushButton   select_all_compts_btn;
     PushButton   deselect_all_compts_btn;
     PushButton   select_prev_compts_btn;
     GroupBox     group_box_1;

     CheckBox     conc_cb;
     CheckBox     amount_cb;
     CheckBox     temp_cb;
     CheckBox     volume_cb;
     CheckBox     press_cb;

     GroupBox     group_box_2;

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
     OKButton     show_btn;
     HelpButton   help_btn;

     LineList*           pline_list;

     StatusWindow*       pstatwin;

     SchemeWindow*       p_scheme_window;
     BOOL8               show_species;
     DATA_TYPE           current_data_type;
     UINT32              num_pts_to_print;
     String              column_spacing;
     String              blank_numeric_field;

     void                InitializeDialog( );
     void                InitializeSpeciesSelection( BOOL8 State );
     DATA_TYPE           GetDataType( const CheckBox*  pCheckBox ) const;
     DATA_TYPE           GetDataType( const String& rTypeStr ) const;
     void                SaveSelectionsForRecall();
     void                ChangeShowButtonActivation();

     void                CreateDataWindow( );

     void                BuildHeader( );
     void                BuildFooter( );
     void                BuildColumnHeadings( );
     void                BuildDataLines( );

     void                CenterString(  const String& rStringtoCenter, String& rTargetBuffer, 
                                        UINT16 Width, char FillChar );

     void                SetDataTypeFlag( DATA_TYPE Type ) 
                         { current_data_type = current_data_type | Type; }

     void                ClearDataTypeFlag( DATA_TYPE Type )
                         { current_data_type = ( current_data_type & (~Type) ); }


     BOOL8               ShowTemperatureData() const { return ( current_data_type & TEMP_DATA);}
     BOOL8               ShowConcentrationData() const { return ( current_data_type & CONC_DATA);}
     BOOL8               ShowVolumeData() const { return ( current_data_type & VOLUME_DATA);}
     BOOL8               ShowPressureData() const { return ( current_data_type & PRESS_DATA);}
     BOOL8               ShowAmountData() const { return ( current_data_type & AMOUNT_DATA);}


public:
                         TextResultsDialog( SchemeFrameWindow* pSchemeFrameWin );

     void                SpeciesListboxSelectHandler( MultiListBox* );
     void                CompartmentListboxHandler( MultiListBox* );
     void                ConcBtnHandler( CheckBox *pCheckBox );
     void                CheckBoxHandler( CheckBox* pCheckBox );
     void                DeselectAllSpeciesHandler( PushButton* );
     void                SelectAllSpeciesHandler( PushButton* );
     void                PreviousSpeciesHandler( PushButton* );
     void                DeselectAllComptsHandler( PushButton* );
     void                SelectAllComptsHandler( PushButton* );
     void                PreviousComptsHandler( PushButton* );
     void                ShowButtonHandler( OKButton* );
};






// ----------------------------------------------------------------------------
//                TextResultsDialog::TextResultsDialog()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

TextResultsDialog::TextResultsDialog( SchemeFrameWindow* pSchemeFrameWin ) :
ModalDialog( pSchemeFrameWin, ResId( vID_DLG_TEXT_RESULTS_SETUP ) ),
compartment_listbox( this, ResId( vID_LB_COMPT_LIST ) ),
select_all_compts_btn( this, ResId( vID_BTN_SELECT_ALL_COMPTS ) ),
deselect_all_compts_btn( this, ResId( vID_BTN_DESELECT_ALL_COMPTS ) ),
select_prev_compts_btn( this, ResId( vID_BTN_PREV_COMPTS_SELECT ) ),
group_box_1( this, ResId( 1 ) ),
conc_cb( this, ResId( vID_CB_CONC ) ),
amount_cb( this, ResId( vID_CB_AMOUNT ) ), 
temp_cb( this, ResId( vID_CB_TEMP ) ),
volume_cb( this, ResId( vID_CB_VOLUME ) ),
press_cb( this, ResId( vID_CB_PRESS ) ),
group_box_2( this, ResId( 2 ) ),
species_listbox( this, ResId( vID_LB_SPECIES_LIST ) ),
select_all_species_btn( this, ResId( vID_BTN_SELECT_ALL_SPECIES ) ),
deselect_all_species_btn( this, ResId( vID_BTN_DESELECT_ALL_SPECIES ) ),
select_prev_species_btn( this, ResId( vID_BTN_PREV_SPECIES_SELECT ) ),
species_list_group_box( this, ResId( vID_GB_SPECIES_LABEL ) ),
fixed_text_4( this, ResId( 4 ) ),
fixed_text_7( this, ResId( 7 ) ),
num_pts_avail_text( this, ResId( vID_STEXT_NUM_POINTS_AVAIL ) ),
max_pts_to_plot_ef( this, ResId( vID_EF_MAX_POINTS_TO_DISPLAY ) ),
group_box_5( this, ResId( 5 ) ),
num_selected_species_label( this, ResId( vID_STEXT_NUM_SPECIES_SELECTED_LABEL ) ),
fixed_text_10( this, ResId( 10 ) ),
num_species_selected_text( this, ResId( vID_STEXT_NUM_SPECIES_SELECTED ) ),
num_compts_selected_text( this, ResId( vID_STEXT_NUM_COMPTS_SELECTED ) ),
group_box_6( this, ResId( 6 ) ),
cancel_btn( this, ResId( vID_BTN_CANCEL ) ),
show_btn( this, ResId( vID_BTN_PLOT ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
pline_list( 0 ),
p_scheme_window( pSchemeFrameWin->GetpSchemeWindow() ),
show_species( FALSE ),
current_data_type( NO_DATA ),
num_pts_to_print( 0 )
{
     FreeResource();

     InitializeDialog( );

     compartment_listbox.ChangeSelectHdl( LINK( this, TextResultsDialog, CompartmentListboxHandler ) );
     species_listbox.ChangeSelectHdl( LINK( this, TextResultsDialog, SpeciesListboxSelectHandler ) );

     conc_cb.ChangeClickHdl( LINK( this, TextResultsDialog, ConcBtnHandler ) );
     amount_cb.ChangeClickHdl( LINK( this, TextResultsDialog, ConcBtnHandler ) );

     temp_cb.ChangeClickHdl( LINK( this, TextResultsDialog, CheckBoxHandler ) );
     volume_cb.ChangeClickHdl( LINK( this, TextResultsDialog, CheckBoxHandler ) );
     press_cb.ChangeClickHdl( LINK( this, TextResultsDialog, CheckBoxHandler ) );

     select_all_species_btn.ChangeClickHdl( LINK( this, TextResultsDialog, SelectAllSpeciesHandler ) );
     deselect_all_species_btn.ChangeClickHdl( LINK( this, TextResultsDialog, DeselectAllSpeciesHandler ) );
     select_prev_species_btn.ChangeClickHdl( LINK( this, TextResultsDialog, PreviousSpeciesHandler ) );

     select_all_compts_btn.ChangeClickHdl( LINK( this, TextResultsDialog, SelectAllComptsHandler ) );
     deselect_all_compts_btn.ChangeClickHdl( LINK( this, TextResultsDialog, DeselectAllComptsHandler ) );
     select_prev_compts_btn.ChangeClickHdl( LINK( this, TextResultsDialog, PreviousComptsHandler ) );

     show_btn.ChangeClickHdl( LINK( this, TextResultsDialog, ShowButtonHandler ) );

     GetpMainApp()->CascadeWindowOnParent( this, GetParent() );

     column_spacing.Fill( COLUMN_SPACER_WIDTH, SPACE_CHAR );
     blank_numeric_field.Fill( NUMERIC_FIELD_WIDTH, SPACE_CHAR ); 
}






// ----------------------------------------------------------------------------
//                TextResultsDialog::BuildHeader( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::BuildHeader( )
{
     if (!pline_list)
          return; 

     String* pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     CenterString( String( ResId( vID_STR_RESULTS_HDR1 ) ) + p_scheme_window->GetScheme().GetName(), 
                   *pstr, HEADER_WIDTH, HEADER_DELIMITER_CHAR );

     // a blank line 

     pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     // current time and date printed 

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     DateTime       current_time;

     International  intl;

     CenterString( String( ResId( vID_STR_RESULTS_HDR2 ) ) + intl.GetLongDate( current_time ) +
                   String( ResId( vID_STR_RESULTS_HDR3 ) ) + intl.GetTime( current_time ),
                   *pstr, HEADER_WIDTH, SPACE_CHAR );


     // add time stamp of simulation 

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     CenterString( String( ResId( vID_STR_RESULTS_HDR4 ) ) + 
                   intl.GetLongDate( p_scheme_window->GetScheme().GetSimulationStartDate() ) + 
                   String( ResId( vID_STR_RESULTS_HDR5 ) ) 
                   + intl.GetTime( p_scheme_window->GetScheme().GetSimulationStartDate() ), 
                   *pstr, HEADER_WIDTH, SPACE_CHAR );

     // add a couple blank lines to delimit header 

     pline_list->AddAnotherLine();
     pline_list->AddAnotherLine();
}







// ----------------------------------------------------------------------------
//                TextResultsDialog::BuildFooter( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::BuildFooter( )
{
     if (!pline_list)
          return; 

     // add a blank line as footer

     pline_list->AddAnotherLine();
}










// ----------------------------------------------------------------------------
//                TextResultsDialog::BuildDataLines( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::BuildDataLines()
{
     String*     pstr;

     if (!pline_list)
          return; 

     SimulationResult*   p_results = p_scheme_window->GetScheme().GetpResults();
     SpeciesDatabase*    p_spec_db = p_scheme_window->GetScheme().GetpSpeciesDatabase();

     const FloatArray& rtime = p_results->GetElapsedTimes();

     UINT32 num_pts = p_scheme_window->GetScheme().GetNumSimulationDataPoints();

     const UINT32 init_line_num = pline_list->GetNumLines();


     // first create as many empty Strings as we are going to need to tabulate all the requested data 
     // write the time data while we are at it since no matter what the Time values are always
     // written first 

     for ( UINT32 i = 0; i < num_pts; i++ )
     {
          pstr = pline_list->AddAnotherLine();

          if ( NULL == pstr )
               return;
          else
          {
               // write the index 
               String index;

               CenterString( String( i+1 ), 
                                  index, 
                                  NUMERIC_FIELD_WIDTH, SPACE_CHAR );

               *pstr += index;
               *pstr += column_spacing;

               // and the elapsed time value 
               *pstr += NumericString( rtime[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
               *pstr += column_spacing;
          }

     }

     // now go through compartment by compartment and append the requested data to the 
     // end of each String 

     const UINT16 num_selected_compts   = compartment_listbox.GetSelectEntryCount();
     const UINT16 num_selected_species  = species_listbox.GetSelectEntryCount();

     for ( UINT16 j = 0; j < num_selected_compts; j++)
     {
          if (NULL != pstatwin)
          {
              pstatwin->SetStatusText( String( ResId( vID_STR_PROCESSING_COMPT ) ) +
                                     compartment_listbox.GetSelectEntry( j ) );

//            GetpMainApp()->Reschedule();
          }

          CompartmentState*   p_cmp_state = p_results->GetpComptState( compartment_listbox.GetSelectEntry( j ) );

          if ( ShowAmountData() )
          {
               for ( UINT16 k = 0; k < num_selected_species; k++ )
               {
                    UINT16 position = species_listbox.GetSelectEntryPos( k );
     
                    SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
     
                    const FloatArray& r_amt = p_cmp_state->GetAmounts( id );

                    UINT32 cur_line;

                    for ( cur_line = init_line_num, i = 0 ; i < num_pts; i++, cur_line++ )
                    {
                         pstr = pline_list->GetLineNum( cur_line );
               
                         *pstr += NumericString( r_amt[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
                         *pstr += column_spacing;
               
                    }
     
               }
          }


          if ( ShowConcentrationData() )
          {
               for ( UINT16 k = 0; k < num_selected_species; k++ )
               {
                    UINT16 position = species_listbox.GetSelectEntryPos( k );
     
                    SPECIES_ID id = p_spec_db->GetSpeciesDataByPosition( position ).GetID();
     
                    const FloatArray& r_conc = p_cmp_state->GetConcentrations( id );

                    UINT32 cur_line;

                    for ( i = 0, cur_line = init_line_num; i < num_pts; i++, cur_line++ )
                    {
                         pstr = pline_list->GetLineNum( cur_line );
               
                         *pstr += NumericString( r_conc[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
                         *pstr += column_spacing;
               
                    }
     
               }
          }

          if ( ShowTemperatureData() )
          {
                    const FloatArray& r_temp = p_cmp_state->GetTemperatures();

                    UINT32 cur_line;
                    for ( i = 0, cur_line = init_line_num; i < num_pts; i++, cur_line++ )
                    {
                         pstr = pline_list->GetLineNum( cur_line );
               
                         *pstr += NumericString( r_temp[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
                         *pstr += column_spacing;
               
                    }
          }
     
          if ( ShowVolumeData() )
          {
                    const FloatArray& r_vol = p_cmp_state->GetVolumes();

                    UINT32 cur_line;
                    for ( i = 0, cur_line = init_line_num; i < num_pts; i++, cur_line++ )
                    {
                         pstr = pline_list->GetLineNum( cur_line );
               
                         *pstr += NumericString( r_vol[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
                         *pstr += column_spacing;
               
                    }
          }
     
          if ( ShowPressureData() )
          {
                    const FloatArray& r_press = p_cmp_state->GetPressures();

                    UINT32 cur_line;
                    for ( i = 0, cur_line = init_line_num; i < num_pts; i++, cur_line++ )
                    {
                         pstr = pline_list->GetLineNum( cur_line );
               
                         *pstr += NumericString( r_press[i], NUMERIC_FIELD_WIDTH, NUM_DIGITS_RIGHT_OF_DEC_PT );
                         *pstr += column_spacing;
               
                    }
          }

     }
}













// ----------------------------------------------------------------------------
//                TextResultsDialog::CenterString()
// ............................................................................
//
//   centers a string within a longer one of width Width using FillChar at the 
//   beginning and end to fill it out
//
// ----------------------------------------------------------------------------

void TextResultsDialog::CenterString(  const String& rStringToCenter, 
                                       String& rTargetBuffer, 
                                       UINT16 Width, char FillChar )

{
     // strip leading. trailing blanks

     String temp( rStringToCenter );

     temp.EraseTrailingChars().EraseLeadingChars();

     // if line is too big just copy it - note we allow for the blank
     // chars we will add to each end of the centered string 

     if ( temp.Len() >= Width-2 )
     {
          rTargetBuffer = temp;
     }
     else
     {
          String blank_char( SPACE_CHAR );
     
          rTargetBuffer.Erase();
          rTargetBuffer.Fill( Width, FillChar );
          rTargetBuffer.Replace( blank_char +
           temp + blank_char, (Width - (rStringToCenter.Len() + 2) )/2 );
     }

}









// ----------------------------------------------------------------------------
//                TextResultsDialog::BuildColumnHeadings( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::BuildColumnHeadings( )
{
     String* pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     const UINT16 num_selected_compts   = compartment_listbox.GetSelectEntryCount();
     const UINT16 num_selected_species  = species_listbox.GetSelectEntryCount();

     UINT16 columns_per_compartment = 0;
        
     if ( ShowTemperatureData() )
          columns_per_compartment++;

     if ( ShowVolumeData() )
          columns_per_compartment++;

     if ( ShowPressureData() )
          columns_per_compartment++;

     if ( ShowConcentrationData() )
          columns_per_compartment += num_selected_species;

     if ( ShowAmountData() )
          columns_per_compartment += num_selected_species;

     const UINT16 overall_width_per_compartment = 
      ( (columns_per_compartment * (NUMERIC_FIELD_WIDTH  + COLUMN_SPACER_WIDTH )) 
       -  COLUMN_SPACER_WIDTH );


     //  now let's start building the column headers 

     // the Compartment names go at the top

     // first fill to leave room for 'Point Number' and 'Time' columns 

     *pstr += blank_numeric_field;
     *pstr += column_spacing;
     *pstr += blank_numeric_field;

     for ( UINT16 c = 0;  c < num_selected_compts; c++ )
     {
          // center compt name in a line of blanks 

          String workstr;

          CenterString( compartment_listbox.GetSelectEntry( c ), workstr, overall_width_per_compartment, SPACE_CHAR );

          // insert blanks for column separation 

          *pstr += column_spacing;

          // add the substring to the buffer 

          *pstr += workstr;
     }


     // now do the dashed line under the compartment names 

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     // first fill to leave room for 'Point Number' and 'Time' columns

     *pstr += blank_numeric_field;
     *pstr += column_spacing;
     *pstr += blank_numeric_field;

     for ( UINT16 i= 0;  i < num_selected_compts; i++ )
     {
          String workstr;

          workstr.Fill( overall_width_per_compartment, UNDERLINE_CHAR );

          // insert blanks for column separation 

          *pstr += column_spacing;

          // add the substring to the buffer 

          *pstr += workstr;
     }

     // add another blank line 

     pline_list->AddAnotherLine();

     // now deal with 'amounts' and 'concs' headers

     if ( ShowConcentrationData() || ShowAmountData()  )
     {
          String workstr;

          // get another string from LineList

          pstr = pline_list->AddAnotherLine();
     
          if ( NULL == pstr )
               return;

          // first fill to leave room for 'Point Number' and 'Time' columns

          *pstr += blank_numeric_field;
          *pstr += column_spacing;
          *pstr += blank_numeric_field;

          UINT16 required_width = (num_selected_species * NUMERIC_FIELD_WIDTH)  + 
                                  ( (num_selected_species-1) *COLUMN_SPACER_WIDTH);

          for ( UINT16 j = 0;  j < num_selected_compts; j++ )
          {

               if ( ShowAmountData() )
               {
                    *pstr += column_spacing;
     
                    CenterString( String(ResId( vID_STR_AMOUNT) ), 
                                  workstr, 
                                  required_width, SPACE_CHAR );
     
                    *pstr += workstr;
               }
     
               if ( ShowConcentrationData() )
               {
                    *pstr += column_spacing;
     
                    CenterString( String(ResId( vID_STR_CONCENTRATION ) ), 
                                  workstr, 
                                  required_width, SPACE_CHAR );
     
                    *pstr += workstr;
               }

               if ( ShowTemperatureData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          
               if ( ShowVolumeData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          
               if ( ShowPressureData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          }


          // add underlines under the "Amount" and Conc" headers

          pstr = pline_list->AddAnotherLine();
     
          if ( NULL == pstr )
               return;

          *pstr += blank_numeric_field;
          *pstr += column_spacing;
          *pstr += blank_numeric_field;

          for ( j = 0;  j < num_selected_compts; j++ )
          {

               if ( ShowAmountData() )
               {
                    *pstr += column_spacing;
     
                    workstr.Fill( required_width, UNDERLINE_CHAR );
     
                    *pstr += workstr;
               }
     
               if ( ShowConcentrationData() )
               {
                    *pstr += column_spacing;
     
                    workstr.Fill( required_width, UNDERLINE_CHAR );
     
                    *pstr += workstr;
               }

               if ( ShowTemperatureData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          
               if ( ShowVolumeData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          
               if ( ShowPressureData() )
               {
                    *pstr += column_spacing;
                    *pstr += blank_numeric_field;
               }
          }

     }

     // now we are ready to create the line with the actual column labels 

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;
     else
     {
          CenterString( String( ResId( vID_STR_POINT) ), *pstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
          *pstr += column_spacing;

          String timestr;

          CenterString( String( ResId( vID_STR_TIME ) ), timestr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
          *pstr += timestr;
     }

     for ( UINT16 k = 0; k < num_selected_compts; k++ )
     {
          String tempstr;

          if ( ShowAmountData() )
          {

               for ( UINT16 m=0; m < num_selected_species; m++ )
               {
                    *pstr += column_spacing;
     
                    CenterString( species_listbox.GetSelectEntry( m ), tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
                    *pstr += tempstr;
               }
          }


          if ( ShowConcentrationData() )
          {

               for ( UINT16 m=0; m < num_selected_species; m++ )
               {
                    *pstr += column_spacing;

                    CenterString( species_listbox.GetSelectEntry( m ), tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
                    *pstr += tempstr;
               }
          }

          if ( ShowTemperatureData() )
          {
               *pstr += column_spacing;
               CenterString( String( ResId( vID_STR_TEMPERATURE)), tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
               *pstr += tempstr;

          }
     
          if ( ShowVolumeData() )
          {
               *pstr += column_spacing;
               CenterString( String( ResId( vID_STR_VOLUME)), tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
               *pstr += tempstr;
          }
     
          if ( ShowPressureData() )
          {
               *pstr += column_spacing;
               CenterString( String( ResId( vID_STR_PRESSURE)), tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
               *pstr += tempstr;
          }

     }

     // add units information 

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     String units_work_str;

     ReactionScheme& scheme = p_scheme_window->GetScheme();

     String time_units    = scheme.GetTimeUnits();
     String amount_units  = scheme.GetAmountUnits();
     String conc_units    = scheme.GetConcentrationUnits();
     String temp_units    = scheme.GetTemperatureUnits();
     String volume_units  = scheme.GetVolumeUnits();
     String press_units   = scheme.GetPressureUnits();



     CenterString( String( ResId( vID_STR_NUMBER) ), *pstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );
          *pstr += column_spacing;

     CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                   scheme.GetTimeUnits() + 
                   String( ResId( vID_STR_RIGHT_BRACKET ) ),
                   time_units, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

          *pstr += time_units;



     for ( k = 0; k < num_selected_compts; k++ )
     {
          String tempstr;

          if ( ShowAmountData() )
          {
               for ( UINT16 m=0; m < num_selected_species; m++ )
               {
                    *pstr += column_spacing;
     
                    CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                                  amount_units  + 
                                  String( ResId( vID_STR_RIGHT_BRACKET ) ), 
                                  tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

                    *pstr += tempstr;
               }
          }


          if ( ShowConcentrationData() )
          {
               for ( UINT16 m=0; m < num_selected_species; m++ )
               {
                    *pstr += column_spacing;
     
                    CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                                  conc_units + 
                                  String( ResId( vID_STR_RIGHT_BRACKET ) ), 
                                  tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

                    *pstr += tempstr;
               }
          }

          if ( ShowTemperatureData() )
          {
               *pstr += column_spacing;

               CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                             temp_units +
                             String( ResId( vID_STR_RIGHT_BRACKET ) ), 
                             tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

               *pstr += tempstr;
          }
     
          if ( ShowVolumeData() )
          {
               *pstr += column_spacing;

               CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                             volume_units +
                             String( ResId( vID_STR_RIGHT_BRACKET ) ), 
                             tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

               *pstr += tempstr;
          }
     
          if ( ShowPressureData() )
          {
               *pstr += column_spacing;

               CenterString( String( ResId( vID_STR_LEFT_BRACKET ) ) + 
                             press_units  + 
                             String( ResId( vID_STR_RIGHT_BRACKET ) ), 
                             tempstr, NUMERIC_FIELD_WIDTH, SPACE_CHAR );

               *pstr += tempstr;
          }

     }


     // add underline right before data

     pstr = pline_list->AddAnotherLine();

     if ( NULL == pstr )
          return;

     pstr->Fill( ( (overall_width_per_compartment + COLUMN_SPACER_WIDTH)  * num_selected_compts ) + NUMERIC_FIELD_WIDTH +
     NUMERIC_FIELD_WIDTH + COLUMN_SPACER_WIDTH, 
     UNDERLINE_CHAR );
}






// ----------------------------------------------------------------------------
//                TextResultsDialog::CreateDataWindow( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::CreateDataWindow( )
{
     pline_list = new LineList;

     if ( NULL == pline_list )
     {
          GetpMainApp()->OutOfMemory();
          return;
     }
     else
     {
          pstatwin = new StatusWindow( NULL );

          if (pstatwin)
          {
               pstatwin->SetStatusText( String( ResId( vID_STR_BUILDING_HEADER ) ) );
               pstatwin->Show();
               pstatwin->Update();
               GetpMainApp()->Reschedule();
          }

          BuildHeader( );
          BuildColumnHeadings( );
          BuildDataLines();
          BuildFooter( );

          delete pstatwin;
     }

     TextFrameWindow* p_frame_win = new TextFrameWindow( *p_scheme_window, pline_list );

     if( NULL != p_frame_win )
     {
          GetpMainApp()->CascadeWindowOnParent( p_frame_win, this );
          p_frame_win->Show();
     }
     else
          GetpMainApp()->OutOfMemory();
}






// ----------------------------------------------------------------------------
//              TextResultsDialog::ChangeShowButtonActivation( )
// ............................................................................
//
//  Only activates 'Create Window' buttons when appropariate selections have been
//  made by the user 
//
// ----------------------------------------------------------------------------

void TextResultsDialog::ChangeShowButtonActivation()
{
     // are any compartments selected 

     if ( compartment_listbox.GetSelectEntryCount() )
     {
          // if so, are any requesting plotting of concentrations ?

          if ( current_data_type & ( CONC_DATA | AMOUNT_DATA ) )
          {
               // if so, are any species selected 

               if ( species_listbox.GetSelectEntryCount() )
               {
                      show_btn.Enable();
               }
               else
               {
                      show_btn.Disable();
               }

          }
          else
          {
               if ( current_data_type != NO_DATA )
                    show_btn.Enable();
               else
                    show_btn.Disable();
          }
     }
     else
          show_btn.Disable();
}










// ----------------------------------------------------------------------------
//                TextResultsDialog::SaveSelectionsForRecall()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::SaveSelectionsForRecall()
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

          for ( USHORT j = 1; j <= num_saved_species_names; j++ )
               config.DeleteKey( String( ResId( vID_STR_PREV_SPECIES_NAME_STUB )) +
                               String( j ) );

          UINT16 num_species_to_save = species_listbox.GetSelectEntryCount();

          if ( num_species_to_save )
          {
               config.WriteKey( String( ResId( vID_STR_NUM_PREV_SPECIES_NAMES )),
                num_species_to_save );

               for ( j = 0; j < num_species_to_save; j++ )
                    config.WriteKey(
                     String( ResId( vID_STR_PREV_SPECIES_NAME_STUB )) + String( j+1 ),
                     species_listbox.GetSelectEntry( j )
                     );
          }

     }

}







// ----------------------------------------------------------------------------
//              TextResultsDialog::PlotButtonHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::ShowButtonHandler( OKButton* )
{
     // first check user input in specifying number of points to plot 

     NumericString numstr( max_pts_to_plot_ef.GetText() );
     UINT32        value;

     if ( numstr.IsValidUINT32( &value ) && 
          ( value >= MIN_PTS ) &&
          ( value <= MAX_PTS )
        )
     {
          num_pts_to_print = value;

          SaveSelectionsForRecall();

          CreateDataWindow( );

          EndDialog();
     }
     else
     {
          String message = String( ResId( vID_STR_MAX_SHOW_PTS_INPUT_ERR1 ) ) + 
           numstr + 
           String( ResId( vID_STR_MAX_SHOW_PTS_INPUT_ERR2 ) );

          WarningBox( GetParent(), WB_OK | WB_DEF_OK, message ).Execute( );

          return;
     }

}








// ----------------------------------------------------------------------------
//              TextResultsDialog::PreviousSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::PreviousSpeciesHandler( PushButton* )
{
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
//              TextResultsDialog::PreviousComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::PreviousComptsHandler( PushButton* )
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
//              TextResultsDialog::SelectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::SelectAllSpeciesHandler( PushButton* )
{
     UINT16 num_entries = species_listbox.GetEntryCount();

     while (num_entries--)
          species_listbox.SelectEntryPos( num_entries );

     SpeciesListboxSelectHandler( &species_listbox );
}



// ----------------------------------------------------------------------------
//              TextResultsDialog::SelectAllComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::SelectAllComptsHandler( PushButton* )
{
     UINT16 num_entries = compartment_listbox.GetEntryCount();

     while (num_entries--)
          compartment_listbox.SelectEntryPos( num_entries );

     CompartmentListboxHandler( &compartment_listbox );
}



// ----------------------------------------------------------------------------
//              TextResultsDialog::DeselectAllSpeciesHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::DeselectAllSpeciesHandler( PushButton* )
{
     species_listbox.SetNoSelection( );
     SpeciesListboxSelectHandler( &species_listbox );
}



// ----------------------------------------------------------------------------
//              TextResultsDialog::DeselectAllComptsHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::DeselectAllComptsHandler( PushButton* )
{
     compartment_listbox.SetNoSelection( );
     CompartmentListboxHandler( &compartment_listbox );
}





// ----------------------------------------------------------------------------
//                TextResultsDialog::SpeciesListboxSelectHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::SpeciesListboxSelectHandler( MultiListBox* )
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

     ChangeShowButtonActivation( );
}






// ----------------------------------------------------------------------------
//                TextResultsDialog::ConcBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::ConcBtnHandler( CheckBox *pCheckBox )
{
     BOOL8 test = ( conc_cb.IsChecked( ) ||
                    amount_cb.IsChecked( ) 
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
//           TextResultsDialog::GetDataType( const CheckBox* pCheckBox )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

DATA_TYPE TextResultsDialog::GetDataType( const CheckBox* pCheckBox ) const
{
     if ( pCheckBox  == &conc_cb )
          return CONC_DATA;

     if ( pCheckBox  == &amount_cb )
          return AMOUNT_DATA;

     if ( pCheckBox  == &temp_cb )
          return TEMP_DATA;

     if ( pCheckBox  == &volume_cb )
          return VOLUME_DATA;

     if ( pCheckBox  == &press_cb )
          return PRESS_DATA;

     // abort if we get here 

     ASSERT( 0 );
}









// ----------------------------------------------------------------------------
//                TextResultsDialog::CheckBoxHandler( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::CheckBoxHandler( CheckBox * pCheckBox )
{
     // figure out plot type and set flag in current_plot_type

     if ( pCheckBox->IsChecked() )
          SetDataTypeFlag( GetDataType( pCheckBox ) );
     else
          ClearDataTypeFlag( GetDataType( pCheckBox ) );

     ChangeShowButtonActivation();
}






// ----------------------------------------------------------------------------
//              TextResultsDialog::InitializeSpeciesSelection()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::InitializeSpeciesSelection( BOOL8 State )
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
//                TextResultsDialog::CompartmentListboxHandler( )
// ............................................................................
//
//  called whenever a new Compartment is selected in compartment listbox
//
// ----------------------------------------------------------------------------

void TextResultsDialog::CompartmentListboxHandler( MultiListBox* )
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

     ChangeShowButtonActivation();
}









// ----------------------------------------------------------------------------
//                TextResultsDialog::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextResultsDialog::InitializeDialog( )
{
     // initialize the "select compartment" listbox

     UINT16 num_compts  = p_scheme_window->GetScheme().GetNumCompartments( );


     for ( UINT16 i = 0; i < num_compts; i++ )
          compartment_listbox.InsertEntry(
           p_scheme_window->GetScheme().GetCompartmentObjectPtrFromIndex( i )->GetName()
          );

     // initialize radiobutton selection

     // set some text

      num_pts_avail_text.SetText( String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );

     // set to minimum of available points in simulation of MAX_PTS_DEFAULT

      max_pts_to_plot_ef.SetText(
       String( p_scheme_window->GetScheme().GetNumSimulationDataPoints() > MAX_PTS_DEFAULT ?
       MAX_PTS_DEFAULT : p_scheme_window->GetScheme().GetNumSimulationDataPoints() ) );
}







// ----------------------------------------------------------------------------
//                    ShowTextResultsDialog( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowTextResultsDialog( SchemeFrameWindow* pSchemeFrameWin )
{
     TextResultsDialog* pdlg = new TextResultsDialog( pSchemeFrameWin );

     if ( pdlg )
     {
          pdlg->Execute();
          delete pdlg;
     }
     else
          GetpMainApp()->OutOfMemory();
}



