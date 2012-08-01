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
   |  msimFile.cxx   :   contains file I/O functions for msim               |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  0.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   August 14, 1993                                    |
   |                                                                        |
   |  CHANGE HISTORY  : 5.24.95 changed MSIM name to CKS in strings         |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimfile.hxx"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(__OS2__) || defined(__MSDOS__)
#include <dir.h>
#include <dos.h>
#endif

#ifdef   __AIX__
#include <errno.h>
#include <unistd.h>
#endif

#if defined(__MAC__)
#include <errno.h>
#include <SysDep.hxx>
#define TEST_TYPE msimCASE_INSENSITIVE
#define  SPECIAL_CHARS                           ":"

static void AddPathSeparator( PCHAR str );
void  SaveCurDir( );
void SetCurDir( PCHAR DirSpec);
void RestoreSavedDir();


#endif

/* the following sets the size of buffers used to collect filenames and     */
/* directories in creating a filename dialog                                */

#if  defined(__OS2__) || defined(__MSDOS__)
#define TEST_TYPE     msimCASE_INSENSITIVE
#endif

#ifdef   __AIX__
#define TEST_TYPE msimCASE_SENSITIVE
#define  SPECIAL_CHARS                           "/"
#endif


/* prototypes of local functions follow                                     */

static msimBOOL OKtoDestroySimData( PCHAR Filename, msimWID Owner );
static msimBOOL FileContainsSimulationData( PCHAR Filename );
static msimRC WriteNotebookTextToFile( msimPINSTANCE Instance, FILE PTR ThisFile );




/*--------------------------------------------------------------------------*/
/*                        msimFileExists()                                  */
/*..........................................................................*/
/*                                                                          */
/* tests for the existence of a file given  a ptr to a asciiz filename      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimFileExists( PCHAR Filename )
{
     return ( msimBOOL ) DirEntry( String( Filename ) ) .Exists( );
}



msimBOOL FileContainsSimulationData( PCHAR Filename )
{
     msimBOOL simulation_data_there = FALSE;
     msimINSTANCE file_header;
     FILE *f;

     /* peek at the existing file header to see if simulation data is       */
     /* attached to it                                                      */

     if ( NULL == ( f = fopen( Filename, "rb" ) ) )
          return FALSE;
     else
     {
          size_t num_read = fread( &file_header, sizeof ( file_header ), 1, f );

          if ( ( num_read != 1 ) || ferror( f ) )
          {
               fclose( f );
               return FALSE;
          }

          fclose( f );

          /* now check for file validity and for whether there is      */
          /* simulation data attached to it                            */

          simulation_data_there = ( strcmp( file_header.msimID,
                    msimRXN_FILE_FLG ) == 0 &&
               file_header.num_simulation_data_records != 0L );
     }

     return simulation_data_there;
}


/*--------------------------------------------------------------------------*/
/*                        msimOKtoOverwrite()                               */
/*..........................................................................*/
/*                                                                          */
/* tests for the existence of a file given  a ptr to a asciiz filename      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimOKtoOverwrite( PCHAR Filename, msimWID Owner )
{
     /* return immediately with TRUE if user does not want to check         */
     /* for file over-writes                                                */

     if ( ! msimAppOptions.confirm_on_overwrite )
          return TRUE;

     /* build string for display                                            */

     String message = String( ResId( msimOVERWRITE_STR1 ) )
          + String( msimBaseFilename( Filename ) )
          + String( ResId( msimOVERWRITE_STR2 ) );

     return ( RET_YES == QueryBox( Owner, WB_YES_NO | WB_DEF_NO, message ) .Execute( ) );
}


msimBOOL OKtoDestroySimData( PCHAR Filename, msimWID Owner )
{

     /* return immediately with TRUE if user does not want to check         */
     /* for file over-writes                                                */

     if ( ! msimAppOptions.confirm_on_overwrite )
          return TRUE;

     /* build string for display                                            */
     String message = String( ResId( msimDESTROY_SIMDATA_STR1 ) )
          + String( msimBaseFilename( Filename ) )
          + String( ResId( msimDESTROY_SIMDATA_STR2 ) );

     return ( RET_YES == QueryBox( Owner, WB_YES_NO | WB_DEF_NO, message ) .Execute( ) );
}



