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
/*  FILE NAME : msimextd.cxx                                                */
/*                                                                          */
/*  This module implements a dialog for specifying external data types      */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for multiple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1994                                          */
/*                                                                          */
/*  Authors : Bill Hinsberg                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/


#include "msim2.hxx"
#pragma hdrstop

#include "msimplot.hxx"

class SelectExtDataTypesDlg : public ModalDialog
{

protected:

    FixedText aFixedText;
    ListBox aListBox;
    OKButton aOKButton;
    CancelButton aCancelButton;
    HelpButton aHelpButton;
    GroupBox aGroupBox;

    USHORT& rxdata_type;
    USHORT& rydata_type;

    USHORT volume_option;

public:

    SelectExtDataTypesDlg( Window PTR pParent, USHORT VolumeOption,
     USHORT& rXDataType, USHORT& rYDataType, PCHAR Filename );

    void OKHandler( OKButton PTR );
    void CancelHandler( CancelButton PTR );
    void ListboxDoubleClickHandler( ListBox PTR );

};



SelectExtDataTypesDlg::SelectExtDataTypesDlg( Window PTR pParent, USHORT VolumeOption,
     USHORT& rXDataType, USHORT& rYDataType, PCHAR Filename ) :

    ModalDialog( pParent, ResId( msimEXT_DATA_TYPE_DLG) ),
    aFixedText( this, ResId( 1 ) ),
    aListBox( this, ResId( 1 ) ),
    aOKButton( this, ResId( 1 ) ),
    aCancelButton( this, ResId( 1 ) ),
    aHelpButton( this, ResId( 1 ) ),
    aGroupBox( this, ResId( 1 ) ),
    rxdata_type( rXDataType),
    rydata_type( rYDataType)
{
    FreeResource( );

    // save refs so we can return data when done 


    volume_option = VolumeOption;

    // remove the listbox entries pertaining to concentration if
    // this is a variable volume simulation, otherwise remove the
    // entries pertaining to amounts
      
    if ( VolumeOption == msimVAR_VOL)
    {
         // remove the first and second entries

	 aListBox.RemoveEntry(0);
	 aListBox.RemoveEntry(0);
              
    }
    else
    {
	 // remove the third and fourth entries

	 aListBox.RemoveEntry(2);
	 aListBox.RemoveEntry(2);
    }

    // set an initial selection in listbox

     aListBox.SelectEntryPos( 0, TRUE );

     aOKButton.ChangeClickHdl( LINK( this, SelectExtDataTypesDlg,
               OKHandler ) );

     aCancelButton.ChangeClickHdl( LINK( this, SelectExtDataTypesDlg,
               CancelHandler ) );

     aFixedText.SetText( String( ResId( msimEXT_DATA_TYPE_DLG_STR1 ) ) +
      String( Filename ) + String( ResId( msimEXT_DATA_TYPE_DLG_STR2 ) ) );

     aListBox.ChangeDoubleClickHdl( LINK( this, SelectExtDataTypesDlg, ListboxDoubleClickHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}

void SelectExtDataTypesDlg::ListboxDoubleClickHandler( ListBox PTR )
{
     // simulate pressing the ok button

     OKHandler( &aOKButton );
}



void SelectExtDataTypesDlg::OKHandler( OKButton PTR )
{
     // figure out what the data types are based on user input
     // the listbox order is
     // 
     //        entry                              LB position
     //
     //	    "Conc / time" or  "Amount / time"      0
     //	    "Conc / temp" or "Amount / temp"       1
     //	    "Volume / time"                        2
     //	    "Volume / temp"                        3
     //     "Pressure / time"                      4
     //	    "Pressure / temp"                      5
     //	    "Temp / time"                          6

     switch ( aListBox.GetSelectEntryPos( ) )
     {

     default :

     case 0 :

	  rxdata_type = TIME_DATA;
	  rydata_type = CONC_DATA;

	  break;

     case 1 :

	  rxdata_type = TEMP_DATA;
	  rydata_type = CONC_DATA;
	  break;


     case 2 :

	  rxdata_type = TIME_DATA;
	  rydata_type = VOL_DATA;
	  break;

     case 3 :

	  rxdata_type = TEMP_DATA;
	  rydata_type = VOL_DATA;
	  break;

     case 4 :
	  rxdata_type = TIME_DATA;
	  rydata_type = PRESS_DATA;
	  break;


     case 5 :
	  rxdata_type = TEMP_DATA;
	  rydata_type = PRESS_DATA;
	  break;

     case 6 :
	  rxdata_type = TIME_DATA;
	  rydata_type = TEMP_DATA_Y;

     }


     // send return code to caller about exit status

     EndDialog( msimNO_ERROR );
}

void SelectExtDataTypesDlg::CancelHandler( CancelButton PTR )
{
     EndDialog( msimUSER_ABORT );

}




msimRC msimSelectExternalDataTypesDlg( msimWID Owner, USHORT VolumeOption,
        USHORT& rXDataType, USHORT& rYDataType, PCHAR Filename )
{
     msimRC rc;

     SelectExtDataTypesDlg PTR pdlg =
			   new SelectExtDataTypesDlg( Owner, VolumeOption,
                                     rXDataType, rYDataType, Filename ) ;


     if ( pdlg )
     {
          rc = pdlg->Execute( );
          delete pdlg;
          return rc;
     }
     else
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );
          return msimMEM_ALLOC_ERROR;
     }
}
