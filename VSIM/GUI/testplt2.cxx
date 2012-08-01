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


#include "abargraf.hxx"





BOOL8 EditAxis( XyAxis* pAxisObject ) { return FALSE; }
BOOL8 Edittitle( PlotTitle* ptitle ) { return FALSE; }





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
     AnimatedBarGraph    graph;

public:
            MainWindow( Window* pParent, WinBits aWinStyle );
              

     BOOL8              AddElement( const FloatArray& rArray, const String& rName )
                                      { return graph.AddElement( rArray, rName ); }

     BOOL8               SetDeterminateVariableBar( const FloatArray& rArray, const String& rName ) 
                                      { return graph.SetDeterminateVariableBar( rArray, rName ); }

     virtual void    Resize();
     virtual void    Paint( const Rectangle& rRect );

};




// ----------------------------------------------------------------------------
//                      MainWindow::Paint( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MainWindow::Paint( const Rectangle& )
{
     graph.Play();
}




// ----------------------------------------------------------------------------
//                         MainWindow::MainWindow( )
// ............................................................................
//
//  constructor - creates  StatusBar also
// ----------------------------------------------------------------------------

MainWindow::MainWindow( Window* pParent, WinBits aWinStyle ):
WorkWindow( pParent, aWinStyle ),
graph( this )
{
     graph.SetStepTime( 20 );
     graph.Show();
}



// ----------------------------------------------------------------------------
//                             MainWindow::Resize( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void MainWindow::Resize( )
{
     graph.SetPosSizePixel( Point( ), GetOutputSizePixel() );

     Invalidate();

     graph.Rewind();
     graph.Play();
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
     const UINT32 ARRAY_SIZE = 200;

     FloatArray x1(ARRAY_SIZE);
     FloatArray y1(ARRAY_SIZE);
     FloatArray y2(ARRAY_SIZE);
     FloatArray y3(ARRAY_SIZE);
     FloatArray y4(ARRAY_SIZE);
     FloatArray y5(ARRAY_SIZE);
     FloatArray y6(ARRAY_SIZE);
     FloatArray y7(ARRAY_SIZE);
     FloatArray y8(ARRAY_SIZE);
     FloatArray y9(ARRAY_SIZE);
     FloatArray y10(ARRAY_SIZE);
     FloatArray y11(ARRAY_SIZE);
     FloatArray y12(ARRAY_SIZE);
     FloatArray y13(ARRAY_SIZE);
     FloatArray y14(ARRAY_SIZE);
     FloatArray y15(ARRAY_SIZE);

     for ( UINT16 i = 0 ; i < x1.GetArraySize(); i++ )
     {
          x1[i] = (FLOAT64) i; 
     }


     for ( i = 0 ; i < x1.GetArraySize(); i++ )
     {
          y1[i] = x1[i] * x1[i];
          y2[i] = x1[i] * x1[i] * x1[i];
          y3[i] = ARRAY_SIZE - i; 
     }


     for ( i = 0 ; i < x1.GetArraySize()/2; i++ )
     {
          y4[i] = (FLOAT64) i;   // goes up then down 
          y5[i] = ARRAY_SIZE/2 - i;   // goes down then up 
     }

     for ( i = x1.GetArraySize()/2; i < x1.GetArraySize(); i++ )
     {
          y4[i] = (FLOAT64) (x1.GetArraySize()/2 - i); 
          y5[i] = (FLOAT64) ( i - x1.GetArraySize()/2 );
     }



     MainWindow main_window( NULL, WB_APP | WB_STDWORK | WB_SVLOOK );


     main_window.ChangeFont( main_window.GetFontMetric( Font( FAMILY_SWISS, Size( 0,10 ) ) ).GetPhysicalFont() );

     main_window.SetDeterminateVariableBar( x1, "Set 1");

      main_window.AddElement( x1, "Set1" );
#if 1
      main_window.AddElement( y1, "Set2" );
      main_window.AddElement( y2, "Set3" );
      main_window.AddElement( y3, "Set4" );
      main_window.AddElement( y4, "Set5" );
      main_window.AddElement( y5, "Set6" );
#endif
      main_window.Show();

     Execute();

}

// instantiate MainApp

MainApp aMainApp;



