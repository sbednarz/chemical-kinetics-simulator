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
   |  instprxn.cxx   :   a  utility for MSIM                                |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this module patches the header in the files listed |
   |                     in the first fcn parameter, such that the          |
   |                     ext temp profile filename is updated to be the     |
   |                     path given in tje second parm. The base name of the|
   |                     ext t profile is retained; just the path is        |
   |                     changed. The purpose of the action is to ensure    |
   |                     that the path is correct after installation        |
   |                                                                        |
   |  author         :   Bill Hinsberg                    IBM Almaden       |
   |                                                                        |
   |  file created   :   Feb 15, 1994                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop
#include "install.hxx"

#include <stdlib.h>
#include <string.h>


static  char * BaseFilename( char * FullName );


int PatchFiles( const PCHAR FileList[], const PCHAR DestDir, const PCHAR TProfileDir )
{
     msimINSTANCE rxn_instance;
     msimFILE_STRING buffer, filename;

     FILE * f;
     size_t len;
//             cerr << "\nstarting to patch files \n\n";
     for ( int count = 1; FileList[count] != NULL ; )
     {

          // build the filename

       strcpy( filename, DestDir );
          AddPathSeparator( filename );
       strcat( filename, FileList[0] );
       AddPathSeparator( filename );
          strcat( filename, FileList[count] );

       cout << "\nUpdating reaction file " << filename <<"\n";

#if !defined(TEST)                       
          // open the rxn file for reading and writing in binary

          f = fopen( filename, "r+b" );

          if ( !f )
          {
               cerr << "\nError opening rxn file " << filename << "\n\n";
               return( FAILURE );
          }

           // initialize Rxn Instance from file

          fread( &rxn_instance, sizeof ( msimINSTANCE), 1, f );

          if ( ferror( f ) )
          {
              cerr << "\nError reading rxn file " << filename << "\n\n";
              fclose( f );
              return( FAILURE );
          }

          // now update the field in the Instance struct
          // get the new subdir spec in buffer

          strcpy( buffer, DestDir );
          AddPathSeparator( buffer );
          strcat( buffer, TProfileDir );
          AddPathSeparator( buffer );


          // concatenate the base file name in the t profile filename to buffer to
          // construct the full file name, then copy the full name to RxnInstance field

          strcat( buffer, BaseFilename( rxn_instance.temp_profile_data) );
          strcpy( rxn_instance.temp_profile_data, buffer );

         // we are done - now just write it all back out to the file and quit

          // reset position to start of file

          fseek( f, 0L, SEEK_SET );

          fwrite( &rxn_instance, sizeof ( msimINSTANCE ), 1, f );

          if ( ferror( f ) )
          {
               cerr << "\nError writing to rxn file " << filename << "\n\n";
               fclose( f );
               return( FAILURE );
          }

          if ( EOF == fclose( f ) )
          {
               cerr << "\nError closing rxn file " << filename << "\n\n";
               return ( FAILURE );
          }

#endif
          count++;
     }

     return SUCCESS;
}



/*--------------------------------------------------------------------------*/
/*                        msimBaseFilename ( )                              */
/*..........................................................................*/
/*                                                                          */
/* returns a pointer to the start of the base filename (i.e., the  filename */
/* with extension if present without any path/drive info attached           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

char * BaseFilename( char * FullName )
{
     PCHAR colon_posn;
     PCHAR sep_posn;

     if ( ! ( sep_posn = strrchr( FullName, msimPATH_SEPARATOR ) ) )
     {

#if  defined(__OS2__) || defined(__MSDOS__)

          /* if we did not find path seaprator, look for drive separator*/

          if ( ! ( colon_posn = strrchr( FullName, ':' ) ) )
          {

               /* did not find colon either - return original pointer*/

               return FullName;
          }
          else
          {

               /* found colon - return pointer to char following colom*/

               return colon_posn + 1;
          }
#endif

#ifdef   __AIX__
          return  FullName;
#endif
     }

     /* if we made it here we found path separator - return ptr to char  */
     /* following last path sep                                          */

     return sep_posn + 1;

}

