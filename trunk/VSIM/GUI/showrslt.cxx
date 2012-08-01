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
//  SHOWRSLT.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This header file declares and defines the SimResultDialog class, which 
//  allows user selection of plot types
//
//  CREATED : 6.22.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "showrslt.hxx"
#include "schemwin.hxx"
#include "mainapp.hxx"

#include "snglplot.hxx"
#include "mltcplot.hxx"
#include "textrslt.hxx"

#endif





class ViewResultsDialog : public ModalDialog
{
protected:
     OKButton      close_btn;
     HelpButton    help_btn;
     PushButton    create_plot_btn;
     GroupBox      groupbox1;
     RadioButton   single_plot_rb;
     FixedBitmap   bitmap1;
     RadioButton   mult_compt_plot_rb;
     FixedBitmap   bitmap2;
     RadioButton   bar_graph_rb;
     FixedBitmap   bitmap3;
     RadioButton   waterfall_plot_rb;
     FixedBitmap   bitmap4;
     RadioButton   text_table_rb;
     FixedBitmap   bitmap5;

     SchemeFrameWindow* p_parent;

     SchemeFrameWindow* GetpParent( ) const { return p_parent; }

public:
     ViewResultsDialog( SchemeFrameWindow * pParent );

     void RadioBtnHandler( RadioButton* );
     void PlotBtnHandler( PushButton* );
};




// ----------------------------------------------------------------------------
//                   SimResultsDialog::SimResultsDialog( )
// ............................................................................
// 
//  constructor 
//
// ----------------------------------------------------------------------------

ViewResultsDialog::ViewResultsDialog( SchemeFrameWindow * pParent ) :
ModalDialog( pParent, ResId( vID_DLG_SIM_RESULTS )  ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
create_plot_btn( this, ResId( vID_BTN_CREATE_PLOT ) ),

groupbox1( this, ResId( 1 ) ),

single_plot_rb( this, ResId( vID_RB_SINGLE_PLOT ) ),
bitmap1( this, ResId( 1 ) ),

mult_compt_plot_rb( this, ResId( vID_RB_MULTCOMPT_PLOT ) ),
bitmap2( this, ResId( 2 ) ),

bar_graph_rb( this, ResId( vID_RB_BAR_GRAPH ) ),
bitmap3( this, ResId( 3 ) ),

waterfall_plot_rb( this, ResId( vID_RB_WATERFALL_PLOT ) ),
 bitmap4( this, ResId( 4 ) ),

text_table_rb( this, ResId( vID_RB_TEXT_TABLE ) ),
bitmap5( this, ResId( 5 ) ),
p_parent( pParent )
{
     FreeResource();

     single_plot_rb.ChangeClickHdl( LINK( this, ViewResultsDialog, RadioBtnHandler ) );  
     mult_compt_plot_rb.ChangeClickHdl( LINK( this, ViewResultsDialog, RadioBtnHandler ) );  
     bar_graph_rb.ChangeClickHdl( LINK( this, ViewResultsDialog, RadioBtnHandler ) );  
     waterfall_plot_rb.ChangeClickHdl( LINK( this, ViewResultsDialog, RadioBtnHandler ) );  
     text_table_rb.ChangeClickHdl( LINK( this, ViewResultsDialog, RadioBtnHandler ) );  

     create_plot_btn.ChangeClickHdl( LINK( this, ViewResultsDialog, PlotBtnHandler ) );   

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );

}






// ----------------------------------------------------------------------------
//                   ViewResultsDialog::PlotBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ViewResultsDialog::PlotBtnHandler( PushButton*  )
{
     if ( single_plot_rb.IsChecked( ) )
          ShowOneComptPlotDialog( GetpParent() );

     if ( mult_compt_plot_rb.IsChecked( ) )
          ShowMultComptPlotDialog( GetpParent() ); 

     if ( bar_graph_rb.IsChecked( ) )
          UnderConstruction();

     if ( waterfall_plot_rb.IsChecked( ) )
          ShowWaterfallPlotDialog( GetpParent() ); 

     if ( text_table_rb.IsChecked( ) )
          ShowTextResultsDialog( GetpParent() );

    EndDialog();
}





// ----------------------------------------------------------------------------
//                   ViewResultsDialog::RadioBtnHandler( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ViewResultsDialog::RadioBtnHandler( RadioButton* pBtn )
{
     single_plot_rb.Check( pBtn == &single_plot_rb);
     mult_compt_plot_rb.Check( pBtn == &mult_compt_plot_rb ); 
     bar_graph_rb.Check( pBtn == &bar_graph_rb );       
     waterfall_plot_rb.Check( pBtn == &waterfall_plot_rb );  
     text_table_rb.Check( pBtn == &text_table_rb );      
}



// ----------------------------------------------------------------------------
//                     CreateSimResultDlg( )
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

void ShowViewResultDlg( SchemeFrameWindow* pParent )
{
     ViewResultsDialog* pdlg = new ViewResultsDialog( pParent );

     if ( pdlg )
          pdlg->Execute();
     else
          GetpMainApp()->OutOfMemory();
}


