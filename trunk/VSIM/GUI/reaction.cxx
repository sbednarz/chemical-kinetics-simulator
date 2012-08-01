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
//  REACTION.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the Reaction object, which contains
//  data describing a reaction step in a reaction scheme      
//
//  CREATED : 1.24.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "reaction.hxx"
#include "vsimtype.hxx"
#include "idtypes.hxx"
#include "mainapp.hxx"

#endif


#include <ctype.h>





const char   SPACE_CHAR = ' ';
const char   REVERSIBLE_CHAR = '<';
const char   SPECIES_SEPARATOR = '+';
const String SEPARATOR( "=>" );
const String RESERVED_CHARS( "<=>+"  );
const String NULL_STRING( "" );
const String ZERO_SUFFIX( ".00" );
const String REVERSIBLE_TOKEN( "<=>" );
const Component NULL_COMPONENT;
const NumericString NULL_NUMSTRING;




//--------------------------------------------------------------------------
//                     Reaction::Reaction( )
//..........................................................................
//                                                                          
//  default constructor 
//                                                                          
//--------------------------------------------------------------------------

Reaction::Reaction() :
equation(),
progr_rate_profile_filename(),
num_reactants( 0 ),
num_products( 0 ),
// reactant_list(),
// product_list(),
not_stoich( FALSE ),
rate_constant_format( TEMP_INDEPENDENT ),
equation_modified( FALSE ),
fwdA(),
fwdM(),
fwdEa(),
revA(),
revM(),
revEa()
{
     UINT16 i = MAX_NUMBER_OF_COMPONENTS;

     Component compt; 
         
     while ( i-- )
     {
          // explicitly assign members individually to default (MAC MPW C++  requirement)

          reactant_list[i] = compt;
          product_list[i] = compt;
     }
}


//--------------------------------------------------------------------------
//                     Reaction::Reaction( )
//..........................................................................
//                                                                          
//  copy constructor 
//                                                                          
//--------------------------------------------------------------------------

Reaction::Reaction( const Reaction& rRxn ) :
equation( rRxn.GetEquation() ),
progr_rate_profile_filename( rRxn.GetProgRateProfileName() ),
num_reactants( rRxn.GetNumReactants() ),
num_products( rRxn.GetNumProducts() ),
// reactant_list(),
// product_list(),
not_stoich( rRxn.HasNonStandardRateLaw() ),
rate_constant_format( rRxn.GetRateConstantFormat() ),
equation_modified( rRxn.IsModified( ) ),
fwdA( rRxn.GetFwdA() ),
fwdM( rRxn.GetFwdM() ),
fwdEa( rRxn.GetFwdEa() ),
revA( rRxn.GetRevA() ),
revM( rRxn.GetRevM() ),
revEa( rRxn.GetRevEa() )
{
     UINT16 i = MAX_NUMBER_OF_COMPONENTS;

     while ( i-- )
     {
          // assign members individually 

          reactant_list[i] = rRxn.reactant_list[i];
          product_list[i] = rRxn.product_list[i];
     }
}





//--------------------------------------------------------------------------
//                     Reaction::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator 
//                                                                          
//--------------------------------------------------------------------------

Reaction& Reaction::operator = ( const Reaction& rRxn )
{

     // avoid assignment to self 

     if ( &rRxn == this )
          return *this;

     // simple types 

     num_reactants = rRxn.num_reactants;
     num_products = rRxn.num_products;
     not_stoich = rRxn.not_stoich;
     rate_constant_format = rRxn.rate_constant_format;
     equation_modified = rRxn.equation_modified;

     // String and NumericString types 

     equation = rRxn.equation;
     progr_rate_profile_filename = rRxn.progr_rate_profile_filename;
     fwdA  = rRxn.fwdA;
     fwdM  = rRxn.fwdM;
     fwdEa = rRxn.fwdEa;
     revA  = rRxn.revA;
     revM  = rRxn.revM;
     revEa = rRxn.revEa;

     // Component array 

     UINT16 i = MAX_NUMBER_OF_COMPONENTS;

     while ( i-- )
     {
          // assign members individually 

          reactant_list[i] = rRxn.reactant_list[i];
          product_list[i] =  rRxn.product_list[i];
     }

     return *this;
}





