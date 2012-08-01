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
//  EDITAXIS.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This modules defines the EditAxis dialog used in the plot routines
//  the VSIM user interface.
//
//  CREATED : 8.21.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "editaxis.hxx"
#include "mainapp.hxx"

#endif


#include "editaxis.hrc"
#include "strings.hrc"


class EditAxisDialog : public ModalDialog
{
protected:
     FixedText    fixed_text2;
     FixedText    fixed_text3;
     ListBox      num_dec_places_lb;
     FixedText    fixed_text1;
     FixedText    fixed_text4;
     Edit         min_value_ef;
     Edit         max_value_ef;
     Edit         tic_spacing_ef;
     FixedText    fixed_text5;
     Edit         title_ef;
     CheckBox     show_units_cb;
     CheckBox     show_grid_cb;
     OKButton     ok_btn;
     PushButton   undo_btn;
     HelpButton   help_btn;
     GroupBox     group_box_1;
     GroupBox     group_box_2;

     XyAxis*      p_axis;
     BOOL8        modified;
     UINT16       total_num_digits;
     UINT16       precision;
     Limits       plot_limits;
     FLOAT64      tic_interval;

     void         InitializeDialog();
     BOOL8        NumericEntriesValid( );
     void         SetNumericEntryFields();
     void         SetModifiedFlag();

public:
                   EditAxisDialog( XyAxis* pAxis );

     void          UndoHandler( PushButton* );
     void          CloseHandler( OKButton* );
     void          SetModifiedFlagHandler( Edit* );
     void          CheckBoxHandler( CheckBox* );
     void          ListboxHandler( ListBox* );

};



