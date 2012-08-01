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
//  PLOTATTR.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the EditPlotAttributes dialog used in the plot routines
//  the VSIM user interface.
//
//  CREATED : 8.23.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "plotattr.hxx"
#include "mainapp.hxx"

#endif





#include "plotattr.hrc"
#include "strings.hrc"


class PlotAttributeDialog : public ModalDialog
{
protected:
    CheckBox      vary_linecolor_cb;
    CheckBox      vary_linestyle_cb;
    CheckBox      vary_markerstyle_cb;
    GroupBox      group_box1;
    CheckBox      show_lines_cb;
    CheckBox      show_markers_cb;
    CheckBox      show_rect_cb;
    GroupBox      group_box2;

    FixedText     fixed_text1;
    FixedText     fixed_text2;
    Edit          max_pts_ef;
    FixedText     num_pts_avail_text;
    GroupBox      group_box3;

    OKButton      close_btn;
    PushButton    undo_btn;
    HelpButton    help_btn;

    BOOL8         modified;
    XyPlot*       p_plot;

    void          SetModifiedFlag();
    void          InitializeDialog();

public:
    PlotAttributeDialog( XyPlot * pPlot ) ;

    void          CheckBoxHandler( CheckBox* );
    void          TextModifiedHandler( Edit* );
    void          UndoHandler( PushButton* );
    void          CloseHandler( OKButton* );

};








// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::PlotAttributeDialog()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

PlotAttributeDialog::PlotAttributeDialog( XyPlot * pPlot ) :
ModalDialog( (Window*) pPlot->GetpParent(), ResId( vID_DLG_PLOT_ATTR ) ),
vary_linecolor_cb( this, ResId( vID_CB_VARY_LINECOLOR ) ),
vary_linestyle_cb( this, ResId( vID_CB_VARY_LINESTYLE ) ),
vary_markerstyle_cb( this, ResId( vID_CB_VARY_MARKERSTYLE ) ),
group_box1( this, ResId( 1 ) ),
show_lines_cb( this, ResId( vID_CB_SHOW_LINES ) ),
show_markers_cb( this, ResId( vID_CB_SHOW_MARKERS ) ),
show_rect_cb( this, ResId( vID_CB_SHOW_RECT ) ),
group_box2( this, ResId( 2 ) ),
fixed_text1( this, ResId( 1 ) ), 
fixed_text2( this, ResId( 2 ) ),
max_pts_ef( this, ResId( vID_EF_MAX_POINTS_TO_PLOT ) ), 
num_pts_avail_text( this, ResId( vID_STEXT_NUM_POINTS_AVAIL ) ), 
group_box3( this, ResId( 3 ) ),
close_btn( this, ResId( vID_BTN_CLOSE ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
modified( FALSE ),
p_plot( pPlot )
{
     FreeResource();

     InitializeDialog();

     vary_linecolor_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     vary_linestyle_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     vary_markerstyle_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     show_lines_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     show_markers_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     show_rect_cb.ChangeClickHdl( LINK( this, PlotAttributeDialog, CheckBoxHandler ) );
     max_pts_ef.ChangeModifyHdl( LINK( this, PlotAttributeDialog, TextModifiedHandler ) );

     close_btn.ChangeClickHdl( LINK( this, PlotAttributeDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, PlotAttributeDialog, UndoHandler ) );
     
     GetpMainApp()->CenterWindowOnParent( this, GetParent() );

}







// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::TextModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void PlotAttributeDialog::TextModifiedHandler( Edit* )
{
     modified = TRUE;
}



// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void PlotAttributeDialog::InitializeDialog()
{
     vary_linecolor_cb.Check( p_plot->VaryColor() );
     vary_linestyle_cb.Check( p_plot->VaryLineStyle() );
     vary_markerstyle_cb.Check( p_plot->VaryMarkerStyle() );
     show_lines_cb.Check( p_plot->ShowLines() );
     show_markers_cb.Check( p_plot->ShowMarkers() );
     show_rect_cb.Check( p_plot->ShowBoundingRectangle() );

     num_pts_avail_text.SetText( String( p_plot->GetNumberOfDataPairs() ) );

     // set to minimum of available points in simulation of MAX_PTS_DEFAULT

     max_pts_ef.SetText(
       String( p_plot->GetMaxNumPlotPts() > MAX_PTS_DEFAULT ?
       MAX_PTS_DEFAULT : p_plot->GetMaxNumPlotPts()  ) );

}




// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::CloseHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void PlotAttributeDialog::CloseHandler( OKButton* )
{
     if ( modified )
     {
          // read the user entries

          // first check user input in specifying number of points to plot 
     
          NumericString numstr( max_pts_ef.GetText() );

          UINT32        value;
     
          if ( numstr.IsValidUINT32( &value ) && 
               ( value >= MIN_PTS ) &&
               ( value <= MAX_PTS )
             )
          {
               p_plot->SetMaxNumPlotPts( value );
     
               p_plot->VaryColor( vary_linecolor_cb.IsChecked() );
               p_plot->VaryLineStyle( vary_linestyle_cb.IsChecked() );
               p_plot->VaryMarkerStyle( vary_markerstyle_cb.IsChecked() );
               p_plot->ShowLines( show_lines_cb.IsChecked() );
               p_plot->ShowMarkers( show_markers_cb.IsChecked() );
               p_plot->ShowBoundingRectangle( show_rect_cb.IsChecked() );
          
               EndDialog( TRUE );
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
     else
          EndDialog( FALSE );
}










// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::SetModifiedFlag()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void PlotAttributeDialog::SetModifiedFlag()
{
     modified = TRUE;
     undo_btn.Enable();
}




// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::CheckBoxHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  PlotAttributeDialog::CheckBoxHandler( CheckBox* )
{
     if ( ! modified )
          SetModifiedFlag();
}





// ----------------------------------------------------------------------------
//                 PlotAttributeDialog::UndoHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void PlotAttributeDialog::UndoHandler( PushButton* )
{
     modified = FALSE;
     InitializeDialog();
     undo_btn.Disable();
}




// ----------------------------------------------------------------------------
//                 EditPlotAttributes()
// ............................................................................
//
//
// ----------------------------------------------------------------------------


BOOL8 EditPlotAttributes( XyPlot* pPlot )
{

     PlotAttributeDialog* pdlg = new PlotAttributeDialog( pPlot );

     if ( pdlg )
     {
          BOOL8 rc = pdlg->Execute();
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}