//--------------------------------------------------------------------------
//                     Reaction::~Reaction( )
//..........................................................................
//                                                                          
//  destructor 
//                                                                          
//--------------------------------------------------------------------------

Reaction::~Reaction()
{
}




void Reaction::ClearComponentLists( )
{
     UINT16 i = MAX_NUMBER_OF_COMPONENTS;

     while ( i-- )
          reactant_list[ i ] = product_list[ i ] = NULL_COMPONENT;
}


void Reaction::Reset( )
{
     equation = NULL_STRING;
     progr_rate_profile_filename = NULL_STRING;

     num_reactants = num_products = 0;

     ClearComponentLists();

     not_stoich = FALSE;
     rate_constant_format = TEMP_INDEPENDENT;
     equation_modified = FALSE;

     fwdA = NULL_NUMSTRING;
     fwdM = NULL_NUMSTRING;
     fwdEa = NULL_NUMSTRING; 
     revA = NULL_NUMSTRING;
     revM = NULL_NUMSTRING;
     revEa = NULL_NUMSTRING;
}

//--------------------------------------------------------------------------
//                     Reaction::IsReversible( )
//..........................................................................
//                                                                          
//  returns TRUE is the reaction equation has been entered as a 
//  reversible reaction
//                                                                          
//--------------------------------------------------------------------------

BOOL Reaction::IsReversible( ) const
{
     return ( STRING_NOTFOUND != equation.Search( REVERSIBLE_TOKEN ) );
}






//--------------------------------------------------------------------------
//                     Reaction::ExtractSpeciesData( )
//..........................................................................
//                                                                          
//  analyzes a half-reaction for syntax, and extracts and save species data 
//  in the ComponentArray passed as a parameter
//                                                                          
//--------------------------------------------------------------------------

enum EQN_ANALYSIS_RESULT Reaction::ExtractSpeciesData( const String& rHalfEqn, 
   UINT16& NumSpecies, COMPONENT_ARRAY ComponentArray  ) 
{
     const USHORT num_species = rHalfEqn.GetTokenCount( SPECIES_SEPARATOR );
    

     // too many components  or none at all?? reject if so 

     if ( num_species > MAX_NUMBER_OF_COMPONENTS )
          return EQN_TOO_MANY_COMPS;

     if ( num_species == 0  )
          return EQN_SYNTAX;

     // prepare for syntax analysis 

     String species_data;
     String species_name;
     NumericString stoich;

     // scan through the half-rxn, analyzing its content as a sequence 
     // of stoichiometry-species name pairs. Each pair is separated from 
     // other pairs by a '+' char

     UINT16 i = 0;
     while ( i < num_species )
     {
          species_data  = rHalfEqn.GetToken( i, SPECIES_SEPARATOR );
          species_data.EraseLeadingChars( SPACE_CHAR );
          species_data.EraseTrailingChars( SPACE_CHAR );

          switch( species_data.GetTokenCount( SPACE_CHAR ) )
          {

          case 1 :

               // assume no stoichiometry given - implicit 1 

               stoich = "1";
               species_name = species_data.GetToken( 0, SPACE_CHAR );

               break;

          case 2 :
               // assume explicit stoichiometry 

               stoich = species_data.GetToken( 0, SPACE_CHAR );
               species_name = species_data.GetToken( 1, SPACE_CHAR );

               // be sure that what we assume is the stoichiometry 
               // actually represent an integer 

               if ( !stoich.IsValidPositiveINT16( ) )
               {
                    NumSpecies = 0;
                    return EQN_STOICH_ERROR;
               }
               else
                    break;

          default :                    

               // only come here if there are too many words - a syntax error 

               NumSpecies = 0;
               return EQN_SYNTAX;

          }  // end switch 

          // if we get here then we have successully tokenized the pair 
          // validate the data we have 

          enum EQN_ANALYSIS_RESULT rc = ValidateSpeciesName( species_name, 
           ComponentArray );

          if ( EQN_NO_ERROR != rc )
          {
               NumSpecies = 0;
               return rc;
          }

          // save the data in the component object array 

          ComponentArray[i].SetName( species_name );
          ComponentArray[i].SetStoichCoeff( stoich );

          // here we set the exponent only if there is not already a value 
          // This is done to avoid over-writing data previously entered by user 

          if ( '\0' == ( char) ComponentArray[i].GetExponent() )  // zero only if String is empty
               ComponentArray[i].SetExponent( stoich + ZERO_SUFFIX );

          i++;
     }   // end while 

     // we get here when we have completed the parsing and syntax analysis 
     // if we got here then everything is valid 
     // just be sure that everything else is cleaned up and return 

     NumSpecies = num_species;

     // now fill the unused elements of ComponentArrays with null strings
     // i == num_species at this point 

     while ( i < MAX_NUMBER_OF_COMPONENTS )
     {
          ComponentArray[i].SetName (NULL_STRING );
          ComponentArray[i].SetExponent (NULL_STRING );  // this is purposely set to null string == ""
                                                          // rather than null NumericString  == "0"
          i++;
     }           

     return EQN_NO_ERROR;
}






