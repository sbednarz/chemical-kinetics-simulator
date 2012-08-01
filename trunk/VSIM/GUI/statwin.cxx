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
//  STATWIN.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This modulee defines the StatusWindow class for displaying 
//  a curent status of a procedure in process
//
//  CREATED : 6.25.96 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "statwin.hxx"

#endif







// ----------------------------------------------------------------------------
//                 StausWindow::StatusWindow()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

StatusWindow::StatusWindow( Window * pParent) :
WorkWindow( pParent, ResId( vID_WIN_STATUS ) ),
wait_text( this, ResId( 1 ) ),
status_text( this, ResId( vID_STEXT_CURRENT_OP ) ),
clock_bitmap( this, ResId( vID_CLOCK_ICON ) )
{
     FreeResource();
     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );

     wait_text.Show( );
     status_text.Show( );
}







 void StatusWindow::SetStatusText( const String& rText) 
 { 
      status_text.SetText( rText ); 
      status_text.Invalidate();
      status_text.Update(); 

      GetpMainApp()->Reschedule();
 }
