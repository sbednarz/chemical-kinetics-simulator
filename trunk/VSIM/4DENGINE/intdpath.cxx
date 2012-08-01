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
//        Methods for derived class INTERFACE_DIFFUSION_PATH
//        Date last modified December 16, 1996
//        Written by Frances A. Houle
//        IBM  
//---------------------------------------------------------------------------


#include "intdpath.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

interface_diffusion_path :: interface_diffusion_path ()
: transfer_path()

{

}

                                                                                               
//---------------------------------------------------------------------------                   
//	Definition of method UpdatePathGeometry                                                      
//	Purpose: to update path geometry when the source or target compartment                       
//		has changed volume                                                                          
//	Parameters: none                                                                             
//	Returns: nothing                                                                             
//---------------------------------------------------------------------------                   
                                                                                               
void interface_diffusion_path :: UpdatePathGeometry ()                                          
{                                                                                               
                                                                                               
     compartment_geometry 	SourceGeom, TargetGeom;                                                  
                                                                                               
     SourceGeom = pSourceCompartment->GetCompartmentGeometry();                                     
     TargetGeom = pTargetCompartment->GetCompartmentGeometry();                                     
                                                                                               
     switch ( PathGeometry.Axis ) {                                                                 
                                                                                               
     case X_AXIS:                                                                                   
	    PathGeometry.Length = SourceGeom.X_TransferLength + TargetGeom.X_TransferLength;              
	    if ( SourceGeom.z <= TargetGeom.z )                                                           
	    {                                                                                             
		   PathGeometry.Width = SourceGeom.z;                                                           
	    } else {                                                                                      
		   PathGeometry.Width = TargetGeom.z;                                                           
	    }                                                                                             
	    break;                                                                                        
                                                                                               
     case Y_AXIS:                                                                                   
	    PathGeometry.Length = SourceGeom.Y_TransferLength + TargetGeom.Y_TransferLength;              
	    if ( SourceGeom.z <= TargetGeom.z )                                                           
	    {                                                                                             
		   PathGeometry.Width = SourceGeom.z;                                                           
	    } else {                                                                                      
		   PathGeometry.Width = TargetGeom.z;                                                           
	    }                                                                                             
         break;                                                                                
                                                                                               
     case Z_AXIS:                                                                                   
                                                                                               
         // at present there is no provision for interfacial diffusion along the z direction   
         // since the structure has only x and y segmentation. When this is implemented,       
         // there are two orientations along z for an interface: zy and zx.                    
                                                                                               
	    break;                                                                                        
                                                                                               
     default:                                                                                       
	    break;                                                                                        
                                                                                               
     }                                                                                              
     return; 

}                                                                                       
                                                                                               
                                                                                                 
//---------------------------------------------------------------------------                               
//         Definition of method UpdateTransferredSpecies                                                    
//         Purpose: to update particle counts in connected compartments after transfer                      
//         Parameters:                                                                                      
//         Returns: nothing                                                                                 
//---------------------------------------------------------------------------                               
void interface_diffusion_path :: UpdateTransferredSpecies (selected_event_info& Event)                                 
{   // begin                                                                                                
                                                                                                            
	// update particle concentrations using reaction stoichiometry                                             
                                                                                                            
    UINT16			j;                                                                                             
    reaction_compartment*	pStep;                                                                            
    process_info		Xfr;                                                                                      
    BOOL8			Add;                                                                                            
                                                                                                            
     Xfr = GetStepInfo( Event.EventNo );                                                                    
                                                                                                            
     //only one chemical species transferred at a time - convention is that                                 
     //it is the first species in the step.                                                                 
                                                                                                            
     j=0;                                                                                                   
                                                                                                            
     pStep = Step[ Event.EventNo ]->GetTarget();     Add = TRUE;                                            
     pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add); //increment         
                                                                                                            
     pStep = Step[ Event.EventNo ]->GetSource();                                                            
     Add = FALSE;                                                                                           
     pStep->UpdateChemicalSpecies (Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add); //decrement         
                                                                                                            
     return;                                                                                                
                                                                                                            
}    // end method                                                                                          
                                                                                                            
                                                                                                            

//---------------------------------------------------------------------------
//
//	Definition of method UpdateTransferRateConstants
//	Purpose: to renormalize units of transfer rate constants when
//		volumes of source compartments change
//	Parameters; volume ratio
//	Returns: nothing
//
//---------------------------------------------------------------------------

void interface_diffusion_path :: UpdateTransferRateConstants( reaction_compartment* pCompartment,
			compartment_geometry& CmptGeometry )

{
	UINT32 	j;

	// this method assumes that there has already been a test that the compartment volume is
	// variable,

	// update reaction rate constants only if step source and passed compartment pointers are the same
	for ( j=0; j<NumberOfSteps; j++ )
	{
		if ( Step[j]->GetSource() == pCompartment )
		{
			Step[j]->UpdateRateCoefficients( CmptGeometry.SizeRatio );
		}

	}

	return;

}


