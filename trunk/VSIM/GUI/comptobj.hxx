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
//  COMPTOBJ.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the CompartmentObject class for
//  the VSIM user interface. 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__COMPTOBJ_HXX__)
#define __COMPTOBJ_HXX__

#include "procsobj.hxx"
#include "reaction.hxx"
#include "speclist.hxx"
#include "specdb.hxx"

class TransferObject;
class SchemeWindow;

DECLARE_LIST( TransferObjectList, TransferObject* )
DECLARE_LIST( ReactionList, Reaction* )


class CompartmentObject : public ProcessObject
{
     friend class SpeciesConcDialog;

protected:

     TransferObjectList     transobj_list;
     ReactionList           rxn_list;
     CompartmentSpeciesList species_list;
     Point                  shadow_drop;
     Brush                  shadow_brush;
     Pen                    shadow_pen;

     UINT16                 rxn_listbox_selection;
     UINT16                 num_reversible_rxns;

     FLOAT64                x_dimension;
     FLOAT64                y_dimension;
     FLOAT64                z_dimension;

     enum DIRECTION         unconstrained_direction;

     // for tracking internal simulation settings 

     enum TEMPERATURE_OPTION temperature_option;
     enum VOLUME_OPTION      volume_option;
     enum PRESSURE_OPTION    pressure_option;
     enum COMPARTMENT_PHASE_TYPE  phase;

     NumericString          initial_var_volume;
     NumericString          const_volume;

     NumericString          const_temp;
     NumericString          initial_var_temp;
     NumericString          convergence_std;
     NumericString          initial_prog_temp;
     NumericString          prog_temp_rate;
     NumericString          prog_temp_max_value;
     NumericString          prog_temp_max_step_size;

     String                 t_profile_filename;

     void                   DrawShadow( );
     UINT16                 GetNumTransferObjects( ) const { return ( UINT16) transobj_list.Count(); }
     UINT16                 GetNumberOfSpecies( ) const { return ( UINT16) species_list.GetNumberOfSpecies(); }

public:

     // default constructor declared but not defined to prevent inadvertent use 

                    CompartmentObject( );

                    CompartmentObject( const String& rName, SchemeWindow* pParent );
                    CompartmentObject( const CompartmentObject& rCompt );
     virtual       ~CompartmentObject(  );

     CompartmentObject& operator = ( const CompartmentObject& rObject );

     // virtual methods

     virtual        ProcObjType  GetType( ) const { return PO_TYPE_COMPT; };
     virtual void   DrawObject( );
     virtual void   DrawActiveArea( );
     virtual ProcObjRC  EditContents ( );
     virtual BOOL   IsEmpty ( ) const;
     virtual UINT16 GetIndex( );

     // related to TransferObject manipulation 

     TransferObject* GetNextTransferObject( );
     TransferObject* GetFirstTransferObject( );
     void           AddTransferObject( TransferObject* pTO );
     void           DeleteTransferObject( TransferObject* pTO );
     void           RepositionTransferObjects( );

     // related to Reaction List

     UINT16         GetRxnListboxSelection() const { return (UINT16) rxn_listbox_selection; }
     void           SetRxnListboxSelection( UINT16 NewValue) { rxn_listbox_selection = NewValue; }
     BOOL           AddNewRxnStep( UINT16 Position );
     void           DeleteRxnStep( UINT16 Position );
     UINT16         GetNumberOfRxnSteps( ) const { return ( UINT16) rxn_list.Count(); }
     UINT16         GetNumberOfReversibleRxnSteps( ) const { return num_reversible_rxns; }
     const Reaction&   GetRxnStep( UINT16 i ) const { return *(rxn_list.GetObject( (ULONG) i ) ); }
     void           SetRxnStep( UINT16 i, const Reaction& rRxn ) { *(rxn_list.GetObject( (ULONG) i )) = rRxn; }

     // related to Species List 

     BOOL           UpdateSpeciesList( );
     virtual void   SynchSpeciesListWithDatabase( SpeciesDatabase& rSDB );

     // related to TPV options 

     enum TEMPERATURE_OPTION GetTemperatureOption ( ) const { return temperature_option; }
     enum VOLUME_OPTION      GetVolumeOption() const { return volume_option; }
     enum PRESSURE_OPTION    GetPressureOption() const { return pressure_option; }
     enum COMPARTMENT_PHASE_TYPE GetPhase( ) const { return phase; }