//--------------------------------------------------------------------------
//                     Reaction::ParseEquation( )
//..........................................................................
//                                                                          
//  Parses a full equation into two side containing only species and 
//  stoichiometry data. Returns TRUE is the expected SEPARATOR token 
//  is present, FALSE otherwise 
//                                                                          
//--------------------------------------------------------------------------

BOOL Reaction::ParseEquation( String& rLeftSide, String& rRightSide )
{
     // find the position of the substring which separates the two side of the equation

     USHORT position = equation.Search( SEPARATOR );

     if ( STRING_NOTFOUND == position )
          return FALSE;
     else
     {
          // copy the two sides of the eqation w/o the separator token

          rLeftSide = equation.Copy( 0, position );
          rRightSide = equation.Copy( position + SEPARATOR.Len() );

          // get rid of the reversibility character if there is one 

          if ( REVERSIBLE_CHAR == rLeftSide[ rLeftSide.Len() - 1] )
               rLeftSide.Erase( rLeftSide.Len() - 1, 1 );

        return TRUE;
     }

}


BOOL  Reaction::IsNullEquation( ) const
{
     // make a temporary copy of equation since we are going to alter its contents 

     String tmp = equation;
     tmp.EraseTrailingChars( SPACE_CHAR );

     return ( 0 == tmp.GetTokenCount( SPACE_CHAR )  );
}


//--------------------------------------------------------------------------
//                     Reaction::SyntaxIsValid( )
//..........................................................................
//                                                                          
//  Analyzes and stores the results of syntax analysis of a given equation
//  returns TRUE if valid, FALSE otherwise. returns an Error code through 
//  a reference parameter 
//                                                                          
//--------------------------------------------------------------------------

BOOL Reaction::SyntaxIsValid(enum EQN_ANALYSIS_RESULT& rErrorCode )
{
     String   left_side;
     String   right_side;

     // if eqn has no words in it then its an error 

     if ( IsNullEquation() )
     {

          rErrorCode = EQN_EMPTY_EQUATION;
          return FALSE;
     }
     else
     {
          // preserve old component data so that we save the exponents

          COMPONENT_ARRAY  old_reactants;
          COMPONENT_ARRAY  old_products;

          for (UINT16 i = 0; i < MAX_NUMBER_OF_COMPONENTS; i++ )
          {
                old_reactants[i] = reactant_list[i];
                old_products[i]  = product_list[i];
          }


          enum EQN_ANALYSIS_RESULT rc;

          ClearComponentLists( );

          // first find the middle of the equation and split there 

          if ( ! ParseEquation( left_side, right_side ) )
          {
               rErrorCode = EQN_SYNTAX;
               return FALSE;
          }

          // now check that the last character in leftside is a space
          // and that the first character in rightside is a space 

          if ( !isspace( left_side[ left_side.Len() - 1] ) || !isspace( right_side[0] ) )
          {
               rErrorCode  = EQN_SYNTAX;
               return FALSE;
          }


          // now analyze individually the left- and right-sides of the equation
          // to be sure the syntax describing species and stpichiometry is OK

          if ( EQN_NO_ERROR != ( rc = ExtractSpeciesData( left_side, 
           num_reactants, reactant_list ) ) )
          {
               rErrorCode = rc;
               return FALSE;
          }

          if ( EQN_NO_ERROR != ( rc = ExtractSpeciesData( right_side, 
           num_products, product_list ) ) )
          {
               rErrorCode = rc;
               return FALSE;
          }

          // passed all tests - successful completion 

          RestoreOldComponentData( num_reactants, old_reactants, reactant_list);
          RestoreOldComponentData( num_products, old_products, product_list);

          rErrorCode = EQN_NO_ERROR;
          return TRUE;
     }
}




