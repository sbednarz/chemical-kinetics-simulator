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
//  RXNSCHEME.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This module defins the ReactionScheme class, containing the basic
// components of a reaction scheme.
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "rxnschem.hxx"
#include "results.hxx"
#include "mainapp.hxx"

#endif

#include <limits.h>

#include "strings.hrc"
#include "icons.hrc"



                             
const DateTime INVALID_DATE( Date( 1, 1, 1800), Time(0) );






void ReactionScheme::SetStatus( const SIM_STATUS_CODE NewStatus )
{ 
     simulator_status_code = NewStatus; 

     if ( NewStatus == SIM_DATA_MODIFIED )
     {
         conv_factor_p_to_mole_is_valid = FALSE;
         num_simulation_data_records = 0;
         cumulative_simulation_time  = Time( 0 );
         step_simulation_time        = Time( 0 );  
         last_simulation_start_date = INVALID_DATE;

     }

     GetpMainApp()->UpdateMenuActivation( p_window->GetpFrame());
}



// ----------------------------------------------------------------------------
//                   ReactionScheme::GetConvFactorParticlesToMoles()
// ............................................................................
//
//  returns the conversion factor needed to convert simulation results
//  from particle units to mole units. The conversion is made by multiplying
// the amount in particle units by the value returned by this function
//  
//
// ----------------------------------------------------------------------------

FLOAT64 ReactionScheme::GetConvFactorParticlesToMoles()
{
     if ( !conv_factor_p_to_mole_is_valid )
          CalcConvFactorParticlesToMoles();

     ASSERT ( 0.0 != conv_factor_particles_to_moles );

     return conv_factor_particles_to_moles;
}







// ----------------------------------------------------------------------------
//                   ReactionScheme::GetTotalInitialMoles()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

FLOAT64 ReactionScheme::GetTotalInitialMoles()
{
     FLOAT64 amount_sum = 0.0;

     UINT16 num_compartments = GetNumCompartments();

     for ( UINT16 i = 0; i < num_compartments; i++ )
          amount_sum += compartment_list.GetObject( i )->GetTotalMoles();

     return amount_sum;
}







// ----------------------------------------------------------------------------
//                   ReactionScheme::CalcConvFactorParticlesToMoles()
// ............................................................................
//
//  calculates the conversion factor needed to convert simulation results
//  from particle units to mole units. The conversion is made by multiplying
//  the amount in particle units by the conversion factor calculated here
//
// ----------------------------------------------------------------------------

void ReactionScheme::CalcConvFactorParticlesToMoles()
{
     conv_factor_particles_to_moles = GetTotalInitialMoles()  / (FLOAT64) num_mols;

     conv_factor_p_to_mole_is_valid = TRUE;
}





// ----------------------------------------------------------------------------
//                        ReactionScheme::RunSimulation()
// ............................................................................
//
//  start the simulation 
//
// ----------------------------------------------------------------------------

BOOL8  ReactionScheme::RunSimulation( ) 
{ 

     last_simulation_start_date = DateTime();

     SetStatus( SIM_RUNNING );

     p_window->GetpFrame()->UpdateStatusBar();  

     simulator.SendEventOnCompletion( TRUE );
     simulator.SetCompletionEvent( vEVT_UPDATE_AFTER_SIMULATION );
     simulator.SetpCompletionData( this );

     if ( simulator.RunWithoutWait() )
     {
          SetStatus( SIM_RUNNING );
          return TRUE;
     }
     else
          return FALSE;
}






// ----------------------------------------------------------------------------
//                        ReactionScheme::UpdateSchemeAfterSimulation( )
// ............................................................................
//
//  updates ReactionSchemeObject from rxn file - to be called immediately
//  after the simulation completes 
//
// ----------------------------------------------------------------------------

BOOL8  ReactionScheme::UpdateSchemeAfterSimulation( ) 
{
     BOOL8  error_condition = FALSE; 

     {
          BinaryInputStream  infile( GetName() );
     
          if ( !infile.IsOpen() )
          {
               GetpMainApp( )->FileOpenError( GetName() );
               error_condition = TRUE;
          }
          else
          {
               infile.MoveTo( SIMNUM_POINTS_FILE_POSITION );
          
               infile >> num_simulation_data_records;
               infile >> simulation_data_file_offset;
               infile >> simulation_state_data_offset;
          
               UINT16 temp16;
          
               infile >> temp16;
          
               // get last simulator status code -- change of necessary
          
               simulator_status_code = (enum SIM_STATUS_CODE) temp16;
          
               if ( infile.FileError( ) )
                    error_condition = TRUE;
          }
     }


     if ( !error_condition )
     {
          // now reopen file for writing - we are going to save the 
          // execution time data 

          BinaryOutputStream outfile( GetName() );

          if ( !outfile.IsOpen() )
          {
               GetpMainApp( )->FileOpenError( GetName() );
               error_condition = TRUE;
          }

          step_simulation_time = simulator.GetExecutionTime();

          cumulative_simulation_time += step_simulation_time;

          outfile.MoveTo( SIMCUMULATIVE_SIMULATION_TIME_POSITION );

          outfile << cumulative_simulation_time;

          outfile.MoveTo( SIMSTEP_SIMULATION_TIME_POSITION );

          outfile << step_simulation_time;

          outfile.MoveTo( SIMLAST_SIMULATION_TIMESTAMP_POSITION );
	     outfile <<  last_simulation_start_date;

          if ( outfile.FileError( ) )
               error_condition = TRUE;
     }

          

     p_window->Enable();

     p_window->GetpFrame()->SetStandardIcon();;
     p_window->GetpFrame()->Restore();
     p_window->GetpFrame()->UpdateStatusBar();
     p_window->GetpFrame()->UpdateToolbarActivations( );    

     return !error_condition;
}






