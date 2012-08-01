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
//  SPECDB.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SpeciesDatabase object. Each SchemeWindow contains  
//  one SpeciesDatabase. A SpeciesDatabase contains global information about each
//  species in the reaction scheme, such as thermochem and physical state info
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "specdb.hxx"
#include "mainapp.hxx"
#include "idtypes.hxx"

#endif



const char* FP_ONE = "1.0";
const char* FP_ZERO = "0.0";


// ----------------------------------------------------------------------------
//               SpeciesDatabaseEntry::SpeciesDatabaseEntry( )
// ............................................................................
// 
//  default constructor for SpeciesDatabaseEntry - initializes object
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry::SpeciesDatabaseEntry( ) :
unique_id( 0 ),
used( FALSE ),
name(),
molar_density( FP_ONE ),
physical_state( SOLID_STATE ),
enthalpy( FP_ZERO ),
therm_coeffA( FP_ZERO ),
therm_coeffB( FP_ONE ),
therm_coeffC( FP_ZERO ),
therm_coeffD( FP_ZERO )
{
}



// ----------------------------------------------------------------------------
//               SpeciesDatabaseEntry::SpeciesDatabaseEntry( String& rName )
// ............................................................................
// 
//  constructor for SpeciesDatabaseEntry - initializes object
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry::SpeciesDatabaseEntry( const String& rName ) :
unique_id( 0 ),
used( FALSE ),
name( rName ),
molar_density( FP_ONE ),
physical_state( SOLID_STATE ),
enthalpy( FP_ZERO ),
therm_coeffA( FP_ZERO ),
therm_coeffB( FP_ONE ),
therm_coeffC( FP_ZERO ),
therm_coeffD( FP_ZERO )
{
}




// ----------------------------------------------------------------------------
//         SpeciesDatabaseEntry( const SpeciesDatabaseEntry& rSDE ) 
// ............................................................................
// 
//  copy constructor for SpeciesDatabaseEntry - for passing by value 
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry::SpeciesDatabaseEntry( const SpeciesDatabaseEntry& rSDE ) :
unique_id( rSDE.unique_id ),
used( rSDE.used ),
name( rSDE.name ),
molar_density( rSDE.molar_density ),
physical_state( rSDE.physical_state ),
enthalpy( rSDE.enthalpy ),
therm_coeffA( rSDE.therm_coeffA ),
therm_coeffB( rSDE.therm_coeffB ),
therm_coeffC( rSDE.therm_coeffC ),
therm_coeffD( rSDE.therm_coeffD )
{
}



// ----------------------------------------------------------------------------
//               operator = SpeciesDatabaseEntry
// ............................................................................
// 
//  assignment operator for SpeciesDatabaseEntry
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry&  SpeciesDatabaseEntry::operator = ( const SpeciesDatabaseEntry& rSDE )
{
     // avoid assignment to self 

     if ( &rSDE == this )
          return *this;



     // these items are specifically excluded - the purpose of the assignment
     // operator is such that we want to assign only the data members that are
     // directly modifiable by the user
     // these are not
     //    unique_id       = rSDE.unique_id;
     //    name            = rSDE.name;         
     //    used            = rSDE.used;

     // assigne simple types 

     physical_state  = rSDE.physical_state;

     // String and NumericString types 

     molar_density = rSDE.molar_density;
     enthalpy      = rSDE.enthalpy;
     therm_coeffA  = rSDE.therm_coeffA;
     therm_coeffB  = rSDE.therm_coeffB;
     therm_coeffC  = rSDE.therm_coeffC;
     therm_coeffD  = rSDE.therm_coeffD;

     return *this;
}

// ----------------------------------------------------------------------------
//               SpeciesDatabaseEntry::~SpeciesDatabaseEntry( )
// ............................................................................
// 
//  destructor for SpeciesDatabaseEntry 
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry::~SpeciesDatabaseEntry( )
{
}




// ----------------------------------------------------------------------------
//               SpeciesDatabaseEntry::SpeciesDatabaseEntry( )
// ............................................................................
// 
//   default constructor for SpeciesDatabaseEntry - initializes all objects
//
// ----------------------------------------------------------------------------

