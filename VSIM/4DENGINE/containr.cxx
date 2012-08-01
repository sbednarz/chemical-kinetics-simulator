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

//---------------------------------------------------------------------------
// methods for class shapeless_container
// Date last modified: January 23, 1997
// Written by Frances Houle
// IBM  
//---------------------------------------------------------------------------


#include "containr.hxx"
#include "amrphsys.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

shapeless_container :: shapeless_container ()
: reaction_compartment()

{

}


//---------------------------------------------------------------------------
//	Definition of method UpdateVolume
//	Purpose: to recalculate compartment volume after amounts of material
//		change following reaction or transfer
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void shapeless_container :: UpdateVolume()

{
	UINT32 			j;
	selected_event_info	Event;
	FLOAT64			OldV;

	switch ( V_option )
	{
		case VARIABLE_VOLUME:

		OldV = Volume;

		// calculate new volume

		Volume = 0;
		for ( j=1; j <= NumberOfChemicalSpecies; j++ )
		{
			Volume += ChemicalSpecies[j].PartialVolume();
		}

		if ( Volume == 0.0 )
		{
			Volume = OldV;
			Geometry.SizeRatio = 1.0;
		}
          break;

		default:  // CONSTANT_VOLUME
		break;

	}  // end switch

	return;

}


//---------------------------------------------------------------------------             
//        Definition of method UpdatePressure                                             
//        Purpose: to update pressure of gas phase compartments using ideal               
//             gas law                                                                    
//        Parameters: none                                                                
//        Returns: nothing                                                                
//---------------------------------------------------------------------------             
                                                                                          
void shapeless_container :: UpdatePressure()                                             
{                                                                                         
                                                                                          
     UINT32         j;                                                                    
     FLOAT64        NoMoles;                                                              
     ParticleType   np;                                                                   
                                                                                          
                                                                                          
     if ( (Phase == GAS) && (P_option == VARIABLE_PRESSURE) )                                                                  
     {                                                                                    
          np = 0;                                                                         
          for ( j=1; j <= NumberOfChemicalSpecies; j++ )                                  
          {                                                                               
               np += ChemicalSpecies[ j ].GetParticles();                                 
          }                                                                               
                                                                                          
          NoMoles = ( FLOAT64 )np / Simulator->GetParticlesPerMol();                      
                                                                                          
 	     Pressure = ( NoMoles * GAS_CONSTANT * Temperature ) / Geometry.Volume;             
                                                                                          
     }                                                                                    
                                                                                          
                                                                                          
     return;                                                                              
                                                                                          
}                                                                                         
                                                                                          
                                                                                          
//---------------------------------------------------------------------------                                    
//	Definition of method UpdateKs                                                                             
//	Purpose: to recalculate compartment rate constants after volume changes                                          
//		following reaction or transfer                                                                        
//	Parameters: none                                                                                              
//	Returns: nothing                                                                                              
//---------------------------------------------------------------------------                                    
                                                                                                                 
void shapeless_container :: UpdateKs()                                                                       
                                                                                                                 
