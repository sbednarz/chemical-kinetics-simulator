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

/* []----------------------------------------------------------------------[]
   |  msimtext.cxx   :   contains text I/O function  for msim               |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  This file contains the functions that create a text file summary      |
   |  of a reaction scheme                                                  |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   August 20 1993                                     |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop

#include <string.h>
#include <time.h>

#include "msimfile.hxx"
#include "msimstrg.hxx"

#if defined(__MAC__) && defined(__PPC__)
#include <sysdep.hxx>
#endif


#define  HEADER_STR_1                            \
             "--- Summary of reaction scheme in file \"%s\"\n"
#define  HEADER_STR_2                            \
             "--- This summary was created on %s\n"

static PCHAR IndentString = "   ";

static msimRC PrintSpecialKinetics( FILE *F, msimPRXN RxnPtr );
static void WriteNotebookTextToFile( FILE PTR pFile,  PCHAR pTextBuffer );

/*--------------------------------------------------------------------------*/
/*                        msimCreateTextSummary()                           */
/*..........................................................................*/
/*                                                                          */
/* gets a filename for the text output file, and creates the file using     */
/* the contents of the Instance struct passed to it                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimCreateTextSummary( msimPINSTANCE Instance, msimWID Owner )
{
     FILE *f;
     msimPRXN rxnptr;
     msimPSPECIES species_ptr;
     struct tm *current_time;
     time_t local_time;
     USHORT rxn_no = 0;
     USHORT species_no = 0;
     PCHAR tmp_ptr;
     msimSTRING title;
     msimFLOAT fwd_exponent, rev_exponent;

     sprintf( title, "Create Text Summary : %s", Instance->base_filename );

     /* check to be sure some data has been defined                         */

     if ( ( Instance == NULL ) || ( Instance->ptr_to_rxnlist == NULL && Instance->
                    ptr_to_notebook_text == NULL ) )
     {
          WarningBox( Owner, ResId( msimNO_RXN_LOADED_MSG ) ) .Execute( );
          return msimNO_EQUATION;
     }

     /* get a filename                                               */

     if ( msimUSER_ABORT == msimGetFileName( Owner, msimAppOptions.text_output_template,
                    Instance->print_target, WB_SAVEAS, title, msimTEXTFILE_TYPE,
                    sizeof Instance->print_target ) )
          return msimUSER_ABORT;

     aMainApp.Wait( TRUE );

     /* open the file and check for problems                                */

     if ( NULL == ( f = fopen( Instance->print_target, "w" ) ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( Instance->print_target, Owner, ( USHORT )
               msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     fprintf( f, HEADER_STR_1, Instance->filename );

     time( &local_time );              /* get time in seconds                 */
     current_time = localtime( &local_time );

     fprintf( f, HEADER_STR_2, asctime( current_time ) );

     if ( Instance->ptr_to_notebook_text )
     {
          fprintf( f, "\n\n---------- USER NOTES ON REACTION SCHEME ----------\n\n" );

          WriteNotebookTextToFile( f,  Instance->ptr_to_notebook_text );

          fprintf( f, "\n\n");
     }

     /* write out the reaction scheme, walking through the linked list      */

     fprintf( f, "\n\n---------- LIST OF REACTION STEPS ----------\n\n" );

     rxnptr = Instance->ptr_to_rxnlist;

     while ( ( rxnptr != NULL ) && ! ferror( f ) )
     {
          msimCalcAFactorConvExponent( rxnptr, &fwd_exponent, &rev_exponent );

          rxn_no++;
          fprintf( f, "\nReaction no. %hd :\n\n%s%s\n\n", rxn_no,
               IndentString, rxnptr->equation );

          if ( rxnptr->singlerate )
          {
               fprintf( f, "%sForward Rate = %s (%s)\n", IndentString, rxnptr->
                    fwdA, msimAFactorUnits( fwd_exponent, Instance->conc_units,
                         Instance->volume_units, Instance->time_units ) );
               if ( rxnptr->reversible )
                    fprintf( f, "%sReverse Rate = %s (%s)\n", IndentString, rxnptr
                         ->revA, msimAFactorUnits( rev_exponent, Instance->
                              conc_units, Instance->volume_units, Instance->time_units )
                    );
          }
          else
          {
               fprintf( f,
                    "%sForward A Factor             = %s (%s)\n", IndentString, rxnptr->fwdA,
                    msimAFactorUnits( fwd_exponent, Instance->conc_units, Instance
                    ->volume_units, Instance->time_units ) );
               fprintf( f, "%sForward Temperature Exponent = %s\n", IndentString,
                    rxnptr->fwdM );
               fprintf( f,
                    "%sForward Activation Energy    = %s (k%s)\n", IndentString, rxnptr->fwdEa,
                    msimEaUnits( Instance->energy_units ) );

               if ( rxnptr->reversible )
               {
                    fprintf( f,
                         "%sReverse A Factor             = %s (%s)\n", IndentString, rxnptr->revA,
                         msimAFactorUnits( rev_exponent, Instance->conc_units,
                              Instance->volume_units, Instance->time_units ) );
                    fprintf( f,
                         "%sReverse Temperature Exponent = %s\n", IndentString, rxnptr->revM );
                    fprintf( f,
                         "%sReverse Activation Energy    = %s (k%s)\n", IndentString, rxnptr->
                         revEa, msimEaUnits( Instance->energy_units ) );
               }
          }

          if ( rxnptr->not_stoich )
               PrintSpecialKinetics( f, rxnptr );

          rxnptr = rxnptr->next;
     }

     /* write out the species data, walking through the linked list         */

     fprintf( f, "\n\n---------- LIST OF SPECIES ----------\n\n" );

     species_ptr = Instance->ptr_to_species_list;

     while ( ( species_ptr != NULL ) && ! ferror( f ) )
     {
          species_no++;
          fprintf( f, "\nSpecies no. %hd : %s\n\n", species_no, species_ptr->
               name );
          fprintf( f, "%sInitial %s = %s (%s)\n",
               IndentString,
               Instance->volume_option == msimVAR_VOL ? "amount" : "concentration",
               species_ptr->initialconc,
               msimConcUnits( Instance->conc_units, Instance->volume_option ) );

          if ( Instance->volume_option == msimVAR_VOL )
          {
               switch ( species_ptr->phys_state )
               {
               case msimSOLID :
                    tmp_ptr = "Solid";
                    break;

               case msimLIQUID :
                    tmp_ptr = "Liquid";
                    break;

               case msimGAS :
                    tmp_ptr = "Gas";
                    break;

               default :
                    tmp_ptr = "Undefined";
                    break;
               }

               fprintf( f, "%sPhysical State : %s\n", IndentString, tmp_ptr );
               fprintf( f, "%sMolar Density  : %s (moles/%s)\n", IndentString,
                    species_ptr->molardensity, msimVolumeUnits( Instance->
                         volume_units ) );
          }

          if ( Instance->temp_option == msimVAR_TEMP )
          {
               fprintf( f, "\n%sThermochemical Coefficients :\n", IndentString );
               fprintf( f, "%s  Enthalpy = %s (k%s)\n", IndentString, species_ptr
                    ->thermcoeff[0], msimEaUnits( Instance->energy_units ) );
               fprintf( f, "%s  Coeff A  = %s (%s)\n", IndentString, species_ptr->
                    thermcoeff[1], msimEaUnits( Instance->energy_units ) );
               fprintf( f, "%s  Coeff B  = %s (%s-deg)\n", IndentString,
                    species_ptr->thermcoeff[2], msimEaUnits( Instance->energy_units
                    ) );
               fprintf( f, "%s  Coeff C  = %s (%s-deg^2)\n", IndentString,
                    species_ptr->thermcoeff[3], msimEaUnits( Instance->energy_units
                    ) );
               fprintf( f, "%s  Coeff D  = %s (%s-deg^3)\n", IndentString,
                    species_ptr->thermcoeff[4], msimEaUnits( Instance->energy_units
                    ) );
          }

          species_ptr = species_ptr->next;
     }

     fprintf( f, "\n\n---------- REACTION CONDITIONS ----------\n\n" );

     switch ( Instance->volume_option )

     {
     case msimCONST_VOL :
          tmp_ptr = "constant";
          break;

     case msimVAR_VOL :
          tmp_ptr = "variable";
          break;

     case msimDONT_CARE_VOL :
          tmp_ptr = "not explicitly tracked";
          break;

     default :
          tmp_ptr = "undefined";
          break;
     }

     fprintf( f, "%sVolume is %s\n", IndentString, tmp_ptr );

     fprintf( f, "%sPressure is %s\n", IndentString, Instance->variablepress ?
               "variable" : "constant" );

     switch ( Instance->temp_option )

     {
     case msimCONST_TEMP :
          tmp_ptr = "constant";
          break;

     case msimVAR_TEMP :
          tmp_ptr = "variable";
          break;

     case msimPROGR_TEMP :
          tmp_ptr = "programmed";
          break;

     default :
          tmp_ptr = "undefined";
          break;
     }

     fprintf( f, "%sTemperature is %s\n", IndentString, tmp_ptr );

     if ( Instance->temp_option == msimCONST_TEMP )
          fprintf( f, "%sat a value of %s (deg K)\n\n", IndentString, Instance->
               temp_data.const_temp );

     if ( Instance->temp_option == msimVAR_TEMP )
          fprintf( f, "%swith an initial value of %s (deg K)\n\n", IndentString,
               Instance->temp_data.initial_temp );

     if ( ( Instance->temp_option == msimPROGR_TEMP ) && ( Instance->
                    temp_prog_data_format == msimANALYTIC_TPROG ) )
     {

          /* write out the coefficients for temp programming                */
          /* if that option  has been selected                              */

          fprintf( f, "\n%sCoefficients for the temperature program\n\n", IndentString );
          fprintf( f,
               "%s   T(t) = Initial Temperature +  (slope * t)\n\n", IndentString );
          fprintf( f, "%s where             t = time in %s\n\n", IndentString,
               msimTimeUnits( Instance->time_units ) );
          fprintf( f, "%s Initial Temperature = %s (deg K)\n", IndentString, Instance->
               temp_data.prog_initial_temp );
          fprintf( f, "%s               slope = %s (deg/%s)\n", IndentString, Instance
               ->temp_data.progr_coeffA, msimTimeUnits( Instance->time_units ) );
          fprintf( f, "%s\n maximum allowable temperature = %s deg K\n", IndentString,
               Instance->temp_data.max_temp );
          fprintf( f, "%s maximum allowable step size   = %s deg\n", IndentString,
               Instance->temp_data.max_step_size );
     }

     if ( ( Instance->temp_option == msimPROGR_TEMP ) && ( Instance->
                    temp_prog_data_format == msimFILE_TPROG ) )
     {
          fprintf( f, "\n%sThe temperature profile defined in file %s is used\n",
               IndentString, Instance->temp_profile_data );
          fprintf( f, "%s maximum allowable step size = %s deg K\n",
               IndentString, Instance->temp_data.max_step_size );
     }

     fprintf( f, "\n\n---------- SIMULATION CONDITIONS ----------\n\n" );

     fprintf( f, "%sTotal number of events    = %s\n", IndentString, Instance->
          optionvalue[msimEVENTS] );
     fprintf( f, "%sTotal number of particles = %s\n", IndentString, Instance->
          optionvalue[msimPARTICLES] );
     fprintf( f, "%sRandom number seed        = %s\n", IndentString, Instance->
          optionvalue[msimSEED] );
     fprintf( f,
          "%sState of simulation is recorded at intervals of %s events\n", IndentString, Instance
          ->optionvalue[msimINTERVAL] );
     fprintf( f, "%sEquilibrium Detect is %s\n\n", IndentString, Instance->
          enable_equil_detect ? "enabled" : "disabled" );

     if ( Instance->enable_equil_detect )
     {
          fprintf( f, "%sEquilibrium test cycle length : %s events\n",
               IndentString, Instance->equil_data.cycle_length );
          fprintf( f, "%sEquil selection frequency     : %s percent\n\n",
               IndentString, Instance->equil_data.min_eff );
     }

     aMainApp.Wait( FALSE );

     /* check for errors during write                                       */

     if ( ferror( f ) )
     {

          msimFileError( Instance->print_target, Owner, ( USHORT ) msimWRITE_ERROR );
          fclose( f );
          return msimWRITE_ERROR;
     }

     /* close file and check for errors                                     */

     if ( EOF == fclose( f ) )
     {
          msimFileError( Instance->print_target, Owner, ( USHORT ) msimWRITE_ERROR );
          return msimWRITE_ERROR;
     }

     /* if we got this far everything went ok                               */

#if 0	 
// #if defined(__MAC__) && defined(__PPC__)

     Sysdepen::SetFileInfo( String( Instance->print_target), String( msimTEXTFILE_TYPE),
                  String( msimDEFAULT_CREATOR_NAME) );

#endif


     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        PrintSpecialKinetics()                            */
/*..........................................................................*/
/*                                                                          */
/* formats strings showing the special kinetics rate law for the given      */
/* reaction struct and writes the strings to file F                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC PrintSpecialKinetics( FILE *F, msimPRXN RxnPtr )
{
     msimNAME_ARRAY reactants;
     msimEXPONENT_ARRAY r_expnt;
     msimNAME_ARRAY products;
     msimEXPONENT_ARRAY p_expnt;
     msimRC rc;
     USHORT numreactants, numproducts;
     USHORT i = 0;
     msimSTRING workstr;
     msimSTRING tmpstr;

     PCHAR kstring = " Rate = k x [ %s ]^%s";
     PCHAR continue_str = " x [ %s ]^%s";

     fprintf( F, "\n%sKinetic rate law for this reaction "
          "is not derived from reaction stoichiometry\n", IndentString );

     if ( msimNO_ERROR != ( rc = msimAnalyzeEqn( RxnPtr->equation, reactants,
                         r_expnt, &numreactants, products, p_expnt, &numproducts ) ) )
     {
          fprintf( F, "\n%s*** ERROR ANALYZING EQUATION - RATE LAW"
               " IS NOT AVAILABLE ***\n", IndentString );

          return rc;
     }

     /* construct the label strings                                         */
     /* first do the forward rxn which is always active                     */

     fprintf( F, "%sFor the forward reaction :\n", IndentString );
     sprintf( workstr, kstring, reactants[0], RxnPtr->fwdexponent[0] );

     i = 1;
     while ( i < RxnPtr->numreactants )
     {
          sprintf( tmpstr, continue_str, reactants[i], RxnPtr->fwdexponent[i] );
          msimStringCat( workstr, tmpstr, sizeof workstr );
          i++;
     }

     fprintf( F, "%s%s\n\n", IndentString, workstr );

     if ( RxnPtr->reversible )
     {
          fprintf( F, "%sFor the reverse reaction :\n", IndentString );
          sprintf( workstr, kstring, products[0], RxnPtr->revexponent[0] );

          i = 1;
          while ( i < RxnPtr->numproducts )
          {
               sprintf( tmpstr, continue_str, products[i], RxnPtr->revexponent
                    [i] );
               msimStringCat( workstr, tmpstr, sizeof workstr );
               i++;
          }

          fprintf( F, "%s%s\n", IndentString, workstr );
     }

     return msimNO_ERROR;
}


void WriteNotebookTextToFile( FILE PTR pFile,  PCHAR pTextBuffer )
{
     // create a copy of the text in the buffer to modify

     PCHAR temp_ptr;

     size_t size = strlen( pTextBuffer );

     temp_ptr = new CHAR[ size ];

     if ( temp_ptr )
     {
          strcpy( temp_ptr, pTextBuffer );

          msimSaveEditorTextToFile( temp_ptr, pFile );

          delete [] temp_ptr;
     }
}