SpeciesDatabase::SpeciesDatabase( ) :
species_data_list(),
num_species( 0 )
{
}


// ----------------------------------------------------------------------------
//               SpeciesDatabaseEntry::~SpeciesDatabaseEntry( )
// ............................................................................
// 
//   destructor for SpeciesDatabaseEntry - deletes objects listed in 
//   species_data_list
//
// ----------------------------------------------------------------------------

SpeciesDatabase::~SpeciesDatabase( ) 
{
     // do we have any entries in the database? if so work through 
     // and delete all entries

     // work from the last entry to the first 

     ULONG position = species_data_list.Count( );

     while (position--)
          delete species_data_list.GetObject( position );
}



// ----------------------------------------------------------------------------
//               SpeciesDatabase::InvalidateAllEntries( )
// ............................................................................
// 
//  this fcn invalidates the unique ID for all entries in the database. The 
//  intent is that the database is then rebuilt while analyzing a modified
//  reaction scheme. If a species is present in the reaction scheme, its  ID
//  is set to a valid value. After rebuilding the database any entries still with 
//  an invalid ID can be deleted. The purpose of all this is that we retain 
//  species data in the database until it is certain the species has been 
//  removed from the reaction scheme 
//
// ----------------------------------------------------------------------------

void SpeciesDatabase::InvalidateAllEntries( )
{
     // do we have any entries in the database? if so invalidate all 

     if ( species_data_list.First() )
     {
          species_data_list.GetCurObject()->SetID( INVALID_ID );

          while ( species_data_list.Next() )
               species_data_list.GetCurObject()->SetID( INVALID_ID );
     }

     num_species = 0;
}




// ----------------------------------------------------------------------------
//               SpeciesDatabase::RemoveInvalidEntries( )
// ............................................................................
// 
//  this fcn removes from the database all entries with an invalid ID.
//  The function is to be called after first calling 
//  InvalidateAllEntries(),  followed by rebuilding the database
//
// ----------------------------------------------------------------------------

void SpeciesDatabase::RemoveInvalidEntries( )
{
     // do we have any entries in the database? if so work through 
     // and delete all invalid entries

     // work from the last entry to the first 

     ULONG position = species_data_list.Count( );

     while (position--)
     {
          SpeciesDatabaseEntry* p_entry = species_data_list.GetObject( position );

          if (  p_entry->GetID() == INVALID_ID )
          {
               delete p_entry;
               species_data_list.Remove( position );   // delete currentObject
          }
     }
}



// ----------------------------------------------------------------------------
//               SpeciesDatabase::CreateNewDatabaseEntry( )
// ............................................................................
// 
//  this fcn creates and initializes a new SpeciesDatabaseEntry
//  return ptr to the new object if successul, displays error msg and returns
//  NULL is failure during creation
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry* SpeciesDatabase::CreateNewDatabaseEntry( const String& rSpeciesName )
{
     SpeciesDatabaseEntry* prec = new SpeciesDatabaseEntry( rSpeciesName );

     if ( NULL == prec )

#if defined(__UNIT_TEST__)
          ;
#else
          GetpMainApp()->OutOfMemory();
#endif
     else
     {
          // set the ID member in the new object and increment count of species

          num_species++;
          prec->SetID( num_species );
     }

     return prec;
}



// ----------------------------------------------------------------------------
//               SpeciesDatabase::GetSpeciesID( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a name 
//  matching the rSpeciesName parameter. If it finds it in the list, a valid 
//  ID is assigned to that entry if necessary and the ID is returned. If the 
//  name was not found then a new entry is added to the list with that name. The
//  new entry is givne a valid ID which is returned. If an error occurs the 
//  value INVALID_ID is returned.
//
// ----------------------------------------------------------------------------

