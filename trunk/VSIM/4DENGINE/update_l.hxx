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
//      Declaration of class UPDATE_LIST
//      Base class
//      This object contains a pointer to an array of pointers to
//		LISTs of update_step_info data structures (update_list_item). LIST is a
//		StarView class. Functions for building the array of lists
//		and retrieving member data are included.
//      Date last modified: January 10, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef UPDATE_L_HXX
#define UPDATE_L_HXX

#include "vsimtype.hxx"
#include "udl_item.hxx"



class update_list {   //  base class

protected:
	update_list_item    StepList;
	UINT32              Length;

public:
	update_list (): StepList(), Length(0) {}
	update_list ( const update_list& u_l );
	virtual ~update_list () {}

	void           		SetLength ( UINT32 l ) { Length = l; }
	UINT32         		GetLength () { return Length; }
	update_step_info* 	GetData ( UINT16 n ) { return StepList.GetObject( n ); }

};

#endif
