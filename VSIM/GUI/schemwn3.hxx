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
//  SCHEMWN3.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the ThreeDArraySchemeWindow class, an MDI window 
//  containing a view of a reaction scheme.
//
//  CREATED : 6.24.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__SCHEMWN3_HXX__)
#define __SCHEMWN3_HXX__



#include "schemwin.hxx"


class ThreeDArraySchemeWindow : public SchemeWindow 
{
protected:     

     UINT16              num_compts_x;
     UINT16              num_compts_y;
     UINT16              num_compts_z;

     virtual void        MouseMove( const MouseEvent& rMEvt );
     virtual void        MouseButtonDown( const MouseEvent& rMEvt );
     virtual void        MouseButtonUp( const MouseEvent& rMEvt );

     void                DeselectAllObjects( );
     void                SelectTransferObjects( );
     void                SelectCompartmentObjects( );


public:

                        ThreeDArraySchemeWindow( Window* pParent );


     virtual BOOL8      PasteObject( const ProcessObject* const pClipObject );
     virtual BOOL8      CopySelectedObject( ProcessObject*& rpClipObject );

     BOOL8              Add3DArray( UINT16 NumComptsX, UINT16 NumComptsY, UINT16 NumComptsZ,
                                    FLOAT64 CellXDimension, FLOAT64 CellYDimension,  
                                    FLOAT64 CellZDimension, BOOL8 WrapX, BOOL8 WrapY, BOOL8 WrapZ );

     void               SelectAllTransferObjects( );
     void               SelectAllCompartmentObjects( );

     virtual void       Activate();
     virtual void       Deactivate();

     void               ChangeCompartmentDimensions();
     void               ChangeSchemeTemperature();
    virtual void        ChangeSpeciesConcentration();

     virtual enum SCHEMWIN_TYPE  GetSchemeType( ) const { return SW_TYPE_3D; }

};


#endif

