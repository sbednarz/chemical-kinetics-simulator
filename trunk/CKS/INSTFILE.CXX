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

/* []----------------------------------------------------------------------[]
   |  instfile.cxx  :   a component of the MSIM installation utility        |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this module contains file manipulation functions   |
   |                     for the MSIM installation utility.                 |
   |                                                                        |
   |                                                                        |
   |  author         :   Bill Hinsberg                    IBM Almaden       |
   |                                                                        |
   |  file created   :   Feb 15, 1994                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <dir.h>
#include <ctype.h>
#include <conio.h>
#include <io.h>
#include <stdlib.h>

#include "install.hxx"

#define DISKTAG   "DISK"
#define BUFSIZE    64000

static int CopyFile( FILE *Source, FILE *Target );
static int RequestInstallDisk( PCHAR SourceDir, PCHAR DiskNum );


int CopyFiles( const PCHAR FileList[], const PCHAR SourceDir, const PCHAR DestDir )
{
     char sourcename[FILENAME_MAX], targetname[FILENAME_MAX];
     FILE *source, *target;
     size_t len;

     if ( FAILURE == RequestInstallDisk( SourceDir, FileList[1] ) )
     {
          cerr << "\n\nQuitting installation ...";
          return FAILURE;
     }

     for ( int count = 2; FileList[count] != NULL; )
     {

          // build the filename

          strcpy( sourcename, SourceDir );
          AddPathSeparator( sourcename );
          strcat( sourcename, FileList[count] );

          strcpy( targetname, DestDir );
          AddPathSeparator( targetname );
          strcat( targetname, FileList[0] );// the sub-subdir
          AddPathSeparator( targetname );
          strcat( targetname, FileList[count] );

          // open the source file for reading in binary
#if !defined(TEST)
          source = fopen( sourcename, "rb" );

          if ( ! source )
          {
               cerr << "\nError opening rxn file " << sourcename << "\n\n";
               return ( FAILURE );
          }

          // open the target file for writing in binary

          target = fopen( targetname, "wb" );

          if ( ! target )
          {
               cerr << "\nError opening rxn file " << targetname << "\n\n";
               return ( FAILURE );
          }
#endif
          cout << "Creating " << targetname << "  .";

#if !defined(TEST)
          if ( FAILURE == CopyFile( source, target ) )
          {
               fclose( source );
               fclose( target );
               return FAILURE;
          }

          if ( ferror( source ) )
          {
               cerr << "\nError reading from rxn file " << sourcename << "\n\n";
               fclose( source );
               fclose( target );
               return ( FAILURE );
          }

          if ( EOF == fclose( source ) )
          {
               cerr << "\nError closing rxn file " << sourcename << "\n\n";
               return ( FAILURE );
          }


          if ( ferror( target ) )
          {
               cerr << "\nError reading from rxn file " << targetname << "\n\n";
               fclose( target );
               return ( FAILURE );
          }

          if ( EOF == fclose( target ) )
          {
               cerr << "\nError closing rxn file " << targetname << "\n\n";
               return ( FAILURE );
          }
#endif

          count++;
     }

     return SUCCESS;
}



int CopyFile( FILE *Source, FILE *Target )
{
     size_t bytes_read, bytes_written;
     struct ftime ft;

     if ( 0 != getftime( fileno( Source ), &ft ) )
          return FAILURE;

     PCHAR pbuffer = new char[BUFSIZE];

     if ( ! pbuffer )
          return FAILURE;

     while ( ( bytes_read = fread( pbuffer, 1, BUFSIZE, Source ) ) > 0 )
     {
          bytes_written = fwrite( pbuffer, 1, bytes_read, Target );
          cout << ".";
          if ( bytes_written != bytes_read )
          {
               cerr << "\n\nFile copy error...Quitting installation\n";
               delete[]pbuffer;
               return FAILURE;
          }
     }

     cout << "\n";

     delete[]pbuffer;

     if ( 0 != setftime( fileno( Target ), &ft ) )
          return FAILURE;

     return SUCCESS;
}


int RequestInstallDisk( PCHAR SourceDir, PCHAR DiskNum )
{
     char name[FILENAME_MAX];

     // construct disk tag name

     strcpy( name, SourceDir );
     AddPathSeparator( name );
     strcat( name, DISKTAG );
     strcat( name, DiskNum );

     struct ffblk ffblock;

     while ( 0 != findfirst( name, &ffblock, 0 ) )
     {
          cout << "\nInsert Installation Disk #" << DiskNum << ".\n";
          cout << "Press Q to quit installation, any other key to continue\n";

          if ( 'Q' == toupper( getch( ) ) )
          {
               cout << "\n";
               return FAILURE;
          }
          else
               cout << "\n";
     }

     return SUCCESS;
}



