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

/* []-----------------------------------------------------------------------[]
   |  msimfile.hxx   :   a header file for msim                             |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : declarations for module msimfile.cxx                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 11,1993                                        |
   |                                                                        |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#define msimNO_CHECK_FOR_OVERWRITE   FALSE
#define msimCHECK_FOR_OVERWRITE      TRUE
#define msimSHOW_WARNING             TRUE
#define msimNO_SHOW_WARNING          FALSE


msimRC msimSaveEditorTextToFile( PCHAR EditorString, FILE PTR fileptr );
msimRC msimSaveRxnScheme( msimPINSTANCE Instance, msimWID Owner, msimBOOL CheckForOverWrite );
msimRC msimLoadRxnScheme( msimPINSTANCE Instance, PCHAR filename, msimWID Owner );
msimBOOL msimOKtoOverwrite( PCHAR Filename, msimWID Owner, msimBOOL
              WarnAboutDataLoss );
VOID msimFileError( PCHAR Filename, msimWID Owner, USHORT Errtype );
void msimSaveRxnSchemeAs( msimPINSTANCE Instance, msimWID Owner, msimBOOL ShowWarning );
msimBOOL msimFileExists( PCHAR Filename );
msimBOOL msimLocateAppFile( PCHAR BaseFileName, PCHAR Destination, size_t BufSize );
msimRC msimGetFileName( msimWID Owner, const PCHAR Template, PCHAR FileName,
            WinBits Flags, const PCHAR Title, const PCHAR FileType, size_t BufSize );
void msimConstructFullyQualifiedFilename( PCHAR FileName, size_t BufSize );
msimBOOL msimOKtoOverwrite( PCHAR Filename, msimWID Owner );
msimBOOL msimFileAlreadyLoaded( msimWID Owner, PCHAR Name );
msimBOOL msimGetSizeOfFile( PCHAR FileName, PULONG pSize );

