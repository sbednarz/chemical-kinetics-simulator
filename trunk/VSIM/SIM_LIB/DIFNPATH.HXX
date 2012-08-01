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
//      Declaration of class MASS_DIFFUSION_PATH
//      Derived from transfer_path, base class is process_area
//      This object manages events that diffuse chemical species between two
//              linked compartments. It does not contain any chemical species
//		it is set up to handle either explicit diffusio, or virtual
//		diffusion
//      Date last modified: August 5, 1998
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef DIFNPATH_HXX
#define DIFNPATH_HXX

#include "xferpath.hxx"
#include "massdifn.hxx"
#include "virtdifn.hxx"
#include "txtstrem.hxx"


class mass_diffusion_path : public transfer_path {       //derived

public:
	mass_diffusion_path ();
	mass_diffusion_path ( const mass_diffusion_path& m_d_p );
	~mass_diffusion_path () {}

	void	  UpdatePathGeometry();
	void	  UpdateAttachedVolumes();
	void	  UpdateTransferRateConstants( reaction_compartment* pCompartment,
				compartment_geometry& CmptGeometry );
	void	  InitPathGeometry();

	friend    TextInputStream& operator >> ( TextInputStream& rTIS, mass_diffusion_path& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, mass_diffusion_path& rObject );

#endif


