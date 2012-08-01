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
   |  msimanal.cxx   :   a module for MSIM                                  |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this file  contains functions for analysing the    |
   |                     input data for the reaction scheme                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   August 19 1993                                     |
   |                                                                        |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define  SEPARATOR                               "=>"
#define  SPECIES_SEPARATOR                       "+"
#define  RESERVED_CHARS                          "<=>+"

/* local function  declarations                                             */

static msimRC SetSpeciesData( PCHAR half_eqn, msimCOMPONENT comp_list[], USHORT
                   *numspecies, msimPINSTANCE Instance );

static void DeleteSpecies( msimPSPECIES Current, msimPINSTANCE Instance );

static msimRC ExtractSpeciesData( PCHAR half_eqn, msimNAME_ARRAY names,
                   msimEXPONENT_ARRAY exponent, USHORT
                   *numspecies );

static msimRC ValidateSpeciesName( const PCHAR Name );


/*--------------------------------------------------------------------------*/
/*                       ExtractSpeciesData( )                              */
/*..........................................................................*/
/*                                                                          */
/* Parses a half-equation to derive the names, the exponents and the        */
/* count of reactants or products in the half-eqn. Significant checking     */
/* of equation syntax is implemented. An error results in an error code     */
/* return indicating the type of error                                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC ExtractSpeciesData( PCHAR half_eqn, msimNAME_ARRAY names,
            msimEXPONENT_ARRAY exponent, USHORT *numspecies )

{
     msimSTRING next_species_set;
     USHORT i = 0;
     USHORT j;
     msimSTRING tmp_str, work_str, rest;
     msimRC rc;

     msimStringCopy( work_str, half_eqn, sizeof work_str );/* make a copy of string for to work
                                                              with            */
     do
     {

          /* if we find the '+' character in the work string then           */
          /* save the remaining text 'rest' in work_str, otherwise          */
          /* copy the work_str to 'next_species_set to be analyzed          */
          /* and set the work_str to null string                            */

          if ( msimParse( work_str, SPECIES_SEPARATOR, next_species_set, rest ) )
          {
               msimStringCopy( work_str, rest, sizeof work_str );

               /* check for a non-zero length in work_str                   */
               /* if zero length it's an error                              */

               if ( ! strlen( work_str ) )
               {
                    *numspecies = 0;
                    return msimEQN_SYNTAX;
               }
          }
          else
          {
               msimStringCopy( next_species_set, work_str, sizeof next_species_set );
               work_str[0] = '\0';
          }

          /* first check for too many species defined                       */

          if ( i >= msimMAX_NUMBER_OF_COMPONENTS )
          {
               *numspecies = 0;
               return msimTOO_MANY_COMPS;
          }

          switch ( msimWords( next_species_set ) )/* check no. of words in
                                                     next_species_set         */
          {
          case 2 :

               /* if exponent[i] is set to NULL_STR then it was not         */
               /* set as a  special kinetics rate law so fill in defaults   */

               if ( msimWords( exponent[i] ) == 0 )
               {
                    msimStringCopy( tmp_str, msimSubWord( next_species_set, ( USHORT ) 1 ),
                         sizeof tmp_str );

                    /* check for valid format and length                    */

                    if ( ! msimValidPositiveShortInteger( tmp_str ) || ( strlen( tmp_str )
                                   > msimLENGTH_OF_INTEGER_FIELD ) )
                    {
                         *numspecies = 0;
                         return msimSTOICH_ERROR;
                    }
                    else

                         msimStringCopy( exponent[i], tmp_str, sizeof exponent[0] );

                    /* append ".00" to integer to make it a real number     */

                    msimStringCat( exponent[i], msimZERO_TAIL, sizeof exponent[0] );
               }

               /* get the species name into tmpstr and check validity below */

               msimStringCopy( tmp_str, msimSubWord( next_species_set, ( USHORT ) 2 ),
                    sizeof tmp_str );
               break;
          case 1 :

               if ( msimWords( exponent[i] ) == 0 )
                    msimStringCopy( exponent[i], msimREAL_ONE_STR, sizeof exponent[0] );

               msimStringCopy( tmp_str, msimSubWord( next_species_set, ( USHORT ) 1 ),
                    sizeof tmp_str );

               break;
          default :                    /* only here if too many words         */
               *numspecies = 0;
               return msimEQN_SYNTAX;
          }                            /* endswitch                           */

          if ( msimNO_ERROR != ( rc = ValidateSpeciesName( tmp_str ) ) )
          {
               *numspecies = 0;
               return rc;
          }
          else

               msimStringCopy( names[i], tmp_str, sizeof names[0] );

          /* now check to see if the species name is already in the list    */

          for ( j = 0; ( j < i ) && ( j <= msimMAX_NUMBER_OF_COMPONENTS ); j++ )
          {
               if ( msimStringsMatch( names[j], names[i], ( USHORT )
                              msimCASE_INSENSITIVE ) )
               {
                    *numspecies = 0;
                    return msimAPPEARS_TWICE;
               }
          }

          i++;

     }
     while ( work_str[0] != '\0' );
     *numspecies = i;

     /* now fill the remaining elements of the arrays of strings with null strings*/

     while ( i < msimMAX_NUMBER_OF_COMPONENTS )
     {
          msimStringCopy( names[i], msimNULL_STR, sizeof names[0] );
          msimStringCopy( exponent[i], msimNULL_STR, sizeof exponent[0] );
          i++;
     }                                 /* endwhile                            */
     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                       msimAnalyzeEquation ()                             */
