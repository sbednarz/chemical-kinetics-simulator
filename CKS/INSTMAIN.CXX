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
   |  instmain.cxx   :   a  component of installation uitility for MSIM     |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this program carries out the installation of the   |
   |                     MSIM package. It is designed to run under both     |
   |                     Windows and OS/2  as a native application.         |
   |                     It is invoked by typing INSTALL. It then queries   |
   |                     the user for the info it needs                     |
   |                                                                        |
   |                                                                        |
   |                                                                        |
   |                                                                        |
   |  author         :   Bill Hinsberg                    IBM Almaden       |
   |                                                                        |
   |  file created   :   Feb 15, 1994                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#include <iostream.h>
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <dir.h>
#include <string.h>

#include "install.hxx"

#define VERSION       "1.0 7/19/94"


// this lists the names of files to be copied at various stages in the
// installation process. Each list has it's last member set to NULL to
// enable detection of the end of each list. The first member of each array
// is the subdirectory below the user selected target into which they
// should be copied

// first, the files which comprise the executable portion of the simulator

const FAR PCHAR ExecFiles[] =
                {
                     "",              // to be copied into base target directory
                     "1",              // disk number as a string
                     "msim.exe",// the main exec file - prog obj created for this if requested
                     "simequil.exe",
                     "convert.exe",
                     NULL
                };


// the files that will be attached to workplace objects or icons. FIrst comes Folder title; the
// remaining strings are arranged as pairs : executable file name, title for icon, etc.

const FAR PCHAR WPObjFiles[] =
                {
                     "MSIM",
                     "msim.exe",// the main exec file - prog obj created for this if requested
                     "MSIM Chemical Rxn Simulator",
                     "convert.exe",
                     "MSIM File Conversion Utility",
                     NULL,
                     NULL
                };


#if defined(USE_4_DISKS)


const FAR PCHAR LibFiles1[] =
{
     "",
     "3",                 // disk number as a string
     "SVB210.DLL",
     NULL
};

const FAR PCHAR LibFiles2[] =
{
     "",
     "4",                 // disk number as a string
#if defined(__MSDOS__)
     "BC30RTL.DLL",
#endif
     "TOOLB210.DLL",
     NULL
};

#else

// for using two disk package

const FAR PCHAR LibFiles[] =
                {
                     "",
                     "2",              // disk number as a string
#if defined(__MSDOS__)
     "BC30RTL.DLL",
#endif
                     "SVB210.DLL",
                     "TOOLB210.DLL",
                     NULL
                };

#endif



// then the help file(s)

const FAR PCHAR HelpFiles[] =
                {
                     "",           // to be copied into base target subdirectory
                     "2",
                     "msim.hlp",
                     NULL
                };

// the reaction demo files

const FAR PCHAR RxnFiles[] =
                {
                     DEMOS_DIR,        // subdir to be copied to
#if defined(USE_4_DISKS)
     "2",
#else
                     "1",
#endif
                     "ACCRETE.RXN",
                     "ADSORB.RXN",
                     "BATCHRX.RXN",
                     "CHIRAL.RXN",
                     "COPOLYMR.RXN",
                     "DIAZENE.RXN",
                     "DISILANE.RXN",
                     "EDISILAN.RXN",
                     "FLOW.RXN",
                     "FLOREACT.RXN",
                     "N2O5.RXN",
                     "ORGNATOR.RXN",
                     "OZONE.RXN",
                     "OZONEO2.RXN",
                     "PARALLEL.RXN",
                     "POLYIMID.RXN",
                     "PHTSNX.RXN",
                     "RESOLVE1.RXN",
                     "RESOLVE2.RXN",
                     "SILANE.RXN",
                     "SMOG1.RXN",
                     "SMOG2.RXN",
                     "SOLIDTGA.RXN",
                     "SUPERCRT.RXN",                     "TBOCSBF6.RXN",
                     "UTFERRIN.RXN",
                     "WCOTPD.RXN",
                     NULL
                };

const FAR PCHAR ExtDataFiles[] =
                {
                     EXTDATA_DIR,      // subdir to be copied to
#if defined(USE_4_DISKS)
     "2",
#else
                     "1",
#endif
                     "dzn252k.dat",
                     "TBSB313K.DAT",
                     "TBSB318K.DAT",
                     "TBSB323K.DAT",
                     "TBSB328K.DAT",
                     "TBSB333K.DAT",
                     NULL
                };


