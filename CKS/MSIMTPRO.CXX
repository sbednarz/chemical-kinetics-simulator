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
/*  FILE NAME : msimtpro.cxx                                                */
/*                                                                          */
/*  This module defines the class TProfile which is an object containing    */
/*  an arbitrary temperature profile as a series of X,Y data pairs          */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for mutliple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Aug  1993                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimtprg.hxx"
#include "msimstrg.hxx"
#include "msimfile.hxx"

#include <string.h>
#include <ctype.h>


#if defined(__OS2__)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MSDOS__)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\r\n"
#endif

#if defined(__AIX__)
#define FILE_END_OF_LINE     "\n"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MAC__)

#if defined(__PPC__)

#include <sysdep.hxx>

#define FILE_END_OF_LINE   "\r\n\15"

#else
#define FILE_END_OF_LINE   "\r\n\xOD"
#endif
#define MLE_END_OF_LINE    "\n"
#endif




TProfile::TProfile( ) :
text_buffer ( msimNULL_STR )
{
     num_valid_xy_pairs = 0;
     maxX = minX = maxY = minY = 0.0;
}

msimTPRO_RC TProfile::AnalyzeAndStoreBuffer( PCHAR pBuffer, PCHAR Separator )
{
     PCHAR ptr;
     msimFLOAT time, temperature, prev_time;
     msimBOOL first_pt_acquired = FALSE;
     msimSTRING temp_buf;
     msimTPRO_RC rc = msimTPRO_NO_ERROR;

     String fmt_err_mark( CONVERSION_ERROR_MARKER );
     String time_err_mark( TIME_ERROR_MARKER );
     String temp_err_mark( TEMP_ERROR_MARKER );
     String end_of_line( MLE_END_OF_LINE );


     // here we retrieve each line from the buffer at pBuffer sequentially
     // after retrieval we check that 1. exactly two valid numeric values
     // are given on each line and 2. that the first value (== time)
     // is greater than the previous time value
     // if either is not then mark that line in the String text_buffer
     // and return code indicating errors found

     prev_time = minX = maxX = minY = maxY = 0.0;
     num_valid_xy_pairs = 0;

     text_buffer = String( msimNULL_STR );

     ptr = strtok( pBuffer, Separator );

     // we need to acquire an initial set of valid time,temperature pts
     // before preceding - this is upposed to achieve that

     while ( ! first_pt_acquired )
     {
          if ( ! ptr )        // finished tokenizing , failed to find any points
               return ( rc | msimTPRO_NO_DATA_PTS );

          // eliminate non-printing characters

          msimDeleteNonPrintableChars( ptr, ptr );

          // got a comment line or blank line, so just add to buffer but do not attempt
          // conversion to binary

          if ( *ptr == COMMENT_CHAR || msimWords( ptr ) == 0 )
          {
               text_buffer += ( String( ptr ) + end_of_line );

               ptr = strtok( NULL, Separator );
          }
          else
          {
               // analyze for conversion

               msimStrip( ptr, 'T' );  // remove trailing whitespace

               if ( 2 == sscanf( ptr, " %lf %lf%s", &time, &temperature, temp_buf ) )
               {
                    // successful conversion, we may have initial data point.
                    // Validate first

                    if ( temperature <= 0.0 )
                    {
                         rc |= msimTPRO_INVALID_TEMP_VALUE;
                         text_buffer += ( temp_err_mark + String( ptr ) + end_of_line );
                         ptr = strtok( NULL, Separator );
                    }
                    else
                    {
                         if ( time != 0.0 )// be sure t0 == 0.0; error if not
                              rc |= msimTPRO_INIT_TIME_NONZERO;

                         prev_time = time;
                         minX = maxX = time;
                         minY = maxY = temperature;
                         num_valid_xy_pairs = 1;
                         text_buffer += ( String( ptr ) + end_of_line );
                         first_pt_acquired = TRUE;
                    }
               }
               else
               {
                    rc |= msimTPRO_INVALID_DATA;
                    text_buffer += ( fmt_err_mark + String( ptr ) + end_of_line );
                    ptr = strtok( NULL, Separator );
               }
          }

     }

     // now work through rest of buffer

     while ( NULL != ( ptr = strtok( NULL, Separator ) ) )
     {
          msimDeleteNonPrintableChars( ptr, ptr );

          // skip processing comments and blank lines - just append to buffer

          if ( *ptr == COMMENT_CHAR || msimWords( ptr ) == 0 )
               text_buffer += ( String( ptr ) + end_of_line );
          else
          {
               // analyze for conversion

               msimStrip( ptr, 'T' );  // remove trailing whitespace

               if ( 2 == sscanf( ptr, " %lf %lf%s", &time, &temperature, temp_buf ) )
               {
                    if ( temperature <= 0.0 )
                    {
                         rc |= msimTPRO_INVALID_TEMP_VALUE;
                         text_buffer += ( temp_err_mark + String( ptr ) + end_of_line );
                    }
                    else
                    {
                         if ( time > prev_time )
                         {
                              // the data pair is valid

                              prev_time = time;

                              // adjust min/max if necessary

                              if ( maxX < time )
                                   maxX = time;
                              if ( maxY < temperature )
                                   maxY = temperature;
                              if ( minX > time )
                                   minX = time;
                              if ( minY > temperature )
                                   minY = temperature;

                              if ( ++num_valid_xy_pairs > MAX_TPRO_NUM_DATA_PTS )
                                   rc |= msimTPRO_TOO_MANY_PTS;

                              text_buffer += ( String( ptr ) + end_of_line );
                         }
                         else
                         {
                              rc |= msimTPRO_TIME_ORDER_ERROR;
                              text_buffer +=
                              ( time_err_mark + String( ptr ) + end_of_line );
                         }
                    }
               }
               else
               {
                    rc |= msimTPRO_INVALID_DATA;
                    text_buffer +=
                    ( fmt_err_mark + String( ptr ) + end_of_line );
               }
          }
     }

     // done tokenizing - return possibly non-zero rc

     return rc;
}