SPECIES_ID SpeciesDatabase::GetSpeciesID( const String& rSpeciesName )
{
     // first scan the existing database for a record with same name.
     // the database is arranged in alphabetical order but is not 
     // case sensitive 

     SpeciesDatabaseEntry* current_rec;

     ULONG count = species_data_list.Count();

     BOOL  insert_new_record = FALSE;

     if ( count > 0 )
     {
          for (ULONG i = 0; ( i < count ) && !insert_new_record; i++ )
          {
               current_rec = species_data_list.GetObject( i );

               // compare name in current_rec with search name ( case-insensitive )

               switch ( rSpeciesName.ICompare( current_rec->GetName() ) )
               {

               case COMPARE_EQUAL:

                    // found a match - if valid ID then return, otherwise 
                    // get a valid ID

                    if  ( current_rec->GetID() == INVALID_ID )
                    {
                         num_species++;
                         current_rec->SetID( num_species );
                    }

                    return current_rec->GetID();


               case COMPARE_LESS:

                    // still scanning - don't stop 

                    continue;


               case COMPARE_GREATER:

                    // we are the past the place the record would have been if present
                    // it's not here, so we will need to add a new one in front 
                    // of current_rec

                    insert_new_record = TRUE;
                    break;
               }
          }

          // if we reach here then we need to add a new record to list, 
          // either in front of current_rec or at the end of the list


          // first create and initialize a new object

          SpeciesDatabaseEntry* new_rec = CreateNewDatabaseEntry( rSpeciesName ); 

          if ( NULL == new_rec )

               // exit with message and return invalid value if we fail to create object 

               return INVALID_ID;
          else
          {
               // OK, now where do we insert it? IF insert_new_record is TRUE 
               // then we add it right in front of current_rec. Otherwise we 
               // append it at the end of the list 

               if ( insert_new_record )
                    species_data_list.Insert( new_rec, current_rec );
               else
                    species_data_list.Insert( new_rec, LIST_APPEND );

               return new_rec->GetID( );
          }

     }  
     else
     {  
           // if count == 0 

          SpeciesDatabaseEntry* new_rec = CreateNewDatabaseEntry( rSpeciesName ); 

          if ( NULL == new_rec )

               // exit with message and return invalid value if we fail to create object 

               return INVALID_ID;
          else
               // append new object at the end of the list 

                    species_data_list.Insert( new_rec, LIST_APPEND );

          return new_rec->GetID( );
     }
}






// ----------------------------------------------------------------------------
//               SpeciesDatabase::GetSpeciesPtrByName( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a name 
//  matching the rSpeciesName parameter. If it finds it in the list, a 
//  pointer to that record is returned. If the name is not found or the 
//  list is empty NULL is returned 
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry*  SpeciesDatabase::GetSpeciesPtrByName( const String& rSpeciesName )
{
     SpeciesDatabaseEntry* current_rec;

     ULONG count = species_data_list.Count();

     if ( count > 0 )
     {
          for (ULONG i = 0;  i < count;  i++ )
          {
               current_rec = species_data_list.GetObject( i );

               // compare name in current_rec with search name ( case-insensitive )

               switch ( rSpeciesName.ICompare( current_rec->GetName() ) )
               {

               case COMPARE_EQUAL:

                    // found a match - send it back 

                    return current_rec;


               case COMPARE_LESS:

                    // still scanning - don't stop 

                    continue;


               case COMPARE_GREATER:

                    // we are the past the place the record would have been if present
                    // it's not here, so we return NULL

                    return NULL;
               }
          }
     }

     // if we get here we went through th whole list w/o finding match 
     // or the list was empty so we return NULL

     return NULL;
}






// ----------------------------------------------------------------------------
//               SpeciesDatabase::GetSpeciesDataByName( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a name 
//  matching the rSpeciesName parameter. If it finds it in the list, a 
//  reference to that record is returned. If the name is not found or the 
//  list is empty we should not have called this function -- it's a 
//  logic error  and we abort 
//
// ----------------------------------------------------------------------------

const SpeciesDatabaseEntry&  SpeciesDatabase::GetSpeciesDataByName( const String& rSpeciesName )
{
     SpeciesDatabaseEntry* p_rec = GetSpeciesPtrByName( rSpeciesName );

     ASSERT( NULL != p_rec );

     return *p_rec;
}





