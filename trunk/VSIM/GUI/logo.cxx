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
//  LOGO.CXX - a module for the VSIM User interface
// ............................................................................
//
//  This module defines the ABoutDialog and a timed splash screen for program 
//  startup.
//
//  CREATED :  1.30.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "logo.hxx"
#include "mainapp.hxx"

#endif



#include "logo.hrc"



// ----------------------------------------------------------------------------
//                 class AboutDialog
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

class AboutDialog : public ModalDialog
{
protected:
    FixedBitmap  aFixedBitmap1;
    OKButton     aOKButton1;
    FixedText    aFixedText1;
public:
    AboutDialog( Window * pParent );
};






// ----------------------------------------------------------------------------
//                 AboutDialog::AboutDialog()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

AboutDialog::AboutDialog( Window * pParent)
: ModalDialog( pParent, ResId( vID_DLG_ABOUT ) ),
aFixedBitmap1( this, ResId( 1 ) ),
aOKButton1( this, ResId( 1 ) ),
aFixedText1( this, ResId( 1 ) )
{
     FreeResource();
     SetText( GetParent()->GetText() );
    
     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}





#define LOGO_VIEW_PERIOD       3500

Timer LogoTimer;



// ----------------------------------------------------------------------------
//                 class LogoWindow
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class LogoWindow : public WorkWindow
{
protected:
     FixedBitmap aFixedBitmap1;

     void StartTimer( );

public:

    LogoWindow( Window * pParent );

     void TimeOutHandler( Timer* );
};





LogoWindow* LogoWindowPtr = NULL;





// ----------------------------------------------------------------------------
//                 LogoWindow::LogoWindow()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

LogoWindow::LogoWindow( Window * pParent)
: WorkWindow( pParent, ResId( vID_WIN_LOGO ) ),
aFixedBitmap1( this, ResId( 1 ) )
{
    FreeResource();

     GetpMainApp()->CenterWindowOnParent( this, NULL );
     aFixedBitmap1.Show( );
     StartTimer( );
     Update( );
}





// ----------------------------------------------------------------------------
//                 LogoWindow::StartTimer()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void LogoWindow::StartTimer( )
{
     LogoTimer.ChangeTimeout( LOGO_VIEW_PERIOD );
     LogoTimer.ChangeTimeoutHdl( LINK( this, LogoWindow, TimeOutHandler ) );
     LogoTimer.Start( );
}




// ----------------------------------------------------------------------------
//                 LogoWindow::TimeOutHandler()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void LogoWindow::TimeOutHandler( Timer* )
{
     Hide( );
     LogoWindowPtr = NULL;
     delete this;
}



// ----------------------------------------------------------------------------
//                 ShowAboutDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowAboutDialog( Window* pParent )
{
    AboutDialog* ptr = new AboutDialog( pParent );

     if (ptr) 
          ptr->Execute();

}




// ----------------------------------------------------------------------------
//                 ShowLogo()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowLogo( Window* pParent )
{
    LogoWindowPtr = new LogoWindow( pParent );

     if ( LogoWindowPtr ) 
     {
          LogoWindowPtr->Show();

          while ( LogoWindowPtr )
               GetpMainApp()->Reschedule();
     }
      else
           return;

}