msimTPRO_RC TProfile::InitializeFromString( const String& rSource )
{
     PCHAR pbuffer;
     msimTPRO_RC rc;

     if ( ! CreateBuffer( &pbuffer, rSource.Len( ) + 1 ) )
          return msimTPRO_MEM_ALLOC_ERROR;

     strcpy( pbuffer, rSource );

     rc = AnalyzeAndStoreBuffer( pbuffer, MLE_END_OF_LINE );

     delete [] pbuffer;

     return rc;
}

msimTPRO_RC TProfile::InitializeFromFile( PCHAR Filename )
{
     ULONG file_size;
     FILE PTR fileptr;
     PCHAR pbuffer;
     msimTPRO_RC rc;

     // open file and get file size

     if ( ! msimGetSizeOfFile( Filename, &file_size ) )
          return msimTPRO_FILE_ACCESS_ERROR;

     if ( NULL == ( fileptr = fopen( Filename, "rb" ) ) )
          return msimTPRO_FILE_OPEN_ERROR;

     if ( file_size > MAX_TPRO_FILE_SIZE )
          return msimTPRO_FILE_TOO_LARGE;

     // now prepare a buffer to hold file contents

     if ( ! CreateBuffer( &pbuffer, file_size + 1 ) )
          return msimTPRO_MEM_ALLOC_ERROR;

     aMainApp.Wait( TRUE );

     fread( pbuffer, file_size, 1, fileptr );

     // terminate buffer with binary zero

     pbuffer[file_size] = '\0';

     //scan back from end replacing non-printing chars ( eg EOF under DOS )

     for ( PCHAR ptr = pbuffer + file_size;
               ( ptr >= pbuffer ) && ! isprint( *ptr ) && ! isspace( *ptr );
               --ptr
          )
          *ptr = '\0';

     if ( ferror( fileptr ) )
     {
          aMainApp.Wait( FALSE );

          fclose( fileptr );
          delete [] pbuffer;
          return msimTPRO_FILE_READ_ERROR;
     }

     aMainApp.Wait( FALSE );

     fclose( fileptr );

     rc = AnalyzeAndStoreBuffer( pbuffer, FILE_END_OF_LINE );

     delete [] pbuffer;

     return rc;
}

