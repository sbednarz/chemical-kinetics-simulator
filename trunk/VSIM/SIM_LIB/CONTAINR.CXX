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
// Date last modified: January 14, 1997
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
			Geometry.VolumeRatio = 1.0;
		}

		// update rate constants if there are any chemical species left
		if ( Volume != OldV )
		{
			Geometry.VolumeRatio = Volume/OldV;
			UpdateConcentrations( Geometry.VolumeRatio );

			// update transfer path step rate constants. Method called only
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
//	Definition of method InitGeometry
//	Purpose: to initialize geometry of the system
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void shapeless_container :: InitGeometry()

{
	// all quantities initialized to 1 for this process area class

	Geometry.x = 1;
	Geometry.y = 1;
	Geometry.z = 1;
	Geometry.X_TransferLength = 1;
	Geometry.Y_TransferLength = 1;
	Geometry.Z_TransferLength = 1;
	Geometry.X_TransferArea = 1;
	Geometry.Y_TransferArea = 1;
	Geometry.Z_TransferArea = 1;
	Geometry.VolumeRatio = 1;
	Geometry.UnconstrainedAxis = NONE;

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

     // process area type is set by default to COMPARTMENT_TYPE_CONTAINER in the
     // process_area constructor - do not explicitly set type during this read-in

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
	       rTIS >> rObject.Volume;
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