//----------------------------------------------------------------------------
//                    Reaction::RestoreOldComponentData( )
//............................................................................
//                                                                          
//  for each member of NewArray, this function searches through OldArray for 
//  a name that matches. If one is found then the exponent value of the 
//  member in OldArray is copied to the exponent of the member in NewArray 
//
//----------------------------------------------------------------------------

void Reaction::RestoreOldComponentData( UINT16 NumComponents, 
 COMPONENT_ARRAY OldArray, COMPONENT_ARRAY NewArray )
{
     // scan through  NewArray 

     for ( UINT16 i = 0 ; i < NumComponents; i++ )
     {
          for ( UINT16 j = 0; j < MAX_NUMBER_OF_COMPONENTS; j++)
          {
               // scan through OldArray looking for match 

               if ( COMPARE_EQUAL == NewArray[i].GetName().ICompare( OldArray[j].GetName( ) ) )
                    NewArray[i].SetExponent( OldArray[j].GetExponent() );
          }
     }


}



//----------------------------------------------------------------------------
//                        ValidateSpeciesName( )                            
//............................................................................
//                                                                          
// checks a string to be sure it meets all the criteria for a valid species 
// name : these are                                                         
//                                                                          
//   1. first character is alphabetical                                     
//                                                                          
//   2. the name does not contain characters used a sumbols in analyzing    
//      the equation : "+ < = >" and whitespace.                            
//                                                                          
//   3. the maximum allowed length is MAX_LENGTH_OF_NAME                    
//                                                                          
//  function returns EQN_NO_ERROR if string is OK, an error code    
//  otherwise                                                               
//                                                                          
//----------------------------------------------------------------------------

enum EQN_ANALYSIS_RESULT Reaction::ValidateSpeciesName( const String& rName, 
 const COMPONENT_ARRAY ComponentArray )
{
     USHORT length = rName.Len( );

     if ( length > MAX_LENGTH_OF_NAME )
               return EQN_SPECIES_NAME_TOO_LONG;

     char c = rName[0];

     if ( ! isgraph( c ) || isdigit( c ) )
          return EQN_SPECIES_ERROR;

     // now scan backward through string looking for illegal chars 

     while ( length-- )
     {
          c = rName[length];

          if ( ! isgraph( c ) || ( STRING_NOTFOUND != RESERVED_CHARS.Search( c ) ) )
               return EQN_SPECIES_ERROR;
     }

     // we get here is the name is valid - 
     // now check to see if the species name is already in the list 

     for ( UINT16 j = 0; j < MAX_NUMBER_OF_COMPONENTS; j++ )
     {
          if ( COMPARE_EQUAL == rName.ICompare( ComponentArray[j].GetName( ) ) )
                    return EQN_NAME_APPEARS_TWICE;
     }

     // if we make it here then we have passed all the tests 

     return EQN_NO_ERROR;
}








//----------------------------------------------------------------------------
//                   Reaction::SynchRxnDataWithSpeciesList( )
//............................................................................
//                                                                          
//                                                                          
//                                                                          
//----------------------------------------------------------------------------

void Reaction::SynchRxnDataWithSpeciesList( const CompartmentSpeciesList& rSpeciesList )
{
     for ( UINT16 i = 0; i < num_reactants; i++ )
     {
          const String& name = reactant_list[i].GetName();

          SPECIES_ID id = rSpeciesList.GetSpeciesDataByName( name ).GetID();
          reactant_list[i].SetSpeciesID( id );
     }

     for ( i = 0; i < num_products; i++ )
     {
          const String& name = product_list[i].GetName();

          SPECIES_ID id = rSpeciesList.GetSpeciesDataByName( name ).GetID();

          product_list[i].SetSpeciesID( id );
     }
}



