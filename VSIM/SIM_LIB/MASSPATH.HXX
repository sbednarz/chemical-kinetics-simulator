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
//      Declaration of class MASS_TRANSFER_PATH
//      Derived from transfer_path, base class is process_area
//      This object manages events that transfer chemical species between two
//              linked compartments. It does not contain any chemical species
//      Date last modified: March 14, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef MASSPATH_HXX
#define MASSPATH_HXX

#include "xferpath.hxx"
#include "massxfer.hxx"
#include "txtstrem.hxx"


class mass_transfer_path : public transfer_path {       //derived

protected:


public:
	mass_transfer_path ();
	mass_transfer_path ( const mass_transfer_path& m_t_p );
	~mass_transfer_path () {}

	void	  UpdateAttachedVolumes();
	void	  UpdateTransferRateConstants( reaction_compartment* pCompartment,
				compartment_geometry& CmptGeometry );
	void	  InitPathGeometry();

	friend    TextInputStream& operator >> ( TextInputStream& rTIS, mass_transfer_path& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, mass_transfer_path& rObject );

#endif