//---------------------------------------------------------------------------                                                 
//         Definition of method UpdateProbabilities                                                                           
//         Purpose: to update transfer path probability after an event, and then                                              
//		manage update of reaction step probabilities in target and                                                                
//		source compartments using list if const T & V                                                                             
//         Parameters: selected event data structure                                                                          
//         Returns: nothing                                                                                                   
//---------------------------------------------------------------------------                                                 
                                                                                                                              
void interface_diffusion_path :: UpdateProbabilities (selected_event_info& Event)
{    // begin                                                                                                                 
                                                                                                                              
	UINT16              Counter, j;                                                                                              
	UINT32              NoSteps;                                                                                                 
	update_step_info*   ListPtr;                                                                                                 
     transfer_path*      pPath;                                                                                               
                                                                                                                              
                                                                                                                              
	if ( ConstT && ConstV )                                                                                                      
	{                                                                                                                            
		// THIS TRANSFER PATH                                                                                                       
                                                                                                                              
		// update transfer event probability for path. Also update the                                                              
		// companion step if the step is reversible.                                                                                
		Step[Event.EventNo]->ProbCalc();                                                                                            
		if ( Step[Event.EventNo]->QueryDirection() == REVERSIBLE )                                                                  
		{                                                                                                                           
			j = Step[Event.EventNo]->GetPartnerStepNo();                                                                               
			Step[j]->ProbCalc();                                                                                                       
		}                                                                                                                           
                                                                                                                              
		//calculate new total probability for path                                                                                  
		CalcTotalProbability();                                                                                                     
                                                                                                                              
                                                                                                                              
		// SOURCE AND TARGET COMPARTMENTS                                                                                           
                                                                                                                              
		// step through update list for selected transfer step, tell                                                                
		// compartments to update themselves                                                                                        
		NoSteps = UpdateReactionAfterTransfer[ Event.EventNo ].GetLength();                                                         
                                                                                                                              
		// if NoSteps == 0, then nothing happens, otherwise updating                                                                
		// occurs step by step                                                                                                      
                                                                                                                              
		for ( Counter = 0; Counter < NoSteps; Counter++ )                                                                           
		{                                                                                                                           
		      ListPtr = UpdateReactionAfterTransfer[ Event.EventNo ].GetData( Counter );                                            
		      ListPtr->pLocation->UpdateStepProbability( ListPtr->StepNo );                                                         
		}                                                                                                                           
                                                                                                                              
		// update connected transfer paths' total probability                                                                       
                                                                                                                              
		if ( UpdateReactionAfterTransfer[ Event.EventNo ].CalcTotalSourceProb() )                                                   
		{                                                                                                                           
			pSourceCompartment->CalcTotalProbability();                                                                                
		}                                                                                                                           
                                                                                                                              
		if ( UpdateReactionAfterTransfer[ Event.EventNo ].CalcTotalTargetProb() )                                                   
		{                                                                                                                           
			pTargetCompartment->CalcTotalProbability();                                                                                
		}                                                                                                                           
                                                                                                                              
                                                                                                                              
		// TRANSFER PATHS CONNECTED TO SOURCE AND TARGET COMPARTMENTS                                                               
                                                                                                                              
		if (UpdateTransferAfterTransfer)   // only used if there are more than one path                                             
		{                                                                                                                           
                                                                                                                              
			// step through update list for selected transfer step, tell                                                               
			// transfer paths to update themselves                                                                                     
			NoSteps = UpdateTransferAfterTransfer[ Event.EventNo ].GetLength();                                                        
                                                                                                                              
			// if NoSteps == 0, then nothing happens, otherwise updating                                                               
			// occurs step by step                                                                                                     
                                                                                                                              
			for ( Counter = 0; Counter < NoSteps; Counter++ )                                                                          
			{                                                                                                                          
				ListPtr = UpdateTransferAfterTransfer[ Event.EventNo ].GetData( Counter );                                                
				ListPtr->pLocation->UpdateStepProbability ( ListPtr->StepNo );                                                            
			}                                                                                                                          
                                                                                                                              
			// update connected transfer paths' total probability                                                                      
			for ( Counter = 0; Counter < UpdateTransferAfterTransfer[ Event.EventNo ].GetNoToRecalc(); Counter++ )                     
			{                                                                                                                          
			       pPath = UpdateTransferAfterTransfer[ Event.EventNo ].GetPathNoToRecalc( Counter );                                  
			       pPath->CalcTotalProbability();                                                                                      
			}                                                                                                                          
                                                                                                                              
		}  // end if update transfer after transfer                                                                                 
                                                                                                                              
                                                                                                                              
	} else {    // transfer path not const T & V                                                                                 
                                                                                                                              
		// recalculate all probabilities for THIS TRANSFER PATH                                                                     
		CalcAllProbabilities();                                                                                                     
                                                                                                                              
		// recalculate all probabilities for CONNECTED COMPARTMENTS                                                                 
		pSourceCompartment->CalcAllProbabilities();                                                                                 
		pTargetCompartment->CalcAllProbabilities();                                                                                 
                                                                                                                              
		// recalculate all probabilities for CONNECTED TRANSFER PATHS                                                               
		pSourceCompartment->UpdateAllConnectedTransferPathProbs( ID );                                                              
		pTargetCompartment->UpdateAllConnectedTransferPathProbs( ID );                                                              
	}                                                                                                                            
                                                                                                                              
                                                                                                                              
	return;                                                                                                                      
                                                                                                                              
}                                                                                                                             
                                                                                                                              
                                                                                                                              

