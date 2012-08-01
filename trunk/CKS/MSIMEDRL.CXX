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

/* []-----------------------------------------------------------------------[]
   |  msimedrl.cxx   :   a module of msim                                   |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : defines a dialog for viewing/editing rxn rate laws   |
   |                                                                        |
   |  authors        :   Fumiko Allen and Bill Hinsberg , IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 1993                                           |
   |                                                                        |
   |  Uses the Starview Class libraries to provide multi-platform support   |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <string.h>


class EditRateLawDlg : public ModalDialog
{
protected :
     FixedText aFixedText1;
     FixedText FwdSpec1FixedText;
     FixedText FwdSpec2FixedText;
     FixedText FwdSpec3FixedText;
     FixedText FwdSpec4FixedText;

     Edit FwdSpec1Exponent;
     Edit FwdSpec2Exponent;
     Edit FwdSpec3Exponent;
     Edit FwdSpec4Exponent;

     FixedText aFixedText4;

     FixedText RevSpec1FixedText;
     FixedText RevSpec2FixedText;
     FixedText RevSpec3FixedText;
     FixedText RevSpec4FixedText;

     Edit RevSpec1Exponent;
     Edit RevSpec2Exponent;
     Edit RevSpec3Exponent;
     Edit RevSpec4Exponent;

     GroupBox FwdRateLawGB;
     GroupBox RevRateLawGB;

     OKButton ClosePB;
     PushButton UndoPB;
     PushButton DefaultsPB;
     HelpButton HelpPB;

     msimBOOL initialized;
     msimBOOL reversible;

     // default coefficients extracted from equation

     USHORT numreactants, numproducts;
     msimNAME_ARRAY reactants, products;
     msimEXPONENT_ARRAY r_def_exponents, p_def_exponents;

     // temporary arrays to hold either the default coefiicient values
     // or the values previously stored in the Data structure

     msimEXPONENT_ARRAY r_exponents, p_exponents;

     msimPINSTANCE pSimInstance;
     msimBOOL data_altered;
     msimPRXN rxnptr;
     msimEQUATION_STRING equation;
     msimWID owner;

     void InitializeStaticText( );
     msimRC AnalyzeEquation( );
     void NotifyOfBadRateLaw( PCHAR Direction, USHORT Position, PCHAR Entry );
     void ResizeDialog( );
     void Initialize_Entries( );

public :
     EditRateLawDlg( Window PTR pParent, msimPINSTANCE PSimInstance, msimPRXN Rxnptr, PCHAR Equation );

     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
     void DefaultsHandler( PushButton PTR pButton );
     void SetModiedFlagHandler( Edit PTR pEdit );

     inline msimBOOL SuccessfulInitialization( )
     {
          return initialized;
     };

};


EditRateLawDlg::
EditRateLawDlg( Window PTR pParent, msimPINSTANCE PSimInstance, msimPRXN Rxnptr, PCHAR Equation )
: ModalDialog ( pParent, ResId ( msimEDIT_RATELAW_PANEL ) ),
aFixedText1 ( this, ResId ( 1 ) ),
FwdSpec1FixedText ( this, ResId ( msimFWD_SPC1 ) ),
FwdSpec2FixedText ( this, ResId ( msimFWD_SPC2 ) ),
FwdSpec3FixedText ( this, ResId ( msimFWD_SPC3 ) ),
FwdSpec4FixedText ( this, ResId ( msimFWD_SPC4 ) ),
FwdSpec1Exponent ( this, ResId ( msimFWD_EXP1 ) ),
FwdSpec2Exponent ( this, ResId ( msimFWD_EXP2 ) ),
FwdSpec3Exponent ( this, ResId ( msimFWD_EXP3 ) ),
FwdSpec4Exponent ( this, ResId ( msimFWD_EXP4 ) ),
aFixedText4 ( this, ResId ( 4 ) ),
RevSpec1FixedText ( this, ResId ( msimREV_SPC1 ) ),
RevSpec2FixedText ( this, ResId ( msimREV_SPC2 ) ),
RevSpec3FixedText ( this, ResId ( msimREV_SPC3 ) ),
RevSpec4FixedText ( this, ResId ( msimREV_SPC4 ) ),
RevSpec1Exponent ( this, ResId ( msimREV_EXP1 ) ),
RevSpec2Exponent ( this, ResId ( msimREV_EXP2 ) ),
RevSpec3Exponent ( this, ResId ( msimREV_EXP3 ) ),
RevSpec4Exponent ( this, ResId ( msimREV_EXP4 ) ),
FwdRateLawGB ( this, ResId ( msimFWD_RATE_LAW_LABEL ) ),
RevRateLawGB ( this, ResId ( msimREV_RATELAW_LABEL ) ),
ClosePB ( this, ResId ( msimERL_CLOSE ) ),
UndoPB ( this, ResId ( msimERL_UNDO ) ),
DefaultsPB ( this, ResId ( msimERL_DEFAULTS ) ),
HelpPB ( this, ResId ( msimERL_HELP ) )
{
     FreeResource( );
     pSimInstance = PSimInstance;
     rxnptr = Rxnptr;
     msimStringCopy( equation, Equation, sizeof equation );
     owner = pParent;

     reversible = msimSeeIfReversible( equation );

     if ( ! reversible )
          ResizeDialog( );

     // Set the title of the dialog

     SetText( GetText( ) + String( pSimInstance->base_filename ) );

     // Initialize the panel

     if ( msimNO_ERROR != AnalyzeEquation( ) )
          initialized = FALSE;
     else
          initialized = TRUE;

     InitializeStaticText( );
     Initialize_Entries( );

     ClosePB.ChangeClickHdl(
          LINK( this, EditRateLawDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, EditRateLawDlg, UndoHandler ) );
     DefaultsPB.ChangeClickHdl(
          LINK( this, EditRateLawDlg, DefaultsHandler ) );

     FwdSpec1Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     FwdSpec2Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     FwdSpec3Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     FwdSpec4Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     RevSpec1Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     RevSpec2Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     RevSpec3Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );
     RevSpec4Exponent.ChangeModifyHdl(
          LINK( this, EditRateLawDlg, SetModiedFlagHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


void EditRateLawDlg::ResizeDialog( )
{
     SHORT offset = ( RevRateLawGB.GetPosPixel( ) .Y( ) +
          RevRateLawGB.GetSizePixel( ) .Height( ) ) -
     ( FwdRateLawGB.GetPosPixel( ) .Y( ) +
          FwdRateLawGB.GetSizePixel( ) .Height( ) );

     Point displacement = ( Point( 0, offset ) );

     ClosePB.ChangePosPixel( ClosePB.GetPosPixel( ) - displacement );
     UndoPB.ChangePosPixel( UndoPB.GetPosPixel( ) - displacement );
     DefaultsPB.ChangePosPixel( DefaultsPB.GetPosPixel( ) - displacement );
     HelpPB.ChangePosPixel( HelpPB.GetPosPixel( ) - displacement );

     ChangeSizePixel( Size( GetSizePixel( ) .Width( ), GetSizePixel( ) .Height( ) - offset ) );

}


msimRC EditRateLawDlg::AnalyzeEquation( )
{
     msimRC rc;
     USHORT i;

     // initialize the variables
     memset( r_exponents, 0, sizeof ( r_exponents ) );
     memset( p_exponents, 0, sizeof ( p_exponents ) );
     memset( r_def_exponents, 0, sizeof ( r_def_exponents ) );
     memset( p_def_exponents, 0, sizeof ( p_def_exponents ) );

     // obtain names of each species and the numbers of reactants and
     // products

     if ( msimNO_ERROR != ( rc = msimAnalyzeEqn( equation, reactants,
                         r_def_exponents,
                         &numreactants,
                         products, p_def_exponents,
                         &numproducts ) ) )
     {
          msimDisplayAnalysisError( rc, this );
          return rc;
     }


     /* if we find previously defined exponent data in the Data struct then */
     /* copy it into the temporary  exponent arrays  */

     for ( i = 0; i < numreactants; i++ )
     {
          if ( msimWords( rxnptr->fwdexponent[i] ) == 1 )
               msimStringCopy( r_exponents[i], rxnptr->fwdexponent[i],
                    sizeof r_exponents[0] );
          else
          {
               msimStringCopy( r_exponents[i], r_def_exponents[i],
                    sizeof r_exponents[0] );
               data_altered = TRUE;
          }
     }

     /* fill the remaining with default strings - should be zero-length strings */

     for (; i < msimMAX_NUMBER_OF_COMPONENTS; i++ )
          msimStringCopy( r_exponents[i], r_def_exponents[i],
               sizeof r_exponents[0] );


     if ( reversible )
     {
          for ( i = 0; i < numproducts; i++ )
          {
               if ( msimWords( rxnptr->revexponent[i] ) == 1 )
                    msimStringCopy( p_exponents[i], rxnptr->revexponent[i],
                         sizeof p_exponents[0] );
               else
               {
                    msimStringCopy( p_exponents[i], p_def_exponents[i],
                         sizeof p_exponents[0] );
                    data_altered = TRUE;
               }
          }

          /* fill the remaining with default strings - should be zero-length strings */

          for (; i < msimMAX_NUMBER_OF_COMPONENTS; i++ )
               msimStringCopy( p_exponents[i], p_def_exponents[i],
                    sizeof p_exponents[0] );
     }

     return msimNO_ERROR;
}


