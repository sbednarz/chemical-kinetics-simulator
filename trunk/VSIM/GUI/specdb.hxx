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
//  SPECDB.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the SpeciesDatabase object. Each SchemeWindow contains  
//  one SpeciesDatabase. A SpeciesDatabase contains global information about each
//  species in the reaction scheme, such as thermochem and physical state info
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__SPECDB_HXX__)
#define __SPECDB_HXX__

#include "vsimtype.hxx"
#include "numstrng.hxx"
#include "specid.hxx"

#include "assert.hxx"


class TextOutputStream;

const UINT16  INVALID_ID = 0;

enum PHYSICAL_STATE { GAS_STATE, LIQUID_STATE, SOLID_STATE };

class SpeciesDatabaseEntry
{
     friend class SpeciesDatabase;   // so that it can access  SetID method

private:
     SPECIES_ID           unique_id;
     BOOL8                used;
     String               name;
     NumericString        molar_density;
     enum PHYSICAL_STATE  physical_state;

     NumericString        enthalpy;
     NumericString        therm_coeffA;
     NumericString        therm_coeffB;
     NumericString        therm_coeffC;
     NumericString        therm_coeffD;

     void            SetID ( SPECIES_ID NewID ){ unique_id = NewID; }
     void            SetName( String& rNewName ) { name = rNewName; }

     // note that the constructors and destructor are private 
     // so that only friends of this class can create and destroy 
     // SpeciesDatabaseEntry objects 

                     SpeciesDatabaseEntry( const String& rName );

public:
                     SpeciesDatabaseEntry( );
                     SpeciesDatabaseEntry( const SpeciesDatabaseEntry& rSDE );
                    ~SpeciesDatabaseEntry( );


     SpeciesDatabaseEntry&  operator = ( const SpeciesDatabaseEntry& rSDE );


     SPECIES_ID      GetID ( ) const { return unique_id; }
     const String&   GetName( ) const { return name; }

     void            SetMolarDensity( NumericString& rNewStr ) { molar_density = rNewStr; }
     const NumericString&   GetMolarDensity( ) const { return molar_density; }

     void                 SetPhysicalState( enum PHYSICAL_STATE NewState ) { physical_state = NewState; }
     enum PHYSICAL_STATE  GetPhysicalState( ) const { return physical_state; }

     const NumericString&   GetEnthalpy( ) const { return enthalpy; }
     void            SetEnthalpy( NumericString& rNewStr ) { enthalpy = rNewStr; }

     const NumericString&   GetThermCoeffA( ) const { return therm_coeffA; }
     void            SetThermCoeffA( NumericString& rNewStr ) { therm_coeffA = rNewStr; }

     const NumericString&   GetThermCoeffB( ) const { return therm_coeffB; }
     void            SetThermCoeffB( NumericString& rNewStr ) { therm_coeffB = rNewStr; }

     const NumericString&   GetThermCoeffC( ) const { return therm_coeffC; }
     void            SetThermCoeffC( NumericString& rNewStr ) { therm_coeffC = rNewStr; }

     const NumericString&   GetThermCoeffD( ) const { return therm_coeffD; }
     void            SetThermCoeffD( NumericString& rNewStr ) { therm_coeffD = rNewStr; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabaseEntry& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabaseEntry& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabaseEntry& rObject );
};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabaseEntry& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabaseEntry& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabaseEntry& rObject );




DECLARE_LIST( SpeciesDataList, SpeciesDatabaseEntry* )


class SpeciesDatabase
{
private:
     SpeciesDataList        species_data_list;
     UINT16                 num_species;

     SpeciesDatabaseEntry* CreateNewDatabaseEntry( const String& rSpeciesName );

     SpeciesDatabaseEntry*  GetpSpeciesDataRecordFromPosition( UINT16 Position ) const
                            { return ( species_data_list.GetObject( (ULONG) Position ) ); }

     SpeciesDatabaseEntry*  GetSpeciesPtrByName( const String& rSpeciesName );

     SpeciesDatabaseEntry*  GetSpeciesPtrByID( const SPECIES_ID ID );

public:
                            SpeciesDatabase( );
                           ~SpeciesDatabase( );

     BOOL                   IsEmpty() { return ( species_data_list.Count() == 0 ); }

     BOOL8                  ContainsEntryFor( const String& rSpeciesName ) { return (BOOL8) ( NULL != GetSpeciesPtrByName( rSpeciesName ) ); }
     
     void                   InvalidateAllEntries( );
     void                   RemoveInvalidEntries( );
     SPECIES_ID             GetSpeciesID( const String& rSpeciesName );

     const SpeciesDatabaseEntry&  GetSpeciesDataByName( const String& rSpeciesName );
     const SpeciesDatabaseEntry&  GetSpeciesDataByID( const SPECIES_ID ID );
     const SpeciesDatabaseEntry&  GetSpeciesDataByPosition( const UINT16 Position );
                                   

     void  SetSpeciesDataByName( const String& rSpeciesName, const SpeciesDatabaseEntry& rEntry );
     void  SetSpeciesDataByID( SPECIES_ID ID, const SpeciesDatabaseEntry& rEntry );

     UINT16                 GetNumberOfSpecies( ) const { return num_species; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabase& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabase& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabase& rObject );

};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabase& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabase& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabase& rObject );


#endif

