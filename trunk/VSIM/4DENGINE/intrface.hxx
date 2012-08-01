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
//	Declaration of class INTERFACE
//		derived from derived class REACTION_COMPARTMENT, base class
//   		PROCESS_AREA. This object is connected by interfacial transfer path
//		objects to other interfacial and volume element reaction compartments. It has
//		a geometry dependent on the compartments it separates
//        and is used with both diffusion and adsorption/desorption kinetics.It requires the
//		three_d_system simulation system
//	Date last modified: September 30, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef INTRFACE_HXX
#define INTRFACE_HXX

#include "cmprtmnt.hxx"
#include "rxn_step.hxx"
#include "txtstrem.hxx"
#include "idtypes.hxx"
#include "assert.hxx"


class interface : public reaction_compartment {

protected:

public:
     interface ();
     interface ( const interface& i_c );
     ~interface () {}

	void	     UpdateGeometry();
	void      UpdateVolume() { return; }
     void      UpdatePressure() { return; }
     void      UpdateKs();
	void  	UpdateProbabilities(selected_event_info& Event);

	void      InitGeometry();
     void      InitTPV();
     void      Connect();

	friend    TextInputStream&    operator >> ( TextInputStream& rTIS, interface& rObject );

};

TextInputStream&    operator >> ( TextInputStream& rTIS, interface& rObject );


#endif