msimBOOL TProfile::CreateBuffer( PCHAR PTR ppBuffer, size_t Size )
{
     PCHAR temp_ptr;

     temp_ptr = new CHAR[Size];

     if ( ! temp_ptr )
          return FALSE;
     else
     {
          *ppBuffer = temp_ptr;
          return TRUE;
     }

}


msimTPRO_RC TProfile::WriteContentsToFile( PCHAR Filename )
{
     FILE PTR fileptr;
     msimTPRO_RC rc;
     PCHAR pbuffer;

     if ( ! CreateBuffer( &pbuffer, text_buffer.Len( ) + 1 ) )
          return msimTPRO_MEM_ALLOC_ERROR;

     if ( NULL == ( fileptr = fopen( Filename, "w" ) ) )
     {
          delete [] pbuffer;
          return msimTPRO_FILE_OPEN_ERROR;
     }

     aMainApp.Wait( TRUE );

     strcpy( pbuffer, text_buffer );
     rc = msimSaveEditorTextToFile( pbuffer, fileptr );

     delete [] pbuffer;
     aMainApp.Wait( FALSE );

     fclose( fileptr );
	 
#if defined(__MAC__) && defined(__PPC__)

     Sysdepen::SetFileInfo( String( Filename ), String( msimTEXTFILE_TYPE),
                  String( msimDEFAULT_CREATOR_NAME ) );

#endif

     return rc;
}


msimTPRO_RC TProfile::GetValidDataInBinary( msimPFLOAT pTime, msimPFLOAT pTemperature )
{
     PCHAR pbuffer;
     PCHAR ptr;

     // at this point all lines that do not convert to two floating pt numbers
     // are marked with a COMMENT_CHAR in column 1 so this is easy
     // We just scan through and convert all lines to binary representation

     if ( ! pTime || ! pTemperature )
          return msimTPRO_ILLEGAL_VALUE;

     if ( num_valid_xy_pairs == 0 )
          return msimTPRO_NO_DATA_PTS;

     if ( ! CreateBuffer( &pbuffer, text_buffer.Len( ) + 1 ) )
          return msimTPRO_MEM_ALLOC_ERROR;

     strcpy( pbuffer, text_buffer );

     ptr = strtok( pbuffer, MLE_END_OF_LINE );

     // scan through, skip comments, convert and store all others
     // work through pbuffer

     while ( ptr )
     {
          if ( *ptr != COMMENT_CHAR &&
                    msimWords( ptr ) == 2 )
               sscanf( ptr, " %lf %lf ", pTime++, pTemperature++ );

          ptr = strtok( NULL, MLE_END_OF_LINE );
     }

     delete [] pbuffer;

     return msimTPRO_NO_ERROR;
}

msimTPRO_RC TProfile::WriteValidDataToTextFile( FILE PTR pFile, USHORT TimeUnits )
{
     PCHAR pbuffer;
     PCHAR ptr;
     msimFLOAT time, temperature;

     msimFLOAT conversion_factor = msimConvFactorTimeToSec ( TimeUnits );

     // at this point all lines that do not convert to two floating pt numbers
     // are marked with a COMMENT_CHAR in column 1 so this is easy
     // We just scan through and convert all lines to binary representation

     if ( ! pFile )
          return msimTPRO_ILLEGAL_VALUE;

     if ( num_valid_xy_pairs == 0 )
          return msimTPRO_NO_DATA_PTS;

     if ( ! CreateBuffer( &pbuffer, text_buffer.Len( ) + 1 ) )
          return msimTPRO_MEM_ALLOC_ERROR;

     strcpy( pbuffer, text_buffer );

     ptr = strtok( pbuffer, MLE_END_OF_LINE );

     // scan through, skip comments, convert and store all others
     // work through pbuffer

     while ( ptr )
     {
          if ( *ptr != COMMENT_CHAR &&
                    msimWords( ptr ) == 2 )
          {
               sscanf( ptr, " %lf %lf ", &time, &temperature );

               // convert time to expected units and write to file
               // make format exponential and wide

               fprintf( pFile, "%-18.14E %-18.14E\n",
                    time * conversion_factor, temperature );
          }

          ptr = strtok( NULL, MLE_END_OF_LINE );
     }

     delete [] pbuffer;

     return ( ferror( pFile ) ? msimTPRO_FILE_WRITE_ERROR : msimTPRO_NO_ERROR );
}



