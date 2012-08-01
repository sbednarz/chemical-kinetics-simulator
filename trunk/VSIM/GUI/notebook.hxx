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

// ----------------------------------------------------------------------------
//  EDITNOTE.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//
//  CREATED : 5.17.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__EDITNOTE_HXX__)
#define __EDITNOTE_HXX__

class SchemeFrameWindow;

class NotebookDialog : public ModelessDialog
{
protected:
     MultiLineEdit text_mle;
     OKButton close_btn;
     PushButton undo_btn;
     PushButton clear_btn;
     HelpButton help_btn;

     SchemeFrameWindow*   p_parent;
     BOOL8           data_altered;

     void SetDataAltered( BOOL8 NewVal ) { data_altered = NewVal; }
     BOOL8 IsDataAltered(  ) const { return data_altered; }

     SchemeFrameWindow*   GetpParent() { return p_parent; }

public:
     NotebookDialog( SchemeFrameWindow * pParent );

     void CloseBtnHandler( OKButton* );
     void ClearBtnHandler( PushButton* );
     void UndoBtnHandler( PushButton* );
     void TextModifiedHandler( Edit* );

     void CloseDialog(  );

};

NotebookDialog* EditNotebook( SchemeFrameWindow* pParent );


#endif

