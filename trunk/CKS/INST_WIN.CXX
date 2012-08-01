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

#include <windows.h>
#include <ddeml.h>

#include <iostream.h>
#include <strstrea.h>

#include <stdio.h>
#include <string.h>

#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <process.h>

#include "install.hxx"

// these are used for initializing/carrying out creation of prog objects
// using DDE under Windows

static HANDLE hInst;
static DWORD idInst = 0L;              // instance identifier for DDE



static int FileExists( const PCHAR Str );
static PCHAR UpCaseString( PCHAR source, PCHAR target );
static PCHAR Strip( PCHAR Str, char Type );


// we use WinMain here just to capture the value of hInstance, then call main (Main)

#pragma argsused

int PASCAL WinMain( HANDLE hInstance, HANDLE hPrevInstance, LPSTR
                lpCmdLine, int nCmdShow )
{
     char module_name[260];

     PCHAR ptr_array[1];
     ptr_array[0] = module_name;

     GetModuleFileName( hInstance, module_name, sizeof ( module_name ) );


     hInst = hInstance;

     _InitEasyWin( );

     Main( 1, ptr_array );

//    DdeUninitialize( idInst);   // last thing before quitting

     return 0;
}




// try to learn as much as we can about the hardware

int CheckUserEquipment( void )
{
     int problem = FALSE;
     int disk;
     struct diskfree_t free;
     long avail;
     int found_one = FALSE;

     cout << "\n\nChecking the existing setup...\n\n";


     DWORD win_flags;

     win_flags = GetVersion( );
     cout << "Checking Windows version ...";

     cout << "Version " << ( ( int ) LOBYTE( LOWORD( win_flags ) ) ) << "." << ( ( int ) HIBYTE( LOWORD( win_flags ) ) ) << " detected.\n";
     if ( ( LOBYTE( LOWORD( win_flags ) ) != 3 ) ||
               ( HIBYTE( LOWORD( win_flags ) ) < 10 )
          )
     {
          cout << "This level of Windows is not supported\n";
          problem = TRUE;
     }


     win_flags = GetWinFlags( );

     cout << "Checking operating mode ...";

     if ( ! ( win_flags & WF_PMODE ) )
     {
          cout << "Running in Real Mode! This is not supported.\n";
          problem = TRUE;
     }
     else
          cout << "Running in protected mode.\n";

     // do we have coprocessor ?

     cout << "Checking for math coprocessor ...";

     if ( ! ( win_flags & WF_80x87 ) )
     {
          cout << "No coprocessor detected! Simulator performance will be degraded\n";
          problem = TRUE;
     }
     else
          cout << "Coprocessor detected.\n";


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



// dummy fcn for call to DdeInitialize - not actually used

#pragma argsused

HDDEDATA CALLBACK DdeCallback( UINT Type, UINT Fmt, HCONV hConv, HSZ hSz1,
                       HSZ hSz2, HDDEDATA hData, DWORD Data1, DWORD Data2 )

{
     return ( HDDEDATA ) NULL;
}




void CreateProgObjects( const PCHAR FileList[], const PCHAR DestDir )
{
     HCONV hconv;
     HSZ hstring;
     FARPROC p_dde_proc;               /* procedure instance address          */
     char msg[2048];
     ostrstream os( msg, sizeof msg );
     DWORD result;
     int count = 0;

     const DWORD TIME_OUT = 3000;// how long we wait for response from PROGMAN server in msec


     cout << "\n\nCreating program objects ... \n";

     // protected mode is necessary for using DDE

     if ( ! ( GetWinFlags( ) & WF_PMODE ) )
     {
          cerr << "\nCannot create program objects - not running in protected mode.\n\n";
          return;
     }

     p_dde_proc = MakeProcInstance(( FARPROC ) DdeCallback, hInst );

     if ( p_dde_proc == NULL )
     {
          cerr << "\nCannot create program objects - error making proc instance\n\n";
          return;
     }

     if ( DdeInitialize( &idInst, ( PFNCALLBACK ) p_dde_proc, APPCMD_CLIENTONLY, 0L ) )
     {
          // fail initialization

          cerr << "\nCannot create program objects - DDE initialization failed.\n\n";
          return;
     }


     hstring = DdeCreateStringHandle(
          idInst,                      /* instance identifier                 */
          "PROGMAN",                   /* string to register                  */
          CP_WINANSI );                /* code page                           */

     hconv = DdeConnect( idInst, hstring, hstring, NULL );

     if ( hconv == NULL )
     {
          cerr << "\nCannot create program objects - DDE connection failed.\n\n";
          return;
     }

     // construct the message string for setting program object properties

     // first create program group and show it
     // in ShowGroup, 2nd paramter == 1 means activate and show group

     os << "[CreateGroup(\"" << FileList[count] <<
      "\")][ShowGroup(\"" << FileList[count] << "\",1)]";

      count++;

     // now create program items within the group 

     int xpos = 0;

     while ( FileList[count] != NULL)
     {
          PCHAR filename = FileList[count++];

          PCHAR title = FileList[count++];

          os << "[AddItem(\"";

          // the fully qualified filename 
          os << DestDir << PATH_SEPARATOR << filename << "\",";   // command line

          os << "\"" << title << "\",\"" <<                       // visible title
            DestDir << PATH_SEPARATOR << filename;                // icon 
            os << "\",," << ( 40 + ( xpos * 100) ) << ",10,\"" <<          // icon position
            DestDir << "\")]";                                    // default dir

          xpos++;
     }

     os << ends;


     if ( FALSE == DdeClientTransaction( msg, strlen( msg ) + 1, hconv, NULL, CF_TEXT,
                    XTYP_EXECUTE, TIME_OUT, &result ) )
     {
          cerr << "\nCannot create program objects - DDE client transaction failed.\n";
          cerr << "Result code = " << result << "; Error code = " << DdeGetLastError( idInst ) << "\n";
          cerr << "Msg content = " << msg << "\n\n";

          return;
     }

     DdeDisconnect( hconv );

     DdeFreeStringHandle( idInst, hstring );

     DdeUninitialize( idInst );        // last thing before quitting

     return;
}




void ModifySysFiles( const PCHAR DestDir )
{
     const size_t LINE_LENGTH = 127;

     cout << "\nModifying system cfg file ...\n";

     //  first get which drive we booted from

     char oldname[FILENAME_MAX];
     char newname[FILENAME_MAX];

     FILE *f_in;
     FILE *f_out;

     ostrstream oldname_str( oldname, sizeof oldname );

     oldname_str << "C:\\AUTOEXEC." << ends;// leave off extension temporarily

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
          cerr << "\nUnable to generate unused filename. AUTOEXEC.BAT not updated\n";
          return;
     }

     // we now have newname, no complete oldname

     strcat( oldname, "BAT" );

     if ( 0 != rename( oldname, newname ) )// failed
     {
          // error renaming config.sys
          cerr << "\nError renaming existing file. AUTOEXEC.BAT not updated\n";
          return;
     }


     // now open both files, read newname line by line, look for LIBPATH and PATH
     // if found then modify by appending new subdir to it;
     // finally write to file oldname

     if ( NULL == ( f_in = fopen( newname, "rt" ) ) )
     {
          cerr << "\nError opening " << newname << " for reading. AUTOEXEC.BAT not updated\n";
          return;
     }

     if ( NULL == ( f_out = fopen( oldname, "wt" ) ) )
     {
          cerr << "\nError opening " << oldname << " for writing. AUTOEXEC.BAT not updated\n";
          fclose( f_in );
          return;
     }


     // main loop - here we look for key words PATH or SET
     // if we find PATH we try to append new path spec to end of line
     // if we find SET, them we look at next word. If it is PATH
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

               // first look for the pattern "SET PATH="

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

                              if ( ( 0 == strcmpi( word, "PATH" ) ) )
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

                    // look for the pattern "PATH xxxxx;xxx"

                    if ( 0 == strcmpi( word, "PATH" ) )
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


               // look for the pattern "PATH="   wdh bug fix 3.24.94

               next_token = strtok( workstr, "=" );

               if ( next_token )
               {
                    strcpy( word, next_token );
                    Strip( word, 'B' );

                    if ( 0 == strcmpi( word, "PATH" ) )
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
                         cerr << "\nCannot append path. New entry will exceed legal line length.\n";
                         cerr << "You must manually edit AUTOEXEC.BAT to include the path entry\n";
                         cerr << DestDir << " in the PATH statement\n\n";
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
          cerr << "\nErrors occurred during the AUTOEXEC.BAT update procedure. Replace the \n";
          cerr << "existing AUTOEXEC.BAT file with the copy saved as\n";
          cerr << newname << " and manually update the PATH setting so\n";
          cerr << "that it contains the path entry " << DestDir << "\n\n";
     }
     else
     {
          // successful if we get here

          cout << "\nREMINDER : You must reboot DOS to put the changes in AUTOEXEC.BAT into effect.\n\n";
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



