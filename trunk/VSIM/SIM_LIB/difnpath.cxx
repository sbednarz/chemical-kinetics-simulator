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
//        Date last modified July 17, 1998
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
			Step[j]->UpdateRateCoefficients( CmptGeometry.VolumeRatio );
		}

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

		// set up transfer steps

		n = 0;

		// read in sets of data, one per reaction step
		while ( n < rObject.NumberOfSteps )
		{
			// check data code to see what type of diffusion step is being used -
			// mass diffusion or virtual diffusion

			rTIS >> DataCode;
			switch (DataCode) {

			case XFER_MASS_DIFN:
			//link to mass diffusion object
			rObject.Step[ n ] = new mass_diffusion;

			// check for allocation error
			if ( rObject.Step[ n ] == 0 )
				{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
				}
			// read in data
			rTIS >> *(( mass_diffusion*) rObject.Step[n] );

			// store pointer to process area containing step
			rObject.Step[n]->SetPointer( &rObject );

			// if reversible step link partner step and copy data into it

			if ( rObject.Step[n]->QueryDirection() == REVERSIBLE )
			{
				k = n + 1;

				rObject.Step[ k ] = new mass_diffusion;

				// check for allocation error
				if ( rObject.Step[ k ] == 0 )
				{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
				}
				rObject.Step[n]->SetPartnerStepNo( k );

				// store pointer to process area containing reverse step
				rObject.Step[k]->SetPointer( &rObject );

				// move data from temporary storage in reaction n into reaction k data structures
				// and release temporary storage
				rObject.Step[k]->MoveTempStepData( n );
				rObject.Step[k]->MoveTempKData( n );

				// increment n to keep array index on track
				n++;

			} // end if Step[n]
			break;

			case XFER_VIRT_DIFN:
			//link to virtual diffusion object
			rObject.Step[ n ] = new virtual_diffusion;

			// check for allocation error
			if ( rObject.Step[ n ] == 0 )
				{
				Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				return rTIS;
				}
			// read in data
			rTIS >> *(( virtual_diffusion*) rObject.Step[n] );

			// store pointer to process area containing step
			rObject.Step[n]->SetPointer( &rObject );

			// if reversible step link partner step and copy data into it

			if ( rObject.Step[n]->QueryDirection() == REVERSIBLE )
			{
				k = n + 1;

				rObject.Step[ k ] = new virtual_diffusion;

				// check for allocation error
				if ( rObject.Step[ k ] == 0 )
					{
					Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
					return rTIS;
					}
				rObject.Step[n]->SetPartnerStepNo( k );

				// store pointer to process area containing reverse step
				rObject.Step[k]->SetPointer( &rObject );

				// move data from temporary storage in reaction n into reaction k data structures
				// and release temporary storage
				rObject.Step[k]->MoveTempStepData( n );
				rObject.Step[k]->MoveTempKData( n );

				// increment n to keep array index on track
				n++;

			} // end if Step[n]
			break;

			default:
				break;

			} // end switch

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