//---------------------------------------------------------------------------
//	Definition of method InitPathGeometry
//	Purpose: to initialize variable, setting to default values since this
//		path object is geometry-independent
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void interface_diffusion_path :: InitPathGeometry ()

{
	compartment_geometry 	SourceGeom, TargetGeom;

	SourceGeom = pSourceCompartment->GetCompartmentGeometry();
	TargetGeom = pTargetCompartment->GetCompartmentGeometry();

	switch ( PathGeometry.Axis ) {

	case X_AXIS:
		PathGeometry.Length = SourceGeom.X_TransferLength + TargetGeom.X_TransferLength;
		if ( SourceGeom.z <= TargetGeom.z )                
		{                                                  
			PathGeometry.Width = SourceGeom.z;                
		} else {                                           
			PathGeometry.Width = TargetGeom.z;                
		}                                                  
		break;

	case Y_AXIS:
		PathGeometry.Length = SourceGeom.Y_TransferLength + TargetGeom.Y_TransferLength;
		if ( SourceGeom.z <= TargetGeom.z )
		{
			PathGeometry.Width = SourceGeom.z;
		} else {
			PathGeometry.Width = TargetGeom.z;
		}
		break;

	case Z_AXIS:

          // at present there is no provision for interfacial diffusion along the z direction
          // since the structure has only x and y segmentation. When this is implemented,
          // there are two orientations along z for an interface: zy and zx.

		break;

	default:
		break;

	}
	return;

}



//---------------------------------------------------------------------------
//         Definition of operator >>
//         Purpose: to read in transfer path data
//         Parameters: addresses of input file and transfer path instance
//         Returns: nothing
//---------------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, interface_diffusion_path& rObject )

{
	UINT16 n, j, k, dummyint, temp;
	FLOAT64 dummyfloat;
	UINT32 TotalRevSteps, TotalNonRevSteps;
	UINT16 Source, Target;

	while (DataCode != IDType(END_TRANSFER_OBJECT_INIT) )
	{
		// check error code with each pass through loop
		if ( Simulator->GetStatusCode() != SIM_RUNNING )
		{
			return rTIS;
		}

		rTIS >> DataCode;

		switch ( DataCode ) {

		case TRANSFER_OBJECT_DATA:
			rTIS >> rObject.SourceID;
			rTIS >> rObject.TargetID;
			rTIS >> rObject.NumberOfRevSteps;
			rTIS >> TotalNonRevSteps;

			rObject.NumberOfSteps = TotalNonRevSteps + 2 * rObject.NumberOfRevSteps;
			break;

	  case TRANSFER_OBJECT_DIRECTION:
	       // read enum into temp variable and cast to type
	       rTIS >> temp;
	       rObject.PathGeometry.Axis = ( DIRECTION )temp;
	       break;

		case START_XFER_STEP_INIT:
			//check to be sure file is in order
			ASSERT ( rObject.NumberOfSteps != 0 );

			// allocate memory for pointers to transfer steps.

			rObject.Step = new process *[ rObject.NumberOfSteps ];
			if ( rObject.Step == 0 )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
			}

		    // link to interface diffusion steps
		    for ( n = 0; n < rObject.NumberOfSteps; n++ )
		    {
		       rObject.Step[ n ] = new mass_interface_diffusion;
		       if ( rObject.Step[ n ] == 0 )
			  {
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
			  }

		    }


			// set up transfer steps

			n = 0;

			// read in sets of data, one per reaction step
			while ( n < rObject.NumberOfSteps )
			{
				// read in data
				rTIS >> *(( mass_interface_diffusion*) rObject.Step[n] );

				// store pointer to process area containing step
				rObject.Step[n]->SetPointer( &rObject );

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

				} // end if Step[n]

			       // look for beginning of next set of transfer step data if
			       // not at last transfer step in scheme. This keeps data
			       // codes in synch

			       if ( n < ( rObject.NumberOfSteps - 1 ) )
			       {
				    rTIS >> DataCode;
			       }

				n++;

			}  // end while
			break;

		default:
		if (DataCode != IDType (END_TRANSFER_OBJECT_INIT) )
		{
			Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		}
		break;

		} // end switch

	} // end while

	return rTIS;

}


//---------------------------------------------------------------------------
