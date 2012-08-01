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
//  XYDATFIL.CXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This file declares the XyDataFile class, to be used for 
//  reading and writing x-y arrays in ASCII and binary format 
//
//  CREATED : 8.07.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "xydatfil.hxx"
#include "txtstrem.hxx"
#include "mainapp.hxx"

#endif

#include "strings.hrc"

#include <string.h>
#include <stdio.h>
#include <ctype.h>





#if defined(__OS2__)
     #define FILE_END_OF_LINE     "\r"
     #define MLE_END_OF_LINE      "\n"
#endif

#if defined(__MSDOS__)
     #define FILE_END_OF_LINE     "\r"
     #define MLE_END_OF_LINE      "\r\n"
#endif

#if defined(__AIX__)
     #define FILE_END_OF_LINE     "\n"
     #define MLE_END_OF_LINE      "\n"
#endif

#if defined(__MAC__)
     #define FILE_END_OF_LINE     "\r\n\xOD"
     #define MLE_END_OF_LINE      "\n"
#endif



const char    COMMENT_CHAR           = '*';
const UINT32  MAX_FILE_SIZE          = 32000;
const UINT32  MAX_NUM_DATA_PTS       = 8000;
const UINT32  TEMP_BUFFER_SIZE       = 1024;

const char* X_ERROR_MARKER       = "*>>>";
const char* CONVERSION_ERROR_MARKER = "*-->";
const char* Y_ERROR_MARKER       = "***>";

String FMT_ERR_MARK( CONVERSION_ERROR_MARKER );
String X_ERR_MARK( X_ERROR_MARKER );
String Y_ERR_MARK( Y_ERROR_MARKER );

// ----------------------------------------------------------------------------
//                     XyDataFile::XyDataFile( )
// ............................................................................
// 
//   constructor 
//
// ----------------------------------------------------------------------------

XyDataFile::XyDataFile( ) :
text_buffer(),
file_name(),
error_msg(),
mark_errors( FALSE ),
num_valid_xy_pairs( 0 ),
zero_y_vals_allowed( FALSE ),
rx(),
ry(),
status_code( BUFFER_EMPTY )
{
}




// ----------------------------------------------------------------------------
//                     XyDataFile::~XyDataFile( )
// ............................................................................
// 
//   destructor 
//
// ----------------------------------------------------------------------------

XyDataFile::~XyDataFile( )
{
}





// ----------------------------------------------------------------------------
//                     XyDataFile::Reset( )
// ............................................................................
// 
//   restores to default condition
//
// ----------------------------------------------------------------------------

void XyDataFile::Reset( )
{
     error_msg          = file_name = text_buffer = String();                        
     num_valid_xy_pairs = 0;                                                  
                                                                              
     rx.Initialize( 0 );                                                      
     ry.Initialize( 0 );                                                      
}                                                                             







// ----------------------------------------------------------------------------
//                     XyDataFile::InitializeFromString()
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 XyDataFile::InitializeFromString( const String& rSource )
{
     ResetStatusFlags( );

     char* pbuffer;

     if ( ! CreateBuffer( &pbuffer, rSource.Len( ) + 1 ) )
     {
          return FALSE;
     }

     strcpy( pbuffer, rSource );

     AnalyzeAndStoreBuffer( pbuffer, MLE_END_OF_LINE );

     delete [] pbuffer;

     return !IsStatusFlagSet( SEVERE_ERROR );
}








// ----------------------------------------------------------------------------
//                     XyDataFile::GetFileSize( )
// ............................................................................
// 
//
//
// ----------------------------------------------------------------------------

BOOL8 XyDataFile::GetFileSize( UINT32& rSize )
{

     DirEntry file_entry( file_name );

     file_entry.ToAbs( );

     FileStat fs( file_entry );

     UINT32 size = fs.GetSize ( );

     if ( FSYS_ERR_OK == fs.GetError( ) )
     {
          rSize = size;
          return TRUE;
     }
     else
          return FALSE;
}







// ----------------------------------------------------------------------------
//                     XyDataFile::CreateBuffer()
// ............................................................................
// 
//   returns TRUE if initialization successful
//   
//
// ----------------------------------------------------------------------------

BOOL8 XyDataFile::CreateBuffer( char** pBuffer, UINT32 Size )
{
     char* temp_ptr;

     temp_ptr = new char[Size];

     if ( ! temp_ptr )
     {          
          SetStatusFlag( MEM_ALLOC_ERROR );
          return FALSE;
     }
     else
     {
          *pBuffer = temp_ptr;
          return TRUE;
     }
}





