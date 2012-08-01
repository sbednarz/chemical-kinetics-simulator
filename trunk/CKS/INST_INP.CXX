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
   |  inst_inp.cxx   :   a component of the MSIM installation utility       |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this module handle the user input. We need to get  |
   |                     from the user the following info : the source      |
   |                     directory, the target directory, and whether to    |
   |                     to create a program object.                        |
   |                                                                        |
   |  author         :   Bill Hinsberg                    IBM Almaden       |
   |                                                                        |
   |  file created   :   Feb 15, 1994                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <dir.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>

#include "install.hxx"


static int DirExists( const PCHAR Str );
static USHORT Words( PCHAR Str );
static int CreateSubDirs( PCHAR Parent );
static void GetDirectory( PCHAR Drive, PCHAR Directory, int Length );
static void DissectFilename( const PCHAR Fullname, PCHAR Drive, PCHAR Directory,
                 PCHAR Filename );
static void BuildFilename( PCHAR Target, const PCHAR Drive, const PCHAR Directory,
                 const PCHAR Filename );
static void ConstructFullyQualifiedFilename( PCHAR FileName );

char DefaultDestination[FILENAME_MAX] = DEFAULT_DESTINATION;


int GetUserInput( PCHAR Source, PCHAR Target, PCHAR InitDir, int &CreateObjsFlag, int &ModifySysFilesFlag )
{
     char source_dir[FILENAME_MAX];
     char dest_dir[FILENAME_MAX];
     char req_dest[FILENAME_MAX];
     char req_source[FILENAME_MAX];

     int progobj_create;
     int modify_sys_files;
     int quit = FALSE;
     int dest_dir_exists = FALSE;

     strcpy( dest_dir, DefaultDestination );

     // first find out where we are

     strcpy( source_dir, InitDir );

     while ( quit == FALSE )
     {
          // get user request for target subdirectory

          cout << "\nEnter the name of the subdirectory where MSIM should be\n";
          cout << "installed. Press enter to accept " << dest_dir << "\n\n";
          cout << "     New target directory : ";

          cin.getline( req_dest, sizeof req_dest );


          switch ( Words( req_dest ) )
          {
          case 1 :

               break;

          case 0 :

               strcpy( req_dest, dest_dir );
               break;


          default :

               cerr << "\n\nThe entry " << req_dest << " is invalid!\n\n";
               continue;
          }



          /* get user request for source subdirectory */

          cout << "\n\nEnter the location of the files to be installed.\n";
          cout << "Press enter to accept the current directory = " << source_dir << "\n\n";
          cout << "     New source directory : ";

          cin.getline( req_source, sizeof req_source );


          switch ( Words( req_source ) )
          {
          case 1 :

               break;

          case 0 :

               strcpy( req_source, source_dir );

               break;

          default :

               cerr << "\n\nThe entry " << req_source << " is invalid!\n\n";
               continue;
          }


          // what about creating a program object after installation ?

          cout << "\n\nDo you want to create a program object for MSIM\n";
          cout << "as part of the installation process? ( y/n) ";

          progobj_create = ( 'Y' == GetYesNoResponse( ) );


#if defined (__OS2__)
          cout << "\n\nSeveral settings in CONFIG.SYS can be updated to\n";
#endif
#if defined (__MSDOS__)
          cout << "\n\nThe PATH setting in AUTOEXEC.BAT can be updated to\n";
#endif
          cout << "include the subdirectory containing MSIM. This updating is not\n";
          cout << "required if you plan to only start MSIM from its icon. If you choose\n";
          cout << "to automatically update this file, the unmodified file will be\n";
          cout << "saved under another name. Do you want this file to be \n";
          cout << "updated as part of the installation process? ( y/n) ";

          modify_sys_files = ( 'Y' == GetYesNoResponse( ) );


          // we now have all the user inputs
          // check with user before starting actual installation

          ConstructFullyQualifiedFilename( req_source );

          ConstructFullyQualifiedFilename( req_dest );


          cout << "\n\nSummary of your selections :\n\n";
          cout << "   MSIM will be installed in the directory " << req_dest << "\n";
          cout << "   using the installation files found in " << req_source << ".\n";
          cout << "   A program object will " << ( progobj_create ? "" : "not " ) << "be created.\n";
#if defined (__OS2__)
          cout << "   CONFIG.SYS will " << ( modify_sys_files ? "" : "not " ) << "be modified.\n\n";
#endif
#if defined (__MSDOS__)
          cout << "   AUTOEXEC.BAT will " << ( modify_sys_files ? "" : "not ") << "be modified.\n\n";
#endif
          cout << "Are these selections correct? (y/n) ";

          if ( 'N' == GetYesNoResponse( ) )
          {
               clrscr( );
               continue;
          }

          cout << "\n";

          if ( DirExists( req_source ) )
               strcpy( source_dir, req_source );
          else
          {
               cerr << "\n\nDirectory " << req_source << " does not exist!\n\n";
               continue;
          }


          if ( DirExists( req_dest ) )
          {
               quit = TRUE;
               strcpy( dest_dir, req_dest );
               dest_dir_exists = TRUE;
          }
          else
          {
               cout << "\nCreating directories...\n\n";

               // now try to make the directory

               if ( 0 == mkdir( req_dest ) )
               {
                    cout << "Directory " << req_dest << " created.\n";
                    strcpy( dest_dir, req_dest );
                    quit = TRUE;
               }
               else
               {
                    // failed to create dir
                    cerr << "Unable to create directory " << req_dest << " ...\n\n";
                    continue;
               }
          }

     }

     if ( FAILURE == CreateSubDirs( dest_dir ) )
          return FAILURE;


     // we come here when user has accepted the selections

     cout << "\n\nReady to begin installation . . .\n\n";

     if ( dest_dir_exists )
     {
          cout << "NOTE : Existing files in the destination directory with names\n";
          cout << "identical to the program files being installed will be replaced.\n\n";
     }

     cout << "OK to proceed with installation? (y/n) ";

     if ( 'N' == GetYesNoResponse( ) )
     {
          cerr << "\n\nQuitting installation ...\n";
          return FAILURE;
     }
     else
     {
          cout << "\n\n";
          strcpy( Source, source_dir );
          strcpy( Target, dest_dir );

          CreateObjsFlag = progobj_create;
          ModifySysFilesFlag = modify_sys_files;
          return SUCCESS;
     }
}


