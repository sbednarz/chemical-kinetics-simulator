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
/*  FILE NAME : msimtprg.hxx                                                */
/*                                                                          */
/*  This module contains declarations for the various temperature program-  */
/*  ming dialogs used in MSIM                                               */
/*                                                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#define TIME_ERROR_MARKER          "*>>>"
#define CONVERSION_ERROR_MARKER    "*-->"
#define TEMP_ERROR_MARKER          "***>"
#define COMMENT_CHAR               msimCOMMENT_CHAR
#define MAX_TPRO_FILE_SIZE         32000
#define MAX_TPRO_NUM_DATA_PTS      8000
#define NO_FILENAME                "<none selected>"

#define msimTPRO_STARTING_SIMULATION   TRUE
#define msimTPRO_EDIT_PROFILE          FALSE

// the following are used for constructing msimTPRO_RC values

typedef USHORT msimTPRO_RC;

#define  msimTPRO_NO_ERROR                ( (USHORT) ( 0 )     )
#define  msimTPRO_NO_DATA_PTS             ( (USHORT) (1 << 0)  )
#define  msimTPRO_INVALID_DATA            ( (USHORT) (1 << 1)  )
#define  msimTPRO_TIME_ORDER_ERROR        ( (USHORT) (1 << 2)  )
#define  msimTPRO_INIT_TIME_NONZERO       ( (USHORT) (1 << 3)  )
#define  msimTPRO_FILENAME_ERROR          ( (USHORT) (1 << 4)  )
#define  msimTPRO_INVALID_TEMP_VALUE      ( (USHORT) (1 << 5)  )

#define  msimTPRO_TOO_MANY_PTS            ( (USHORT) (1 << 7)  )
#define  msimTPRO_FILE_OPEN_ERROR         ( (USHORT) (1 << 8)  )
#define  msimTPRO_FILE_ACCESS_ERROR       ( (USHORT) (1 << 9)  )
#define  msimTPRO_FILE_TOO_LARGE          ( (USHORT) (1 << 10) )
#define  msimTPRO_FILE_READ_ERROR         ( (USHORT) (1 << 11) )
#define  msimTPRO_MEM_ALLOC_ERROR         ( (USHORT) (1 << 12) )
#define  msimTPRO_FILE_WRITE_ERROR        ( (USHORT) (1 << 13) )
#define  msimTPRO_ILLEGAL_VALUE           ( (USHORT) (1 << 14) )

#define  msimTPRO_DATA_FORMAT_ERROR (msimTPRO_NO_ERROR | msimTPRO_NO_DATA_PTS | msimTPRO_INVALID_DATA \
             | msimTPRO_TIME_ORDER_ERROR | msimTPRO_INIT_TIME_NONZERO | msimTPRO_INVALID_TEMP_VALUE )

#define  msimTPRO_SEVERE_ERROR  (msimTPRO_FILE_OPEN_ERROR  | msimTPRO_FILE_ACCESS_ERROR \
             | msimTPRO_FILE_TOO_LARGE | msimTPRO_FILE_READ_ERROR | msimTPRO_MEM_ALLOC_ERROR  \
             | msimTPRO_FILE_WRITE_ERROR | msimTPRO_ILLEGAL_VALUE | msimTPRO_TOO_MANY_PTS )

// defined in msimtpgp.cxx

void msimLinearTempProgDlg( msimWID Owner, msimPINSTANCE pInstance );


// defined in msimetpp.cxx

void msimExternalTempProgDlg( msimWID Owner, msimPINSTANCE pInstance );

// defined in msimtemp.cxx

void msimVarTempDlg( msimWID Owner, msimPINSTANCE pInstance );
void msimConstTempDlg( msimWID Owner, msimPINSTANCE pInstance );



// defined in msimtpro.cxx

class TProfile
{
protected :
     String text_buffer;
     size_t num_valid_xy_pairs;
     msimFLOAT maxX, minX;
     msimFLOAT maxY, minY;

     msimTPRO_RC AnalyzeAndStoreBuffer( PCHAR pBuffer, PCHAR Separator );

     msimBOOL CreateBuffer( PCHAR PTR ppBuffer, size_t Size );

public :
     TProfile( );

     msimTPRO_RC InitializeFromFile( PCHAR Filename );
     msimTPRO_RC WriteContentsToFile( PCHAR Filename );
     msimTPRO_RC GetValidDataInBinary( msimPFLOAT pTime, msimPFLOAT pTemperature );
     msimTPRO_RC WriteValidDataToTextFile( FILE PTR pFile, USHORT TimeUnits );
     msimTPRO_RC InitializeFromString( const String& rSource );
     void ShowErrorMsg( PCHAR Name, msimTPRO_RC ErrorCode,
          msimBOOL CurrentState, msimWID Owner );

     String& GetContents( )
     {
          return text_buffer;
     };
     size_t GetNumberOfValidDataPoints( )
     {
          return num_valid_xy_pairs;
     };
     msimFLOAT GetMinX( )
     {
          return minX;
     };
     msimFLOAT GetMaxX( )
     {
          return maxX;
     };
     msimFLOAT GetMinY( )
     {
          return minY;
     };
     msimFLOAT GetMaxY( )
     {
          return maxY;
     };

};