// ----------------------------------------------------------------------------
//                        ReactionScheme::WriteNotebookTextToFile()
// ............................................................................
//
//  updates notebook text in reaction file and reset notebook_modified flag
//
// ----------------------------------------------------------------------------

BOOL8 ReactionScheme::WriteNotebookTextToFile()
{
     // pen file for writing - we are going to save the 
     // notebook text data 

     BinaryOutputStream outfile( GetName() );

     if ( !outfile.IsOpen() )
     {
          GetpMainApp( )->FileOpenError( GetName() );
          return FALSE;
     }

     // set notebook timestamp to current time 

     notebook_last_modification_date = DateTime();

     // write timestamp to file 

     outfile.MoveTo( SIMLAST_NOTEBOOK_TIMESTAMP_POSITION );
	outfile << notebook_last_modification_date;     

     // then write notebook text to file 

     outfile.MoveTo( SIM_NOTEBOOK_TEXT_POSITION );

     outfile << GetNotebookText( ); 

     if ( outfile.FileError( ) )
          return FALSE;
     else
     {
          // reset modified flag 

          notebook_data_modified_since_last_save = FALSE;
          return TRUE;
     }
}





// ----------------------------------------------------------------------------
//                        ReactionScheme::ReactionScheme( )
// ............................................................................
//
//  constructor 
//
// ----------------------------------------------------------------------------

ReactionScheme::ReactionScheme( SchemeWindow* pWindow ):
full_name( ),
short_name( ResId( vID_STR_SCHEME_DEFAULT_NAME ) ),
runparm_filename(),
compartment_list(1024, 128, 128),  // trial values
transfer_list(1024, 128, 128),
species_database(),
notebook_str(),
num_mols( ResId( vID_STR_DEFAULT_MAX_NUM_MOLS ) ),
record_interval( ResId( vID_STR_DEFAULT_RECORD_INTERVAL ) ),
random_num_seed( ResId( vID_STR_DEFAULT_RANDOM_NUM_SEED ) ),
max_num_events( ResId( vID_STR_DEFAULT_MAX_NUM_EVENTS ) ),
elapsed_time_limit( ResId( vID_STR_DEFAULT_ELAPSED_TIME_LIMIT ) ),
equil_cycle_count( ResId( vID_STR_DEFAULT_EQUIL_CYCLE_COUNT ) ),
min_equil_efficiency( ResId( vID_STR_DEFAULT_MIN_EQUIL_EFFICIENCY ) ),
cumulative_simulation_time(0),
step_simulation_time(0),
file_creation_date( Date(), Time() ),
scheme_last_modification_date( (Date) INVALID_DATE, (Time) INVALID_DATE ),
notebook_last_modification_date( (Date) INVALID_DATE, (Time) INVALID_DATE ), 
last_simulation_start_date( (Date) INVALID_DATE, (Time) INVALID_DATE ),
equil_detect_active( TRUE ),
scheme_data_modified_since_last_save( TRUE ),
species_list_has_been_modified( TRUE ),
notebook_data_modified_since_last_save( TRUE ),
conv_factor_p_to_mole_is_valid( FALSE ),
simulator(),
simulator_status_code( SIM_NEW ),
simulation_data_file_offset( 0 ),
simulation_state_data_offset( 0 ),
num_simulation_data_records( 0 ),
conv_factor_particles_to_moles( 0.0 ),
timebase_units( TIME_UNITS_SEC ),
p_results( NULL ),
p_window( pWindow )
{
}


// ----------------------------------------------------------------------------
//                         SchemeWindow::SetSimulatorName( )
// ............................................................................
//
//  takes a filename, coinverts it to a fully qualified name and sets
//  the simulator to use that filename as the exetuable engine 
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetSimulatorName( const String& rName )
{
     DirEntry engine_name = DirEntry( GetpMainApp()->GetAppFileName() );

     engine_name.ChangeName( rName );

     simulator.SetProgramName( engine_name.GetFull() );

//     simulator.SetProcessCompleteHandler( &::SimulationCompleteHandler );
}




// ----------------------------------------------------------------------------
//                    ReactionScheme::GetUniqueCompartmentName()
// ............................................................................
//
//  creates a compartment name that is different from any currently in use
//  in the reaction scheme. 
//
// ----------------------------------------------------------------------------

const String& ReactionScheme::GetUniqueCompartmentName( )
{
     const UINT16 MAXNUM_COMPTS = USHRT_MAX;
     INT32 i = 1;
     BOOL8 got_one = FALSE;
     UINT16 num_compartments = GetNumCompartments( );

     static String title; 

     do 
     {
          // construct the name 

          title = String( ResId( vID_STR_DEFAULT_COMPT_STEM ) ) + String( i );

          //see if it already exists 

          BOOL8 already_there = FALSE;

          for (UINT16 j = 0 ; j < num_compartments; j++) 
          {
             if ( COMPARE_EQUAL == title.ICompare( GetCompartmentObjectPtrFromIndex( j )->GetName() ) )
                  already_there = TRUE;
          } 

          got_one = !already_there;

          i++;

     } while ( !got_one && ( i < MAXNUM_COMPTS ) );

     return title;
}





