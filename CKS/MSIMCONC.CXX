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
/*  FILE NAME : msimconc.cxx                                                */
/*                                                                          */
/*  This module  contains the functions for creating and processing all     */
/*  actions of the "Set Concentrations" dialog box                          */
/*                                                                          */
/*  Written using the Starview class libraries to provide common code for   */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 0.1  started August 1993                                        */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimstrg.hxx"

#include <stdlib.h>

static msimPSPECIES CheckForValidConcentrationData( msimPSPECIES SpeciesList,
                         msimWID Owner );



class ConcPanel : public ModalDialog
{

protected :
     FixedText aMsgString1;
     FixedText aFixedText2;
     GroupBox aGroupBox1;
     FixedText aFixedText3;
     ListBox aListBox;
     FixedText aConcLabel;
     FixedText aNameLabel;
     FixedText aConcUnits;
     FixedText aFixedText4;
     Edit aConcSLE;
     GroupBox aGroupBox2;
     OKButton aOKButton;
     CancelButton aCancelButton;
     HelpButton aHelpButton;
     msimBOOL data_altered;
     msimPSPECIES selected_species;
     msimRC rc;
     msimPINSTANCE instance;

     msimPSPECIES GetSpeciesPtrFromListboxPos( USHORT LineNumber, msimPSPECIES ptr );

public :
     ConcPanel( msimWID Owner, msimPINSTANCE Instance );

     void ListboxSelectHandler( ListBox PTR );
     void ConcModifyHandler( Edit PTR );
     void CloseHandler( OKButton PTR );
     void CancelHandler( CancelButton PTR );
};


// constructor

