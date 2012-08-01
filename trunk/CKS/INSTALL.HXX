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

#if defined (__MSDOS__)
#define main Main
#define FAR _far
void Main( int argc, char * argv[] );
#endif

#if defined(__OS2__)
#define FAR
#endif

#define TRUE          1
#define FALSE         0
#define SUCCESS       TRUE
#define FAILURE       FALSE
#define MIN_FREE_SPACE 2200000L


#define CONFIG_FILENAME      "msim.cnf"

typedef char *PCHAR;
typedef unsigned short USHORT;

#if defined(__MSDOS__) || defined(__OS2__)
#define PATH_SEP "\\"
#define PATH_SEPARATOR '\\'
#define PATH_DELIMITER ';'
#endif


#define DEMOS_DIR             "demos"
#define EXTDATA_DIR           "extdata"
#define TPROFILES_DIR         "tprofile"
#define TEXT_OUTPUT_DIR       "output"
#define GRAPH_OUTPUT_DIR      "output"
#define DEFAULT_DESTINATION   "c:\\msim"

void AddPathSeparator( PCHAR str );

int GetUserInput( PCHAR Source, PCHAR Target, PCHAR InitDir, int &pCreateObjsFlag, int &ModifySysFilesFlag );

int CopyFiles( const PCHAR FileList[], const PCHAR SourceDir, const PCHAR DestDir );

int PatchFiles( const PCHAR FileList[], const PCHAR DestDir, const PCHAR TProfileDir );

int BuildConfigFile( const PCHAR ConfigFilename, const PCHAR DestDir );

void CreateProgObjects( const PCHAR FileList[], const PCHAR DestDir );

void ModifySysFiles( const PCHAR DestDir );

char GetYesNoResponse( );

int CheckUserEquipment( void );

extern char DefaultDestination[];





