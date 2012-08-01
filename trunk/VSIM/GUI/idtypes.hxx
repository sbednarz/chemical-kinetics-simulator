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
// VSIM - a Visual Kinetics Simulator
// ............................................................................
//
//  MODULE  : idtypes.hxx
//
//             This header file contains class definitions and #defines for
//             use in defining data structure types in files
//
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)
//
//  CHANGE HISTORY :
//        created Nov. 10 1994
//           two types added by fah 3/14/96
//           8 new types added by fah 9/20/96 - process area identifiers
//	     10 new types added by fah 1/18/97 - rate constant data
//
// ----------------------------------------------------------------------------


#if !defined(IDTYPES_HXX)
#define  IDTYPES_HXX

#include <tools.hxx>
#include "vsimtype.hxx"
#include "binstrem.hxx"
#include "txtstrem.hxx"

class TextOutputStream;
class TextInputStream;


enum REC_TYPE {

     // simulation engine record types

                    START_INPUT_FILE                = 'InpI',
                    START_APP_INIT                  = 'AppI',
                    PARENT_SCREEN_POSN              = 'PPos',
                    PARENT_SCREEN_SIZE              = 'PSiz',
                    END_APP_INIT                    = 'AppX',
                    START_SIM_INIT                  = 'SimI',
                    SIM_TIME_BASE                   = 'TimB',
                    RAND_NUM_SEED                   = 'RNSd',
                    OUT_FILE_NAME                   = 'OutF',
                    RESUME_FLAG                     = 'Resm',
                    NUM_PARTICLES                   = 'TotP',
                    MAX_EVENTS                      = 'MxEv',
                    WRITE_INTERVAL                  = 'WInt',
                    ELAPSED_TIME_LIMIT              = 'ETLm',
                    NUMBER_OF_COMPARTMENTS          = 'NumC',
                    NUMBER_OF_TRANSPORT_PATHS       = 'NumT',
                    NUM_SPECIES_IN_DATA_TABLE       = 'NumS',
                    EQUIL_DETECT_STATE              = 'EquO',
                    EQUIL_DETECT_MIN_EFF            = 'EquE',
                    EQUIL_DETECT_CYCLE_LEN          = 'EquC',
                    SIM_DATA_FILE_OFFSET            = 'DtFO',
                    SIM_STATUS_CODE_FILE_OFFSET     = 'ScFO',
                    START_SPECIES_DATA_TABLE_INIT   = 'SpDI',
                    SPECIES_DATA_TABLE_ENTRY        = 'SpDE',
                    END_SPECIES_DATA_TABLE_INIT     = 'SpDX',
                    END_SIM_INIT                    = 'SimX',
                    START_COMPARTMENT_INIT          = 'CmpI',
                    COMPARTMENT_TYPE_CONTAINER      = 'CTCn',  //new   
                    COMPARTMENT_TYPE_3D             = 'CT3d',  //new
                    COMPARTMENT_TYPE_INTERFACE      = 'CTIn',  //new
                    COMPARTMENT_INDEX               = 'CNdx',
                    COMPARTMENT_DIMENSIONS          = 'CmpD',
                    COMPARTMENT_PHASE               = 'CPha',  //new
                    COMPARTMENT_TEMPERATURE_OPTION  = 'TOpt',
                    COMPARTMENT_PRESSURE_OPTION     = 'POpt',
                    COMPARTMENT_VOLUME_OPTION       = 'VOpt',
                    COMPARTMENT_INIT_VOLUME         = 'CVol',
                    COMPARTMENT_INIT_PRESSURE       = 'CPrs',
                    COMPARTMENT_INIT_TEMP           = 'CTmp',
                    COMPARTMENT_NUM_RXNS            = 'CRxn',
                    COMPARTMENT_NUM_REVERSIBLE_RXNS = 'CRRx',
                    COMPARTMENT_NUM_SPECIES         = 'CSpc',
                    COMPARTMENT_NUM_XFER_OBJS       = 'CXfr',
                    COMPARTMENT_XFER_OBJ_INDEX      = 'CXIn',
                    COMPARTMENT_SPECIES_INFO        = 'SpcC',
                    START_RXN_INIT                  = 'RxnI',
                    RXN_DATA                        = 'RxnD',
                    RXN_RATE_CONSTANT_INIT          = 'RxKI', // new - k
          		RXN_RATE_CONSTANT_TYPE          = 'RxKt', // new - k
		          PROFILE_INIT		            = 'ProI', // new - k
		          NO_PROFILE_POINTS               = 'ProN', // new - k
          		PROFILE_DATA_PAIR               = 'ProD', // new - k
		          END_PROFILE_INIT                = 'ProX', // new - k
		          RXN_FWD_ACT_PARMS               = 'RxnF',
		          RXN_REV_ACT_PARMS               = 'RxnB',
		          END_RXN_RATE_CONSTANT_INIT      = 'RxKX', // new - k
		          RXN_FWD_RATE_LAW                = 'RxLF',
		          RXN_REV_RATE_LAW                = 'RxLB',
		          RXN_SPECIES_DATA                = 'RxnS',
		          END_RXN_INIT                    = 'RxnX',
		          END_COMPARTMENT_INIT            = 'CmpX',
		          START_TRANSFER_OBJECT_INIT      = 'XfOI',
		          TRANSFER_TYPE_XFER              = 'TTXf',  //new
		          TRANSFER_TYPE_DIFN              = 'TTDf',  //new
		          TRANSFER_TYPE_INTERFACE_DIFN    = 'TTId',  //new
		          TRANSFER_TYPE_INTERFACE_XFER    = 'TTIx',  //new
		          TRANSFER_OBJECT_INDEX           = 'TNdx',
		          TRANSFER_OBJECT_DIRECTION       = 'ToDI',
		          TRANSFER_OBJECT_DATA            = 'XfOD',
		          START_XFER_STEP_INIT            = 'XfSI',
                    XFER_MASS_DIFN                  = 'XfMD',  //new 7/98
                    XFER_MASS_TRANSFER              = 'XfMT',  //new 7/98
                    XFER_VIRT_DIFN                  = 'XfVD',  //new 7/98
		          XFER_STEP_DATA                  = 'XfSD',
		          XFER_RATE_CONSTANT_INIT         = 'XfKI', // new - k
		          XFER_RATE_CONSTANT_TYPE         = 'XfKt', // new - k
		          XFER_STEP_FWD_ACT_PARMS         = 'XfFP',
		          XFER_STEP_REV_ACT_PARMS         = 'XfRP',
		          END_XFER_RATE_CONSTANT_INIT     = 'XfKX', // new - k
		          XFER_STEP_RATE_LAW              = 'XfRL',
                    END_TRANSFER_STEP_INIT          = 'XfSX',
                    END_TRANSFER_OBJECT_INIT        = 'XfOX',
                    END_INPUT_FILE                  = 'InpX',
                                                    
