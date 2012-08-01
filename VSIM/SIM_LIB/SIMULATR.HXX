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
//      Declaration of SIMULATION_SYSTEM
//      This base class contains a core set of objects and methods
//		necessary to run a simulation of a chemical reaction in
//		multiple compartments with linking transfer paths. Derived classes
//		are intended to be the working classes
//      Date last modified: October 9, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef SIMULATR_HXX
#define SIMULATR_HXX

#include "vsimtype.hxx"
#include "constant.hxx"
#include "cmprtmnt.hxx"
#include "xferpath.hxx"
#include "random.hxx"
#include "timebase.hxx"
#include "ud_mach.hxx"
#include "sim_stat.hxx"
#include "selector.hxx"
#include "out_obj.hxx"
#include "txtstrem.hxx"
#include "binstrem.hxx"
#include "eq_emul.hxx"
#include "prop_lib.hxx"
#include "assert.hxx"
#include <math.h>


class simulation_system {              //call this "Simulator"

protected:
     transfer_path**          TransferPath; //[NumberOfTransferPaths]
     reaction_compartment**   Compartment; //[NumberOfCompartments]
     FLOAT64                  TotalProbability;
     ParticleType             TotalNumberOfParticles;
     RandomNumberSeries       RandomNum;
     UINT16                   NumberOfCompartments;
     UINT16                   NumberOfTransferPaths;
     UINT16                   NumberOfChemicalSpecies;
     simulation_time          Time;
     selector                 EventSelector;
     update_machine           Updater;
     simulation_status        Status;
     output_object            Output;
     equilibrium_emulator     EquilEmulator;
     props_library            SpeciesDataTable;
     BOOL8                    EquilEmulatorOn;
     FLOAT64                  ParticlesPerMol;

public:
     simulation_system ();
     simulation_system ( const simulation_system& s_s );
     virtual ~simulation_system() {}

     FLOAT64   GetParticlesPerMol () { return ParticlesPerMol; }
     UINT16    GetNoOfCompartments () { return NumberOfCompartments; }
     UINT16    GetNoOfTransferPaths () { return NumberOfTransferPaths; }
     UINT16    GetNoOfChemicalSpecies () { return NumberOfChemicalSpecies; }
     UINT32    GetNoStepsInCompartment( UINT16 ProcessAreaNo ) {
			return Compartment[ProcessAreaNo]->GetNoSteps(); }
     UINT32    GetNoStepsInTransferPath( UINT16 TransferPathNo ) {
			return TransferPath[TransferPathNo]->GetNoSteps(); }

     process_info&	GetStepDataFromCompartment( UINT16 ProcessAreaNo, UINT32 StepNo ) {
				return Compartment[ ProcessAreaNo ]->GetStepInfo( StepNo ); }
     process_info&	GetStepDataFromTransferPath( UINT16 TransferPathNo, UINT32 StepNo ) {
				return TransferPath[ TransferPathNo ]->GetStepInfo( StepNo ); }

     void      CalcTotalProbability ();
     FLOAT64   GetTotalProbability () { return TotalProbability; }
     void      SetTotalProbability ( FLOAT64 tp ) { TotalProbability = tp; }

     void                     SelectEvent () { EventSelector.Select(); }
     selected_event_info&     GetEventInfo () {return EventSelector.GetSelectedEventInfo();}

     BOOL8     QueryEmulatorOn() { return EquilEmulatorOn; }
     void      RecordEventForEquilDetect() { EquilEmulator.LogEvent(); }
     BOOL8     CheckForEquilibration() { return EquilEmulator.CheckEventPatternForEquilibrium(); }
     BOOL8     QueryEquilibriumDetected() { return EquilEmulator.QueryEquilibrium(); }
     BOOL8     FreezeEquilibrium( equil_detect_info& EquilStep );
     void      RestoreEquilibrium( equil_detect_info& EquilStep );
     void      ResetEmulationCycle() { EquilEmulator.StartNewCycle(); }

     FLOAT64   GetRandomNumber () { return RandomNum.NextValue(); }

     void      SetTime () { Time.SetNewTime(); }
     FLOAT64   GetTime () { return Time.GetTime(); }

     properties&	GetMaterialsProperties( UINT16 SpeciesNo ) {
				return SpeciesDataTable.GetProperties( SpeciesNo ); }

     FLOAT64   GetCompartmentProbability (UINT16 ProcessAreaNo) {
			return Compartment[ProcessAreaNo]->GetTotalProbability();}
     FLOAT64   GetTransferPathProbability (UINT16 TransferPathNo) {
			return TransferPath[TransferPathNo]->GetTotalProbability();}
     FLOAT64   GetStepProbFromCompartment (UINT16 ProcessAreaNo, UINT32 ReactionStepNo) {
			return Compartment[ProcessAreaNo]->GetStepProbability(ReactionStepNo); }
     FLOAT64   GetStepProbFromTransferPath (UINT32 TransferPathNo, UINT32 TransferStepNo) {
			return TransferPath[TransferPathNo]->GetStepProbability(TransferStepNo);}

     void      UpdateSystem () { Updater.UpdateTheSystem(); }
     void      UpdateEventCounters () { Status.UpdateStatus(); }
     void      UpdateChem ( UINT16 ProcessAreaNo, selected_event_info& Event) {  //after reaction
			     Compartment[ProcessAreaNo]->UpdateChemicalSpecies (Event);}
     void      UpdateMassTransfer ( UINT16 TransferPathNo, selected_event_info& Event) {
			TransferPath[TransferPathNo]->UpdateTransferredSpecies (Event);}
     void      UpdateVolumeAfterReaction ( UINT16 ProcessAreaNo ) {
			Compartment[ProcessAreaNo]->UpdateVolume(); }
     void      UpdateVolumeAfterTransfer ( UINT16 ProcessAreaNo ) {
			TransferPath[ProcessAreaNo]->UpdateAttachedVolumes(); }
     void      UpdateCompartmentProbs (UINT16 ProcessAreaNo, selected_event_info& Event) {
			Compartment[ProcessAreaNo]->UpdateProbabilities (Event);}
     void      UpdateTransferPathProbs (UINT16 TransferPathNo, selected_event_info& Event) {
			TransferPath[TransferPathNo]->UpdateProbabilities (Event);}

     void                RecordState () { Output.WriteState (); }
     void                SetStatusCode ( SIM_STATUS_CODE s ) { Status.SetCode( s ); }
     SIM_STATUS_CODE     GetStatusCode () { return Status.GetCode(); }
     BOOL8               IsSimulationOver () { return Status.IsReactionOver(); }
     void                Terminate();

     // the following are for initialization of the simulation
     void      SetOutputFileName ( String f ) { Output.SetFileName ( f ); }
     void      OpenOutputFile () { Output.OpenFile (); }
     void      SetParticlesPerMol ();
     void      ApportionParticles ();
     void      InitSystemGeometry();
     void      InitStateVariables ();
     void      InitRateConstants ();
     void      SetUpConnections ();
     void      InitProbabilities ();
     void      InitEquilEmulator ();
     void      SetUpUpdating ();

     friend    BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_system& rObject );

};

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, simulation_system& rObject );

extern    simulation_system*   Simulator;

#endif