// ----------------------------------------------------------------------------
//               SpeciesDatabase::GetSpeciesDataByID( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with an ID
//  matching the ID parameter. If it finds it in the list, a reference to that
//  record is returned. If the ID is not found or the list is empty
//  we should not have called this function -- it's a logic error 
//  and we abort 
//
// ----------------------------------------------------------------------------

const SpeciesDatabaseEntry&  SpeciesDatabase::GetSpeciesDataByID( const SPECIES_ID ID )
{
     SpeciesDatabaseEntry* p_rec = GetSpeciesPtrByID( ID );

     ASSERT( NULL != p_rec );

     return *p_rec;
}





// ----------------------------------------------------------------------------
//               SpeciesDatabase::SetSpeciesDataByName( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a name 
//  matching the rSpeciesName parameter. If it finds it in the list, it is 
//  assigned the new values contained in rEntry. If the name is not found or the 
//  list is empty we should not have called this function -- it's a 
//  logic error  and we abort 
//
// ----------------------------------------------------------------------------

void  SpeciesDatabase::SetSpeciesDataByName( const String& rSpeciesName, const SpeciesDatabaseEntry& rEntry )
{
     SpeciesDatabaseEntry* p_rec = GetSpeciesPtrByName( rSpeciesName );

     ASSERT( NULL != p_rec );

     *p_rec = rEntry;
}



// ----------------------------------------------------------------------------
//               SpeciesDatabase::SetSpeciesDataByID( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a unique_id
//  matching the ID parameter. If it finds it in the list, it is 
//  assigned the new values contained in rEntry. If the name is not found or the 
//  list is empty we should not have called this function -- it's a 
//  logic error  and we abort 
//
// ----------------------------------------------------------------------------

void  SpeciesDatabase::SetSpeciesDataByID( SPECIES_ID ID, const SpeciesDatabaseEntry& rEntry )
{
     SpeciesDatabaseEntry* p_rec = GetSpeciesPtrByID( ID );

     ASSERT( NULL != p_rec );

     *p_rec = rEntry;
}







// ----------------------------------------------------------------------------
//            SpeciesDatabase::GetSpeciesPtrByID( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with an ID             
//  matching the Index parameter. If it finds a match in the list, a pointer 
//  to the matching record is returned. If the name is not found then NULL is 
//  returned
//
// ----------------------------------------------------------------------------

SpeciesDatabaseEntry*  SpeciesDatabase::GetSpeciesPtrByID( const SPECIES_ID ID )
{
     SpeciesDatabaseEntry* current_rec;

     ULONG count = species_data_list.Count();

     if ( count > 0 )
     {
          for (ULONG i = 0;  i < count;  i++ )
          {
               current_rec = species_data_list.GetObject( i );

               // compare name in current_rec with search name ( case-insensitive )

               // found a match - return ptr 

               if ( ID == current_rec->GetID() )
                    return current_rec;
          }
     }

     // if we get here we went through the whole list w/o finding match 
     // or the list was empty so we return NULL 

     return NULL;
}


const SpeciesDatabaseEntry&  SpeciesDatabase::GetSpeciesDataByPosition( const UINT16 Position )
{
     const SpeciesDatabaseEntry*  p_rec = GetpSpeciesDataRecordFromPosition( Position );

     ASSERT (p_rec != NULL );

     return *p_rec;

}






// ----------------------------------------------------------------------------
//            operator << SpeciesDatabaseEntry
// ............................................................................
// 
// provides for writing the contents of a SpeciesDatabaseEntry to a 
// BinaryOutputStream
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabaseEntry& rObject )
{
     rBOS << IDType( SPECIES_DB_ENTRY );

     rBOS << rObject.unique_id;
     rBOS << rObject.name;
     rBOS << rObject.molar_density;
     rBOS << (UINT16) rObject.physical_state;

     rBOS << rObject.enthalpy;
     rBOS << rObject.therm_coeffA;
     rBOS << rObject.therm_coeffB;
     rBOS << rObject.therm_coeffC;
     rBOS << rObject.therm_coeffD;

     return rBOS;
}


