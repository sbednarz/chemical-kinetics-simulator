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
//  NOTESTRG.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This module declares the NotebookString class, which provides the means of 
//  saving a character string in fixed length format to a file and for reading
//  a fixed length record from a file and 
//
//  CREATED : 1.25.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if !defined(__NOTESTRG_HXX__)
#define __NOTESTRG_HXX__

#include "vsimtype.hxx"
#include "binstrem.hxx"

const int DEFAULT_MAX_LENGTH = 5000;

class NotebookString : public String
{
private :
     UINT16        max_length; 

public :
     void SetMaxLength( const UINT16 NewLength ) { max_length = NewLength; }
     UINT16 GetMaxLength( ) const { return max_length; }

     // constructors

     NotebookString( ) : String(), max_length( DEFAULT_MAX_LENGTH ) { } 
     NotebookString( UINT16 Length ) : String(), max_length( Length ) { } 
     NotebookString( const String& rStr ): String( rStr ), max_length( DEFAULT_MAX_LENGTH ) {}
     NotebookString( const String& rStr, USHORT nPos, USHORT nLen ) : String( rStr, nPos, nLen ), max_length( DEFAULT_MAX_LENGTH ) {} 
     NotebookString( const char* pCharStr ): String( pCharStr ), max_length( DEFAULT_MAX_LENGTH ) {}
     NotebookString( const char* pCharStr, USHORT nLen ): String( pCharStr, nLen ), max_length( DEFAULT_MAX_LENGTH ) {}

     NotebookString( const NotebookString& rNoteStr ) : String( rNoteStr.GetStr() ), max_length( rNoteStr.GetMaxLength() ) { }


      // operators 

    NotebookString&   operator =  ( const NotebookString& rStr );
    NotebookString&   operator =  ( const String& rStr );

     friend BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const NotebookString& rValue );
};


BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, const NotebookString& rValue );

#endif

