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
//  COMPTOBJ.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the CompartmentObject class for
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

#include "comptobj.hxx"
#include "xferobj.hxx"
#include "comptdlg.hxx"
#include "mainapp.hxx"
#include "schemwin.hxx"
#include "idtypes.hxx"
#include "specdb.hxx"
#include "assert.hxx"

#endif



#include "strings.hrc"


const short COMPARTMENT_WIDTH = 10;  // in char widths
const short COMPARTMENT_HEIGHT = 3;  // in char heights
const Color SHADOW_COLOR = Color( COL_3DSHADOW );

const enum TEMPERATURE_OPTION DEFAULT_TEMP_OPTION = CONSTANT_TEMP;
const enum VOLUME_OPTION DEFAULT_VOLUME_OPTION = NOT_TRACKED_VOLUME;
const enum PRESSURE_OPTION DEFAULT_PRESSURE_OPTION  = CONSTANT_PRESSURE;
const enum COMPARTMENT_PHASE_TYPE  DEFAULT_PHASE = GAS;



const FLOAT64  DEFAULT_X_DIMENSION = 1.0;
const FLOAT64  DEFAULT_Y_DIMENSION = 1.0;
const FLOAT64  DEFAULT_Z_DIMENSION = 1.0;




// ----------------------------------------------------------------------------
//                    CompartmentObject::CompartmentObject( )
// ............................................................................
// 
//  default constructor for CompartmentObject
//
// ----------------------------------------------------------------------------

CompartmentObject::CompartmentObject( const String& rName, SchemeWindow* pParent ) :
ProcessObject( rName, pParent ),
transobj_list(),
rxn_list(),
species_list(),
shadow_drop( char_height / 4, char_height / 4 ),
shadow_brush( SHADOW_COLOR ),
shadow_pen( SHADOW_COLOR ),
rxn_listbox_selection( 0 ),
num_reversible_rxns( 0 ),
x_dimension( DEFAULT_X_DIMENSION ),
y_dimension( DEFAULT_Y_DIMENSION ), 
z_dimension( DEFAULT_Z_DIMENSION ), 
unconstrained_direction( NONE ),
temperature_option( DEFAULT_TEMP_OPTION ),
volume_option( DEFAULT_VOLUME_OPTION ),
pressure_option( DEFAULT_PRESSURE_OPTION ),
phase( DEFAULT_PHASE ),
initial_var_volume( ResId( vID_STR_DEFAULT_VOLUME) ),
const_volume( ResId( vID_STR_DEFAULT_VOLUME) ), 
const_temp( ResId( vID_STR_DEFAULT_TEMP) ), 
initial_var_temp( ResId( vID_STR_DEFAULT_TEMP) ),   
convergence_std ( ResId( vID_STR_DEFAULT_T_CONV_STD) ),   
initial_prog_temp( ResId( vID_STR_DEFAULT_TEMP) ),  
prog_temp_rate( ResId( vID_STR_DEFAULT_PROG_T_RATE) ),  
prog_temp_max_value( ResId( vID_STR_DEFAULT_PROG_T_MAX_VAL ) ),
prog_temp_max_step_size( ResId( vID_STR_DEFAULT_PROG_T_MAX_STEP ) ),
t_profile_filename(  ResId( vID_STR_DEFAULT_PROG_T_FILENAME ) )
{
     // Initially set variables to place visual object in upper left corner of window.
     // The active area is a rectangle surrounding the title. set the size of the
     // rectangle in proportion to character size.

     active_area = Rectangle( GetpOutputDevice()->PixelToLogic( Point( 0, 0) ), 
      Size( COMPARTMENT_WIDTH * char_width, COMPARTMENT_HEIGHT * char_height) );

}







