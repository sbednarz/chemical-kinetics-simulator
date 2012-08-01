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
//      Declaration of class REACTIONUPDATETABLE
//      derived from base class UPDATE_TABLE
//      Date last modified: February 13, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef RXN_UD_T_HXX
#define RXN_UD_T_HXX

#include "update_t.hxx"
#include "cmprtmnt.hxx"
#include "datastru.hxx"

class reaction_compartment;


class reaction_update_table : public update_table {     //derived

// for reactions in a compartment only

protected:
     reaction_compartment*    pCompartment;

public:
	reaction_update_table (): update_table() {}
	reaction_update_table (UINT32 NumberOfReactions);
	reaction_update_table ( const reaction_update_table& r_u_t );
     virtual ~reaction_update_table () {}

     void                     SetCompartmentPtr( reaction_compartment* p ) { pCompartment = p; }
     reaction_compartment*    GetCompartmentPtr() { return pCompartment; }
	void                     BuildTable ( reaction_compartment* p );

};

#endif


