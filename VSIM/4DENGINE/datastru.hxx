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

//---------------------------------------------------------------------//
//    DATASTRU.HXX
//    Data structures defined for simulation engine
//    Date last modified: January 23, 1997
//    Written by Frances Houle
//    IBM  
//---------------------------------------------------------------------//


#ifndef DATASTRU_HXX
#define DATASTRU_HXX

#include "vsimcons.hxx"
#include "constant.hxx"
#include "vsimtype.hxx"

class process_area;



enum PROCESS_AREA_TYPE { REACTION_COMPARTMENT, MASS_TRANSFER, CONTAINER };

enum SIMULATOR_TYPE { AMORPHOUS_SYSTEM, THREE_D_SYSTEM };

typedef	unsigned long ParticleType;


struct process_info {             //all members constant for the simulation
	UINT16 			numreactants, numproducts;
	UINT16 			ReactantsInStep[MAX_NUMBER_OF_COMPONENTS];
	UINT16 			ReactantStoich[MAX_NUMBER_OF_COMPONENTS];
	UINT16 			ProductsInStep[MAX_NUMBER_OF_COMPONENTS];
	UINT16 			ProductStoich[MAX_NUMBER_OF_COMPONENTS];
	UINT16 			numSpeciesInRateLaw;
	UINT16 			SpeciesInRateLaw[MAX_NUMBER_OF_COMPONENTS];
	FLOAT64 		     ReactantOrder[MAX_NUMBER_OF_COMPONENTS];
	enum XFER_DIRECTION Direction;
     BOOL8               Reversible;
	UINT16 			PartnerStepNo;
};


struct properties {
     UINT16 ID;
     enum COMPARTMENT_PHASE_TYPE  Phase;
	FLOAT64                      Density;    // moles/liter
	FLOAT64                      Enthalpy;   // kcal/mol
	FLOAT64                      CoeffA;     // cal/deg-mol
	FLOAT64                      CoeffB;
	FLOAT64                      CoeffC;
	FLOAT64                      CoeffD;
};

struct compartment_geometry {     // cm units except for volume which is in liters
     FLOAT64	x;
     FLOAT64	y;
     FLOAT64	z;
     FLOAT64   Area;   // for 2d interface compartments
     FLOAT64   Volume; // for 3d compartments
     FLOAT64	X_TransferLength; // x/2
     FLOAT64	Y_TransferLength; // y/2
     FLOAT64	Z_TransferLength; // z/2
     FLOAT64	X_TransferArea;   // y*z
     FLOAT64	Y_TransferArea;   // x*z
     FLOAT64	Z_TransferArea;   // x*y
     FLOAT64   SizeRatio;       // new/old volume or area for units update 
     enum DIRECTION UnconstrainedAxis;
};

struct transfer_geometry {    // cm units
	FLOAT64 Length;
	FLOAT64 Area; // for 3d diffusion
     FLOAT64 Width; // for 2d diffusion
	enum DIRECTION Axis;
};



struct selected_event_info {
     UINT16 ProcessAreaNo;
     UINT32 EventNo;
     enum PROCESS_AREA_TYPE Location;
};


struct rate_coefficient_info {   // moles_liter_sec units
	FLOAT64				Coefficient[3];
	FLOAT64				UnitsConversionExponent;
	enum RATE_CONST_FORMAT	Format;
};

struct equil_detect_info {
	selected_event_info EventInfo;
	UINT32		    PartnerStepNo;
	UINT32              NoTimesSelected;
};

struct update_step_info {
     update_step_info ( process_area* l, UINT32 s ) { pLocation = l; StepNo = s; }
     process_area*    	pLocation;   // pointer to compartment or transfer path
     UINT32    	 	StepNo;      // process step number
};

struct profile_status {
	FLOAT64 NextProfilePoint;
	FLOAT64	Slope;
	FLOAT64	IdlingValueStep;
	FLOAT64	MaxTimeStep;
	FLOAT64	CurrentValueInProfile;
	FLOAT64	CurrentTimeInProfile;
};

#endif