// ----------------------------------------------------------------------------
//                     XyDataFile::InitializeFromFile( )
// ............................................................................
// 
//   returns TRUE if initialization successful
//   
//
// ----------------------------------------------------------------------------

BOOL8 XyDataFile::InitializeFromFile( )
{
     ResetStatusFlags( );

     // does file exist??

     if ( !FileExists( file_name) )
     {
          SetStatusFlag( FILENAME_ERROR );
          return FALSE;
     }

     // get file size

     UINT32 file_size;

     if ( ! GetFileSize( file_size ) )
     {
          SetStatusFlag( FILE_ACCESS_ERROR );
          return FALSE;
     }

     if ( file_size > MAX_FILE_SIZE )
     {
          SetStatusFlag( FILE_TOO_LARGE );
          return FALSE;
     }

     // go ahead and attempt open 

     FILE* fileptr;

     if ( NULL == ( fileptr = fopen( file_name, "rb" ) ) )
     {
          SetStatusFlag( FILE_OPEN_ERROR );
          return FALSE;
     }

     // now prepare a buffer to hold file contents

     char* pbuffer;

     if ( ! CreateBuffer( &pbuffer, file_size + 1 ) )
     {
          return FALSE;
     }

     // turn on 'waiting' pointer on screen 

     GetpMainApp()->Wait( TRUE );

     // read in the file 

     fread( pbuffer, file_size, 1, fileptr );

     GetpMainApp()->Wait( FALSE );

     if ( ferror( fileptr ) )
     {
          fclose( fileptr );
          delete [] pbuffer;
          SetStatusFlag( FILE_READ_ERROR );
          return FALSE;
     }
     else
     {
          // terminate buffer with binary zero

          pbuffer[file_size] = '\0';

          // now scan back from end of buffer, replacing non-printing chars 
          // ( eg EOF under DOS ) along the way 

          for ( char* ptr = pbuffer + file_size; 
                ( ptr >= pbuffer ) && ! isprint( *ptr ) && ! isspace( *ptr ); 
                --ptr 
              )
               *ptr = '\0';

          // buffer is in consistent shape now - parse and store valid 
          // x/y data pairs in the buffer 

          AnalyzeAndStoreBuffer( pbuffer, FILE_END_OF_LINE );

          delete [] pbuffer;

          fclose( fileptr );

          return !IsStatusFlagSet( SEVERE_ERROR );
     }
}





//----------------------------------------------------------------------------
//               XyDataFile::DeleteNonPrintableChars( )
//............................................................................
//                                                                          
// copies a string from source to target, deleting any characters that do   
// not correspond to printable characters along the way. The tab character  
// is an exception, being copied over anyway                                
// returns nothing                                                          
// Source and Target can point to the same location, which leads to deleting
// the nonprinting chars on the string in place                             
//                                                                          
//----------------------------------------------------------------------------

void XyDataFile::DeleteNonPrintableChars( char* Source, char* Target )
{
     while ( *Source )
     {
          if ( isprint( *Source ) || *Source == '\t' )
               *Target++= *Source;

          Source++;
     }
     *Target = '\0';
}









//--------------------------------------------------------------------------
//                            XyDataFile::Words( )
//..........................................................................
//                                                                          
// returns the number of blank-delimited words in an ASCIIZ string          
//                                                                          
//--------------------------------------------------------------------------

UINT16 XyDataFile::Words( char* Str )
{
     UINT16 retval = 0;

     // scan to first word        

     while ( isspace( *Str ) )         
          Str++;

     while ( isgraph( *Str ) )
     {
          retval++;
          while ( isgraph( *Str ) )
               Str++;
          while ( isspace( *Str ) )
               Str++;
     }            

     return retval;
}







//--------------------------------------------------------------------------
//                            XyDataFile::Strip( )
//..........................................................................
//                                                                          
// removes leading, trailing or both leading and trailing blanks from a     
// zero-terminated string. Puts the reulst back in the buffer pointed to    
// by Str.                                                                  
//                                                                          
// The parameter Type is either 'L','T' or 'B' upper or lower case          
// which stands for 'leading', 'trailing' or 'both'                         
//                                                                          
// returns ptr to the original string                                       
//                                                                          
//--------------------------------------------------------------------------

