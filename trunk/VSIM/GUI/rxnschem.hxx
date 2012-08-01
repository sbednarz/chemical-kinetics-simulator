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
//  RXNSCHEME.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the ReactionScheme class, containing the basic
// components of a reaction scheme.
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__RXNSCHEM_HXX__)
#define __RXNSCHEM_HXX__



#include <sv.hxx>

#include "procsobj.hxx"
#include "comptobj.hxx"
#include "xferobj.hxx"
#include "notestrg.hxx"
#include "childprc.hxx"
#include "strings.hrc"

#include "results.hxx"

DECLARE_LIST( CompartmentObjectList, CompartmentObject* )

class SchemeWindow;


enum CHANGE_CONC_RC   { CC_NO_ERROR, CC_INVALID_FILE, 
                        CC_INVALID_VALUE, CC_FILE_IO_ERROR, CC_FILE_LENGTH_ERROR,
                        CC_OTHER_ERROR };

// ----------------------------------------------------------------------------
//                                class ReactionScheme
// ............................................................................
//
//  This class contain the elements that define a reaction scheme 
//
// ----------------------------------------------------------------------------

class ReactionScheme 
{

protected:

     String                  full_name;
     String                  short_name;
     String                  runparm_filename;
     CompartmentObjectList   compartment_list;
     TransferObjectList      transfer_list;  
  
     SpeciesDatabase         species_database;
     NotebookString          notebook_str;

     NumericString           num_mols;
     NumericString           record_interval;
     NumericString           random_num_seed;
     NumericString           max_num_events;
     NumericString           elapsed_time_limit;
     NumericString           equil_cycle_count;
     NumericString           min_equil_efficiency;

     Time                    cumulative_simulation_time;
     Time                    step_simulation_time;
     DateTime                file_creation_date;
     DateTime                scheme_last_modification_date;
     DateTime                notebook_last_modification_date;
     DateTime                last_simulation_start_date;

     BOOL8                   equil_detect_active;
     BOOL8                   scheme_data_modified_since_last_save;
     BOOL8                   species_list_has_been_modified;
     BOOL8                   notebook_data_modified_since_last_save;
     BOOL8                   conv_factor_p_to_mole_is_valid;

     ChildProcess            simulator;
     SIM_STATUS_CODE         simulator_status_code;
     INT32                   simulation_data_file_offset;
     INT32                   simulation_state_data_offset;
     UINT32                  num_simulation_data_records;
     FLOAT64                 conv_factor_particles_to_moles;

     enum ELAPSED_TIME_UNITS timebase_units;
     SimulationResult*       p_results;
     SchemeWindow*           p_window;

     void                    CalcConvFactorParticlesToMoles();

public:
                             ReactionScheme( SchemeWindow* pWindow );
                            ~ReactionScheme();  
  
     const String&           GetName( ) const { return full_name;}
     void                    SetName( const String& rNewName );

     void                    SetSimulatorName( const String& rNewName );
  
     const String&           GetNickName( ) { return short_name;}

     const String&           GetRunParameterFileName( ) const { return simulator.GetCommandLineParameters(); }
     void                    SetRunParameterFileName( const String& rNewName ) { simulator.SetCommandLineParameters( rNewName ); }
     BOOL8                   RunSimulation( );
     BOOL8                   AbortSimulation();

     const NotebookString&   GetNotebookText( ) const { return notebook_str;}
     BOOL8                   IsNotebookTextModified() const { return notebook_data_modified_since_last_save; }
     BOOL8                   WriteNotebookTextToFile();

     void                    SetNotebookText(const String& rNewText  );
         
     BOOL8                   LoadResults( ); 
     SimulationResult*       GetpResults( ) { return p_results; }

     void                    DrawAllObjects( const Rectangle& rVisibleRect );

    const NumericString&     GetNumMols() const { return num_mols; }
    const NumericString&     GetRecordInterval() const { return record_interval; } 
    const NumericString&     GetRandomNumSeed() const { return random_num_seed; } 
    const NumericString&     GetMaxNumEvents () const { return max_num_events; } 
    const NumericString&     GetElapsedTimeLimit() const { return elapsed_time_limit; }
    const NumericString&     GetEquilCycleCount() const { return equil_cycle_count; }
    const NumericString&     GetMinEquilEfficiency() const { return min_equil_efficiency; } 
    const Time&              GetCumulativeSimulationTime() const { return cumulative_simulation_time; }
    const Time&              GetLastStepSimulationTime() const { return step_simulation_time; }
    BOOL8                    IsEquilDetectActive() const { return equil_detect_active; }
    INT32                    GetSimulationStateFileOffset() const { return simulation_state_data_offset; }

    const DateTime&          GetFileCreationDate() const { return file_creation_date; }
    const DateTime&          GetSchemeModificationDate() const { return scheme_last_modification_date; }
    const DateTime&          GetNotebookModificationDate() const { return notebook_last_modification_date; }
    const DateTime&          GetSimulationStartDate() const  { return last_simulation_start_date; }

