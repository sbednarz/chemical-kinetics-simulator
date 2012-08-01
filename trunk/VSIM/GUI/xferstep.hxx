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
//  XFERSTEP.HXX - a header file of the VSIM User interface
// ............................................................................
//
//  This module declares the TransferStep object, which contains
//  data describing a materials transfer step in a reaction scheme
//
//  CREATED : 4.18.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__XFERSTEP_HXX__)
#define __XFERSTEP_HXX__

#include "constant.hxx"
#include "specid.hxx"
#include "numstrng.hxx"


class BinaryOutputStream;
class BinaryInputStream;
class TextOutputStream;
class SpeciesDatabase;


enum XFER_STEP_TYPE  { XS_TYPE_NONE, XS_TYPE_MASS_XFER, XS_TYPE_STD_DIFFUSION, XS_TYPE_VIRTUAL_DIFFUSION  };

class  TransferStep
{

private:
     XFER_STEP_TYPE           step_type;
     SPECIES_ID               species_id;     
     SPECIES_ID               species_id2;     
     String                   name;           
     String                   name2;           

     enum RATE_CONST_FORMAT   rate_constant_format;
     BOOL8                    is_modified;
     enum XFER_DIRECTION      transfer_direction;

     NumericString            fwdA;         //  for the rate k=A x T**M x e**(-Ea/RT) 
     NumericString            fwdM;
     NumericString            fwdEa; 
                                          
     NumericString            revA;
     NumericString            revM;
     NumericString            revEa;

     void SetID( const SPECIES_ID NewId ) { species_id = NewId; }
     void SetID2( const SPECIES_ID NewId ) { species_id2 = NewId; }

public:
                    TransferStep();
                    TransferStep( XFER_STEP_TYPE StepType );
                    TransferStep( const TransferStep& rStep );

     TransferStep&  operator = ( const TransferStep& rStep );

     void           SynchWithSpeciesDatabase( SpeciesDatabase& rSDB );

     void           SetSpecies( const String& rSpeciesName, const SPECIES_ID SpeciesId )
                    { name = rSpeciesName; species_id = SpeciesId; }

     void           SetSpecies2( const String& rSpeciesName, const SPECIES_ID SpeciesId )
                    { name2 = rSpeciesName; species_id2 = SpeciesId; }


     // inlined functions 

     const String&  GetSpeciesName( ) const { return name; }
     SPECIES_ID     GetSpeciesID( ) const { return species_id; }
     const String&  GetSpeciesName2( ) const { return name2; }
     SPECIES_ID     GetSpeciesID2( ) const { return species_id2; }

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

     enum XFER_DIRECTION GetTransferDirection( ) const { return transfer_direction; }
     BOOL           IsModified( ) const { return is_modified; }
     void           SetModified( BOOL8 NewVal ) { is_modified = NewVal; }
     void           SetTransferDirection( enum XFER_DIRECTION NewVal ) { transfer_direction = NewVal; }

     XFER_STEP_TYPE GetType( ) const { return step_type; }
     void           SetType( const XFER_STEP_TYPE NewType ) { step_type = NewType; }

     enum RATE_CONST_FORMAT   GetRateConstantFormat() const { return rate_constant_format; }
     void SetRateConstantFormat( enum RATE_CONST_FORMAT NewFormat ){ rate_constant_format = NewFormat; }

     // stream operators

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferStep& rObject );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, TransferStep& rObject );
     friend TextOutputStream& operator << ( TextOutputStream& rTOS, const TransferStep& rObject );

};



BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferStep& rObject );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, TransferStep& rObject );
TextOutputStream& operator << ( TextOutputStream& rTOS, const TransferStep& rObject );


#endif