{                                                                                                                
	UINT32 			j;                                                                                                    
	selected_event_info	Event;                                                                                      
                                                                                                                 
	switch ( V_option )                                                                                             
	{                                                                                                               
		case VARIABLE_VOLUME:                                                                                          
                                                                                                                 
		// update rate constants if there are any chemical species left                                                
		if ( Geometry.SizeRatio != 1.0 )
		{                                                                                                              
			UpdateConcentrations( Geometry.SizeRatio );                                                                   
                                                                                                                 
			// update transfer path step rate constants if not first order in 
               // concentration.
			// updates steps whose source compartment is the same as this                                                 
			// compartment                                                                                                
                                                                                                                 
			if ( NumberOfTransferPaths )                                                                                  
			{                                                                                                             
				Event = Simulator->GetEventInfo();                                                                           
                                                                                                                 
				switch ( Event.Location ) {                                                                                  
                                                                                                                 
				case REACTION_COMPARTMENT:                                                                                   
					for ( j=0; j<NumberOfTransferPaths; j++ )                                                                   
					{                                                                                                           
						ConnectedTransferPath[j]->UpdateTransferRateConstants( this, Geometry );                                   
					}                                                                                                           
					break;                                                                                                      
                                                                                                                 
				case MASS_TRANSFER:                                                                                          
					for ( j=0; j<NumberOfTransferPaths; j++ )                                                                   
					{                                                                                                           
						if ( TransferPathID[j] != Event.ProcessAreaNo )                                                            
						{                                                                                                          
							ConnectedTransferPath[j]->UpdateTransferRateConstants( this, Geometry );                                  
						}                                                                                                          
					}                                                                                                           
					break;                                                                                                      
                                                                                                                 
				default:                                                                                                     
					break;                                                                                                      
                                                                                                                 
				} // end switch                                                                                              
                                                                                                                 
			}  // end if Number of Transfer paths                                                                         
                                                                                                                 
		} // end if Volume                                                                                             
		break;                                                                                                         
                                                                                                                 
		default:  // CONSTANT_VOLUME                                                                                   
		break;                                                                                                         
                                                                                                                 
	}  // end switch                                                                                                
                                                                                                                 
	return;                                                                                                         
                                                                                                                 
}                                                                                                                
                                                                                                                 
 
//---------------------------------------------------------------------------                                             
//         Definition of method UpdateProbabilities                                                                       
//         Purpose: to update reaction step probabilities after a reaction using either the                               
//              update tables if the simulation is constant T, V, or scanning through                                     
//              the entire list if the simulation has changing state variables, and                                       
//              manage updating of connected transfer paths if reaction occurrence causes                                 
//     	        transfer step probabilities to change                                                                     
//         Parameters: selected event data structure                                                                      
//         Returns: nothing                                                                                               
//---------------------------------------------------------------------------                                             
                                                                                                                          
void shapeless_container :: UpdateProbabilities (selected_event_info& Event)                                             
{    // begin                                                                                                             
                                                                                                                          
                                                                                                                          
	UINT16              nr, Counter, j, NoPaths;                                                                             
	UINT32              RowLength, NoSteps;                                                                                  
	update_step_info*   ListPtr;                                                                                             
	transfer_path*      pPath;                                                                                               
                                                                                                                          
                                                                                                                          
	if ( T_option == CONSTANT_TEMP && V_option == CONSTANT_VOLUME )                                                          
                                                                                                                          
	{                                                                                                                        
		// COMPARTMENT                                                                                                          
		// update selected reaction steps using ReactionUpdateTable                                                             
                                                                                                                          
		RowLength = ReactionUpdateTable -> GetRowLength(Event.EventNo);                                                         
                                                                                                                          
		for (Counter = 0; Counter < RowLength ; Counter++ )                                                                     
		{                                                                                                                       
		   nr = ReactionUpdateTable->GetStepNumber( Event.EventNo, Counter );                                                   
		   UpdateStepProbability( nr );                                                                                         
                                                                                                                          
		} // endfor                                                                                                             
                                                                                                                          
		// update compartment's total probability                                                                               
		CalcTotalProbability();                                                                                                 
                                                                                                                          
		// TRANSFER PATHS                                                                                                       
		if ( NumberOfTransferPaths )                                                                                            
		{                                                                                                                       
			// step through update list for selected reaction, tell                                                                
			// connected transfer paths to update themselves.                                                                      
			NoSteps = UpdateTransferAfterReaction[ Event.EventNo ].GetLength();                                                    
                                                                                                                          
			// if NoSteps == 0, then nothing happens, otherwise updating                                                           
			// occurs step by step                                                                                                 
                                                                                                                          
			for ( Counter = 0; Counter < NoSteps; Counter++ )                                                                      
			{                                                                                                                      
			      ListPtr = UpdateTransferAfterReaction[ Event.EventNo ].GetData( Counter );                                       
  		           ListPtr->pLocation->UpdateStepProbability ( ListPtr->StepNo );                                             
			}                                                                                                                      
                                                                                                                          
		       // update connected transfer paths' total probability if single steps were recalculated                          
			NoPaths = UpdateTransferAfterReaction[ Event.EventNo ].GetNoPaths();                                                   
		       for (Counter = 0; Counter < NoPaths; Counter++ )                                                                 
		       {                                                                                                                
			    pPath = UpdateTransferAfterReaction[ Event.EventNo ].GetPathNumber( Counter );                                     
			    pPath->CalcTotalProbability();                                                                                     
		       }                                                                                                                
                                                                                                                          
		}  // end if NumberOfTransferPaths                                                                                      
                                                                                                                          
	} else {   // not constant volume and temperature                                                                        
                                                                                                                          
		// update all reaction step probabilities                                                                               
		CalcAllProbabilities();                                                                                                 
                                                                                                                          
		// tell all connected transfer paths (if any) to update all their probabilities                                         
		for (Counter = 0; Counter < NumberOfTransferPaths; Counter++ )                                                          
		{                                                                                                                       
			(ConnectedTransferPath[ Counter ])->CalcAllProbabilities();                                                            
		}                                                                                                                       
                                                                                                                          
	}   // end if constant volume and temperature                                                                            
                                                                                                                          
	return;                                                                                                                  
                                                                                                                          
}      // end method                                                                                                      
                                                                                                                          
                                                                                                                          
                                                                                                                

