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
//      Declaration of methods for INITIALIZE_AMORPHOUS_SYSTEM
//      Date last modified: December 16, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "initasys.hxx"


//---------------------------------------------------------------------------       
//                                                                                  
//	Definition of method SetUpSimulation                                             
//	Purpose: to initialize internal variables for simulation                         
//	Parameters: none                                                                 
//	Returns: nothing                                                                 
//                                                                                  
//---------------------------------------------------------------------------       
                                                                                    
                                                                                    
void initialize_amorphous_system :: SetUpSimulation()
                                                                                    
{                                                                                   
	// initialize connection data for transfer paths and compartments                  
	Simulator->SetUpConnections();                                                     
                                                                                    
	// calculate particles per mol                                                     
	Simulator->SetParticlesPerMol();                                                   
                                                                                    
	// initialize particles for all species                           
	Simulator->InitParticles();                                                   
                                                                                    
	// initialize T, P, V in system                                                    
	Simulator->InitStateVariables();                                                   
                                                                                    
	// convert constants to particle units in system                                   
	Simulator->InitRateConstants();                                                    
                                                                                    
	// initialize probabilities and calculate totals                                   
	Simulator->InitProbabilities();                                                    
                                                                                    
	// initialize equilibrium detect & emulation                                       
	Simulator->InitEquilEmulator();                                                    
                                                                                    
	// set up update object                                                            
	Simulator->SetUpUpdating();                                                        
                                                                                    
	return;                                                                            
                                                                                    
}                                                                                   
                                                                                    


//---------------------------------------------------------------------------
//
//	Definition of method ReadInData
//	Purpose: to read data from runparm file and allocate memory as needed
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------
void initialize_amorphous_system :: ReadInData()

{
     String name;


	//set up to read in data
	TextInputStream infile ( "runparm.tst" );

	//abort run if runparm file cannot be opened
	if ( !infile.IsOpen() )
	{
		Simulator->SetStatusCode( SIM_TERMINATE_FILE_ERROR );
		return;
	}

	//read first line of file into global variable DataCode
	infile >> DataCode;
	if (DataCode != IDType (START_INPUT_FILE) )
	{
		Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		return ;
	}

	while ( DataCode != IDType (END_INPUT_FILE) )
	// read in data identifiers one by one, and store data
	// the structure of this loop is to read in header and start
	// simulator read-in. Each obnject receiving data handles its own data
	// entry

	{
	  // check error status as program loops through
	  if ( Simulator->GetStatusCode() != SIM_RUNNING )
	  {
	       return;
	  }

		infile >> DataCode;

		switch (DataCode)
		{

		case START_APP_INIT:
			while ( DataCode != IDType (END_APP_INIT) )
			{
				infile >> DataCode;

		    //global struct init_data is a temporary home for some data in header - needs
		    //to be replaced with appropriate variable/object names

				switch (DataCode) {
				case OUT_FILE_NAME:
					infile >> name;
					Simulator->SetOutputFileName ( name );
					break;

				case PARENT_SCREEN_SIZE:
					infile >> HeaderData.win_size[1];
					infile >> HeaderData.win_size[2];
					break;

				case PARENT_SCREEN_POSN:
					infile >> HeaderData.win_pos[1];
					infile >> HeaderData.win_pos[2];
					break;

				default:
					if ( DataCode != IDType (END_APP_INIT) )
					{
						Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
						return;
					}
					break;
				} // end switch
			}  // end while
			break;

		case START_SIM_INIT:
			// cast to type of simulator used
			infile >> *( ( amorphous_system* )Simulator );
			break;

		default:
			if ( DataCode != IDType (END_INPUT_FILE) )
			{
				Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
				return;
			}
			break;
		};     // end switch

	}   //endwhile  DataCode != END_INPUT_FILE

}  //end method


//---------------------------------------------------------------------------