void msimFileError( PCHAR Filename, msimWID Owner, USHORT Errtype )
{
     String message;

     switch ( Errtype )
     {
     case msimFILE_OPEN_ERROR :

          message = "Error opening \"";

          message += msimBaseFilename( Filename );

          message += "\". \nSys err msg : ";

          message += strerror( errno );

          break;

     case msimWRITE_ERROR :

          message = "Error writing to \"";

          message += msimBaseFilename( Filename );

          message += "\". ""Data has not been saved.\n Sys err msg : ";

          message += strerror( errno );

          break;

     case msimREAD_ERROR :

          message = "Error reading from \"";

          message += msimBaseFilename( Filename );

          message += "\". ""Data has not been loaded\n Sys err msg : ";
          message += strerror( errno );

          break;

     case msimNOT_MSIM_FILE :

          message = "File \"";

          message += msimBaseFilename( Filename );

          message += "\" is not a valid reaction file. Data was not loaded";

          break;

     case msimFOREIGN_MSIM_FILE :

          message = "File \"";

          message += msimBaseFilename( Filename );

          message += "\" is a reaction file from another operating system. It must be "
          "converted before it can be loaded.";

          break;


     case msimSEEK_ERROR :

          message = "Seek error in file \"";

          message += msimBaseFilename( Filename );
          message += "\".\n Sys err msg : ";

          message += strerror( errno );

          break;

     case msimFILE_ACCESS_ERROR :
     default :

          message = "I/O Error accessing file \"";

          message += msimBaseFilename( Filename );
          message += "\".\n Sys err msg : ";

          message += strerror( errno );

          break;

     }                                 /* endswitch                           */
     ErrorBox( Owner, WB_OK | WB_DEF_OK, message ) .Execute( );

     return;
}


/*--------------------------------------------------------------------------*/
/*                        msimGetFilename( )                                */
/*..........................................................................*/
/*                                                                          */
/* invokes file dialog - returns msimUSER_ABORT if canceled, msimNO_ERROR   */
/* otherwise.  Flags must == WB_SAVEAS or WB_OPEN                           */
/*                                                                          */
/*  FileType under Macintosh contains the 4 character file type             */
/*  under other contains the file extension when implemented                */
/*--------------------------------------------------------------------------*/

msimRC msimGetFileName( msimWID Owner, const PCHAR Template, PCHAR FileName,
            WinBits Flags, const PCHAR Title, const PCHAR FileType, size_t BufSize )
{
     String title = String( ResId( msimFILE_DLG_TITLE_HDR ) );
     String name;
     String file_template = String( Template );

     /* construct the dialog box title                                      */

     if ( Title )
          title += Title;
     else
          title += String( ResId( msimFILE_DLG_DEFAULT_TITLE ) );



     FileDialog aDlg( Owner, Flags | WB_SVLOOK );

     aDlg.SetText( title );

#if defined(__MSDOS__) || defined(__OS2__) || defined(__AIX__)

     String extension;

     aDlg.SetPath( String( Template ) );

     extension = DirEntry( file_template ) .GetExtension( );
     aDlg.SetDefaultExt( extension );

     extension = String( "*." ) + extension;

#if defined(__AIX__)
     aDlg.AddFilter( extension, extension );
#else
     aDlg.AddFilter( extension, String( ) );
#endif

     aDlg.SetCurFilter( extension );

#endif

#if defined(__MAC__)

#if !defined(__PPC__)
      SaveCurDir( );
     SetCurDir( Template);
#else

     char                  pathname[265];



      // contruct a pascal-style string of a filename for making a FSSpec

      msimStringCopy( pathname, Template, sizeof pathname );
      AddPathSeparator( pathname);
	  
	  if (WB_SAVEAS == Flags )
	        msimStringCat( pathname, "untitled", sizeof pathname );    

     aDlg.SetPath( String( pathname ) );


#endif	 


     // on the MAC we expect the FileType to contain the filetype
      // and the Template to contain default dir to search
      // as a four character string

     // save the cwd
     // now change to that spec'ed in file_template


     aDlg.RemoveAllFilter();
     aDlg.AddFilter( String( FileType), String( FileType)  );
     aDlg.SetCurFilter( FileType );

#endif

     if ( RET_CANCEL == aDlg.Execute( ) )
     {

#if defined(__MAC__)

         RestoreSavedDir();

#endif

          return msimUSER_ABORT;
     }
     else
     {
          name = aDlg.GetPath( );
          msimStringCopy( FileName, name, BufSize );
     }

#if defined(__MAC__)

     RestoreSavedDir();

#endif

     return msimNO_ERROR;
}