//---------------------------------------------------------------------------                     
//	Definition of method Connect                                                                   
//	Purpose: to retrieve connected compartment pointers from transfer                              
//		paths and interfaced compartments                                                             
//	Parameters: none                                                                               
//	Returns: nothing                                                                               
//---------------------------------------------------------------------------                     
                                                                                                  
void shapeless_container :: Connect()                                                            
                                                                                                  
{                                                                                                 
	UINT16  j;                                                                    
                                                                                                  
	for ( j=0; j<NumberOfTransferPaths; j++ )                                                        
	{                                                                                                
                                                                                                  
		AdjacentArea[j] = ( ConnectedTransferPath[j] )->GetSourceCompartmentPtr();                      
                                                                                                  
		// check to see if source no is the same number as this compartment,                            
		// if it is use target                                                                          
		if ( (AdjacentArea[j])->GetID() == ID )                                                         
		{                                                                                               
			AdjacentArea[j] = ( ConnectedTransferPath[j] )->GetTargetCompartmentPtr();                     
		}                                                                                               
                                                                                                  
	}                                                                                                
                                                                                                  
                                                                                                  
	return;                                                                                          
                                                                                                  
}                                                                                                 
                                                                                                  

//---------------------------------------------------------------------------                
//                                                                                           
//	Definition of method InitTPV                                                              
//	Purpose: to initialize variables needed to update volume when variable                    
//	Parameters: none                                                                          
//	Returns: nothing                                                                          
//                                                                                           
//---------------------------------------------------------------------------                
                                                                                             
void shapeless_container :: InitTPV ()                                                      
                                                                                             
{                                                                                            
	UINT32 j;
     FLOAT64 InitMoles;


     // volume, temperature and TPV options are read in for this compartment type
     // calculate P (if gas) and volume update info
                                                                                             
	switch (V_option) {                                                                         
                                                                                             
	case CONSTANT_VOLUME:                                                                       
		break;                                                                                     
                                                                                             
	case VARIABLE_VOLUME:                                                                       
		for ( j=1; j <= NumberOfChemicalSpecies; j++ )                                             
		{                                                                                          
                                                                                             
			ChemicalSpecies[ j ].CalcVolPerParticle( j );                                             
                                                                                             
		}                                                                                          
		break;                                                                                     
                                                                                             
	default:                                                                                    
		break;                                                                                     
                                                                                             
	} 


     if ( Phase == GAS )
     {
	    switch ( P_option ) {                                                                    
                                                                                          
	    case CONSTANT_PRESSURE:                                                                  
	         	break;                                                                                       
                                                                                          
	    case VARIABLE_PRESSURE:
                                                                  
		   // for gas phase systems only. An explicit check for initial phase needs to be          
		   // put in when the variable volume option is fully implemented                          
		   InitMoles = CalcInitialNoOfMoles();                                                     
		   Pressure = ( InitMoles * GAS_CONSTANT * Temperature ) / Geometry.Volume;                         
		   break;                                                                                  
                                                                                          
	    default:                                                                                 
	 	   break;                                                                                  
         }
                                                                                          
	}                                                                                        
                                                                                          
                                                                                             
	// convert system value for ParticlesPerMol to molar units using volume of                  
	// compartment - this factor is used to convert rate constants to                           
	// particle units, to update conversion for variable volume                                 
	// simulations and to convert particles to concentrations when simulation                   
	// status is saved                                                                          
                                                                                             
	MolarConversionFactor = (Simulator->GetParticlesPerMol()) * Geometry.Volume;                         
                                                                                             
	return;                                                                                     
                                                                                             
}                                                                                            
                                                                                             

