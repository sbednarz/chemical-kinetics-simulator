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
//
//      Methods for class AMORPHOUS_SYSTEM
//      Date last modified: September 27, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "amrphsys.hxx"


//---------------------------------------------------------------------------
//	Destructor
//---------------------------------------------------------------------------

amorphous_system :: ~amorphous_system ()

{
     delete [] ( TransferPath );
     delete [] ( Compartment );
}


//---------------------------------------------------------------------------
//
//	Definition of text input operator >>
//	Purpose: to read data from runparm file into simulation_system object
//		and allocate memory as needed
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------
TextInputStream& operator >> ( TextInputStream& rTIS, amorphous_system& rObject )

{

	UINT16 n, AreaNo;

     // set simulation type
     rObject.SimulatorType = AMORPHOUS_SYSTEM;

	// read data into global variable DataCode for typing, then into objects

	while ( DataCode != IDType (END_SIM_INIT) )
	{
	  // check error status with each loop through & break out if error
	  if ( rObject.Status.GetCode() != SIM_RUNNING )
	  {
	       return rTIS;
          }

          // proceed
		rTIS >> DataCode;

		switch (DataCode) {

		case SIM_TIME_BASE:
			rTIS >> rObject.Output;
			break;

		case NUM_PARTICLES:
			rTIS >> rObject.TotalNumberOfParticles;
			break;

		case WRITE_INTERVAL:
			rTIS >> rObject.Status;
			break;

		case RAND_NUM_SEED:
			rTIS >> rObject.RandomNum;
			break;

		case MAX_EVENTS:
			rTIS >> rObject.Status;
			break;

		case ELAPSED_TIME_LIMIT:
			rTIS >> rObject.Status;
			break;

		case EQUIL_DETECT_STATE:
			rTIS >> rObject.EquilEmulatorOn;
			break;

		case EQUIL_DETECT_MIN_EFF:
			rTIS >> rObject.EquilEmulator;
			break;

		case EQUIL_DETECT_CYCLE_LEN:
			rTIS >> rObject.Status;
			break;

		case SIM_DATA_FILE_OFFSET:
			rTIS >> rObject.Output;
			break;

		case NUM_SPECIES_IN_DATA_TABLE:
			rTIS >> rObject.NumberOfChemicalSpecies;
			break;

		case NUMBER_OF_COMPARTMENTS:
			rTIS >> rObject.NumberOfCompartments;

		       // allocate memory for pointers to compartments
		       rObject.Compartment = new reaction_compartment *[ rObject.NumberOfCompartments ];
		       if ( rObject.Compartment == 0 )
		       {
			    Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
			    return rTIS;

		       }

		       // link to compartments
		       for ( n = 0; n < rObject.NumberOfCompartments; n++ )
		       {
				rObject.Compartment[n] = new shapeless_container;
			}
		       break;

		case NUMBER_OF_TRANSPORT_PATHS:
			rTIS >> rObject.NumberOfTransferPaths;

		       // allocate memory for pointers to transfer paths if non-zero
		       if ( rObject.NumberOfTransferPaths )
		       {
			    rObject.TransferPath = new transfer_path *[ rObject.NumberOfTransferPaths ];
			    if ( rObject.TransferPath == 0 )
			    {
				 Simulator->SetStatusCode( SIM_TERMINATE_MEM_ALLOC_ERROR );
				 return rTIS;
			    }

		       // link to transfer paths
		       for ( n = 0; n < rObject.NumberOfTransferPaths; n++ )
		       {
				rObject.TransferPath[n] = new mass_transfer_path;
			}
		       break;

		       }
			break;

		case START_COMPARTMENT_INIT:
	       //check to be sure memory for compartments has been allocated. If not
	       //this means the input file is out of proper order
			ASSERT ( rObject.NumberOfCompartments != 0 );

	       // check that the number of species has been read in, if not the file is
	       // out of order
			ASSERT ( rObject.NumberOfChemicalSpecies != 0 );

	       //proceed to build compartment
		// read in first item in line of data
			rTIS >> DataCode;

	       // if datum is not compartment index abort read-in - file out of order
		       ASSERT ( DataCode == IDType (COMPARTMENT_INDEX) );

	       // proceed: read in compartment index, label compartment, then build compartment

		       rTIS >> n;
		       rObject.Compartment[n]->SetID( n );
		       rTIS >> *( (shapeless_container*) rObject.Compartment[n] );
			break;

		case START_TRANSFER_OBJECT_INIT:
	       //check to be sure memory for transfer paths has been allocated. If not
	       //this means the input file is out of proper order. It is ok to have no
	       //transfer paths, but if none not OK to read into them
			ASSERT ( rObject.NumberOfTransferPaths != 0 );

		// check to be sure that memory for compartments has been allocated. Array
		// addresses will be needed. If not the input file is out of proper order
			ASSERT ( rObject.NumberOfCompartments != 0 );

		// read in first item in line of data
			rTIS >> DataCode;

	       // if datum is not path index abort read-in - file out of order
		       ASSERT ( DataCode == IDType (TRANSFER_OBJECT_INDEX) );

	       // proceed: read in path index, label path, then build transfer path
		       rTIS >> n;
		       rObject.TransferPath[n]->SetID( n );
		       rTIS >> *( (mass_transfer_path*) rObject.TransferPath[n] );

		       // set addresses of source and target compartments - also sets addresses for steps
		       AreaNo = rObject.TransferPath[n]->GetSourceID();
		       rObject.TransferPath[n]->SetSourceCompartmentPtr( rObject.Compartment[AreaNo] );
		       AreaNo = rObject.TransferPath[n]->GetTargetID();
		       rObject.TransferPath[n]->SetTargetCompartmentPtr( rObject.Compartment[AreaNo] );
		 break;

		case START_SPECIES_DATA_TABLE_INIT:
		       rTIS >> rObject.SpeciesDataTable;
			break;

		default:
			if ( DataCode != IDType (END_SIM_INIT) )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
			}
			break;

		}  //end switch

	} // end while

     return rTIS;

} // end method

//---------------------------------------------------------------------------