/*--------------------------------------------------------------------------*/
/*                        msimSaveRxnScheme()                               */
/*..........................................................................*/
/*                                                                          */
/* writes all the data in memory which is ascociated withe the rxn instance */
/* Instance to the file name fiven in Instance->filename. returns           */
/* msimNO_ERROR is successful, a menginful error code otherwise             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimSaveRxnScheme( msimPINSTANCE Instance, msimWID Owner, msimBOOL CheckForOverWrite )

{
     FILE *f;
     msimPRXN rxnptr;
     msimPSPECIES species_ptr;
     msimRC rc;

     /* check for no filename - get one if missing                          */

     if ( ! msimWords( Instance->filename ) )
     {
          if ( msimUSER_ABORT == msimGetFileName( Owner,
                         msimAppOptions.data_template,
                         Instance->filename,
                         WB_SAVEAS,
                         "Save Reaction Scheme",
                         msimRXNFILE_TYPE,
                         sizeof Instance->filename ) )
          {
               return msimUSER_ABORT;
          }

          if ( FileContainsSimulationData( Instance->filename ) )
          {
               if ( ! OKtoDestroySimData( Instance->filename, Owner ) )
                    return msimUSER_ABORT;
          }

          msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
     }
     else
     {
          /* if file exists and contains existing simulation data check before overwrite */

          if ( CheckForOverWrite == msimCHECK_FOR_OVERWRITE && msimFileExists( Instance->filename ) )
          {
               if ( FileContainsSimulationData( Instance->filename ) )
               {
                    if ( ! OKtoDestroySimData( Instance->filename, Owner ) )
                         return msimUSER_ABORT;
               }
               else
                    if ( ! msimOKtoOverwrite( Instance->filename, Owner ) )
                         return msimUSER_ABORT;
          }
     }

     /* if notebook window is opened for this file, save data               */

     if ( Instance->p_notebook_window != msimWID_INVALID )
          msimReadNotebookMLE( Instance );

     /* open the file and check for problems                                */

     if ( NULL == ( f = fopen( Instance->filename, "wb" ) ) )
     {
          msimFileError( Instance->filename, Owner, ( USHORT ) msimFILE_OPEN_ERROR
          );
          return msimFILE_OPEN_ERROR;
     }

     aMainApp.Wait( TRUE );

     /* If we are overwriting a previously existing file, the simulation    */
     /* results data is now deleted and is invalid anyway. Be sure that     */
     /* the following field in instance == 0 as we use it in the plot       */
     /* routines to be test for the availability of simulation results      */

     Instance->num_simulation_data_records = 0L;

     Instance->sim_return_code = msimNO_ERROR;

     /* write the first record - we will come back and write it again at the*/
     /* end of this function with some updated fields                       */

     fwrite( Instance, sizeof ( msimINSTANCE ), 1, f );

     /* write out the reaction scheme, walking through the linked list      */

     rxnptr = Instance->ptr_to_rxnlist;

     while ( ( rxnptr != NULL ) && ! ferror( f ) )
     {
          fwrite( rxnptr, sizeof ( msimRXN ), 1, f );
          rxnptr = rxnptr->next;
     }

     /* write out the species data, walking through the linked list         */

     species_ptr = Instance->ptr_to_species_list;

     while ( ( species_ptr != NULL ) && ! ferror( f ) )
     {
          fwrite( species_ptr, sizeof ( msimSPECIES ), 1, f );
          species_ptr = species_ptr->next;
     }

     /* write out the notebook string to the output file                    */

     if ( msimNO_ERROR != ( rc = WriteNotebookTextToFile( Instance, f ) ) )
     {
          fclose( f );

          aMainApp.Wait( FALSE );

          if ( rc == msimMEM_ALLOC_ERROR )
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );

          return rc;
     }


     /* find out where we are in the file as this is where we will end      */
     /* up putting the simulation results                                   */
     /* We can also use this offset to find the notebook text since it      */
     /* is at a constant offset = msimNOTEBOOK_SIZE back in the file from   */
     /* here                                                                */

     if ( -1L == ( Instance->sim_data_file_offset.offset = ftell( f ) ) )
     {
          msimFileError( Instance->filename, Owner, msimSEEK_ERROR );
          fclose( f );
          return msimSEEK_ERROR;
     }

     Instance->data_altered_since_lastsave = FALSE;
     Instance->notebook_altered = ( USHORT ) FALSE;

     /* now rewrite the first record to the file, which now                 */
     /* contains updated values for the data_file_offset and the            */
     /* notebook text length                                                */

     rewind( f );
     fwrite( Instance, sizeof ( msimINSTANCE ), 1, f );

     aMainApp.Wait( FALSE );

     /* check for errors during write                                       */

     if ( ferror( f ) )
     {
          msimFileError( Instance->filename, Owner, ( USHORT ) msimWRITE_ERROR );
          fclose( f );
          return msimWRITE_ERROR;
     }

     /* close file and check for errors                                     */

     if ( EOF == fclose( f ) )
     {
          msimFileError( Instance->filename, Owner, ( USHORT ) msimWRITE_ERROR );
          return msimWRITE_ERROR;
     }

#if defined(__MAC__)

     Sysdepen::SetFileInfo( String( Instance->filename), String( msimRXNFILE_TYPE),
                  String( msimCREATOR_NAME) );

#endif

     /* if we got this far everything went ok                               */

     return msimNO_ERROR;
}



