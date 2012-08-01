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
//  SPECLIST.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SpeciesList object. Each compartment contains 
//  a SpeciesList. A Species list maintains a list of all the species in 
//  a Compartment, their concentrations, and an index pointing to a specifid
//  SpeciesDatabase
//
//  CREATED : 1.24.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "speclist.hxx"
#include "mainapp.hxx"
#include "idtypes.hxx"

#endif



// ----------------------------------------------------------------------------
//               SpeciesListEntry::SpeciesListEntry( )
// ............................................................................
// 
//  default constructor for CompartmentSpeciesListEntry - initializes object
//
// ----------------------------------------------------------------------------

SpeciesListEntry::SpeciesListEntry( ) :
name(),
initial_conc( ),
species_index( INVALID_ID ),
name_in_use( FALSE ),
is_reactive( FALSE )
{

}



// ----------------------------------------------------------------------------
//               SpeciesListEntry::~SpeciesListEntry( )
// ............................................................................
// 
//  destructor for CompartmentSpeciesListEntry 
//
// ----------------------------------------------------------------------------

SpeciesListEntry::~SpeciesListEntry( ) 
{

}




// ----------------------------------------------------------------------------
//          SpeciesListEntry::SpeciesListEntry( )
// ............................................................................
// 
//  constructor for CompartmentSpeciesListEntry - initializes object
//
// ----------------------------------------------------------------------------

SpeciesListEntry::SpeciesListEntry( const String& rName ) :
name( rName ),
initial_conc( ),
species_index( INVALID_ID ),
name_in_use( TRUE ),
is_reactive( FALSE )
{
}

// ----------------------------------------------------------------------------
//         SpeciesListEntry( const SpeciesListEntry& rSDE ) 
// ............................................................................
// 
//  copy constructor for SpeciesListEntry - for passing by value 
//
// ----------------------------------------------------------------------------

SpeciesListEntry::SpeciesListEntry( const SpeciesListEntry& rSLE ) :
name( rSLE.name ),
initial_conc( rSLE.initial_conc ),
species_index( rSLE.species_index ),
name_in_use( rSLE.name_in_use ),
is_reactive( rSLE.is_reactive )
{
}





// ----------------------------------------------------------------------------
//               operator = SpeciesListEntry
// ............................................................................
// 
//  assignment operator for SpeciesListEntry
//
// ----------------------------------------------------------------------------

SpeciesListEntry&  SpeciesListEntry::operator = ( const SpeciesListEntry& rSLE )
{
     // avoid assignment to self 

     if ( &rSLE == this )
          return *this;

     // assign simple types 

     species_index   = rSLE.species_index;
     name_in_use     = rSLE.name_in_use;
     is_reactive     = rSLE.is_reactive;

     // String and NumericString types 

     name          = rSLE.name;
     initial_conc  = rSLE.initial_conc;

     return *this;
}





SpeciesListEntry* CompartmentSpeciesList::CreateNewListEntry( const String& rSpeciesName )
{
     SpeciesListEntry* prec = new SpeciesListEntry( rSpeciesName );

     if ( NULL == prec )

#if defined(__UNIT_TEST__)
          ;
#else
          GetpMainApp()->OutOfMemory();
#endif
     else
          num_species_in_use++;  // the added entry has in use flag set 

     return prec;
}





// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::CompartmentSpeciesList( )
// ............................................................................
// 
//   default constructor - initializes all objects
//
// ----------------------------------------------------------------------------

CompartmentSpeciesList::CompartmentSpeciesList( ) :
species_list( ),
num_species_in_use( 0 )
{
}








// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::CompartmentSpeciesList( )
// ............................................................................
// 
//   copy constructor - initializes all objects
//
// ----------------------------------------------------------------------------

