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
//  XFEROBJ.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the TransferObject class for
//  the VSIM user interface. 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "xferobj.hxx"
#include "comptobj.hxx"
#include "vsimtype.hxx"
#include "schemwin.hxx"
#include "idtypes.hxx"
#include "mainapp.hxx"
#include "xobjdlg.hxx"

#endif







// ----------------------------------------------------------------------------
//                    TransferObject::TransferObject( )
// ............................................................................
//
//  constructor for TransferObject
//
// ----------------------------------------------------------------------------

TransferObject::TransferObject( const String& rName, SchemeWindow* pParent ) :
ProcessObject( rName, pParent ),
transfer_step_list(),
listbox_selection( 0 ),
direction( NONE ),
psource_compartment( NULL ),
ptarget_compartment( NULL ),
arrow( GetpOutputDevice() )
{
     // the active area is a rectangle surrounding the title

     const short COMPARTMENT_WIDTH = 10;  // in char widths
     const short COMPARTMENT_HEIGHT = 15;  // 1 character height = 10

     active_area = Rectangle( GetpOutputDevice()->PixelToLogic( Point( 0, 0) ),
      Size( COMPARTMENT_WIDTH * char_width, ( COMPARTMENT_HEIGHT * char_height)/10 ) );

	SetFillColor();
}




// ----------------------------------------------------------------------------
//                    TransferObject::IsVisibleWithin()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 TransferObject::IsVisibleWithin( const Rectangle& rRect )
{
     return ( rRect.IsOver( active_area )           || 
              rRect.IsInside( arrow.SourcePoint() ) ||
              rRect.IsInside( arrow.SourcePoint() )
            );
}





void  TransferObject::SetpParent(  SchemeWindow* pParent )
{
     arrow.SetpOutputDevice( pParent );
     ProcessObject::SetpParent( pParent );
}



void  TransferObject::SetpOutputDevice(  OutputDevice* pDevice ) 
{ 
     arrow.SetpOutputDevice( pDevice );
     ProcessObject::SetpOutputDevice( pDevice );
}





// ----------------------------------------------------------------------------
//                    TransferObject::TransferObject( )
// ............................................................................
//
//  copy constructor for TransferObject
//
// ----------------------------------------------------------------------------

TransferObject::TransferObject( const TransferObject& rXferObj ) :
ProcessObject( rXferObj.GetName(), rXferObj.GetpParent() ),
transfer_step_list(),
listbox_selection( rXferObj.listbox_selection ),
direction( NONE ),
psource_compartment( NULL ),
ptarget_compartment( NULL ),
arrow( rXferObj.GetpOutputDevice())
{
     // make a copy of each transfer step in object

     UINT16 num_xfer_steps = rXferObj.GetNumberOfTransferSteps( );

     for ( UINT16 i = 0; i < num_xfer_steps;  i++ )
     {
          // create a new object by copying

          TransferStep* pcurrent_step = new TransferStep( rXferObj.GetTransferStep( i ) );

          if ( NULL != pcurrent_step )

               // add to the reaction list 

               transfer_step_list.Insert( pcurrent_step, LIST_APPEND );
          else
          {
               GetpMainApp()->OutOfMemory();
               return;
          }
     }

     // data member from base class

     active_area = rXferObj.active_area;

	SetFillColor();
}






// ----------------------------------------------------------------------------
//                    TransferObject::operator =
// ............................................................................
// 
//  assignment operator 
//
// ----------------------------------------------------------------------------

TransferObject& TransferObject::operator = ( const TransferObject& rObject )
{
     // note : we explicitly do not copy info about connected compartments
     // eg psource_compartment and  ptarget_compartment; only copy internal info

     // avoid assignment to self 

     if ( &rObject == this )
          return *this;

     UINT16 count = GetNumberOfTransferSteps( );

     for ( UINT16 i = 0; i < count;  i++ )
          delete transfer_step_list.GetObject( (ULONG) i );

     transfer_step_list.Clear();

     count = rObject.GetNumberOfTransferSteps( );

     for ( i = 0; i < count;  i++ )
     {
          // create a new object by copying

          TransferStep* pcurrent_step = new TransferStep( rObject.GetTransferStep( i ) );

          if ( NULL != pcurrent_step )

               // add to the reaction list 

               transfer_step_list.Insert( pcurrent_step, LIST_APPEND );
          else
          {
               GetpMainApp()->OutOfMemory();
               return *this;
          }
     }


     listbox_selection    = rObject.listbox_selection;    
     direction            = rObject.direction;
                                                  
     arrow                = rObject.arrow;                

     ProcessObject::operator =( rObject );

     return *this;

}




