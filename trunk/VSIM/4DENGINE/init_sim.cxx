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
//      Declaration of methods for INITIALIZE_SIMULATOR
//      Date last modified: September 23, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "init_sim.hxx"

// instantiate output file
BinaryOutputStream outfile;
init_data HeaderData;

// instantiate variable for decoding data types while reading runparm file

IDType DataCode;



//---------------------------------------------------------------------------
//
//	Definition of method Go
//	Purpose: to manage simulation setup process
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void initialize_simulator :: Go ()

{

	ReadInData();
	if ( Simulator->GetStatusCode() != SIM_RUNNING )
		{
		  return;
		}

	SetUpSimulation();
	if ( Simulator->GetStatusCode() != SIM_RUNNING )
		{
		  return;
		}

	SetUpOutputFile();
	if ( Simulator->GetStatusCode() != SIM_RUNNING )
		{
		  return;
		}


	return;
}


//---------------------------------------------------------------------------
//        Definition of method SetUpOutputFile
//        Purpose: to open output file for append and save initial state
//             of simulation system
//        Parameters: none
//        Returns: nothing
//---------------------------------------------------------------------------

void initialize_simulator :: SetUpOutputFile ()

{
	Simulator->OpenOutputFile ();
	if ( Simulator->GetStatusCode() != SIM_RUNNING )
		{
			return;
		}

	Simulator->RecordState ();
	return;

}



//---------------------------------------------------------------------------