/*..........................................................................*/
/*                                                                          */
/* Function for parsing and analyzing a reaction text string. It produces   */
/* filled arrays for the names and stoichiometries                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimAnalyzeEqn( msimEQUATION_STRING Eqn, msimNAME_ARRAY Reactants,
            msimEXPONENT_ARRAY R_Expnt, USHORT *numreactants,
            msimNAME_ARRAY Products, msimEXPONENT_ARRAY P_Expnt,
            USHORT *numproducts )
{
     msimEQUATION_STRING leftside, rightside;
     msimRC rc;

     /* first find the middle of the equation and split there               */

     if ( ! msimWords( Eqn ) )
          return msimNO_EQUATION;

     if ( ! msimParse( Eqn, SEPARATOR, leftside, rightside ) )
     {
          return msimEQN_SYNTAX;
     }

     /* test leftside and rightside for appropriate whitespace, etc         */
     /* first remove '<' char from leftside if present                      */

     if ( leftside[strlen( leftside ) - 1] == '<' )
          leftside[strlen( leftside ) - 1] = '\0';

     /* now check that the last character in leftside is a space            */

     if ( ! isspace( leftside[strlen( leftside ) - 1] ) )
          return msimEQN_SYNTAX;

     /* check that the first character in rightside is a space              */

     if ( ! isspace( rightside[0] ) )
          return msimEQN_SYNTAX;

     if ( msimNO_ERROR != ( rc = ExtractSpeciesData( leftside, Reactants, R_Expnt
                         , numreactants ) ) )
     {
          return rc;
     }

     if ( msimNO_ERROR != ( rc = ExtractSpeciesData( rightside, Products, P_Expnt
                         , numproducts ) ) )
     {
          return rc;
     }
     return msimNO_ERROR;
}


