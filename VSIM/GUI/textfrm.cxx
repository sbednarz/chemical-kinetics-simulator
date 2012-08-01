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
//  TEXTFRAM.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines a ScrollingMDIWindow specialized for presenting
//  simulation results in text format
//
//  CREATED : 8.20.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "textfrm.hxx"

#endif

#include "mainmenu.hrc"
#include "icons.hrc"
#include "strings.hrc"




const Fraction INITIAL_SCALE( ScalableWindow::MINIMUM_SCALE );
const INT16    MIN_VISIBLE_PORTION   = 3;


// ----------------------------------------------------------------------------
//                         TextFrameWindow::TextFrameWindow()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

TextFrameWindow::TextFrameWindow( SchemeWindow& rSchemeWin, LineList* pLineList ) :
ScrollingMDIWindow( GetpApp()->GetAppWindow(), NULL, ResId( vID_TOOLBOX_TEXT ) ),
pline_list( pLineList ),
rschem_win( rSchemeWin ),
scrolling_on( FALSE ),
text_window( this, pLineList )
{
     SetpDrawingArea( &text_window );

     GetToolBar().ChangeHighlightHdl(  LINK( this, TextFrameWindow, ToolBarHighlight ) );
     GetToolBar().ChangeSelectHdl( LINK( this, TextFrameWindow, ToolBarSelect ) );

     SetText( GetReactionScheme().GetNickName() + String( ResId( vID_STR_TEXTWIN_TITLE ) ) );
     ChangeIcon( Icon(  ResId( vID_ICO_TEXTWIN ) ) );

     GetSchemeWindow().AddResultsWindow( this );

     ActivateZoomTools( TRUE );

     GetpMainApp()->AddToWindowList( this );

     GetVerticalScrollBar().ChangeScrollHdl( LINK(this, TextFrameWindow, ScrollHandler) );
     GetHorizontalScrollBar().ChangeScrollHdl( LINK(this, TextFrameWindow, ScrollHandler) );

     GetVerticalScrollBar().ChangeRange( Range( 0, MAX_RANGE ) );
     GetHorizontalScrollBar().ChangeRange( Range( 0, MAX_RANGE ) );
}










// ----------------------------------------------------------------------------
//                         TextFrameWindow::~TextFrameWindow()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

TextFrameWindow::~TextFrameWindow(  ) 
{
     delete pline_list;
     GetpMainApp()->RemoveFromWindowList( this );
}







// ----------------------------------------------------------------------------
//                   TextFrameWindow::ScrollHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextFrameWindow::ScrollHandler( ScrollBar* pScroll )
{

//     if ( scrolling_on )
//          return;
 
     scrolling_on = TRUE;
 
     if ( pScroll == &GetHorizontalScrollBar() )
     {
           switch ( GetHorizontalScrollBar().GetType() )
           {
           case SCROLL_PAGEDOWN:
      
              text_window.PageRight();
              break;
      
      
           case SCROLL_LINEDOWN:
      
              text_window.CharRight();
              break;
      
      
           case SCROLL_LINEUP:
      
              text_window.CharLeft();
              break;
      
      
           case SCROLL_PAGEUP:
      
              text_window.PageLeft();
              break;
      
      
           case SCROLL_DRAG:
      
              text_window.HorizontalDrag( GetHorizontalScrollBar().GetDelta() );
              break;
      
           default : 
      
                ASSERT( 0 );
                break;
           }
     }
     else
     {
           switch ( GetVerticalScrollBar().GetType() )
           {
           case SCROLL_PAGEUP:
      
              text_window.PageUp();
              break;
      
      
           case SCROLL_LINEUP:
      
              text_window.LineUp();
              break;
      
      
           case SCROLL_LINEDOWN:
      
              text_window.LineDown();
              break;
      
      
           case SCROLL_PAGEDOWN:
      
              text_window.PageDown();
              break;
      
      
           case SCROLL_DRAG:
      
              text_window.VerticalDrag( GetVerticalScrollBar().GetDelta());
              break;
      
           default : 
      
                ASSERT( 0 );
                break;
           }
     }

     SetScrollbarRangeAndThumbPosition();

//     scrolling_on = FALSE;
}


     
     
     





// ----------------------------------------------------------------------------
//                   TextFrameWindow::SetScrollbarPageAndLineSizes()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextFrameWindow::SetScrollbarPageAndLineSizes()
{
     ScrollBar& v_scroll = GetVerticalScrollBar();
     ScrollBar& h_scroll = GetHorizontalScrollBar();
     
     v_scroll.ChangePageSize( 0 );
     v_scroll.ChangeLineSize( 0 );

     h_scroll.ChangePageSize( 0 );
     h_scroll.ChangeLineSize( 0 );

}






// ----------------------------------------------------------------------------
//                   TextFrameWindow::SetScrollbarActivation()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextFrameWindow::SetScrollbarActivation( )
{
     if ( pline_list->GetNumLines() > text_window.GetVerticalPageSize() )
          GetVerticalScrollBar().Enable();
     else
          GetVerticalScrollBar().Disable();

     if ( pline_list->GetMaxLineLength() > text_window.GetHorizontalPageSize() )
          GetHorizontalScrollBar().Enable();
     else
          GetHorizontalScrollBar().Disable();
}





