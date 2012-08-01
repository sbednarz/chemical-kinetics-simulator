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
//  XYPLTFRM.CXX - a module for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a Window specialized for presenting XY plots 
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "xypltfrm.hxx"

#endif

#include "mainmenu.hrc"
#include "icons.hrc"
#include "strings.hrc"



const Fraction INITIAL_SCALE( ScalableWindow::MINIMUM_SCALE );


// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::XyPlotFrameWindow()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

XyPlotFrameWindow::XyPlotFrameWindow( SchemeWindow& rSchemeWin ) :
ScrollingMDIWindow( GetpApp()->GetAppWindow(), NULL, ResId( vID_TOOLBOX_XYPLOT ) ),
plot_window( this, rSchemeWin.GetScheme() ),
rschem_win( rSchemeWin )
{
     SetpDrawingArea( &plot_window );

     GetToolBar().ChangeHighlightHdl(  LINK( this, XyPlotFrameWindow, ToolBarHighlight ) );
     GetToolBar().ChangeSelectHdl( LINK( this, XyPlotFrameWindow, ToolBarSelect ) );

     SetText( GetReactionScheme().GetNickName() + String( ResId( vID_STR_XYPLOTWIN_TITLE ) ) );
     ChangeIcon( Icon(  ResId( vID_ICO_XYPLOTWIN ) ) );

     GetSchemeWindow().AddResultsWindow( this );

     ChangeDrawingScale( INITIAL_SCALE );
     SetDrawingAreaSizeLogic( GetpPlotWindow()->GetTotalSizeLogic() );
     ActivateZoomTools( TRUE );

     GetpMainApp()->AddToWindowList( this );
}







// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::~XyPlotFrameWindow()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

XyPlotFrameWindow::~XyPlotFrameWindow(  ) 
{
     GetpMainApp()->RemoveFromWindowList( this );
}







// ----------------------------------------------------------------------------
//                  XyPlotFrameWindow::ChangeScale( )
// ............................................................................
//
//  called when the user changes the scale of the plot window using toolbar
//  buttons
//
// ----------------------------------------------------------------------------

Fraction XyPlotFrameWindow::ChangeDrawingScale( const Fraction& rNewScale )
{

    GetStatusBar().SetText(  String( ResId( vID_STR_PLOT_SCALE_HEAD ) ) +
                             String(  Round( 100.0 * (double) rNewScale ) ) + 
                             String( ResId( vID_STR_PLOT_SCALE_TAIL ) )
                          );

   return ScrollingMDIWindow::ChangeDrawingScale( rNewScale );
}





// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::Activate( )
// ............................................................................
//
//  called upon activation of MDI Window. We activate/deactivate ToolButtons
//  as necessary
//
// ----------------------------------------------------------------------------

void XyPlotFrameWindow::Activate( )
{
    MainWindow* pmw = GetpMainApp()->GetMainWindow();

    // deactivate the edit tools
    
    pmw->ActivateEditTools( FALSE );
    pmw->ActivatePaste( FALSE );
    pmw->ActivateAddTransferObject( FALSE );
    pmw->ActivateAddCompartment( FALSE  );
    ActivateZoomTools( TRUE );

    UpdateWindow();
}




// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::Deactivate( )
// ............................................................................
//
//  called upon deactivation of MDI Window
//
// ----------------------------------------------------------------------------

void XyPlotFrameWindow::Deactivate()
{
}








// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::ToolBarSelect( )
// ............................................................................
//
//  handler for ToolBar select events
//
// ----------------------------------------------------------------------------

void XyPlotFrameWindow::ToolBarSelect( ToolBox* pBox )
{
     switch ( pBox->GetCurItemId() )
     {
          case vID_TI_XYPLOT_HELP :

            GetpMainApp()->GetHelp()->Start( HLP_XYPLOTWIN );
               break;


          case vID_TI_XYPLOT_ATTRIBUTES :

               GetpPlotWindow()->EditAttributes( );
               break;




          case vID_TI_XYPLOT_PRINT :

               GetpPlotWindow()->ShowPrintDlg();
               break;

          case vID_TI_XYPLOT_SAVE:

               UnderConstruction();
               break;
     }
}


// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::Close( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL XyPlotFrameWindow::Close()
{
     if ( OKToClose() )
     {
          GetSchemeWindow().RemoveResultsWindow( this );
          delete this;
          return TRUE;
     }
     else
          return FALSE;
}




// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::OKToClose( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 XyPlotFrameWindow::OKToClose()
{
     QueryBox q_box( this, WB_YES_NO | WB_DEF_YES | WB_SVLOOK,
      String( ResId( vID_STR_CLOSE_PLOT_MSG1) ) +
      GetText() +
      String( ResId( vID_STR_CLOSE_PLOT_MSG2) )
      );

     return ( RET_YES == q_box.Execute() );
}




// ----------------------------------------------------------------------------
//                         XyPlotFrameWindow::ToolBarHighlight( )
// ............................................................................
//
//  Set help text appropriately
//
// ----------------------------------------------------------------------------

void XyPlotFrameWindow::ToolBarHighlight( ToolBox* pBox )
{
     String text = pBox->GetHelpText( pBox->GetCurItemId( ) );

     if ( ! text )
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( GetpMainApp()->GetMainWindow()->GetHelpText( ) );
     else
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( text );

    GetpMainApp()->SetWindowUnderPointer( NULL );
}