// ----------------------------------------------------------------------------
//            operator << SpeciesDatabaseEntry
// ............................................................................
// 
// provides for writing the contents of a SpeciesDatabaseEntry to a 
// TextOutputStream
//
// ----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabaseEntry& rObject )
{
     rTOS << IDType( SPECIES_DATA_TABLE_ENTRY ) << rObject.unique_id <<
      rObject.molar_density << (UINT16) rObject.physical_state <<
      rObject.enthalpy  << rObject.therm_coeffA << rObject.therm_coeffB <<
      rObject.therm_coeffC << rObject.therm_coeffD << EOL;

     return rTOS;
}





// ----------------------------------------------------------------------------
//            operator >> SpeciesDatabaseEntry
// ............................................................................
// 
// provides for reading the contents of a SpeciesDatabaseEntry from a
// BinaryInputStream
//
// ----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabaseEntry& rObject )
{
     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SPECIES_DB_ENTRY ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }

     rBIS >> rObject.unique_id;
     rBIS >> rObject.name;
     rBIS >> rObject.molar_density;

     UINT16 temp_val;

     rBIS >> temp_val;
     rObject.physical_state = (enum PHYSICAL_STATE) temp_val;

     rBIS >> rObject.enthalpy;
     rBIS >> rObject.therm_coeffA;
     rBIS >> rObject.therm_coeffB;
     rBIS >> rObject.therm_coeffC;
     rBIS >> rObject.therm_coeffD;

     return rBIS;
}


// ----------------------------------------------------------------------------
//            operator << SpeciesDatabase
// ............................................................................
// 
// provides for writing the contents of a SpeciesDatabase to a 
// BinaryOutputStream
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesDatabase& rObject )
{
     UINT16 num_species = rObject.GetNumberOfSpecies();

     rBOS << IDType( SPECIES_DATABASE );
     rBOS << num_species;

     for (UINT16 i = 0; i < num_species; i++ )
     {
          SpeciesDatabaseEntry* ptr = rObject.GetpSpeciesDataRecordFromPosition( i );

          rBOS << *ptr;
     }

     return rBOS;
}





// ----------------------------------------------------------------------------
//            operator << SpeciesDatabase
// ............................................................................
// 
// provides for writing the contents of a SpeciesDatabase to a 
// TextOutputStream
//
// ----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesDatabase& rObject )
{
     UINT16 num_species = rObject.GetNumberOfSpecies();

     rTOS << IDType( START_SPECIES_DATA_TABLE_INIT ) << EOL;
     rTOS << IDType( NUM_SPECIES_IN_DATA_TABLE ) << num_species << EOL;

     for (UINT16 i = 0; i < num_species; i++ )
     {
          SpeciesDatabaseEntry* ptr = rObject.GetpSpeciesDataRecordFromPosition( i );

          rTOS << *ptr;
     }

     rTOS << IDType( END_SPECIES_DATA_TABLE_INIT ) << EOL;

     return rTOS;
}