// ----------------------------------------------------------------------------
//              TextFrameWindow::SetScrollbarRangeAndThumbPosition()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void TextFrameWindow::SetScrollbarRangeAndThumbPosition()
{
     // first set vertical scroll bar 

     INT16 pos;
     
     if ( 0 == text_window.GetCurrentTopLineNum() ) 
     {
          pos = 0;
     }
     else
     {
          // is last line on screen ? 

          UINT32 last_possible_line_num = text_window.GetCurrentTopLineNum() + text_window.GetVerticalWindowSizeLines();
     
          if ( last_possible_line_num >= (pline_list->GetNumLines()-1) )
          {
              pos = MAX_RANGE;
          }
          else
          {
               FLOAT64 fposn =  (MAX_RANGE * text_window.GetCurrentTopLineNum()) /
                      (pline_list->GetNumLines() -
                      (UINT32) text_window.GetVerticalWindowSizeLines());

               // move thumb away from zero position if we are not on first line

               pos = Round( fposn );

               if ( ( pos < MIN_VISIBLE_PORTION ) && (0 != text_window.GetCurrentTopLineNum() ) )
                    pos = MIN_VISIBLE_PORTION;

               if ( pos == MAX_RANGE )
                    pos = MAX_RANGE - MIN_VISIBLE_PORTION;
          }
     }

     GetVerticalScrollBar().ChangeThumbPos( pos );

     // now do horizontal scroll bar 

     if ( 0 == text_window.GetCurrentLeftCharNum() )
     {
          pos = 0;
     }
     else
     {
          // is rightmost character ?

          UINT16 rightmost_possible_char_num = text_window.GetCurrentLeftCharNum() + text_window.GetHorizontalWindowSizeChars();

          if ( rightmost_possible_char_num >= (pline_list->GetMaxLineLength()+EXTRA_CHARS_LENGTH) )
          {
              pos = MAX_RANGE;
          }
          else
          {
               FLOAT64 fposn = (MAX_RANGE * text_window.GetCurrentLeftCharNum() ) /
               (pline_list->GetMaxLineLength() + 
               (UINT32) EXTRA_CHARS_LENGTH -
               (UINT32) text_window.GetHorizontalWindowSizeChars()
               );
          
               // move thumb away from zero position if we are not on first char 

               pos = Round( fposn );

               if ( ( pos < MIN_VISIBLE_PORTION) && (0 != text_window.GetCurrentLeftCharNum() ) )
                    pos = MIN_VISIBLE_PORTION;

               if ( pos == MAX_RANGE )
                    pos = MAX_RANGE - MIN_VISIBLE_PORTION;
          }

     }

     GetHorizontalScrollBar().ChangeThumbPos( pos );
}






// ----------------------------------------------------------------------------
//                  TextFrameWindow::ChangeScale( )
// ............................................................................
//
//  called when the user changes the scale of the plot window using toolbar
//  buttons
//
// ----------------------------------------------------------------------------

Fraction TextFrameWindow::ChangeDrawingScale( const Fraction& rNewScale )
{

    GetStatusBar().SetText(  String( ResId( vID_STR_TABLE_SCALE_HEAD ) ) +
                             String(  Round( 100.0 * (double) rNewScale ) ) + 
                             String( ResId( vID_STR_TABLE_SCALE_TAIL ) )
                          );

   return ScrollingMDIWindow::ChangeDrawingScale( rNewScale );
}





// ----------------------------------------------------------------------------
//                         TextFrameWindow::Activate( )
// ............................................................................
//
//  called upon activation of MDI Window. We activate/deactivate ToolButtons
//  as necessary
//
// ----------------------------------------------------------------------------

void TextFrameWindow::Activate( )
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
//                         TextFrameWindow::Deactivate( )
// ............................................................................
//
//  called upon deactivation of MDI Window
//
// ----------------------------------------------------------------------------

void TextFrameWindow::Deactivate()
{
}








// ----------------------------------------------------------------------------
//                         TextFrameWindow::ToolBarSelect( )
// ............................................................................
//
//  handler for ToolBar select events
//
// ----------------------------------------------------------------------------

void TextFrameWindow::ToolBarSelect( ToolBox* pBox )
{
     switch ( pBox->GetCurItemId() )
     {
          case vID_TI_TEXT_HELP :

            GetpMainApp()->GetHelp()->Start( HLP_TEXTWIN );
               break;



          case vID_TI_TEXT_PRINT :

               text_window.ShowPrintDlg();
               break;


          case vID_TI_TEXT_SAVE:

               SaveToFile();
               break;
     }
}








// ----------------------------------------------------------------------------
//                      TextFrameWindow::SaveToFile()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  TextFrameWindow::SaveToFile()
{
     String filename;

     if (SaveAsFileDialog( filename, TEXT_FILETYPE) )
     {
          TextOutputStream txtstream( filename );

          if (txtstream.IsOpen() )
          {
               txtstream << *pline_list;

               if ( txtstream.FileError() )
                    GetpMainApp()->FileWriteError( filename );

          }
          else
          {
               GetpMainApp()->FileOpenError( filename );
          }
     }
}





// ----------------------------------------------------------------------------
//                         TextFrameWindow::Close( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL TextFrameWindow::Close()
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
//                         TextFrameWindow::OKToClose( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 TextFrameWindow::OKToClose()
{
     QueryBox q_box( this, WB_YES_NO | WB_DEF_YES | WB_SVLOOK,
      String( ResId( vID_STR_CLOSE_PLOT_MSG1) ) +
      GetText() +
      String( ResId( vID_STR_CLOSE_PLOT_MSG2) )
      );

     return ( RET_YES == q_box.Execute() );
}




// ----------------------------------------------------------------------------
//                         TextFrameWindow::ToolBarHighlight( )
// ............................................................................
//
//  Set help text appropriately
//
// ----------------------------------------------------------------------------

void TextFrameWindow::ToolBarHighlight( ToolBox* pBox )
{
     String text = pBox->GetHelpText( pBox->GetCurItemId( ) );

     if ( ! text )
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( GetpMainApp()->GetMainWindow()->GetHelpText( ) );
     else
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( text );

    GetpMainApp()->SetWindowUnderPointer( NULL );
}