char GetYesNoResponse( )
{
     char char_resp;

     for ( char_resp = '?'; ! ( char_resp == 'Y' || char_resp == 'N' ); )
          char_resp = toupper( getch( ) );

     cout << char_resp;

     return char_resp;
}



int DirExists( const PCHAR Str )
{
     struct ffblk ffblock;
     char name[FILENAME_MAX];

     strcpy( name, Str );

     // is it a root ??

     if ( isalpha( name[0] ) && ( name[1] == ':' ) && ( name[2] == PATH_SEP[0] )
               && name[3] == '\0' )
     {
          int save = getdisk ( );      //save for later restore

          int requested_disk = toupper ( name[0] ) - 'A';// calc drive number

          setdisk( requested_disk );   //try to set to requested drive
          if ( requested_disk == getdisk( ) )
          {
               setdisk( save );
               return TRUE;
          }
          else
          {
               setdisk( save );
               return FALSE;
          }
     }
     else
          return ( 0 == findfirst( Str, &ffblock, FA_DIREC ) ) ? TRUE : FALSE;

}


USHORT Words( PCHAR Str )
{
     USHORT retval = 0;

     while ( isspace( *Str ) )         /* scan to first word                  */
          Str++;

     while ( isgraph( *Str ) )
     {
          retval++;
          while ( isgraph( *Str ) )
               Str++;
          while ( isspace( *Str ) )
               Str++;
     }                                 /* endwhile                            */
     return retval;
}


void AddPathSeparator( PCHAR str )
{
     str = str + strlen( str ) - 1;

     if ( *str != PATH_SEP[0] )
          strcat( str, PATH_SEP );

     return;
}


int CreateSubDirs( PCHAR Parent )
{
     PCHAR subdirs[] =
      {
           DEMOS_DIR,
           EXTDATA_DIR,
           TPROFILES_DIR,
           TEXT_OUTPUT_DIR,
           GRAPH_OUTPUT_DIR,
           NULL
      };

     char name[FILENAME_MAX];

     for ( int i = 0; subdirs[i] != NULL; )
     {
          // contruct the subdir name

          strcpy( name, Parent );
          AddPathSeparator( name );
          strcat( name, subdirs[i] );

          if ( ! DirExists( name ) )
               if ( 0 == mkdir( name ) )
                    cout << "Subdirectory " << name << " created.\n";
               else
                    return FAILURE;

          i++;
     }

     return SUCCESS;
}