/*--------------------------------------------------------------------------*/
/*                        msimLoadRxnScheme()                               */
/*..........................................................................*/
/*                                                                          */
/* loads a rxn file into memory, filling in the Instance struct as it       */
/* proceeds. optionally invoked the filename dialog to get a filename from  */
/* the user if the parameter requested_file is a blank or null string.      */
/* Returns msimNO_ERROR if successful, a meningful error code otherwise     */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimLoadRxnScheme( msimPINSTANCE Instance, PCHAR requested_file, msimWID
            Owner )
{
     FILE PTR f;
     msimPRXN currentrxn, previousrxn;
     msimPSPECIES currentspecies, previous_species;
     msimFILE_STRING filename;
     PCHAR fnptr = filename;

     filename[0] = '\0';

     if ( ! msimWords( requested_file ) )
     {
          if ( msimUSER_ABORT == msimGetFileName( Owner,
                         msimAppOptions.data_template,
                         filename,
                         WB_OPEN,
                         "Load Reaction Scheme",
                         msimRXNFILE_TYPE,
                         sizeof filename )
               )
               return msimUSER_ABORT;
     }
     else

          msimStringCopy( filename, requested_file, sizeof filename );

     /* if name is still null then return cause it's an error               */

     if ( ! msimWords( filename ) )
          return msimFILENAME_ERROR;

     /* clear memory - if user aborts memory clear then quit                */

     if ( ! msimClearMemory( Instance, Owner, TRUE ) )
          return msimUSER_ABORT;

     if ( msimFileAlreadyLoaded( Owner, filename ) )
     {
          return msimFILE_ALREADY_LOADED;
     }


     /* go ahead and try to open the file                                   */

     if ( NULL == ( f = fopen( fnptr, "rb" ) ) )
     {

          msimFileError( filename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     aMainApp.Wait( TRUE );

     fread( Instance, sizeof ( msimINSTANCE ), 1, f );

     if ( ! msimStringsMatch( Instance->msimID, msimRXN_FILE_FLG, msimCASE_SENSITIVE ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( filename, Owner,
               msimStringsMatch( Instance->msimID, msimRXN_FILE_FLG, msimCASE_INSENSITIVE ) ?
                    ( USHORT ) msimFOREIGN_MSIM_FILE : ( USHORT ) msimNOT_MSIM_FILE );

          fclose( f );
          *Instance = msimC_INSTANCE;
          return msimNOT_MSIM_FILE;

     }                                 /* endif                               */

     /* if there has previously been a reaction scheme constructed,         */
     /* then it was saved - load it now                                     */

     if ( Instance->ptr_to_rxnlist != NULL )
     {

          /* read in first struct                                           */

          Instance->ptr_to_rxnlist = new msimRXN;

          if ( ! Instance->ptr_to_rxnlist )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );
               aMainApp.Wait( FALSE );

               fclose( f );
               return msimMEM_ALLOC_ERROR;
          }

          fread( Instance->ptr_to_rxnlist, sizeof ( msimRXN ), 1, f );
          Instance->ptr_to_rxnlist->prev = NULL;

          currentrxn = Instance->ptr_to_rxnlist;

          while ( currentrxn->next != NULL )
          {
               currentrxn->next = new msimRXN;
               if ( ! currentrxn->next )
               {
                    msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                         __TIMESTAMP__, __LINE__, Owner );
                    aMainApp.Wait( FALSE );

                    fclose( f );
                    return msimMEM_ALLOC_ERROR;
               }

               previousrxn = currentrxn;
               currentrxn = currentrxn->next;
               fread( currentrxn, sizeof ( msimRXN ), 1, f );
               currentrxn->prev = previousrxn;
          }
     }

     /* now repeat the same process to load the species list into memory    */

     if ( Instance->ptr_to_species_list != NULL )
     {

          /* read in first struct                                           */

          Instance->ptr_to_species_list = new msimSPECIES;

          if ( ! Instance->ptr_to_species_list )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );
               aMainApp.Wait( FALSE );

               fclose( f );
               return msimMEM_ALLOC_ERROR;
          }

          fread( Instance->ptr_to_species_list, sizeof ( msimSPECIES ), 1, f );
          Instance->ptr_to_species_list->prev = NULL;

          currentspecies = Instance->ptr_to_species_list;

          while ( currentspecies->next != NULL )
          {
               currentspecies->next = new msimSPECIES;

               if ( ! currentspecies->next )
               {
                    msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                         __TIMESTAMP__, __LINE__, Owner );
                    aMainApp.Wait( FALSE );

                    fclose( f );
                    return msimMEM_ALLOC_ERROR;
               }

               previous_species = currentspecies;
               currentspecies = currentspecies->next;
               fread( currentspecies, sizeof ( msimSPECIES ), 1, f );
               currentspecies->prev = previous_species;
          }
     }

     /* finally load in the notebook text which is stored as a long         */
     /* ASCIIZ string. We get its size from the Instance field              */

     if ( Instance->notebook_size )
     {
          // this block modified 2/28/94 to be consistent with the usage of
          // notebook_size in msimnb.cxx. notebook_size does not include
          // the trailing zero in the length calculation

          Instance->ptr_to_notebook_text = new CHAR[Instance->notebook_size + 1];

          if ( ! Instance->ptr_to_notebook_text )
          {
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );
               aMainApp.Wait( FALSE );

               fclose( f );
               return msimMEM_ALLOC_ERROR;
          }

          fread( Instance->ptr_to_notebook_text, Instance->notebook_size, 1, f );

          // now be sure that trailing zero is there

          * ( Instance->ptr_to_notebook_text + Instance->notebook_size ) = '\0';

     }
     else
          Instance->ptr_to_notebook_text = NULL;// just be sure we have valid value here

     if ( ferror( f ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( Instance->filename, Owner, ( USHORT ) msimREAD_ERROR );
          fclose( f );
          return msimREAD_ERROR;
     }

     fclose( f );

     aMainApp.Wait( FALSE );

     // set some of the fields in the Instance struct to appropriate values

     if ( ( Instance->listbox_selection >= Instance->speciescount )  && ( Instance->speciescount > 0 ) )
          Instance->listbox_selection = Instance->speciescount - 1;

     Instance->specieslist_altered = FALSE;/* since it was just loaded from
                                              a file                          */

     Instance->session_id = msimNO_SESSION;/* since it was just loaded from
                                              a file                          */

     Instance->data_altered_since_lastsave = FALSE;
     Instance->notebook_altered = ( USHORT ) FALSE;
     Instance->p_plot_dialog = NULL;
     Instance->p_notebook_window = NULL;

     msimStringCopy( Instance->filename, filename, sizeof Instance->filename );
     msimStringCopy( Instance->base_filename, msimBaseFilename( filename ),
          sizeof Instance->base_filename );

     return msimNO_ERROR;
}




