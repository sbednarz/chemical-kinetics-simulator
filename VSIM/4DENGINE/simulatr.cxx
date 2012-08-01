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
//      Methods for class SIMULATION_SYSTEM
//      Date last modified: January 23, 1997
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#include "simulatr.hxx"


//---------------------------------------------------------------------------
//	Constructor
//---------------------------------------------------------------------------

simulation_system :: simulation_system ()

{

     TransferPath = 0;
     Compartment = 0;
     TotalProbability = 0.0;
     TotalNumberOfParticles = 0;

     NumberOfCompartments = 0;
     NumberOfTransferPaths = 0;
     NumberOfChemicalSpecies = 0;

     EquilEmulatorOn = FALSE;

     ParticlesPerMol = 0.0;

}


//---------------------------------------------------------------------------
//
//      Definition of method CalcTotalProbability
//      Purpose: To calculate total probability for all events in the simulation system
//      Parameters: none
//      Returns: nothing
//
//---------------------------------------------------------------------------

void simulation_system :: CalcTotalProbability (void)
{     // begin method

	UINT16 n;
	FLOAT64 RunningTotal = 0.0;

	for (n = 0; n < NumberOfCompartments ; n++ )
	{
	     RunningTotal += Compartment[n]->GetTotalProbability();
	}

	for (n = 0; n < NumberOfTransferPaths ; n++ )
	{
	   RunningTotal += TransferPath[n]->GetTotalProbability();
	}

	TotalProbability = RunningTotal;

     // check for zero probability
     if ( TotalProbability == 0.0 )
     {
	  SetStatusCode ( SIM_TERMINATE_ZERO_PROBABILITY );
     }

	return;

}       // end method


//---------------------------------------------------------------------------
//
//	Definition of method FreezeEquilibrium
//	Purpose: to set probabilities of equilibrated steps to zero
//		and adjust prob subtotals
//	Parameters: reference to equil step data
//	Returns: flag to indicate whether non-frozen probabilities > 0
//
//---------------------------------------------------------------------------

BOOL8  simulation_system :: FreezeEquilibrium( equil_detect_info& EquilStep )

{
	FLOAT64		DeltaProbability;


	switch ( EquilStep.EventInfo.Location ) {

	case REACTION_COMPARTMENT:
		DeltaProbability = Compartment[EquilStep.EventInfo.ProcessAreaNo]->
			FreezeSteps( EquilStep.EventInfo.EventNo, EquilStep.PartnerStepNo );
		break;

	case MASS_TRANSFER:
		DeltaProbability = TransferPath[EquilStep.EventInfo.ProcessAreaNo]->
			FreezeSteps( EquilStep.EventInfo.EventNo, EquilStep.PartnerStepNo );
		break;

	default:
		break;

	}  // end switch

	// adjust system probability
	TotalProbability -= DeltaProbability;

	if ( TotalProbability > 0.0 )
	{
		return TRUE;

	} else {

		return FALSE;
	}


}


//---------------------------------------------------------------------------
//
//	Definition of method RestoreEquilibrium
//	Purpose: to reactivate equilibrated steps that had been frozen and
//		adjust probabilities of process areas - use after prob update
//	Parameters: reference to equil step data
//	Returns: nothing
//
//---------------------------------------------------------------------------

void simulation_system :: RestoreEquilibrium ( equil_detect_info& EquilStep )

{
	FLOAT64		DeltaProbability;


	switch ( EquilStep.EventInfo.Location ) {

	case REACTION_COMPARTMENT:
		DeltaProbability = Compartment[EquilStep.EventInfo.ProcessAreaNo]->
			RestoreSteps( EquilStep.EventInfo.EventNo, EquilStep.PartnerStepNo );
		break;

	case MASS_TRANSFER:
		DeltaProbability = TransferPath[EquilStep.EventInfo.ProcessAreaNo]->
			RestoreSteps( EquilStep.EventInfo.EventNo, EquilStep.PartnerStepNo );
		break;

	default:
		break;

	}  // end switch

	// adjust system probability
	TotalProbability += DeltaProbability;


}

