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
//  CSCHEMWN.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the CompartmentSchemeWindow class, an MDI window 
//  containing a view of a reaction scheme. This is the base class for the 
//  various varieties of SchemeWindows
//
//  CREATED : 4.29.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__CSCHEMWN_HXX__)
#define __CSCHEMWN_HXX__



#include "schemwin.hxx"


class CompartmentSchemeWindow : public SchemeWindow 
{
protected:     

    virtual void        MouseMove( const MouseEvent& rMEvt );
    virtual void        MouseButtonDown( const MouseEvent& rMEvt );
    virtual void        MouseButtonUp( const MouseEvent& rMEvt );

    void                EndAddTransferObject( Point& rPos );
    void                EndMoveCompartment( Point& rPos );


    void                SetMoveObject ( ProcessObject* pPO = NULL );
    ProcessObject*      GetMoveObject ();


public:

                        CompartmentSchemeWindow( Window* pParent );

     virtual void       StartAddCompartment(  );
     virtual void       StartAddTransferObject( );
     virtual void       CancelAddTransferObject( );
     virtual void       CancelAddCompartmentObject( );
     virtual BOOL8      PasteObject( const ProcessObject* const pClipObject );
     virtual void       DeleteSelectedObject( );

     virtual void       Activate();

     virtual enum SCHEMWIN_TYPE  GetSchemeType( ) const { return SW_TYPE_COMPARTMENT; }

};


#endif