void ResolveRelativePathInfo( PCHAR Drive, PCHAR Directory, USHORT Size )
{
     char abs_dir[FILENAME_MAX];
     PCHAR end_of_target;
     PCHAR target;
     PCHAR source;

     if ( *Directory == PATH_SEP[0] )
          strcpy( abs_dir, Directory );// spec starts from root
     else
     {
          // we start with a relative path spec - append spec to current working dir
          // first get cwd spec

          GetDirectory( Drive, abs_dir, sizeof ( abs_dir ) );
          AddPathSeparator( abs_dir );
          strcat( abs_dir, Directory );
     }

     AddPathSeparator( abs_dir );

     // at this point abs_dir starts and ends with a path separator char
     // it may contain relative path info initially imbedded in the string
     // pointed to by the fcn parameter Directory

     // Now copy contents of abs_dir to Directory buffer one character at
     // a time. We check for and adjust for any remaining relative path
     // info as we go

     end_of_target = Directory + Size;
     target = Directory;
     source = abs_dir;

     while ( ( *source ) && ( target < end_of_target ) )
     {

          // did we find the segment "/."??  If so, look at following
          // characters to figure out whether we have "/../" or "/./"
          // or just a subdir name starting with .

          if ( *source == '.' && * ( source - 1 ) == PATH_SEP[0] )
          {
               switch ( * ( source + 1 ) )
               {

               case  '.' :            // found the segment "/.." - keep checking

                    if ( * ( source + 2 ) == PATH_SEP[0] )
                    {
                         // the segment is "/../" .
                         // Backspace in the target to remove previous subdir name,
                         // and move source ptr to just past end of the "/../" segment

                         // To get here, target must currently point to a path separator char.
                         // If it is not the initial path separator
                         // (indicating the root directory), then
                         // back up one character in target, then scan backward
                         // until we hit the preceeding path separator char

                         if ( --target > Directory )
                              while ( * --target != PATH_SEP[0] )
                                   /* NULL*/;   // do nothing

                         target++;// move to position immediately after last path sep char

                         source += 3;// point just past "/../" segment in source
                    }
                    else
                         // it's just a subdir name staring with ".."
                         // just copy in the normal manner
                         *target++= *source++;

                    break;

               case PATH_SEPARATOR :

                    // we just found the segment "/./" just skip
                    // over it without doing anything to target
                    source += 2;
                    break;

               default :

                    // next char is not a special one - just copy
                    // and adjust ptrs in the normal manner
                    *target++= *source++;
                    break;

               }                       // end switch

          }                            // end if
          else
               *target++= *source++;

     }                                 // end while

     *target = '\0';                   // terminate with trailing zero

     return;
}



void ConstructFullyQualifiedFilename( PCHAR FileName )
{
     char base_filename[FILENAME_MAX], directory[FILENAME_MAX];
     char drive[4];

     /* analyze the template to separate the drive directory and filename */

     DissectFilename( FileName, drive, directory, base_filename );

     ResolveRelativePathInfo( drive, directory, sizeof ( directory ) );

     BuildFilename( FileName, drive, directory, base_filename );
}

/*--------------------------------------------------------------------------*/
/*                            DissectFilename()                             */
/*..........................................................................*/
/*                                                                          */
/* splits a filename passed as a pointer to string   into its components    */
/* and copies those components into  the approporiate strings               */
/*--------------------------------------------------------------------------*/

void DissectFilename( const PCHAR Fullname, PCHAR Drive, PCHAR Directory,
          PCHAR Filename )
{
     PCHAR start_of_path, end_of_path;

     start_of_path = Fullname;

#if      defined(__OS2__)                        || defined(__MSDOS__)

     /* look for a colon signifying a drive letter at front                 */

     if ( * ( start_of_path + 1 ) == ':' )

     {
          * ( Drive++ ) = *Fullname;
          start_of_path += 2;
     }
     else

          /* if not found then get current drive value                   */

          * ( Drive++ ) = getdisk( ) + 'a';

     * ( Drive++ ) = ':';

#endif

     *Drive = '\0';

     /* find the last backslash - NULL returned if not there                */

     if ( end_of_path = strrchr( Fullname, PATH_SEP[0] ) )
          strcpy( Filename, end_of_path + 1 );
     else
          strcpy( Filename, start_of_path );

     while ( start_of_path <= end_of_path )
          *Directory++= *start_of_path++;

     /* terminate with zero                                                 */

     *Directory = '\0';

     return;
}



void BuildFilename( PCHAR Target, const PCHAR Drive, const PCHAR Directory,
          const PCHAR Filename )
{

     /* check for trailing path separator on Directory string - add if needed*/

     AddPathSeparator( Directory );

     sprintf( Target, "%s%s%s", Drive, Directory, Filename );
     return;
}


/*--------------------------------------------------------------------------*/
/*                        GetDirectory()                                    */
/*..........................................................................*/
/*                                                                          */
/* gets the current logged directory and returns it on the string pointed   */
/* to by Directory. If an error of some sort occurs then a message is       */
/* displayed and the Directory string is set to zero length                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void GetDirectory( PCHAR Drive, PCHAR Directory, int Length )
{

#if defined(__OS2__) || defined(__MSDOS__)
     int drive_number;
     int rc;                           /* Return code                         */



     // null string in Drive - look in default drive

     drive_number = ( *Drive == '\0' ) ? 0 : toupper( *Drive ) - 'A' +1;/* calc drive in integer form*/

     *Directory = PATH_SEPARATOR;      // getcurdir does not return
     // initial backslash

     rc = ( -1 == getcurdir( drive_number, Directory + 1 ) );// -1 returned if fail

#endif

#ifdef   __AIX__
     msimBOOL           rc;

        rc = (NULL == getcwd(Directory, Length));
#endif

     /* if non-zero return code then an error has occurred -             */
     /* set Directory = null string                                      */

     if ( rc )
          *Directory = '\0';

     return;
}

