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
//  XYPLTFRM.HXX - a header file for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a ScrollingMDIWindow specialized for presenting XY plots 
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__XTPLTFRM_HXX__)
     #define __XYPLTFRM_HXX__

#include "xyplotwn.hxx"
#include "scrmdiwn.hxx"
#include "mainapp.hxx"


class XyPlotFrameWindow : public ScrollingMDIWindow
{
protected:
     XyPlotWindow           plot_window;
     SchemeWindow&          rschem_win; 

     virtual void           Resize(){  RecalcWindowLayout( ); }

     virtual void           Activate( );
     virtual void           Deactivate();

     BOOL8                  OKToClose();

     void                   ActivateZoomTools( BOOL8 State  )
                            { GetpMainApp()->GetMainWindow()->ActivateZoomTools( State );}

  
public:
                            XyPlotFrameWindow( SchemeWindow& rSchemeWin );
                           ~XyPlotFrameWindow();

     virtual MDI_WIN_TYPE   WindowType( ) const { return MDI_XYPLOTWINDOW;}

     void                   ToolBarSelect( ToolBox* pBox );
     void                   ToolBarHighlight( ToolBox* pBox );

     virtual BOOL           Close();
     virtual Fraction       ChangeDrawingScale( const Fraction& rNewScale );

     virtual const String&  GetName( ) const { return rschem_win.GetScheme().GetName(); }

     XyPlotWindow*          GetpPlotWindow( ) { return &plot_window; }

     SchemeWindow&          GetSchemeWindow() const  { return rschem_win; }
     ReactionScheme&        GetReactionScheme() const  { return rschem_win.GetScheme(); }


};



#endif