#if 0

// commented out

/*--------------------------------------------------------------------------*/
/*                            DissectFilename()                             */
/*..........................................................................*/
/*                                                                          */
/* splits a filename passed as a pointer to string   into its components    */
/* and copies those components into  the approporiate strings               */
/*--------------------------------------------------------------------------*/

msimRC DissectFilename( const PCHAR Fullname, PCHAR Drive, PCHAR Directory,
            PCHAR Filename )
{
     DirEntry full_name;
     full_name = DirEntry( String( Fullname ) );

     DirEntry device;

     device = full_name.GetDevice( );

     strcpy( Filename, full_name.GetName( ) );

     strcpy( Drive, device.GetFull( ) );

     full_name.ToRel( device );

     strcpy( Directory, full_name.GetFull( ) );

     return msimNO_ERROR;
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

msimRC GetDirectory( PCHAR Drive, PCHAR Directory, size_t Length )
{
     DirEntry cwd;

     cwd = DirEntry( String( Drive ) );

     if ( cwd.IsValid( ) )
     {
          msimStringCopy( Directory, cwd.GetFull( ), Length);
          return msimNO_ERROR;
     }
     else
          return msimREAD_ERROR;
}

#endif



/*--------------------------------------------------------------------------*/
/*                  msimSaveRxnSchemeAs()                                   */
/*..........................................................................*/
/*                                                                          */
/* provides for saving a rxn scheme under a filename different than its     */
/* current name.                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimSaveRxnSchemeAs( msimPINSTANCE Instance, msimWID Owner, msimBOOL ShowWarning )
{
     /* replace the filename and save data under that name */

     msimBOOL done;
     msimFILE_STRING new_name;

     /* save current filename - might need it later */

     if ( ShowWarning == msimSHOW_WARNING )
     {
          String message = String( ResId( msimSAVEAS_STRING1 ) );

          message += String( ResId( msimSAVEAS_STRING3 ) );

          message += String( ResId( msimSAVEAS_STRING4 ) );;

          if ( RET_NO == QueryBox( Owner, WB_YES_NO | WB_DEF_NO, message ) .Execute( ) )
               return;
     }

     /* if we made it to here it is ok to proceed                        */
     /* close any open windows before attempting to save the data           */

     if ( Instance->p_notebook_window )
          msimCloseNotebookWindow( Instance );

     if ( Instance->p_plot_dialog )
          msimClosePlotDialogWindow( Instance );

     /* now get new filename - abort if none selected                       */

     done = FALSE;

     while ( ! done )
     {
          if ( msimUSER_ABORT == msimGetFileName( Owner, msimAppOptions.data_template
                         , new_name, WB_SAVEAS,
                         "Save Reaction Scheme as...", msimRXNFILE_TYPE,
                         sizeof new_name ) )
               return;

          // if we are planning to rename the file in memory , first
          // check to be sure will we not be giving it the name of another  file
          // that is already loaded in memory

          if ( msimFileAlreadyLoaded( Owner, new_name ) )
               continue;


          if ( FileContainsSimulationData( new_name ) )
          {
               if ( OKtoDestroySimData( new_name, Owner ) )
                    done = TRUE;
          }
          else
               done = TRUE;

     }

     /* attempt to save under new filename                                  */

     msimStringCopy( Instance->filename, new_name, sizeof Instance->filename );

     msimStringCopy( Instance->base_filename, msimBaseFilename( new_name ),
          sizeof Instance->base_filename );

     /* bail out if save failed                                             */

     if ( msimNO_ERROR != msimSaveRxnScheme( Instance, Owner, msimNO_CHECK_FOR_OVERWRITE ) )
          return;


     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );

     return;
}