//----------------------------------------------------------------------------
//                        operator << Reaction                           
//............................................................................
//                                                                          
//  for output of contents of a Reaction object to BinaryOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Reaction& rObject )
{
     const UINT16 num_reactants = rObject.GetNumReactants();
     const UINT16 num_products = rObject.GetNumProducts();
     const BOOL8  non_stoich = rObject.HasNonStandardRateLaw();

     enum RATE_CONST_FORMAT rate_format = rObject.GetRateConstantFormat();

     // write some binary values 

     rBOS << IDType( REACTION );

     rBOS << num_reactants;
     rBOS << num_products;
     rBOS << (UINT16) non_stoich;
     rBOS << (UINT16) rate_format;

     if ( PROGRAMMED == rate_format )
     {
          rBOS << rObject.progr_rate_profile_filename;
     }

     // write equation 

     rBOS << rObject.GetEquation();

     // write rate constant values 

     rBOS << rObject.GetFwdA();
     rBOS << rObject.GetFwdM();
     rBOS << rObject.GetFwdEa();

     rBOS << rObject.GetRevA();
     rBOS << rObject.GetRevM();
     rBOS << rObject.GetRevEa();

     for ( UINT16 i = 0; i < num_reactants; i++ )
          rBOS << rObject.reactant_list[i];

     for ( i = 0; i < num_products; i++ )
          rBOS << rObject.product_list[i];

     return rBOS;
}





// NEW FORMAT

//----------------------------------------------------------------------------
//                        operator << Reaction                           
//............................................................................
//                                                                          
//  for output of contents of a Reaction object to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const Reaction& rObject )
{

     const UINT16 num_reactants = rObject.GetNumReactants();
     const UINT16 num_products = rObject.GetNumProducts();
     const BOOL8  non_stoich = rObject.HasNonStandardRateLaw();

     enum RATE_CONST_FORMAT rate_format = rObject.GetRateConstantFormat();

     // write some binary values 

     rTOS << IDType( START_RXN_INIT ) << EOL;
     rTOS << IDType( RXN_DATA ) << rObject.IsReversible() << EOL; 

     rTOS << IDType( RXN_RATE_CONSTANT_INIT ) << EOL;
     rTOS << IDType( RXN_RATE_CONSTANT_TYPE ) << (UINT16) rate_format << EOL;

     // write rate constant values 

     switch ( rate_format )
     {
     case TEMP_INDEPENDENT :
     case TEMP_DEPENDENT :

          rTOS << IDType( RXN_FWD_ACT_PARMS ) << rObject.GetFwdA() <<
           rObject.GetFwdM() << rObject.GetFwdEa() << EOL;
     
          rTOS << IDType( RXN_REV_ACT_PARMS ) << rObject.GetRevA() <<
           rObject.GetRevM() << rObject.GetRevEa() << EOL;

          break;
         


     case PROGRAMMED :

          {
               rTOS << IDType( PROFILE_INIT ) << EOL;

               XyDataFile profile;

               profile.SetFilename( rObject.GetProgRateProfileName() );

               if ( profile.InitializeFromFile() )
               {
                    profile.WriteValidDataToTextStream( rTOS, NO_PROFILE_POINTS, PROFILE_DATA_PAIR );
               }
               else
               {
                    WarningBox( NULL, WB_OK | WB_DEF_OK, profile.GetErrorMessage() ).Execute();
               }

               rTOS << IDType( END_PROFILE_INIT ) << EOL;

          }
          break;


     default :

          // we should never get here !

          ASSERT ( 0 );

          break;
     }

     rTOS << IDType( END_RXN_RATE_CONSTANT_INIT ) << EOL;

     // we deal with output of rate law data in a manner that 
     // depends on whether the rate law is based on stoichiometry or not 

     // write out the forward rate law info 

     rTOS << IDType( RXN_FWD_RATE_LAW ) << num_reactants << EOL;

     for ( UINT16 i = 0; i < num_reactants; i++ )
     {
          const Component& rcomp = rObject.reactant_list[i];

          // write out id and stoich coefficient 

          rTOS << IDType( RXN_SPECIES_DATA ) << rcomp.GetSpeciesID() <<
           rcomp.GetStoichCoeff();

          // the next data member is the exponent in the rate law 
          // if we are using a standard rate law, then just repeat 
          // stoich coeff, otherwise write out the special exponent 

          if ( non_stoich )
               rTOS << rcomp.GetExponent() << EOL; 
          else
               rTOS << rcomp.GetStoichCoeff() << EOL; 
     }


     // write out the reverse rate law info in analogous form ...

     rTOS << IDType( RXN_REV_RATE_LAW ) << num_products << EOL;

     for ( i = 0; i < num_products; i++ )
     {
          const Component& rcomp = rObject.product_list[i];

          // write out id and stoich coefficient 

          rTOS << IDType( RXN_SPECIES_DATA ) << rcomp.GetSpeciesID() <<
           rcomp.GetStoichCoeff();

          // the next data member is the exponent in the rate law 
          // if we are using a standard rate law, then just repeat 
          // stoich coeff, otherwise write out the special exponent 

          if ( non_stoich )
               rTOS << rcomp.GetExponent() << EOL; 
          else
               rTOS << rcomp.GetStoichCoeff() << EOL; 
     }


     rTOS << IDType( END_RXN_INIT ) << EOL;

     return rTOS;
}