// ----------------------------------------------------------------------------
//                    ReactionScheme::GetUniqueTransferObjectName()
// ............................................................................
//
//  creates a compartment name that is different from any currently in use
//  in the reaction scheme. 
//
// ----------------------------------------------------------------------------

const String& ReactionScheme::GetUniqueTransferObjectName( )
{
     const UINT16 MAXNUM_XFER_OBJS = USHRT_MAX;
     INT32 i = 1;
     BOOL8 got_one = FALSE;
     UINT16 num_xfer_objs = GetNumTransferObjects( );

     static String title; 

     do 
     {
          // construct the name 

          title = String( ResId( vID_STR_DEFAULT_XFEROBJ_STEM ) ) + String( i );

          //see if it already exists 

          BOOL8 already_there = FALSE;

          for (UINT16 j = 0 ; j < num_xfer_objs; j++) 
          {
             if ( COMPARE_EQUAL == title.ICompare( GetTransferObjectPtrFromIndex( j )->GetName() ) )
                  already_there = TRUE;
          } 

          got_one = !already_there;

          i++;

     } while ( !got_one && ( i < MAXNUM_XFER_OBJS ) );

     return title;
}






// ----------------------------------------------------------------------------
//                        ReactionScheme::DrawAllObjects( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::DrawAllObjects( const Rectangle& rVisibleRect )
{
     if ( compartment_list.First() )
     {
          CompartmentObject* pobj = compartment_list.GetCurObject();

          if ( pobj->IsVisibleWithin( rVisibleRect ) )
               pobj->DrawObject();

          while ( compartment_list.Next() )
          {
               pobj = compartment_list.GetCurObject();

               if ( pobj->IsVisibleWithin( rVisibleRect ) )
                    pobj->DrawObject();
          }
     }

     // same with the transfer object list

     if ( transfer_list.First() )
     {
          TransferObject* pobj = transfer_list.GetCurObject();

          if ( pobj->IsVisibleWithin( rVisibleRect ) )
               pobj->DrawObject();

          while ( transfer_list.Next() )
          {
               pobj = transfer_list.GetCurObject();
     
               if ( pobj->IsVisibleWithin( rVisibleRect ) )
                    pobj->DrawObject();
          }
     }
}        





// ----------------------------------------------------------------------------
//                         SchemeWindow::GetProcessObjectPtrFromLocation( )
// ............................................................................
//
//  checks the lists of all objects in the window to see if we are over one
//  If so the pointer to that object is returned, otherwise NULL is returned
//
// ----------------------------------------------------------------------------

ProcessObject* ReactionScheme::GetProcessObjectPtrFromLocation( const Point& rPoint )
{
     if ( compartment_list.First() )
     {
          CompartmentObject* pco;

          while ( NULL != (pco = compartment_list.GetCurObject() )  )
          {
               if ( pco->ObjectHit( rPoint ) )
                    return  pco;               // got the hit - return immediately
               else
                   if ( !compartment_list.Next( ) )
                        break;
          }
     }

     if ( transfer_list.First() )
     {
          TransferObject* pto;
          while ( NULL != ( pto = transfer_list.GetCurObject() ) )
          {
               if ( pto->ObjectHit( rPoint ) )
                    return  pto;               // got the hit - return immediately
               else
                   if ( !transfer_list.Next( ) )
                        break;
          }
     }

     // if we got to here we had no hits - return NULL;

     return NULL;
}





// ----------------------------------------------------------------------------
//                        ReactionScheme::LoadResults( )
// ............................................................................
//
//  load results from file if not already in memory - return TREU if we have 
//  results loaded
//
// ----------------------------------------------------------------------------

BOOL8 ReactionScheme::LoadResults( ) 
{
     if ( NULL == p_results )
     {
          GetpMainApp()->Wait( TRUE );
          p_results = new SimulationResult( this );
          GetpMainApp()->Wait( FALSE );
     }
                           
     return ( NULL != p_results );
}                     




// ----------------------------------------------------------------------------
//                        ReactionScheme::~ReactionScheme( )
// ............................................................................
//
//  destructor 
//
// ----------------------------------------------------------------------------

ReactionScheme::~ReactionScheme()
{
     // have we created any transfer objects?  If so then delete them first
     // since doing so will remove them from the lists within the Source and
     // Target Compartment objects

     if ( transfer_list.First() )
     {
          delete transfer_list.GetCurObject();

          while ( transfer_list.Next() )
               delete transfer_list.GetCurObject();
     }

     // now delete any compartments we have created

     if ( compartment_list.First() )
     {
          delete compartment_list.GetCurObject();

          while ( compartment_list.Next() )
               delete compartment_list.GetCurObject();
     }
     
	DiscardResults();
}








// ----------------------------------------------------------------------------
//                         SchemeWindow::SetName( )
// ............................................................................
//
//  takes a fully qualified filename and derives from it a short name. Updates
//  internal data appropriately
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetName( const String& rName )
{
     full_name = rName;
     short_name = DirEntry( full_name).GetName( );
}





// ----------------------------------------------------------------------------
//                         SchemeWindow::SetNotebookText( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetNotebookText( const String& rNewText )
{
     notebook_str = rNewText;
     notebook_data_modified_since_last_save = TRUE;
}