    void                     SetNumMols( const NumericString& rNewVal )  { num_mols = rNewVal; }
    void                     SetRecordInterval( const NumericString& rNewVal )  { record_interval = rNewVal; }
    void                     SetRandomNumSeed( const NumericString& rNewVal ) { random_num_seed = rNewVal; } 
    void                     SetMaxNumEvents ( const NumericString& rNewVal ) { max_num_events = rNewVal; }
    void                     SetElapsedTimeLimit( const NumericString& rNewVal ) { elapsed_time_limit = rNewVal; }
    void                     SetEquilCycleCount( const NumericString& rNewVal ) { equil_cycle_count = rNewVal; }
    void                     SetMinEquilEfficiency( const NumericString& rNewVal ) { min_equil_efficiency = rNewVal; }
    void                     SetEquilDetectActive( const BOOL8 NewState ) { equil_detect_active = NewState; }

    void                     SetSpeciesDataModifiedFlag( const BOOL8 NewState ) { species_list_has_been_modified = NewState; }

    SIM_STATUS_CODE          GetStatus() const { return simulator_status_code; }
    const String&           GetStatusStr( );

    void                     SetStatus( const SIM_STATUS_CODE NewStatus );


    UINT16                   GetNumCompartments( ) const
                             { return (UINT16) compartment_list.Count();}


    UINT16                   GetNumTransferObjects( ) const
                             { return (UINT16) transfer_list.Count();}

    BOOL8                    SchemeDataIsModified() const { return ( SIM_DATA_MODIFIED ==  simulator_status_code ); }


    INT32                    GetSimulationDataFilePosition( ) const { return simulation_data_file_offset; }
    UINT32                   GetNumSimulationDataPoints() const { return num_simulation_data_records; }

    SpeciesDatabase*         GetpSpeciesDatabase() { return &species_database; }

    UINT16                   GetTransferObjectIndexFromPtr( TransferObject* pTO ) const
                             { return transfer_list.GetPos( pTO ); }

    UINT16                   GetCompartmentObjectIndexFromPtr( CompartmentObject* pCO ) const
                             { return compartment_list.GetPos( pCO ); }

    TransferObject*          GetTransferObjectPtrFromIndex( UINT16 Index ) const
                             { return transfer_list.GetObject( Index ); }

    CompartmentObject*       GetCompartmentObjectPtrFromIndex( UINT16 Index ) const
                             { return compartment_list.GetObject(Index ); }

    CompartmentObject*       GetCompartmentObjectPtrFromName( const String& rName );
        
    ProcessObject*           GetProcessObjectPtrFromLocation( const Point& rPoint );

    void                     ReconstructSpeciesDatabase( );
        
    void                     AddCompartmentObject( CompartmentObject* pCompartment )
                                                 { compartment_list.Insert( pCompartment, LIST_APPEND ); }

    CompartmentObject*       RemoveCompartmentObject( CompartmentObject* pComptObj )
                             { return compartment_list.Remove( pComptObj );}
                                                         
    void                     AddTransferObject( TransferObject* pTO )
                             { transfer_list.Insert( pTO, LIST_APPEND ); }
                                                         
     TransferObject*         RemoveTransferObject( TransferObject* pTransObj )
                             {  return transfer_list.Remove( pTransObj );} 
                                                         
     const String&           GetUniqueCompartmentName( );
     const String&           GetUniqueTransferObjectName( );

     void                    DiscardResults( );

     BOOL8                   UpdateSchemeAfterSimulation( );

     FLOAT64                 GetConvFactorParticlesToMoles();
     FLOAT64                 GetConvFactorMolesToParticles() { return ( 1.0 / GetConvFactorParticlesToMoles() ); }

     const String            GetTimeUnits() const;
     const String            GetAmountUnits() const;
     const String            GetConcentrationUnits() const;
     const String            GetVolumeUnits() const;
     const String            GetPressureUnits() const;
     const String            GetTemperatureUnits() const;
     const String            GetDistanceUnits() const;
     const String            GetRateConstantUnits() const;
     const String            GetDiffusionCoefficientUnits() const;
     const String            GetSmallEnergyUnits() const;
     const String            GetLargeEnergyUnits() const;

     FLOAT64                 GetConversionFactorDimensionsToVolume();
     FLOAT64                 GetTotalInitialMoles();

     void                    GetCompartmentDimensions( FLOAT64& CurrentXCellDimension,
                                                       FLOAT64& CurrentYCellDimension,
                                                       FLOAT64& CurrentZCellDimension );

     void                    SetCompartmentDimensions( FLOAT64 NewXCellDimension,
                                                       FLOAT64 NewYCellDimension,
                                                       FLOAT64 NewZCellDimension );

     void                    ChangeAllCompartmentDimensions();

     void                    SetSchemeToConstantTemperature( const NumericString& NewTemperature );

     void                    ChangeSchemeTemperature();

     void                    SetSpeciesConcToValue( const String& rSpeciesName, const NumericString& NewConc );
     enum CHANGE_CONC_RC     SetSpeciesConcUsingFileTable( const String& rSpeciesName, const String& InputFile );

     void                    ChangeSpeciesConcentration();

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, ReactionScheme& rObject );
     friend TextOutputStream& operator   << ( TextOutputStream& rTOS, const ReactionScheme& rObject );
     friend BinaryInputStream& operator  >> ( BinaryInputStream& rBIS, ReactionScheme& rObject );

};  


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, ReactionScheme& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const ReactionScheme& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, ReactionScheme& rObject );

extern const DateTime INVALID_DATE;

#endif

