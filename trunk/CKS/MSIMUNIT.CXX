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
/*  FILE NAME : msimunit.cxx                                                */
/*                                                                          */
/*  This module  contains the functions for setting strings to correctly    */
/*  reflect the dimensional units chosen by the user. ALso contains         */
/*  functions to retrieve conversion factors for converting between units   */
/*                                                                          */
/*                                                                          */
/*  Written using the "Starview" libraries to provide common code for       */
/*  multiple platforms                                                      */
/*                                                                          */
/*  File created August 16, 1993                                            */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include <stdlib.h>

/* #defines for conversion factors                                          */

#define  MOLECULES_PER_MOLE                      6.022045E23
#define  SEC_PER_MIN                             60.0
#define  SEC_PER_HOUR                            3600.0
#define  TORR_PER_ATM                            760.0
#define  PASCALS_PER_ATM                         1.013E5
#define  CC_PER_LITER                            1000.0
#define  CAL_PER_JOULE                           0.2389
#define  MOLE_PER_MOLECULE                       ( 1.0 / MOLECULES_PER_MOLE)
#define  MIN_PER_SEC                             ( 1.0 / SEC_PER_MIN )
#define  HOUR_PER_SEC                            ( 1.0 / SEC_PER_HOUR )
#define  ATM_PER_TORR                            ( 1.0 / TORR_PER_ATM )
#define  ATM_PER_PASCAL                          ( 1.0 / PASCALS_PER_ATM )
#define  LITER_PER_CC                            ( 1.0 / CC_PER_LITER )
#define  JOULE_PER_CAL                           ( 1.0 / CAL_PER_JOULE )

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorVolumeFromLiters( )                   */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts time from sec units to other units   */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorVolumeFromLiters( USHORT VolumeUnits )

