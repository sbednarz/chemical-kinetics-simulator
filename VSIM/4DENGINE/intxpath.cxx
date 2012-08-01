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
//        Methods for derived class INTERFACE_TRANSFER_PATH
//        Date last modified January 23, 1997
//        Written by Frances A. Houle
//        IBM  
//---------------------------------------------------------------------------


#include "intxpath.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

interface_transfer_path :: interface_transfer_path ()
: transfer_path()

{

}


//---------------------------------------------------------------------------
//
//	Definition of method UpdateAttachedVolumes
//	Purpose: to cause volumes in connected compartments to be updated
//		after a transfer event
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------
void interface_transfer_path :: UpdateAttachedVolumes()

{
	// update volume of 3d compartment only - interface has no volume

     if ( pSourceCompartment->GetProcessAreaType() == COMPARTMENT_TYPE_3D )
     {
     	pSourceCompartment->UpdateVolume();         
     }
     else
     {    
          pTargetCompartment->UpdateVolume();
     }

	return;
}


//---------------------------------------------------------------------------                                   
//                                                                                                              
//	Definition of method UpdateAttachedCompartmentPressure                                                                   
//	Purpose: to cause pressure in connected  3d compartment to be updated                                            
//		after a transfer event                                                                                      
//	Parameters: none                                                                                             
//	Returns: nothing                                                                                             
//                                                                                                              
//---------------------------------------------------------------------------                                   
void interface_transfer_path :: UpdateAttachedCompartmentPressure()                                                             
                                                                                                                
{                                                                                                               
	// update pressure of 3d compartment only - interface has no pressure
     if ( pSourceCompartment->GetProcessAreaType() == COMPARTMENT_TYPE_3D )          
     {                                                                               
     	pSourceCompartment->UpdatePressure();                                            
     }                                                                               
     else                                                                            
     {                                                                               
          pTargetCompartment->UpdatePressure();                                        
     }                                                                               
                                                                                     
                                                                                                                
	return;                                                                                                        
}                                                                                                               
     

//---------------------------------------------------------------------------                  
//                                                                                             
//	Definition of method UpdateGeometry                                      
//	Purpose: to calculate new geometries in connected 3d compartment and indirectly
//		the interface compartment. This transfer path has no geom dependence                                                                     
//	Parameters: none                                                                            
//	Returns: nothing                                                                            
//                                                                                             
//---------------------------------------------------------------------------                  
void interface_transfer_path :: UpdateGeometry()                            
                                                                                               
{                                                                                              
     if ( !ConstV )
     {
          if ( pSourceCompartment->GetProcessAreaType() == COMPARTMENT_TYPE_3D )                    
          {                                                                                         
       	     pSourceCompartment->UpdateGeometry();                                                    
          }                                                                                         
          else                                                                                      
          {                                                                                         
               pTargetCompartment->UpdateGeometry();                                                
          }                                                                                         
          
     }                                                                                     
	return;                                                                                       
}                                                                                              
                                                                                               
                                                                                               
                                                                                                                
//---------------------------------------------------------------------------                               
//         Definition of method UpdateTransferredSpecies                                                    
//         Purpose: to update particle counts in connected compartments after transfer                      
//         Parameters:                                                                                      
//         Returns: nothing                                                                                 
//---------------------------------------------------------------------------                               