char* XyDataFile::Strip( char* Str, char Type )
{
     char* tmp1;
     char* tmp2;

     UINT32 length;

     Type = toupper( Type );

     switch ( Type )
     {
     case 'L' :
     case 'B' :

          tmp1 = tmp2 = Str;

          // scan to first word 

          while ( isspace( *tmp1 ) )   
               tmp1++;

          // now move all chars forward in array 

          while ( *tmp1 )              
               *tmp2++= *tmp1++;

          // and terminate with zero

          *tmp2 = '\0';                

          if ( Type == 'L' )
               break;

     case 'T' :

          length = strlen( Str );      // added to check for zero-length string

          if ( length )
          {
               // point to end of string 

               tmp1 = Str + length - 1;

               // scan backward until a nonspace char is found

               while ( isspace( *tmp1 ) )
                    tmp1--;

               // place a terminal zero after it

               * ( ++tmp1 ) = '\0';    
          }
          break;

     default :

          ASSERT( 0 );

     }

     return Str;
}






//----------------------------------------------------------------------------
//               XyDataFile::AnalyzeAndStoreBuffer( )
//............................................................................
//                                                                          
//                                                                          
//----------------------------------------------------------------------------

void XyDataFile::AnalyzeAndStoreBuffer( char* pBuffer, char* Separator )
{
     String fmt_err_mark( COMMENT_CHAR );
     String x_err_mark( COMMENT_CHAR ); 
     String y_err_mark( COMMENT_CHAR ); 

     if ( mark_errors )
     {
          fmt_err_mark = FMT_ERR_MARK;
          x_err_mark   = X_ERR_MARK;
          y_err_mark   = Y_ERR_MARK;
     }

     char* ptr;

     FLOAT64 x;
     FLOAT64 y;
     FLOAT64 prev_x;

     BOOL8 first_pt_acquired = FALSE;

     char temp_buf[ TEMP_BUFFER_SIZE ];

     String end_of_line( MLE_END_OF_LINE );

     // here we retrieve each line from the buffer at pBuffer sequentially
     // after retrieval we check that 1. exactly two valid numeric values
     // are given on each line and 2. that the first value (== x)
     // is greater than the previous x value
     // if either is not then mark that line in the String text_buffer
     // and return code indicating errors found

     prev_x = 0.0;
     num_valid_xy_pairs = 0;

     text_buffer = String( );

     ptr = strtok( pBuffer, Separator );

     // we need to acquire an initial set of valid x,y pts
     // before preceding - this is upposed to achieve that

     while ( ! first_pt_acquired )
     {
          if ( ! ptr )        // finished tokenizing , failed to find any points
          {
               SetStatusFlag( NO_DATA_PTS );
               return;
          }

          // eliminate non-printing characters

          DeleteNonPrintableChars( ptr, ptr );

          // got a comment line or blank line, so just add to buffer but do not attempt
          // conversion to binary

          if ( *ptr == COMMENT_CHAR || Words( ptr ) == 0 )
          {
               text_buffer += ( String( ptr ) + end_of_line );

               ptr = strtok( NULL, Separator );
          }
          else
          {
               // analyze for conversion

               Strip( ptr, 'T' );  // remove trailing whitespace

               if ( 2 == sscanf( ptr, " %lf %lf%s", &x, &y, temp_buf ) )
               {
                    // successful conversion, we may have initial data point.
                    // Validate first

                    if ( y < 0.0 )
                    {
                         SetStatusFlag( NEGATIVE_Y_VAL );
                         text_buffer += ( y_err_mark + String( ptr ) + end_of_line );
                         ptr = strtok( NULL, Separator );
                    }
                    else
                    {
                         if ( y == 0.0 && ( FALSE == zero_y_vals_allowed ) )
                         {
                              SetStatusFlag( ZERO_Y_VAL );
                              text_buffer += ( y_err_mark + String( ptr ) + end_of_line );
                              ptr = strtok( NULL, Separator );
                         }
                         else
                         {
                              if ( x != 0.0 )     // be sure x0 == 0.0; error if not
                                   SetStatusFlag( INIT_XVAL_NONZERO );
     
                              prev_x = x;
                              num_valid_xy_pairs = 1;
                              text_buffer += ( String( ptr ) + end_of_line );
                              first_pt_acquired = TRUE;
                         }
                    }
               }
               else
               {
                    SetStatusFlag( INVALID_DATA );
                    text_buffer += ( fmt_err_mark + String( ptr ) + end_of_line );
                    ptr = strtok( NULL, Separator );
               }
          }

     }

     // now work through rest of buffer

     while ( NULL != ( ptr = strtok( NULL, Separator ) ) )
     {
          DeleteNonPrintableChars( ptr, ptr );

          // skip processing comments and blank lines - just append to buffer

          if ( *ptr == COMMENT_CHAR || Words( ptr ) == 0 )
               text_buffer += ( String( ptr ) + end_of_line );
          else
          {
               // analyze for conversion

               Strip( ptr, 'T' );  // remove trailing whitespace

               if ( 2 == sscanf( ptr, " %lf %lf%s", &x, &y, temp_buf ) )
               {
                    if ( y < 0.0 )
                    {
                         SetStatusFlag( NEGATIVE_Y_VAL );
                         text_buffer += ( y_err_mark + String( ptr ) + end_of_line );
                    }
                    else
                    {
                         if ( y == 0.0 && ( FALSE == zero_y_vals_allowed ) )
                         {
                              SetStatusFlag( ZERO_Y_VAL );
                              text_buffer += ( y_err_mark + String( ptr ) + end_of_line );
                         }
                         else
                         {
                              if ( x > prev_x )
                              {
                                   // the data pair is valid
     
                                   prev_x = x;
     
                                   if ( ++num_valid_xy_pairs > MAX_NUM_DATA_PTS )
                                        SetStatusFlag( TOO_MANY_PTS );
     
                                   text_buffer += ( String( ptr ) + end_of_line );
                              }
                              else
                              {
                                   SetStatusFlag( XVALS_OUT_OF_ORDER );
                                   text_buffer +=
                                   ( x_err_mark + String( ptr ) + end_of_line );
                              }
                         }
                    }
               }
               else
               {
                    SetStatusFlag( INVALID_DATA );
                    text_buffer +=
                    ( fmt_err_mark + String( ptr ) + end_of_line );
               }
          }
     }

     GetValidDataInBinary();

}