//---------------------------------------------------------------------------
//        Definition of method Terminate
//        Purpose: to write final data to file
//        Parameters: none
//        Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: Terminate ()

{

     // retrieve and store final exit code
     outfile.MoveTo( SIMSTATUS_CODE_FILE_POSITION );
     outfile << (UINT16) ( Status.GetCode() );

     // retrieve and store number of data points
     outfile.MoveTo( SIMNUM_POINTS_FILE_POSITION );
     outfile << Status.GetNoRecords();

     return;
}


//---------------------------------------------------------------------------
//   Definition of method SetProgVarPointersInTimebase
//   Purpose: to set required pointers for synchronizing programmed
//        variables to timebase
//   Parameters: pointer to profile object, pointer to process area containing it
//   Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: SetProgVarPointersInTimebase( profile* p, process_area* r )
{
     BOOL8     flag = TRUE;

     Time.SetProgrammedVariableFlag( flag );
     Time.SetProgVariableLocation( r );
     Time.SetProgVariablePointer( p );
     Time.SetIdlerPointers( p );

     return;

}


//---------------------------------------------------------------------------
//	Definition of method SetParticlesPerMol
//	Purpose: to determine the total initial concentration in the system
//		and calculate the conversion factor for moles/l to particles
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: SetParticlesPerMol ()

{
	UINT16 j;
	FLOAT64 total;

	// calculate particles per mol conversion factor for the entire system
	total = 0.0;

	for ( j=0; j<NumberOfCompartments; j++ )
	{
	      total += Compartment[j]->CalcInitialNoOfMoles ();
	}

	ParticlesPerMol = (FLOAT64) TotalNumberOfParticles / total;

	return;

}


//---------------------------------------------------------------------------
//	Definition of method ApportionParticles
//	Purpose: to convert initial amounts of chemical species to particles
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: InitParticles ()

{
	UINT16 j;

	for ( j=0; j<NumberOfCompartments; j++ )
	{
	     Compartment[j]->InitSpecies();
	}

}

//---------------------------------------------------------------------------
//	Definition of method InitSystemGeometry
//	Purpose: to calculate required geometrical quantities for compartments
//		and transfer paths
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------
void simulation_system :: InitSystemGeometry()

