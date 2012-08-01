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
// methods for class interface
// Date last modified: January 23, 1997
// Written by Frances Houle
// IBM  
//---------------------------------------------------------------------------


#include "intrface.hxx"
#include "3d_sys.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

interface :: interface ()
: reaction_compartment()

{

}


//---------------------------------------------------------------------------
//	Definition of method UpdateGeometry
//	Purpose: to recalculate interface dimensions in variable
//		volume simulations ONLY if interfaced compartment volume has changed
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void interface :: UpdateGeometry()

{
     UINT16                  n;
     compartment_geometry    AdjacentGeometry[ 2 ];
     selected_event_info     Event;
     FLOAT64                 OldArea;

     Event = Simulator->GetEventInfo();

     // this function should only execute if it is called by a variable volume
     // compartment during its own geometry update. Mass diffusion, transfer
     // and interfacial reaction do not affect the geometry of an interface 
     // compartment. Set SizeRatio = 1 to indicate no change

     if ( (Event.EventNo != ID) && (Event.Location == REACTION_COMPARTMENT) )
     {

     	// use mating areas of adjacent compartments to update dimensions

          for ( n = 0; n < 2; n++ )
          {
              AdjacentGeometry[ n ] = pInterfacedCompartment[ n ]->GetCompartmentGeometry();
          }

          OldArea = CmptGeometry.Area;

          if ( CmptGeometry.x )  // xz interface compartment
          {
               CmptGeometry.x = ( AdjacentGeometry[ 0 ].x  +  AdjacentGeometry[ 1 ].x ) / 2;
               CmptGeometry.z = ( AdjacentGeometry[ 0 ].z  +  AdjacentGeometry[ 1 ].z ) / 2;
               CmptGeometry.Area = CmptGeometry.x * CmptGeometry.z;
               CmptGeometry.X_TransferLength = CmptGeometry.x / 2;
          }

          if ( CmptGeometry.y )  // yz interface compartment
          {                                                                      
              CmptGeometry.y = ( AdjacentGeometry[ 0 ].y  +  AdjacentGeometry[ 1 ].y ) / 2;
              CmptGeometry.z = ( AdjacentGeometry[ 0 ].z  +  AdjacentGeometry[ 1 ].z ) / 2;
              CmptGeometry.Area = CmptGeometry.y * CmptGeometry.z;
              CmptGeometry.Y_TransferLength = CmptGeometry.y / 2;
          }

          CmptGeometry.SizeRatio = CmptGeometry.Area / OldArea;

          // update geometries of connected interfacial diffusion compartments - they know
          // who they are
          for ( n=0; n<NumberOfTransferPaths; n++ )
          {
	     	pConnectedTransferPath[n]->UpdatePathGeometry();
          }


     } else {             

          CmptGeometry.SizeRatio = 1.0;

     }          

	return;

}



//---------------------------------------------------------------------------                                               
//	Definition of method UpdateKs                                                                                        
//	Purpose: to update rate constants in interface area after its area has
//        has changed                                                                            
//	Parameters: none                                                                                                         
//	Returns: nothing                                                                                                         
//---------------------------------------------------------------------------                                               
                                                                                                                            
void interface :: UpdateKs()                                                                                            
                                                                                                                            
