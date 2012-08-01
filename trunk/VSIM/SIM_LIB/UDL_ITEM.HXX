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
//      Declaration of class UPDATE_LIST_ITEM
//      This object defines an instance of a
//		LIST of update_step_info data structures. LIST is a
//		StarView class.
//      Date last modified: December 7 , 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef UDL_ITEM_HXX
#define UDL_ITEM_HXX

#include "datastru.hxx"
#include <tools.hxx>


// instantiate list variable update_step_list for update tables - StarView class. The data
// structure update_step_info is defined in datastru.hxx

DECLARE_LIST ( update_step_list, update_step_info* );



class update_list_item  : public  update_step_list {

public:
     update_list_item () : update_step_list (1,1) {}

};

#endif