// ----------------------------------------------------------------------------
//                   ReactionScheme::GetCompartmentObjectPtrFromName()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

CompartmentObject*  ReactionScheme::GetCompartmentObjectPtrFromName( const String& rName )
{
     compartment_list.First();

     CompartmentObject* pco;

     while ( NULL != (pco = compartment_list.GetCurObject() )  )
     {
          if ( pco->GetName() == rName )
                    return  pco;               // found it - return immediately
               else
                   compartment_list.Next( );
     }

     // should not get here - we ran off the end of list w/o finding name

     ASSERT( 0 );
}






// ----------------------------------------------------------------------------
//                     ReactionScheme::ReconstructSpeciesDatabase( )
// ............................................................................
//
//  this function rebuilds the species database after the reaction scheme
//  has been modified
//
// ----------------------------------------------------------------------------

void ReactionScheme::ReconstructSpeciesDatabase( )
{
     // we first invalidate the database, then go through the compartments
     // one-by-one, adding each species to the database and updating the
     // species indices used within the compartments along the way

     species_database.InvalidateAllEntries( );

     UINT16 num_compartments = GetNumCompartments( );

     // now get species databse and compartment species lists
     // synchronized

     for ( UINT16 i = 0; i < num_compartments; i++ )
          compartment_list.GetObject( i )->SynchSpeciesListWithDatabase( species_database );

     // at this point we have scanned all the compartments - now
     // remove any invalid species entries in the database

     species_database.RemoveInvalidEntries( );

     // finally we need to go through each transfer object and
     // update the species indices used to refer to each species
     // being transferred

     UINT16 num_transfer_objs = GetNumTransferObjects( );

     for ( i = 0; i < num_transfer_objs; i++ )
          transfer_list.GetObject( i )->SynchSpeciesListWithDatabase( species_database );

     return;
}








// ----------------------------------------------------------------------------
//                     ReactionScheme::DiscardResults( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::DiscardResults( ) 
{ 

     if ( p_results )
     {
          delete p_results;

          p_results = NULL;
     }
}




// ----------------------------------------------------------------------------
//                     ReactionScheme::GetStatusStr( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

const String& ReactionScheme::GetStatusStr( )
{
     static String str;

     switch ( GetStatus() )
     {
     case SIM_NEW :
         str =  String( ResId( vID_STR_STATUS_NEW_SIM ) );
         break;

     case SIM_DATA_MODIFIED :
         str =  String( ResId( vID_STR_STATUS_MODIFIED ) );
         break;

     case SIM_DATA_UNMODIFIED :
         str = String( ResId( vID_STR_STATUS_UNMODIFIED ) );
         break;

     case SIM_READY_TO_RUN :
         str =  String( ResId( vID_STR_STATUS_READY ) );
         break;

     case SIM_WAITING_IN_QUEUE :
         str = String( ResId( vID_STR_STATUS_WAITING ) );
         break;

     case SIM_RUNNING :
         str =  String( ResId( vID_STR_STATUS_RUNNING ) );
         break;

     case SIM_TERMINATE_ZERO_PROBABILITY :
         str = String( ResId( vID_STR_STATUS_TERM_ZEROPROB ) );
         break;

     case SIM_TERMINATE_EVENT_LIMIT :
         str = String( ResId( vID_STR_STATUS_TERM_EVENTLIMIT ) );
         break;

     case SIM_TERMINATE_RECORD_LIMIT :
         str = String( ResId( vID_STR_STATUS_TERM_RECLIMIT ) );
         break;

     case SIM_TERMINATE_ELAPSED_TIME_LIMIT :
         str = String( ResId( vID_STR_STATUS_TERM_TIMELIMIT ) );
         break;

     case SIM_TERMINATE_FINAL_TEMP_REACHED :
         str = String( ResId( vID_STR_STATUS_TERM_FINALTEMP ) );
         break;

     case SIM_TERMINATE_USER_ABORT :
         str = String( ResId( vID_STR_STATUS_TERM_INTERRUPTED ) );
         break;

     case SIM_TERMINATE_CHILD_ABORT :
         str = String( ResId( vID_STR_STATUS_TERM_ABORT ) );
         break;

     case SIM_TERMINATE_FILE_ERROR :
         str = String( ResId( vID_STR_STATUS_TERM_FILEERROR ) );
         break;

     case SIM_TERMINATE_SEG_VIOLATION :
         str = String( ResId( vID_STR_STATUS_TERM_SEGVIOLATION ) );
         break;

     case SIM_TERMINATE_MATH_ERROR :
         str =  String( ResId( vID_STR_STATUS_TERM_MATHERROR ) );
         break;

     case SIM_TERMINATE_ILLEGAL_INSTR :
         str = String( ResId( vID_STR_STATUS_TERM_ILLEGALINSTR ) );
         break;

     default :
         str  =  String( ResId( vID_STR_STATUS_UNKNOWN ) ) + String( (UINT16) GetStatus() );
         break;
     }

     return str;
}



