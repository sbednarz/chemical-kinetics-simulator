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
//  XYPLOTWN.CXX - a module for use in the VSIM User interface
// ............................................................................
//
//  This module defines a Window specialized for presenting XY plots
//
//  CREATED : 8.15.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "xyplotwn.hxx"
#include "pltwnatt.hxx"

#endif

#include "strings.hrc"
#include "mainmenu.hrc"
#include "icons.hrc"


// paper width in points ( 72 pts /inch * 8 inches )     
// paper height in points ( 72 pts /inch * 10.5 inches )

const INT16  PAPER_HEIGHT_LOGIC   = 576;
const INT16  PAPER_WIDTH_LOGIC  =  756;

const Size VIRTUAL_WINDOW_SIZE( PAPER_WIDTH_LOGIC, PAPER_HEIGHT_LOGIC );




// ----------------------------------------------------------------------------
//                         XyPlotWindow::XyPlotWindow( )
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

XyPlotWindow::XyPlotWindow( Window* pParent, ReactionScheme& rScheme ) :
DisplayWindow( pParent ),
plot( this ),
ext_data(),
r_scheme( rScheme )
{
     Initialize();

}






// ----------------------------------------------------------------------------
//                         XyPlotWindow::Initialize( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void XyPlotWindow::Initialize()
{
     //get font and color info from configuration file

     RetrieveSavedPlotAttributes();
     
     // size things as needed
     
//     ChangeScale( INITIAL_SCALE );
     SetTotalSize( VIRTUAL_WINDOW_SIZE );

     // we always want plot to fill entire virtual area
     
     plot.SetActiveArea( Rectangle( Point(), VIRTUAL_WINDOW_SIZE ));
     plot.ResizeObject(  );
     

}






// ----------------------------------------------------------------------------
//                         XyPlotWindow::~XyPlotWindow( )
// ............................................................................
//
//   destructor
//
// ----------------------------------------------------------------------------

XyPlotWindow::~XyPlotWindow( )
{
}








// ----------------------------------------------------------------------------
//                         XyPlotWindow::PrintPlot( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void XyPlotWindow::PrintPlot( Printer* pPrinter )
{
        // set up for directing output to printer
        // preserve current state so that we can restore when done

        OutputDevice* old_parent = plot.ChangepParent( pPrinter );
        Rectangle     old_area   = plot.GetActiveArea( );

        Rectangle new_area = Rectangle( Point(), pPrinter->PixelToLogic( pPrinter->GetOutputSizePixel() ) );

        MapMode map_mode  = pPrinter->GetMapMode();

        // calc offset so that plot is centered after
        // we shrink to 80% of page size

        Point offset = pPrinter->PixelToLogic( pPrinter->GetPageOffsetPixel() );

        offset = offset + Point( new_area.GetWidth() /10, new_area.GetHeight() /10 );

        map_mode.ChangeOrigin( offset );

        const Fraction scaling_factor( 8, 10 );

        Fraction scale = map_mode.GetScaleX();
        scale *= scaling_factor;
        map_mode.ChangeScaleX( scale );

        scale = map_mode.GetScaleY();
        scale *= scaling_factor;
        map_mode.ChangeScaleY( scale );

        pPrinter->ChangeMapMode( map_mode );

        plot.SetActiveArea(  new_area );

        plot.ResizeObject();
        plot.DrawObject();

        // restore previous state

        plot.ChangepParent( old_parent );
        plot.SetActiveArea( old_area );
        plot.ResizeObject();
}









// ----------------------------------------------------------------------------
//                         XyPlotWindow::MouseButtonDown( )
// ............................................................................
//
//  handler - called whenever a MousButton is pressed
//
// ----------------------------------------------------------------------------

void XyPlotWindow::MouseButtonDown( const MouseEvent& rMEvt )
{
     if ( rMEvt.GetClicks() > 1 )
     {
          // edit plot, redraw if modified

          if ( plot.EditObject( PixelToLogic( rMEvt.GetPosPixel() ) ) )
          {
               Invalidate();
          }
     }
}











// ----------------------------------------------------------------------------
//                         XyPlotWindow::Resize( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void XyPlotWindow::Resize( )
{
     plot.ResizeObject(  );
    
     Invalidate();
}






// ----------------------------------------------------------------------------
//                         XyPlotWindow::EditAttributes()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void XyPlotWindow::EditAttributes( )
{
     EditWindowAttributes( this );
     Resize();
     Invalidate();
     
     SavePlotAttributes();
}




// ----------------------------------------------------------------------------
//                         XyPlotWindow::GetColorScheme( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------


enum  COLOR_SCHEME_TYPE XyPlotWindow::GetColorScheme() const
{
     return plot.GetColorScheme();
}








// ----------------------------------------------------------------------------
//                         XyPlotWindow::SetColorScheme()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  XyPlotWindow::SetColorScheme(enum COLOR_SCHEME_TYPE NewScheme )
{
     plot.SetColorScheme(NewScheme);
}



void  XyPlotWindow::Paint( const Rectangle& )
{

     plot.DrawObject();

}