CompartmentSpeciesList::CompartmentSpeciesList( const CompartmentSpeciesList& rList ) :
species_list( ),
num_species_in_use( rList.num_species_in_use )
{
     UINT16 count = rList.species_list.Count();
     for ( UINT16 i = 0; i < count; i++ )
     {
          SpeciesListEntry* p_spc = new SpeciesListEntry( *(rList.species_list.GetObject( i )) );

          if ( p_spc)
          {
               species_list.Insert( p_spc, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               return;
          }
     }
}





// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::operator =
// ............................................................................
// 
//   assignment operator 
//
// ----------------------------------------------------------------------------

CompartmentSpeciesList& CompartmentSpeciesList::operator = ( const CompartmentSpeciesList& rObject )
{
     // first clear out the existing list 

     UINT16 count = species_list.Count( );

     while ( count-- )
          delete species_list.GetObject( count );

     species_list.Clear();

     count = rObject.species_list.Count();

     for ( UINT16 i = 0; i < count; i++ )
     {
          SpeciesListEntry* p_spc = new SpeciesListEntry( *(rObject.species_list.GetObject( i )) );

          if ( p_spc)
          {
               species_list.Insert( p_spc, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               return *this;
          }
     }

     num_species_in_use = rObject.num_species_in_use;

     return *this;

}





// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::~CompartmentSpeciesList( )
// ............................................................................
// 
//   destructor for CompartmentSpeciesList - deletes objects listed in 
//   species_list
//
// ----------------------------------------------------------------------------

CompartmentSpeciesList::~CompartmentSpeciesList( )
{
     // do we have any entries in the list? if so work through 
     // and delete all invalid entries

     // work from the last entry to the first 

     ULONG position = species_list.Count( );

     while (position--)
          delete species_list.GetObject( position );
}





// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::InvalidateAllEntries( )
// ............................................................................
// 
//  this fcn invalidates the species_index all entries in the list. The 
//  intent is that the list is then rebuilt while analyzing a modified
//  reaction scheme. If a species is present in the reaction scheme, its InUse flag
//  is set to TRUE. After rebuilding the list any entries with their InUse flag
//  set to FALSE can safely be deleted. The purpose of all this is that we retain 
//  species concentration data in the list until it is certain that the species has been 
//  deleted from the reaction scheme by the user 
//
// ----------------------------------------------------------------------------

void CompartmentSpeciesList::InvalidateAllReactiveEntries( )
{
     // do we have any  entries in the list? if so invalidate all Reactive ones 

     SpeciesListEntry* p_entry = species_list.First();

     if ( NULL != p_entry )
     {
          if ( p_entry->IsReactive() )
          {
               species_list.GetCurObject()->SetInUseFlag( FALSE );
               num_species_in_use--;
          }

          // continue walk through list 

          p_entry = species_list.Next();

          while (  NULL != p_entry ) 
          {
               if ( p_entry->IsReactive() )
               {
                    species_list.GetCurObject()->SetInUseFlag( FALSE );
                    num_species_in_use--;
               }

               p_entry = species_list.Next();

          }
     }
}




// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::RemoveInvalidEntries( )
// ............................................................................
// 
//  this fcn removes from the list all entries with an invalid ID.
//  The function is to be called after first calling 
//  InvalidateAllEntries(),  followed by rebuilding the list 
//
// ----------------------------------------------------------------------------

void CompartmentSpeciesList::RemoveUnusedReactiveEntries( )
{
     // do we have any entries in the database? if so work through 
     // and delete all invalid Reactive entries

     // work from the last entry to the first 

     ULONG position = species_list.Count( );

     while (position--)
     {
          SpeciesListEntry* p_entry = species_list.GetObject( position );

          if ( ( p_entry->IsReactive() ) &&
               ( p_entry->GetInUseFlag() == FALSE )
             )
          {
               delete p_entry;
               species_list.Remove( position );   // delete current Object
          }
     }
}


void CompartmentSpeciesList::RemoveNonreactiveSpeciesWithZeroConc( )
{
     // do we have any entries in the database? if so work through 
     // and delete all Nonreactive entries with initial conc == 0.0

     // work from the last entry to the first 

     ULONG position = species_list.Count( );

     while (position--)
     {
          SpeciesListEntry* p_entry = species_list.GetObject( position );

          if ( p_entry->GetInUseFlag() && ( !p_entry->IsReactive() ) &&
               ( 0.0 == (FLOAT64) p_entry->GetInitialConcentration() )
             )
          {
               delete p_entry;
               species_list.Remove( position );   // delete current Object
               num_species_in_use--;
          }
     }
}



// ----------------------------------------------------------------------------
//               CompartmentSpeciesList::AddSpecies( )
// ............................................................................
// 
//  this fcn scans the existing list for the given species name. If it 
//  is found thern the fcn returns TRUE. Otherwise a new entry is created and
//  the name is added to the list ( which is maintained in alphabetical order )
//  if the new entry is successfully added to the list then TRUE is returned
//  otherwise FALSE is returned 
//
// ----------------------------------------------------------------------------

BOOL8 CompartmentSpeciesList::AddSpecies( const String& rSpeciesName, BOOL8 Reactivity )
{
     // first scan the existing list for a record with same name.
     // the list is arranged in alphabetical order but is not 
     // case sensitive 

     SpeciesListEntry* current_rec;

     ULONG count = species_list.Count();

     BOOL  insert_new_record = FALSE;

     if ( count > 0 )
     {
          for (ULONG i = 0; ( i < count ) && !insert_new_record; i++ )
          {
               current_rec = species_list.GetObject( i );

               // compare name in current_rec with search name ( case-insensitive )

               switch ( rSpeciesName.ICompare( current_rec->GetName() ) )
               {

               case COMPARE_EQUAL:

                    // found a match - indicate that the list entry is now in use
                    // and keep the count of valid list entries accurate 

                    if  ( current_rec->GetInUseFlag() == FALSE )
                    {
                         num_species_in_use++;
                         current_rec->SetInUseFlag( TRUE );

                    }

                    // here we set reactivity flag to true if this fcn 
                    // was called during reaction/reaction list analysis
                    // There is the possiblity that the species is already
                    // present in the list as a non-reactive species 

                    if ( TRUE == Reactivity )
                         current_rec->SetReactivity( TRUE );

                    return TRUE;


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

          SpeciesListEntry* new_rec = CreateNewListEntry( rSpeciesName ); 

          if ( NULL == new_rec )

               // exit with message and return FALSE if we fail to create object 

               return FALSE;
          else
          {
               // OK, now where do we insert it? IF insert_new_record is TRUE 
               // then we add it right in front of current_rec. Otherwise we 
               // append it at the end of the list 

               new_rec->SetReactivity( Reactivity );

               if ( insert_new_record )
                    species_list.Insert( new_rec, current_rec );
               else
                    species_list.Insert( new_rec, LIST_APPEND );

               return TRUE;
          }

     }  
     else
     {  
           // if count == 0 

          SpeciesListEntry* new_rec = CreateNewListEntry( rSpeciesName ); 

          if ( NULL == new_rec )

               // exit with message and return FALSE  if we fail to create object 

               return FALSE;
          else
          {
               // append new object at the end of the list 

               new_rec->SetReactivity( Reactivity );

               species_list.Insert( new_rec, LIST_APPEND );
          }

          return TRUE;
     }
}






// ----------------------------------------------------------------------------
//            CompartmentSpeciesList::GetpSpeciesListEntryFromName( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with a name 
//  matching the rSpeciesName parameter. If it finds it in the list, a pointer 
//  to that record is returned. If the name is not found then NULL is returned
//
// ----------------------------------------------------------------------------

SpeciesListEntry* CompartmentSpeciesList::GetpSpeciesListEntryFromName( const String& rSpeciesName ) const
{
     SpeciesListEntry* current_rec;

     ULONG count = species_list.Count();

     if ( count > 0 )
     {
          for (ULONG i = 0;  i < count;  i++ )
          {
               current_rec = species_list.GetObject( i );

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
//               CompartmentSpeciesList::GetpSpeciesListEntry( )
// ............................................................................
// 
//  this fcn scans the existing list looking for an entry with an ID             
//  matching the Index parameter. If it finds a match in the list, a pointer 
//  to the matching record is returned. If the name is not found then NULL is 
//  returned
//
// ----------------------------------------------------------------------------

SpeciesListEntry* CompartmentSpeciesList::GetpSpeciesListEntryFromSpeciesID( const SPECIES_ID ID ) const
{
     SpeciesListEntry* current_rec;

     ULONG count = species_list.Count();

     if ( count > 0 )
     {
          for (ULONG i = 0;  i < count;  i++ )
          {
               current_rec = species_list.GetObject( i );

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





const SpeciesListEntry&  CompartmentSpeciesList::GetSpeciesDataByName( const String& rSpeciesName ) const
{
     const SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromName( rSpeciesName );

     ASSERT (p_rec != NULL );

     return *p_rec;
}

const SpeciesListEntry&  CompartmentSpeciesList::GetSpeciesDataByID( const SPECIES_ID ID ) const
{
     const SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromSpeciesID( ID );

     ASSERT (p_rec != NULL );

     return *p_rec;
}



const SpeciesListEntry&  CompartmentSpeciesList::GetSpeciesDataByPosition( const UINT16 Position ) const
{
     const SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromPosition( Position );

     ASSERT (p_rec != NULL );

     return *p_rec;
}

                              
void  CompartmentSpeciesList::SetSpeciesDataByName( String& rSpeciesName, const SpeciesListEntry& rEntry )
{
     SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromName( rSpeciesName );

     ASSERT( NULL != p_rec );

     *p_rec = rEntry;
}

void  CompartmentSpeciesList::SetSpeciesDataByID( SPECIES_ID ID, const SpeciesListEntry& rEntry )
{
     SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromSpeciesID( ID );

     ASSERT( NULL != p_rec );

     *p_rec = rEntry;
}

void  CompartmentSpeciesList::SetSpeciesDataByPosition( const UINT16 Position, const SpeciesListEntry& rEntry )
{
     SpeciesListEntry*  p_rec = GetpSpeciesListEntryFromPosition( Position );

     ASSERT( NULL != p_rec );

     *p_rec = rEntry;
}


//----------------------------------------------------------------------------
//                        operator << CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for output of contents of a SpeciesListEntry object to BinaryOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SpeciesListEntry& rEntry )
{
     rBOS << IDType( SPECIES_LIST_ENTRY );
     rBOS << rEntry.name;
     rBOS << rEntry.initial_conc;
     rBOS << rEntry.species_index;
     rBOS << rEntry.is_reactive;
     rBOS << rEntry.name_in_use;

     return rBOS;
}



//----------------------------------------------------------------------------
//                        operator << CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for output of contents of a SpeciesListEntry object to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const SpeciesListEntry& rEntry )
{
     rTOS << IDType( COMPARTMENT_SPECIES_INFO ) << rEntry.species_index << 
      rEntry.initial_conc << EOL;

     return rTOS;
}



//----------------------------------------------------------------------------
//                        operator >> CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for input of contents of a SpeciesListEntry object from BinaryInputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SpeciesListEntry& rEntry )
{
     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SPECIES_LIST_ENTRY ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }

     rBIS >> rEntry.name;
     rBIS >> rEntry.initial_conc;
     rBIS >> rEntry.species_index;
     rBIS >> rEntry.is_reactive;
     rBIS >> rEntry.name_in_use;

     return rBIS;
}






//----------------------------------------------------------------------------
//                        operator << CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for output of contents of a CompartmentSpeciesList object to BinaryOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const CompartmentSpeciesList& rObject )
{
     UINT16 count = rObject.species_list.Count();

     ASSERT ( count == rObject.num_species_in_use );

     rBOS << IDType( SPECIES_LIST );

     rBOS << count;

     // we scan though the entire list but write out only 
     // those species list entries currently in use 


     for ( UINT16 i=0;  i < count; i++)
     {
          SpeciesListEntry* p_entry = rObject.species_list.GetObject(i);

          rBOS << *p_entry;
     }

     return rBOS;
}



//----------------------------------------------------------------------------
//                        operator << CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for output of contents of a CompartmentSpeciesList object to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const CompartmentSpeciesList& rObject )
{
     UINT16 count = rObject.species_list.Count();

     ASSERT ( count == rObject.num_species_in_use );

     rTOS << IDType( SPECIES_LIST ) << count << EOL;

     for ( UINT16 i=0; i < count; i++)
     {
          SpeciesListEntry* p_entry = rObject.species_list.GetObject(i);

          rTOS << *p_entry;
     }

     return rTOS;
}






//----------------------------------------------------------------------------
//                        operator >> CompartmentSpeciesList             
//............................................................................
//                                                                          
//  for input of contents of a CompartmentSpeciesList object from BinaryInputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, CompartmentSpeciesList& rObject )
{
     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SPECIES_LIST ) )
     {
          GetpMainApp()->InputFileCorrupt(rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }

     UINT16 count;
     rBIS >> count;

     rObject.num_species_in_use  = count;


     for ( UINT16 i=0;  i < count; i++)
     {
          // allocate a new entry for list for each species 

          SpeciesListEntry* ptr = new SpeciesListEntry;

          // is successful then initialize the object from the input stream 

          if ( NULL != ptr )
          {
               rBIS >> *ptr;

               if ( rBIS.FileError() )
                    return rBIS;

               // and then add the object to the end of the list

               rObject.species_list.Insert( ptr, LIST_APPEND );
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
     CompartmentSpeciesList db;
     String mnco6 = String("Mn(CO)6");

     BOOL test = db.IsEmpty();
     cout << "test = " << (int) test << "  (should be 1 )" << endl;

     // add some names to the species databas

     db.AddSpecies( String("ccccc") ); // #1
     db.AddSpecies( String("acccc") ); // #2
     db.AddSpecies( String("Ccccc") ); // same as #1
     db.AddSpecies( String("Zcccc") ); // #3 
     db.AddSpecies( String("Qwertyui") ); // #4
     db.AddSpecies( String("aaaaaaaa") ); // #5
     db.AddSpecies( String("B") ); // #6
     db.AddSpecies( mnco6 ); // #7
     test = db.AddSpecies( String("PMMA-R.")); // #8

     cout << "test = " << (int) test << "  (should be 1 )" << endl;
     cout << "NUMBER OF SPECIES = " << db.GetNumberOfSpecies( ) << 
             " ... *(should be 8)"  << endl;

     cout << "Get alphabetical list of species names..." << endl;

     for (UINT16 i = 0; i < db.GetNumberOfSpecies() ; i++ )
          cout << "Position " << i << " " << 
           (const char *) db.GetpSpeciesListEntryFromPosition( i )->GetName() << endl;
     
     // get some pointers from database and test entries 

     SpeciesListEntry*  ptr =  db.GetpSpeciesListEntryFromSpeciesID( 1 );

     cout << "ptr = " << (int) ptr << "  (should be 0 )" << endl;

     ptr = db.GetpSpeciesListEntryFromName( mnco6 );

     cout << "\nFor Species name = " << (const char *)  mnco6 << 
      "\n GetName = " << (const char *) ptr->GetName() <<
      "\n Initial Concentration = " <<  (const char *) ptr->GetInitialConcentration() << endl;


#if 0
     // these should cause compile-time errors -xtor and dtor are private  
    delete ptr;
    SpeciesListEntry*  p_sle = new SpeciesListEntry;
#endif

     cout << "\n\nModifying data...\nfor species " << (const char *) ptr->GetName() << endl;
     ptr->SetInitialConcentration( NumericString( "1.234e-5") );

     cout <<  "\n Initial Concentration = " <<  (const char *) ptr->GetInitialConcentration();

     cout << " (should be 1.234e-5)" << endl;


     // now invalidata the database and re-validate 

     db.InvalidateAllEntries( );

     test = db.AddSpecies( String("Zcccc") );
     cout << "test = " << (int) test << "  (should be 1 )" << endl;
     test = db.AddSpecies( String("ccccc") );
     cout << "test = " << (int) test << "  (should be 1 )" << endl;
     test = db.AddSpecies( String("Ccccc") );
     cout << "test = " << (int) test << "  (should be 1 )" << endl;
     test = db.AddSpecies( mnco6 ); // 
     cout << "test = " << (int) test << "  (should be 1 )" << endl;

     db.RemoveUnusedEntries( );

     cout << "NUMBER OF SPECIES = " << db.GetNumberOfSpecies( ) << 
     "  (should be 3)" << endl;

     cout << "\n\nTesting data after db revalidate...\nfor species = " << 
      (const char *) ptr->GetName() << endl;  

     cout <<  "\n Initial Concentration = " <<  (const char *) ptr->GetInitialConcentration();

     cout << " (should be 1.234e-5)" << endl;

     ptr = db.GetpSpeciesListEntryFromName( String("ccccc") );

     cout << "\n\nTesting data after db revalidate...\nfor species = " << 
      (const char *) ptr->GetName() << endl;  

     cout <<  "\n Initial Concentration = " <<  (const char *) ptr->GetInitialConcentration();
     cout << " (should be 0)" << endl;



}


#endif


