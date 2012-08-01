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



enum XY_DATAFILE_RC { 
     NO_ERROR           =         0,
     NO_DATA_PTS        =  (1 << 0),
     INVALID_DATA       =  (1 << 1),
     XVALS_OUT_OF_ORDER =  (1 << 2),
     INIT_XVAL_NONZERO  =  (1 << 3),
     FILENAME_ERROR     =  (1 << 4),
     INVALID_Y_VAL      =  (1 << 5),
     TOO_MANY_PTS       =  (1 << 6),
     FILE_OPEN_ERROR    =  (1 << 7),
     FILE_ACCESS_ERROR  =  (1 << 8),
     FILE_TOO_LARGE     = (1 <<  9),
     FILE_READ_ERROR    = (1 << 10),
     MEM_ALLOC_ERROR    = (1 << 11),
     FILE_WRITE_ERROR   = (1 << 12),
     ILLEGAL_VALUE      = (1 << 13),
     DATA_FORMAT_ERROR  = ( NO_DATA_PTS | INVALID_DATA | XVALS_OUT_OF_ORDER | 
      INIT_XVAL_NONZERO | INVALID_Y_VAL ),
     SEVERE_ERROR       =  ( FILE_OPEN_ERROR  | FILE_ACCESS_ERROR | FILE_TOO_LARGE | 
      FILE_READ_ERROR | MEM_ALLOC_ERROR | FILE_WRITE_ERROR | ILLEGAL_VALUE | TOO_MANY_PTS )
};
     



class XyDataFile 
{
protected :
     String        text_buffer;
     String        file_name;
     BOOL8         not_read;

     FloatArray    rx;
     FloatArray    ry;

     enum XY_DATAFILE_RC return_code;

public :

     XyDataFile( );
     XyDataFile( const String& rFilename );

     void SetFilename( const String& rFilename ) { file_name = rFilename; not_read = TRUE; }
     const String& GetFilename( ) const { return file_name; }

     BOOL8 InitializeFromString( const String& rSource );

     BOOL8 WriteAllDataToTextStream( TextOutputStream& rTOS );
     BOOL8 WriteValidDataToTextStream( TextOutputStream& rTOS ); 

     const String& GetContents( ) const { return text_buffer; }

     enum XY_DATAFILE_RC GetStatus()  const {  return return_code; }

     const FloatArray& GetXData( ) const { return rx; }
     const FloatArray& GetYData( ) const { return ry; }

     void ShowErrorMsg( window* pOwner = NULL );

};

#endif

