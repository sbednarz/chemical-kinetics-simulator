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

#include <string.h>
#include <sv.hxx>

#include "globconc.hrc"
#include "help.hhc"
#include "junk.hxx"


ModalDialogvID_DLG_GLOBAL_CONC_CHANGE::ModalDialogvID_DLG_GLOBAL_CONC_CHANGE( Window * pParent, const ResId & rResId, BOOL bFreeRes )
    : ModalDialog( pParent, rResId ),
    aListBoxvID_LB_SPECIES_LIST( this, ResId( vID_LB_SPECIES_LIST ) ),
    aRadioButtonvID_RB_USE_FILE( this, ResId( vID_RB_USE_FILE ) ),
    aRadioButtonvID_RB_USE_SINGLE_VALUE( this, ResId( vID_RB_USE_SINGLE_VALUE ) ),
    aGroupBox2( this, ResId( 2 ) ),
    aPushButtonvID_BTN_SELECT_FILE( this, ResId( vID_BTN_SELECT_FILE ) ),
    aEditvID_EF_CONC_VALUE( this, ResId( vID_EF_CONC_VALUE ) ),
    aOKButtonvID_BTN_OK( this, ResId( vID_BTN_OK ) ),
    aHelpButtonvID_BTN_HELP( this, ResId( vID_BTN_HELP ) ),
    aFixedTextvID_STEXT_CONC_UNITS( this, ResId( vID_STEXT_CONC_UNITS ) ),
    aCancelButtonvID_BTN_CANCEL( this, ResId( vID_BTN_CANCEL ) ),
    aGroupBox1( this, ResId( 1 ) )
{
    if( bFreeRes ) FreeResource();
}

