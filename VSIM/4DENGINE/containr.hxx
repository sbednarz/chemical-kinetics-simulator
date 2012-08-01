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
//	Declaration of class SHAPELESS_CONTAINER
//	derived from derived class REACTION_COMPARTMENT
//	This object is a complete reaction system which can be connected
//		by transfer path objects to other reaction compartments. It has
//        	no shape and is to be used with mass transfer only
//		It is to be used with the amorphous_system derived simulator
//		class
//	Date last modified: September 30, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef CONTAINR_HXX
#define CONTAINR_HXX

#include "cmprtmnt.hxx"
#include "rxn_step.hxx"
#include "txtstrem.hxx"
#include "idtypes.hxx"
#include "assert.hxx"

class shapeless_container : public reaction_compartment {

public:
     shapeless_container ();
     shapeless_container ( const shapeless_container& s_c );
     ~shapeless_container (){}

     void 	UpdateVolume();
     void      UpdatePressure();
     void      UpdateKs();
     void      UpdateProbabilities( selected_event_info& Event );
     void      InitTPV();
     void      Connect();

     friend    TextInputStream&    operator >> ( TextInputStream& rTIS, shapeless_container& rObject );


};

TextInputStream&    operator >> ( TextInputStream& rTIS, shapeless_container& rObject );


#endif