#if 0 

// OLD FORMAT

//----------------------------------------------------------------------------
//                        operator << Reaction                           
//............................................................................
//                                                                          
//  for output of contents of a Reaction object to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const Reaction& rObject )
{

     const UINT16 num_reactants = rObject.GetNumReactants();
     const UINT16 num_products = rObject.GetNumProducts();
     const BOOL8  non_stoich = rObject.HasNonStandardRateLaw();

     enum RATE_CONST_FORMAT rate_format = rObject.GetRateConstantFormat();

     // write some binary values 

     rTOS << IDType( START_RXN_INIT ) << EOL;
     rTOS << IDType( RXN_DATA ) <<  non_stoich << 
      (UINT16) rate_format <<  rObject.IsReversible() << EOL;

     // write rate constant values 

     rTOS << IDType( RXN_FWD_ACT_PARMS ) << rObject.GetFwdA() <<
      rObject.GetFwdM() << rObject.GetFwdEa() << EOL;

     rTOS << IDType( RXN_REV_ACT_PARMS ) << rObject.GetRevA() <<
      rObject.GetRevM() << rObject.GetRevEa() << EOL;

     // we deal with output of rate data data in a manner that 
     // depends on whether the rate law is based on stoichiometry or not 

     // write out the forward rate law info 

     rTOS << IDType( RXN_FWD_RATE_LAW ) << num_reactants << EOL;

     for ( UINT16 i = 0; i < num_reactants; i++ )
     {
          const Component& rcomp = rObject.reactant_list[i];

          // write out id and stoich coefficient 

          rTOS << IDType( RXN_SPECIES_DATA ) << rcomp.GetSpeciesID() <<
           rcomp.GetStoichCoeff();

          // the next data member is the exponent in the rate law 
          // if we are using a standard rate law, then just repeat 
          // stoich coeff, otherwise write out the special exponent 

          if ( non_stoich )
               rTOS << rcomp.GetExponent() << EOL; 
          else
               rTOS << rcomp.GetStoichCoeff() << EOL; 
     }


     // write out the reverse rate law info in analogous form ...

     rTOS << IDType( RXN_REV_RATE_LAW ) << num_products << EOL;

     for ( i = 0; i < num_products; i++ )
     {
          const Component& rcomp = rObject.product_list[i];

          // write out id and stoich coefficient 

          rTOS << IDType( RXN_SPECIES_DATA ) << rcomp.GetSpeciesID() <<
           rcomp.GetStoichCoeff();

          // the next data member is the exponent in the rate law 
          // if we are using a standard rate law, then just repeat 
          // stoich coeff, otherwise write out the special exponent 

          if ( non_stoich )
               rTOS << rcomp.GetExponent() << EOL; 
          else
               rTOS << rcomp.GetStoichCoeff() << EOL; 
     }


     rTOS << IDType( END_RXN_INIT ) << EOL;

     return rTOS;
}

#endif




