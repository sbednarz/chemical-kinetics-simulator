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
//        Methods for derived class MASS_TRANSFER_PATH
//        Date last modified September 30, 1996
//        Written by Frances A. Houle
//        IBM  
//---------------------------------------------------------------------------


#include "masspath.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

mass_transfer_path :: mass_transfer_path ()
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
void mass_transfer_path :: UpdateAttachedVolumes()

{
	// update volumes of compartments and reaction rate constants,
	// as well as rate constants for transfer for all connected paths
	// including this one
	pSourceCompartment->UpdateVolume();
	pTargetCompartment->UpdateVolume();
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

void mass_transfer_path :: UpdateTransferRateConstants( reaction_compartment* pCompartment,
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

void mass_transfer_path :: InitPathGeometry ()

{
	PathGeometry.Length = 1;
	PathGeometry.Area = 1;
	PathGeometry.Axis = NONE;

	return;

}


//---------------------------------------------------------------------------
//         Definition of operator >>
//         Purpose: to read in transfer path data
//         Parameters: addresses of input file and transfer path instance
//         Returns: nothing
//---------------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, mass_transfer_path& rObject )

{
	UINT16 n, j, k, dummyint;
	FLOAT64 dummyfloat;
	UINT32 TotalNonRevSteps;
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
	       rObject.Step[ n ] = new mass_transfer;
		if ( rObject.Step[ n ] == 0 )
		{
			Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			return rTIS;
		}	    }


			// set up transfer steps

			n = 0;

			// read in sets of data, one per reaction step
			while ( n < rObject.NumberOfSteps )
			{
				// read in data
				rTIS >> *(( mass_transfer *)  rObject.Step[n] );

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
