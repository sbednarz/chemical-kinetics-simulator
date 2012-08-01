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
// 	Definition of PROPS_LIBRARY
//	This object contains materials information for all chemical species
//		in the simulation system
//	Date last modified: February 28, 1996
//	Written by Frances A. Houle
//	IBM  
//
//--------------------------------------------------------------------------

#ifndef PROP_LIB_HXX
#define PROP_LIB_HXX

#include "datastru.hxx"
#include "txtstrem.hxx"
#include "assert.hxx"
#include "vsimglob.hxx"


class props_library {

protected:
	properties 	*MaterialsProps; // [NumberOfReactants]

public:
	props_library() { MaterialsProps = 0; };
	props_library( const props_library& p_l );
	~props_library() { delete [] ( MaterialsProps ); }

	properties& 	GetProperties (UINT16 SpeciesNo) {return MaterialsProps[SpeciesNo];}

	friend TextInputStream& operator >> ( TextInputStream& rTIS, props_library& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, props_library& rObject );


#endif

