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
//  XYDATFIL.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This file declares the XyDataFile class, to be used for 
//  reading and writing x-y arrays in ASCII and binary format 
//
//  CREATED : 8.07.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if !defined(__XYDATFIL_HXX__)
     #define __XYDATFIL_HXX__

#include <sv.hxx>
#include "vsimtype.hxx"
#include "flotarry.hxx"
#include "idtypes.hxx"

class TextOutputStream;

typedef unsigned long XY_DATAFILE_RC;

const XY_DATAFILE_RC 
     NO_ERROR           =         0,
     NO_DATA_PTS        =  (1 << 0),
     INVALID_DATA       =  (1 << 1),
     XVALS_OUT_OF_ORDER =  (1 << 2),
     INIT_XVAL_NONZERO  =  (1 << 3),
     FILENAME_ERROR     =  (1 << 4),
     NEGATIVE_Y_VAL     =  (1 << 5),
     TOO_MANY_PTS       =  (1 << 6),
     FILE_OPEN_ERROR    =  (1 << 7),
     FILE_ACCESS_ERROR  =  (1 << 8),
     FILE_TOO_LARGE     = (1 <<  9),
     FILE_READ_ERROR    = (1 << 10),
     MEM_ALLOC_ERROR    = (1 << 11),
     FILE_WRITE_ERROR   = (1 << 12),
     ILLEGAL_VALUE      = (1 << 13),
     BUFFER_EMPTY       = (1 << 14),  
     BUFFER_INVALID     = (1 << 15),
     ZERO_Y_VAL         = (1 << 16),

     DATA_FORMAT_ERROR  = ( INVALID_DATA | XVALS_OUT_OF_ORDER | INIT_XVAL_NONZERO | 
      NEGATIVE_Y_VAL | ZERO_Y_VAL ),

     SEVERE_ERROR       =  ( NO_DATA_PTS | FILE_OPEN_ERROR  | FILE_ACCESS_ERROR | 
      FILE_TOO_LARGE | FILE_READ_ERROR | MEM_ALLOC_ERROR | FILE_WRITE_ERROR | 
      ILLEGAL_VALUE | TOO_MANY_PTS | BUFFER_INVALID ),

     ANY_ERROR          = ( DATA_FORMAT_ERROR | SEVERE_ERROR )
;
     



class XyDataFile 
{
protected :
     String        text_buffer;
     String        file_name;
     String        error_msg;
     BOOL8         mark_errors;
     UINT32        num_valid_xy_pairs;
     BOOL8         zero_y_vals_allowed;

     FloatArray    rx;
     FloatArray    ry;

     XY_DATAFILE_RC status_code;

     BOOL8            GetFileSize( UINT32& rSize );
     BOOL8            CreateBuffer( char** pBuffer, UINT32 Size );
     UINT16           Words( char* Str );
     char*            Strip( char* Str, char Type );
     void             AnalyzeAndStoreBuffer( char* pBuffer, char* Separator );
     void             DeleteNonPrintableChars( char* Source, char* Target );

     void             SetStatusFlag( XY_DATAFILE_RC Type ) { status_code |= Type; }
     void             ResetStatusFlags( ) { status_code = NO_ERROR; }
     void             GetValidDataInBinary( );

public :

      XyDataFile( );
     ~XyDataFile( );

     void            Reset();

     void            SetFilename( const String& rFilename ) { file_name = rFilename; status_code = BUFFER_INVALID; }
     const String&   GetFilename( ) const { return file_name; }
     UINT32          GetNumberOfValidDataPoints( ) const { return num_valid_xy_pairs; }
     Limits          GetXLimits( ) { return rx.GetLimits(); }
     Limits          GetYLimits( ) { return ry.GetLimits(); }

     void  MarkFormatErrors( BOOL8 NewValue ) { mark_errors = NewValue; }
     void  AllowZeroYValues( BOOL8 NewState ) { zero_y_vals_allowed = NewState; }

     BOOL8 InitializeFromString( const String& rSource );
     BOOL8 InitializeFromFile();

     void  WriteAllDataToTextStream( TextOutputStream& rTOS );
     void  WriteValidDataToTextStream( TextOutputStream& rTOS, 
                                       enum REC_TYPE     PtCountID = NULL_IDTYPE, 
                                       enum REC_TYPE     DataPairID = NULL_IDTYPE,
                                       FLOAT64           XMultiplier = 1.00,
                                       FLOAT64           YMultiplier = 1.00 );

     const String& GetContents( ) const { return text_buffer; }

     BOOL8 IsStatusFlagSet(XY_DATAFILE_RC Type )  const {  return ( status_code & Type) ; }

     const FloatArray& GetXData( ) const { return rx; }
     const FloatArray& GetYData( ) const { return ry; }

     const String& GetErrorMessage( );
};


#endif

