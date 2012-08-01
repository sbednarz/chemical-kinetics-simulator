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
//      Methods for class VOL_UPDATE
//      Date last modified: March 8, 1996
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------


#include "ud_vol.hxx"
#include "simulatr.hxx"



//-----------------------------------------------------------------------------
//       Definition of UpdateState method
//       Purpose: to manage updating process for compartment volumes
//       Parameters: none
//       Returns: nothing
//-----------------------------------------------------------------------------

void vol_update :: UpdateState ()

{            //begin
	selected_event_info event;

	event = Simulator->GetEventInfo();
	switch (event.Location) {

	case REACTION_COMPARTMENT:
		Simulator->UpdateVolumeAfterReaction (event.ProcessAreaNo);
		break;

	default: // MASS_TRANSFER
		Simulator->UpdateVolumeAfterTransfer (event.ProcessAreaNo);
		break;
	} //end switch

	return;

}           //end function

//-----------------------------------------------------------------------------


