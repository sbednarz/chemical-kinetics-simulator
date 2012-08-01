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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimtemp.cxx                                                */
/*                                                                          */
/*  This module contains definitions of functions for setting variable and  */
/*  constant temperature parameters from dialogs                            */
/*                                                                          */
/*                                                                          */
/*  Author : Bill Hinsberg                                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"
#include "msimtprg.hxx"

#include <string.h>

class ConstTempDlg : public ModalDialog
{

protected :

     Edit ConstTempSLE;
     OKButton ClosePB;
     PushButton UndoPB;
     HelpButton HelpPB;
     FixedText aFixedText1;
     FixedText aFixedText2;
     GroupBox aGroupBox1;

     msimBOOL data_altered;
     msimPINSTANCE p_instance;

public :

     ConstTempDlg( Window PTR pParent, msimPINSTANCE pInstance );

     void SetModifiedFlagHandler( Edit PTR pEdit );
     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
};

ConstTempDlg::ConstTempDlg( Window PTR pParent, msimPINSTANCE pInstance )
: ModalDialog ( pParent, ResId ( msimCONST_TEMP_DLG ) ),
ConstTempSLE ( this, ResId ( msimCONST_TEMP_VALUE ) ),
ClosePB ( this, ResId ( msimCONST_TEMP_OK ) ),
UndoPB ( this, ResId ( msimCONST_TEMP_UNDO ) ),
HelpPB ( this, ResId ( msimCONST_TEMP_HELP ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aGroupBox1 ( this, ResId ( 1 ) )

{
     FreeResource( );
     p_instance = pInstance;

     SetText( GetText( ) + String( p_instance->base_filename ) );

     ConstTempSLE.SetText( String( pInstance->temp_data.const_temp ) );
     data_altered = FALSE;

     ConstTempSLE.ChangeModifyHdl(
          LINK( this, ConstTempDlg, SetModifiedFlagHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, ConstTempDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, ConstTempDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}

void ConstTempDlg::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}

void ConstTempDlg::CloseHandler( PushButton PTR )
{
     msimREAL_STRING temp;

     if ( data_altered )
     {
          if ( ! msimOKToInvalidateRxnData( this, p_instance ) )
               return;

          msimStringCopy( temp, ConstTempSLE.GetText( ), sizeof temp );

          // check the validity of the entry fields

          if ( ! msimValidPositiveFloat( temp ) )
          {
               WarningBox( this, ResId( msimCONST_T_ERROR_MSG ) ) .Execute( );
               return;
          }
          else
          {
               msimStringCopy( p_instance->temp_data.const_temp, temp,
                    sizeof p_instance->temp_data.const_temp );

               p_instance->data_altered_since_lastsave = TRUE;
          }
     }

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


void ConstTempDlg::UndoHandler( PushButton PTR )
{
     ConstTempSLE.SetText( String( p_instance->temp_data.const_temp ) );
     data_altered = FALSE;

     return;
}



class VarTempDlg : public ModalDialog
{
protected :
     Edit InitTempSLE;
     Edit TempConvStdSLE;
     OKButton ClosePB;
     PushButton UndoPB;
     HelpButton HelpPB;
     FixedText aFixedText1;
     FixedText aFixedText2;
     FixedText aFixedText3;
     FixedText aFixedText4;
     GroupBox aGroupBox1;


     msimBOOL data_altered;
     msimPINSTANCE p_instance;

     void InitializeEntries( );

public :
     VarTempDlg( Window PTR pParent, msimPINSTANCE pInstance );

     void SetModifiedFlagHandler( Edit PTR pEdit );
     void CloseHandler( PushButton PTR pButton );
     void UndoHandler( PushButton PTR pButton );
};


VarTempDlg::VarTempDlg( Window PTR pParent, msimPINSTANCE pInstance )
: ModalDialog ( pParent, ResId ( msimVAR_TEMP_DLG ) ),
InitTempSLE ( this, ResId ( msimVAR_TEMP_INITTEMP ) ),
TempConvStdSLE ( this, ResId ( msimVAR_TEMP_TC_STD ) ),
ClosePB ( this, ResId ( msimVAR_TEMP_OK ) ),
UndoPB ( this, ResId ( msimVAR_TEMP_UNDO ) ),
HelpPB ( this, ResId ( msimVAR_TEMP_HELP ) ),
aFixedText1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aFixedText4 ( this, ResId ( 4 ) ),
aGroupBox1 ( this, ResId ( 1 ) )
{
     FreeResource( );

     p_instance = pInstance;

     SetText( GetText( ) + String( p_instance->base_filename ) );
     InitializeEntries( );

     InitTempSLE.ChangeModifyHdl(
          LINK( this, VarTempDlg, SetModifiedFlagHandler ) );
     TempConvStdSLE.ChangeModifyHdl(
          LINK( this, VarTempDlg, SetModifiedFlagHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, VarTempDlg, CloseHandler ) );
     UndoPB.ChangeClickHdl(
          LINK( this, VarTempDlg, UndoHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}

void VarTempDlg::InitializeEntries( )
{
     InitTempSLE.SetText( String( p_instance->temp_data.initial_temp ) );
     TempConvStdSLE.SetText( String( p_instance->temp_data.convergence_std ) );

     data_altered = FALSE;
}

void VarTempDlg::UndoHandler( PushButton PTR )
{
     InitializeEntries( );
     return;
}


void VarTempDlg::SetModifiedFlagHandler( Edit PTR )
{
     data_altered = TRUE;

     return;
}


void VarTempDlg::CloseHandler( PushButton PTR )
{
     msimREAL_STRING init_temp;
     msimREAL_STRING conv_std;

     if ( data_altered )
     {
          msimStringCopy( init_temp, InitTempSLE.GetText( ), sizeof init_temp );
          msimStringCopy( conv_std, TempConvStdSLE.GetText( ), sizeof conv_std );

          // check the validity of the entry fields

          if ( ! msimValidPositiveFloat( init_temp ) )
          {
               WarningBox( this, ResId( msimINIT_T_ERROR_MSG ) ) .Execute( );
               return;
          }

          if ( ! msimValidPositiveFloat( conv_std ) )
          {
               WarningBox( this, ResId( msimCONV_STD_ERROR_MSG ) ) .Execute( );
               return;
          }

          // if we get here all data is valid

          msimStringCopy( p_instance->temp_data.initial_temp, init_temp, sizeof p_instance->temp_data.initial_temp );
          msimStringCopy( p_instance->temp_data.convergence_std, conv_std, sizeof p_instance->temp_data.convergence_std );

          p_instance->data_altered_since_lastsave = TRUE;
     }

     EndDialog( );

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}


void msimVarTempDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     VarTempDlg PTR pdlg =
                    new VarTempDlg( Owner, pInstance );

     if ( pdlg )
     {
          pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}




void msimConstTempDlg( msimWID Owner, msimPINSTANCE pInstance )
{
     ConstTempDlg PTR pdlg =
                      new ConstTempDlg( Owner, pInstance );

     if ( pdlg )
     {
          pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}




