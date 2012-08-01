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
//  STATWIN.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the StatusWindow class for displaying 
//  a curent status of a procedure in process
//
//  CREATED : 6.25.96 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if !defined(__STATWIN_HXX__)
#define __STATWIN_HXX__


#include <sv.hxx>
#include "mainapp.hxx"

#include "statwin.hrc"

// ----------------------------------------------------------------------------
//                 class StatusWindow
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class StatusWindow : public WorkWindow
{
protected:
     FixedText    wait_text;
     FixedText    status_text;
     FixedIcon    clock_bitmap;

public:

     StatusWindow( Window * pParent );

     void SetStatusText( const String& rText);
};



#endif