// ----------------------------------------------------------------------------
//                  operator <<   ReactionScheme
// ............................................................................
//
//  this function writes the internal data of a ReactionScheme  to a
//  BinaryOutputStream object
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, ReactionScheme& rObject )
{
     INT32 offset;

	// deal with timestamps

     DateTime current_date; 

     if ( INVALID_DATE == rObject.file_creation_date )
          rObject.file_creation_date = current_date;

     if ( ( INVALID_DATE == rObject.notebook_last_modification_date ) || 
            rObject.IsNotebookTextModified() 
        )
     {
          rObject.notebook_last_modification_date = current_date;
          rObject.notebook_data_modified_since_last_save = FALSE;
          
     }   

     if ( ( INVALID_DATE == rObject.scheme_last_modification_date ) || 
            rObject.scheme_data_modified_since_last_save 
        )
     {
          rObject.scheme_last_modification_date = current_date;
          rObject.scheme_data_modified_since_last_save = FALSE;
          
     }   

     // the simulation data is goint to be discarded as a result of this
     // so reset simulation info 

     rObject.last_simulation_start_date  = INVALID_DATE;
     rObject.DiscardResults();

     // since we are writing a new copy of the reaction scheme to file there cannot 
     // be any accumulated or stepwise simulation time on it so we reset to zero .

     rObject.cumulative_simulation_time = Time(0);
     rObject.step_simulation_time       = Time(0);


     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMNUM_POINTS_FILE_POSITION );
     rBOS << rObject.GetNumSimulationDataPoints();           // 4 bytes, offset = 24d


     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMDATA_FILE_OFFSET_POSITION );
     rBOS << rObject.GetSimulationDataFilePosition();         // 4 bytes, offset = 28d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMSTATE_DATA_FILE_OFFSET_POSITION );
     rBOS << rObject.GetSimulationStateFileOffset();         // 4 bytes, offset = 32d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMSTATUS_CODE_FILE_POSITION );
     rBOS << (UINT16) rObject.GetStatus();                   // 2 bytes, offset = 36d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMCUMULATIVE_SIMULATION_TIME_POSITION );
     rBOS << rObject.GetCumulativeSimulationTime();          // 4 bytes, offset = 38d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMSTEP_SIMULATION_TIME_POSITION );
     rBOS << rObject.GetLastStepSimulationTime();            // 4 bytes, offset = 42d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMLAST_SIMULATION_TIMESTAMP_POSITION );
	rBOS <<  rObject.last_simulation_start_date;            // 8 bytes, offset = 46d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIMLAST_NOTEBOOK_TIMESTAMP_POSITION );
	rBOS <<  rObject.notebook_last_modification_date;       // 8 bytes, offset 54d

     ASSERT( rBOS.GetCurrentFilePosition( offset ) && offset == SIM_NOTEBOOK_TEXT_POSITION );
     rBOS << rObject.GetNotebookText( );                     // 5000 bytes, offset 62d

	rBOS <<  rObject.file_creation_date;
	rBOS <<  rObject.scheme_last_modification_date;

     rBOS << rObject.GetNumMols();
     rBOS << rObject.GetRecordInterval();
     rBOS << rObject.GetRandomNumSeed();
     rBOS << rObject.GetMaxNumEvents ();
     rBOS << rObject.GetElapsedTimeLimit();
     rBOS << rObject.GetEquilCycleCount();
     rBOS << rObject.GetMinEquilEfficiency();

     rBOS << (UINT16) rObject.IsEquilDetectActive();

     UINT16 num_compartments = rObject.GetNumCompartments( );
     UINT16 num_transfer_objs = rObject.GetNumTransferObjects( );

     rBOS << num_compartments;
     rBOS << num_transfer_objs;

     // this is the end of the fixed-size header

     // now write out each compartment's content in order

     for ( UINT16 i = 0; i < num_compartments; i++ )
     {
          CompartmentObject* current_compt = rObject.compartment_list.GetObject( i );

          rBOS << *current_compt;
     }

     for ( i = 0; i < num_transfer_objs; i++ )
     {
          TransferObject* current_xobj = rObject.transfer_list.GetObject( i );

           // wdh added 9.11.98 to allow for WrappedTransferObject

          switch ( current_xobj->GetType() )
          {
               case PO_TYPE_XFER :

                    rBOS << IDType( TRANSFER_OBJECT );
                    break;


               case PO_TYPE_WRAPPED_XFER :

                    rBOS << IDType( WRAPPED_TRANSFER_OBJECT );
                    break;


               default :

                    ASSERT( 0 );
                    break;
          }

          rBOS << *current_xobj;
     }

     rBOS << rObject.species_database;

     // update the header with file offset information
     // first get the current file position and save its value

     if ( rBOS.GetCurrentFilePosition( offset ) )
          rObject.simulation_data_file_offset = offset;
     else
     {
          GetpMainApp()->FileSeekError( rBOS.GetName() );
          return rBOS;
     }

     // move to the location in the file header where we will write
     // this bit of information

     if ( !rBOS.MoveTo( SIMDATA_FILE_OFFSET_POSITION ) )
     {
          GetpMainApp()->FileSeekError( rBOS.GetName() );
          return rBOS;
     }

     // write it

     rBOS << rObject.simulation_data_file_offset;

     // and reset file position to where we were when we started

     if ( !rBOS.MoveTo( offset ) )
          GetpMainApp()->FileSeekError( rBOS.GetName() );

     return rBOS;
}







