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

class ModalDialogUSER_PREFS_DLG : public ModalDialog
{
protected:
    Edit aEditvID_RXN_FILE_DIR;
    Edit aEditvID_TEXT_FILE_DIR;
    Edit aEditvID_ANY_FILE_DIR;
    RadioButton aRadioButtonvID_RB_USE_CLIP_DIMS;
    RadioButton aRadioButtonvID_RB_USE_TARGET_DIMS;
    RadioButton aRadioButtonvID_RB_ALWAYS_ASK;
    CheckBox aCheckBoxvID_CB_SAVE_WINDOW_DATA;
    CheckBox aCheckBoxvID_CB_CONFIRM_FILE_OVERWRITE;
    OKButton aOKButtonvID_BTN_CLOSE;
    PushButton aPushButtonvID_BTN_UNDO;
    PushButton aPushButtonvID_BTN_DEFAULTS;
    HelpButton aHelpButtonvID_BTN_HELP;
    FixedText aFixedText1;
    FixedText aFixedText2;
    FixedText aFixedText3;
    GroupBox aGroupBox1;
    GroupBox aGroupBox2;
    GroupBox aGroupBox3;
public:
    ModalDialogUSER_PREFS_DLG( Window * pParent, const ResId & rResId, BOOL bFreeRes = TRUE );
};