void EditRateLawDlg::InitializeStaticText( )
{
     String openbracket( "x [" );
     String closebracket( "]^" );

     FwdSpec1FixedText.SetText( openbracket + String( reactants[0] ) +
          closebracket );
     if ( numreactants >= 2 )
          FwdSpec2FixedText.SetText( openbracket + String( reactants[1] ) +
               closebracket );
     else
          FwdSpec2FixedText.Hide( );

     if ( numreactants >= 3 )
          FwdSpec3FixedText.SetText( openbracket + String( reactants[2] ) +
               closebracket );
     else
          FwdSpec3FixedText.Hide( );

     if ( numreactants == 4 )
          FwdSpec4FixedText.SetText( openbracket + String( reactants[3] ) +
               closebracket );
     else
          FwdSpec4FixedText.Hide( );

     if ( reversible )
     {
          // repeat the above
          RevSpec1FixedText.SetText( openbracket + String( products[0] ) +
               closebracket );
          if ( numproducts >= 2 )
               RevSpec2FixedText.SetText( openbracket + String( products[1] ) +
                    closebracket );
          else
               RevSpec2FixedText.Hide( );

          if ( numproducts >= 3 )
               RevSpec3FixedText.SetText( openbracket + String( products[2] ) +
                    closebracket );
          else
               RevSpec3FixedText.Hide( );

          if ( numproducts == 4 )
               RevSpec4FixedText.SetText( openbracket + String( products[3] ) +
                    closebracket );
          else
               RevSpec4FixedText.Hide( );
     }
     else
     {
          // if the reaction is not reversible then hide the information
          // regarding products.

          RevRateLawGB.Hide( );
          aFixedText4.Hide( );

          RevSpec1FixedText.Hide( );
          RevSpec2FixedText.Hide( );
          RevSpec3FixedText.Hide( );
          RevSpec4FixedText.Hide( );
     }
     return;
}