// ----------------------------------------------------------------------------
//                    TransferObject::~TransferObject( )
// ............................................................................
// 
//  destructor for TransferObject. Removes references to this object 
//  imbedded in the source and target compartments. A virtual function 
//
// ----------------------------------------------------------------------------

TransferObject::~TransferObject( )
{
     if (psource_compartment )
          psource_compartment->DeleteTransferObject( this );

     if (ptarget_compartment )
          ptarget_compartment->DeleteTransferObject( this );

}






// ----------------------------------------------------------------------------
//                    TransferObject::SetFillColor( )
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

void TransferObject::SetFillColor()
{
     Color color = GetpOutputDevice()->GetBackgroundBrush().GetColor();
     highlight_font.ChangeFillColor( color );
     std_font.ChangeFillColor( color );
     std_fill.ChangeColor( color );
}








// ----------------------------------------------------------------------------
//                    TransferObject::DrawObject( )
// ............................................................................
//
//  standard draw procedure
//
// ----------------------------------------------------------------------------

void TransferObject::DrawObject( )
{
     arrow.DrawObject();
     DrawActiveArea( );
}





// ----------------------------------------------------------------------------
//                    TransferObject::DrawActiveArea( )
// ............................................................................
//
//  standard draw procedure
//
// ----------------------------------------------------------------------------

void TransferObject::DrawActiveArea( )
{
     SelectBrush();

     Pen old_pen;

     // do not draw outline of active area if normal state

     if ( state == PO_NORMAL )
     {
          Pen pen = GetpOutputDevice()->GetPen();
          pen.ChangeStyle( PEN_NULL );

          old_pen = GetpOutputDevice()->ChangePen( pen );
     }

     GetpOutputDevice()->DrawRect( active_area );

     if ( state == PO_NORMAL )
          GetpOutputDevice()->ChangePen( old_pen );

     DrawTitle( );
}





// ----------------------------------------------------------------------------
//                    TransferObject::GetIndex( )
// ............................................................................
//
// should be declared const but SV template mechanism leads to erroro msgs
//
// ----------------------------------------------------------------------------

UINT16 TransferObject::GetIndex( )
{ 
     return GetpParent()->GetScheme().GetTransferObjectIndexFromPtr( this );
} 







// ----------------------------------------------------------------------------
//                    TransferObject::Reposition( )
// ............................................................................
// 
//  this function uses the knowledge of the source and target conpartments
//  to recalculate the line connecting them so it does not extend
//  into their active areas. We also recalculate the position of the 
//  active area of this object, i.e., the box where the title is displayed
//  that box is supposed to be centered on the line connecting soutce and 
//  target compartments. The default StarView coordinate system 
//  orientation in the design ( upper left = (0,0) )
//
// ----------------------------------------------------------------------------

void TransferObject::Reposition( )
{
     // reposition the arrow so that it poijnt from source compartment to
     // target compartment   

     arrow.Reposition( GetpSourceCompartment()->GetActiveArea( ),
                        GetpTargetCompartment()->GetActiveArea()  );

     // now move the active area so that its center is over the center of 
     // the newly calculated line

     short x = ( arrow.SourcePoint().X() + arrow.TargetPoint().X())/2;
     short y = (arrow.SourcePoint().Y() + arrow.TargetPoint().Y())/2;


     Point new_origin( x - active_area.GetWidth()/2, y - active_area.GetHeight() /2 );

     active_area.ChangePos( new_origin );
}







// ----------------------------------------------------------------------------
//                    TransferObject::EditContents( )
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

ProcObjRC TransferObject::EditContents( )
{
     // this following fcn call returns an enum EDIT_XOBJ_RC - we xlate to other enum

     switch ( EditTransferObject( this ) )
     {
     case EXO_RC_FAIL : 

          return PO_RC_FAIL;


     case EXO_RC_CANCEL :     

          return PO_RC_CANCEL;


     case EXO_RC_CLOSE :

          return PO_RC_ACCEPT;

     }
}




