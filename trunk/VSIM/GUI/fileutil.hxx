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
//  FILEUTIL.HXX - a header file of the VSIM User interface 
// ............................................................................
// 
//  This header file declares various file utility functions for 
//  the VSIM user interface. 
//
//  CREATED : 4.14.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#if !defined(__FILEUTIL_HXX__)
#define __FILEUTIL_HXX__

#include "vsimtype.hxx"
#include <sv.hxx>


enum FILE_TYPE { ANY_FILETYPE, RXNSCHEME_FILETYPE, RUN_FILETYPE, TEXT_FILETYPE, 
                 RATE_PROFILE_FILETYPE, TEMP_PROFILE_FILETYPE  };


BOOL8 SaveAsFileDialog( String& rFilename, enum FILE_TYPE FileType );
BOOL8 OpenFileDialog( String& rFilename, enum FILE_TYPE FileType );

BOOL8 FileExists( const String& rFilename );

BOOL8  FilenamesMatch( const String& rName1, const String& rName2 );

#endif