//----------------------------------------------------------------------------
//                        operator >> Reaction                           
//............................................................................
//                                                                          
//  for output of contents of a Reaction object to TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Reaction& rObject )
{
     // first check the IDType

     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( REACTION ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }

     UINT16 temp_value;

     UINT16 num_reactants;
     UINT16 num_products;

     // read in some binary data

     rBIS >> num_reactants;
     rObject.num_reactants = num_reactants;

     rBIS >> num_products;
     rObject.num_products = num_products;

     rBIS >> temp_value;
     rObject.SetNonStandardRateLaw( (BOOL8) temp_value );

     rBIS >> temp_value;
     rObject.rate_constant_format = (enum RATE_CONST_FORMAT) temp_value;

     if ( PROGRAMMED == rObject.rate_constant_format )
     {
          rBIS >> rObject.progr_rate_profile_filename;
     }

     // read equation 

     rBIS >> rObject.equation;

     // read rate constant values 

     rBIS >> rObject.fwdA;
     rBIS >> rObject.fwdM;
     rBIS >> rObject.fwdEa;

     rBIS >> rObject.revA;
     rBIS >> rObject.revM;
     rBIS >> rObject.revEa;

     for ( UINT16 i = 0; i < num_reactants; i++ )
     {
          rBIS >> rObject.reactant_list[i];

          if ( rBIS.FileError() )
               return rBIS;
     }

     for ( i = 0; i < num_products; i++ )
     {
          rBIS >> rObject.product_list[i];

          if ( rBIS.FileError() )
               return rBIS;
     }

     return rBIS;
}





#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>