ConcPanel::ConcPanel( msimWID Owner, msimPINSTANCE Instance )
: ModalDialog ( Owner, ResId ( msimCONC_PANEL ) ),
aMsgString1 ( this, ResId ( msimCONC_MSG_STRING1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aListBox ( this, ResId ( msimCONC_LISTBOX ) ),
aConcLabel ( this, ResId ( msimCONC_CONC_LABEL ) ),
aNameLabel ( this, ResId ( msimCONC_SPECIES_LABEL ) ),
aConcUnits ( this, ResId ( msimCONC_CONC_UNITS ) ),
aFixedText4 ( this, ResId ( 4 ) ),
aConcSLE ( this, ResId ( msimCONC_CONC ) ),
aGroupBox2 ( this, ResId ( 2 ) ),
aOKButton ( this, ResId ( msimCONC_OK ) ),
aCancelButton ( this, ResId ( msimCONC_CANCEL ) ),
aHelpButton ( this, ResId ( msimCONC_HELP ) )
{
     FreeResource( );
     instance = Instance;              // save ptr
     data_altered = FALSE;

     aListBox.ChangeSelectHdl( LINK( this, ConcPanel, ListboxSelectHandler ) );
     aConcSLE.ChangeModifyHdl( LINK( this, ConcPanel, ConcModifyHandler ) );
     aOKButton.ChangeClickHdl( LINK( this, ConcPanel, CloseHandler ) );
     aCancelButton.ChangeClickHdl( LINK( this, ConcPanel, CancelHandler ) );

     // set window title
     if ( Instance->volume_option == msimVAR_VOL )
     {
          SetText( String( ResId( msimCONC_PANEL_INIT_AMT_STR ) )
               + String( Instance->base_filename ) );
          aConcLabel.SetText( String( ResId( msimCONC_PANEL_INIT_AMT_LBL ) ) );
     }
     else
     {
          SetText( String( ResId( msimCONC_PANEL_INIT_CONC_STR ) )
               + String( Instance->base_filename ) );
          aConcLabel.SetText( String( ResId( msimCONC_PANEL_INIT_CONC_LBL ) ) );
     }

     msimCascadeWindowOnOwner( this, Owner );

     // initialize the listbox text

     if ( Instance->ptr_to_species_list )
     {
          // turn off update to prevent flicker                   //
          aListBox.ChangeUpdateMode( FALSE );

          // fill the listbox                                     //
          for ( msimPSPECIES speciesptr = Instance->ptr_to_species_list;
                    speciesptr != NULL; speciesptr = speciesptr->next )
               aListBox.InsertEntry( String( speciesptr->name ) );

          aListBox.SelectEntryPos( 0 );
          aListBox.ChangeUpdateMode( TRUE );
          aListBox.Update( );
     }

     /* set the concentration units string */
     aConcUnits.SetText( String( msimConcUnits( Instance -> conc_units, Instance->volume_option ) ) );

     selected_species = Instance -> ptr_to_species_list;

     aNameLabel.SetText( String( selected_species->name ) );

     /* then set the conc entry field correspondingly */

     aConcSLE.SetText( String( selected_species -> initialconc ) );
}

void ConcPanel::ConcModifyHandler( Edit PTR )
{
     data_altered = TRUE;
}

void ConcPanel::CloseHandler( OKButton PTR )
{
     /* read conc entry field */
     msimStringCopy( selected_species->initialconc, aConcSLE.GetText( ),
          sizeof selected_species->initialconc );

     if ( selected_species = CheckForValidConcentrationData(
                    instance -> ptr_to_species_list, this ) )
     {
          /* returns non-NULL if erroneous  */
          /* concentration entries found   */
          /* return value points to erroneous species struct */

          /* set conc entry field, listbox and name static string */
          /* to offending data */

          aConcSLE.SetText( String( selected_species->initialconc ) );

          aNameLabel.SetText( String( selected_species->name ) );

          aListBox.SelectEntry( String( selected_species -> name ) );

          return;
     }

     if ( data_altered )
     {
          instance -> data_altered_since_lastsave = TRUE;
          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
     }

     instance -> nonzeroconcs = msimNumberNonZero( instance -> ptr_to_species_list );

     EndDialog( msimNO_ERROR );
}


// come here is user wants to cancel simulation start-up
void ConcPanel::CancelHandler( CancelButton PTR )
{
     EndDialog( msimUSER_ABORT );
}



msimPSPECIES ConcPanel::GetSpeciesPtrFromListboxPos( USHORT LineNumber, msimPSPECIES ptr )
{
     USHORT i = 0;

     while ( i++ < LineNumber )
          ptr = ptr -> next;
     return ptr;
}



void ConcPanel::ListboxSelectHandler( ListBox PTR )
{
     /* New element selected in listbox */

     /* first read current data entry */
     msimStringCopy( selected_species->initialconc, aConcSLE.GetText( ),
          sizeof selected_species->initialconc );

     /* now get new species ptr from listbox position */

     selected_species = GetSpeciesPtrFromListboxPos( aListBox.GetSelectEntryPos( ),
          instance -> ptr_to_species_list );

     /* set static text to reflect this */

     aNameLabel.SetText( String( selected_species->name ) );

     /* and initialize the entry field */

     aConcSLE.SetText( String( selected_species->initialconc ) );

}


/*--------------------------------------------------------------------------*/
/*                            msimSetConcentrations()                       */
/*..........................................................................*/
/*                                                                          */
/* dialog box for setting the species concentrations                        */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimSetConcentrations( msimPINSTANCE Instance, msimWID Owner )
{
     msimBeep( );
     return ConcPanel( Owner, Instance ) .Execute( );
}



/*--------------------------------------------------------------------------*/
/*                   CheckForValidConcentrationData()                       */
/*..........................................................................*/
/*                                                                          */
/* Function that scans through the list of defined species structures and   */
/* checks to be sure that all concentrations have been entered in valid     */
/* numeric format. Returns a pointer to the first invalid  data struct it   */
/* finds. If all are valid then returns NULL;                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
msimPSPECIES CheckForValidConcentrationData( msimPSPECIES SpeciesList, msimWID Owner )

{
     msimPSPECIES workptr;

     workptr = SpeciesList;
     while ( workptr )
     {
          if ( ! msimValidNonNegativeFloat( workptr -> initialconc ) )
          {
               String message = String( "All concentrations/amounts "
                    "must be non-negative real "
                    "values. The value [" );

               message += String( workptr -> initialconc );

               message += String( "] entered for species " );

               message += String( workptr->name );
               message += String( " is not in valid numeric form." );

               WarningBox( Owner, WB_OK | WB_DEF_OK, message ) .Execute( );

               return workptr;
          }
          workptr = workptr -> next;
     }

     return NULL;
}

/*--------------------------------------------------------------------------*/
/*                          msimNumberNonZero ()                            */
/*..........................................................................*/
/*                                                                          */
/* Function that counts the number of species in the linked list of         */
/* species data structures which have initial concentrations > 0.0 .        */
/* Returns the count as an unsigned integer                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

USHORT msimNumberNonZero( msimPSPECIES StartOfList )
{

     msimPSPECIES workptr;
     float tempreal;
     int returncode;
     USHORT i;

     i = 0;
     workptr = StartOfList;
     while ( workptr )
     {
          returncode = sscanf( workptr -> initialconc, "%f", &tempreal );
          if ( ( tempreal > 0.0 ) && ( returncode == 1 ) )
          {
               i++;
               workptr -> nonzero = TRUE;
          }
          else
               workptr -> nonzero = FALSE;

          workptr = workptr -> next;
     }

     return i;
}