     // GUI class types                             
                                                    
                    SCHEME_FRAME_WINDOW             = 'VSim',
                    SPECIES_DATABASE                = 'SpDB',
                    COMPARTMENT                     = 'Cmpt',
                    REACTION                        = 'Rxn ',
                    RXN_COMPONENT                   = 'RCmp',
                    TRANSFER_OBJECT                 = 'Xfer',
                    TRANSFER_STEP                   = 'XfSt',
                    SPECIES_LIST                    = 'SpLs',
                    SPECIES_LIST_ENTRY              = 'SpLE',
                    SPECIES_DB_ENTRY                = 'SpDE',
                    WRAPPED_TRANSFER_OBJECT         = 'WXfr',
                                                    
     //  Simulation results types                   
                                                    
                    COMPARTMENT_STATE               = 'CmpS',
                    SIMULATION_DATA                 = 'SimD',
                    SIMULATION_DATA_RECORD          = 'SmDR',
                                                    
                                                    
     // operating system types                      
                                                    
                    OPSYS_OS2                       =  'OS2 ',
                    OPSYS_DOS                       =  'DOS ',
                    OPSYS_MAC                       =  'MAC ',
                    OPSYS_AIX                       =  'AIX ',

     // misc. types                      
                                                    
                    NULL_IDTYPE                     =  'null'

};


#if defined(OS2)
const  REC_TYPE CURRENT_OS = OPSYS_OS2 ;
#endif

#if defined(WIN)
const  REC_TYPE CURRENT_OS = OPSYS_DOS ;
#endif

#if defined(MAC)
const  REC_TYPE CURRENT_OS = OPSYS_MAC ;
#endif

#if defined(AIX)
const  REC_TYPE CURRENT_OS = OPSYS_AIX ;
#endif

// class type

// -------------------------------------------------------------------
//               IDType
// ...................................................................
//
// Class definition for the class IDType. IDType is intended to
// encapsulate a 4 byte code, unique for each record type in a file
// which provides an unambiguous means of determining what type of
// data immediately follows. The idea here is that we can use the
// ID tag as either a string of characters ( e.g. "RXDT" for reaction
// data and manipulate is as a StarView String, a asciiz string, or
// a 4 byte unsigned long integer for as in the Macintosh resource and file
// typing mechanism for easy contruction of switch statements.
// In this case the string "RXDT" would also be represented as the unsigned
// long integer 'RXDT' == 52 58 44 54 hex == 1,381,516,372 dec in BigEndian
// format.
//
// -------------------------------------------------------------------

class IDType
{
private:

     union
     {
          CHAR8 as_str[5];
          REC_TYPE as_rec_type;
     }                         data;

public:

     // cast operators for extracting the ID in either
     // numeric (UINT32) or character format

     operator        const CHAR8 *() const { return data.as_str; };
     operator        REC_TYPE () const { return  data.as_rec_type; };

     // IO stream operators

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rOStream, const IDType& rID );
     friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, IDType& rID );

     friend TextOutputStream& operator << ( TextOutputStream& rOStream, const IDType& rID );
     friend TextInputStream& operator >> ( TextInputStream& rBIS, IDType& rID );


     // inequality and equality operators

     friend BOOL8  operator != ( const IDType& rID1,  const IDType& rID2  );
     friend BOOL8  operator == ( const IDType& rID1,  const IDType& rID2  );

     // constructors
                    IDType();
                    IDType( const String& rStr );
                    IDType( REC_TYPE );
                    IDType( const CHAR8 PTR );

};




BOOL8  operator != ( const IDType& rID1,  const IDType& rID2  );
BOOL8  operator == ( const IDType& rID1,  const IDType& rID2  );
BinaryOutputStream& operator << ( BinaryOutputStream& rOStream, const IDType& rID );
BinaryInputStream& operator >> ( BinaryInputStream& rBIS, IDType& rID );

TextOutputStream& operator << ( TextOutputStream& rOStream, const IDType& rID );
TextInputStream& operator >> ( TextInputStream& rBIS, IDType& rID );

#endif