void interface_transfer_path :: UpdateTransferredSpecies (selected_event_info& Event)                                 
{   // begin                                                                                                
                                                                                                            
	// update particle concentrations using reaction stoichiometry                                             
                                                                                                            
    UINT16			          j;
    reaction_compartment*	     pCompartment[2];
    enum COMPARTMENT_PHASE_TYPE    CPhase[2], SpeciesPhase;
    process_info		          Xfr;
    properties                     SpeciesProp;
    BOOL8			               Add;

     // retrieve pointers and phases of source and target - one of them is interface
     pCompartment[0] = Step[ Event.EventNo ]->GetSource(); 
     CPhase[0] = pCompartment[0]->GetPhase();
     pCompartment[1] = Step[ Event.EventNo ]->GetTarget();
     CPhase[1] = pCompartment[1]->GetPhase();
                                                                                                            
     Xfr = GetStepInfo( Event.EventNo );

     // update reactants and products of transfer step, sorting through which species belongs to
     // which compartment
     
     Add = FALSE;
     for ( j=0; j<Xfr.numreactants; j++ )
     {         
          SpeciesProp = Simulator->GetMaterialsProperties( Xfr.ReactantsInStep[j] );
          if ( SpeciesProp.Phase == CPhase[0] )
          {
               // species resides in source compartment, update source
               pCompartment[0]->UpdateChemicalSpecies( Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add);
          }
          else
          {
               // species resides in target compartment but has kinetic influence, update target
               pCompartment[1]->UpdateChemicalSpecies( Xfr.ReactantsInStep[j], Xfr.ReactantStoich[j], Add);
          }
     }
                                                                                                            
                                                                                                            
     Add = FALSE;                                                                                           
     for ( j=0; j<Xfr.numproducts; j++ )                                                                                       
     {                                                                                                                          
          SpeciesProp = Simulator->GetMaterialsProperties( Xfr.ProductsInStep[j] );                                            
          if ( SpeciesProp.Phase == CPhase[1] )                                                                                 
          {                                                                                                                     
               // species resides in target compartment, update target
               pCompartment[1]->UpdateChemicalSpecies( Xfr.ProductsInStep[j], Xfr.ProductStoich[j], Add);
          }                                                                                                                     
          else                                                                                                                  
          {                                                                                                                     
               // species resides in source compartment and remains there, update source                                
               pCompartment[0]->UpdateChemicalSpecies( Xfr.ProductsInStep[j], Xfr.ProductStoich[j], Add);
          }                                                                                                                     
     }                                                                                                                          
                                                                                                            

     return;                                                                                                
                                                                                                            
}    // end method
                                                                                                            
                                                                                                            
//---------------------------------------------------------------------------                 
//
//   Definition of method UpdateKs
//   Purpose: to update rate constants if there has been a volume change
//        after reaction or transfer
//   Parameters: none
//   Returns: nothing
//
//---------------------------------------------------------------------------                 

void interface_transfer_path :: UpdateKs()

{
	UINT32   	               j;                                                             
     compartment_geometry     CGeomS, CGeomT;
                                                                                         
                                                                                         
     if ( ConstV == FALSE )                                                              
     {                                                                                   
          // first update 3d compartment and indirectly the interface compartment
          if ( pSourceCompartment->GetProcessAreaType() == COMPARTMENT_TYPE_3D )   
          {                                                                        
       	     pSourceCompartment->UpdateKs();                                 
          }                                                                        
          else                                                                     
          {                                                                        
               pTargetCompartment->UpdateKs();                               
          }                                                                        
                                                                                         
          // update rate constants of this transfer path in both directions              
          CGeomS = pSourceCompartment->GetCompartmentGeometry();                         
          CGeomT = pTargetCompartment->GetCompartmentGeometry();                         
                                                                                         
	     UpdateTransferRateConstants( pSourceCompartment, CGeomS );
          UpdateTransferRateConstants( pTargetCompartment, CGeomT );
                                                                                         
     } // end if                                                                         
                                                                                         
     return;                                                                             
                                                                                         
}


//---------------------------------------------------------------------------
//
//	Definition of method UpdateTransferRateConstants
//	Purpose: to renormalize units of transfer rate constants when
//		volume of source compartments change
//	Parameters; volume ratio
//	Returns: nothing
//
//---------------------------------------------------------------------------

void interface_transfer_path :: UpdateTransferRateConstants( reaction_compartment* pCompartment,
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
                                                                                                                              
void interface_transfer_path :: UpdateProbabilities (selected_event_info& Event)
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
                                                                                                                              
		// recalculate all probabilities for CONNECTED 3D COMPARTMENT - it
          // will handle updating of the interface and all transfer paths
          if ( pSourceCompartment->GetProcessAreaType() == COMPARTMENT_TYPE_3D )
          {
               pSourceCompartment->UpdateProbabilities( Event );
          }
          else
          {
               pTargetCompartment->UpdateProbabilities( Event );
          }

	}
                                                                                                                              
                                                                                                                              
	return;                                                                                                                      
                                                                                                                              
}                                                                                                                             
                                                                                                                              
                                                                                                                              


//---------------------------------------------------------------------------
//         Definition of operator >>
//         Purpose: to read in transfer path data
//         Parameters: addresses of input file and transfer path instance
//         Returns: nothing
//---------------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, interface_transfer_path& rObject )

{
	UINT16   n, j, k, dummyint;
	FLOAT64  dummyfloat;
	UINT32   TotalNonRevSteps;
	UINT16   Source, Target;
     profile*  pProfile;

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

	    // link to mass transfer steps
	    for ( n = 0; n < rObject.NumberOfSteps; n++ )
	    {
	          rObject.Step[ n ] = new mass_interface_transfer;
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
				rTIS >> *(( mass_interface_transfer *)  rObject.Step[n] );

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
