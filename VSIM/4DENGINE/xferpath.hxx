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
//      Declaration of class TRANSFER_PATH
//      Derived from process_area
//      This object is a virtual generic transfer system that manages events
//		that transfer chemical species between two
//              linked compartments. It does not contain any chemical species.
//      Date last modified: December 17, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef XFERPATH_HXX
#define XFERPATH_HXX

#include "procarea.hxx"
#include "cmprtmnt.hxx"
#include "vsimtype.hxx"
#include "r_x_udl.hxx"
#include "x_x_udl.hxx"
#include "datastru.hxx"
#include "assert.h"

class rxn_after_xfr_update_list;
class xfr_after_xfr_update_list;


class transfer_path : public process_area {       //derived

protected:
	rxn_after_xfr_update_list*      UpdateReactionAfterTransfer; // [NumberOfSteps]
	xfr_after_xfr_update_list*      UpdateTransferAfterTransfer; // [NumberOfSteps]
	reaction_compartment*           pSourceCompartment;  // left side of transfer step
	reaction_compartment*           pTargetCompartment;  // right side of transfer step
	UINT16			            SourceID;
	UINT16			            TargetID;
	BOOL8                           ConstT;    // flag for target and source compartments having const T option
	BOOL8                           ConstP;    // flag for target and source compartments having const P option
	BOOL8                           ConstV;    // flag for target and source compartments having const V option
     FLOAT64                         AvgPressure;
	transfer_geometry		       PathGeometry;

public:
	transfer_path ();
	transfer_path ( const transfer_path& t_p );
	~transfer_path ();

	reaction_compartment*    GetSourceCompartmentPtr () { return pSourceCompartment; }
	reaction_compartment*    GetTargetCompartmentPtr () { return pTargetCompartment; }
	UINT16			 GetSourceID () { return SourceID; }
	UINT16		 	 GetTargetID () { return TargetID; }

	BOOL8     	IsConstT () { return ConstT; }
	BOOL8     	IsConstV () { return ConstV; }
	BOOL8     	IsConstP () { return ConstP; }
	void      	CalcTemperature();
	virtual void	CalcPressure() {}
	FLOAT64   	GetPressure() { return AvgPressure; }

	FLOAT64		GetTransferLength() { return PathGeometry.Length; }
	FLOAT64		GetTransferArea() { return PathGeometry.Area; } // 3d diffusion
	FLOAT64        	GetTransferWidth() { return PathGeometry.Width; } // 2d diffusion
	enum DIRECTION	GetTransferAxis() { return PathGeometry.Axis; }

	virtual void     UpdateTransferredSpecies (selected_event_info& Event) {}
	virtual void	  UpdatePathGeometry() {}
	virtual void	  UpdateAttachedVolumes() {}
	virtual void     UpdateAttachedCompartmentPressure() {}
	virtual void     UpdateGeometry() {}
	virtual void     UpdateKs() {}
	virtual void	  UpdateTransferRateConstants( reaction_compartment* pCompartment,
                              compartment_geometry& CmptGeometry ) {}

	// the following are for initialization of the object
	void 	  	SetSourceCompartmentPtr( reaction_compartment* p );
	void 	  	SetTargetCompartmentPtr( reaction_compartment* p );
	void      	SetConstTFlag ();
	void      	SetConstVFlag ();
	virtual void   	SetConstPFlag () {}
	void      	InitUpdateTables ();
	virtual void 	InitPathGeometry () {}

};


#endif


