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
//  MODULE  : specid.hxx
//
//             This header file declares and definec the class 
//             SPECIES_ID which is used to provide each individual 
//             species with a unique positive integer identifier
//
//  AUTHORS : Bill Hinsberg (wdh) and Frances Houle (fah)
// 
//  CHANGE HISTORY :
//        created Feb. 9 1994
//
// ----------------------------------------------------------------------------


#if !defined(__SPECID_HXX__)
#define  __SPECID_HXX__


#include "vsimtype.hxx"
#include "binstrem.hxx"
#include "txtstrem.hxx"


class SPECIES_ID 
{ 
private:
     UINT16     data;

public: 
     SPECIES_ID( ) { data = 0; }
     SPECIES_ID( UINT16 I ) { data = I; }

     BOOL operator == ( const SPECIES_ID& rID ) const
          { return ( data == rID.data ); }

     SPECIES_ID&  operator = ( const SPECIES_ID& rID )
          { data = rID.data; return *this; }

     UINT16 AsIndex( ) const { return data; }

friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SPECIES_ID& rObject );
friend BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SPECIES_ID& rObject );
friend TextOutputStream& operator << ( TextOutputStream& rTOS, const SPECIES_ID& rObject );

};

// added to allow for output of SPECIS_ID to 
// my BinaryOutputStream class

inline BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const SPECIES_ID& rObject )
{
     rBOS << rObject.data;
     return rBOS;
}


inline TextOutputStream& operator << ( TextOutputStream& rTOS, const SPECIES_ID& rObject )
{
     rTOS << rObject.data;
     return rTOS;
}


inline BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SPECIES_ID& rObject )
{
     rBIS >> rObject.data;
     return rBIS;
}


#endif