// ----------------------------------------------------------------------------
//                    TransferObject::IsSpeciesNameInList( )
// ............................................................................
// 
//  this function scans through the existing list and returns TRUE
//  if the species name given by rSpeciesName represents a species already
//  in the TransferStep List, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 TransferObject::IsSpeciesNameInList( const String& rSpeciesName )
{
     TransferStep* current_rec;

     ULONG count = transfer_step_list.Count();

     BOOL8  found_it = FALSE;

     // now look for place to insert new record 

     for ( ULONG i = 0; ( i < count ) && !found_it; i++ )
     {
          current_rec = transfer_step_list.GetObject( i );

          // compare name in current_rec with search name ( case-insensitive )

          if ( COMPARE_EQUAL == rSpeciesName.ICompare( current_rec->GetSpeciesName() ) )
               found_it = TRUE;

          if ( XS_TYPE_VIRTUAL_DIFFUSION == current_rec->GetType() )
          {
               if ( COMPARE_EQUAL == rSpeciesName.ICompare( current_rec->GetSpeciesName2() ) )
                    found_it = TRUE;
          }
     }
     return found_it;
}






// ----------------------------------------------------------------------------
//                    TransferObject::AddNewTransferStep( )
// ............................................................................
// 
//  this function adds a new member to the transfer step list by 
//  making a copy of the one that is passed as a reference parameter.
//  After creating the new TransferStep object, this function then 
//  scans through the existing list and places the new object such that 
//  the records are all arranged in alphabetical order by species name    
//  if the list is empty then the record is added. returns TRUE is successul,
//  FALSE if memory allocation failure. We assume that the rStep object 
// is fully formed and validated before being passed to this function
//
// ----------------------------------------------------------------------------

BOOL8 TransferObject::AddNewTransferStep( const TransferStep& rStep )
{
     TransferStep* new_rec = new TransferStep( rStep );

     if ( NULL == new_rec )
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }

     // if we get to here we have successfully created another 
     // TransferStep object

     // now find the place to insert - we maintain list in alphabetical
     // order so we need to keep that in mind. We assume here that it
     // is not possible to be trying to add a record for a species that 
     // already is in the list; i.e., the functon that calls this one 
     // has responsibility for preventing that occurence 

     String species_name( new_rec->GetSpeciesName() );

     TransferStep* current_rec;

     ULONG count = transfer_step_list.Count();

     BOOL  insert_new_record = FALSE;

     // now look for place to insert new record 

     for (ULONG i = 0; ( i < count ) && !insert_new_record; i++ )
     {
          current_rec = transfer_step_list.GetObject( i );

          // compare name in current_rec with search name ( case-insensitive )

          if ( COMPARE_GREATER == species_name.ICompare( current_rec->GetSpeciesName() ) )
               insert_new_record = TRUE;
     }

     // OK, now where do we insert it? IF insert_new_record is TRUE 
     // then we add it right in front of current_rec. Otherwise we
     // append it at the end of the list 

     if ( insert_new_record )
          transfer_step_list.Insert( new_rec, current_rec );
     else
          transfer_step_list.Insert( new_rec, LIST_APPEND );

     return TRUE;
}


// ----------------------------------------------------------------------------
//                    CompartmentObject::DeleteRxnStep( )
// ............................................................................
// 
//  removes an existing TransferStep object from the internal list at Position. The value 
//  of Position is taken from the selected position in the transfer step list box.
//
// ----------------------------------------------------------------------------

void TransferObject::DeleteTransferStep( UINT16 Position )
{
     TransferStep* p_step = transfer_step_list.Remove( (ULONG) Position );

     ASSERT( p_step != NULL );
     delete p_step;
}





// ----------------------------------------------------------------------------
//                    TransferObject::SynchSpeciesListWithDatabase( )
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

void  TransferObject::SynchSpeciesListWithDatabase( SpeciesDatabase& rSDB )
{
     UINT16 num_steps = GetNumberOfTransferSteps( );

     for ( UINT16 i = 0; i < num_steps; i++)
     {
          TransferStep step = GetTransferStep( i );
          step.SynchWithSpeciesDatabase( rSDB );
          SetTransferStep( i, step );
     }

}