{                                                                                                                           
	UINT32 			j, k;                                                                                                            
	selected_event_info	Event;                                                                                                 
                                                                                                                            
     // the interface itself has no volume, and its area depends on the geometries                                          
     // of the two volume elements it couples. When either of these changes dimensions                                      
     // the interface concentrations must be updated 
     // A reaction in the interface cannot trigger a geometry change nor need for rate    
     // constant updating, so updating only occurs after a reaction in an attached 3d compartment
     // or a transfer event into or out of a 3d compartment has changed the compartment's dimensions

     Event = Simulator->GetEventInfo();
                                                                                           
     if ( CmptGeometry.SizeRatio != 1.0 )
     {
          UpdateConcentrations( CmptGeometry.SizeRatio );

          switch ( Event.Location )
          {
          case REACTION_COMPARTMENT:
     		// update all connected transfer path step rate constants for which the interface    
               // is the source compartment                                                 
		     for ( j=0; j<NumberOfTransferPaths; j++ )                                            
		     {                                                                                    
                                                                                       
		        pConnectedTransferPath[j]->
                                    UpdateTransferRateConstants( this, CmptGeometry );
		     }                                                                                    
               break;

          case MASS_TRANSFER:
               // if the event occurred in a transfer path connecting interface and 3d
               // update is in the transfer path not here
               // if the event occurred in a different transfer path but triggered an
               // interface geometry change this gives a full update
               for ( j=0; j<NumberOfTransferPaths; j++ )
               {
				if ( TransferPathID[j] != Event.ProcessAreaNo )                                            
				{                                                                                          
					pConnectedTransferPath[j]->UpdateTransferRateConstants( this, CmptGeometry );
				}                                                                                          

               }
               break;

          default: 
               break;
          
          } // end switch
                                                                                                                  
     }  // end if
                                                                                                                            
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
                                                                                                                          
void interface :: UpdateProbabilities (selected_event_info& Event)
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
			(pConnectedTransferPath[ Counter ])->CalcAllProbabilities();
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
                                                                                                  
void interface :: Connect()                                                            
                                                                                                  
{                                                                                                 
	UINT16                  n, j;
     enum REC_TYPE           AreaType;
                                                                                                  
	for ( j=0; j<NumberOfTransferPaths; j++ )
	{                                                                                                
                                                                                                  
		pAdjacentArea[j] = ( pConnectedTransferPath[j] )->GetSourceCompartmentPtr();
                                                                                                  
		// check to see if source no is the same number as this compartment,                            
		// if it is use target                                                                          
		if ( (pAdjacentArea[j])->GetID() == ID )
		{                                                                                               
			pAdjacentArea[j] = ( pConnectedTransferPath[j] )->GetTargetCompartmentPtr();
		}                                                                                               
                                                                                                  
	}      

                                                                                             
    // store pointers to interfaced compartments in array in same order as the transfer      
    // paths that connect them                                                               
                                                                                             
    for ( j=0, NumberOfInterfaces=0; j<NumberOfTransferPaths; j++ )
    {                                                                                        
             AreaType = pAdjacentArea[ j ]->GetProcessAreaType();
             if ( AreaType == COMPARTMENT_TYPE_INTERFACE )                                   
             {                                                                               
                  pInterfacedCompartment[ j ] = pAdjacentArea[ j ];                          
                  NumberOfInterfaces++;
             }

    }


	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitGeometry
//	Purpose: to initialize geometry of the system
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void interface :: InitGeometry()

{
     // assumes all geometries are read in initially - if variable updating
     // depends on changes in interfaced compartments

	// calculate area for interface, dimensions
	// are in cm. Depends on whether interface lies in xz or yz plane
     // flag for orientation is non-zero x or y dimension. This needs 
     // to be expanded when the program is expanded to include segmented
     // z dimension

     if ( CmptGeometry.x )
     {
          CmptGeometry.Area = CmptGeometry.x * CmptGeometry.z;
          CmptGeometry.X_TransferLength = CmptGeometry.x / 2;
          CmptGeometry.X_TransferLength = 0;

     }

     if ( CmptGeometry.y )
     {
          CmptGeometry.Area = CmptGeometry.y * CmptGeometry.z;
          CmptGeometry.Y_TransferLength = CmptGeometry.y / 2;
          CmptGeometry.X_TransferLength = 0;

     }
          CmptGeometry.SizeRatio = 1;

	// set everything else to zero 
	CmptGeometry.Z_TransferLength = 0;
	CmptGeometry.X_TransferArea = 0;
	CmptGeometry.Y_TransferArea = 0;
	CmptGeometry.Z_TransferArea = 0;
     CmptGeometry.Volume = 0;

	return;

}


//---------------------------------------------------------------------------                
//                                                                                           
//	Definition of method InitTPV                                                              
//	Purpose: to initialize TPV and TPV options for an interface                    
//	Parameters: none                                                                          
//	Returns: nothing                                                                          
//                                                                                           
//---------------------------------------------------------------------------                
                                                                                             
void interface :: InitTPV ()                                                      
                                                                                             
{                                                                                            
	UINT16   j;

     // T, V and T and V options are determined by the volume elements separated
     // by the interface. Pressure is not defined. The interface has no volume, so
     // variable and constant volume are interpreted as variable and constant area
     // concentrations are actually in per cm2 units.

     // set options first
     P_option = CONSTANT_PRESSURE;

     if ( ((pInterfacedCompartment[ 0 ]->QueryTOption()) || 
           (pInterfacedCompartment[ 1 ]->QueryTOption())) == VARIABLE_TEMP )
     {
          T_option = VARIABLE_TEMP;
     } else {
          T_option = CONSTANT_TEMP;
     }


     if ( ((pInterfacedCompartment[ 0 ]->QueryVOption()) ||                          
           (pInterfacedCompartment[ 1 ]->QueryVOption())) == VARIABLE_VOLUME )  
     {                                                                               
          V_option = VARIABLE_VOLUME;                                           
     } else {                                                                               
          V_option = CONSTANT_VOLUME;
     }

     // initialize temperature
     Temperature = ( pInterfacedCompartment[ 0 ]->GetTemperature() + 
                     pInterfacedCompartment[ 1 ]->GetTemperature() ) / 2;

     // volume = 0, area is initialized when the geometry is initialized
                                                                                             
	// convert system value for ParticlesPerMol to molar units using area of                  
	// interface - this factor is used to convert rate constants to                           
	// particle units, to update conversion for variable volume                                 
	// simulations and to convert particles to concentrations when simulation                   
	// status is saved                                                                          
                                                                                             
	MolarConversionFactor = (Simulator->GetParticlesPerMol()) * CmptGeometry.Area;
                                                                                             
                                                                                             
	return;                                                                                     
                                                                                             
}                                                                                            
                                                                                             


//---------------------------------------------------------------------------
//   Definition of text input operator >>
//   Purpose: to construct a single compartment (allocate memory, read in data)
//   Parameters: input file and compartment reference addresses
//   Returns: address of input file
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, interface& rObject )

{

     UINT16 j, k, n, SpeciesID, NoSpecies, temp;
     UINT32 StepCounter;
     UINT32 TotalRevAndNonRevSteps;
     profile*  pProfile;

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
		    rObject.pConnectedTransferPath = new transfer_path *[ rObject.NumberOfTransferPaths ];
		    rObject.pAdjacentArea = new reaction_compartment *[ rObject.NumberOfTransferPaths ];

		    // check for allocation errors
		    if ( rObject.TransferPathID == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    if ( rObject.pConnectedTransferPath == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    if ( rObject.pAdjacentArea == 0 )
			{
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;
			}
		    // set array members to zero
		    for ( k=0; k<rObject.NumberOfTransferPaths; k++ )
		    {
			rObject.TransferPathID[k] = 0;
			rObject.pConnectedTransferPath[k] = 0;
			rObject.pAdjacentArea[k] = 0;
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

	      // read in concentration
		    rTIS >> *(rObject.ChemicalSpecies + k);
	       } // end for
	       break;

	  case COMPARTMENT_DIMENSIONS:
          // for this 2d object, there are two possible inputs: x and z finite, y = 0 OR
          // y and z finite, x = 0. Constrained axis is irrelevent (read in and ignored)
		rTIS >> rObject.CmptGeometry.x;
		rTIS >> rObject.CmptGeometry.y;
		rTIS >> rObject.CmptGeometry.z;
		rTIS >> temp;
		rObject.CmptGeometry.UnconstrainedAxis = ( DIRECTION )temp;
		break;

       case COMPARTMENT_PHASE:
            rTIS >> temp;
            rObject.Phase = ( COMPARTMENT_PHASE_TYPE )temp;
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

