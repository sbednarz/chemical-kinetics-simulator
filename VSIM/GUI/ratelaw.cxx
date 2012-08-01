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
//  RATELAW.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the SpecialRateLawDialog object, which allows
//  user netry of a non-stoiciometric rate law 
//
//  CREATED : 7.26.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "mainapp.hxx"
#include "ratelaw.hxx"
#include "assert.hxx"

#endif


#include "ratelaw.hrc"
#include "strings.hrc"




//--------------------------------------------------------------------------
//                     class SpecialRateLawDialog
//..........................................................................
//                                                                          
//                                                                          
//--------------------------------------------------------------------------

class SpecialRateLawDialog: public ModalDialog
{
protected:
     FixedText fwd_k_label;
 
     FixedText fwd_speciesname_1;
     FixedText fwd_speciesname_2;
     FixedText fwd_speciesname_3;
     FixedText fwd_speciesname_4;
 
     Edit      fwd_exponent_1;
     Edit      fwd_exponent_2;
     Edit      fwd_exponent_3;
     Edit      fwd_exponent_4;
 
     GroupBox  fwd_ratelaw_gb;
 
     FixedText rev_k_label;
     FixedText rev_speciesname_1;
     FixedText rev_speciesname_2;
     FixedText rev_speciesname_3;
     FixedText rev_speciesname_4;
 
     Edit      rev_exponent_1;
     Edit      rev_exponent_2;
     Edit      rev_exponent_3;
     Edit      rev_exponent_4;
 
     GroupBox rev_ratelaw_gb;
 
     OKButton ok_btn;
     PushButton undo_btn;
     PushButton defaults_btn;
     HelpButton help_btn;

     Reaction&   r_init_rxn;
     Reaction    rxn;

     BOOL8       reversible;
     BOOL8       data_altered;

     void        ResizeDialog();
     void        InitializeDialog( );
     void        InitializeEntryFields( );
     void        SendErrorMessage( const String& FieldName, const String& UserEntry ) const;

public:

     SpecialRateLawDialog( Window * pParent, Reaction& rReaction );

     void        TextModifiedHandler( Edit* );
     void        OKHandler( OKButton* );
     void        UndoHandler( PushButton* );
     void        DefaultsHandler( PushButton* );
};







//--------------------------------------------------------------------------
//                SpecialRateLawDialog::SpecialRateLawDialog()
//..........................................................................
//                                                                          
//   constructor 
//                                                                          
//--------------------------------------------------------------------------