{
     switch ( VolumeUnits )
     {
     case msimVOL_LITERS :
     case msimNO_CONVERSION :
          return 1.00;

     case msimVOL_CC :
     case msimVOL_CM :
          return CC_PER_LITER;

     default :
          return 1.00;

     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorTimeFromSec( )                        */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts time from sec units to other units   */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorTimeFromSec( USHORT TimeUnits )
{
     switch ( TimeUnits )
     {
     case msimTIME_SECONDS :
     case msimNO_CONVERSION :
          return 1.00;

     case msimTIME_MINUTES :
          return MIN_PER_SEC;
     case msimTIME_HOURS :
          return HOUR_PER_SEC;
     default :
          return 1.00;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorTimeToSec( )                          */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts tie to sec units from other units    */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorTimeToSec( USHORT TimeUnits )
{
     switch ( TimeUnits )
     {

     case msimTIME_SECONDS :
     case msimNO_CONVERSION :

          return 1.00;

     case msimTIME_MINUTES :
          return SEC_PER_MIN;

     case msimTIME_HOURS :
          return SEC_PER_HOUR;

     default :
          return 1.00;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorEnergyToCal( )                        */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts energy to calories from other units  */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorEnergyToCal( USHORT EnergyUnits )
{
     switch ( EnergyUnits )
     {
     case msimENERGY_CALS :
     case msimNO_CONVERSION :
          return 1.00;

     case msimENERGY_JOULES :
          return CAL_PER_JOULE;
     default :
          return 1.00;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorPressFromAtm( )                       */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts pressure to other units from         */
/* atm when multiplied by it                                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorPressFromAtm( USHORT PressUnits )
{
     switch ( PressUnits )
     {
     case msimPRESS_ATM :
     case msimNO_CONVERSION :
          return 1.00;

     case msimPRESS_TORR :
          return TORR_PER_ATM;

     case msimPRESS_PASCALS :
          return PASCALS_PER_ATM;
     default :
          return 1.00;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorPressToAtm( )                         */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts concentration to mole/liter units    */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorPressToAtm( USHORT PressUnits )
{
     switch ( PressUnits )
     {
     case msimPRESS_ATM :
     case msimNO_CONVERSION :
          return 1.00;

     case msimPRESS_TORR :

          return ATM_PER_TORR;
     case msimPRESS_PASCALS :
          return ATM_PER_PASCAL;
     default :
          return 1.00;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimConvFactorConcToMolar()                         */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts concentration to mole/liter units    */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorConcToMolar( USHORT ConcUnits )
{
     switch ( ConcUnits )
     {
     case msimCONC_MOLES_LITER :
     case msimNO_CONVERSION :
          return 1.00;

     case msimCONC_MOLES_CM3 :
     case msimCONC_MOLES_CM2 :
     case msimCONC_MOLES_CM :
          return CC_PER_LITER;

     case msimCONC_MOL_LITER :
          return MOLE_PER_MOLECULE;

     case msimCONC_MOL_CM2 :
     case msimCONC_MOL_CM :
     case msimCONC_MOL_CM3 :
          return ( MOLE_PER_MOLECULE * CC_PER_LITER );

     default :
          return 1.00;
     }
}


/*--------------------------------------------------------------------------*/
/*                      msimConvFactorAmtToMoles()                          */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts amount to moles units                */
/* when multiplied by it. It differs from the corresponding ConcToMolar fcn */
/* in that this routine does no conversion of volume units, only amount     */
/* units                                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorAmtToMoles( USHORT ConcUnits )
{
     switch ( ConcUnits )
     {
     case msimCONC_MOLES_LITER :
     case msimCONC_MOLES_CM3 :
     case msimCONC_MOLES_CM2 :
     case msimCONC_MOLES_CM :
          return 1.00;

     case msimCONC_MOL_LITER :
     case msimCONC_MOL_CM2 :
     case msimCONC_MOL_CM :
     case msimCONC_MOL_CM3 :
          return MOLE_PER_MOLECULE;

     case msimNO_CONVERSION :
     default :
          return 1.00;
     }
}


/*--------------------------------------------------------------------------*/
/*                      msimConvFactorConcFromMolar()                       */
/*..........................................................................*/
/*                                                                          */
/* returns a real number that converts concentration from mole/liter units  */
/* when multiplied by it                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimFLOAT msimConvFactorConcFromMolar( USHORT ConcUnits )
{
     switch ( ConcUnits )
     {
     case msimCONC_MOLES_LITER :
     case msimNO_CONVERSION :
          return 1.00;

     case msimCONC_MOLES_CM3 :
     case msimCONC_MOLES_CM2 :
     case msimCONC_MOLES_CM :
          return LITER_PER_CC;

     case msimCONC_MOL_LITER :
          return MOLECULES_PER_MOLE;

     case msimCONC_MOL_CM2 :
     case msimCONC_MOL_CM :
     case msimCONC_MOL_CM3 :
          return ( MOLECULES_PER_MOLE * LITER_PER_CC );

     default :
          return 1.00;
     }
}

PCHAR msimConcUnits( USHORT UnitType, USHORT VolumeOption )
{
     static PCHAR units_string[] =
             {
                  "mole/l", "mole/cm3", "mole/cm2", "mo1e/cm^n", "mol/l", "mol/cm3",
                  "mol/cm2", "mo1/cm^n", "mole", "mol", "?"
             }
     ;
     if ( VolumeOption == msimVAR_VOL )
     {
          switch ( UnitType )
          {
          case msimCONC_MOLES_LITER :
          case msimCONC_MOLES_CM3 :
          case msimCONC_MOLES_CM2 :
          case msimCONC_MOLES_CM :
               return units_string[8];
          case msimCONC_MOL_LITER :
          case msimCONC_MOL_CM3 :
          case msimCONC_MOL_CM2 :
          case msimCONC_MOL_CM :
               return units_string[9];
          default :
               return units_string[10];
          }
     }
     else
     {
          switch ( UnitType )
          {
          case msimCONC_MOLES_LITER :
               return units_string[0];
          case msimCONC_MOLES_CM3 :
               return units_string[1];
          case msimCONC_MOLES_CM2 :
               return units_string[2];
          case msimCONC_MOLES_CM :
               return units_string[3];
          case msimCONC_MOL_LITER :
               return units_string[4];
          case msimCONC_MOL_CM3 :
               return units_string[5];
          case msimCONC_MOL_CM2 :
               return units_string[6];
          case msimCONC_MOL_CM :
               return units_string[7];
          default :
               return units_string[10];
          }
     }
}

PCHAR msimTimeUnits( USHORT UnitType )
{
     static PCHAR units_string[] =
             {
                  "sec", "min", "hr", "?"
             }
     ;
     switch ( UnitType )
     {
     case msimTIME_SECONDS :
          return units_string[0];

     case msimTIME_MINUTES :
          return units_string[1];
     case msimTIME_HOURS :
          return units_string[2];
     default :
          return units_string[3];
     }
}

PCHAR msimPressureUnits( USHORT UnitType )
{
     static PCHAR units_string[] =
             {
                  "atm", "torr", "Pa", "?"
             }
     ;
     switch ( UnitType )
     {
     case msimPRESS_ATM :
          return units_string[0];
     case msimPRESS_TORR :
          return units_string[1];
     case msimPRESS_PASCALS :
          return units_string[2];
     default :
          return units_string[3];
     }
}

PCHAR msimVolumeUnits( USHORT UnitType )
{
     static PCHAR units_string[] =
             {
                  "l", "cm3", "cm2", "?"
             }
     ;
     switch ( UnitType )
     {
     case msimVOL_LITERS :
          return units_string[0];
     case msimVOL_CC :
          return units_string[1];
     case msimVOL_CM :
          return units_string[2];
     default :
          return units_string[3];
     }
}

PCHAR msimEnergyUnits( USHORT UnitType )
{
     static PCHAR units_string[] =
             {
                  "cal", "J", "?"
             }
     ;
     switch ( UnitType )
     {
     case msimENERGY_CALS :
          return units_string[0];
     case msimENERGY_JOULES :
          return units_string[1];
     default :
          return units_string[2];
     }

}

PCHAR msimMolarDensityInMolarUnits( PCHAR Str, USHORT CurrentUnits )
{
     static msimSTRING retval;
     msimFLOAT tempreal;

     tempreal = atof( Str ) *msimConvFactorConcToMolar( CurrentUnits );
     sprintf( retval, "%E", tempreal );
     return retval;
}

PCHAR msimEaUnits( USHORT EnergyUnits )
{
     static msimSTRING retval;

     sprintf( retval, "%s/%s", msimEnergyUnits( EnergyUnits ), msimConcUnits
          (( USHORT ) msimCONC_MOLES_LITER, ( USHORT ) msimVAR_VOL ) );

     /* always given in energy/mole                                         */

     return retval;
}

#define  MIN_DIFF                                0.005
PCHAR msimAFactorUnits( msimFLOAT NumSpecies, USHORT ConcUnits, USHORT
           VolumeUnits, USHORT TimeUnits )
{
     static msimSTRING retval;

     if ( NumSpecies != 0.0 )
     {
          if ( NumSpecies - 1.0 <= MIN_DIFF )/* if ~= 1.00                    */
          {

               /* here we leave off the concentration stuff                 */

               sprintf( retval, "1/%s", msimTimeUnits( TimeUnits ) );
               return retval;
          }

          if ( NumSpecies - 2.0 <= MIN_DIFF )/* if ~= 2.00                    */
          {
               sprintf( retval, "%s/%s-%s", msimVolumeUnits( VolumeUnits ),
                    msimConcUnits( ConcUnits, ( USHORT ) msimVAR_VOL ), msimTimeUnits
                    ( TimeUnits ) );
               return retval;
          }
          else
          {
               sprintf( retval, "%s^%4.2f /%s^%4.2f -%s", msimVolumeUnits
                    ( VolumeUnits ), ( NumSpecies - 1.0 ), msimConcUnits( ConcUnits,
                         ( USHORT ) msimVAR_VOL ), ( NumSpecies - 1.0 ), msimTimeUnits
                    ( TimeUnits ) );
               return retval;
          }

     }
     else
     {

          /* NumSpecies == 0.0 - use indeterminate exponent in string       */

          sprintf( retval, "%s/%s-%s", msimVolumeUnits( VolumeUnits ),
               msimConcUnits( ConcUnits, ( USHORT ) msimVAR_VOL ), msimTimeUnits
               ( TimeUnits ) );
          return retval;
     }
}

/*--------------------------------------------------------------------------*/
/*                      msimCalcAFactorConvExponent( )                      */
/*..........................................................................*/
/*                                                                          */
/*   calc the exponent for the conversion of the AFactor to appropriate     */
/*   units. The exponent = the sum of all the exponents in the rate         */
/*   law.        These are found in the fwdexponent and revexponent         */
/*   arrays is the not_stoich flag is set, otherwise they are equal to      */
/*   the sum of the stoichimetric coefficients in the reaction              */
/*                                                                          */
/*   returns the calculated fwd and rev expoenents at the addresses         */
/*   pointed to be FwdExponent and RevExponent                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimCalcAFactorConvExponent( msimPRXN Reaction, msimPFLOAT FwdExponent,
          msimPFLOAT RevExponent )
{
     msimFLOAT exponent = 0.0;
     USHORT i;

     /* for the forward reaction                                            */

     if ( Reaction->not_stoich )

          /* sum up the exponents                                           */

          for ( i = 0; i < Reaction->numreactants; exponent += atof
                    ( Reaction->fwdexponent[i++] ) )
               ;
     else

          /* otherwise sum up the stoichiometric coefficients               */

          for ( i = 0; i < Reaction->numreactants; exponent += atof
                    ( Reaction->reactantlist[i++].stoichcoeff ) )
               ;
     *FwdExponent = exponent;

     /* now for the reverse rxn - if rxn is not reversible then the value of*/
     /* *RevExponent ends up as 0.0                                         */

     if ( Reaction->reversible )
     {
          exponent = 0.0;
          if ( Reaction->not_stoich )

               /* sum up the exponents                                      */

               for ( i = 0; i < Reaction->numproducts; exponent += atof
                         ( Reaction->revexponent[i++] ) )
                    ;
          else

               /* otherwise sum up the stoichiometric coefficients          */

               for ( i = 0; i < Reaction->numproducts; exponent += atof
                         ( Reaction->productlist[i++].stoichcoeff ) )
                    ;
          *RevExponent = exponent;
     }
     else
          *RevExponent = 0.0;
     return;
}
