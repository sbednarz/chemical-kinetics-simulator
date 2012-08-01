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
//	Declaration of class VOLUME_ELEMENT
//		derived from derived class REACTION_COMPARTMENT, base class
//   		PROCESS_AREA. This object is connected by transfer path
//		objects to other volume elements reaction compartments. It has
//		a specified geometry and is used with diffusion.It requires the
//		three_d_system simulation system
//	Date last modified: May 6, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef VOLELMNT_HXX
#define VOLELMNT_HXX

#include "cmprtmnt.hxx"
#include "rxn_step.hxx"
#include "txtstrem.hxx"
#include "idtypes.hxx"
#include "assert.hxx"


class volume_element : public reaction_compartment {

protected:

public:
     volume_element ();
     volume_element ( const volume_element& v_e );
     ~volume_element () {}

	void	  UpdateGeometry();
	void      UpdateVolume();
	void      InitGeometry();

	friend    TextInputStream&    operator >> ( TextInputStream& rTIS, volume_element& rObject );

};

TextInputStream&    operator >> ( TextInputStream& rTIS, volume_element& rObject );


#endif