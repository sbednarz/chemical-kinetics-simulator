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


#define INCL_DOSMISC
#define INCL_BASE
#define INCL_WINWORKPLACE


#include <os2.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <dos.h>

#include <iostream.h>
#include <strstrea.h>
#include <iomanip.h>

#include "install.hxx"

int FileExists( const PCHAR Str );
PCHAR UpCaseString( PCHAR source, PCHAR target );
PCHAR Strip( PCHAR Str, char Type );

#define BUFFER_SIZE  4




// this fcn creates a folder on the OS/2 desktop, then places inside that
// folder a program object with correct EXENAME and STARTUPDIR settings

void CreateProgObjects( const PCHAR FileList[], const PCHAR DestDir )
{
     int failure = FALSE;
     char attrs_string[1024];
     int count = 0;

     cout << "\nCreating program objects...\n";

     // create folder on Desktop

     HOBJECT h_object = WinCreateObject( "WPFolder",
                                         FileList[count++],                      // title to be displayed
                                         "OBJECTID=<MSIM_FOLDER>",    // setup string
                                         "<WP_DESKTOP>",
                                         CO_REPLACEIFEXISTS );

    if ( h_object == NULLHANDLE )
         failure = TRUE;

    // create objects inside folder

     while( ( FileList[count] != NULL ) && ( failure == FALSE ) )
     {
          // construct the attributes string for setting program object properties

          ostrstream os( attrs_string, sizeof attrs_string );

          os << "EXENAME=" << DestDir << PATH_SEPARATOR << FileList[count++] << ";";
          os << "STARTUPDIR=" << DestDir << ";";
          os << "MINIMIZED=NO;";
          os << "PROGTYPE=PM;" << ends;

          h_object = WinCreateObject( "WPProgram",
               FileList[count++],
               attrs_string,
               "<MSIM_FOLDER>",
               CO_REPLACEIFEXISTS );

          if ( h_object == NULLHANDLE )
               failure = TRUE;
      }

     if ( failure == TRUE )
          cout << "\nUnable to create program objects on desktop.\n\n";
}