// ----------------------------------------------------------------------------
//            operator >> SpeciesDatabase
// ............................................................................
// 
// provides for reading the contents of a SpeciesDatabase from a
// BinaryInputStream
//
// ----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesDatabase& rObject )
{
     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SPECIES_DATABASE ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }


     UINT16 num_species;
     rBIS >> num_species;

     rObject.num_species = num_species;

     // now read in each database entry in order 

     for (UINT16 i = 0; i < num_species; i++ )
     {
          SpeciesDatabaseEntry* ptr = new SpeciesDatabaseEntry;

          if ( ptr )
          {
               // initialize from input stream

               rBIS >> *ptr;

               // and add to 
               rObject.species_data_list.Insert( ptr, LIST_APPEND );
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



#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>

void main()
{
     SpeciesDatabase db;

     BOOL test = db.IsEmpty();
     cout << "Test = " << (int) test << "  (should be 1)" << endl;

     // add some names to the species databas

     SPECIES_ID idc = db.GetSpeciesID( String("ccccc") ); // #1
     SPECIES_ID ida = db.GetSpeciesID( String("acccc") ); // #2
     SPECIES_ID idC = db.GetSpeciesID( String("Ccccc") ); // same as #1
     SPECIES_ID idZ = db.GetSpeciesID( String("Zcccc") ); // #3 
     SPECIES_ID idQ = db.GetSpeciesID( String("Qwertyui") ); // #4
     SPECIES_ID idaa = db.GetSpeciesID( String("aaaaaaaa") ); // #5
     SPECIES_ID idB = db.GetSpeciesID( String("B") ); // #6
     SPECIES_ID idMn = db.GetSpeciesID( String("Mn(CO)6")); // #7
     SPECIES_ID idP = db.GetSpeciesID( String("PMMA-R.")); // #8

     cout << "NUMBER OF SPECIES = " << db.GetNumberOfSpecies( ) << 
             " ... *(should be 8)"  << endl;

     test = (idC == idc);
     cout << "Test = " << (int) test << "  (should be 1)" << endl;

     test = (idC == idZ);
     cout << "Test = " << (int) test << "  (should be 0)" << endl;



     cout << "Get alphabetical list of species names..." << endl;

     for (UINT16 i = 0; i < db.GetNumberOfSpecies() ; i++ )
          cout << "Position " << i << " " << 
           (const char *) db.GetSpeciesDataByPosition( i ).GetName() << endl;
     
     // get some pointers from database and test entries 

     for ( i = 1; i <= db.GetNumberOfSpecies() ; i++ )
     {
          cout << "\nFOR Species ID = " << i << 
          "\nname = " << (const char *)  db.GetSpeciesDataByID( i ).GetName() <<
          "\nPhysical State = " <<  db.GetSpeciesDataByID( i ).GetPhysicalState() <<
          "\nMolarDensity = " << (const char *) db.GetSpeciesDataByID( i ).GetMolarDensity() <<
          "\nEnthalpy = " << (const char *) db.GetSpeciesDataByID( i ).GetEnthalpy() <<
          "\nCoeffA = " << (const char *) db.GetSpeciesDataByID( i ).GetThermCoeffA() <<
          "\nCoeffB = " << (const char *) db.GetSpeciesDataByID( i ).GetThermCoeffB() <<
          "\nCoeffC = " << (const char *) db.GetSpeciesDataByID( i ).GetThermCoeffC() <<
          "\nCoeffD = " << (const char *) db.GetSpeciesDataByID( i ).GetThermCoeffC() <<  
          endl;

     }

     SpeciesDatabaseEntry entryc = db.GetSpeciesDataByID( idc );
     SpeciesDatabaseEntry entryZ = db.GetSpeciesDataByName( String("Zcccc") );

     cout << "\n\nModifying data...\nfor species id 1" << endl;
     cout << "name = " << ( const char *) entryc.GetName( ) << endl;
     cout << "Enthalpy =" << ( const char *) entryc.GetEnthalpy( ) << endl;

     // now modify the values and try again 

     entryc.SetEnthalpy( NumericString("111.0") );
     cout << "Enthalpy =" << ( const char *) entryc.GetEnthalpy( ) <<
      "  (should be 111.0)" << endl;

     cout << "for species id 3" << endl;
     cout << "name = " << ( const char *) entryZ.GetName( ) << endl;

     cout << "MolarDensity =" << ( const char *) entryZ.GetMolarDensity( ) << endl;

     entryZ.SetMolarDensity( NumericString("15.0") );
     cout << "MolarDensity =" << ( const char *) entryZ.GetMolarDensity( ) << 
     "  (should be 15.0)" <<endl;


     // now invalidata the database and re-validate 
     db.InvalidateAllEntries( );

     SPECIES_ID idZ2 = db.GetSpeciesID( String("Zcccc") );
     SPECIES_ID idc2 = db.GetSpeciesID( String("ccccc") );
     SPECIES_ID idC2 = db.GetSpeciesID( String("Ccccc") );

     db.RemoveInvalidEntries( );

     cout << "NUMBER OF SPECIES = " << db.GetNumberOfSpecies( ) << 
     "  (should be 2)" << endl;

     cout << "\n\nTesting data after db revalidate...\nfor species id 1" <<  endl;
     cout << "name = " << ( const char *) entryc.GetName( ) << "  (should be \"ccccc\")"  << endl;
     cout << "Enthalpy =" << ( const char *) entryc.GetEnthalpy( ) <<
      "  (should be 111.0)" << endl;


     cout << "for species id 2" << endl;
     cout << "name = " << ( const char *) entryZ.GetName( ) << "(should be \"Zcccc\")" << endl;
     cout << "MolarDensity =" << ( const char *) entryZ.GetMolarDensity( ) << 
     "  (should be 15.0)" <<endl;




     cout << "\n\nTesting assignment operator ...\nfor species id idZ2" <<  endl;

     entryc = db.GetSpeciesDataByID( idZ2 ); 

     cout << "initial values of entryc:" << endl;

          cout << 
          "\nname = " << (const char *)          entryc.GetName() <<
          "\nPhysical State = " <<               entryc.GetPhysicalState() <<
          "\nMolarDensity = " << (const char *)  entryc.GetMolarDensity() <<
          "\nEnthalpy = " << (const char *)      entryc.GetEnthalpy() <<
          "\nCoeffA = " << (const char *)        entryc.GetThermCoeffA() <<
          "\nCoeffB = " << (const char *)        entryc.GetThermCoeffB() <<
          "\nCoeffC = " << (const char *)        entryc.GetThermCoeffC() <<
          "\nCoeffD = " << (const char *)        entryc.GetThermCoeffC() <<  
          endl;

     cout << "initial values of entryZ:" << endl;

          cout << 
          "\nname = " << (const char *)          entryZ.GetName() <<
          "\nPhysical State = " <<               entryZ.GetPhysicalState() <<
          "\nMolarDensity = " << (const char *)  entryZ.GetMolarDensity() <<
          "\nEnthalpy = " << (const char *)      entryZ.GetEnthalpy() <<
          "\nCoeffA = " << (const char *)        entryZ.GetThermCoeffA() <<
          "\nCoeffB = " << (const char *)        entryZ.GetThermCoeffB() <<
          "\nCoeffC = " << (const char *)        entryZ.GetThermCoeffC() <<
          "\nCoeffD = " << (const char *)        entryZ.GetThermCoeffC() <<  
          endl;

     entryZ.SetThermCoeffA( NumericString( "2.2222") );
     entryZ.SetThermCoeffB( NumericString( "2.2222") );
     entryZ.SetThermCoeffC( NumericString( "2.2222") );
     entryZ.SetThermCoeffD( NumericString( "2.2222") );

     cout << "new values of entryZ:" << endl;

          cout << 
          "\nname = " << (const char *)          entryZ.GetName() <<
          "\nPhysical State = " <<               entryZ.GetPhysicalState() <<
          "\nMolarDensity = " << (const char *)  entryZ.GetMolarDensity() <<
          "\nEnthalpy = " << (const char *)      entryZ.GetEnthalpy() <<
          "\nCoeffA = " << (const char *)        entryZ.GetThermCoeffA() <<
          "\nCoeffB = " << (const char *)        entryZ.GetThermCoeffB() <<
          "\nCoeffC = " << (const char *)        entryZ.GetThermCoeffC() <<
          "\nCoeffD = " << (const char *)        entryZ.GetThermCoeffC() <<  
          endl;



     db.SetSpeciesDataByID( idZ2, entryZ );

     entryc = db.GetSpeciesDataByID( idZ2 );

     cout << "new values of entryc after assigning entryZ to entryc :" << endl;
          cout << 
          "\nname = " << (const char *)          entryc.GetName() <<
          "\nPhysical State = " <<               entryc.GetPhysicalState() <<
          "\nMolarDensity = " << (const char *)  entryc.GetMolarDensity() <<
          "\nEnthalpy = " << (const char *)      entryc.GetEnthalpy() <<
          "\nCoeffA = " << (const char *)        entryc.GetThermCoeffA() <<
          "\nCoeffB = " << (const char *)        entryc.GetThermCoeffB() <<
          "\nCoeffC = " << (const char *)        entryc.GetThermCoeffC() <<
          "\nCoeffD = " << (const char *)        entryc.GetThermCoeffC() <<  
          endl;

}



#endif


