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
//  REACTION.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the Reaction object, which contains
//  data describing a reaction step in a reaction scheme      
//
//  CREATED : 1.27.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__REACTION_HXX__)
#define __REACTION_HXX__

#include "componnt.hxx"
#include "numstrng.hxx"
#include "constant.hxx"


class BinaryOutputStream;
class TextOutputStream;
class CompartmentSpeciesList;


  
typedef Component COMPONENT_ARRAY[ MAX_NUMBER_OF_COMPONENTS ];

enum EQN_ANALYSIS_RESULT { EQN_NO_ERROR, EQN_SYNTAX, EQN_SPECIES_ERROR, 
                           EQN_STOICH_ERROR, EQN_TOO_MANY_COMPS, EQN_NAME_APPEARS_TWICE,
                           EQN_EMPTY_EQUATION, EQN_SPECIES_NAME_TOO_LONG, EQN_OTHER };

class  Reaction
{

private:
     String      equation;
     String      progr_rate_profile_filename;

     UINT16      num_reactants;
     UINT16      num_products;

     COMPONENT_ARRAY  reactant_list;
     COMPONENT_ARRAY  product_list;

     BOOL                     not_stoich;
     enum RATE_CONST_FORMAT   rate_constant_format;
     BOOL                     equation_modified;

     NumericString      fwdA;         //  for the rate k=A x T**M x e**(-Ea/RT) 
     NumericString      fwdM;
     NumericString      fwdEa; 
                                          
     NumericString      revA;
     NumericString      revM;
     NumericString      revEa;

     enum EQN_ANALYSIS_RESULT ValidateSpeciesName( const String& rName, 
      const COMPONENT_ARRAY ComponentArray );

     enum EQN_ANALYSIS_RESULT ExtractSpeciesData( const String& rHalfEqn, 
          UINT16& NumSpecies, COMPONENT_ARRAY ComponentArray  );

     BOOL ParseEquation( String& rLeftSide, String& rRightSide );
     void ClearComponentLists( );

     void RestoreOldComponentData( UINT16 NumComponents, 
      COMPONENT_ARRAY OldArray, COMPONENT_ARRAY NewArray );

public:
                    Reaction();
                    Reaction( const Reaction& rRxn );
                   ~Reaction();


     Reaction&      operator = ( const Reaction& rRxn );

     void           SetEquation( const String& rNewStr ) { equation = rNewStr; } 
     const String&  GetEquation( ) const { return equation; } 

     const String&  GetProgRateProfileName() const { return progr_rate_profile_filename;}
     void           SetProgRateProfileName( const String& rNewStr ) { progr_rate_profile_filename = rNewStr;}

     void           SetFwdA( const String& rNewStr ) { fwdA  = rNewStr; } 
     void           SetFwdM( const String& rNewStr ) { fwdM  = rNewStr; } 
     void           SetFwdEa( const String& rNewStr ) { fwdEa  = rNewStr; } 
     void           SetRevA( const String& rNewStr ) { revA  = rNewStr; } 
     void           SetRevM( const String& rNewStr ) { revM  = rNewStr; } 
     void           SetRevEa( const String& rNewStr ) { revEa  = rNewStr; } 

     const NumericString&  GetFwdA( ) const { return fwdA; } 
     const NumericString&  GetFwdM( ) const { return fwdM; } 
     const NumericString&  GetFwdEa( ) const { return fwdEa; } 
     const NumericString&  GetRevA( ) const { return revA; } 
     const NumericString&  GetRevM( ) const { return revM; } 
     const NumericString&  GetRevEa( ) const { return revEa; } 

     BOOL           IsReversible( ) const ;
     BOOL           IsModified( ) const { return equation_modified; }
     BOOL           IsNullEquation( ) const;
     BOOL           HasNonStandardRateLaw( ) const { return not_stoich; }
     BOOL           SyntaxIsValid(enum EQN_ANALYSIS_RESULT& rErrorCode );
     UINT16         GetNumReactants( ) const { return num_reactants; }
     UINT16         GetNumProducts( ) const { return num_products; }

     enum RATE_CONST_FORMAT   GetRateConstantFormat() const { return rate_constant_format; }
     void SetRateConstantFormat( enum RATE_CONST_FORMAT NewFormat ){ rate_constant_format = NewFormat; }
     void           Reset( );
     void           SynchRxnDataWithSpeciesList( const CompartmentSpeciesList& rSpeciesList );
     void           SetNonStandardRateLaw( BOOL8 State ) { not_stoich = State; }

     void   SetReactantSpeciesID( UINT16 ReactantNumber, SPECIES_ID NewIndex ) 
     { 
          reactant_list[ ReactantNumber ].SetSpeciesID( NewIndex ); 
     }


     SPECIES_ID   GetReactantSpeciesID( UINT16 ReactantNumber ) const
     { 
         return reactant_list[ ReactantNumber ].GetSpeciesID( ); 
     }

     void   SetReactantExponent( UINT16 ReactantNumber, const NumericString& rNewStr ) 
     { 
          reactant_list[ ReactantNumber ].SetExponent( rNewStr ); 
     }

     const NumericString&   GetReactantExponent( UINT16 ReactantNumber ) const
     { 
         return reactant_list[ ReactantNumber ].GetExponent( ); 
     }

     const NumericString&    GetReactantStoichiometry( UINT16 ReactantNumber ) const 
     { 
          return reactant_list[ ReactantNumber ].GetStoichCoeff( ); 
     }

     const String&    GetReactantName(  UINT16 ReactantNumber ) const 
     { 
          return reactant_list[ ReactantNumber ].GetName( ); 
     }


     void   SetProductSpeciesID( UINT16 ProductNumber, SPECIES_ID NewID ) 
     { 
          product_list[ ProductNumber ].SetSpeciesID( NewID ); 
     }


     SPECIES_ID GetProductSpeciesID( UINT16 ProductNumber ) const
     { 
         return product_list[ ProductNumber ].GetSpeciesID( ); 
     }

     void   SetProductExponent( UINT16 ProductNumber, const NumericString& rNewStr ) 
     { 
          product_list[ ProductNumber ].SetExponent( rNewStr ); 
     }

     const NumericString&   GetProductExponent( UINT16 ProductNumber )  const
     { 
         return product_list[ ProductNumber ].GetExponent( ); 
     }

     const NumericString&    GetProductStoichiometry( UINT16 ProductNumber ) const 
     { 
          return product_list[ ProductNumber ].GetStoichCoeff( ); 
     }

     const String&    GetProductName( UINT16 ProductNumber ) const 
     { 
          return product_list[ ProductNumber ].GetName( ); 
     }


     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Reaction& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Reaction& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const Reaction& rObject );

};



BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const Reaction& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, Reaction& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const Reaction& rObject );


#endif

