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
//      Declaration of THREE_D_SYSTEM
//      This class is derived from SIMULATION_SYSTEM and contains additional
//		objects and methods necessary to run a simulation of a chemical
//		reaction in a spatially resolved, three dimensional system. All
//		transfer takes place by diffusion except for transfer involving
//        phase change across an interface
//      Date last modified: September 25, 1996
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef SYS_3D_HXX
#define SYS_3D_HXX

#include "simulatr.hxx"
#include "volelmnt.hxx"
#include "difnpath.hxx"
#include "intrface.hxx"
#include "intdpath.hxx"
#include "intxpath.hxx"


class three_d_system : public simulation_system {       //call this "Simulator"

protected:

public:
     three_d_system () {}
     three_d_system ( const three_d_system& t_d_s );
     virtual ~three_d_system();


     friend    TextInputStream&    operator >> ( TextInputStream& rTIS, three_d_system& rObject );

};

TextInputStream&    operator >> ( TextInputStream& rTIS, three_d_system& rObject );


#endif