// ----------------------------------------------------------------------------
//                  operator <<   ReactionScheme
// ............................................................................
//
//  this function writes the internal data of a ReactionScheme to a
//  TextOutputStream object
//
// ----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const ReactionScheme& rObject )
{
     // first write some general info

     rTOS << IDType( START_INPUT_FILE ) << EOL;

     rTOS << IDType( START_APP_INIT ) << EOL;

     rTOS << rObject.GetName();

     Window* pmainwin = GetpMainApp()->GetAppWindow();
     
     rTOS << pmainwin->GetSizePixel() << pmainwin->GetPosPixel();
     rTOS << IDType( END_APP_INIT ) << EOL;


     UINT16 num_compartments = rObject.GetNumCompartments( );
     UINT16 num_transfer_objs = rObject.GetNumTransferObjects( );

     rTOS << IDType( START_SIM_INIT ) << EOL;

     rTOS << IDType( SIM_TIME_BASE ) << (UINT16) rObject.timebase_units << EOL;
     rTOS << IDType( NUM_PARTICLES ) << rObject.num_mols << EOL;
     rTOS << IDType( WRITE_INTERVAL ) << rObject.record_interval << EOL;
     rTOS << IDType( RAND_NUM_SEED ) << rObject.random_num_seed << EOL;
     rTOS << IDType( MAX_EVENTS ) << rObject.max_num_events << EOL;
     rTOS << IDType( ELAPSED_TIME_LIMIT ) << rObject.elapsed_time_limit << EOL;

     rTOS << IDType( EQUIL_DETECT_STATE) << rObject.equil_detect_active << EOL;

     rTOS << IDType( EQUIL_DETECT_CYCLE_LEN) << rObject.equil_cycle_count << EOL;
     rTOS << IDType( EQUIL_DETECT_MIN_EFF ) << rObject.min_equil_efficiency << EOL;

     rTOS << IDType( SIM_DATA_FILE_OFFSET ) << rObject.simulation_data_file_offset << EOL;

     rTOS << IDType( NUMBER_OF_COMPARTMENTS ) << num_compartments << EOL;
     rTOS << IDType( NUMBER_OF_TRANSPORT_PATHS ) << num_transfer_objs << EOL;
     rTOS << IDType( NUM_SPECIES_IN_DATA_TABLE ) << rObject.species_database.GetNumberOfSpecies() << EOL;

     // now write out each compartment's content in order

     for ( UINT16 i = 0; i < num_compartments; i++ )
     {
          CompartmentObject* current_compt = rObject.compartment_list.GetObject( i );

          rTOS << *current_compt;
     }

     for ( i = 0; i < num_transfer_objs; i++ )
     {
          TransferObject* current_xobj = rObject.transfer_list.GetObject( i );

          rTOS << *current_xobj;
     }

     rTOS << rObject.species_database;

     rTOS << IDType( END_SIM_INIT ) << EOL;

     rTOS << IDType( END_INPUT_FILE ) << EOL;

     return rTOS;
}