/*--------------------------------------------------------------------------*/
/*                       msimAnalysisError( )                               */
/*..........................................................................*/
/*                                                                          */
/* returns ptr to a character string describing the error symbolized        */
/* by the msimRC error code passed as the parameter                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

PCHAR msimAnalysisError( msimRC rc )
{
     static PCHAR AnalysisMsg[9] =
             {
                  "\"No error detected\"",
                  "\"Incorrect equation syntax\"",
                  "\"Incorrect species name\"",
                  "\"Incorrect stoichiometric coefficient\"",
                  "\"Too many reactants or products in eqn\"",
                  "\"Species appears twice on one side of eqn\"",
                  "\"No equation entered\"",
                  "\"A species name exceeds the allowed length\"",
                  "\"<< Unknown Error >>\""
             };


     return ( ( rc <= msimOTHER ) ? AnalysisMsg[rc] : AnalysisMsg[msimOTHER] );
}

/*--------------------------------------------------------------------------*/
/*                       msimFindSpeciesIndex ()                            */
/*..........................................................................*/
/*                                                                          */
/* Finds a species by name in an existing list of species, returns the      */
/* index of the species as they are arranged in alphabetical order. The     */
/* first species in the list has index 1. Each unique species ends up with  */
/* a unique index. The comparison/search is insensitive to case. If the     */
/* list does not exist (i.e. the pointer to the list == NULL) then a list   */
/* is created                                                               */
/*--------------------------------------------------------------------------*/

USHORT msimFindSpeciesIndex( PCHAR SearchString, msimPINSTANCE Instance )
{
     msimNAME_STRING upcase_liststr, upcase_searchstr;
     msimPSPECIES previous, current, new_record;
     int compare_result;
     USHORT i;

     msimUpCaseString( SearchString, upcase_searchstr );

     if ( current = Instance->ptr_to_species_list )/* if a list has already
                                                      been started            */
     {
          while ( current != NULL )
          {
               compare_result = strcmp( msimUpCaseString( current->name,
                         upcase_liststr ), upcase_searchstr );
               if ( compare_result == 0 )/* if theres a match return          */
               {
                    if ( ! current->index )/* if index == 0                   */
                         current->index = ++ ( Instance->speciescount );
                    return current->index;
               }

               if ( compare_result > 0 )
                    break;             /* if we have passed the insertion     */

               /* point then stop the loop                                  */

               previous = current;     /* otherwise get the next element in
                                          list                                */
               current = current->next;
          }

          /* if we have reached  this point of the function either we need  */
          /* to add the name SearchString to the list as it is not there    */
          /* yet.                                                           */
          /* Either we are now                                              */
          /* at the end of the list or we are just past the point on the    */
          /* linked list of species data where we should insert the new element*/
          /* First create a new record and initialize it                    */

          new_record = new msimSPECIES;
          if ( ! new_record )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, msimWID_NONE );
               return 0;
          }

          *new_record = msimC_SPECIES;

          new_record->index = ++ ( Instance->speciescount );

          /* now deal with setting the pointers to prev, next               */
          /* three possibilities: we need to insert at the start of the list,*/
          /* somewhere in the middle of the list, or at the end of the list */
          /* do we need to insert at the very beginning ?                   */
/* remember that current points to record following the point where we  need to insert*/

          if ( current == Instance->ptr_to_species_list )
               Instance->ptr_to_species_list = new_record;
          else
               previous->next = new_record;

          new_record->next = current;

          if ( current )               /* if not at end of list               */
          {
               new_record->prev = current->prev;
               current->prev = new_record;
          }
          else
               new_record->prev = previous;

     }
     else
     {

          /* start a new list                                               */

          new_record = new msimSPECIES;
          if ( ! new_record )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, msimWID_NONE );
               return 0;
          }

          *new_record = msimC_SPECIES;

          new_record->next = new_record->prev = NULL;
          Instance->speciescount = new_record->index = 1;
          Instance->ptr_to_species_list = new_record;
     }

     msimStringCopy( new_record->name, SearchString, sizeof new_record->name );
     msimStringCopy( new_record->initialconc, msimC_SPECIES.initialconc, sizeof new_record->initialconc );

     for ( i = 0; i < msimNUMBER_OF_THERM_COEFFS; i++ )
          msimStringCopy( new_record->thermcoeff[i], msimC_SPECIES.thermcoeff[i],
               sizeof new_record->thermcoeff[0] );

     return new_record->index;
}

/*--------------------------------------------------------------------------*/
/*                        msimAnalyzeRxnScheme ()                           */
/*..........................................................................*/
/*                                                                          */
/* the main reaction scheme analysis function, called whenever editing of   */
/* the reaction scheme is ended by closing the Rxn data edit window         */
/*--------------------------------------------------------------------------*/