void ModifySysFiles( const PCHAR DestDir )
{
     const size_t LINE_LENGTH = 260;

     cout << "\nModifying system cfg file ...\n";

     //  first get which drive we booted from

     ULONG boot_drive;
     APIRET rc;                        /* Return code                         */

     char oldname[FILENAME_MAX];
     char newname[FILENAME_MAX];

     FILE *f_in;
     FILE *f_out;

     rc = DosQuerySysInfo(( ULONG ) QSV_BOOT_DRIVE, ( ULONG ) QSV_BOOT_DRIVE, ( PVOID ) &boot_drive,
          ( ULONG ) sizeof boot_drive );

     if ( rc != 0 )
     {
          cout << "\nError querying boot drive\n";
          return;
     }

     ostrstream oldname_str( oldname, sizeof oldname );

     oldname_str << ( char ) ( boot_drive + 'A' - 1 ) << ":\\CONFIG." << ends;// leave off extension temporarily

     // oldname now is fully qualified name of config.?

     // generate a new name for backup of config.sys


     for ( int i = 0; i < 999; i++ )
     {
          sprintf( newname, "%s%-03.3d", oldname, i );

          if ( ! FileExists( newname ) )
               break;
     }

     if ( i > 998 )
     {
          // we could not get an unused filename !?!?
          cout << "\nUnable to generate unused filename. CONFIG.SYS not updated\n";
          return;
     }

     // we now have newname, no complete oldname

     strcat( oldname, "SYS" );

     if ( 0 != rename( oldname, newname ) )// failed
     {
          // error renaming config.sys
          cout << "\nError renaming existing file. CONFIG.SYS not updated\n";
          return;
     }


     // now open both files, read newname line by line, look for LIBPATH and PATH
     // if found then modify by appending new subdir to it;
     // finally write to file oldname

     if ( NULL == ( f_in = fopen( newname, "rt" ) ) )
     {
          cout << "\nError opening " << newname << " for reading. CONFIG.SYS not updated\n";
          return;
     }

     if ( NULL == ( f_out = fopen( oldname, "wt" ) ) )
     {
          cout << "\nError opening " << oldname << " for writing. CONFIG.SYS not updated\n";
          fclose( f_in );
          return;
     }


     // main loop - here we look for key words LIBPATH or SET
     // if we find LIBPATH we try to append new path spec to end of line
     // if we find SET, them we look at next word. If it is HELP or PATH
     // then we try to append new path spec to end of line

     while ( ! feof( f_in ) )
     {
          char line_str[LINE_LENGTH];

          fgets( line_str, sizeof ( line_str ), f_in );

          if ( ! feof( f_in ) )
          {
               char workstr[LINE_LENGTH];
               char word[LINE_LENGTH];
               int append_path = FALSE;
               PCHAR next_token;

               UpCaseString( line_str, workstr );
               Strip( workstr, 'B' );

               // first look for the patterns "SET PATH=" or "SET HELP="

               next_token = strtok( workstr, " \t" );

               if ( next_token )
               {
                    strcpy( word, next_token );
                    Strip( word, 'B' );

                    if ( 0 == strcmpi( word, "SET" ) )
                    {
                         next_token = strtok( NULL, "=" );// get next word delimited by = sign

                         if ( next_token )
                         {
                              strcpy( word, next_token );
                              Strip( word, 'B' );

                              if ( ( 0 == strcmpi( word, "HELP" ) ) ||
                                        ( 0 == strcmpi( word, "PATH" ) ) )
                              {
                                   int already_there = FALSE;

                                   // now check to be sure that entry is not already there

                                   while ( next_token = strtok( NULL, ";\n\r" ) )
                                        if ( 0 == strcmpi( next_token, DestDir ) )
                                             already_there = TRUE;

                                   if ( already_there == FALSE )
                                        append_path = TRUE;
                              }
                         }
                    }
               }

               // look for the pattern "LIBPATH="

               next_token = strtok( workstr, "=" );

               if ( next_token )
               {
                    strcpy( word, next_token );
                    Strip( word, 'B' );

                    if ( 0 == strcmpi( word, "LIBPATH" ) )
                    {
                         int already_there = FALSE;

                         // now check to be sure that entry is not already there

                         while ( next_token = strtok( NULL, ";\n\r" ) )
                              if ( 0 == strcmpi( next_token, DestDir ) )
                                   already_there = TRUE;

                         if ( already_there == FALSE )
                              append_path = TRUE;
                    }
               }


               if ( append_path == TRUE )
               {
                    // we try to append the path
                    // first we check whether the existing entry is too long

                    // remove trailing whitespace including \n

                    Strip( line_str, 'T' );

                    if ( ( strlen( line_str ) + strlen( DestDir ) + 2 ) > LINE_LENGTH )
                    {
                         cout << "\nCannot append path. New entry will exceed legal line length.\n";
                         cout << "You must manually edit CONFIG.SYS to include the path entry\n";
                         cout << DestDir << " in the PATH, HELP and LIBPATH statements\n\n";
                    }

                    // if workstr oes not already have a trailing path delimiter then add one

                    if ( line_str[strlen( line_str ) - 1] != ';' )// add separator if necessaary
                         strcat( line_str, ";" );

                    strcat( line_str, DestDir );
                    strcat( line_str, ";\n" );
               }

               fputs( line_str, f_out );

          }
     }

     if ( ferror( f_in ) || ferror( f_out ) || fclose( f_in ) || fclose( f_out ) )
     {
          cout << "\nErrors occurred during the CONFIG.SYS update procedure. Replace the \n";
          cout << "existing CONFIG.SYS file with the copy saved as\n";
          cout << newname << " and manually update the PATH, HELP and LIBPATH settings so\n";
          cout << "that they contain the path entry " << DestDir << "\n\n";
     }
     else
     {
          // successful if we get here

          cout << "\nREMINDER : You must reboot OS/2 to put the changes in CONFIG.SYS into effect.\n\n";
     }
     return;
}




PCHAR UpCaseString( PCHAR source, PCHAR target )
{
     PCHAR sav;

     sav = target;

     while ( 1 )                       /* forever                             */
     {
          if ( '\0' == ( *target = toupper( *source ) ) )
               return sav;
          target++;
          source++;
     }
}



