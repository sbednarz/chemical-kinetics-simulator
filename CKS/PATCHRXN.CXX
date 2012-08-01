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

// main unit for patchrxn.exe
// run as an exit routine from IBM Software Installer

// the purpose is to patch rxn demo files so that they correctly
// reflect the current location of the temperature profile



#include "msim2.hxx"
#pragma  hdrstop

#undef __MSDOS__
#include "install.hxx"

#include <stdlib.h>
#include <string.h>



int PatchFile( const PCHAR RxnFile, const PCHAR TProfileFile )
{
     msimINSTANCE rxn_instance;

     FILE * f;
     size_t len;

     // open the rxn file for reading and writing in binary

     f = fopen( RxnFile, "r+b" );

     if ( !f )
          return( FAILURE );

     // initialize Rxn Instance from file

     fread( &rxn_instance, sizeof ( msimINSTANCE), 1, f );

     if ( ferror( f ) )
     {
         fclose( f );
         return( FAILURE );
     }

     // copy the full name of temperature profile to RxnInstance field

     strcpy( rxn_instance.temp_profile_data, TProfileFile );

    // we are done - now just write it all back out to the file and quit

    // reset position to start of file

    fseek( f, 0L, SEEK_SET );

    fwrite( &rxn_instance, sizeof ( msimINSTANCE ), 1, f );

    if ( ferror( f ) )
    {
         fclose( f );
         return( FAILURE );
    }

    if ( EOF == fclose( f ) )
         return ( FAILURE );

     return SUCCESS;
}





// argv[1] is full name of rxn file to update, argv[2] is full name
// of temperature profile file

void main( int argc, char *argv[] )
{
     if ( argc != 3 )
          exit(1);


     if ( FAILURE == PatchFile( argv[1], argv[2] ) )
          exit (2);

     exit( 0 );
}