void main()
{
     Reaction rxn;
     enum EQN_ANALYSIS_RESULT err_code;
     BOOL test;

     cout << "testing valid equation -  errcode = 0" << endl;
     rxn.SetEquation( String( "1 A + 2 B => 3 C + 4 D") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible()<< "\n"<< endl;
     rxn.Reset();

     cout << "testing valid equation -  errcode = 0" << endl;
     rxn.SetEquation( String( "ABCDEFGH <=> BCDEFGHI") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n" << endl;
     rxn.Reset();


     cout << "testing valid equation  with spaces -  errcode = 0" << endl;
     rxn.SetEquation( String( "    ABCDEFG   <=>   BCDEFGH   ") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n" << endl;
     rxn.Reset();


     cout << "testing valid equation -  errcode = 0" << endl;
     rxn.SetEquation( String( "aaaaaaaa + 2 bbbbbbbb + 3 cccccccc + 4 dddddddd <=> 5 eeeeeeee + 6 ffffffff + 7 gggggggg + 8 hhhhhhhh") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;

     cout << "Reactants : " << endl;
     for ( USHORT i = 0; i <rxn.GetNumReactants() ; i++ )
          cout << (const char *) rxn.GetReactantName( i ) << endl;
     cout << "Products : " << endl;
     for ( i = 0; i <rxn.GetNumProducts() ; i++ )
          cout << (const char *) rxn.GetProductName(i) << endl;

     cout << "\n";

     cout << "testing copy constructor for Reaction\n" << endl;
     Reaction copy = rxn;
     cout << "eqn = " << (const char *) copy.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << copy.GetNumReactants()<< endl;
     cout << "num products = " << copy.GetNumProducts()<< endl;
     cout << "reversible = " << (int) copy.IsReversible() <<"\n"<< endl;

     cout << "Reactants : " << endl;
     for ( i = 0; i <copy.GetNumReactants() ; i++ )
          cout << (const char *) copy.GetReactantName(i) << endl;
     cout << "Products : " << endl;
     for ( i = 0; i <copy.GetNumProducts() ; i++ )
          cout << (const char *) copy.GetProductName(i) << endl;

     cout << "\n";


     cout << "testing copy constructor for Reaction - compare to previous 2 lists \n" << endl;
     test = copy.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) copy.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << copy.GetNumReactants()<< endl;
     cout << "num products = " << copy.GetNumProducts()<< endl;
     cout << "reversible = " << (int) copy.IsReversible() <<"\n"<< endl;

     cout << "Reactants : " << endl;
     for ( i = 0; i <copy.GetNumReactants() ; i++ )
          cout << (const char *) copy.GetReactantName(i)<< endl;
     cout << "Products : " << endl;
     for ( i = 0; i <copy.GetNumProducts() ; i++ )
          cout << (const char *) copy.GetProductName(i) << endl;

     cout << "\n";


     rxn.Reset();

     cout << "testing invalid equation syntax -  errcode = 1" << endl;
     rxn.SetEquation( String( "fk dklfj df' d' + D") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing invalid equation syntax -  errcode = 1" << endl;
     rxn.SetEquation( String( "a + b=>f") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing invalid equation syntax -  errcode = 1" << endl;
     rxn.SetEquation( String( "a + b = > f") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing invalid species name  -  errcode = 2" << endl;
     rxn.SetEquation( String( "1A + 2 B => 3 C + 4 D") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing stoichiometry error - errcode = 3" << endl;
     rxn.SetEquation( String( "Q A => B ") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing stoichiometry error - errcode = 3" << endl;
     rxn.SetEquation( String( "1.0 A  => B ") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing too many components - errcode = 4" << endl;
     rxn.SetEquation( String( "A + B + C + D + E => F") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing species twice -  errcode = 5" << endl;
     rxn.SetEquation( String( "aaaaaaaa + aaaaaaaa => eeeeeeee ") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing empty equation - errcode = 6" << endl;
     rxn.SetEquation( String( " ") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;
     rxn.Reset();

     cout << "testing invalid species name length -  errcode = 7" << endl;
     rxn.SetEquation( String( "aaaaaaaaaa => C + D") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible() <<"\n"<< endl;


     cout << "testing valid equation -  errcode = 0" << endl;
     rxn.SetEquation( String( "1 First + 2 Second => 3 Third + 4 Fourth") );
     test = rxn.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) rxn.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << rxn.GetNumReactants()<< endl;
     cout << "num products = " << rxn.GetNumProducts()<< endl;
     cout << "reversible = " << (int) rxn.IsReversible()<< "\n"<< endl;

     cout << "Reactants : " << endl;
     for ( i = 0; i <rxn.GetNumReactants() ; i++ )
          cout << (const char *) rxn.GetReactantName(i) << " " << 
           (const char *) rxn.GetReactantStoichiometry(i) << endl;
     cout << "Products : " << endl;
     for ( i = 0; i <rxn.GetNumProducts() ; i++ )
          cout << (const char *) rxn.GetProductName(i)<< " " << 
          (const char *)rxn.GetProductStoichiometry(i)  << endl;

     cout << "\n";



     cout << "testing assignment operator  for Reaction\n" << endl;
     copy = rxn;
     cout << "eqn = " << (const char *) copy.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << copy.GetNumReactants() << endl;
     cout << "num products = " << copy.GetNumProducts()<< endl;
     cout << "reversible = " << (int) copy.IsReversible() <<"\n"<< endl;

     cout << "Reactants : " << endl;
     for ( i = 0; i <copy.GetNumReactants() ; i++ )
          cout << (const char *) copy.GetReactantName(i) << " " << 
          (const char *)copy.GetReactantStoichiometry(i) << endl;
     cout << "Products : " << endl;
     for ( i = 0; i <copy.GetNumProducts() ; i++ )
          cout << (const char *) copy.GetProductName(i)<< " " << 
          (const char *)copy.GetProductStoichiometry(i)  << endl;

     cout << "\n";


     cout << "testing assignment operator for Reaction - compare to previous 2 lists \n" << endl;
     test = copy.SyntaxIsValid( err_code );
     cout << "eqn = " << (const char *) copy.GetEquation( ) << endl;
     cout << "valid = " << (int) test << endl;
     cout << "err_code = " << (int) err_code << endl;
     cout << "num reactants = " << copy.GetNumReactants()<< endl;
     cout << "num products = " << copy.GetNumProducts()<< endl;
     cout << "reversible = " << (int) copy.IsReversible() <<"\n"<< endl;


     cout << "Reactants : " << endl;
     for ( i = 0; i <copy.GetNumReactants() ; i++ )
          cout << (const char *) copy.GetReactantName(i) << " " << 
          (const char *) copy.GetReactantStoichiometry(i) << endl;
     cout << "Products : " << endl;
     for ( i = 0; i <copy.GetNumProducts() ; i++ )
          cout << (const char *) copy.GetProductName(i)<< " " << 
          (const char *) copy.GetProductStoichiometry(i)  << endl;

     cout << "\n";
     cout << "\n";




}



#endif


