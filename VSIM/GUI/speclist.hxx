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
//  SPECLIST.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the SpeciesList object. Each compartment contains 
//  a SpeciesList. A Species list maintains a list of all the species in 
//  a Compartment, their concentrations, and an index pointing to a specifid
//  SpeciesDatabase
//
//  CREATED : 2.06.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__SPECLIST_HXX__)
#define __SPECLIST_HXX__

#include <iostream.h>

#include "specdb.hxx"
#include "numstrng.hxx"
#include "vsimtype.hxx"
#include "specid.hxx"
#include "txtstrem.hxx"

class SpeciesListEntry
{
     friend class CompartmentSpeciesList;   // so that it can access  SetID method

private:
     String                 name;
     NumericString          initial_conc;
     SPECIES_ID             species_index;       // unique ID in SpeciesDatabase
     BOOL8                  name_in_use;
     BOOL8                  is_reactive;

                            SpeciesListEntry( const String& rName );


     void                   SetInUseFlag ( BOOL NewValue ){ name_in_use = NewValue; }
     BOOL8                  GetInUseFlag ( ) const { return name_in_use; }

     void                   SetReactivity ( BOOL NewValue ){ is_reactive = NewValue; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesListEntry& rEntry );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesListEntry& rEntry );

     friend TextOutputStream& operator << ( TextOutputStream& rBOS, const SpeciesListEntry& rEntry );

public:
                            SpeciesListEntry();
                            SpeciesListEntry( const SpeciesListEntry& rSDE );
                           ~SpeciesListEntry();

     SpeciesListEntry&  operator = ( const SpeciesListEntry& rSDE );

     const String&          GetName( ) const { return name; }
     void                   SetName( String& rNewName ) { name = rNewName; }

     void                   SetID ( SPECIES_ID NewID ){ species_index = NewID; }
     SPECIES_ID             GetID ( ) const { return species_index; }

     void                   SetInitialConcentration( const NumericString& rNewStr ) { initial_conc = rNewStr; }
     const NumericString&   GetInitialConcentration( ) const { return initial_conc; }

     BOOL8                  IsReactive ( ) const { return is_reactive; }

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentSpeciesList& rObject );
     friend TextOutputStream&  operator << ( TextOutputStream& rTOS, const CompartmentSpeciesList& rObject );
};


DECLARE_LIST( SpeciesList, SpeciesListEntry* )

class CompartmentSpeciesList
{
private:
     SpeciesList            species_list;
     UINT16                 num_species_in_use;

     SpeciesListEntry*      CreateNewListEntry( const String& rSpeciesName );

     SpeciesListEntry*      GetpSpeciesListEntryFromName( const String& rSpeciesName ) const;
     SpeciesListEntry*      GetpSpeciesListEntryFromSpeciesID( const SPECIES_ID ID ) const;
     SpeciesListEntry*      GetpSpeciesListEntryFromPosition( const UINT16 Position ) const
                            { return ( species_list.GetObject( Position ) ); }

     BOOL8                  AddSpecies( const String& rSpeciesName, BOOL8 IsReactive );

public:
                            CompartmentSpeciesList( );
                            CompartmentSpeciesList( const CompartmentSpeciesList& rList );
                           ~CompartmentSpeciesList( );

     CompartmentSpeciesList& operator = ( const CompartmentSpeciesList& rObject );


     void                   InvalidateAllReactiveEntries( );
     void                   RemoveUnusedReactiveEntries( );
     void                   RemoveNonreactiveSpeciesWithZeroConc( );
     BOOL8                  AddReactiveSpecies( const String& rSpeciesName ) { return AddSpecies( rSpeciesName, TRUE ); }
     BOOL8                  AddNonreactiveSpecies( const String& rSpeciesName ) { return AddSpecies( rSpeciesName, FALSE ); } 
     BOOL8                  NameIsInList( const String& rName)
                            { return (BOOL8) ( NULL != GetpSpeciesListEntryFromName(rName) ); }

     const SpeciesListEntry&  GetSpeciesDataByName( const String& rSpeciesName ) const;
     const SpeciesListEntry&  GetSpeciesDataByID( const SPECIES_ID ID ) const;
     const SpeciesListEntry&  GetSpeciesDataByPosition( const UINT16 Position ) const;
                                   
     void  SetSpeciesDataByName( String& rSpeciesName, const SpeciesListEntry& rEntry );
     void  SetSpeciesDataByID( SPECIES_ID ID, const SpeciesListEntry& rEntry );
     void  SetSpeciesDataByPosition( const UINT16 Position, const SpeciesListEntry& rEntry );


     UINT16                 GetNumberOfSpecies( ) const { return num_species_in_use; }
     BOOL8                  IsEmpty() { return (BOOL8) ( species_list.Count() == 0 ); }
     
     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentSpeciesList& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, CompartmentSpeciesList& rObject );
     friend TextOutputStream&  operator << ( TextOutputStream& rTOS, const CompartmentSpeciesList& rObject );
};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentSpeciesList& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, CompartmentSpeciesList& rObject );

TextOutputStream& operator << ( TextOutputStream& rTOS, const CompartmentSpeciesList& rObject );




#endif

