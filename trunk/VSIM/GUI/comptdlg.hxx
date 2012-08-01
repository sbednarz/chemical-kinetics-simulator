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
//  COMPTDLG.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares the CompartmentObject Edit dialog used in the 
//  the VSIM user interface. 
//
//  CREATED : 1.23.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__COMPTDLG_HXX__)
#define  __COMPTDLG_HXX__


#include "comptobj.hxx"

class CompartmentDialog : public ModalDialog
{
private:
    FixedText     name_stext;
    Edit          name;
    FixedText     dimension_stext;
    FixedText     x_dimension;
    FixedText     y_dimension;
    FixedText     z_dimension;
    PushButton    rxn_condn_pb;
    PushButton    edit_conc_pb;
    PushButton    compt_color_pb;
    Window        color_sample;
    GroupBox      attr_gb;

    PushButton    add_step_pb;
    PushButton    edit_step_pb;
    PushButton    delete_step_pb;
    ListBox       rxn_step_lb;
    GroupBox      steps_groupbox;

    OKButton      close_button;
    HelpButton    help_button;

    CompartmentObject* const pcompartment;
    BOOL8         data_altered;
    BOOL8         species_data_needs_refresh;
    Color         color;

    void  InitializeListBox( );
    void  InitializeDimensions( );
    void  UpdateActivation( );

public:
                  CompartmentDialog( Window* pParent, 
                   CompartmentObject* pCompartment);

    CompartmentObject* const GetpCompartment() const { return pcompartment; }

    void          CloseHandler( OKButton * );
    void          AddRxnHandler( PushButton * );
    void          EditRxnHandler( PushButton * );
    void          EditSpeciesHandler( PushButton* );
    void          DeleteRxnHandler( PushButton * );
    void          EditRxnCondnsHandler( PushButton * );
    void          EditColorHandler( PushButton * );

    void          ListboxSelectHandler( ListBox* );
    void          ListboxDoubleClickHandler( ListBox* );

    void          SetModifiedFlagHandler( Edit* );

    void          ReplaceListboxEntry( UINT16 EntryNum,  const String rNewStr );

    void          RemoveListboxEntry( UINT16 EntryNum ) 
                  { 
                       rxn_step_lb.RemoveEntry( EntryNum );
                  }

    void          AddListboxEntry( UINT16 EntryNum,  const String rNewStr )
                  { 
                       rxn_step_lb.InsertEntry( rNewStr, EntryNum );
                  }

    void          SelectListboxEntry( UINT16 EntryNum ) 
                  { 
                       rxn_step_lb.SelectEntryPos( EntryNum ); 
                  }
};


ProcObjRC EditCompartment( Window* pParent, CompartmentObject* pCompartment);

#endif