// ----------------------------------------------------------------------------
//              operator <<
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferObject& rObject )
{
     rBOS << rObject.title;
     rBOS << rObject.active_area;

     // write out the indices of source and target compartments 

     UINT16 index = rObject.GetpParent()->GetScheme().GetCompartmentObjectIndexFromPtr( rObject.psource_compartment );

     rBOS << index;

     index = rObject.GetpParent()->GetScheme().GetCompartmentObjectIndexFromPtr( rObject.ptarget_compartment );

     rBOS << index;

     rBOS << ( (UINT16) rObject.GetDirection() );

     // info about the transfer step list 

     rBOS << rObject.GetTransferListboxSelection();

     UINT16 num_steps = rObject.GetNumberOfTransferSteps( ); 

     rBOS << num_steps;

     // write contents of each transfer step to stream 

     for ( UINT16 i=0;  i < num_steps; i++)
          rBOS << *(rObject.transfer_step_list.GetObject(i) );

     return rBOS;
}



// ----------------------------------------------------------------------------
//                 operator <<
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, TransferObject& rObject )
{
     rTOS << IDType( START_TRANSFER_OBJECT_INIT ) << EOL;

     rTOS << IDType( TRANSFER_OBJECT_INDEX) << rObject.GetIndex() << EOL;



     switch ( rObject.GetpParent()->GetSchemeType() )
     {
     case SW_TYPE_3D :

     rTOS << IDType( TRANSFER_OBJECT_DIRECTION ) << ( (UINT16) rObject.GetDirection() ) << EOL;

          break;


     default : 
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;

     case SW_TYPE_COMPARTMENT :
          break;

     }

     rTOS << IDType( TRANSFER_OBJECT_DATA );

     // write out the indices of source and target compartments 

     UINT16 index = rObject.GetpParent()->GetScheme().GetCompartmentObjectIndexFromPtr( rObject.psource_compartment );

     rTOS << index;

     index = rObject.GetpParent()->GetScheme().GetCompartmentObjectIndexFromPtr( rObject.ptarget_compartment );

     rTOS << index;

     // info about the transfer step list 

     UINT16 num_steps = rObject.GetNumberOfTransferSteps( ); 


     // calculate the total number of reversible steps 

     UINT16 num_rev_steps = 0; 

     for ( UINT16 i=0;  i < num_steps; i++)
          if ( REVERSIBLE == (rObject.transfer_step_list.GetObject(i)->GetTransferDirection()) )
               num_rev_steps++;

     // write out the number of reversible steps followd by the number of irreversible steps 

     rTOS << num_rev_steps << (UINT16) ( num_steps - num_rev_steps ) << EOL;

     // write contents of each transfer step to stream 

     for ( i=0;  i < num_steps; i++)
          rTOS << *(rObject.transfer_step_list.GetObject(i) );

     rTOS << IDType( END_TRANSFER_OBJECT_INIT ) << EOL;

     return rTOS;
}




// ----------------------------------------------------------------------------
//                    operator >>
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, TransferObject& rObject )
{
     String tempstr;

     rBIS >> tempstr;
     rObject.SetName( tempstr );

     Rectangle rect;

     rBIS >> rect;

     rObject.SetActiveArea( rect );

     // read in the indices of source and target compartments 

     UINT16 source_compt_index;
     UINT16 target_compt_index;

     rBIS >> source_compt_index;
     rBIS >> target_compt_index;

     // convert them to pointers to the appropriate objects 

     rObject.psource_compartment = rObject.GetpParent()->GetScheme().GetCompartmentObjectPtrFromIndex( source_compt_index );
     rObject.ptarget_compartment = rObject.GetpParent()->GetScheme().GetCompartmentObjectPtrFromIndex( target_compt_index );


     // make the source and target compartments aware that this transferObject 
     // is connected to them 

     rObject.psource_compartment->AddTransferObject( &rObject );
     rObject.ptarget_compartment->AddTransferObject( &rObject );

     // and update the position data internal to the object 

     rObject.Reposition();

     // get geometric data 

     UINT16 temp_direction;

     rBIS >> temp_direction;

     rObject.SetDirection((enum DIRECTION) temp_direction ); 

     // read in info about transfer step list 

     rBIS >> rObject.listbox_selection;

     UINT16 num_steps; 

     rBIS >> num_steps;

     for ( UINT16 i=0;  i < num_steps; i++)
     {
          // allocate a new entry for list for each species 

          TransferStep* ptr = new TransferStep;

          // if successful then initialize the object from the input stream 

          if ( NULL != ptr )
          {
               rBIS >> *ptr;

               if (rBIS.FileError() )
                    return rBIS;

               // and then add the object to the end of the list

               rObject.transfer_step_list.Insert( ptr, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               rBIS.SetExtErrorFlag( );

               return rBIS;
          }
     }



     return rBIS;
}