/*--------------------------------------------------------------------------*/
/*                  msimSaveNotebookText( )                                 */
/*..........................................................................*/
/*                                                                          */
/* saves notebook text to a given rxn file. Each reaction file has a        */
/* segment of size msimNOTEBOOK_SIZE reserved for notebook text. It is      */
/* located immediately preceding the simulation data. Since we have saved   */
/* the file location of the simulation data( even if the simulation has     */
/*  not yet been run)  we can always find the notebook text position. If    */
/* the existing notebook text does not fill the entire buffer then the      */
/* rest of the buffer is filled with binary zero                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimSaveNotebookText( msimPINSTANCE Instance, msimWID Owner )

{
     FILE PTR f;
     msimRC rc;

     /* check for no filename                                               */

     if ( ! msimWords( Instance->filename ) )
          return msimFILENAME_ERROR;

     /* if file does not exist then save the whole rxn scheme               */

     if ( ! msimFileExists( Instance->filename ) )
          return ( msimSaveRxnScheme( Instance, Owner, msimCHECK_FOR_OVERWRITE ) );

     /* if notebook window is opened for this file, save data               */

     if ( Instance->p_notebook_window != msimWID_INVALID )
          msimReadNotebookMLE( Instance );

     aMainApp.Wait( TRUE );

     /* open the file and check for problems                                */

     if ( NULL == ( f = fopen( Instance->filename, "r+b" ) ) )
     {
          aMainApp.Wait( FALSE );

          msimFileError( Instance->filename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
          return msimFILE_OPEN_ERROR;
     }

     /* move to the start of the notebook text segment in the rxn file      */
     /* we first move to the start of rxn data ( which immediately follows  */
     /* the notebook text ) and then back up to the                         */
     /* start of the notebook text                                          */

     if ( 0 != fseek( f, Instance->sim_data_file_offset.offset - ( LONG ) msimNOTEBOOK_SIZE,
                    SEEK_SET ) )
     {
          aMainApp.Wait( FALSE );

          fclose( f );

          msimFileError( Instance->filename, Owner, ( USHORT ) msimSEEK_ERROR );
          return msimSEEK_ERROR;
     }


     /* write out the notebook string to the output file                    */

     if ( msimNO_ERROR != ( rc = WriteNotebookTextToFile( Instance, f ) ) )
     {
          aMainApp.Wait( FALSE );

          fclose( f );

          if ( rc == msimMEM_ALLOC_ERROR )
               msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
                    __TIMESTAMP__, __LINE__, Owner );

          return rc;
     }

     /* update the file header to indicate correct notebook text size       */
     /* and to make ptr_to_notebook_text = current value                    */

     fseek( f, offsetof( msimINSTANCE, notebook_size ), SEEK_SET );
     fwrite( &( Instance->notebook_size ), sizeof ( Instance->notebook_size ),
          1, f );

     Instance->notebook_altered = ( USHORT ) FALSE;

     aMainApp.Wait( FALSE );


     /* check for errors during write                                       */

     if ( ferror( f ) )
     {
          msimFileError( Instance->filename, Owner, ( USHORT ) msimWRITE_ERROR );
          fclose( f );
          return msimWRITE_ERROR;
     }

     /* close file and check for errors                                     */

     if ( EOF == fclose( f ) )
     {
          msimFileError( Instance->filename, Owner, ( USHORT )
               msimFILE_CLOSE_ERROR );
          return msimWRITE_ERROR;
     }

     /* if we got this far everything went ok                               */

     return msimNO_ERROR;
}



