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
//  FILEUTIL.CXX - a module of the VSIM User interface 
// ............................................................................
// 
//  This module defines various file utility functions for 
//  the VSIM user interface. 
//
//  CREATED : 4.14.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "fileutil.hxx"
#include "mainapp.hxx"

#endif


#include "strings.hrc"



// ----------------------------------------------------------------------------
//                   class vFileDialog 
// ............................................................................
// 
//   This class, derived from FileDialog, allows the user to specify
//   that the current directory is to be saved as the default.
//
// ----------------------------------------------------------------------------

class vFileDialog : public FileDialog
{
protected:
     CheckBox     checkbox;
     FILE_TYPE    file_type;

     virtual long OK();

public:
          vFileDialog( Window* pParent, WinBits nWinStyle, enum FILE_TYPE FileType );

};






// ----------------------------------------------------------------------------
//                   vFileDialog::vFileDialog()
// ............................................................................
// 
//    constructor
//
// ----------------------------------------------------------------------------

vFileDialog::vFileDialog( Window* pParent, WinBits nWinStyle, enum FILE_TYPE FileType ) :
FileDialog( pParent, nWinStyle ),
checkbox( this, WB_TABSTOP ),
file_type( FileType )
{
     // set the title 

      if ( nWinStyle & WB_SAVEAS )
           SetText(  ResId( vID_STR_SAVE_AS_FILE_DLG_TITLE ) );

      if ( nWinStyle & WB_OPEN )
          SetText(  ResId( vID_STR_OPEN_FILE_DLG_TITLE ) );

     // set some strings based on file type 

     String filter_name;
     String extension;
     String filter_type;

     switch ( file_type )
     {
     case ANY_FILETYPE :

          filter_name  = String( ResId( vID_STR_FILE_FILTERNAME_ANY ) );
          extension    = String( ResId( vID_STR_FILE_EXTENSION_ANY ) ); 
          filter_type  = String( ResId( vID_STR_FILE_FILTERTYPE_ANY ) ); 

          SetPath( GetpMainApp()->GetDefaultAnyFilePath() );

          break;


     case RXNSCHEME_FILETYPE :

          filter_name  = String( ResId( vID_STR_FILE_FILTERNAME_RXN ) );
          extension    = String( ResId( vID_STR_FILE_EXTENSION_RXN ) ); 
          filter_type  = String( ResId( vID_STR_FILE_FILTERTYPE_RXN ) ); 

          SetPath( GetpMainApp()->GetDefaultRxnFilePath() );

          break;


     case TEXT_FILETYPE :

          filter_name  = String( ResId( vID_STR_FILE_FILTERNAME_TXT ) );
          extension    = String( ResId( vID_STR_FILE_EXTENSION_TXT ) ); 
          filter_type  = String( ResId( vID_STR_FILE_FILTERTYPE_TXT ) ); 

          SetPath( GetpMainApp()->GetDefaultTxtFilePath() );

          break;

     case RATE_PROFILE_FILETYPE :

          filter_name  = String( ResId( vID_STR_FILE_FILTERNAME_RAT ) );
          extension    = String( ResId( vID_STR_FILE_EXTENSION_RAT ) ); 
          filter_type  = String( ResId( vID_STR_FILE_FILTERTYPE_RAT ) ); 

          SetPath( GetpMainApp()->GetDefaultRatFilePath() );

          break;

     case TEMP_PROFILE_FILETYPE :

          filter_name  = String( ResId( vID_STR_FILE_FILTERNAME_PRF ) );
          extension    = String( ResId( vID_STR_FILE_EXTENSION_PRF ) ); 
          filter_type  = String( ResId( vID_STR_FILE_FILTERTYPE_PRF ) ); 

          SetPath( GetpMainApp()->GetDefaultPrfFilePath() );

          break;

     default:

          break;
     }

     // add filters etc.

     SetDefaultExt( extension );

     // add the capability of viewing all file names  

     if (ANY_FILETYPE != file_type )
     {
          AddFilter( filter_name, filter_type );
          SetCurFilter( filter_name );
     }

     checkbox.SetText( String( ResId( vID_STR_FILE_DLG_CB_STRING ) ) );
     checkbox.Show();
}