     void SetTemperatureOption ( enum TEMPERATURE_OPTION NewVal ) { temperature_option = NewVal; }
     void SetVolumeOption( enum VOLUME_OPTION  NewVal ) { volume_option = NewVal; }
     void SetPressureOption( enum PRESSURE_OPTION NewVal ) { pressure_option = NewVal; }
     void SetPhase( enum COMPARTMENT_PHASE_TYPE NewVal ) { phase = NewVal; }

     void SetXDimension( FLOAT64 Value ) { x_dimension = Value; }
     void SetYDimension( FLOAT64 Value ) { y_dimension = Value; }
     void SetZDimension( FLOAT64 Value ) { z_dimension = Value; }

     void SetVarVolumeInitialValue( const NumericString& rNewVal ) { initial_var_volume = rNewVal; }
     void SetConstVolumeValue( const NumericString& rNewVal ) { const_volume = rNewVal; }
     
     void SetConstTempValue( const NumericString& rNewVal ) { const_temp = rNewVal; }
     void SetVarTempInitialValue( const NumericString& rNewVal ) { initial_var_temp = rNewVal; }
     void SetVarTempConvergenceStd( const NumericString& rNewVal ) { convergence_std = rNewVal; }
     void SetProgTempInitialValue( const NumericString& rNewVal ) { initial_prog_temp = rNewVal; }
     void SetProgTempRate( const NumericString& rNewVal ) { prog_temp_rate = rNewVal; }
     void SetProgTempMaxValue( const NumericString& rNewVal ) { prog_temp_max_value = rNewVal; }
     void SetProgTempMaxStepSize( const NumericString& rNewVal ) { prog_temp_max_step_size = rNewVal; }
     void SetProgTempProfileName( const String& rNewVal ) { t_profile_filename = rNewVal; }
     
     const NumericString&  GetVarVolumeInitialValue( ) const { return initial_var_volume; }
     const NumericString&  GetConstVolumeValue( ) const { return const_volume; }
     
     const NumericString& GetConstTempValue( ) const { return const_temp; }
     const NumericString& GetVarTempInitialValue( ) const { return initial_var_temp; }
     const NumericString& GetVarTempConvergenceStd( ) const { return convergence_std; }
     const NumericString& GetProgTempInitialValue( ) const { return initial_prog_temp; }
     const NumericString& GetProgTempRate( ) const { return prog_temp_rate; }
     const NumericString& GetProgTempMaxValue( ) const { return prog_temp_max_value; }
     const NumericString& GetProgTempMaxStepSize( ) const { return prog_temp_max_step_size; }
     const String& GetProgTempProfileName( ) const { return t_profile_filename; }

     void  SetSpeciesConcentration( SPECIES_ID SpeciesID, const NumericString& NewConc );

     
     FLOAT64 GetXDimension( ) const { return x_dimension; }
     FLOAT64 GetYDimension( ) const { return y_dimension; }
     FLOAT64 GetZDimension( ) const { return z_dimension; }

     enum DIRECTION        GetUnconstrainedDirection( ) const { return unconstrained_direction; }
     void                  SetUnconstrainedDirection( enum DIRECTION NewDirection ) { unconstrained_direction = NewDirection; }
     FLOAT64 GetTotalMoles();

     virtual BOOL8       IsVisibleWithin( const Rectangle& rRect );

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentObject& rObject );
     friend BinaryInputStream& operator >>  ( BinaryInputStream& rBIS, CompartmentObject& rObject );

     friend TextOutputStream& operator << ( TextOutputStream& rTOS, CompartmentObject& rObject );
};

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentObject& rObject );
BinaryInputStream& operator >>  ( BinaryInputStream& rBIS, CompartmentObject& rObject );


class ClippedCompartmentObject : public CompartmentObject
{
protected:
     SpeciesDataList        species_data_list;

public:
                    ClippedCompartmentObject( const CompartmentObject& rCompt );
         virtual   ~ClippedCompartmentObject();

          UINT16     GetNumberOfSpeciesToPaste() const { return species_data_list.Count();} 

         const SpeciesDatabaseEntry& GetSpeciesData( UINT16 Position ) const { return *(species_data_list.GetObject( Position )); } 
};


#endif


