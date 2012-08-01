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

// for testing plotting classes 




// ----------------------------------------------------------------------------
//  testplot.CXX - a module for use in testing of the VSIM User interface 
// ............................................................................
// 
//  This module defines a simple application for testing the behavior 
//  fof the plotting objects 
//
//  CREATED : 7.13.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#pragma hdrstop

#include "assert.hxx"

#include "multplot.hxx"
#include "stakplot.hxx"




// ----------------------------------------------------------------------------
//                                class MainApp
// ............................................................................
// 
//  This class is derived from Application
//
// ----------------------------------------------------------------------------

class MainApp : public Application
{

public:
                    MainApp();
    virtual void    Main( int, char*[] );

};


// ----------------------------------------------------------------------------
//                             MainApp::MainApp()
// ............................................................................
// 
//  constructor for the MainApp object 
//
// ----------------------------------------------------------------------------

MainApp::MainApp()
{
}



// ----------------------------------------------------------------------------
//  class MainWindow                                   
// ............................................................................
// 
//  This class is derived from WorkWindow                       
//
// ----------------------------------------------------------------------------

class MainWindow : public WorkWindow
{
protected:
     MultiXyPlot    plot;

public:
            MainWindow( Window* pParent, WinBits aWinStyle );
              

     XyPlot*         AddPlot( ) { return plot.AddPlot(); }
     virtual void    Resize();
     virtual void    Paint( const Rectangle& rRect );

};


class StackWindow : public WorkWindow
{
protected:
     StackedXyPlot    plot;

public:
            StackWindow( Window* pParent, WinBits aWinStyle ) :
             WorkWindow( pParent, aWinStyle ),
             plot( this ) {}
              

     BOOL8           AddDataSet( const FloatArray& rX, const FloatArray& rY, const String& rStr ) { return plot.AddDataSet( rX, rY, rStr); }

     virtual void    Paint( const Rectangle& rRect ) { plot.DrawObject(); }

     virtual void    Resize( )
     {
          plot.SetActiveArea( Rectangle( Point( 0,0 ), GetOutputSize() ) );
          plot.ResizeObject(  );
     
          Invalidate();
     }
     

};



// ----------------------------------------------------------------------------
//                      MainWindow::Paint( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MainWindow::Paint( const Rectangle& )
{
     plot.DrawObject();
}




// ----------------------------------------------------------------------------
//                         MainWindow::MainWindow( )
// ............................................................................
//
//  constructor - creates  StatusBar also
// ----------------------------------------------------------------------------

MainWindow::MainWindow( Window* pParent, WinBits aWinStyle ):
WorkWindow( pParent, aWinStyle ),
plot( this )
{
}



// ----------------------------------------------------------------------------
//                             MainWindow::Resize( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MainWindow::Resize( )
{
     plot.SetActiveArea( Rectangle( Point( 0,0 ), GetOutputSize() ) );
     plot.ResizeObject(  );

     Invalidate();
}




// ----------------------------------------------------------------------------
//                         MainApp::Main(  )
// ............................................................................
// 
//  The main application function
//
// ----------------------------------------------------------------------------

