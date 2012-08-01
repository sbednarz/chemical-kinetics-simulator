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
//  SCHEMWN2.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the TwoDArraySchemeWindow class, an MDI window 
//  containing a view of a reaction scheme. This is the base class for the 
//  various varieties of SchemeWindows
//
//  CREATED : 6.24.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__SCHEMWN2_HXX__)
#define __SCHEMWN2_HXX__



#include "schemwin.hxx"


class TwoDArraySchemeWindow : public SchemeWindow 
{
protected:     

     UINT16              num_rows;
     UINT16              num_columns;

     virtual void        MouseMove( const MouseEvent& rMEvt );
     virtual void        MouseButtonDown( const MouseEvent& rMEvt );
     virtual void        MouseButtonUp( const MouseEvent& rMEvt );


public:

                        TwoDArraySchemeWindow( Window* pParent );


     virtual BOOL8      PasteObject( const ProcessObject* const pClipObject );
     virtual BOOL8      CopySelectedObject( ProcessObject*& rpClipObject );

     virtual BOOL8       Add2DArray( UINT16 NumRows, UINT16 NumColumns );


     virtual enum SCHEMWIN_TYPE  GetSchemeType( ) const { return SW_TYPE_2D; }

};


#endif

