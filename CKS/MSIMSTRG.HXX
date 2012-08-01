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
   |  msimstrg.hxx   :   main header file for msim                          |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : contains global declarations for msim string fcns    |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 11,1993                                        |
   |                                                                        |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


msimBOOL msimParse( PCHAR source, PCHAR key, PCHAR left, PCHAR right );
msimBOOL msimStringsMatch( PCHAR str1, PCHAR str2, USHORT test_type );
PCHAR msimSubWord( PCHAR Str, USHORT WordNum );
USHORT msimWords( PCHAR Str );
PCHAR msimUpCaseString( PCHAR source, PCHAR target );
PCHAR msimStrip( PCHAR Str, char Type );
PCHAR msimBaseFilename( PCHAR FullName );
void msimDeleteNonPrintableChars( PCHAR Source, PCHAR Target );

msimBOOL msimValidFloat( PCHAR str, msimPFLOAT pResult = NULL );
msimBOOL msimValidNonNegativeFloat( PCHAR str, msimPFLOAT pResult = NULL );
msimBOOL msimValidPositiveFloat( PCHAR str, msimPFLOAT pResult = NULL );
msimBOOL msimValidNonZeroFloat( PCHAR str, msimPFLOAT pResult = NULL );
msimBOOL msimValidPositiveULongInteger( PCHAR str, PULONG pResult = NULL );
msimBOOL msimValidPositiveLongInteger( PCHAR str, LONG PTR pResult = NULL );
msimBOOL msimValidPositiveShortInteger( PCHAR str, PUSHORT pResult = NULL );
msimBOOL msimValidNonNegativeShortInteger( PCHAR str, PSHORT pResult = NULL );

VOID msimStringCat( char *Dest, const char *Source, size_t DestSize );
VOID msimStringCopy( char *Dest, const char *Source, size_t DestSize );
