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
//      Declaration of class INTERFACE_TRANSFER_PATH
//      Derived from transfer_path, base class is process_area
//      This object manages events that transfer chemical species between a 
//              volume element and an interface compartment. It does not 
//              contain any chemical species
//      Date last modified: December 17, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef INTXPATH_HXX
#define INTXPATH_HXX

#include "xferpath.hxx"
#include "massintx.hxx"
#include "txtstrem.hxx"


class interface_transfer_path : public transfer_path {       //derived

protected:


public:
	interface_transfer_path ();
	interface_transfer_path ( const interface_transfer_path& i_t_p );
	~interface_transfer_path () {}

	void   	CalcPressure() { return; }

	void   	  UpdateTransferredSpecies (selected_event_info& Event);
	void	  UpdateAttachedVolumes();
	void   	  UpdateAttachedCompartmentPressure();
	void   	  UpdatePathGeometry() { return; }
	void   	  UpdateGeometry();
	void   	  UpdateKs();
	void	  UpdateTransferRateConstants( reaction_compartment* pCompartment, compartment_geometry& CmptGeometry );
	void   	  UpdateProbabilities(selected_event_info& Event);

	void      SetConstPFlag() { return; }


	friend    TextInputStream& operator >> ( TextInputStream& rTIS, interface_transfer_path& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, interface_transfer_path& rObject );

#endif