//----------------------------------------------------------------------------
//               XyDataFile::GetValidDataInBinary( )
//............................................................................
//                                                                          
// fills in the FloatArray objects with validated data
//                                                                          
//----------------------------------------------------------------------------

void XyDataFile::GetValidDataInBinary( )
{
     char* pbuffer;
     char* ptr;

     // at this point all lines that do not convert to two floating pt numbers
     // are marked with a COMMENT_CHAR in column 1 so this is easy
     // We just scan through and convert all lines to binary representation

     if ( num_valid_xy_pairs == 0 )
     {
          SetStatusFlag( NO_DATA_PTS );
          return;
     }

     if ( ! CreateBuffer( &pbuffer, text_buffer.Len( ) + 1 ) )
     {
          return;
     }

     strcpy( pbuffer, text_buffer );

     ptr = strtok( pbuffer, MLE_END_OF_LINE );

     // scan through, skip comments, convert and store all others
     // work through pbuffer

     UINT32 index = 0;

     rx.Initialize( GetNumberOfValidDataPoints() );
     ry.Initialize( GetNumberOfValidDataPoints() );

     while ( ptr )
     {
          if ( ( *ptr != COMMENT_CHAR ) && ( Words( ptr ) == 2 ) )
          {
               FLOAT64 x;
               FLOAT64 y;

               if ( 2 == sscanf( ptr, " %lf %lf ", &x, &y ) )
               {
                    rx[ index ] = x;
                    ry[ index ] = y;

                    index++;
               }

          }

          ptr = strtok( NULL, MLE_END_OF_LINE );
     }

     ASSERT( index == GetNumberOfValidDataPoints( ) );

     delete [] pbuffer;
}






// ----------------------------------------------------------------------------
//                     XyDataFil::WriteAllDataToTextStream()
// ............................................................................
// 
//   
//
// ----------------------------------------------------------------------------

void XyDataFile::WriteAllDataToTextStream( TextOutputStream& rTOS )
{
     // make a temporary buffer and fill with text_buffer contents 

     char* pbuffer;

     if ( ! CreateBuffer( &pbuffer, text_buffer.Len( ) + 1 ) )
     {
          return;
     }

     strcpy( pbuffer, text_buffer );

     char tempbuff[ TEMP_BUFFER_SIZE ];

     // This function tokenizes a char buffer using \n as separator,
     // removes all trailing blanks, linefeeds, tabs and newline indicators and
     // replaces them with single newline indicator.  The contents of each
     // adjusted line is then saved into the file.

     // first check for the newline character, \n

     char* ptr = strtok( pbuffer, "\n" );

     while ( ptr )
     {

          if ( strlen( ptr ) <= TEMP_BUFFER_SIZE )
          {
               DeleteNonPrintableChars( ptr, tempbuff );

               rTOS << tempbuff  << EOL;
          }

          ptr = strtok( NULL, "\n" );

     }

     // now check for errors and return 

     if ( rTOS.FileError() )
          SetStatusFlag( FILE_WRITE_ERROR );
}