// ----------------------------------------------------------------------------
//                  operator >>   ReactionScheme
// ............................................................................
//
//  this function sets the internal state of a ReactionScheme using data from a
//  BinaryInputStream object
//
// ----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, ReactionScheme& rObject )
{
     rObject.SetName( rBIS.GetName() );
     rBIS >> rObject.num_simulation_data_records;
     rBIS >> rObject.simulation_data_file_offset;
     rBIS >> rObject.simulation_state_data_offset;

     UINT16 temp16;

     rBIS >> temp16;

     // get last simulator status code -- change of necessary

     rObject.simulator_status_code = (enum SIM_STATUS_CODE) temp16;

     switch(rObject.simulator_status_code )
     {

     case SIM_DATA_MODIFIED:
     case SIM_READY_TO_RUN:
     case SIM_WAITING_IN_QUEUE:

          rObject.simulator_status_code = SIM_DATA_UNMODIFIED;

          break;

     default:
          break;
     }

     // read in the saved time values 

     rBIS >> rObject.cumulative_simulation_time;

     rBIS >> rObject.step_simulation_time;

	rBIS >> rObject.last_simulation_start_date;

	rBIS >> rObject.notebook_last_modification_date;
     rBIS >> rObject.notebook_str;
     rObject.notebook_data_modified_since_last_save = FALSE;

	rBIS >> rObject.file_creation_date;
	rBIS >> rObject.scheme_last_modification_date;

     // this is the end of the fixed-size header

     //  some reaction scheme data

     rBIS >> rObject.num_mols;
     rBIS >> rObject.record_interval;
     rBIS >> rObject.random_num_seed;
     rBIS >> rObject.max_num_events;
     rBIS >> rObject.elapsed_time_limit;
     rBIS >> rObject.equil_cycle_count;
     rBIS >> rObject.min_equil_efficiency;


     rBIS >> temp16;
     rObject.equil_detect_active = (BOOL8) temp16;

     // now get the process object info

     UINT16 num_compartments;
     UINT16 num_transfer_objs;

     rBIS >> num_compartments;
     rBIS >> num_transfer_objs;

     // now read in each compartment's content in order
     // the compartments are added first, then the transfer object which
     // reference the compartments by their position

     String null_str;

     for ( UINT16 i = 0; i < num_compartments; i++ )
     {
          CompartmentObject* pco = new CompartmentObject( null_str, rObject.p_window );

          if ( pco )
          {
               // initialize compartment

               rBIS >> *pco;

               if (rBIS.FileError() )
                    return rBIS;

               // and add to Scheme

               rObject.compartment_list.Insert( pco, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               rBIS.SetExtErrorFlag( );

               return rBIS;
          }

     }


     for ( i = 0; i < num_transfer_objs; i++ )
     {
          TransferObject* pto;
          IDType id_type;
     
          // find out type of TransferObject that was saved and handle
          // appropriately 

          rBIS >> id_type;

          if ( id_type == IDType( TRANSFER_OBJECT ) )
          {
               // make a conventional Transfer Object

               pto = new TransferObject( null_str, rObject.p_window );
          }
          else
          {
               if ( id_type == IDType( WRAPPED_TRANSFER_OBJECT ) )
               {
                    // make a Wrapped Transfer Object instead

                    pto = new WrappedTransferObject( null_str, rObject.p_window );
               }
               else
               {
                    GetpMainApp()->InputFileCorrupt(rBIS.GetName() );
                    rBIS.SetExtErrorFlag( );
                    return rBIS;
               }
          }


          if ( pto )
          {
               // initialize new object from stream

               rBIS >> *pto;

               if (rBIS.FileError() )
                    return rBIS;

               // and add to the Scheme

               rObject.transfer_list.Insert( pto, LIST_APPEND );
          }
          else
          {
               GetpMainApp()->OutOfMemory();
               rBIS.SetExtErrorFlag( );

               return rBIS;
          }
     }

     rBIS >> rObject.species_database;

     rObject.conv_factor_p_to_mole_is_valid = FALSE;

     return rBIS;
}





// ----------------------------------------------------------------------------
//                   ReactionScheme::GetTimeUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetTimeUnits() const 
{
     return String( ResId( vID_STR_TIME_UNITS_SEC));
}





// ----------------------------------------------------------------------------
//                   ReactionScheme::GetAmountUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetAmountUnits() const 
{
     return String( ResId( vID_STR_AMOUNT_UNITS_MOLE));
}






// ----------------------------------------------------------------------------
//                   ReactionScheme::GetConcentrationUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetConcentrationUnits() const 
{
     return String( ResId( vID_STR_CONC_UNITS_MOLAR));

#if 0 
     return GetAmountUnits() + 
            String( ResId( vID_STR_SYMBOL_SLASH ) ) +
            GetVolumeUnits();
#endif
}





// ----------------------------------------------------------------------------
//                   ReactionScheme::GetVolumeUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetVolumeUnits() const 
{
     return String( ResId( vID_STR_VOLUME_UNITS_LITER));
}




// ----------------------------------------------------------------------------
//                   ReactionScheme::GetPressureUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetPressureUnits() const 
{
     return String( ResId( vID_STR_PRESSURE_UNITS_ATM));
}






// ----------------------------------------------------------------------------
//                   ReactionScheme::GetTemperatureUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetTemperatureUnits() const 
{
     return String( ResId( vID_STR_TEMPERATURE_UNITS_DEGK));
}






// ----------------------------------------------------------------------------
//                   ReactionScheme::GetDistanceUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetDistanceUnits() const 
{
     return String( ResId( vID_STR_DISTANCE_UNITS_CM));
}



// ----------------------------------------------------------------------------
//                   ReactionScheme::GetSmallEnergyUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetSmallEnergyUnits() const 
{
     return String( ResId( vID_STR_ENERGY_UNITS_CAL) );
}



// ----------------------------------------------------------------------------
//                   ReactionScheme::GetLargeEnergyUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetLargeEnergyUnits() const 
{
     return String( ResId( vID_STR_KILO ) ) + GetSmallEnergyUnits() ;
}






// ----------------------------------------------------------------------------
//                   ReactionScheme::GetRateConstantUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetRateConstantUnits() const 
{
     return GetVolumeUnits() + 
            String( ResId( vID_STR_SYMBOL_SLASH ) ) +
            GetAmountUnits() +
            String( ResId( vID_STR_SYMBOL_DASH ) ) +
            GetTimeUnits();
}






// ----------------------------------------------------------------------------
//                   ReactionScheme::GetDiffusionCoefficientsUnits()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

const String ReactionScheme::GetDiffusionCoefficientUnits() const 
{
     return GetDistanceUnits() + 
            String( ResId( vID_STR_SYMBOL_SQUARED ) ) +
            String( ResId( vID_STR_SYMBOL_SLASH ) ) +
            GetTimeUnits();
}


// ----------------------------------------------------------------------------
//             ReactionScheme::GetConversionFactorDimensionsToVolume()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

FLOAT64  ReactionScheme::GetConversionFactorDimensionsToVolume()
{
     // temporarily we return the multiplier to 
     // convert volume in cm * cm * cm into iter units

     return 0.001;
}




// ----------------------------------------------------------------------------
//             ReactionScheme::AbortSimulation()
// ............................................................................
//
//  
//
// ----------------------------------------------------------------------------

BOOL8  ReactionScheme::AbortSimulation( ) 
{ 
     BOOL8 rc = TRUE;

     if ( FALSE == simulator.DoesChildExist() )
     {
          // it's not running for some reason 
          // don't bother to abort but return TRUE anyway 

          SetStatus( SIM_TERMINATE_CHILD_ABORT );

          rc = TRUE;
     }
     else
     {
          if ( simulator.Abort() )
          {
               SetStatus( SIM_TERMINATE_CHILD_ABORT );

               rc = TRUE;
          }
          else
               rc = FALSE;
     }

     p_window->GetpFrame()->SetStandardIcon();
     p_window->Enable();

     return rc;
}




// ----------------------------------------------------------------------------
//                ReactionScheme::ChangeAllCompartmentDimensions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::ChangeAllCompartmentDimensions()
{
     if ( CCD_RC_ACCEPT == ChangeCompartmentDimensionDialog( this, p_window ) )
     {
          SetStatus( SIM_DATA_MODIFIED );
     }
}



// ----------------------------------------------------------------------------
//                ReactionScheme::GetCompartmentDimensions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::GetCompartmentDimensions( FLOAT64& CurrentXCellDimension,
                                                  FLOAT64& CurrentYCellDimension,
                                                  FLOAT64& CurrentZCellDimension )
{
     // just get dimension of first compartment - they are all the same

     CompartmentObject* pcompt = GetCompartmentObjectPtrFromIndex( 0 );

     CurrentXCellDimension = pcompt->GetXDimension();
     CurrentYCellDimension = pcompt->GetYDimension();
     CurrentZCellDimension = pcompt->GetZDimension();
}







