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
//      Definition of methods for OUTPUT_OBJECT
//      Date last modified: March 7, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "out_obj.hxx"
#include "simulatr.hxx"


//---------------------------------------------------------------------------
//   Constructor
//---------------------------------------------------------------------------

output_object :: output_object ()

{
     TimeUnits = TIME_UNITS_SEC;
     FileOffset = 0;
     OutFileName = "";

}


//---------------------------------------------------------------------------
//        Definition of method OpenFile
//        Purpose: to open file for output
//        Parameters: none
//        Returns: nothing
//---------------------------------------------------------------------------

void output_object :: OpenFile ()

{
     BOOL8 error;

     // open output file
     error = outfile.SetName ( OutFileName );
     if ( ! error )
     {
	  Simulator->SetStatusCode( SIM_TERMINATE_FILE_ERROR );
          return;
     }
     // move to end of file
     error = outfile.MoveTo ( FileOffset );
     if ( ! error )
     {
	  Simulator->SetStatusCode( SIM_TERMINATE_FILE_ERROR );
          return;
     }

     // write data code for output
     outfile << IDType ( SIMULATION_DATA );

     return;

}

//---------------------------------------------------------------------------
//        Definition of method WriteState
//        Purpose: to record current state of simulation system
//        Parameters: none
//        Returns: nothing
//---------------------------------------------------------------------------

void output_object :: WriteState ()

{
     // write data code
     outfile << IDType ( SIMULATION_DATA_RECORD );

     // write time and compartment data
     outfile << *( Simulator );

     return;

}

 //---------------------------------------------------------------------------
//	Definition of text input operator >>
//	Purpose: to read some data from an input file into class data members
//		using global DataCode
//	Parameters: input bytes and output object address
//	Output: none
//---------------------------------------------------------------------------

TextInputStream& operator >> ( TextInputStream& rTIS, output_object& rObject )

{
	switch (DataCode) {

	case SIM_TIME_BASE:
		// read enum into temp variable and cast to type
		UINT16 temp;
		rTIS >> temp;
		rObject.TimeUnits = ( ELAPSED_TIME_UNITS )temp;
		break;

	case SIM_DATA_FILE_OFFSET:
		rTIS >> rObject.FileOffset;
		break;

	default:
		Simulator->SetStatusCode( SIM_TERMINATE_INPUT_ERROR );
		break;

	}  //end switch

     return rTIS;

}

//---------------------------------------------------------------------------