// ----------------------------------------------------------------------------
//                   vFileDialog::OK()
// ............................................................................
// 
//    overloaded handler
//
// ----------------------------------------------------------------------------

long vFileDialog::OK()
{
     if ( FileDialog::OK() )
     {
          if ( checkbox.IsChecked() )
          {
               switch ( file_type )
               {
               case ANY_FILETYPE :
          
                    GetpMainApp()->SetDefaultAnyFilePath( DirEntry( GetPath() ).GetPath().GetFull() );
          
                    break;
          
          
               case RXNSCHEME_FILETYPE :
          
                    GetpMainApp()->SetDefaultRxnFilePath( DirEntry( GetPath() ).GetPath().GetFull() );
          
                    break;
          
          
               case TEXT_FILETYPE :
          
                    GetpMainApp()->SetDefaultTxtFilePath( DirEntry( GetPath() ).GetPath().GetFull() );
          
                    break;

               case RATE_PROFILE_FILETYPE :

                    GetpMainApp()->SetDefaultRatFilePath( DirEntry( GetPath() ).GetPath().GetFull() );

                    break;


               case TEMP_PROFILE_FILETYPE :

                    GetpMainApp()->SetDefaultPrfFilePath( DirEntry( GetPath() ).GetPath().GetFull() );

                    break;

          
               default:
                    break;
               }
          }

          return TRUE;
     }
     else
          return FALSE;
}






// ----------------------------------------------------------------------------
//                          FilenamesMatch()
// ............................................................................
// 
//  returns TRUE if, by the file naming rules of the current OS, the filenames
//  are identical
//
// ----------------------------------------------------------------------------

BOOL8  FilenamesMatch( const String& rName1, const String& rName2 )
{ 
     if ( DirEntry::IsCaseSensitive() )
          return ( COMPARE_EQUAL == rName1.Compare( rName2 ) ); 
     else
          return ( COMPARE_EQUAL == rName1.ICompare( rName2 ) ); 
}






// ----------------------------------------------------------------------------
//                          BOOL8 SaveAsFileDialog( )
// ............................................................................
// 
//  presents a dialog to the user for selecting a file name to save as.
//  returns TRUE if user presses OK button to close, FALSE if Cancel btn
//
// ----------------------------------------------------------------------------

BOOL8 SaveAsFileDialog( String& rFilename, enum FILE_TYPE FileType )
{
     // create dialog object

     vFileDialog file_dlg( GetpMainApp()->GetAppWindow(), WB_SAVEAS | WB_SVLOOK, FileType );


     // and execute 
     if ( RET_CANCEL == file_dlg.Execute( ) )
          return FALSE;
     else
     {
          rFilename = file_dlg.GetPath( );
          return TRUE;
     }

}






// ----------------------------------------------------------------------------
//                          BOOL8 OpenFileDialog( )
// ............................................................................
// 
//  presents a dialog to the user for selecting a file name to open.
//  returns TRUE if user presses OK button to close, FALSE if Cancel btn
//
// ----------------------------------------------------------------------------

BOOL8 OpenFileDialog( String& rFilename, enum FILE_TYPE FileType )
{

     // create dialog object

     vFileDialog file_dlg( GetpMainApp()->GetAppWindow(), WB_OPEN | WB_SVLOOK, FileType );

     // and execute 

     if ( RET_CANCEL == file_dlg.Execute( ) )
          return FALSE;
     else
     {
          rFilename = file_dlg.GetPath( );
          return TRUE;
     }
}




// ----------------------------------------------------------------------------
//                          BOOL8 FileExists( )
// ............................................................................
// 
//  for testing whether a given file already exists.
//  returns TRUE if file is found, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8 FileExists( const String& rFilename )
{
     return ( BOOL8 ) DirEntry( rFilename ).Exists( );
}