void EditRateLawDlg::Initialize_Entries( )
{
     data_altered = FALSE;

     FwdSpec1Exponent.SetText( String( r_exponents[0] ) );

     if ( numreactants < 2 )
          FwdSpec2Exponent.Hide( );
     else
          FwdSpec2Exponent.SetText( String( r_exponents[1] ) );

     if ( numreactants < 3 )
          FwdSpec3Exponent.Hide( );
     else
          FwdSpec3Exponent.SetText( String( r_exponents[2] ) );

     if ( numreactants < 4 )
          FwdSpec4Exponent.Hide( );
     else
          FwdSpec4Exponent.SetText( String( r_exponents[3] ) );

     // if the equation is reversible repeat the above process for the
     // products.

     if ( reversible )
     {
          RevSpec1Exponent.SetText( String( p_exponents[0] ) );

          if ( numproducts < 2 )
               RevSpec2Exponent.Hide( );
          else
               RevSpec2Exponent.SetText( String( p_exponents[1] ) );

          if ( numproducts < 3 )
               RevSpec3Exponent.Hide( );
          else
               RevSpec3Exponent.SetText( String( p_exponents[2] ) );

          if ( numproducts < 4 )
               RevSpec4Exponent.Hide( );
          else
               RevSpec4Exponent.SetText( String( p_exponents[3] ) );
     }
     else
     {
          RevSpec1Exponent.Hide( );
          RevSpec2Exponent.Hide( );
          RevSpec3Exponent.Hide( );
          RevSpec4Exponent.Hide( );
     }

     return;
}


