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
//  XFEROBJ.HXX - a header file  of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the TransferObject class for
//  the VSIM user interface. 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#include <sv.hxx>
#pragma hdrstop


#if !defined(__XFEROBJ_HXX__)
#define __XFEROBJ_HXX__

#include "procsobj.hxx"
#include "xferstep.hxx"
#include "arrow.hxx"

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


DECLARE_LIST( TransferStepList, TransferStep* )


class TransferObject : public ProcessObject
{
protected:
     TransferStepList      transfer_step_list;
     UINT16                listbox_selection;
     enum DIRECTION        direction;

     CompartmentObject*    psource_compartment;
     CompartmentObject*    ptarget_compartment;
     Arrow                 arrow;

     void                  SetFillColor();

public:
                           // declare but do not define default constructor to avoid inadvertent use 

                           TransferObject( );
                           TransferObject( const String& rName, SchemeWindow* pParent );
                           TransferObject( const TransferObject& rXferObj );
     virtual              ~TransferObject( );

     TransferObject& operator = ( const TransferObject& rObject );

     virtual void        SetpParent(  SchemeWindow* pParent );
     virtual void        SetpOutputDevice(  OutputDevice* pDevice );

     virtual void          DrawObject( );
     virtual UINT16        GetIndex( );

     CompartmentObject*    GetpSourceCompartment( ) const 
          { return psource_compartment; }

     CompartmentObject*    GetpTargetCompartment( ) const 
          { return ptarget_compartment; }

     void                  SetpSourceCompartment( CompartmentObject* pComp) 
          { psource_compartment = pComp; }

     void                  SetpTargetCompartment( CompartmentObject* pComp) 
          { ptarget_compartment = pComp; }

     virtual ProcObjType   GetType( ) const { return PO_TYPE_XFER; };

     virtual void          Reposition( );

     void                  ShowArrowHead( BOOL8 NewState ) { arrow.ShowArrowHead( NewState ); }

     virtual void          DrawActiveArea( );

     virtual ProcObjRC     EditContents ( );
     virtual BOOL8         IsEmpty (  )  const { return ( 0 ==  transfer_step_list.Count() ); }

     virtual void          SynchSpeciesListWithDatabase( SpeciesDatabase& rSDB );

     UINT16                GetTransferListboxSelection() const { return (UINT16) listbox_selection; }
     void                  SetTransferListboxSelection( UINT16 NewValue) { listbox_selection = NewValue; }
     BOOL8                 AddNewTransferStep( const TransferStep& rStep );
     BOOL8                 IsSpeciesNameInList( const String& rSpeciesName );
     void                  DeleteTransferStep( UINT16 Position );
     UINT16                GetNumberOfTransferSteps( ) const { return ( UINT16) transfer_step_list.Count(); }
     const TransferStep&   GetTransferStep( UINT16 i ) const { return *(transfer_step_list.GetObject( (ULONG) i ) ); }
     void                  SetTransferStep( UINT16 i, const TransferStep& rStep ) { *(transfer_step_list.GetObject( (ULONG) i )) = rStep; }

     enum DIRECTION        GetDirection( ) const { return direction; }
     void                  SetDirection( enum DIRECTION NewDirection ) { direction = NewDirection; }

     virtual BOOL8       IsVisibleWithin( const Rectangle& rRect );

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferObject& rObject );
     friend BinaryInputStream& operator  >> ( BinaryInputStream& rBIS, TransferObject& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, TransferObject& rObject );

};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferObject& rObject );
BinaryInputStream& operator  >> ( BinaryInputStream& rBIS, TransferObject& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, TransferObject& rObject );

#endif

