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
//   Definition of methods for class SPECIES
//   Date last modified: September 12, 1995
//   Written by Frances A. Houle
//   IBM  
//
//--------------------------------------------------------------------------

#include "species.hxx"


//--------------------------------------------------------------------------
//        Definition of binary output stream operator <<
//        Purpose: to write out particle counts
//        Parameters: pointers to file and object
//        Returns: pointer
//--------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, species& rObject )

{
     rBOS << ( FLOAT64 ) rObject.no_particles;
     return rBOS;

}


//--------------------------------------------------------------------------
