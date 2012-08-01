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
//  COMTDIMR.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChangeCompartmentDimensionDialog class
//  the VSIM user interface. 
//
//  CREATED : 9.09.98
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "comptdim.hxx"
#include "mainapp.hxx"


#endif


#include "comptdim.hrc"




// ----------------------------------------------------------------------------
//                class CompartmentDimensionsDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class CompartmentDimensionsDialog : public ModalDialog
{
protected:
     GroupBox            group_box1;
     OKButton            ok_btn;
     PushButton          undo_btn;
     HelpButton          help_btn;

     Edit                x_dim_edit;
     Edit                y_dim_edit;
     Edit                z_dim_edit;

     FixedText           x_label_text;
     FixedText           y_label_text;
     FixedText           z_label_text;

     FixedText           x_dimen_text;
     FixedText           y_dimen_text;
     FixedText           z_dimen_text;

     ReactionScheme*     p_scheme;
     BOOL8               data_altered;

     void                InitializeDialog( );
     void                SetUnitsText();

     BOOL8               ValidateAndStoreData( );

     void                SendErrorMessage( const String& FieldName, const String& UserEntry ) const;

public:

     CompartmentDimensionsDialog( ReactionScheme* pScheme, Window* pParent );

     void SetModifiedFlagHandler( Edit* );

     void CloseHandler( OKButton* );
     void UndoHandler( PushButton* );
};



CompartmentDimensionsDialog::CompartmentDimensionsDialog( ReactionScheme* pScheme, Window* pParent ) :
ModalDialog( pParent, ResId( vID_DLG_COMPT_DIM_EDIT ) ),
group_box1( this, ResId( vID_GB_1 ) ), 
ok_btn( this, ResId( vID_BTN_CLOSE ) ),                     
undo_btn( this, ResId( vID_BTN_UNDO ) ),                     
help_btn( this, ResId( vID_BTN_HELP ) ),                     
x_dim_edit( this, ResId( vID_X_DIM_EF  ) ),                     
y_dim_edit( this, ResId( vID_Y_DIM_EF ) ),                     
z_dim_edit( this, ResId( vID_Z_DIM_EF ) ),                     
x_label_text( this, ResId( 6 ) ),                     
y_label_text( this, ResId( 7 ) ),                     
z_label_text( this, ResId( 8 ) ),                     
x_dimen_text( this, ResId( vID_STEXT_X_DIMENSION  ) ),                     
y_dimen_text( this, ResId( vID_STEXT_Y_DIMENSION ) ),                     
z_dimen_text( this, ResId( vID_STEXT_Z_DIMENSION ) ),                     
p_scheme( pScheme ),
data_altered( FALSE )
{
     FreeResource( );

     InitializeDialog();

     ok_btn.ChangeClickHdl(
          LINK( this, CompartmentDimensionsDialog, CloseHandler ) );
     undo_btn.ChangeClickHdl(
          LINK( this, CompartmentDimensionsDialog, UndoHandler ) );

     x_dim_edit.ChangeModifyHdl( LINK( this, CompartmentDimensionsDialog, SetModifiedFlagHandler ) ); 
     y_dim_edit.ChangeModifyHdl( LINK( this, CompartmentDimensionsDialog, SetModifiedFlagHandler ) ); 
     z_dim_edit.ChangeModifyHdl( LINK( this, CompartmentDimensionsDialog, SetModifiedFlagHandler ) ); 

     String units = p_scheme->GetDistanceUnits();

     x_dimen_text.SetText( units );
     y_dimen_text.SetText( units );
     z_dimen_text.SetText( units );

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}





// ----------------------------------------------------------------------------
//            CompartmentDimensionsDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDimensionsDialog::InitializeDialog( )
{
      FLOAT64 x;
      FLOAT64 y;
      FLOAT64 z;

      p_scheme->GetCompartmentDimensions( x, y, z );

      x_dim_edit.SetText( NumericString( x ) );
      y_dim_edit.SetText( NumericString( y ) );
      z_dim_edit.SetText( NumericString( z ) );
}



// ----------------------------------------------------------------------------
//            CompartmentDimensionsDialog::UndoHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDimensionsDialog::UndoHandler( PushButton* )
{
     InitializeDialog();
     data_altered = FALSE;
}




// ----------------------------------------------------------------------------
//            CompartmentDimensionsDialog::SetModifiedFlagHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDimensionsDialog::SetModifiedFlagHandler( Edit* )
{
     data_altered = TRUE;
     return;
}






// ----------------------------------------------------------------------------
//               CompartmentDimensionsDialog::ValidateAndStoreData( )
// ............................................................................
// 
//   this fcn retrieves the data from the entry fields, checks for its validity,
//   and stores it back in the parent Object if valid. If
//   the data has a problem then a diagnostic message is displayed. The function 
//   returns TRUE if data is valid, FALSE if not 
//
// ----------------------------------------------------------------------------

BOOL8 CompartmentDimensionsDialog::ValidateAndStoreData( )
{
     // retrieve and test the data in all the numeric entry fields 
     
     NumericString teststr_x( x_dim_edit.GetText( ) );
     NumericString teststr_y( y_dim_edit.GetText( ) );
     NumericString teststr_z( z_dim_edit.GetText( ) );

     if ( !teststr_x.IsValidNonZeroFLOAT64() )
     {
          SendErrorMessage( x_label_text.GetText(), x_dim_edit.GetText() );

          return FALSE;
     }


     if ( !teststr_y.IsValidNonZeroFLOAT64() )
     {
          SendErrorMessage( y_label_text.GetText(), y_dim_edit.GetText() );

          return FALSE;
     }


     teststr_z = z_dim_edit.GetText();

     if ( !teststr_z.IsValidNonZeroFLOAT64() )
     {
          SendErrorMessage( z_label_text.GetText(), z_dim_edit.GetText() );

          return FALSE;
     }

     // all data is valid if we make it to here so store it 

     FLOAT64 x = (FLOAT64) teststr_x;
     FLOAT64 y = (FLOAT64) teststr_y;
     FLOAT64 z = (FLOAT64) teststr_z;

     p_scheme->SetCompartmentDimensions( x, y, z );

     return TRUE;
}



// ----------------------------------------------------------------------------
//            CompartmentDimensionsDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDimensionsDialog::CloseHandler( OKButton* )
{
     if ( data_altered )
     {
          if ( ! ValidateAndStoreData( ) )
               return;

          p_scheme->SetStatus( SIM_DATA_MODIFIED );
          EndDialog( CCD_RC_ACCEPT );
     }
     else
          EndDialog( CCD_RC_CANCEL );
}



// ----------------------------------------------------------------------------
//            CompartmentDimensionsDialog::SendErrorMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void CompartmentDimensionsDialog::SendErrorMessage( const String& FieldName, const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_CCD_INPUT_ERR1 ) ) + UserEntry + 
      String( ResId( vID_STR_CCD_INPUT_ERR2 ) ) + FieldName +
      String( ResId( vID_STR_CCD_INPUT_ERR3 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}






// ----------------------------------------------------------------------------
//            ChangeCompartmentDimensionDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum CCD_RC ChangeCompartmentDimensionDialog( ReactionScheme* pScheme, Window* pParent )
{
     enum CCD_RC rc;

     CompartmentDimensionsDialog* pdlg = new CompartmentDimensionsDialog( pScheme, pParent);

     if ( pdlg )
     {
          rc = (CCD_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return CCD_RC_FAIL;
     }
}


