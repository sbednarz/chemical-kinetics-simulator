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

//---------------------------------------------------------------------------
//
//      Declaration of AMORPHOUS_SYSTEM
//      This base class contains a core set of objects and methods
//		necessary to run a simulation of a chemical reaction in
//		multiple compartments with linking transfer paths. Derived classes
//		are intended to be the working classes
//      Date last modified: March 11, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef AMRPHSYS_HXX
#define AMRPHSYS_HXX

#include "simulatr.hxx"
#include "containr.hxx"
#include "masspath.hxx"


class amorphous_system : public simulation_system {       //call this "Simulator"

protected:

public:
     amorphous_system () {}
     amorphous_system ( const amorphous_system& a_s );
     virtual ~amorphous_system();


     friend    TextInputStream&    operator >> ( TextInputStream& rTIS, amorphous_system& rObject );

};

TextInputStream&    operator >> ( TextInputStream& rTIS, amorphous_system& rObject );


#endif



