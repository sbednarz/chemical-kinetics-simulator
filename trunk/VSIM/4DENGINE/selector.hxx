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
//      Declaration of class SELECTOR
//      base class
//      Date last modified: October 30, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#ifndef SELECTOR_HXX
#define SELECTOR_HXX

#include "vsimtype.hxx"
#include "datastru.hxx"
#include "txtstrem.hxx"

class selector {

private:
     selected_event_info SelectedEvent;

public:
     selector ();
     selector ( const selector& s );
     ~ selector () {}
     void Select ();  //finds SelectedEvent
     void SetSelectedEventInfo ( UINT16 pan, UINT32 en, PROCESS_AREA_TYPE l );
     selected_event_info& GetSelectedEventInfo () { return SelectedEvent; }
     friend TextInputStream& operator >> ( TextInputStream& rTIS, selector& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, selector& rObject );

#endif