// ----------------------------------------------------------------------------
//                 XyDataFile::WriteValidDataToTextStream()
// ............................................................................
// 
//   
//
// ----------------------------------------------------------------------------

void  XyDataFile::WriteValidDataToTextStream( TextOutputStream& rTOS, 
                                              enum REC_TYPE     PtCountID, 
                                              enum REC_TYPE     DataPairID,
                                              FLOAT64           XMultiplier,
                                              FLOAT64           YMultiplier )
{
     IDType pt_count_id( PtCountID );
     IDType data_pair_id( DataPairID );

     const UINT32 num_pts = GetNumberOfValidDataPoints();

     if ( IDType( NULL_IDTYPE) != pt_count_id ) 
     {
          rTOS << pt_count_id << num_pts << EOL;
     }


     for ( UINT32 i = 0; i < num_pts; i++)
     {
          if ( IDType( NULL_IDTYPE) != pt_count_id ) 
               rTOS << data_pair_id;

          rTOS << ( XMultiplier * rx[i] ) << ( YMultiplier * ry[i] ) << EOL;
     }

     // now check for errors and return 

     if ( rTOS.FileError() )
          SetStatusFlag( FILE_WRITE_ERROR );
}






// ----------------------------------------------------------------------------
//                     XyDataFile::GetErrorMessage()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

const String& XyDataFile::GetErrorMessage( )
{
     // set error_msg to empty string 


     // is there are no errors then return 

     if ( !IsStatusFlagSet( ANY_ERROR ) )
     {
          error_msg = String();
     }
     else
     {
          error_msg = String( ResId( XY_DATAFILE_ERR_MSG_HEADER ) ) +
           file_name + String( ResId( XY_DATAFILE_ERR_MSG1 ) );

          // filename error
     
          if ( IsStatusFlagSet( FILENAME_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG2 ) );
     
          // severe errors
     
          if ( IsStatusFlagSet( FILE_OPEN_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG3 ) );
     
          if ( IsStatusFlagSet( FILE_ACCESS_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG4 ) );
     
          if ( IsStatusFlagSet( FILE_TOO_LARGE ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG5 ) );
     
          if ( IsStatusFlagSet( FILE_READ_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG6 ) );
     
          if ( IsStatusFlagSet( MEM_ALLOC_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG7 ) );
     
          if ( IsStatusFlagSet( FILE_WRITE_ERROR ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG8 ) );
     
          if ( IsStatusFlagSet( ILLEGAL_VALUE ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG9 ) );
     
          if ( IsStatusFlagSet( TOO_MANY_PTS ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG14 ) );
     
          if ( IsStatusFlagSet( NO_DATA_PTS ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG10 ) );
     
          // data format errors
     
          if ( IsStatusFlagSet( INIT_XVAL_NONZERO ) )
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG11 ) );
     
          if ( IsStatusFlagSet( NEGATIVE_Y_VAL ) )
          {
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG16 ) );
     
               if ( mark_errors)
               {
                    error_msg += ( String( ResId( XY_DATAFILE_ERR_MSG15 ) ) +
                          Y_ERROR_MARKER );
               }
          }     
     
          if ( IsStatusFlagSet( ZERO_Y_VAL ) )
          {
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG17 ) );
     
               if ( mark_errors)
               {
                    error_msg += ( String( ResId( XY_DATAFILE_ERR_MSG15 ) ) +
                          Y_ERROR_MARKER );
               }
          }     
     
          if ( IsStatusFlagSet( INVALID_DATA ) )
          {
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG12 ) );
     
               if ( mark_errors)
               {
                    error_msg += ( String( ResId( XY_DATAFILE_ERR_MSG15 ) ) +
                          CONVERSION_ERROR_MARKER );
               }
          }
     
          if ( IsStatusFlagSet( XVALS_OUT_OF_ORDER ) )
          {
               error_msg += String( ResId( XY_DATAFILE_ERR_MSG13 ) );
     
               if ( mark_errors)
               {
                    error_msg += ( String( ResId( XY_DATAFILE_ERR_MSG15 ) ) +
                          X_ERROR_MARKER );
               }
          }
     }

     return error_msg;
}


