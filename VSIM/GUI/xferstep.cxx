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
//  XFERSTEP.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines the TransferStep object, which contains
//  data describing a materials transfer step in a reaction scheme      
//
//  CREATED : 4.18.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "vsimtype.hxx"
#include "idtypes.hxx"
#include "specdb.hxx"
#include "mainapp.hxx"
#include "xferstep.hxx"

#endif

#include "strings.hrc"



//--------------------------------------------------------------------------
//                     TransferStep::TransferStep()
//..........................................................................
//                                                                          
//  default constructor 
//                                                                          
//--------------------------------------------------------------------------

TransferStep::TransferStep() :
step_type( XS_TYPE_NONE ),
species_id( INVALID_ID ),   
species_id2( INVALID_ID ),   
name(),
name2(),
rate_constant_format( TEMP_INDEPENDENT ),
is_modified( FALSE ),
transfer_direction( SOURCE_TO_TARGET ),
fwdA( ResId( vID_STR_REAL_ONE ) ),
fwdM(),
fwdEa(),
revA( ResId( vID_STR_REAL_ONE ) ),
revM(),
revEa()
{
}

//--------------------------------------------------------------------------
//                     TransferStep::TransferStep()
//..........................................................................
//                                                                          
//  constructor 
//                                                                          
//--------------------------------------------------------------------------

TransferStep::TransferStep( XFER_STEP_TYPE StepType ) :
step_type( StepType ),
species_id( INVALID_ID ),   
species_id2( INVALID_ID ),   
name(),
name2(),
rate_constant_format( TEMP_INDEPENDENT ),
is_modified( FALSE ),
transfer_direction( SOURCE_TO_TARGET ),
fwdA( ResId( vID_STR_REAL_ONE ) ),
fwdM(),
fwdEa(),
revA( ResId( vID_STR_REAL_ONE ) ),
revM(),
revEa()
{
}


//--------------------------------------------------------------------------
//                     Reaction::Reaction( )
//..........................................................................
//                                                                          
//  copy constructor 
//                                                                          
//--------------------------------------------------------------------------

TransferStep::TransferStep( const TransferStep& rStep ) :
step_type( rStep.GetType() ),
species_id( rStep.GetSpeciesID() ),  
species_id2( rStep.GetSpeciesID2() ),  
name( rStep.GetSpeciesName() ),
name2( rStep.GetSpeciesName2() ),
rate_constant_format( rStep.GetRateConstantFormat() ),
is_modified( rStep.IsModified() ),
transfer_direction( rStep.GetTransferDirection() ),
fwdA( rStep.GetFwdA() ),
fwdM( rStep.GetFwdM() ),
fwdEa( rStep.GetFwdEa() ),
revA( rStep.GetRevA() ),
revM( rStep.GetRevM() ),
revEa( rStep.GetRevEa() )
{
}




void TransferStep::SynchWithSpeciesDatabase( SpeciesDatabase& rSDB )
{
     // set its ID to that returned by the Database 

     SetID( rSDB.GetSpeciesID( GetSpeciesName( ) ) );

     // deal with second species name if needed

     if ( XS_TYPE_VIRTUAL_DIFFUSION == GetType() )
     {
          SetID2( rSDB.GetSpeciesID( GetSpeciesName2( ) ) );
     }

}








//--------------------------------------------------------------------------
//                     Reaction::operator = ( )
//..........................................................................
//                                                                          
//  assignment operator 
//                                                                          
//--------------------------------------------------------------------------

TransferStep& TransferStep::operator = ( const TransferStep& rStep )
{

     // avoid assignment to self 

     if ( &rStep == this )
          return *this;

     // simple types 

     step_type            = rStep.step_type;
     species_id           = rStep.species_id;
     species_id2          = rStep.species_id2;
     rate_constant_format = rStep.rate_constant_format;
     is_modified          = rStep.is_modified;
     transfer_direction   = rStep.transfer_direction;

     // String and NumericString types 

     name  = rStep.name;
     name2  = rStep.name2;
     fwdA  = rStep.fwdA;
     fwdM  = rStep.fwdM;
     fwdEa = rStep.fwdEa;
     revA  = rStep.revA;
     revM  = rStep.revM;
     revEa = rStep.revEa;

     return *this;
}





