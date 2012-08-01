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

#include "userpref.hrc"
#include "help.hhc"
#include "temp.hxx"


ModalDialogUSER_PREFS_DLG::ModalDialogUSER_PREFS_DLG( Window * pParent, const ResId & rResId, BOOL bFreeRes )
    : ModalDialog( pParent, rResId ),
    aEditvID_RXN_FILE_DIR( this, ResId( vID_RXN_FILE_DIR ) ),
    aEditvID_TEXT_FILE_DIR( this, ResId( vID_TEXT_FILE_DIR ) ),
    aEditvID_ANY_FILE_DIR( this, ResId( vID_ANY_FILE_DIR ) ),
    aRadioButtonvID_RB_USE_CLIP_DIMS( this, ResId( vID_RB_USE_CLIP_DIMS ) ),
    aRadioButtonvID_RB_USE_TARGET_DIMS( this, ResId( vID_RB_USE_TARGET_DIMS ) ),
    aRadioButtonvID_RB_ALWAYS_ASK( this, ResId( vID_RB_ALWAYS_ASK ) ),
    aCheckBoxvID_CB_SAVE_WINDOW_DATA( this, ResId( vID_CB_SAVE_WINDOW_DATA ) ),
    aCheckBoxvID_CB_CONFIRM_FILE_OVERWRITE( this, ResId( vID_CB_CONFIRM_FILE_OVERWRITE ) ),
    aOKButtonvID_BTN_CLOSE( this, ResId( vID_BTN_CLOSE ) ),
    aPushButtonvID_BTN_UNDO( this, ResId( vID_BTN_UNDO ) ),
    aPushButtonvID_BTN_DEFAULTS( this, ResId( vID_BTN_DEFAULTS ) ),
    aHelpButtonvID_BTN_HELP( this, ResId( vID_BTN_HELP ) ),
    aFixedText1( this, ResId( 1 ) ),
    aFixedText2( this, ResId( 2 ) ),
    aFixedText3( this, ResId( 3 ) ),
    aGroupBox1( this, ResId( 1 ) ),
    aGroupBox2( this, ResId( 2 ) ),
    aGroupBox3( this, ResId( 3 ) )
{
    if( bFreeRes ) FreeResource();
}