msimRC WriteNotebookTextToFile( msimPINSTANCE Instance, FILE PTR ThisFile )
{
     PCHAR char_ptr;
     LONG num_extra_chars;

     // this fcn created 2/28/94 to be consistent with notebook_size
     // usage in msimnb.cxx (WDH)

     if ( Instance->ptr_to_notebook_text != NULL )
     {
          Instance->notebook_size = ( ULONG ) ( strlen( Instance->ptr_to_notebook_text ) );
          fwrite( Instance->ptr_to_notebook_text, Instance->notebook_size, 1, ThisFile );
     }
     else
          Instance->notebook_size = 0UL;

     /* reserve some space in the file for editing/adding to the notebook   */
     /* text. The total space available in the file = msimNOTEBOOK_SIZE     */

     num_extra_chars = msimNOTEBOOK_SIZE - Instance->notebook_size;

     if ( num_extra_chars > 0L )
     {
          char_ptr = new CHAR[num_extra_chars];

          if ( ! char_ptr )
               return msimMEM_ALLOC_ERROR;

          memset( char_ptr, '\0', num_extra_chars );

          fwrite( char_ptr, num_extra_chars, 1, ThisFile );

          delete[]char_ptr;
     }

     return msimNO_ERROR;
}




/*--------------------------------------------------------------------------*/
/*                  msimLocateAppFile( )                                    */
/*..........................................................................*/
/*                                                                          */
/* this function searches for a file that is used by the simulator. The     */
/* function returns a TRUE is the search finds a                            */
/* file with the requested file name, otherwise returns FALSE if the search */
/* fails. Under OS/2 we look first in the current directory, then in the    */
/* directory where the original msim.exe file is found ( as pointed to by   */
/* the command line argument argv[0]). Finally we look in the directory     */
/* given by the environment variable named in msimENV_VAR.                  */
/*                                                                          */
/* Under AIX the search order is the same except we skip the search in the  */
/* directory of the msim executable since argv[0] does not contain full     */
/* pathnames in that instance                                               */
/*                                                                          */
/* If the search is successful the valid fullyqulified filename is copied   */
/* to Destination                                                           */
/*                                                                          */
/* File Search order :                                                      */
/*  1. in current directory                                                 */
/*  2. in directory where msim.exe is                                       */
/*  3. in directory specified in the msimENV_VAR environment variable       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimLocateAppFile( PCHAR BaseFileName, PCHAR Destination, size_t BufSize )
{
     msimBOOL found_it = FALSE;

     // initialize objects

     String base_filename( BaseFileName );
     DirEntry app_file( base_filename );

     // make fully qualified name based on current directory

     app_file.ToAbs( );

     if ( app_file.Exists( ) )
          found_it = TRUE;

     // now try to find the file in the same directory as the current program

     if ( ! found_it )
     {
          app_file = DirEntry( aMainApp.GetAppFileName( ) );
          app_file.ChangeName( base_filename );
          if ( app_file.Exists( ) )
               found_it = TRUE;
     }

     // finally try to find it in the msimENV_VAR path

     if ( ! found_it )
     {
          app_file = DirEntry( base_filename );
          found_it = app_file.Find( String( getenv( msimENV_VAR ) ) );
     }

     if ( found_it )
     {
          msimStringCopy( Destination, app_file.GetFull( ), BufSize );
          return TRUE;
     }
     else
          return FALSE;
}


#if 0
/*--------------------------------------------------------------------------*/
/*                        ResolveRelativePathInfo()                     */
/*..........................................................................*/
/*                                                                          */
/* adjusts a directory specification such that any references               */
/* to relative path information( ie. ".", ".." or no leading path info      */
/* implying cwd) are resolved so that the path spec  is fully qualified and */
/* starts at the root. I  attempt to deal with relative path info           */
/* imbedded anywhere in the string                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC ResolveRelativePathInfo( PCHAR Drive, PCHAR Directory, USHORT Size, msimWID Owner )
{
     msimFILE_STRING abs_dir;
     PCHAR end_of_target;
     PCHAR target;
     PCHAR source;
     msimRC rc;

     if ( *Directory == msimPATH_SEPARATOR )
          strcpy( abs_dir, Directory );// spec starts from root
     else
     {
          // we start with a relative path spec - append spec to current working dir
          // first get cwd spec

          if ( msimNO_ERROR != ( rc = GetDirectory( Drive, abs_dir, sizeof ( abs_dir ) ) ) )
          {
               ErrorBox( Owner, ResId( msimDIR_QUERY_ERROR ) ) .Execute( );
               return rc;
          }

          AddPathSeparator( abs_dir );
          msimStringCat( abs_dir, Directory, sizeof abs_dir );
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

          if ( *source == '.' && * ( source - 1 ) == msimPATH_SEPARATOR )
          {
               switch ( * ( source + 1 ) )
               {

               case  '.' :            // found the segment "/.." - keep checking

                    if ( * ( source + 2 ) == msimPATH_SEPARATOR )
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
                              while ( * --target != msimPATH_SEPARATOR )
                                   /* NULL*/;   // do nothing

                         target++;// move to position immediately after last path sep char

                         source += 3;// point just past "/../" segment in source
                    }
                    else
                         // it's just a subdir name staring with ".."
                         // just copy in the normal manner
                         *target++= *source++;

                    break;

               case msimPATH_SEPARATOR :

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

     return msimNO_ERROR;
}