void TProfile::ShowErrorMsg( PCHAR Name, msimTPRO_RC ErrorCode,
          msimBOOL CurrentState, msimWID Owner )
{
     if ( ErrorCode == msimTPRO_NO_ERROR )
          return;

     String Str( ResId( msimTPRO_ERR_MSG1 ) );

     Str += msimBaseFilename( Name );
     Str += "\" :";


     // filename error

     if ( ErrorCode == msimTPRO_FILENAME_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG2 ) );

     // severe errors

     if ( ErrorCode & msimTPRO_FILE_OPEN_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG3 ) );

     if ( ErrorCode & msimTPRO_FILE_ACCESS_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG4 ) );

     if ( ErrorCode & msimTPRO_FILE_TOO_LARGE )
          Str += String( ResId( msimTPRO_ERR_MSG5 ) );

     if ( ErrorCode & msimTPRO_FILE_READ_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG6 ) );

     if ( ErrorCode & msimTPRO_MEM_ALLOC_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG7 ) );

     if ( ErrorCode & msimTPRO_FILE_WRITE_ERROR )
          Str += String( ResId( msimTPRO_ERR_MSG8 ) );

     if ( ErrorCode & msimTPRO_ILLEGAL_VALUE )
          Str += String( ResId( msimTPRO_ERR_MSG9 ) );

     if ( ErrorCode & msimTPRO_TOO_MANY_PTS )
          Str += String( ResId( msimTPRO_ERR_MSG14 ) );

     // data format errors

     if ( ErrorCode & msimTPRO_NO_DATA_PTS )
          Str += String( ResId( msimTPRO_ERR_MSG10 ) );

     if ( ErrorCode & msimTPRO_INIT_TIME_NONZERO )
          Str += String( ResId( msimTPRO_ERR_MSG11 ) );

     if ( ErrorCode & msimTPRO_INVALID_TEMP_VALUE )
     {
          Str += String( ResId( msimTPRO_ERR_MSG16 ) );

          if ( CurrentState == msimTPRO_EDIT_PROFILE )
          {
               Str += ( String( ResId( msimTPRO_ERR_MSG15 ) ) +
                    String( TEMP_ERROR_MARKER ) );
          }
     }

     if ( ErrorCode & msimTPRO_INVALID_DATA )
     {
          Str += String( ResId( msimTPRO_ERR_MSG12 ) );

          if ( CurrentState == msimTPRO_EDIT_PROFILE )
          {
               Str += ( String( ResId( msimTPRO_ERR_MSG15 ) ) +
                    String( CONVERSION_ERROR_MARKER ) );
          }

     }

     if ( ErrorCode & msimTPRO_TIME_ORDER_ERROR )
     {
          Str += String( ResId( msimTPRO_ERR_MSG13 ) );

          if ( CurrentState == msimTPRO_EDIT_PROFILE )
          {
               Str += ( String( ResId( msimTPRO_ERR_MSG15 ) ) +
                    String( TIME_ERROR_MARKER ) );
          }

     }

     // append line about simulation not starting if needed

     if ( CurrentState == msimTPRO_STARTING_SIMULATION )
          Str += String( ResId( msimSIM_CANNOT_START_STR ) );


     // all done - show the msg box

     WarningBox( Owner, WB_OK | WB_DEF_OK, Str ) .Execute( );

     return;
}