//---------------------------------------------------------------------------
//   Definition of text input operator >>
//   Purpose: to construct a single compartment (allocate memory, read in data)
//   Parameters: input file and compartment reference addresses
//   Returns: address of input file
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, shapeless_container& rObject )

{

     UINT16 j, k, n, SpeciesID, NoSpecies, temp;
     UINT32 StepCounter;
     UINT32 TotalRevAndNonRevSteps;

     // initialize variables to calculate number of reaction steps
     TotalRevAndNonRevSteps = 0;

     while (DataCode != IDType (END_COMPARTMENT_INIT) )
     {
	  // check error code with each pass through the loop
	  if ( Simulator->GetStatusCode() != SIM_RUNNING )
	  {
	       return rTIS;
	  }

	  // proceed

	  rTIS >> DataCode;

	  switch (DataCode)  {

	  case COMPARTMENT_NUM_XFER_OBJS:
	       rTIS >> rObject.NumberOfTransferPaths;

	       // allocate memory for list of transfer paths
	       // if NumberOfTransferPaths == 0 then this is a single compartment simulation

	       if  ( rObject.NumberOfTransferPaths )
	       {
		    // allocate space needed for transfer path indices & connection information
		    rObject.TransferPathID = new UINT16 [ rObject.NumberOfTransferPaths ];
		    rObject.ConnectedTransferPath = new transfer_path *[ rObject.NumberOfTransferPaths ];
		    rObject.AdjacentArea = new reaction_compartment *[ rObject.NumberOfTransferPaths ];

		    // check for allocation errors
		    if ( rObject.TransferPathID == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    if ( rObject.ConnectedTransferPath == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    if ( rObject.AdjacentArea == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    // set array members to zero
		    for ( k=0; k<rObject.NumberOfTransferPaths; k++ )
		    {
			rObject.TransferPathID[k] = 0;
			rObject.ConnectedTransferPath[k] = 0;
			rObject.AdjacentArea[k] = 0;
		    }
	       }
	       break;

	  case COMPARTMENT_NUM_RXNS:
		rTIS >> TotalRevAndNonRevSteps;
		if ( rObject.NumberOfRevSteps )
		{
			rObject.NumberOfSteps = rObject.NumberOfRevSteps + TotalRevAndNonRevSteps;
		}
		break;

	  case COMPARTMENT_NUM_REVERSIBLE_RXNS:
		rTIS >> rObject.NumberOfRevSteps;
		if ( TotalRevAndNonRevSteps )
		{
			rObject.NumberOfSteps = rObject.NumberOfRevSteps + TotalRevAndNonRevSteps;
		}
		break;

	  case COMPARTMENT_NUM_SPECIES:

		// read number of chemical species and discard - value is no of species in
		// compartment's reaction steps, not total number of species in system
		rTIS >> NoSpecies;

		// get real number from Simulator object
		rObject.NumberOfChemicalSpecies = Simulator->GetNoOfChemicalSpecies();


		// allocate memory for list of chemical species. The total is that for the
		// entire simulation system, to allow data members to be easily accessed
		// through a common indexing system, but in general only part will be
		// used in any given compartment. The number of species is incremented by 1
		// because the species numbering system is 1-based and not 0-based. The 0th
		// member of the array is not accessed.

		NoSpecies = rObject.NumberOfChemicalSpecies + 1;
		rObject.ChemicalSpecies = new reactant [ NoSpecies ];
		if ( rObject.ChemicalSpecies == 0 )
		{
		    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		    return rTIS;
		}
	       break;

	  case COMPARTMENT_XFER_OBJ_INDEX:
	       //check to be sure memory for transfer path IDs has been allocated. If not
	       //this means the input file is out of proper order
	       ASSERT ( rObject.NumberOfTransferPaths != 0 );

	       // proceed to read in and store transfer path IDs
	       for ( n = 0; n < rObject.NumberOfTransferPaths; n++ )
	       {
		    rTIS >> rObject.TransferPathID[n];
	       }
	       break;

	  case START_RXN_INIT:
	       // check to be sure the number of reaction steps has been read in. If not
	       // this means the input file is out of proper order. There can be zero reactions, but
	       // if so then reaction data should not be read in

	       // this routine assumes that the reactions are read in as a single block, They are stored
	       // in the order they are read in

	       ASSERT ( rObject.NumberOfSteps != 0 );

	       // allocate memory for pointer to reaction steps
	       rObject.Step = new process *[ rObject.NumberOfSteps ];
	       if ( rObject.Step == 0 )
	       {
		    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
		    return rTIS;
	       }

	    // link to reaction steps
		 for ( n = 0; n < rObject.NumberOfSteps; n++ )
		{

			 rObject.Step[ n ] = new reaction_step;
			 if ( rObject.Step[ n ] == 0 )
				{
					Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
					return rTIS;
				}

		 }

		// read in reaction scheme
		n = 0;

		// read in sets of data, one per reaction step
		while ( n < rObject.NumberOfSteps )
		{
			// read in data

			rTIS >> *((reaction_step*) rObject.Step[n] );

			// store pointer to process area containing step
			rObject.Step[n]->SetPointer( &rObject );

               // set pointers if rate constant for step has a profile
               // only done for forward going since reversible programmed rate constants
               // are not implemented
               pProfile = rObject.Step[n]->GetProfilePointer();
               if ( pProfile )
               {
                    Simulator->SetProgVarPointersInTimebase( pProfile, &rObject );
               }
			// if reversible step copy data into partner step
			if ( rObject.Step[n]->QueryDirection() == REVERSIBLE )
			{
				k = n + 1;
				rObject.Step[n]->SetPartnerStepNo( k );

				// store pointer to process area containing reverse step
				rObject.Step[k]->SetPointer( &rObject );
				// move data from temporary storage in reaction n into reaction k data structures
				// and release temporary storage
				rObject.Step[k]->MoveTempStepData( n );
				rObject.Step[k]->MoveTempKData( n );

				n++;

			} // end if Reaction[n]

	       // look for beginning of next set of reaction step data if
	       // not at last reaction step in scheme. This keeps data
	       // codes in synch

	       if ( n < ( rObject.NumberOfSteps - 1 ) )
	       {
		    rTIS >> DataCode;
	       }

			n++;

		}  // end while
		break;

	  case SPECIES_LIST:

		// check to be sure that data file is in order, array has been allocated
		ASSERT ( rObject.NumberOfChemicalSpecies > 0 );

	       // read in number of species whose concentrations will be read in
	       rTIS >> NoSpecies;

	       for ( n=0; n < NoSpecies; n++ )
	       {
		    rTIS >> DataCode;

		    // check to be sure that the data file is in order
		    ASSERT ( DataCode == IDType (COMPARTMENT_SPECIES_INFO) );

		    rTIS >> k;  // species ID

	      // read in concentration in moles/liter units
		    rTIS >> *(rObject.ChemicalSpecies + k);
	       } // end for
	       break;

	  case COMPARTMENT_TEMPERATURE_OPTION:
	       // read enum into temp variable and cast to type
	       rTIS >> temp;
	       rObject.T_option = ( TEMPERATURE_OPTION )temp;
	       break;

	  case COMPARTMENT_PRESSURE_OPTION:
	       // read enum into temp variable and cast to type
	       rTIS >> temp;
	       rObject.P_option = ( PRESSURE_OPTION )temp;
	       break;

	  case COMPARTMENT_VOLUME_OPTION:
	       // read enum into temp variable and cast to type
	       rTIS >> temp;
	       rObject.V_option = ( VOLUME_OPTION )temp;
	       // if volume option is NOT_TRACKED_VOLUME chage to CONSTANT_VOLUME
	       if ( rObject.V_option == NOT_TRACKED_VOLUME ) rObject.V_option = CONSTANT_VOLUME;
	       break;

	  case COMPARTMENT_INIT_TEMP:
	       rTIS >> rObject.Temperature;
	       break;

	  case COMPARTMENT_INIT_VOLUME:
	       rTIS >> rObject.Geometry.Volume;
	       break;

       case COMPARTMENT_PHASE:
            rTIS >> rObject.Phase;
            break;

	  default:
		if (DataCode != IDType (END_COMPARTMENT_INIT) )
		{
			Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		}
		break;

	  }   // end switch

	} // end while

     return rTIS;

}

 //---------------------------------------------------------------------------

