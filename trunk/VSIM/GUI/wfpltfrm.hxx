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
//  WFPLTFRM.HXX - a header file for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a WaterfallPlotFrameWindow specialized for presenting XY 
//  'waterfall' plots 
//
//  CREATED : 6.10.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__WFPLTFRM_HXX__)
     #define __WFPLTFRM_HXX__

#include "wfplotwn.hxx"
#include "scrmdiwn.hxx"
#include "mainapp.hxx"


class WaterfallPlotFrameWindow : public ScrollingMDIWindow
{
protected:
     WaterfallPlotWindow    plot_window;
     SchemeWindow&          rschem_win; 

     virtual void           Resize(){  RecalcWindowLayout( ); }

     virtual void           Activate( );
     virtual void           Deactivate();

     BOOL8                  OKToClose();

     void                   ActivateZoomTools( BOOL8 State  )
                            { GetpMainApp()->GetMainWindow()->ActivateZoomTools( State );}

  
public:
                            WaterfallPlotFrameWindow( SchemeWindow& rSchemeWin );
                           ~WaterfallPlotFrameWindow( );

     virtual MDI_WIN_TYPE   WindowType( ) const { return MDI_WATERFALLPLOTWINDOW;}

     void                   ToolBarSelect( ToolBox* pBox );
     void                   ToolBarHighlight( ToolBox* pBox );

     virtual BOOL           Close();
     virtual Fraction       ChangeDrawingScale( const Fraction& rNewScale );
     virtual const String&  GetName( ) const { return rschem_win.GetScheme().GetName(); }

     WaterfallPlotWindow*   GetpPlotWindow( ) { return &plot_window; }

     SchemeWindow&          GetSchemeWindow() const  { return rschem_win; }
     ReactionScheme&        GetReactionScheme() const  { return rschem_win.GetScheme(); }


};



#endif