void EditRateLawDlg::CloseHandler( PushButton PTR )
{
     USHORT k;
     msimEXPONENT_ARRAY p_expnt;
     msimEXPONENT_ARRAY r_expnt;

     if ( data_altered )
          if ( ! msimOKToInvalidateRxnData( this, pSimInstance ) )
               return;

     // get the entryfield contents into temp arrays for error checking

     msimStringCopy( r_expnt[0], FwdSpec1Exponent.GetText( ), sizeof r_expnt[0] );
     msimStringCopy( r_expnt[1], FwdSpec2Exponent.GetText( ), sizeof r_expnt[1] );
     msimStringCopy( r_expnt[2], FwdSpec3Exponent.GetText( ), sizeof r_expnt[2] );
     msimStringCopy( r_expnt[3], FwdSpec4Exponent.GetText( ), sizeof r_expnt[3] );

     if ( reversible )
     {
          msimStringCopy( p_expnt[0], RevSpec1Exponent.GetText( ), sizeof p_expnt[0] );
          msimStringCopy( p_expnt[1], RevSpec2Exponent.GetText( ), sizeof p_expnt[1] );
          msimStringCopy( p_expnt[2], RevSpec3Exponent.GetText( ), sizeof p_expnt[2] );
          msimStringCopy( p_expnt[3], RevSpec4Exponent.GetText( ), sizeof p_expnt[3] );
     }

     /* now check for valid numeric data */

     k = 0;

     while ( numreactants >= ( k + 1 ) )
     {
          if ( ! msimValidFloat( r_expnt[k] ) )
          {
               NotifyOfBadRateLaw( "Forward", k, r_expnt[k] );
               return;
          }
          k++;
     }

     if ( reversible )
     {
          k = 0;
          while ( numproducts >= ( k + 1 ) )
          {
               if ( ! msimValidFloat( p_expnt[k] ) )
               {
                    NotifyOfBadRateLaw( "Reverse", k, p_expnt[k] );
                    return;
               }
               k++;
          }
     }

     /* data is valid if we made it to here, so copy temp data to rxn struct */

     msimStringCopy( rxnptr-> fwdexponent[0], FwdSpec1Exponent.GetText( ),
          sizeof rxnptr->fwdexponent[0] );
     msimStringCopy( rxnptr-> fwdexponent[1], FwdSpec2Exponent.GetText( ),
          sizeof rxnptr->fwdexponent[1] );
     msimStringCopy( rxnptr-> fwdexponent[2], FwdSpec3Exponent.GetText( ),
          sizeof rxnptr->fwdexponent[2] );
     msimStringCopy( rxnptr-> fwdexponent[3], FwdSpec4Exponent.GetText( ),
          sizeof rxnptr->fwdexponent[3] );

     if ( reversible )
     {
          msimStringCopy( rxnptr->revexponent[0], RevSpec1Exponent.GetText( ),
               sizeof rxnptr->revexponent[0] );
          msimStringCopy( rxnptr->revexponent[1], RevSpec2Exponent.GetText( ),
               sizeof rxnptr->revexponent[1] );
          msimStringCopy( rxnptr->revexponent[2], RevSpec3Exponent.GetText( ),
               sizeof rxnptr->revexponent[2] );
          msimStringCopy( rxnptr->revexponent[3], RevSpec4Exponent.GetText( ),
               sizeof rxnptr->revexponent[3] );
     }

     if ( data_altered )
     {
          // set the appropriate flags in the data structure to TRUE

          pSimInstance->data_altered_since_lastsave = TRUE;
          pSimInstance->specieslist_altered = TRUE;
     }

     EndDialog( );

     return;
}


void EditRateLawDlg::NotifyOfBadRateLaw( PCHAR Direction, USHORT Position, PCHAR Entry )
{
     String posn[] =
     {
          "first", "second", "third", "fourth"
     };

     String message( "The " );

     message += posn[Position];
     message += String( " exponent in the Rate Law for the " );
     message += String( Direction );
     message += String( " Reaction [" );
     message += String( Entry );
     message += String( "] is not in a valid numeric format" );

     WarningBox( this, WB_OK | WB_DEF_OK, message ) .Execute( );
     return;
}





void EditRateLawDlg::UndoHandler( PushButton PTR )
{
     Initialize_Entries( );
     return;
}


void EditRateLawDlg::DefaultsHandler( PushButton PTR )
{

     // Set up entry fields in the panel with appropriate information

     FwdSpec1Exponent.SetText( String( r_def_exponents[0] ) );
     FwdSpec2Exponent.SetText( String( r_def_exponents[1] ) );
     FwdSpec3Exponent.SetText( String( r_def_exponents[2] ) );
     FwdSpec4Exponent.SetText( String( r_def_exponents[3] ) );

     // if the equation is reversible set up entry fields for the
     // products' rate law.

     if ( reversible )
     {
          RevSpec1Exponent.SetText( String( p_def_exponents[0] ) );
          RevSpec2Exponent.SetText( String( p_def_exponents[1] ) );
          RevSpec3Exponent.SetText( String( p_def_exponents[2] ) );
          RevSpec4Exponent.SetText( String( p_def_exponents[3] ) );
     }

     data_altered = TRUE;
     return;
}

void EditRateLawDlg::SetModiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;
     return;
}



// globally accessible fcn for invoking the dialog

void msimEditRateLawDialog( msimWID Owner, msimPINSTANCE pInstance,
          msimPRXN Rxnptr, PCHAR Equation )
{
     EditRateLawDlg PTR pdlg =
                        new EditRateLawDlg( Owner, pInstance, Rxnptr, Equation );

     if ( pdlg )
     {
          if ( pdlg->SuccessfulInitialization( ) )
               pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}
