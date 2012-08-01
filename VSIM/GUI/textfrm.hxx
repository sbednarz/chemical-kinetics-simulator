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
//  TEXTFRAM.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This module declares a ScrollingMDIWindow specialized for presenting 
//  simulation results in text format
//
//  CREATED : 8.20.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__TEXTFRAM_HXX__)
     #define __TEXTFRAM_HXX__

#include "scrmdiwn.hxx"
#include "mainapp.hxx"
#include "linelist.hxx"
#include "textwin.hxx"



class TextFrameWindow : public ScrollingMDIWindow
{
protected:
     const LineList*        pline_list;
     SchemeWindow&          rschem_win; 
     BOOL8                  scrolling_on;
     TextWindow             text_window;

     virtual void           Resize(){  RecalcWindowLayout( ); SetScrollbarPageAndLineSizes(); }

     virtual void           Activate( );
     virtual void           Deactivate();

     BOOL8                  OKToClose();

     void                   SaveToFile();

     void                   ActivateZoomTools( BOOL8 State  )
                            { GetpMainApp()->GetMainWindow()->ActivateZoomTools( State );}

     virtual void           SetScrollbarPageAndLineSizes();
     virtual void           SetScrollbarActivation( );

     virtual void           SetScrollbarRangeAndThumbPosition();
  
public:
                            TextFrameWindow( SchemeWindow& rSchemeWin, LineList* pLineList );
                           ~TextFrameWindow();

     virtual MDI_WIN_TYPE   WindowType( ) const { return MDI_TEXTWINDOW;}

     void                   ToolBarSelect( ToolBox* pBox );
     void                   ToolBarHighlight( ToolBox* pBox );

     virtual BOOL           Close();
     virtual Fraction       ChangeDrawingScale( const Fraction& rNewScale );

     virtual const String&  GetName( ) const { return rschem_win.GetScheme().GetName(); }

     SchemeWindow&          GetSchemeWindow() const  { return rschem_win; }
     ReactionScheme&        GetReactionScheme() const  { return rschem_win.GetScheme(); }

     void                   RecalcScrollbarSettings(){ SetScrollbarPageAndLineSizes(); }

     void                   ScrollHandler( ScrollBar* pScroll );
};



#endif