EditAxisDialog::EditAxisDialog( XyAxis* pAxis ) :
ModalDialog( (Window*) pAxis->GetpParent(), ResId( vID_DLG_EDIT_AXIS)  ),
fixed_text2( this, ResId( 2 ) ),
fixed_text3( this, ResId( 3 ) ),
num_dec_places_lb( this, ResId( vID_LB_NUM_DEC_PLACES ) ),
fixed_text1( this, ResId( 1 ) ),
fixed_text4( this, ResId( 4 ) ),
min_value_ef( this, ResId( vID_EF_MIN_VALUE ) ),
max_value_ef( this, ResId( vID_EF_MAX_VALUE ) ),
tic_spacing_ef( this, ResId( vID_EF_TIC_SPACING ) ),
fixed_text5( this, ResId( 5 ) ),
title_ef( this, ResId( vID_EF_TITLE ) ),
show_units_cb( this, ResId( vID_CB_SHOW_UNITS ) ),
show_grid_cb( this, ResId( vID_CB_SHOW_GRID ) ),
group_box_1( this, ResId( 1 ) ),
group_box_2( this, ResId( 2 ) ),
ok_btn( this, ResId( vID_BTN_OK ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
p_axis( pAxis ),
modified( FALSE ),
total_num_digits( 0 ),
precision( 0 ),
plot_limits(),
tic_interval( 0.0 )
{
     FreeResource();

     InitializeDialog();

     title_ef.ChangeModifyHdl( LINK( this, EditAxisDialog, SetModifiedFlagHandler ) );
     min_value_ef.ChangeModifyHdl( LINK( this, EditAxisDialog, SetModifiedFlagHandler ) );
     max_value_ef.ChangeModifyHdl( LINK( this, EditAxisDialog, SetModifiedFlagHandler ) );
     tic_spacing_ef.ChangeModifyHdl( LINK( this, EditAxisDialog, SetModifiedFlagHandler ) );

     show_units_cb.ChangeClickHdl( LINK( this, EditAxisDialog, CheckBoxHandler ) );
     show_grid_cb.ChangeClickHdl( LINK( this, EditAxisDialog, CheckBoxHandler ) );

     ok_btn.ChangeClickHdl( LINK( this, EditAxisDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, EditAxisDialog, UndoHandler ) );

     num_dec_places_lb.ChangeSelectHdl( LINK( this, EditAxisDialog, ListboxHandler ) );

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}



// ----------------------------------------------------------------------------
//                 EditAxisDialog::CheckBoxHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void  EditAxisDialog::CheckBoxHandler( CheckBox* )
{
     if ( ! modified )
          SetModifiedFlag();
}





// ----------------------------------------------------------------------------
//                 EditAxisDialog::SetModifiedFlag()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::SetModifiedFlag()
{
     modified = TRUE;
     undo_btn.Enable();
}




// ----------------------------------------------------------------------------
//                 EditAxisDialog::ListboxHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::ListboxHandler( ListBox* )
{
     if ( !NumericEntriesValid() )
     {
          num_dec_places_lb.SelectEntry( String( precision ) );
          return;
     }

     if ( min_value_ef.IsModified() )
          plot_limits.Min() = NumericString( min_value_ef.GetText() );


     if ( max_value_ef.IsModified() )
          plot_limits.Max() = NumericString( max_value_ef.GetText() );


     if ( tic_spacing_ef.IsModified() )
          tic_interval = NumericString( tic_spacing_ef.GetText() );

     UINT16 new_precision = num_dec_places_lb.GetSelectEntry();

     total_num_digits =  total_num_digits - precision + new_precision;

     precision = new_precision;

     SetNumericEntryFields();

     if ( ! modified )
          SetModifiedFlag();
}




// ----------------------------------------------------------------------------
//                 EditAxisDialog::SetNumericEntryFields()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::SetNumericEntryFields()
{
     min_value_ef.SetText( NumericString( plot_limits.Min(), total_num_digits, precision) );
     max_value_ef.SetText( NumericString( plot_limits.Max(), total_num_digits, precision) );
     tic_spacing_ef.SetText( NumericString( tic_interval, total_num_digits, precision) );
}



// ----------------------------------------------------------------------------
//                 EditAxisDialog::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::InitializeDialog()
{
     total_num_digits =  p_axis->GetTicLabelTotalNumDigits();
     precision  =  p_axis->GetTicLabelPrecision();
     plot_limits = p_axis->GetAxisLimits();
     tic_interval = p_axis->GetTicInterval();

     SetNumericEntryFields();

     num_dec_places_lb.SelectEntry( String( precision ) );

     title_ef.SetText( p_axis->GetTitle() );

     show_units_cb.Check( p_axis->ShowUnits() );
     show_grid_cb.Check( p_axis->ShowGrid() );
}




// ----------------------------------------------------------------------------
//                 EditAxisDialog::SetModifiedFlagHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::SetModifiedFlagHandler( Edit* )
{
     if ( ! modified )
          SetModifiedFlag();
}




// ----------------------------------------------------------------------------
//                 EditAxisDialog::NumericEntriesValid()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 EditAxisDialog::NumericEntriesValid( )
{
     String message;

     if ( !NumericString( min_value_ef.GetText() ).IsValidNonNegativeFLOAT64() )
     {
          message = String( ResId( vID_STR_EDIT_AXIS_ERR1 ) ) +  min_value_ef.GetText() +
                    String( ResId( vID_STR_EDIT_AXIS_ERR2 ) );

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }

     if ( !NumericString( max_value_ef.GetText() ).IsValidNonNegativeFLOAT64() )
     {
          message = String( ResId( vID_STR_EDIT_AXIS_ERR1 ) ) +  max_value_ef.GetText() +
                    String( ResId( vID_STR_EDIT_AXIS_ERR3 ) );

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }

     if ( !NumericString( tic_spacing_ef.GetText() ).IsValidNonNegativeFLOAT64() )
     {
          message = String( ResId( vID_STR_EDIT_AXIS_ERR1 ) ) +  tic_spacing_ef.GetText() +
                    String( ResId( vID_STR_EDIT_AXIS_ERR4 ) );

          WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );

          return FALSE;
     }

     return TRUE;
}



// ----------------------------------------------------------------------------
//                 EditAxisDialog::CloseHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::CloseHandler( OKButton* )
{
     if ( modified )
     {
          // read the user entries

          // first check for valid input of numeric entries

          if ( !NumericEntriesValid() )
               return;

          // no syntax errors - now check for other numeric constraints on the
          // values

          plot_limits.Min() = NumericString( min_value_ef.GetText() );
          plot_limits.Max() = NumericString( max_value_ef.GetText() );
          tic_interval      = NumericString( tic_spacing_ef.GetText() );
          precision         = num_dec_places_lb.GetSelectEntry();

          switch (  p_axis->SetAxisParameters( plot_limits, tic_interval, precision ) )
          {
          case AXIS_RC_VALID_DATA :

               // the data is OK - get the rest of the input and end dialog

               p_axis->SetTitle( title_ef.GetText() );
               p_axis->ShowUnits() = show_units_cb.IsChecked( );
               p_axis->ShowGrid()  = show_grid_cb.IsChecked( );
               EndDialog( TRUE );

               break;

          case AXIS_RC_MINMAX_ERROR :

               WarningBox( this, WB_OK | WB_DEF_OK, String( ResId( vID_STR_EDIT_AXIS_ERR5 ))).Execute( );

               break;

          case AXIS_RC_TIC_RANGE_ERROR :

               WarningBox( this, WB_OK | WB_DEF_OK, String( ResId( vID_STR_EDIT_AXIS_ERR6 ))).Execute( );

               break;

          case AXIS_RC_TOO_MANY_TICS :

               WarningBox( this, WB_OK | WB_DEF_OK, String( ResId( vID_STR_EDIT_AXIS_ERR7 ))).Execute( );

               break;

          default :

               ASSERT ( 0 );
          }
     }
     else
          EndDialog( FALSE );
}






// ----------------------------------------------------------------------------
//                 EditAxisDialog::UndoHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void EditAxisDialog::UndoHandler( PushButton* )
{
     modified = FALSE;
     InitializeDialog();
     undo_btn.Disable();
}





// ----------------------------------------------------------------------------
//                 EditAxis()
// ............................................................................
//
//
// ----------------------------------------------------------------------------


BOOL8 EditAxis( XyAxis* pAxisObject )
{

     EditAxisDialog* pdlg = new EditAxisDialog( pAxisObject );

     if ( pdlg )
     {
          BOOL8 rc = pdlg->Execute();
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}