const FAR PCHAR TempProfiles[] =
                {
                     TPROFILES_DIR,    // subdir to be copied to
#if defined(USE_4_DISKS)
     "2",
#else
                     "1",
#endif
                     "step.prf",
                     "step1.prf",
                     NULL
                };


const FAR PCHAR RxnFilesToPatch[] =
                {
                     DEMOS_DIR,        // subdir to be copied to
                     "POLYIMID.RXN",
                     NULL
                };


static void DisplayInitMsg( );
static void GetStartedFromDir( PCHAR PathName, PCHAR FullName );




void main( int argc, char *argv[] )
{
     char source_dir[FILENAME_MAX];
     char dest_dir[FILENAME_MAX];
     int create_objs = FALSE;
     int modify_sys_files = FALSE;
     char started_from_dir[FILENAME_MAX];


     DisplayInitMsg( );

     if ( CheckUserEquipment( ) == FAILURE )
          return;


     GetStartedFromDir( started_from_dir, argv[0] );


     // get the user inputs, store in source dir

     if ( FAILURE == GetUserInput( source_dir, dest_dir, started_from_dir, create_objs, modify_sys_files ) )
          return;

     if ( FAILURE == CopyFiles( ExecFiles, source_dir, dest_dir ) )
          return;

     if ( FAILURE == CopyFiles( RxnFiles, source_dir, dest_dir ) )
          return;

     if ( FAILURE == CopyFiles( ExtDataFiles, source_dir, dest_dir ) )
          return;

     if ( FAILURE == CopyFiles( TempProfiles, source_dir, dest_dir ) )
          return;

     if ( FAILURE == CopyFiles( HelpFiles, source_dir, dest_dir ) )
          return;


#if defined(USE_4_DISKS)
     if ( FAILURE == CopyFiles( LibFiles1, source_dir, dest_dir ) )
         return;

     if ( FAILURE == CopyFiles( LibFiles2, source_dir, dest_dir ) )
         return;
#else

     if ( FAILURE == CopyFiles( LibFiles, source_dir, dest_dir ) )
          return;
#endif

//          cerr << "\nabout to enter patch files \n\n";

     if ( FAILURE == PatchFiles( RxnFilesToPatch, dest_dir, TempProfiles[0] ) )
          return;

     if ( FAILURE == BuildConfigFile( CONFIG_FILENAME, dest_dir ) )
          return;

     if ( create_objs == TRUE )
          CreateProgObjects( WPObjFiles, dest_dir );

     if ( modify_sys_files == TRUE )
          ModifySysFiles( dest_dir );

     cout << "\n\nInstallation completed.\n";

     exit( 0 );
}


void DisplayInitMsg( )
{
     cout << "\n\nINSTALL UTILITY for MSIM : version " << VERSION << "\n";
     cout << "(C) Copyright IBM Corporation 1994. All rights reserved.\n\n";
     cout << "This program automates the installation of the MSIM Chemical\n";
     cout << "Reaction Simulator package. In the installation process, program\n";
     cout << "files are created in a new or existing directory that you specify.\n";
     cout << "The files created include the executable files, online help, and \n";
     cout << "demos and tutorial files. The installation requires about 2.2 megabytes\n";
     cout << "of available space on a hard disk. You may optionally request the\n";
     cout << "installation program to create a program group/folder and program icon. \n";
     cout << "You may also optionally request the installation program to update the \n";
#if defined(__OS2__)
     cout << "CONFIG.SYS file with new path information.\n\n";
#endif
#if defined(__MSDOS__)
     cout << "AUTOEXEC.BAT file with new path information.\n\n";
#endif
     cout << "Press any key to continue . . . ";

     getch( );
     clrscr( );
     return;
}


void GetStartedFromDir( PCHAR PathName, PCHAR FullName )
{
     char drive[MAXDRIVE];
     char dir[MAXDIR];
     char file[MAXFILE];
     char ext[MAXEXT];
     size_t len;

     fnsplit( FullName, drive, dir, file, ext );
     strcpy( PathName, drive );
     strcat( PathName, dir );

     // remove trailing path sep if not root

     if ( dir[1] != '\0' )             // true only if not root dir
     {
          len = strlen( PathName );
          * ( PathName + len - 1 ) = '\0';
     }

     return;
}



