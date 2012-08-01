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

//--------------------------------------------------------------------------
//
//	Declaration of class REACTION_COMPARTMENT
//	derived from base class PROCESS_AREA
//	This object is a virtual generic reaction system which can be connected
//		by transfer path objects to other reaction compartments
//	Date last modified: August 5, 1998
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef CMPRTMNT_HXX
#define CMPRTMNT_HXX

#include "procarea.hxx"
#include "rxn_ud_t.hxx"
#include "x_r_udl.hxx"
#include "reactant.hxx"
#include "binstrem.hxx"


class transfer_path;
class xfr_after_rxn_update_list;
class reaction_update_table;


class reaction_compartment : public process_area {

protected:
	reactant*                     ChemicalSpecies; // [NumberOfChemicalSpecies]
	UINT32			      NumberOfChemicalSpecies;
	FLOAT64                       MolarConversionFactor; // = ParticlesPerMol*InitVol for conc units

	UINT16			      NumberOfTransferPaths;
	UINT16*			      TransferPathID; // [NumberOfTransferPaths]
	transfer_path**               ConnectedTransferPath; // [NumberOfTransferPaths]
	reaction_compartment**        AdjacentArea; // [NumberOfTransferPaths]

	reaction_update_table*        ReactionUpdateTable;  // one of these
	xfr_after_rxn_update_list*    UpdateTransferAfterReaction; // [NumberOfSteps]
	enum TEMPERATURE_OPTION       T_option;
	enum VOLUME_OPTION            V_option;
	enum PRESSURE_OPTION          P_option;
	FLOAT64                       Volume;    //liters
	FLOAT64                       Pressure;
	enum COMPARTMENT_PHASE_TYPE   Phase;
	compartment_geometry	      Geometry;

public:
	reaction_compartment ();
	reaction_compartment ( const reaction_compartment& r_c );
	virtual ~reaction_compartment ();

	void           SetSpeciesCount (UINT16 SpeciesNo, ParticleType sc) {
			       ChemicalSpecies[SpeciesNo].SetParticles(sc); }
	ParticleType   GetSpeciesCount (UINT16 SpeciesNo) {
				  return ChemicalSpecies[SpeciesNo].GetParticles(); }
	FLOAT64        GetMolarConversionFactor () { return MolarConversionFactor; }

	compartment_geometry&	GetCompartmentGeometry() { return Geometry; }

	enum TEMPERATURE_OPTION  	QueryTOption () { return T_option; }
	void                     	SetVolume (FLOAT64 V) { Volume = V; }
	FLOAT64                  	GetVolume () { return Volume; }
	enum VOLUME_OPTION       	QueryVOption () { return V_option; }
	void                     	SetPressure (FLOAT64 P) { Pressure = P; }
	FLOAT64                  	GetPressure () { return Pressure; }
	enum PRESSURE_OPTION     	QueryPOption () { return P_option; }
	enum COMPARTMENT_PHASE_TYPE	GetPhase () { return Phase; }

	UINT16    	     GetNoTransferPaths() { return NumberOfTransferPaths; }
	UINT16	  	     GetConnectedPathID( UINT16 n ) { return TransferPathID[n]; }
	transfer_path* 	     GetConnectedPathPtr( UINT16 n ) { return ConnectedTransferPath[n]; }

	void      	UpdateProbabilities(selected_event_info& Event);
	void      	UpdateAllConnectedTransferPathProbs( UINT16 TransferPathNo );
	void      	UpdateChemicalSpecies(selected_event_info& Event); // after reactions
	void      	UpdateChemicalSpecies(UINT16 SpeciesNumber, UINT16 Stoichiometry, BOOL8 Add); // after transfer
	void		UpdateConcentrations( FLOAT64 VolumeRatio );
	virtual void	UpdateGeometry() {}
	virtual void 	UpdateVolume() {}
	void            UpdatePressure();

	// the following are for initialization of the compartment
	void	  	SetConnectedPathPtr( UINT16 n, transfer_path* ptr ) { ConnectedTransferPath[n] = ptr; }
	void      	InitSpeciesCount ();
	FLOAT64   	CalcInitialNoOfMoles();
	virtual void 	InitGeometry() {}
	void      	InitPressure ();
	void		InitVolume();
	void      	Connect();
	void      	InitUpdateTables();

	friend    BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, reaction_compartment& rObject );

 };

 BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, reaction_compartment& rObject );



 #endif