void MainApp::Main( int, char*[] )
{
    EnableSVLook();

     // create and initialize the main window 

     FloatArray x1(20);
     FloatArray y1(20);
     FloatArray y2(20);
     FloatArray y3(20);
     FloatArray y4(20);
     FloatArray y5(20);
     FloatArray y6(20);
     FloatArray y7(20);
     FloatArray y8(20);
     FloatArray y9(20);
     FloatArray y10(20);
     FloatArray y11(20);
     FloatArray y12(20);
     FloatArray y13(20);
     FloatArray y14(20);
     FloatArray y15(20);

     for ( UINT16 i = 0 ; i < x1.GetArraySize(); i++ )
     {
          x1[i] = (FLOAT64) i; 
          y1[i] = x1[i] * x1[i];
          y2[i] = 2 * x1[i] * x1[i];
          y3[i] = 3 * x1[i] * x1[i];
          y4[i] = 4 * x1[i] * x1[i];
          y5[i] = 5 * x1[i] * x1[i];
          y6[i] = 6 * x1[i] * x1[i];
          y7[i] = 7 * x1[i] * x1[i];
          y8[i] = 8 * x1[i] * x1[i];
          y9[i] = 9 * x1[i] * x1[i];
          y10[i] = 10 * x1[i] * x1[i];
          y11[i] = 11 * x1[i] * x1[i];
          y12[i] = 12 * x1[i] * x1[i];
          y13[i] = 13 * x1[i] * x1[i];
          y14[i] = 14 * x1[i] * x1[i];
          y15[i] = 15 * x1[i] * x1[i];
     }



    MainWindow main_window( NULL, WB_APP | WB_STDWORK | WB_SVLOOK );


  main_window.ChangeFont( main_window.GetFontMetric( Font( FAMILY_SWISS, Size( 0,10 ) ) ).GetPhysicalFont() );

    XyPlot* ptr = main_window.AddPlot();

    ptr->SetXAxisTitle( String("Time") );
    ptr->SetYAxisTitle( String("Conc") );

    ptr->SetXAxisUnits( String("sec") );
    ptr->SetYAxisUnits( String("mol/l") );

    ptr->SetPlotTitle( String( "This is a plot of Conc vs. time" ) );

    ptr->AddDataSet( x1, y1, String( "a") );
    ptr->AddDataSet( x1, y2, String( "b") );
    ptr->AddDataSet( x1, y3, String( "c") );
    ptr->AddDataSet( x1, y4, String( "d") );
    ptr->AddDataSet( x1, y5, String( "e") );
    ptr->AddDataSet( x1, y6, String( "f") );
    ptr->AddDataSet( x1, y7, String( "g") );
    ptr->AddDataSet( x1, y8, String( "h") );
    ptr->AddDataSet( x1, y9, String( "i") );
    ptr->AddDataSet( x1, y10, String( "j"));
    ptr->AddDataSet( x1, y11, String( "k"));
    ptr->AddDataSet( x1, y12, String( "l"));
    ptr->AddDataSet( x1, y13, String( "m"));
    ptr->AddDataSet( x1, y14, String( "n"));
    ptr->AddDataSet( x1, y15, String( "o"));

    ptr = main_window.AddPlot();

    ptr->SetXAxisTitle( String("Time") );
    ptr->SetYAxisTitle( String("Conc") );

    ptr->SetXAxisUnits( String("sec") );
    ptr->SetYAxisUnits( String("mol/l") );

    ptr->SetPlotTitle( String( "This is a 2nd plot of Conc vs. time" ) );

    ptr->ShowMarkers( FALSE );
    ptr->AddDataSet( x1, y1, String( "p") );

    ptr = main_window.AddPlot();

    ptr->SetYAxisTitle( String("Time") );
    ptr->SetXAxisTitle( String("Conc") );

    ptr->SetYAxisUnits( String("sec") );
    ptr->SetXAxisUnits( String("mol/l") );

    ptr->SetPlotTitle( String( "This is a 3nd plot of Conc vs. time" ) );

    ptr->ShowMarkers( FALSE );
    ptr->AddDataSet( y1, x1, String( "q") );

    ptr = main_window.AddPlot();

    ptr->SetYAxisTitle( String("Time") );
    ptr->SetXAxisTitle( String("Conc") );

    ptr->SetYAxisUnits( String("sec") );
    ptr->SetXAxisUnits( String("mol/l") );

    ptr->SetPlotTitle( String( "This is a 4th plot of Conc vs. time" ) );

    ptr->ShowMarkers( TRUE );
    ptr->VaryMarkerStyle( FALSE );
    ptr->VaryColor( FALSE );

    ptr->AddDataSet( y1, x1, String( "r") );


    ptr = main_window.AddPlot();

    ptr->SetXAxisTitle( String("Time") );
    ptr->SetYAxisTitle( String("Conc") );

    ptr->SetXAxisUnits( String("sec") );
    ptr->SetYAxisUnits( String("mol/l") );

    ptr->SetPlotTitle( String( "This is a 5th plot of Conc vs. time" ) );

    ptr->AddDataSet( x1, y1, String( "s") );
    ptr->AddDataSet( x1, y2, String( "t") );
    ptr->AddDataSet( x1, y3, String( "u") );
    ptr->AddDataSet( x1, y4, String( "v") );
    ptr->AddDataSet( x1, y5, String( "w") );
    ptr->AddDataSet( x1, y6, String( "x") );
    ptr->AddDataSet( x1, y7, String( "y") );
    ptr->AddDataSet( x1, y8, String( "z") );
    ptr->AddDataSet( x1, y9, String( "A") );
    ptr->AddDataSet( x1, y10, String( "B"));
    ptr->AddDataSet( x1, y11, String( "C"));
    ptr->AddDataSet( x1, y12, String( "D"));
    ptr->AddDataSet( x1, y13, String( "E"));
    ptr->AddDataSet( x1, y14, String( "F"));
    ptr->AddDataSet( x1, y15, String( "G"));

    ptr->ShowMarkers( FALSE );


     // display the main window 


     main_window.Show();


     StackWindow stack_window( NULL, WB_STDWORK | WB_SVLOOK ); 
     stack_window.AddDataSet( x1, y1, "Set1" );
     stack_window.AddDataSet( x1, y2, "Set2" );
     stack_window.AddDataSet( x1, y3, "Set3" );
     stack_window.AddDataSet( x1, y4, "Set4" );
     stack_window.AddDataSet( x1, y5, "Set5" );
     stack_window.AddDataSet( x1, y6, "Set6" );
     stack_window.AddDataSet( x1, y7, "Set7" );
     stack_window.AddDataSet( x1, y8, "Set8" );

     stack_window.Show();

     Execute();

}

// instantiate MainApp

MainApp aMainApp;



