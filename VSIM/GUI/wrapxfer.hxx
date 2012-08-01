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
//  WRAPXFERHXX - a header file  of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the WrappedTransferObject class for
//  the VSIM user interface. 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#pragma hdrstop


#if !defined(__WRAPXFER_HXX__)
#define __WRAPXFER_HXX__

#include "procsobj.hxx"
#include "xferstep.hxx"
#include "arrow.hxx"
#include "xferobj.hxx"

class CompartmentObject;
class BinaryOutputStream;
class BinaryInputStream;
class TextOutputStream;


// start of TransferObject


// ----------------------------------------------------------------------------
//                    class TransferObject              
// ............................................................................
// 
//  for manipulating properties of a TransferProcess                            
//
// ----------------------------------------------------------------------------

enum WRAP_DIRECTION { UNSPECIFIED_WRAP, HORIZONTAL_WRAP, VERTICAL_WRAP, DEPTH_WRAP };

class WrappedTransferObject : public TransferObject
{
protected:
     Rectangle             phantom_compt_rect;
     enum WRAP_DIRECTION   wrap_direction;
     
     void                  PositionPhantomCompartment();
     enum WRAP_DIRECTION   DetermineWrapDirection();

public:
                           // declare but do not define default constructor to avoid inadvertent use 

                           WrappedTransferObject( );

                           WrappedTransferObject( const String& rName, SchemeWindow* pParent );
     virtual              ~WrappedTransferObject( );

     virtual void          DrawObject( );
     void                  DrawPhantomCompartment( );

     virtual ProcObjType   GetType( ) const { return PO_TYPE_WRAPPED_XFER; };

     virtual void          Reposition( );

     Rectangle             GetPhantomRectangle() const { return phantom_compt_rect; }
     enum WRAP_DIRECTION   GetWrapDirection() const { return wrap_direction; }
};


#endif

