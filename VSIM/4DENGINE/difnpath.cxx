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
//        Methods for derived class MASS_DIFFUSION_PATH
//        Date last modified December 17, 1996
//        Written by Frances A. Houle
//        IBM  
//---------------------------------------------------------------------------


#include "difnpath.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

mass_diffusion_path :: mass_diffusion_path ()
: transfer_path()

{

}


//---------------------------------------------------------------------------
//         Definition of method CalcPressure
//         Purpose: to update average Pressure at interface between
//		compartments, Calculated only once for constant P
//		simulations, updated for variable P simulations
//         Parameters: none
//         Returns: nothing
//---------------------------------------------------------------------------
void mass_diffusion_path :: CalcPressure ()

{   //begin

     if ( ConstP == FALSE )
     {
	AvgPressure = ( pSourceCompartment->GetPressure() + pTargetCompartment->GetPressure() ) / 2.0;
     }
     return;

}  // end


//---------------------------------------------------------------------------
//         Definition of method UpdateTransferredSpecies
//         Purpose: to update particle counts in connected compartments after transfer
//         Parameters:
//         Returns: nothing
//---------------------------------------------------------------------------
void mass_diffusion_path :: UpdateTransferredSpecies (selected_event_info& Event)
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
//	Definition of method UpdateAttachedVolumes
//	Purpose: to cause volumes in connected compartments to be updated
//		after a transfer event
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------
void mass_diffusion_path :: UpdateAttachedVolumes()

{
	// update volumes of compartments and reaction rate constants,
	// transfer path geometries for all attached transfer paths
	// as well as rate constants for transfer for all connected paths
	// the present path is updated at the end
	pSourceCompartment->UpdateVolume();
	pTargetCompartment->UpdateVolume();
	UpdatePathGeometry();
	// update this path for steps going both directions
	UpdateTransferRateConstants( pSourceCompartment, pSourceCompartment->GetCompartmentGeometry() );
	UpdateTransferRateConstants( pTargetCompartment, pTargetCompartment->GetCompartmentGeometry() );

	return;
}



//---------------------------------------------------------------------------
//
//	Definition of method UpdateAttachedCompartmentPressure
//	Purpose: to cause pressures in connected compartments to be updated
//		after a transfer event
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------
void mass_diffusion_path :: UpdateAttachedCompartmentPressure()

{
	// update pressures of compartments and calculate avg pressures for connected
	// transfer paths

	pSourceCompartment->UpdatePressure();
	pTargetCompartment->UpdatePressure();

	// update this transfer path
	CalcPressure();

	return;
}


//---------------------------------------------------------------------------
//
//	Definition of method UpdateTransferRateConstants
//	Purpose: to renormalize units of transfer rate constants when
//		volumes of source compartments change
//	Parameters; volume ratio
//	Returns: nothing
//
//---------------------------------------------------------------------------