PCHAR Strip( PCHAR Str, char Type )
{
     PCHAR tmp1, tmp2;
     size_t length;

     if ( ! Str )
          return NULL;

     Type = toupper( Type );
     switch ( Type )
     {
     case 'L' :
     case 'B' :
          tmp1 = tmp2 = Str;
          while ( isspace( *tmp1 ) )   /* scan to first word                  */
               tmp1++;

          while ( *tmp1 )              /* now move all chars forward in
                                          array                               */
               *tmp2++= *tmp1++;
          *tmp2 = '\0';                /* and terminate with zero             */
          if ( Type == 'L' )
               break;

     case 'T' :
          length = strlen( Str );      // added to check for zero-length string

          if ( length )
          {
               tmp1 = Str + length - 1;/* point to end of string              */
               while ( isspace( *tmp1 ) )/* scan backward until a nonspace
                                            char is found                     */
                    tmp1--;

               * ( ++tmp1 ) = '\0';    /* place a terminal zero after it      */
          }

          break;
     }

     return Str;
}


int FileExists( const PCHAR Str )
{
     struct ffblk ffblock;

     return ( 0 == findfirst( Str, &ffblock, 0 ) ) ? TRUE : FALSE;
}



// try to learn as much as we can about the hardware

int CheckUserEquipment( void )
{
     int problem = FALSE;
     int disk;
     struct diskfree_t free;
     long avail;
     int found_one = FALSE;
     BYTE device_info;

     cout << "\n\nChecking the existing setup...\n\n";


     cout << "Checking OS/2 version ...";

     //  first get which drive we booted from

     ULONG sys_info[2];
     APIRET rc;                        /* Return code                         */

     rc = DosQuerySysInfo(( ULONG ) QSV_VERSION_MAJOR, ( ULONG ) QSV_VERSION_MINOR, ( PVOID ) &sys_info,
          ( ULONG ) sizeof sys_info );

     if ( rc == 0 )
     {
          cout << "Version " << ( sys_info[0] / 10 ) << "." << sys_info[1] << " detected.\n";

          if ( ( sys_info[0] < 20 ) )
          {
               cout << "This level of OS/2 is not supported\n";
               problem = TRUE;
          }
     }

     // do we have coprocessor ?

     cout << "Checking for math coprocessor ...";

     if ( ! DosDevConfig( &device_info, DEVINFO_COPROCESSOR ) )
          if ( device_info )
               cout << "Coprocessor detected.\n";
          else
          {
               cout << "No coprocessor detected! Simulator performance will be degraded\n";
               problem = TRUE;
          }


     // now check disk space on all hard drives and print possibility of installation

     cout << "Checking hard disks ...\n";

     /* print the availability of hard drives for installation */


     // from c disk == 2 through alphabet

     for ( disk = 3; disk < 26; ++disk )
     {

          if ( _dos_getdiskfree( disk, &free ) == 0 )
          {
               avail = ( long ) free.avail_clusters
               * ( long ) free.bytes_per_sector
               * ( long ) free.sectors_per_cluster;

               if ( avail >= MIN_FREE_SPACE )// the amount of free space we need
               {
                    char c = ( ( char ) ( disk - 1 + 'A' ) );

                    if ( found_one == FALSE )
                    {
                         found_one = TRUE;
                         *DefaultDestination = c;
                    }

                    cout << "   Drive " << c << " has " << avail
                    << " bytes free - installation on this drive is possible\n";
               }
          }
     }

     if ( found_one == FALSE )
     {
          problem = TRUE;
          cout << "No drive has enough free space for installation! If installing over a\n"
          << "previous installation of MSIM there should be no problem\n";
     }


     if ( problem == TRUE )
     {
          cout << "\n\nThe detected environment/resources are inadequate for installing or\n"
          << "operating the simulator. Proceed with installation anyway? (y/n)";

          if ( 'Y' == GetYesNoResponse( ) )
          {
               cout << "\n";
               return SUCCESS;
          }
          else
          {
               cout << "\n\nAborting installation ...\n";
               return FAILURE;
          }
     }
     else
          return SUCCESS;
}


