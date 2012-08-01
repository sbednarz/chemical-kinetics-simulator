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
//      Declaration of class INTERFACE_DIFFUSION_PATH
//      Derived from transfer_path, base class is process_area
//      This object manages events that diffuse chemical species between two
//              linked interface compartments. It does not contain any chemical species
//      Date last modified: December 17, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef INTDPATH_HXX
#define INTDPATH_HXX

#include "xferpath.hxx"
#include "massintd.hxx"
#include "txtstrem.hxx"


class interface_diffusion_path : public transfer_path {       //derived

public:
	interface_diffusion_path ();
	interface_diffusion_path ( const interface_diffusion_path& i_d_p );
	~interface_diffusion_path () {}

	void	CalcPressure() { return; }

	void	  UpdatePathGeometry();
	void   UpdateGeometry() { return; }
	void   UpdateAttachedVolumes() { return; }
	void   UpdateAttachedCompartmentPressure() { return; }
	void   UpdateKs() { return; }
	void   UpdateTransferredSpecies (selected_event_info& Event);
	void	  UpdateTransferRateConstants( reaction_compartment* pCompartment, 
                    compartment_geometry& CmptGeometry );
	void   UpdateProbabilities (selected_event_info& Event);

	void	  InitPathGeometry();
	void      SetConstPFlag() { return; }

	friend    TextInputStream& operator >> ( TextInputStream& rTIS, interface_diffusion_path& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, interface_diffusion_path& rObject );

#endif