// ----------------------------------------------------------------------------
//                    CompartmentObject::IsVisibleWithin()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void  CompartmentObject::SetSpeciesConcentration( SPECIES_ID SpeciesID, const NumericString& NewConc )
{
     SpeciesListEntry spec_entry( species_list.GetSpeciesDataByID( SpeciesID ) );

     spec_entry.SetInitialConcentration( NewConc );

     species_list.SetSpeciesDataByID( SpeciesID, spec_entry );
}









// ----------------------------------------------------------------------------
//                    CompartmentObject::IsVisibleWithin()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

BOOL8 CompartmentObject::IsVisibleWithin( const Rectangle& rRect )
{
     return rRect.IsOver( active_area );
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::GetTotalMoles()
// ............................................................................
// 
//  calculates the total number of moles of all species contained in the 
//  compartment
//
// ----------------------------------------------------------------------------

FLOAT64 CompartmentObject::GetTotalMoles()
{
     FLOAT64 volume; 

     // first get the correct volume value 

     switch ( GetpParent()->GetSchemeType() )
     {
     case SW_TYPE_UNKNOWN :

          ASSERT( 0 );
          break;

     case SW_TYPE_3D :


          volume = GetXDimension() * 
                   GetYDimension() * 
                   GetZDimension() * 
                   GetpParent()->GetScheme().GetConversionFactorDimensionsToVolume();
          break;

     case SW_TYPE_COMPARTMENT :
     
          switch ( GetVolumeOption() )
          {
          case CONSTANT_VOLUME :
     
               volume = GetConstVolumeValue();
               break;
     
          case VARIABLE_VOLUME :
     
               volume = GetVarVolumeInitialValue();
               break;
     
          case NOT_TRACKED_VOLUME :
                  
               volume = NumericString( ResId( vID_STR_DEFAULT_VOLUME) ); 
               break;
     
          default :
     
               ASSERT( 0 );
               break;
          }
          break;
     }

     FLOAT64 sum_concs = 0.0;

     UINT16 num_species = GetNumberOfSpecies();

     for ( UINT16 i = 0; i < num_species; i++)
     {
          // get each entry in turn 

          SpeciesListEntry entry = species_list.GetSpeciesDataByPosition( i );

          // set its ID to that returned by the Database 

          sum_concs += ( (FLOAT64) entry.GetInitialConcentration() );
     }
     
     ASSERT ( volume != 0.0 );

     return sum_concs * volume;
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::CompartmentObject( )
// ............................................................................
// 
//  copy constructor for CompartmentObject.
//
// ----------------------------------------------------------------------------

CompartmentObject::CompartmentObject( const CompartmentObject& rCompt ) :
ProcessObject( ( const ProcessObject& ) rCompt ),
transobj_list(),
rxn_list(),
species_list( rCompt.species_list ),
shadow_drop( rCompt.shadow_drop ),
shadow_brush( rCompt.shadow_brush ),
shadow_pen( rCompt.shadow_pen ),
rxn_listbox_selection( rCompt.rxn_listbox_selection ),
num_reversible_rxns( rCompt.num_reversible_rxns ),
x_dimension( rCompt.x_dimension ),
y_dimension( rCompt.y_dimension ), 
z_dimension( rCompt.z_dimension ), 
unconstrained_direction( rCompt.unconstrained_direction ),
temperature_option( rCompt.temperature_option ),
volume_option( rCompt.volume_option ),
pressure_option( rCompt.pressure_option ),
phase( rCompt.phase ),
initial_var_volume( rCompt.initial_var_volume ),
const_volume( rCompt.const_volume ), 
const_temp( rCompt.const_temp ), 
initial_var_temp( rCompt.initial_var_temp ),   
convergence_std ( rCompt.convergence_std ),   
initial_prog_temp( rCompt.initial_prog_temp ),  
prog_temp_rate( rCompt.prog_temp_rate ),  
prog_temp_max_value( rCompt.prog_temp_max_value ),
prog_temp_max_step_size( rCompt.prog_temp_max_step_size ),
t_profile_filename( rCompt.t_profile_filename )
{

     UINT16 num_rxn_steps = rCompt.GetNumberOfRxnSteps();

     for ( UINT16 i = 0; i < num_rxn_steps;  i++ )
     {
          // create a new reaction object 

          Reaction* current_rxn = new Reaction( rCompt.GetRxnStep( i ) );

          if ( NULL != current_rxn )
               // add to the reaction list 
               rxn_list.Insert( current_rxn, LIST_APPEND );
          else
          {
               GetpMainApp()->OutOfMemory();
               return;
          }
     }

     UpdateSpeciesList();
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::operator =
// ............................................................................
// 
//  assignment operator 
//
// ----------------------------------------------------------------------------

CompartmentObject& CompartmentObject::operator = ( const CompartmentObject& rObject )
{
     // avoid assignment to self 

     if ( &rObject == this )
          return *this;

     // note that here we do not modify in any way the transobj_list. Upon asignment
     // the contents of the compartment chaneg but the connection of the compartment
     // with others are no altered 


     // First we go through each the reaction List contained in the compartment; 
     // we delete all the existing entries and create new entries 

     UINT16 count = GetNumberOfRxnSteps();

     // delete each existing reaction object 

     for ( UINT16 i = 0; i < count; i++ )
          delete rxn_list.GetObject( (ULONG) i );

     rxn_list.Clear();

     count = rObject.GetNumberOfRxnSteps();

     for ( i = 0; i < count;  i++ )
     {
          // create a new reaction object 

          Reaction* current_rxn = new Reaction( rObject.GetRxnStep( i ) );

          if ( NULL != current_rxn )
               // add to the reaction list 
               rxn_list.Insert( current_rxn, LIST_APPEND );
          else
          {
               GetpMainApp()->OutOfMemory();
               return *this;
          }
     }


     species_list = rObject.species_list;

     shadow_drop             = rObject.shadow_drop;              
     shadow_brush            = rObject.shadow_brush;             
     shadow_pen              = rObject.shadow_pen;               
                                                                 
     rxn_listbox_selection   = rObject.rxn_listbox_selection;    
     num_reversible_rxns     = rObject.num_reversible_rxns;      
                                                                 
     x_dimension             = rObject.x_dimension;              
     y_dimension             = rObject.y_dimension;              
     z_dimension             = rObject.z_dimension;              
     unconstrained_direction = rObject.unconstrained_direction;
                                                                 
     temperature_option      = rObject.temperature_option;      
     volume_option           = rObject.volume_option;           
     pressure_option         = rObject.pressure_option;         
     phase                   = rObject.phase;                                                            
                                                                 
     initial_var_volume      = rObject.initial_var_volume;       
     const_volume            = rObject.const_volume;             
                                                                 
     const_temp              = rObject.const_temp;               
     initial_var_temp        = rObject.initial_var_temp;         
     convergence_std         = rObject.convergence_std;          
     initial_prog_temp       = rObject.initial_prog_temp;        
     prog_temp_rate          = rObject.prog_temp_rate;           
     prog_temp_max_value     = rObject.prog_temp_max_value;      
     prog_temp_max_step_size = rObject.prog_temp_max_step_size;  
                                                                 
     t_profile_filename      = rObject.t_profile_filename;       


     ProcessObject::operator =( rObject );

     return *this;
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::~CompartmentObject( )
// ............................................................................
// 
//  destructor for CompartmentObject - is declared virtual in class declaration
//
// ----------------------------------------------------------------------------

CompartmentObject::~CompartmentObject( )
{
     const UINT16 num_rxn_steps = GetNumberOfRxnSteps();

     // delete each reaction object 

     for ( UINT16 i = 0; i < num_rxn_steps;  i++ )
          delete rxn_list.GetObject( (ULONG) i );
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::GetFirstTransferObject( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

TransferObject* CompartmentObject::GetFirstTransferObject( )
{
     // are there any entries in the list ? If so return the first, otherwise NULL 

     return  transobj_list.First() ? transobj_list.GetCurObject( ) : NULL;
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::GetNextTransferObject( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

TransferObject* CompartmentObject::GetNextTransferObject( )
{
     // is there another entry in the list ? If so return it, otherwise NULL 

     return  transobj_list.Next() ? transobj_list.GetCurObject( ) : NULL;
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::DrawActiveArea( )
// ............................................................................
// 
//  redraws only the active area, i.e that rectangular region of the object that 
//  is sensitive to mouse clicks
//
// ----------------------------------------------------------------------------

void CompartmentObject::DrawActiveArea( )
{
     SelectBrush();
     GetpOutputDevice()->DrawRect( active_area );
     DrawTitle( );
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::GetIndex( )
// ............................................................................
// 
// should be declared a const function but Starview template leads to errror msgs
//
// ----------------------------------------------------------------------------

UINT16 CompartmentObject::GetIndex( )
{ 
     return GetpParent()->GetScheme().GetCompartmentObjectIndexFromPtr( this );
} 




// ----------------------------------------------------------------------------
//                    CompartmentObject::DrawObject( )
// ............................................................................
// 
//  redraws only the entire object 
//
// ----------------------------------------------------------------------------

void CompartmentObject::DrawObject( )
{
     DrawShadow( );
     DrawActiveArea( );
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::DrawShadow( )
// ............................................................................
// 
//  redraws only the drop shadow   
//
// ----------------------------------------------------------------------------

void CompartmentObject::DrawShadow( )
{
     Brush old_brush = GetpOutputDevice()->ChangeFillInBrush( shadow_brush );
     Pen   old_pen   = GetpOutputDevice()->ChangePen( shadow_pen );  

     GetpOutputDevice()->DrawRect( Rectangle( 
       active_area.TopLeft() + shadow_drop, active_area.GetSize() )  ); 

     GetpOutputDevice()->ChangeFillInBrush( old_brush );
     GetpOutputDevice()->ChangePen( old_pen );  
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::AddTransferObject( )
// ............................................................................
// 
//  adds a new transfer object to the existing internal list 
//
// ----------------------------------------------------------------------------

void CompartmentObject::AddTransferObject( TransferObject* pTO )
{ 
     transobj_list.Insert( pTO, LIST_APPEND );
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::DeleteTransferObject( )
// ............................................................................
// 
//  deletes a transfer object from the existing internal list 
//
// ----------------------------------------------------------------------------

void CompartmentObject::DeleteTransferObject( TransferObject* pTO )
{
     transobj_list.Remove( pTO );
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::AddNewRxnStep( )
// ............................................................................
// 
//  adds a new Reaction object to the internal rxn_list at Position. The value 
//  of Position is taken from the selected position in the reaction step list box.
//
// ----------------------------------------------------------------------------

BOOL  CompartmentObject::AddNewRxnStep( UINT16 Position )
{
     Reaction* p_rxn = new Reaction;

     if ( p_rxn )
     {
          rxn_list.Insert( p_rxn, (ULONG) Position );
          return TRUE;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::DeleteRxnStep( )
// ............................................................................
// 
//  removes an existing Reaction object from the internal rxn_list at Position. The value 
//  of Position is taken from the selected position in the reaction step list box.
//
// ----------------------------------------------------------------------------

void  CompartmentObject::DeleteRxnStep( UINT16 Position )
{
     Reaction* prxn = rxn_list.Remove( (ULONG) Position );

     ASSERT( prxn != NULL );
     delete prxn;
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::RepositionTransferObjects( )
// ............................................................................
// 
//  this fcn is designed to be called after moving the compartment object on
//  screen. This causes all the TransferObjects attached to this Compartment
//  to recalculate their geometries
//
// ----------------------------------------------------------------------------

void CompartmentObject::RepositionTransferObjects( )
{
     if ( transobj_list.First() )
     {
          TransferObject* pto;

          while ( NULL != ( pto = transobj_list.GetCurObject() ) )
          {
               pto->Reposition();

               if ( !transobj_list.Next( ) )
                  break;
          }
     }
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::EditContents( )
// ............................................................................
// 
//  invokes the compartment edit dialog 
//
// ----------------------------------------------------------------------------

ProcObjRC CompartmentObject::EditContents( )
{
     return EditCompartment( GetpParent( ), this );
}




// ----------------------------------------------------------------------------
//                    CompartmentObject::IsEmpty( )
// ............................................................................
// 
//  tests to see whether the object has non-null contents          
//
// ----------------------------------------------------------------------------

BOOL CompartmentObject::IsEmpty ( ) const
{
     return TRUE;
}





// ----------------------------------------------------------------------------
//                    CompartmentObject::UpdateSpeciesList( )
// ............................................................................
// 
//  this fcn first sets as invalid and then reconstructs the list of 
//  species present in the compartment. returns TRUE if all operations successful, 
//  FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL CompartmentObject::UpdateSpeciesList( )
{
     // first set all entries in the species list as invalid 

     species_list.InvalidateAllReactiveEntries( );

     // next scan through the reaction list. For each reaction retrieve 
     // the name of each reactant and product in turn and add it to the 
     // species list. By doing so any existing entry in the species 
     // list with that name is set as valid and in use 


     UINT16 i = GetNumberOfRxnSteps();

     while  ( i-- )
     {
          const Reaction& r_rxn = GetRxnStep( i );

          UINT16 j = r_rxn.GetNumReactants( );

          while (j--)
               if ( FALSE == species_list.AddReactiveSpecies( r_rxn.GetReactantName( j ) ) )
                    return FALSE;

          j = r_rxn.GetNumProducts( );

          while (j--)
               if ( FALSE == species_list.AddReactiveSpecies( r_rxn.GetProductName( j ) ) )
                    return FALSE;

     }

     // finally delete all the previously used but now unused entries from
     // species_list

     species_list.RemoveUnusedReactiveEntries( );
     return TRUE;
}





//----------------------------------------------------------------------------
//              CompartmentObject::SynchSpeciesListWithDatabase( )
//............................................................................
//                                                                          
//  this function goes through the compartment's species list and 
//  acquires a SPECIES_ID value for each of the species in the list.
/// by doing so we synchronize the locally held values of each species 
//  ID with those in the global SpeciesDatabase. Aftet this is complete we 
//  then call an analogous function for each Reaction in the Compartment 
//  to ensure that the data in those is in synch with everything else
//                                                                          
//----------------------------------------------------------------------------

void CompartmentObject::SynchSpeciesListWithDatabase( SpeciesDatabase& rDatabase )
{
     UINT16 num_species = GetNumberOfSpecies();

     for ( UINT16 i = 0; i < num_species; i++)
     {
          // get each entry in turn 

          SpeciesListEntry entry = species_list.GetSpeciesDataByPosition( i );

          // set its ID to that returned by the Database 

          entry.SetID( rDatabase.GetSpeciesID( entry.GetName( ) ) );

          // and update the data entry if thr species_list 

          species_list.SetSpeciesDataByPosition( i, entry );
     }

     UINT16 num_rxns = GetNumberOfRxnSteps( );

     num_reversible_rxns = 0;

     for ( i = 0; i < num_rxns; i++)
     {
          Reaction rxn = GetRxnStep( i );
          rxn.SynchRxnDataWithSpeciesList( species_list );
          SetRxnStep( i, rxn );

          if (rxn.IsReversible() )
               num_reversible_rxns++;
     }

}




//----------------------------------------------------------------------------
//                        operator << CompartmentObject&                 
//............................................................................
//                                                                          
//  for output of contents of a Reaction object to BinaryOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentObject& rObject )
{
     rBOS << IDType( COMPARTMENT );
     rBOS << rObject.title;
     rBOS << rObject.active_area;

     Color   color = rObject.GetObjectColor();

     rBOS << color.GetRed();
     rBOS << color.GetGreen();
     rBOS << color.GetBlue();

     UINT16 num_rxn_steps    = rObject.GetNumberOfRxnSteps( );

     rBOS << rObject.GetNumTransferObjects( );
     rBOS << num_rxn_steps;
     rBOS << rObject.GetNumberOfSpecies( ); 
     rBOS << rObject.GetRxnListboxSelection( );
     rBOS << rObject.GetNumberOfReversibleRxnSteps( );

     rBOS << (UINT16) rObject.GetTemperatureOption ( );
     rBOS << (UINT16) rObject.GetVolumeOption();
     rBOS << (UINT16) rObject.GetPressureOption();
     rBOS << (UINT16) rObject.GetPhase();

     rBOS << rObject.GetVarVolumeInitialValue( );
     rBOS << rObject.GetConstVolumeValue( );
     
     rBOS << rObject.GetConstTempValue( );
     rBOS << rObject.GetVarTempInitialValue( );
     rBOS << rObject.GetVarTempConvergenceStd( );
     rBOS << rObject.GetProgTempInitialValue( );
     rBOS << rObject.GetProgTempRate( );
     rBOS << rObject.GetProgTempMaxValue( );
     rBOS << rObject.GetProgTempMaxStepSize( );

     rBOS << rObject.GetXDimension( );
     rBOS << rObject.GetYDimension( );
     rBOS << rObject.GetZDimension( );

     rBOS << ( (UINT16) rObject.GetUnconstrainedDirection() );
     
     // now write out the contents of each list in order 

     for ( UINT16 i = 0; i < num_rxn_steps; i++ )
     {
          Reaction* current_rxn = rObject.rxn_list.GetObject( i );

          rBOS << *current_rxn;
     }

      rBOS << rObject.species_list;

     return rBOS;
}





//----------------------------------------------------------------------------
//                        operator << CompartmentObject&                 
//............................................................................
//                                                                          
//  for output of contents of a CompartmentObject to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, CompartmentObject& rObject )
{
     rTOS << IDType( START_COMPARTMENT_INIT ) << EOL;

     rTOS << IDType( COMPARTMENT_INDEX) << rObject.GetIndex() << EOL;

     UINT16 num_rxn_steps    = rObject.GetNumberOfRxnSteps( );
     UINT16 num_xfer_objs    = rObject.GetNumTransferObjects( );

     rTOS << IDType( COMPARTMENT_NUM_XFER_OBJS ) << num_xfer_objs << EOL;
     rTOS << IDType( COMPARTMENT_NUM_RXNS ) << num_rxn_steps << EOL;

     rTOS << IDType( COMPARTMENT_NUM_REVERSIBLE_RXNS ) << rObject.num_reversible_rxns << EOL;

     rTOS << IDType( COMPARTMENT_NUM_SPECIES ) << rObject.GetNumberOfSpecies( ) << EOL;

     // write out values of options - what we write later depends on these settings 

     rTOS << IDType( COMPARTMENT_TEMPERATURE_OPTION ) << (UINT16) rObject.GetTemperatureOption( ) << EOL;
     rTOS << IDType( COMPARTMENT_PRESSURE_OPTION    ) << (UINT16) rObject.GetPressureOption( ) << EOL; 
     rTOS << IDType( COMPARTMENT_VOLUME_OPTION      ) << (UINT16) rObject.GetVolumeOption( ) << EOL; 
     rTOS << IDType( COMPARTMENT_PHASE              ) << (UINT16) rObject.GetPhase( ) << EOL; 

     // write out the proper initial temperature value - which one we 
     // use depends on what temperature setting the user has selected 

     rTOS << IDType( COMPARTMENT_INIT_TEMP );

     switch ( rObject.GetTemperatureOption() )
     {

          case CONSTANT_TEMP :
             rTOS << rObject.GetConstTempValue() << EOL;
             break;

          case VARIABLE_TEMP :

             rTOS << rObject.GetVarTempInitialValue() << EOL;
             break;

          case ANALYTIC_PROGR_TEMP :
          case EXT_PROFILE_PROGR_TEMP :

             rTOS << rObject.GetProgTempInitialValue() << EOL;

             break;

          default:

             // if we get here it's a logic error 

            ASSERT( 0 );
            break;
     }


     switch (rObject.GetpParent()->GetSchemeType() )
     {

     case SW_TYPE_UNKNOWN :
     default:

          ASSERT( 0 );
          break;


     case SW_TYPE_COMPARTMENT :

          // now do a similar thing with the volume if this is a Compartmental
          // model-based scheme

          rTOS << IDType( COMPARTMENT_INIT_VOLUME );
     
          switch ( rObject.GetVolumeOption() )
          {
     
               case CONSTANT_VOLUME :
     
                  rTOS << rObject.GetConstVolumeValue() << EOL;
                  break;
     
               case VARIABLE_VOLUME :
     
                  rTOS << rObject.GetVarVolumeInitialValue() << EOL;
                  break;
     
               case NOT_TRACKED_VOLUME :
     
                  rTOS << NumericString( ResId( vID_STR_DEFAULT_VOLUME) ) << EOL;
     
                  break;
     
               default:
     
                  // if we get here it's a logic error 
     
                 ASSERT( 0 );
                 break;
          }

          break;


     case SW_TYPE_3D :

          // if the compartment is part of a 3-D array-based scheme then write out the 
          // dimensions 

          rTOS << IDType( COMPARTMENT_DIMENSIONS );
          rTOS << rObject.GetXDimension( ) << rObject.GetYDimension() << 
           rObject.GetZDimension() << ( (UINT16) rObject.GetUnconstrainedDirection() ) << EOL;

          break;
     }

#if 0 
     // do the same thing a third time with pressure 

     rTOS << IDType( COMPARTMENT_INIT_PRESSURE );
                    COMPARTMENT_INIT_PRESSURE = 'CPrs',


     switch ( rObject.GetPressureOption() )
     {

          case CONSTANT_PRESSURE :

             rTOS << rObject.GetConstPressureValue() << EOL;
             break;

          case VARIABLE_VOLUME

             rTOS << rObject.GetVarPressureInitialValue() << EOL;
             break;

          default:

             // if we get here it's a logic error 

            ASSERT( 0 );
            break;
     }

#endif


     // now write out the contents of each list in order 

     UINT16 i;

     rTOS << IDType(COMPARTMENT_XFER_OBJ_INDEX);

     for ( i = 0; i < num_xfer_objs; i++ )
     {
          rTOS << rObject.transobj_list.GetObject( i )->GetIndex();
     }

     rTOS << EOL;


     for ( i = 0; i < num_rxn_steps; i++ )
     {
          Reaction* current_rxn = rObject.rxn_list.GetObject( i );

          rTOS << *current_rxn;
     }

      rTOS << rObject.species_list;


     rTOS << IDType( END_COMPARTMENT_INIT ) << EOL;

     return rTOS;
}





//----------------------------------------------------------------------------
//                        operator >> CompartmentObject&                 
//............................................................................
//                                                                          
//  for input of contents of a Reaction object from BinaryInputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, CompartmentObject& rObject )
{
     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( COMPARTMENT ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }

     String tempstr;

     rBIS >> tempstr;
     rObject.SetName( tempstr );

     Rectangle rect;

     rBIS >> rect;

     rObject.SetActiveArea( rect );

     UINT16 red;
     UINT16 green;
     UINT16 blue;

     rBIS >> red;
     rBIS >> green;
     rBIS >> blue;

     rObject.ChangeObjectColor( Color( red, green, blue ) );

     UINT16 num_xfer_objects;
     UINT16 num_rxn_steps;
     UINT16 num_species;

     rBIS >> num_xfer_objects;
     rBIS >> num_rxn_steps;
     rBIS >> num_species;


     UINT16 temp_val;

     rBIS >> temp_val;
     rObject.SetRxnListboxSelection( temp_val );

     rBIS >> temp_val;
     rObject.num_reversible_rxns = temp_val;

     rBIS >> temp_val;
     rObject.SetTemperatureOption ( (enum TEMPERATURE_OPTION) temp_val );

     rBIS >> temp_val;
     rObject.SetVolumeOption( (enum VOLUME_OPTION) temp_val );

     rBIS >> temp_val;
     rObject.SetPressureOption( (enum PRESSURE_OPTION) temp_val );  

     rBIS >> temp_val;
     rObject.SetPhase( (enum COMPARTMENT_PHASE_TYPE) temp_val );  

     rBIS >> tempstr;
     rObject.SetVarVolumeInitialValue( tempstr );

     rBIS >> tempstr;
     rObject.SetConstVolumeValue( tempstr );
     
     rBIS >> tempstr;
     rObject.SetConstTempValue( tempstr );

     rBIS >> tempstr;
     rObject.SetVarTempInitialValue( tempstr );

     rBIS >> tempstr;
     rObject.SetVarTempConvergenceStd( tempstr );

     rBIS >> tempstr;
     rObject.SetProgTempInitialValue( tempstr );

     rBIS >> tempstr;
     rObject.SetProgTempRate( tempstr );

     rBIS >> tempstr;
     rObject.SetProgTempMaxValue( tempstr );

     rBIS >> tempstr;
     rObject.SetProgTempMaxStepSize( tempstr );

     FLOAT64 tempflt;

     rBIS >> tempflt;
     rObject.SetXDimension( tempflt );

     rBIS >> tempflt;
     rObject.SetYDimension( tempflt );

     rBIS >> tempflt;
     rObject.SetZDimension( tempflt );
     
     UINT16 temp_direction;

     rBIS >> temp_direction;

     rObject.SetUnconstrainedDirection((enum DIRECTION) temp_direction ); 

     // now read in the contents of each list in order 

     for ( UINT16 i = 0; i < num_rxn_steps; i++ )
     {
          // create a new reaction object 

          Reaction* current_rxn = new Reaction;

          if ( NULL != current_rxn )
          {
               // initialize the new object using data from input stream

               rBIS >> *current_rxn;

               if (rBIS.FileError() )
                    return rBIS;

               // and add to the reaction list 
               rObject.rxn_list.Insert( current_rxn, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               rBIS.SetExtErrorFlag( );
               return rBIS;
          }

     }

      rBIS >> rObject.species_list;



     return rBIS;
}



ClippedCompartmentObject::ClippedCompartmentObject( const CompartmentObject& rCompt ) :
CompartmentObject( rCompt ),
species_data_list()
{
     // fill in species data list 

     UINT16 num_species = species_list.GetNumberOfSpecies( );

     for ( UINT16 i = 0; i < num_species; i++ )
     {
          const String& name = species_list.GetSpeciesDataByPosition( i ).GetName();

          const SpeciesDatabaseEntry& r_entry = rCompt.GetpParent()->GetScheme().GetpSpeciesDatabase()->GetSpeciesDataByName( name );

          SpeciesDatabaseEntry* p_entry = new SpeciesDatabaseEntry( r_entry );

          if ( NULL != p_entry )
          {
               // add to local species_data_list

               species_data_list.Insert( p_entry, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               return;
          }

     }

}

ClippedCompartmentObject::~ClippedCompartmentObject()
{
     // do we have any entries in the list? if so work through 
     // and delete all entries

     // work from the last entry to the first 

     ULONG position = species_data_list.Count( );

     while (position--)
          delete species_data_list.GetObject( position );
}