msimRC msimAnalyzeRxnScheme( msimPINSTANCE Instance, msimPRXN * BadRxnData, USHORT
            *LineNumber, msimWID Owner )
{
     msimPRXN current_rxn;
     USHORT current_line;
     USHORT num_steps;
     msimEQUATION_STRING leftside, rightside;
     msimRC rc;
     msimPSPECIES tmp_ptr;

     /* make sure there is data to work on...                               */

     if ( ! Instance->ptr_to_rxnlist )
     {
          WarningBox( Owner, ResId( msimNO_RXN_LOADED_MSG ) ) . Execute( );
          return msimOTHER;
     }

     aMainApp.Wait( TRUE );

     /* set all species indices in the existing linked list staring at      */
     /* Instance->ptr_to_species_list. The reason for this is that some of*/
     /* the species in the existing list may no longer exist. We will remove*/
     /* those whose value index == 0 after the analysis                     */
     /* In this way we preserve any existing data for those species which   */
     /* are still defined but eliminate those which are no longer used      */

     for ( tmp_ptr = Instance->ptr_to_species_list; tmp_ptr != NULL; tmp_ptr =
               tmp_ptr->next )
          tmp_ptr->index = 0;

     /* set species count to zero ... used in assigning species indices     */

     Instance->speciescount = 0;

     /* now walk through list of reactions checking for errors and extracting*/
     /* the necessary data                                                  */

     current_rxn = Instance->ptr_to_rxnlist;
     num_steps = current_line = 0;

     while ( current_rxn != NULL )
     {

          current_line++;

          /* check to make sure it is not a blank string                    */

          if ( msimWords( current_rxn->equation ) )
          {
               num_steps++;

               if ( ! msimParse( current_rxn->equation, SEPARATOR, leftside,
                              rightside ) )
               {
                    *BadRxnData = current_rxn;
                    *LineNumber = current_line;
                    aMainApp.Wait( FALSE );
                    return msimEQN_SYNTAX;
               }

               /* test leftside and rightside for appropriate whitespace, etc*/
               /* first remove '<' char from leftside if present            */

               if ( leftside[strlen( leftside ) - 1] == '<' )
                    leftside[strlen( leftside ) - 1] = '\0';

               /* now check that the last character in leftside is a space  */
               /* and check that the first character in rightside is a space*/

               if ( ! isspace( leftside[strlen( leftside ) - 1] ) || ! isspace
                         ( rightside[0] ) )
               {
                    *BadRxnData = current_rxn;
                    *LineNumber = current_line;
                    aMainApp.Wait( FALSE );
                    return msimEQN_SYNTAX;
               }

               if ( rc = SetSpeciesData( leftside, current_rxn->reactantlist,
                              &current_rxn->numreactants, Instance ) )
               {
                    *BadRxnData = current_rxn;
                    *LineNumber = current_line;
                    aMainApp.Wait( FALSE );
                    return rc;
               }

               if ( rc = SetSpeciesData( rightside, current_rxn->productlist,
                              &current_rxn->numproducts, Instance ) )
               {
                    *BadRxnData = current_rxn;
                    *LineNumber = current_line;
                    aMainApp.Wait( FALSE );
                    return rc;
               }
          }

          current_rxn->reversible = msimSeeIfReversible( current_rxn->equation )
          ;
          current_rxn = current_rxn->next;

     }                                 /* endwhile                            */

     /* find and delete those species which still have an index value of    */
     /* zero as they were not detected in this analysis and should be removed*/

     for ( tmp_ptr = Instance->ptr_to_species_list; tmp_ptr != NULL; tmp_ptr =
               tmp_ptr->next )
          if ( ! tmp_ptr->index )
               DeleteSpecies( tmp_ptr, Instance );

     Instance->numsteps = num_steps;

     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     aMainApp.Wait( FALSE );

     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        SetSpeciesData()                                  */
/*..........................................................................*/
/*                                                                          */
/*  goes through one side of an equation and extracts the species data      */
/*  (ie., the name, the number of species, the stoichiometry of each        */
/*  species. etc) and stores it in the appropriate memory locations         */
/*                                                                          */
/*  There is significant checking for faulty input and syntax errors here   */
/*   returns msimNO_ERROR if everything's ok, an error code otherwise       */
/*--------------------------------------------------------------------------*/

msimRC SetSpeciesData( PCHAR half_eqn, msimCOMPONENT comp_list[], USHORT
            *numspecies, msimPINSTANCE Instance )

{
     USHORT i = 0;
     USHORT j;
     msimSTRING work_str, rest;
     msimSTRING species_name;
     msimSTRING next_species_set;
     msimRC rc;

     msimStringCopy( work_str, half_eqn, sizeof work_str );/* make a copy of string for to work
                                                              with            */
     do
     {

          /* if we find the '+' character in the work string then           */
          /* save the remaining text 'rest' in work_str, otherwise          */
          /* copy the work_str to 'next_species_set to be analyzed          */
          /* and set the work_str to null string                            */

          if ( msimParse( work_str, SPECIES_SEPARATOR, next_species_set, rest ) )
          {
               msimStringCopy( work_str, rest, sizeof work_str );

               /* check for a non-zero length in work_str                   */
               /* if zero length it's an error                              */

               if ( ! strlen( work_str ) )
               {
                    *numspecies = 0;
                    return msimEQN_SYNTAX;
               }
          }
          else
          {
               msimStringCopy( next_species_set, work_str, sizeof next_species_set );
               work_str[0] = '\0';
          }
          switch ( msimWords( next_species_set ) )/* check no. of words in
                                                     next_species_set         */
          {
          case 2 :
               if ( i >= msimMAX_NUMBER_OF_COMPONENTS )
               {
                    *numspecies = 0;
                    return msimTOO_MANY_COMPS;
               }

               msimStringCopy( comp_list[i].stoichcoeff, msimSubWord( next_species_set,
                         ( USHORT ) 1 ), sizeof comp_list[0].stoichcoeff );
               if ( ! msimValidPositiveShortInteger( comp_list[i].stoichcoeff ) )
                    return msimSTOICH_ERROR;

               msimStringCopy( species_name, msimSubWord( next_species_set, ( USHORT ) 2 ),
                    sizeof species_name );
               break;
          case 1 :
               if ( i >= msimMAX_NUMBER_OF_COMPONENTS )
               {
                    *numspecies = 0;
                    return msimTOO_MANY_COMPS;
               }

               msimStringCopy( comp_list[i].stoichcoeff, "1", sizeof comp_list[0].stoichcoeff );
               msimStringCopy( species_name, msimSubWord( next_species_set, ( USHORT ) 1 ),
                    sizeof species_name );
               break;
          default :                    /* only here if too many words         */
               return msimEQN_SYNTAX;
          }                            /* endswitch                           */

          /* now check that species name is valid                           */

          if ( msimNO_ERROR != ( rc = ValidateSpeciesName( species_name ) ) )
               return rc;

          /* now get the "species index" which is a unique numeric identifier*/
          /* for this particular species name                               */

          comp_list[i].speciesindex = msimFindSpeciesIndex( species_name,
               Instance );

          /* now check to see if the species name is already in the list    */

          for ( j = 0; ( j < i ) && ( j <= msimMAX_NUMBER_OF_COMPONENTS ); j++ )
          {
               if ( comp_list[i].speciesindex == comp_list[j].speciesindex )
                    return msimAPPEARS_TWICE;
          }
          i++;

     }
     while ( work_str[0] != '\0' );
     if ( i < 1 )
          return msimEQN_SYNTAX;

     *numspecies = i;
     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        ValidateSpeciesName( )                            */
/*..........................................................................*/
/*                                                                          */
/* checks a string to be sure it meets all the criteria for a valid species */
/* name : these are                                                         */
/*                                                                          */
/*   1. first character is alphabetical                                     */
/*                                                                          */
/*   2. the name does not contain characters used a sumbols in analyzing    */
/*      the equation : "+ < = >" and whitespace.                            */
/*                                                                          */
/*   3. the maximum allowed length is msimLENGTH_OF_NAME                    */
/*                                                                          */
/*  function returns msimNO_ERROR if string is OK, a non-zero error code    */
/*  otherwise                                                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC ValidateSpeciesName( const PCHAR Name )
{
     USHORT i = 1;

     if ( ! isgraph( Name[0] ) || isdigit( Name[0] ) )
          return msimSPECIES_ERROR;

     /* now scan through string looking for error                           */

     while ( Name[i] )
     {
          if ( i >= msimLENGTH_OF_NAME )
               return msimSPECIES_NAME_TOO_LONG;

          if ( ! isgraph( Name[i] ) || strchr( RESERVED_CHARS, Name[i] ) )
               return msimSPECIES_ERROR;
          i++;
     }

     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                        DeleteSpecies()                                   */
/*..........................................................................*/
/*                                                                          */
/* deletes a msimSPECIES struct from a linked list                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void DeleteSpecies( msimPSPECIES Current, msimPINSTANCE Instance )
{
     if ( Current->prev )              /* if we are not at the beginning of
                                          the list                            */
          Current->prev->next = Current->next;
     else
     {                                 /* then we ARE at the beginning of
                                          the list                            */
          if ( Current->next )         /* if there is a following entryin
                                          the list                            */
               Current->next->prev = NULL;/* make it point to NULL            */
          Instance->ptr_to_species_list = Current->next;
     }
     if ( Current->next )              /* if we are not at the end of the
                                          list then make sure                 */
          Current->next->prev = Current->prev;/* following entry points to
                                                 prev                         */

     /* pointers are all adjusted - deallocate the memory used by Current   */

     delete Current;
}

/*--------------------------------------------------------------------------*/
/*                  msimGetRxnPtrFromLineNumber()                           */
/*..........................................................................*/
/*                                                                          */
/* Function for determining the pointer to a reaction struct from the       */
/*  line number in the main reaction listbox. The paramter RxnPtr is        */
/* the startof the linked list of reaction structs                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPRXN msimGetRxnPtrFromLineNumber( USHORT Selection, msimPRXN RxnPtr )
{
     USHORT i = 0;

     while ( i++ < Selection )
          RxnPtr = RxnPtr->next;

     return RxnPtr;
}



/*--------------------------------------------------------------------------*/
/*                       msimValidTPVCombo( )                               */
/*..........................................................................*/
/*                                                                          */
/* returns zero if a selected combination of temperature. pressure and      */
/* voluem options is not physically meaningful, otherwsie returns the       */
/* sum of all the options. Each valid sum uniquely specifies a specific     */
/* combination of T P V options.                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

USHORT msimValidTPVCombo( msimBOOL VariablePress, USHORT TempOption, USHORT VolOption )

{
     USHORT sum;

     sum = VolOption + TempOption + ( USHORT ) VariablePress;
     switch ( sum )
     {
     case ( msimVAR_VOL + msimVAR_TEMP + FALSE ) :
     case ( msimCONST_VOL + msimVAR_TEMP + TRUE ) :
     case ( msimDONT_CARE_VOL + msimVAR_TEMP + FALSE ) :
     case ( msimVAR_VOL + msimPROGR_TEMP + FALSE ) :
     case ( msimCONST_VOL + msimPROGR_TEMP + TRUE ) :
     case ( msimDONT_CARE_VOL + msimPROGR_TEMP + FALSE ) :
     case ( msimDONT_CARE_VOL + msimCONST_TEMP + FALSE ) :
     case ( msimCONST_VOL + msimCONST_TEMP + TRUE ) :
     case ( msimVAR_VOL + msimCONST_TEMP + FALSE ) :
          return sum;

     default :
          return ( USHORT ) 0;
     }

}