{
	UINT16 j;

	for ( j=0; j<NumberOfCompartments; j++ )
	{
		Compartment[j]->InitGeometry();
	}

	for ( j=0; j<NumberOfTransferPaths; j++ )
	{
		TransferPath[j]->InitPathGeometry();
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitStateVariables
//	Purpose: to initialize V and P for compartments and T for
//		transfer paths
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: InitStateVariables ()

{
	UINT16 j;

	for ( j=0; j<NumberOfCompartments; j++ )
	{
		Compartment[j]->InitTPV();
	}

	for ( j=0; j<NumberOfTransferPaths; j++ )
	{
		TransferPath[j]->CalcTemperature();
		TransferPath[j]->SetConstTFlag();
		TransferPath[j]->SetConstVFlag();
		TransferPath[j]->CalcPressure();
		TransferPath[j]->SetConstPFlag();
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method InitRateConstants
//	Purpose: to convert units of rate constants from moles/liter-sec to
//		particles/sec in compartments and transfer paths, and then
//		pre-calculate rate constants for cases where T is constant
//		and Arrhenius parameters are used
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: InitRateConstants ()

{
	UINT16 j;

	// get molar conversion factor for each compartment and initialize ks
	for ( j=0; j<NumberOfCompartments; j++ )
	{
		Compartment[j]->InitRateConstants();
	}

	for ( j=0; j<NumberOfTransferPaths; j++ )
	{
		TransferPath[j]->InitRateConstants();
	}

	return;

}


//---------------------------------------------------------------------------
//	Definition of method SetUpConnections
//	Purpose: to store pointers to connected compartments in multi-compartment
//        systems
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: SetUpConnections ()

{
	UINT16 j, k, NoPaths, PathID;

     // first store transfer path pointers in each compartment, then have compartments
     // finish the configuration

	for ( j=0; j<NumberOfCompartments; j++ )                                            
	{                                                                                   
		NoPaths = Compartment[j]->GetNoTransferPaths();                                    
                                                                                     
		if ( NoPaths )                                                                     
		{                                                                                  
			for ( k=0; k<NoPaths; k++ )                                                       
			{                                                                                 
				PathID = Compartment[j]->GetConnectedPathID( k );                                
				Compartment[j]->SetConnectedPathPtr( k, TransferPath[PathID] );                  
			}                                                                                 
                                                                                     
			Compartment[j]->Connect();                                                        
		}                                                                                  
                                                                                     
	}                                                                                   


	return;
}


//---------------------------------------------------------------------------
//	Definition of method InitProbabilities
//	Purpose: to calculate initial reaction and transport probabilities
//		in compartments and transfer paths
//	Parameters: none
//	Returns: nothing
//---------------------------------------------------------------------------

void simulation_system :: InitProbabilities ()

{
	UINT16 j;

	for ( j=0; j<NumberOfCompartments; j++ )
	{
		Compartment[j]->CalcAllProbabilities();
	}

	for ( j=0; j<NumberOfTransferPaths; j++ )
	{
		TransferPath[j]->CalcAllProbabilities();
	}

	CalcTotalProbability();

	return;

}


//---------------------------------------------------------------------------
//
//   Definition of method InitEquilEmulator
//   Purpose: to set up emulator data if equil detect option is selected
//   Parameters: none
//   Returns: nothing
//
//---------------------------------------------------------------------------

void simulation_system :: InitEquilEmulator ()
{
     UINT32    j;
     UINT32    NoRevSteps;

     if ( EquilEmulatorOn )
     {
          NoRevSteps = 0;

          for ( j=0; j<NumberOfCompartments; j++ )
          {
	       NoRevSteps += Compartment[j]->GetNoRevSteps();
          }

          for ( j=0; j<NumberOfTransferPaths; j++ )
          {
	       NoRevSteps += TransferPath[j]->GetNoRevSteps();
          }

	  EquilEmulator.InitEventHistory( NoRevSteps, Status.GetEquilDetectCycleLength() );

     }

     return;

}


//---------------------------------------------------------------------------
//
//	Definition of method SetUpUpdating
//	Purpose: to put in place data structures for system updating
//	Parameters: none
//	Returns: nothing
//
//---------------------------------------------------------------------------

void simulation_system :: SetUpUpdating ()

{
	UINT16 j;

	// set up update machine
	Updater.Build();

	// set up update tables for const T, const V simulations -
	// do for each compartment
	for ( j = 0; j < NumberOfCompartments; j++ )
	{
		Compartment[j]->InitUpdateTables ();
	}

	// and then for each transfer path if there are any
	for ( j = 0; j < NumberOfTransferPaths; j++ )
	{
		TransferPath[j]->InitUpdateTables ();
	}


	return;

}


//---------------------------------------------------------------------------
//
//	Definition of binary output operator >>
//	Purpose: to write time and compartment state data to output file
//	Parameters: pointers to file and object
//	Returns: pointer
//
//---------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_system& rObject )

{
     UINT16 	j;
     FLOAT64	SimTime;

     SimTime = rObject.GetTime();

     // write time
     rBOS << SimTime;

     // write time to screen as visual indicator that simulation is running - this is temporary
     // until engine gets its own user interface
     cprintf ("The current time is %g\r\n", SimTime);

     // write compartment data
     for ( j=0; j < rObject.NumberOfCompartments; j++ )
     {
	  rBOS << *( rObject.Compartment[j] );
     }

     return rBOS;

}

//---------------------------------------------------------------------------