#endif


void msimConstructFullyQualifiedFilename( PCHAR FileName, size_t BufSize )
{
     DirEntry file_name;

     file_name = DirEntry( String( FileName ) );

     file_name.ToAbs( );
     msimStringCopy( FileName, file_name.GetFull( ), BufSize );
}


msimBOOL msimFileAlreadyLoaded( msimWID Owner, PCHAR Name )
{
     msimPINSTANCE PTR p_instance_array = aMainApp.GetInstanceArray ( );

     for ( USHORT i = 0; i < msimMAX_NO_INSTANCES; i++ )
          if ( p_instance_array[i] )
               if ( msimStringsMatch( Name, p_instance_array[i]->filename, TEST_TYPE ) )
               {
                    String msg( "A file named " );
                    msg = msg + String( Name ) + String( " is already loaded in memory." );

                    InfoBox( Owner, msg ) .Execute( );

                    return TRUE;
               }

     return FALSE;
}

msimRC msimSaveEditorTextToFile( PCHAR EditorString, FILE PTR pFile )
{
     PCHAR ptr;

     msimSTRING tempbuff;

     // This function tokenizes a char buffer using \n as separator,
     // removes all trailing blanks, linefeeds, tabs and newline indicators and
     // replaces them with single newline indicator.  The contents of each
     // adjusted line is then saved into the file.

     // first check for the newline character, \n

     ptr = strtok( EditorString, "\n" );

     while ( ptr )
     {

          if ( strlen( ptr ) <= msimSTRING_LENGTH )
          {
               msimDeleteNonPrintableChars( ptr, tempbuff );

               fprintf( pFile, "%s\n", tempbuff );
          }

          ptr = strtok( NULL, "\n" );

     }

     return ferror( pFile ) ? msimWRITE_ERROR : msimNO_ERROR;
}


msimBOOL msimGetSizeOfFile( PCHAR FileName, PULONG pSize )
{

     DirEntry file_name;
     file_name = DirEntry( String( FileName ) );
     file_name.ToAbs( );

     FileStat fs( file_name );

     ULONG size = fs.GetSize ( );

     if ( FSYS_ERR_OK == fs.GetError( ) )
     {
          *pSize = size;
          return TRUE;
     }
     else
          return FALSE;
}


#if defined (__MAC__)

#include <mac_start.h>

#include <StandardFile.h>
#include <Errors.h>

#if defined(__PPC__)
#define SFSaveDisk  0x214
#define CurDirStore 0x398

#else
#include <SysEqu.h>
#endif

short   SavedCurrentVolume;
long    SavedCurrentDirectory;

/*--------------------------------------------------------------------------*/
/*                  AddPathSeparator()                                      */
/*..........................................................................*/
/*                                                                          */
/* adds a trailing slash or backslash (depending on value of PATH_SEPARATOR)*/
/* to a string. First checks if the last character is a already a path      */
/* separator. If so, returns without making any changes.                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void AddPathSeparator( PCHAR str )
{
     str = str + strlen( str ) - 1;

     if ( *str != msimPATH_SEPARATOR )
     {
          * ( ++str ) = msimPATH_SEPARATOR;
          * ( ++str ) = '\0';
     }
     return;
}



void  SaveCurDir( )
{
     short *p_cv = (short *) SFSaveDisk;
      long  *p_cd = (long *)  CurDirStore;

      SavedCurrentVolume =  - (*p_cv );  // note negative sign
      SavedCurrentDirectory =  *p_cd;
}


void SetCurDir( PCHAR DirSpec)
{
      OSErr                 err;
      FSSpec                file_spec;
      char                  filename[265];
      short                 *p_cv = (short *) SFSaveDisk;
       long                  *p_cd = (long *)  CurDirStore;



      // contruct a pascal-style string of a filename for making a FSSpec

      msimStringCopy( &filename[1], DirSpec, (sizeof filename) - 1 );
       AddPathSeparator( &filename[1]);
       msimStringCat( &filename[1], "q", sizeof(filename) -1 );   // any old thing so that the parent ID returned is the dir we are after

     filename[0] = (unsigned char) strlen( &filename[1]);

      // now make the FSSpec - we need this for LaunchApplication call

      err = FSMakeFSSpec( 0, 0, (ConstStr255Param) filename, &file_spec);

      // if no problem then set

     if (err == noErr || err == fnfErr)
     {
          *p_cv =  -file_spec.vRefNum ;  // note negative sign
            *p_cd = file_spec.parID ;
     }

}

void RestoreSavedDir()
{
     short *p_cv = (short *) SFSaveDisk;
      long  *p_cd = (long *)  CurDirStore;

     *p_cv =  -SavedCurrentVolume ;  // note negative sign
      *p_cd = SavedCurrentDirectory ;
}


#include <mac_end.h>

#endif