SpecialRateLawDialog::SpecialRateLawDialog( Window * pParent, Reaction& rReaction )
: ModalDialog( pParent, ResId( vID_DLG_RATE_LAW ) ),
fwd_k_label( this, ResId( vID_STEXT_FWD_K ) ),
fwd_speciesname_1( this, ResId( vID_STEXT_FWD_SPECIES_1 ) ),
fwd_speciesname_2( this, ResId( vID_STEXT_FWD_SPECIES_2 ) ),
fwd_speciesname_3( this, ResId( vID_STEXT_FWD_SPECIES_3 ) ),
fwd_speciesname_4( this, ResId( vID_STEXT_FWD_SPECIES_4 ) ),
fwd_exponent_1( this, ResId( vID_EF_FWD_EXPONENT_1 ) ),
fwd_exponent_2( this, ResId( vID_EF_FWD_EXPONENT_2 ) ),
fwd_exponent_3( this, ResId( vID_EF_FWD_EXPONENT_3 ) ),
fwd_exponent_4( this, ResId( vID_EF_FWD_EXPONENT_4 ) ),
fwd_ratelaw_gb( this, ResId( vID_GB_FWD_RATE_LAW ) ),
rev_k_label( this, ResId( vID_STEXT_REV_K ) ),
rev_speciesname_1( this, ResId( vID_STEXT_REV_SPECIES_1 ) ),
rev_speciesname_2( this, ResId( vID_STEXT_REV_SPECIES_2 ) ),
rev_speciesname_3( this, ResId( vID_STEXT_REV_SPECIES_3 ) ),
rev_speciesname_4( this, ResId( vID_STEXT_REV_SPECIES_4 ) ),
rev_exponent_1( this, ResId( vID_EF_REV_EXPONENT_1 ) ),
rev_exponent_2( this, ResId( vID_EF_REV_EXPONENT_2 ) ),
rev_exponent_3( this, ResId( vID_EF_REV_EXPONENT_3 ) ),
rev_exponent_4( this, ResId( vID_EF_REV_EXPONENT_4 ) ),
rev_ratelaw_gb( this, ResId( vID_GB_REV_RATE_LAW ) ),
ok_btn( this, ResId( vID_BTN_OK ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
defaults_btn( this, ResId( vID_BTN_DEFAULTS ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
r_init_rxn( rReaction ), 
rxn( rReaction ),
reversible( FALSE ),
data_altered( FALSE )
{
     FreeResource();

     InitializeDialog( );
     InitializeEntryFields( );

     ok_btn.ChangeClickHdl(
          LINK( this, SpecialRateLawDialog, OKHandler ) );
     undo_btn.ChangeClickHdl(
          LINK( this, SpecialRateLawDialog, UndoHandler ) );

     defaults_btn.ChangeClickHdl(
          LINK( this, SpecialRateLawDialog, DefaultsHandler ) );

     fwd_exponent_1.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) ); 
     fwd_exponent_2.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     fwd_exponent_3.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     fwd_exponent_4.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     rev_exponent_1.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     rev_exponent_2.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     rev_exponent_3.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  
     rev_exponent_4.ChangeModifyHdl( LINK( this, SpecialRateLawDialog, TextModifiedHandler ) );  

     GetpMainApp()->CascadeWindowOnParent( this, GetParent() );
}



//--------------------------------------------------------------------------
//                SpecialRateLawDialog::ResizeDialog( )
//..........................................................................
//                                                                          
//   resize dialog window depending on number of species to be dealt with , and
//   depending on whether the reaction is reversible
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::ResizeDialog()
{
     INT16 offset = ( rev_ratelaw_gb.GetPosPixel( ).Y( ) +
                      rev_ratelaw_gb.GetSizePixel( ).Height( ) 
                    ) -
                    ( fwd_ratelaw_gb.GetPosPixel( ) .Y( ) +
                      fwd_ratelaw_gb.GetSizePixel( ) .Height( ) 
                    );

     Point displacement = ( Point( 0, offset ) );

     ok_btn.ChangePosPixel( ok_btn.GetPosPixel( ) - displacement );
     undo_btn.ChangePosPixel( undo_btn.GetPosPixel( ) - displacement );
     defaults_btn.ChangePosPixel( defaults_btn.GetPosPixel( ) - displacement );
     help_btn.ChangePosPixel( help_btn.GetPosPixel( ) - displacement );

     ChangeSizePixel( Size( GetSizePixel( ).Width( ), GetSizePixel( ).Height( ) - offset ) );
}







//--------------------------------------------------------------------------
//                SpecialRateLawDialog::InitializeDialog( )
//..........................................................................
//                                                                          
//   initialize static text and possible chnage dialog size upon first 
//   dialog initialization
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::InitializeDialog( )
{
     const String head( String( ResId( vID_STR_RATELAW_HEAD ) ) );
     const String tail( String( ResId( vID_STR_RATELAW_TAIL ) ) );

     UINT16 num_species = rxn.GetNumReactants();


     switch ( num_species )
     {
     case 4 :
          fwd_speciesname_4.SetText( head + rxn.GetReactantName( 3 ) + tail );
          fwd_speciesname_4.Show();

          // do not break - fall through on purpose 

     case 3:

          fwd_speciesname_3.SetText( head + rxn.GetReactantName( 2 ) + tail );
          fwd_speciesname_3.Show();

          // do not break - fall through on purpose 

     case 2:

          fwd_speciesname_2.SetText( head + rxn.GetReactantName( 1 ) + tail );
          fwd_speciesname_2.Show();

          // do not break - fall through on purpose 

     case 1:

          fwd_speciesname_1.SetText( head + rxn.GetReactantName( 0 ) + tail );

          break;

     default:

          ASSERT( 0 ) ;
          break;

     }

     // do the same with exponents for reverse rate if necessary 

     if ( rxn.IsReversible() )
     {
          num_species = rxn.GetNumProducts( );

          switch ( num_species )
          {
          case 4 :
               rev_speciesname_4.SetText( head + rxn.GetProductName( 3 ) + tail );
               rev_speciesname_4.Show();
     
               // do not break - fall through on purpose 
     
          case 3:
     
               rev_speciesname_3.SetText( head + rxn.GetProductName( 2 ) + tail );
               rev_speciesname_3.Show();
     
               // do not break - fall through on purpose 
     
          case 2:
     
               rev_speciesname_2.SetText( head + rxn.GetProductName( 1 ) + tail );
               rev_speciesname_2.Show();
     
               // do not break - fall through on purpose 
     
          case 1:
     
               rev_speciesname_1.SetText( head + rxn.GetProductName( 0 ) + tail );
               rev_speciesname_1.Show();
     
               break;
     
          default:
     
               ASSERT( 0 ) ;
               break;
     
          }

          rev_k_label.Show();
          rev_ratelaw_gb.Show();

     }
     else
          // we will not have reverse exponents - resize dialog accordingly

          ResizeDialog();
}






//--------------------------------------------------------------------------
//                SpecialRateLawDialog::InitializeEntryFields()
//..........................................................................
//                                                                          
//   initialize entry fields
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::InitializeEntryFields( )
{
     UINT16 num_species = rxn.GetNumReactants();


     switch ( num_species )
     {
     case 4 :
          fwd_exponent_4.SetText( rxn.GetReactantExponent( 3 ) );
          fwd_exponent_4.Show();

          // do not break - fall through on purpose 

     case 3:

          fwd_exponent_3.SetText( rxn.GetReactantExponent( 2 ) );
          fwd_exponent_3.Show();

          // do not break - fall through on purpose 

     case 2:

          fwd_exponent_2.SetText( rxn.GetReactantExponent( 1 ) );
          fwd_exponent_2.Show();

          // do not break - fall through on purpose 

     case 1:

          fwd_exponent_1.SetText( rxn.GetReactantExponent( 0 ) );

          break;

     default:

          ASSERT( 0 ) ;
          break;

     }

     // do the same with exponents for reverse rate if necessary 

     if ( rxn.IsReversible() )
     {
          num_species = rxn.GetNumProducts( );

          switch ( num_species )
          {
          case 4 :
               rev_exponent_4.SetText( rxn.GetProductExponent( 3 ) );
               rev_exponent_4.Show();
     
               // do not break - fall through on purpose 
     
          case 3:
     
               rev_exponent_3.SetText( rxn.GetProductExponent( 2 ) );
               rev_exponent_3.Show();
     
               // do not break - fall through on purpose 
     
          case 2:
     
               rev_exponent_2.SetText( rxn.GetProductExponent( 1 ) );
               rev_exponent_2.Show();
     
               // do not break - fall through on purpose 
     
          case 1:
     
               rev_exponent_1.SetText( rxn.GetProductExponent( 0 ) );
               rev_exponent_1.Show();
     
               break;
     
          default:
     
               ASSERT( 0 ) ;
               break;
     
          }
     }
}







//--------------------------------------------------------------------------
//                SpecialRateLawDialog::OKHandler( )
//..........................................................................
//                                                                          
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::OKHandler( OKButton* )
{
     if ( data_altered)
     {
          // .... read in all that stuff 

          UINT16 num_species = rxn.GetNumReactants();
     
     
          switch ( num_species )
          {
          case 4 :
               rxn.SetReactantExponent( 3, fwd_exponent_4.GetText() );

               if ( !rxn.GetReactantExponent( 3 ).IsValidFLOAT64() )
               {
                    SendErrorMessage( fwd_speciesname_4.GetText(), 
                     rxn.GetReactantExponent( 3 ) );

                    return;
               }
     
               // do not break - fall through on purpose 
     
          case 3:
     
               rxn.SetReactantExponent( 2, fwd_exponent_3.GetText() );
     
               if ( !rxn.GetReactantExponent( 2 ).IsValidFLOAT64() )
               {
                    SendErrorMessage( fwd_speciesname_3.GetText(), 
                     rxn.GetReactantExponent( 2 ) );

                    return;
               }
     
               // do not break - fall through on purpose 
     
          case 2:
     
               rxn.SetReactantExponent( 1, fwd_exponent_2.GetText() );
     
               if ( !rxn.GetReactantExponent( 1 ).IsValidFLOAT64() )
               {
                    SendErrorMessage( fwd_speciesname_2.GetText(), 
                     rxn.GetReactantExponent( 1 ) );

                    return;
               }
     
               // do not break - fall through on purpose 
     
          case 1:
     
               rxn.SetReactantExponent( 0, fwd_exponent_1.GetText() );

               if ( !rxn.GetReactantExponent( 0 ).IsValidFLOAT64() )
               {
                    SendErrorMessage( fwd_speciesname_1.GetText(), 
                     rxn.GetReactantExponent( 0 ) );

                    return;
               }
     
     
               break;
     
          default:
     
               ASSERT( 0 ) ;
               break;
     
          }
     
          // do the same with exponents for reverse rate if necessary 
     
          if ( rxn.IsReversible() )
          {
               num_species = rxn.GetNumProducts( );
     
               switch ( num_species )
               {
               case 4 :
                    rxn.SetProductExponent( 3, rev_exponent_4.GetText() );

                    if ( !rxn.GetProductExponent( 3 ).IsValidFLOAT64() )
                    {
                         SendErrorMessage( rev_speciesname_4.GetText(), 
                          rxn.GetProductExponent( 3 ) );

                         return;
                    }
     
          
                    // do not break - fall through on purpose 
          
               case 3:
          
                    rxn.SetProductExponent( 2, rev_exponent_3.GetText() );
          
                    if ( !rxn.GetProductExponent( 2 ).IsValidFLOAT64() )
                    {
                         SendErrorMessage( rev_speciesname_3.GetText(), 
                          rxn.GetProductExponent( 2 ) );

                         return;
                    }
                    // do not break - fall through on purpose 
          
               case 2:
          
                    rxn.SetProductExponent( 1, rev_exponent_2.GetText() );
          
                    if ( !rxn.GetProductExponent( 1 ).IsValidFLOAT64() )
                    {
                         SendErrorMessage( rev_speciesname_2.GetText(), 
                          rxn.GetProductExponent( 1 ) );

                         return;
                    }
                    // do not break - fall through on purpose 
          
               case 1:
          
                    rxn.SetProductExponent( 0, rev_exponent_1.GetText() );

                    if ( !rxn.GetProductExponent( 0 ).IsValidFLOAT64() )
                    {
                         SendErrorMessage( rev_speciesname_1.GetText(), 
                          rxn.GetProductExponent( 0 ) );

                         return;
                    }
          
                    break;
          
               default:
          
                    ASSERT( 0 ) ;
                    break;
          
               }
          }

          r_init_rxn = rxn;
          EndDialog( RL_RC_ACCEPT );
     }
     else
          EndDialog( RL_RC_CANCEL );

}






//--------------------------------------------------------------------------
//                SpecialRateLawDialog::UndoHandler( )
//..........................................................................
//                                                                          
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::UndoHandler( PushButton* )
{
     InitializeEntryFields( );
     undo_btn.Disable();
}


// ----------------------------------------------------------------------------
//              RxnDataEntryDialog::SendErrorMessage( )
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void SpecialRateLawDialog::SendErrorMessage( const String& FieldName, const String& UserEntry ) const
{
     String message = String( ResId( vID_STR_RDE_INPUT_ERR1 ) ) + UserEntry + 
      String( ResId( vID_STR_RDE_INPUT_ERR2 ) ) + FieldName +
      String( ResId( vID_STR_RDE_INPUT_ERR3 ) );

     WarningBox box( GetParent(), WB_OK | WB_DEF_OK, message );
     box.Execute( );

     return;
}







//--------------------------------------------------------------------------
//                SpecialRateLawDialog::DefaultsHandler()
//..........................................................................
//                                                                          
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::DefaultsHandler( PushButton* )
{
     const String tail( String( ResId( vID_STR_DEFAULT_EXPONENT_TAIL ) ) );

     UINT16 num_species = rxn.GetNumReactants();


     switch ( num_species )
     {
     case 4 :
          fwd_exponent_4.SetText( rxn.GetReactantStoichiometry( 3 ) + tail );
          fwd_exponent_4.Show();

          // do not break - fall through on purpose 

     case 3:

          fwd_exponent_3.SetText( rxn.GetReactantStoichiometry( 2 ) + tail ); 
          fwd_exponent_3.Show();

          // do not break - fall through on purpose 

     case 2:

          fwd_exponent_2.SetText( rxn.GetReactantStoichiometry( 1 ) + tail ); 
          fwd_exponent_2.Show();

          // do not break - fall through on purpose 

     case 1:

          fwd_exponent_1.SetText( rxn.GetReactantStoichiometry( 0 ) + tail ); 

          break;

     default:

          ASSERT( 0 ) ;
          break;

     }

     // do the same with exponents for reverse rate if necessary 

     if ( rxn.IsReversible() )
     {
          num_species = rxn.GetNumProducts( );

          switch ( num_species )
          {
          case 4 :
               rev_exponent_4.SetText( rxn.GetProductStoichiometry( 3 ) + tail ); 
               rev_exponent_4.Show();
     
               // do not break - fall through on purpose 
     
          case 3:
     
               rev_exponent_3.SetText( rxn.GetProductStoichiometry( 2 ) + tail );  
               rev_exponent_3.Show();
     
               // do not break - fall through on purpose 
     
          case 2:
     
               rev_exponent_2.SetText( rxn.GetProductStoichiometry( 1 ) + tail );  
               rev_exponent_2.Show();
     
               // do not break - fall through on purpose 
     
          case 1:
     
               rev_exponent_1.SetText( rxn.GetProductStoichiometry( 0 ) + tail );  
     
               break;
     
          default:
     
               ASSERT( 0 ) ;
               break;
     
          }
     }

     TextModifiedHandler( NULL );
}



//--------------------------------------------------------------------------
//                SpecialRateLawDialog::TextModifiedHandler()
//..........................................................................
//                                                                          
//                                                                          
//--------------------------------------------------------------------------

void SpecialRateLawDialog::TextModifiedHandler( Edit* )
{
     if (! data_altered )
     {
          data_altered = TRUE;
          undo_btn.Enable();
     }
}



// ----------------------------------------------------------------------------
//                  CompartmentDialog::CancelHandler( )
// ............................................................................
// 
//   this global function creates a CompartmentDialog
//
// ----------------------------------------------------------------------------

RATE_LAW_RC   EditRateLaw( Window* pParent, Reaction& rReaction )
{
     SpecialRateLawDialog* pdlg = new SpecialRateLawDialog( pParent, rReaction);

     if ( pdlg )
          return (RATE_LAW_RC) pdlg->Execute();
     else
          return RL_RC_FAIL;

}