//----------------------------------------------------------------------------
//                        operator << TransferStep                       
//............................................................................
//                                                                          
//  for output of contents of a TransferStep object to BinaryOutputStream
//                                                                          
//----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const TransferStep& rObject )
{
     // write some binary values 

     rBOS << IDType( TRANSFER_STEP );
     rBOS << (UINT16) rObject.step_type;
     rBOS << rObject.species_id;
     rBOS << rObject.species_id2;
     rBOS << (UINT16) rObject.rate_constant_format;
     rBOS << (UINT16) rObject.is_modified;
     rBOS << (UINT16) rObject.GetTransferDirection();

     // String and NumericString values 

     rBOS << rObject.name;           
     rBOS << rObject.name2;           

     rBOS << rObject.fwdA;       
     rBOS << rObject.fwdM;
     rBOS << rObject.fwdEa; 
                         
     rBOS << rObject.revA;
     rBOS << rObject.revM;
     rBOS << rObject.revEa;

     return rBOS;
}



//----------------------------------------------------------------------------
//                        operator << TransferStep                       
//............................................................................
//                                                                          
//  for output of contents of a TransferStep object to a TextOutputStream
//                                                                          
//----------------------------------------------------------------------------

TextOutputStream& operator << ( TextOutputStream& rTOS, const TransferStep& rObject )
{
     // write some binary values 

     rTOS << IDType( START_XFER_STEP_INIT ) << EOL;

     switch( rObject.GetType() )
     {

     case XS_TYPE_NONE :
     default: 

          ASSERT( 0 );
          break;


     case XS_TYPE_MASS_XFER :

          rTOS << IDType( XFER_MASS_TRANSFER ) << EOL;
          break;


     case XS_TYPE_STD_DIFFUSION :

          rTOS << IDType( XFER_MASS_DIFN ) << EOL;
          break;


     case XS_TYPE_VIRTUAL_DIFFUSION :

          rTOS << IDType( XFER_VIRT_DIFN ) << EOL;
          break;
     }

     rTOS << IDType( XFER_STEP_DATA ) << (UINT16) rObject.rate_constant_format << 
                                         (UINT16) rObject.GetTransferDirection() ;


     // output number of diffusant species 

     if ( XS_TYPE_VIRTUAL_DIFFUSION == rObject.GetType() )
     {
          rTOS << (UINT16) 2 << EOL;
     }
     else
     {
          rTOS << (UINT16) 1 << EOL;
     }


     // output rate laws 
     // for now we include the rate law exponent as 1.0; may change in future

     rTOS << IDType( XFER_STEP_RATE_LAW ) << rObject.GetSpeciesID() << (FLOAT64) 1.0 << EOL; 


     if ( XS_TYPE_VIRTUAL_DIFFUSION == rObject.GetType() )
     {
          rTOS << IDType( XFER_STEP_RATE_LAW ) << rObject.GetSpeciesID2() << (FLOAT64) 1.0 << EOL;
     }


     rTOS << IDType( XFER_STEP_FWD_ACT_PARMS ) << rObject.fwdA <<
      rObject.fwdM << rObject.fwdEa << EOL;
                         
     rTOS << IDType( XFER_STEP_REV_ACT_PARMS ) << rObject.revA <<
      rObject.revM << rObject.revEa << EOL;

     rTOS << IDType( END_TRANSFER_STEP_INIT ) << EOL;

     return rTOS;
}





BinaryInputStream& operator >> ( BinaryInputStream& rBIS, TransferStep& rObject )
{
     // first check the IDType

     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( TRANSFER_STEP ) )
     {
          GetpMainApp()->InputFileCorrupt( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );

          return rBIS;
     }
     UINT16 tempval;

     rBIS >> tempval;
     rObject.step_type = (enum XFER_STEP_TYPE ) tempval;  

     rBIS >> rObject.species_id;
     rBIS >> rObject.species_id2;

     rBIS >> tempval;
     rObject.rate_constant_format = (enum RATE_CONST_FORMAT) tempval;  

     rBIS >> tempval;
     rObject.is_modified = (BOOL8) tempval;

     rBIS >> tempval;
     rObject.transfer_direction = (enum XFER_DIRECTION) tempval;

     // String and NumericString values 

     rBIS >> rObject.name;           
     rBIS >> rObject.name2;           


     rBIS >> rObject.fwdA;       
     rBIS >> rObject.fwdM;
     rBIS >> rObject.fwdEa; 
                         
     rBIS >> rObject.revA;
     rBIS >> rObject.revM;
     rBIS >> rObject.revEa;

     return rBIS;
}