// ----------------------------------------------------------------------------
//                ReactionScheme::SetCompartmentDimensions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetCompartmentDimensions( FLOAT64 NewXCellDimension,
                                                  FLOAT64 NewYCellDimension,
                                                  FLOAT64 NewZCellDimension )
{
     GetpMainApp()->Wait( TRUE );

     UINT16 num_compts  = GetNumCompartments();

     CompartmentObject* pcompt;

     for ( UINT16 i = 0; i < num_compts; i++ )
     {
          pcompt = GetCompartmentObjectPtrFromIndex( i );

          pcompt->SetXDimension( NewXCellDimension );
          pcompt->SetYDimension( NewYCellDimension );
          pcompt->SetZDimension( NewZCellDimension );
     }
     GetpMainApp()->Wait( FALSE );
}



// ----------------------------------------------------------------------------
//                ReactionScheme::SetSchemeToConstantTemperature()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetSchemeToConstantTemperature( const NumericString& NewTemperature )
{
     GetpMainApp()->Wait( TRUE );

     UINT16 num_compts  = GetNumCompartments();

     CompartmentObject* pcompt;

     for ( UINT16 i = 0; i < num_compts; i++ )
     {
          pcompt = GetCompartmentObjectPtrFromIndex( i );
          pcompt->SetTemperatureOption( CONSTANT_TEMP );
          pcompt->SetConstTempValue( NewTemperature );
     }

     GetpMainApp()->Wait( FALSE );
}




// ----------------------------------------------------------------------------
//                ReactionScheme::ChangeSchemeTemperature()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::ChangeSchemeTemperature()
{
     if ( RC_RC_ACCEPT == EditGlobalSchemeTemperature( this, p_window ) )
     {
          SetStatus( SIM_DATA_MODIFIED );
     }
}





// ----------------------------------------------------------------------------
//                ReactionScheme::ChangeSpeciesConcentration() 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::ChangeSpeciesConcentration()
{
     if ( CGC_RC_ACCEPT == ChangeGlobalConcentrationDialog( this, p_window ) )
     {
          SetStatus( SIM_DATA_MODIFIED );
     }
}







// ----------------------------------------------------------------------------
//                ReactionScheme::SetSpeciesConcToValue() 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ReactionScheme::SetSpeciesConcToValue( const String& rSpeciesName, const NumericString& NewConc )
{
     GetpMainApp()->Wait( TRUE );

     UINT16 num_compts  = GetNumCompartments();

     SPECIES_ID  species_id =  GetpSpeciesDatabase()->GetSpeciesID( rSpeciesName );


     CompartmentObject* pcompt;

     for ( UINT16 i = 0; i < num_compts; i++ )
     {
          pcompt = GetCompartmentObjectPtrFromIndex( i );
          pcompt->SetSpeciesConcentration( species_id, NewConc );
     }

     GetpMainApp()->Wait( FALSE );
}





     
// ----------------------------------------------------------------------------
//                ReactionScheme::SetSpeciesConcUsingFileTable()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

enum CHANGE_CONC_RC  ReactionScheme::SetSpeciesConcUsingFileTable( const String& rSpeciesName, const String& InputFile )
{
     enum CHANGE_CONC_RC   rc = CC_NO_ERROR;

     UINT16 num_compts  = GetNumCompartments();

     NumericString* p_values = new NumericString[num_compts];

     TextInputStream   instr( InputFile );

     if ( instr.IsOpen() && !instr.FileError() )
     {
          for ( UINT16 i = 0; (i < num_compts) && (rc == CC_NO_ERROR) ; i++ )
          {
               instr >> p_values[i];

               if (instr.FileError() )
               {
                    rc = CC_FILE_LENGTH_ERROR;
               }
               else
               {
                    if ( FALSE == p_values[i].IsValidNonNegativeFLOAT64() )
                    {
                         rc = CC_INVALID_VALUE;
                    }
               }
          }

     }
     else
     {
          rc = CC_INVALID_FILE;
     }


     if ( CC_NO_ERROR == rc )
     {
          SPECIES_ID  species_id =  GetpSpeciesDatabase()->GetSpeciesID( rSpeciesName );
     
          CompartmentObject* pcompt;
     
          for ( UINT16 i = 0; i < num_compts; i++ )
          {
               pcompt = GetCompartmentObjectPtrFromIndex( i );
               pcompt->SetSpeciesConcentration( species_id, p_values[i] );
          }
     }

     delete [] p_values;

     return rc;
}



