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
//  GLOBCONC.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the ChangeCompartmentDimensionDialog class
//  the VSIM user interface. 
//
//  CREATED : 9.14.98
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "globconc.hxx"
#include "mainapp.hxx"


#endif


#include "globconc.hrc"




// ----------------------------------------------------------------------------
//                class ChangeGlobalSpeciesConcDialog
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ChangeGlobalSpeciesConcDialog : public ModalDialog
{
protected:
     GroupBox            group_box1;
     GroupBox            group_box2;
     ListBox             species_list;
     RadioButton         use_file_rb;
     RadioButton         use_value_rb;
     PushButton          select_file_pb;
     Edit                conc_val_ef;
     FixedText           conc_units_text;
     OKButton            ok_btn;
     CancelButton        cancel_btn;
     HelpButton          help_btn;

     ReactionScheme*     p_scheme;
     String              file_rb_prefix;
     String              filename;

     void                InitializeDialog( );
     void                SetUnitsText();
     void                SendInvalidFilenameMessage( ) const;

     BOOL8               ValidateDataAndAttemptChange();

     void                SendErrorMessage( const String& UserEntry ) const;
     void                NotifyOfFileInputError( enum CHANGE_CONC_RC ErrorCode, const String& FileName );

public:

     ChangeGlobalSpeciesConcDialog( ReactionScheme* pScheme, Window* pParent );

     void CloseHandler( OKButton* );
     void CancelHandler( CancelButton* );
     void SetFilenameHandler( PushButton* ); 
};








// ----------------------------------------------------------------------------
//        ChangeGlobalSpeciesConcDialog::ChangeGlobalSpeciesConcDialog()
// ............................................................................
// 
//    constructor 
//
// ----------------------------------------------------------------------------

ChangeGlobalSpeciesConcDialog::ChangeGlobalSpeciesConcDialog( ReactionScheme* pScheme, Window* pParent ) :
ModalDialog( pParent, ResId( vID_DLG_GLOBAL_CONC_CHANGE ) ),
group_box1( this, ResId( 1 ) ), 
group_box2( this, ResId( 2 ) ), 
species_list( this, ResId( vID_LB_SPECIES_LIST ) ),  
use_file_rb( this, ResId( vID_RB_USE_FILE ) ),                      
use_value_rb( this, ResId( vID_RB_USE_SINGLE_VALUE ) ),                                  
select_file_pb( this, ResId( vID_BTN_SELECT_FILE ) ),                                  
conc_val_ef( this, ResId( vID_EF_CONC_VALUE ) ),                                  
conc_units_text( this, ResId( vID_STEXT_CONC_UNITS ) ),                     
ok_btn( this, ResId( vID_BTN_OK ) ),                     
cancel_btn( this, ResId( vID_BTN_CANCEL ) ),                                  
help_btn( this, ResId( vID_BTN_HELP ) ),                     
p_scheme( pScheme ),
file_rb_prefix( use_file_rb.GetText() ),
filename()
{
     FreeResource( );

     InitializeDialog();

     ok_btn.ChangeClickHdl( LINK( this, ChangeGlobalSpeciesConcDialog, CloseHandler ) );
     cancel_btn.ChangeClickHdl( LINK( this, ChangeGlobalSpeciesConcDialog, CancelHandler ) );
     select_file_pb.ChangeClickHdl( LINK( this, ChangeGlobalSpeciesConcDialog, SetFilenameHandler ) );

     String units = p_scheme->GetConcentrationUnits();

     conc_units_text.SetText( units );

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}







// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::InitializeDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::InitializeDialog( )
{

    // initialize species listbox with names 

    SpeciesDatabase* p_db = p_scheme->GetpSpeciesDatabase();

     species_list.ChangeUpdateMode( FALSE ); // turn off update to prevent flicker 
     species_list.Clear( );

     // initialize from Reaction Scheme SpeciesDatabase

     // first get the Species database so we can retrieve the species names 

     UINT16 num_species = p_db->GetNumberOfSpecies( );

     for ( UINT16 i = 0; i < num_species; i++ )
     {
          const SpeciesDatabaseEntry& r_entry = p_db->GetSpeciesDataByPosition( i );
          species_list.InsertEntry( r_entry.GetName( ) );
     }

     species_list.SelectEntryPos( 0 );

     species_list.ChangeUpdateMode( TRUE );
     species_list.Update( );

     // read in previous value of file name if available

     Config config;

     config.SetGroup( String( ResId( vID_STR_APPGROUP ) ) );

     filename = config.ReadKey( String( ResId( vID_STR_CONC_FILENAME )));

     if ( (filename.Len() > 0 ) && FileExists( filename ) )
     {
          use_file_rb.SetText( file_rb_prefix + filename );
          use_file_rb.Check( TRUE );
          use_value_rb.Check( FALSE ); 
     }
     else
     {
          use_file_rb.SetText( file_rb_prefix + String( ResId( vID_STR_NO_SELECTION )) );
          use_file_rb.Check( FALSE );
          use_value_rb.Check( TRUE ); 
     }
}













// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::SetFilenameHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::SetFilenameHandler( PushButton* )
{

   if ( TRUE == OpenFileDialog( filename, TEXT_FILETYPE ) )
   {
          use_file_rb.SetText( file_rb_prefix + filename );
   }
}










// ----------------------------------------------------------------------------
//         ChangeGlobalSpeciesConcDialog::ValidateDataAndAttemptChange()
// ............................................................................
// 
//   this fcn retrieves the data from the entry field, checks for its validity,
//   and stores it back in the parent Object if valid. If
//   the data has a problem then a diagnostic message is displayed. The function 
//   returns TRUE if data is valid, FALSE if not 
//
// ----------------------------------------------------------------------------

BOOL8 ChangeGlobalSpeciesConcDialog::ValidateDataAndAttemptChange()
{

     if ( TRUE == use_value_rb.IsChecked() )
     {
     // retrieve and test the data in the numeric entry field
          NumericString teststr( conc_val_ef.GetText( ) );
     
          if ( !teststr.IsValidNonZeroFLOAT64() )
          {
               SendErrorMessage( conc_val_ef.GetText() );
     
               return FALSE;
          }
          else
          {
               p_scheme->SetSpeciesConcToValue( species_list.GetSelectEntry(), teststr );
               return TRUE;
          }
     }


     if ( TRUE == use_file_rb.IsChecked() )
     {
          // test the filename data

     
          if ( FALSE == FileExists( filename ) )
          {
               SendInvalidFilenameMessage( );
     
               return FALSE;
          }
          else
          {
               enum CHANGE_CONC_RC  rc;

               GetpMainApp()->Wait( TRUE );
               rc = p_scheme->SetSpeciesConcUsingFileTable( species_list.GetSelectEntry(), filename );
               GetpMainApp()->Wait( FALSE );

               if ( CC_NO_ERROR == rc )
               {
                     Config config;
                
                     config.SetGroup( String( ResId( vID_STR_APPGROUP ) ) );
                
                     config.WriteKey( String( ResId( vID_STR_CONC_FILENAME )), filename);
                
                    return TRUE;
               }
               else
               {
                    NotifyOfFileInputError( rc, filename );
                    return FALSE;
               }

          }
     }



     return TRUE;
}





// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::CloseHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::CloseHandler( OKButton* )
{
     if ( TRUE == ChangeGlobalSpeciesConcDialog::ValidateDataAndAttemptChange() )
     {
          p_scheme->SetStatus( SIM_DATA_MODIFIED );
          EndDialog( CCD_RC_ACCEPT );
     }
}




// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::CancelHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::CancelHandler( CancelButton* )
{
     EndDialog( CCD_RC_CANCEL );
}






// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::SendErrorMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::SendErrorMessage( const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_CGC_INPUT_ERR3 ) ) + UserEntry + 
      String( ResId( vID_STR_CGC_INPUT_ERR4 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}


// ----------------------------------------------------------------------------
//            ChangeGlobalSpeciesConcDialog::SendInvalidFilenameMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::SendInvalidFilenameMessage() const
{
     String message = String( ResId( vID_STR_CGC_INPUT_ERR1 ) ) + filename +
      String( ResId( vID_STR_CGC_INPUT_ERR2 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}





// ----------------------------------------------------------------------------
//         ChangeGlobalSpeciesConcDialog::NotifyOfFileInputError()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void ChangeGlobalSpeciesConcDialog::NotifyOfFileInputError( enum CHANGE_CONC_RC ErrorCode, const String& FileName )
{
     String error_desc;

     switch ( ErrorCode )
     {
        case CC_NO_ERROR :
        default: 

             return;
             break;


        case CC_INVALID_FILE :
             error_desc = String( ResId( vID_STR_CGC_INPUT_ERR8 ) );
             break;

        case CC_FILE_IO_ERROR :
             error_desc = String( ResId( vID_STR_CGC_INPUT_ERR9 ) );
             break;


        case CC_INVALID_VALUE :
             error_desc = String( ResId( vID_STR_CGC_INPUT_ERR10 ) );
             break;


        case CC_OTHER_ERROR :
             error_desc = String( ResId( vID_STR_CGC_INPUT_ERR11 ) );
             break;

        case CC_FILE_LENGTH_ERROR :
             error_desc = String( ResId( vID_STR_CGC_INPUT_ERR12 ) );
             break;
     }

     String message =  String( ResId( vID_STR_CGC_INPUT_ERR5 ) ) +
                       FileName +
                       String( ResId( vID_STR_CGC_INPUT_ERR6 ) ) + 
                       error_desc +
                       String( ResId( vID_STR_CGC_INPUT_ERR7 ) ) ; 

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );
}






// ----------------------------------------------------------------------------
//            ChangeGlobalConcentrationDialog()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

enum CGC_RC ChangeGlobalConcentrationDialog( ReactionScheme* pScheme, Window* pParent )
{
     enum CGC_RC rc;

     ChangeGlobalSpeciesConcDialog* pdlg = new ChangeGlobalSpeciesConcDialog( pScheme, pParent);

     if ( pdlg )
     {
          rc = (CGC_RC) pdlg->Execute( );  // cast to enum from short
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return CGC_RC_FAIL;
     }
}