void mass_diffusion_path :: UpdateTransferRateConstants( reaction_compartment* pCompartment,
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
//		source compartments using list if const T & V and not gas phase
//         Parameters: selected event data structure
//         Returns: nothing
//---------------------------------------------------------------------------

void mass_diffusion_path :: UpdateProbabilities (selected_event_info& Event)
{    // begin

	UINT16              Counter, j;
	UINT32              NoSteps;
	update_step_info*   ListPtr;
     transfer_path*      pPath;


	if ( ConstT && ConstV && (AvgPressure == 0.0) )
	{
		// Constant T and V, liquid or solid phases only
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


	} else {    // transfer path not const T & V or is gas phase

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
//	Definition of method UpdatePathGeometry
//	Purpose: to update path geometry when the source or target compartment
//		has changed volume
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void mass_diffusion_path :: UpdatePathGeometry ()

{
	compartment_geometry 	SourceGeom, TargetGeom;

    // only do update if not connected to 2 constant volume transfer paths                           
                                                                                                     
    if ( !ConstV )                                                                                   
    {                                                                                                
                                                                                                     
	SourceGeom = pSourceCompartment->GetCompartmentGeometry();                                          
	TargetGeom = pTargetCompartment->GetCompartmentGeometry();                                          
                                                                                                     
	switch ( PathGeometry.Axis ) {                                                                      
                                                                                                     
	case X_AXIS:                                                                                        
		PathGeometry.Length = SourceGeom.X_TransferLength + TargetGeom.X_TransferLength;                   
		if ( SourceGeom.X_TransferArea <= TargetGeom.X_TransferArea )                                      
		{                                                                                                  
			PathGeometry.Area = SourceGeom.X_TransferArea;                                                    
		} else {                                                                                           
			PathGeometry.Area = TargetGeom.X_TransferArea;                                                    
		}                                                                                                  
		break;                                                                                             
                                                                                                     
	case Y_AXIS:                                                                                        
		PathGeometry.Length = SourceGeom.Y_TransferLength + TargetGeom.Y_TransferLength;                   
		if ( SourceGeom.Y_TransferArea <= TargetGeom.Y_TransferArea )                                      
		{                                                                                                  
			PathGeometry.Area = SourceGeom.Y_TransferArea;                                                    
		} else {                                                                                           
			PathGeometry.Area = TargetGeom.Y_TransferArea;                                                    
		}                                                                                                  
		break;                                                                                             
                                                                                                     
	case Z_AXIS:                                                                                        
		PathGeometry.Length = SourceGeom.Z_TransferLength + TargetGeom.Z_TransferLength;                   
		if ( SourceGeom.Z_TransferArea <= TargetGeom.Z_TransferArea )                                      
		{                                                                                                  
			PathGeometry.Area = SourceGeom.Z_TransferArea;                                                    
		} else {                                                                                           
			PathGeometry.Area = TargetGeom.Z_TransferArea;                                                    
		}                                                                                                  
		break;                                                                                             
                                                                                                     
	default:                                                                                            
		break;                                                                                             
                                                                                                     
	}                                                                                                   
                                                                                                     
   } // end if                                                                                       
                                                                                                     
   return;                                                                                           
                                                                                                     
}                                                                                                    
                                                                                                     

//---------------------------------------------------------------------------
//	Definition of method InitPathGeometry
//	Purpose: to initialize variable, setting to default values since this
//		path object is geometry-independent
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void mass_diffusion_path :: InitPathGeometry ()

{
	compartment_geometry 	SourceGeom, TargetGeom;

	SourceGeom = pSourceCompartment->GetCompartmentGeometry();
	TargetGeom = pTargetCompartment->GetCompartmentGeometry();

	switch ( PathGeometry.Axis ) {

	case X_AXIS:
		PathGeometry.Length = SourceGeom.X_TransferLength + TargetGeom.X_TransferLength;
		if ( SourceGeom.X_TransferArea <= TargetGeom.X_TransferArea )
		{
			PathGeometry.Area = SourceGeom.X_TransferArea;
		} else {
			PathGeometry.Area = TargetGeom.X_TransferArea;
		}
		break;

	case Y_AXIS:
		PathGeometry.Length = SourceGeom.Y_TransferLength + TargetGeom.Y_TransferLength;
		if ( SourceGeom.Y_TransferArea <= TargetGeom.Y_TransferArea )
		{
			PathGeometry.Area = SourceGeom.Y_TransferArea;
		} else {
			PathGeometry.Area = TargetGeom.Y_TransferArea;
		}
		break;

	case Z_AXIS:
		PathGeometry.Length = SourceGeom.Z_TransferLength + TargetGeom.Z_TransferLength;
		if ( SourceGeom.Z_TransferArea <= TargetGeom.Z_TransferArea )
		{
			PathGeometry.Area = SourceGeom.Z_TransferArea;
		} else {
			PathGeometry.Area = TargetGeom.Z_TransferArea;
		}
		break;

	default:
		break;

	}
	return;

}

//---------------------------------------------------------------------------
//	Definition of method SetConstPFlag
//	Purpose: to set option for diffusion path based on volume elements
//		it connects
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void mass_diffusion_path :: SetConstPFlag()

{
	// path is set to constant pressure in constructor

	if ( (pSourceCompartment->GetPhase() == GAS) && (pSourceCompartment->QueryPOption() == VARIABLE_PRESSURE) )
	{
		ConstP = FALSE;
	}

	if ( (pTargetCompartment->GetPhase() == GAS) && (pTargetCompartment->QueryPOption() == VARIABLE_PRESSURE) )
	{
		ConstP = FALSE;
	}

	return;

}



//---------------------------------------------------------------------------
//         Definition of operator >>
//         Purpose: to read in transfer path data
//         Parameters: addresses of input file and transfer path instance
//         Returns: nothing
//---------------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, mass_diffusion_path& rObject )

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

		    // link to mass diffusion steps
		    for ( n = 0; n < rObject.NumberOfSteps; n++ )
		    {
		       rObject.Step[ n ] = new mass_diffusion;
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
				rTIS >> *(( mass_diffusion*) rObject.Step[n] );

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
