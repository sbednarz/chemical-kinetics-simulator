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

//--------------------------------------------------------------------------
//                     MSIM Chemical Reaction Simulator
//                            -----------------
//
//                      by W. Hinsberg and F. Houle
//                      IBM Almaden Research Center
//                                  ----
//
//  FILE NAME : convert.cxx
//
//  This module  contains functions for converting a rxn file from a foreign
//  format to the format of the current OS
//
//  Written using the "Starview" libraries to provide common code for
//  multiple platforms
//
//  Version 1.0  started May 8 1994
//
//  variable naming conventions :
//     variables to a function all in lower case
//     variables global to a module but not to application in mixed case
//     variables global to the application in mixed case with "msim" prefix
//
//  This module contains significant OS-specific code, since the details of
//  the rxn file structure differ from OS to OS
//
//--------------------------------------------------------------------------

// the overall structure of the rxn file is summarized here for
// reference

//                                 offsets in decimal, sizes in byte
//
// COMPONENT                     OS/2 & DOS         MAC          AIX/PMAC
//
//                               size  ofst       size  ofst    size  ofst
// ---------------------------   -----------      ----------    ----------
// INSTANCE STRUCT  (1)                   0                0             0
//      overall size              1867            1870          1876
//
//   msimID                        13     0         13     0      13     0
// ... extra byte for  align                         1    13       1    13
// ... extra WORD for align                                        2    14
//   sim_data_file_offset           8    13          8    14       8    16
//   notebook_size                  4    21          4    22       4    24
//   ptr_to_rxnlist                 4    25          4    26       4    28
//   ptr_to_species_list            4    29          4    30       4    32
//   ptr_to_notebook_text           4    33          4    34       4    36
//   listbox_selection              2    37          2    38       2    40
//   numsteps                       2    39          2    40       2    42
//   nonzeroconcs                   2    41          2    42       2    44
//   speciescount                   2    43          2    44       2    46
//   variablepress                  4    45          4    46       4    48
//   specieslist_altered            4    49          4    50       4    52
//   sim_state_data_file_offset     8    53          8    54       8    56
//   data_altered_since_lastsave    4    61          4    62       4    64
//   enable_equil_detect            4    65          4    66       4    68
//   temp_prog_data_format          2    69          2    70       2    72
// ... extra WORD for alignment                                    2    74
//   reserved4bytes                 4    71          4    72       4    76
//   num_simulation_data_records    4    75          4    76       4    80
//   volume_option                  2    79          2    80       2    84
//   temp_option                    2    81          2    82       2    86
//   optionvalue ( 4 x 11)         44    83         44    84      44    88
//   p_plot_dialog                  4   127          4   128       4   132
//   p_notebook_window              4   131          4   132       4   136
//   sim_return_code                2   135          2   136       2   140
//   notebook_altered               2   137          2   138       2   142
//   operating_sys_flag             1   139          1   140       1   144
//   filename                     260   140        260   141     260   145
//   print_target                 260   400        260   401     260   405
//   plot_target                  260   660        260   661     260   665
// ... extra byte for align                          1   921       1   925
//   time_units                     2   920          2   922       2   926
//   energy_units                   2   922          2   924       2   928
//   conc_units                     2   924          2   926       2   930
//   pressure_units                 2   926          2   928       2   932
//   volume_units                   2   928          2   930       2   934
//   temp_data ( 8 x 15 )         120   930        120   932     120   936
//   session_id                     4  1050          4  1052       4  1056
//   equil_data                    26  1054         26  1056      26  1060
//   temp_profile_data            260  1080        260  1082     260  1086
//   elapsed_time_limit            15  1340         15  1342      15  1346
//   reserved15bytes               15  1355         15  1357      15  1361
//   execution_time_str            15  1370         15  1372      15  1376
//   base_filename                 33  1385         33  1387      33  1391
//   reserved_for_future          449  1418        449  1420     449  1424
// ... extra byte for  align                         1  1869 ?        1873
// ... extra WORD for align                                        2  1874
//
//
// msimRXN STRUCT (numsteps x)   starts at 1867 dec offset in OS/2 and DOS files
//                               starts at 1870 dec offset in MAC file
//                               starts at 1876 dec offset in AIX and PMAC file
//
//         overall size (bytes) = 415             424            428
//
//   equation                      77     0        77     0       77     0
//   ... extra byte for alignment                   1    77        1    77
//   numproducts                    2    79         2    80        2    78
//   reactantlist ( 2 + 11) x 4)   52    81        56    82       56    82
// ... each inc alignment byte on MAC & AIX after string element
//   productlist  ( 2 + 11) x 4)   52   133        56   138       56   138
// ... each inc alignment byte on MAC & AIX after string element
// .. extra WORD for alignment                                     2   194
//   next                           4   185         4   194        4   196
//   prev                           4   189         4   198        4   200
//   reversible                     4   193         4   202        4   204
//   not_stoich                     4   197         4   206        4   208
//   singlerate                     4   201         4   210        4   212
//   fwdA                          15   205        15   214       15   216
//   fwdM                          15   220        15   229       15   231
//   fwdEa                         15   235        15   244       15   246
//   revA                          15   250        15   259       15   261
//   revM                          15   265        15   274       15   276
//   revEa                         15   280        15   289       15   291
//   fwdexponent                   60   295        60   304       60   306
//   revexponent                   60   355        60   364       60   366
// .. extra WORD for alignment                                     2   426
//
// msimSPECIES STRUCT ( speciescount x )
//
//       overall size ( bytes)    130             132            132
//
//   name                           9    0          9     0        9     0
//   ... extra byte for alignment                   1     9        1     9
//   index                          2    9          2    10        2    10
//   nonzero                        4   11          4    12        4    12
//   thermcoeff ( 5 x 15 )         75   15         75    16       75    16
//   initialconc                   15   90         15    91       15    91
//   molardensity                  15  105         15   106       15   106
//   ... extra byte for alignment                   1   121        1   121
//   phys_state                     2  120          2   122        2   122
//   next                           4  122          4   124        4   124
//   prev                           4  126          4   128        4   128
//
// NOTEBOOK buffer               5000                5000          5000
//
// SIMULATION DATA only if simulation run
//    size =  num_simulation_data_records
//                            8            // size of Time - always recorded
//                       +    8           if var temp or prog temp
//                       +    8           if var pressure
//                       +    8           if var volume
//                       +  ( 4 * speciesc)  )// concentrations always recorded
//
// STATE_STRUCT ( only if simulation interrupted by user )
//
//   overall size                 182             182             184     192(PMAC)
//
//   InitialPhase                   4    0          4   0       4   0     4   0     ULONG
//   TempAndPressOption             4    4          4   4       4   4     4   4     ULONG
//   EquilDetectOption              4    8          4   8       4   8     4   8     ULONG
//   ProgTFileIndex                 4   12          4   12      4   12    4   12    ULONG
//   NewIndex                       4   16          4   16      4   16    4   16    ULONG
//   EventCount                     4   20          4   20      4   20    4   20    ULONG
//   EquilTestCycleCount            4   24          4   24      4   24    4   24    ULONG
//   NumberOfRecordsWritten         4   28          4   28      4   28    4   28    ULONG
//   TotalProbability               8   32          8   32      8   32    8   32    msimFLOAT
//   TotalRxnProbability            8   40          8   40      8   40    8   40    msimFLOAT
//   ParticlesPerMol                8   48          8   48      8   48    8   48    msimFLOAT
//   V                              8   56          8   56      8   56    8   56    msimFLOAT
//   P                              8   64          8   64      8   64    8   64    msimFLOAT
//   T                              8   72          8   72      8   72    8   72    msimFLOAT
//   TotalEnthalpy                  8   80          8   80      8   80    8   80    msimFLOAT
//   TotalHeatCapacity              8   88          8   88      8   88    8   88    msimFLOAT
//   ConversionFactor               8   96          8   96      8   96    8   96    msimFLOAT
//   MinFrequency                   8   104         8  104      8  104    8  104    msimFLOAT
//   PreviousTotalRxnProbability    8   112         8  112      8  112    8  112    msimFLOAT
//   ElapsedTime                    8   120         8  120      8  120    8  120    msimFLOAT
//   NoOfProgTFilePoints            4   128         4  128      4  128    4  128    ULONG
//   RandomNoSeed ( 3*2)            6   132         6  132      6  132    6  132    SHORT
// ... extra WORD for alignment                                 2  138    2  138
// ... extra DOUBLEWORD for alignment                                     4  140
// Tfunction data - part of STATE_STRUCT
//   FinalTemp                      8   138         8  138      8  140    8  144    msimFLOAT
//   MaxDeltaTemp                   8   146         8  146      8  148    8  152    msimFLOAT
//   MaxDeltaTime                   8   154         8  154      8  156    8  160    msimFLOAT
//   IdlingTimeStep                 8   162         8  162      8  164    8  168    msimFLOAT
//   TRamp                          8   170         8  170      8  172    8  176    msimFLOAT
//   Slope                          4   178         4  178      4  180    4  184    LONG
// ... extra DOUBLEWORD for alignment                                     4  188    
//
//
//   A lot of data follows the STATE_STRUCT :
//       array of concentrations , size = 4 * speciescount
//       array of rxn probs,              ( 2 * numsteps ) * 8
//       array of RxnEventCtr             ( 2 * numsteps ) * 4
//       array of EquilRxns flag          (  numsteps * 4 )
//       array of rate constants          ( 2 * numsteps ) * 8
//       array of UnitsConversion Exponts ( 2 * numsteps ) * 8

//   BYTE ORDERS              INTEL                           MAC
//                    lsb                msb         msb                  lsb
//
//   double           88 77 66 55 44 33 22 11        11 22 33 44 55 66 77 88
//   long                    44 33 22 11                 11 22 33 44
//   short                      22 11                        11 22
//


#include <sv.hxx>
#include <string.h>
#if defined(__MAC__)
#include <SysDep.hxx>
#endif

#define  MSIM2_HXX
#include "msim1.hxx"
#include "msimstrg.hxx"

#include "convert.hrc"

#define START_DLG 1000

// definitions of file flags

// bits are reserved/defined as follows :                             (every other byte)
//                    +-+--------------- alignment : 0 = byte-aligned; 1 = word-aligned;  2 = dbl-word-aligned( every 4th byte)
//                    | |
//                    | |
//                    | | +-+----------- text line terminator: 0 = 0xOD 0xOA,  1 = 0xOD,
//                    | | | |
//                    | | | |
//                    | | | | +--------- endian: 1 = Big, 0 = little
//                    | | | | |
//                    | | | | |
//                    | | | | | +-+-+--- file system type:   MAC, DOS, etc.
//                    | | | | | | | |
//    F E D C B A 0 9 8 7 6 5 4 3 2 1                                       */

#define DOS_FILE_SYSTEM        (USHORT) (0 << 0)
#define MAC_FILE_SYSTEM        (USHORT) (1 << 0)
#define OS2_FILE_SYSTEM        (USHORT) (2 << 0)
#define UNIX_FILE_SYSTEM       (USHORT) (3 << 0)
#define FILE_SYSTEM_MASK       (USHORT) (7 << 0)

#define LITTLE_ENDIAN          (USHORT) (0 << 3)
#define BIG_ENDIAN             (USHORT) (1 << 3)

#define TERM_0D_0A             (USHORT) (0 << 4)
#define TERM_0D                (USHORT) (1 << 4)
#define TERM_0A                (USHORT) (2 << 4)
#define TERM_MASK              (USHORT) (3 << 4)

#define BYTE_ALIGNED           (USHORT) (0 << 6)
#define WORD_ALIGNED           (USHORT) (1 << 6)
#define DBLWORD_ALIGNED        (USHORT) (2 << 6)

#define MAC_FLAGS   (MAC_FILE_SYSTEM  | BIG_ENDIAN    | WORD_ALIGNED    | TERM_0D    )
#define OS2_FLAGS   (DOS_FILE_SYSTEM  | LITTLE_ENDIAN | BYTE_ALIGNED    | TERM_0D_0A )
#define DOS_FLAGS   (DOS_FILE_SYSTEM  | LITTLE_ENDIAN | BYTE_ALIGNED    | TERM_0D_0A )
#define AIX_FLAGS   (UNIX_FILE_SYSTEM | BIG_ENDIAN    | DBLWORD_ALIGNED | TERM_0D_0A )
#define PMAC_FLAGS  (MAC_FILE_SYSTEM  | BIG_ENDIAN    | DBLWORD_ALIGNED | TERM_0D    )


#define UNKNOWN_FLAGS          (USHORT) 0xFFFF
#define ERROR_FLAGS            (USHORT) 0xFFFE


#if defined(__MAC__)
   #if defined(__PPC__)
      #define TARGET_FLAGS PMAC_FLAGS
   #else	  
      #define TARGET_FLAGS MAC_FLAGS
   #endif	  
#endif

#if defined(__MSDOS__)
#define TARGET_FLAGS DOS_FLAGS
#endif

#if defined(__OS2__)
#define TARGET_FLAGS OS2_FLAGS
#endif

#if defined(__AIX__)
#define TARGET_FLAGS AIX_FLAGS
#endif

void CreateNewFile( String& rNewFileName, String& rOldFileName );
USHORT GetSourceFileFlags( String& rFilename );
BOOL ConvertNotebookText( FILE *pFile, char *pBuffer, ULONG FileFlags );

msimINSTANCE RxnInstance;


class ConvertApp : public Application
{

public :
     ConvertApp( );
     virtual void Main( int argc, char *argv[] );

protected :
     virtual void UserEvent( ULONG nEvent, void PTR pEventData );


};

// instantiate application

ConvertApp aConvertApp;


class MainDialog : public ModalDialog
{
protected :
     FixedText HelpText;

#if defined(__PPC__)
     FixedBitmap aFixedIcon1;
#else
     FixedIcon aFixedIcon1;
#endif

     FixedText aFixedText2;
     FixedText aFixedText3;
     GroupBox aGroupBox1;
     PushButton StartConvPB;
     CancelButton CancelPB;
     GroupBox aGroupBox3;
     PushButton SelectFilePB;
     FixedText SourceFileName;
     FixedText RenamedFilename;

     BOOL ok_to_convert;

     BOOL DeriveNewFilename( String& rName );
     void CenterDialogOnScreen( );
     BOOL CreateNewFile( String& rNewFileName, String& rOldFileName );

public :

     MainDialog( Window * pParent );
     void CloseHandler( PushButton * pButton );
     void ConvertHandler( PushButton * pButton );
     void SelectHandler( PushButton * pButton );
};



MainDialog::MainDialog( Window * pParent ) :
ModalDialog ( pParent, ResId ( ID_MAIN_WINDOW ) ),
HelpText ( this, ResId ( ID_HELPTEXT ) ),
aFixedIcon1 ( this, ResId ( 1 ) ),
aFixedText2 ( this, ResId ( 2 ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
StartConvPB ( this, ResId ( ID_START_CONVERT ) ),
CancelPB ( this, ResId ( ID_QUIT ) ),
aGroupBox3 ( this, ResId ( 3 ) ),
SelectFilePB ( this, ResId ( ID_SELECT_FILE ) ),
SourceFileName ( this, ResId ( ID_SOURCE_FILE_NAME ) ),
RenamedFilename ( this, ResId ( ID_RENAME_ORIG_FILE ) )
{
     FreeResource( );
     ok_to_convert = FALSE;
     CenterDialogOnScreen( );

     GetParent( ) ->SetText( GetText( ) );
     StartConvPB.Disable( );

     HelpText.SetText( ResId( ID_INITIAL_HELP_STR ) );
     SourceFileName.SetText( ResId( NO_FILE_SELECTED_STR ) );
     RenamedFilename.SetText( String( "" ) );


     CancelPB.ChangeClickHdl( LINK( this, MainDialog, CloseHandler ) );
     StartConvPB.ChangeClickHdl( LINK( this, MainDialog, ConvertHandler ) );
     SelectFilePB.ChangeClickHdl( LINK( this, MainDialog, SelectHandler ) );
}


void MainDialog::CenterDialogOnScreen( )
{
     Size screen_size = System::GetScreenSizePixel( );

     Size dlg_size = GetSizePixel( );

     SHORT new_x, new_y;

     //  recalc coords for Dialog

     new_x = ( screen_size.Width( ) - dlg_size.Width( ) ) / 2;

     new_y = ( screen_size.Height( ) - dlg_size.Height( ) ) / 2;

     ChangePosPixel( Point( new_x, new_y ) );

}



void MainDialog::CloseHandler( PushButton PTR )
{
     EndDialog( );
}


void MainDialog::ConvertHandler( PushButton PTR )
{
     // first check file for validity by looking at ID in header

     USHORT source_file_flags = GetSourceFileFlags ( SourceFileName.GetText ( ) );

     if ( source_file_flags == TARGET_FLAGS )
     {
          InfoBox( NULL, ResId( ID_IN_NATIVE_FORMAT ) ) .Execute( );
          StartConvPB.Disable( );
          HelpText.SetText( ResId( ID_INITIAL_HELP_STR ) );
          SourceFileName.SetText( ResId( NO_FILE_SELECTED_STR ) );
          RenamedFilename.SetText( String( "" ) );

          return;
     }

     if ( source_file_flags == UNKNOWN_FLAGS )
     {
          InfoBox( NULL, ResId( ID_ERROR_NOT_MSIM_FILE ) ) .Execute( );
          StartConvPB.Disable( );
          HelpText.SetText( ResId( ID_INITIAL_HELP_STR ) );
          SourceFileName.SetText( ResId( NO_FILE_SELECTED_STR ) );
          RenamedFilename.SetText( String( "" ) );

          return;
     }

     // now rename/move the file named by SourceFilename to its backup name


     FSysError rc =
     DirEntry( SourceFileName.GetText( ) ) .MoveTo( DirEntry( RenamedFilename.GetText( ) ) );

     if ( rc != FSYS_ERR_OK )
     {
          String msg( ResId( ID_MOVE_FILE_MSG ) );
          msg += String(( int ) rc );
          ErrorBox( this, WB_OK | WB_DEF_OK, msg ) .Execute( );
          return;
     }

     HelpText.SetText( String( ResId( ID_CONVERTING_STR ) ) + SourceFileName.GetText( ) );

     if ( CreateNewFile( SourceFileName.GetText( ), RenamedFilename.GetText( ) ) )
          HelpText.SetText( SourceFileName.GetText( ) + String( ResId( ID_SUCCESS_CONV_STR ) ) );
     else
          HelpText.SetText( SourceFileName.GetText( ) + String( ResId( ID_FAILURE_CONV_STR ) ) );

     StartConvPB.Disable( );
     SourceFileName.SetText( ResId( NO_FILE_SELECTED_STR ) );
     RenamedFilename.SetText( String( "" ) );


}


USHORT DetermineSourceFlags( String& rFileID )
{
     if ( rFileID == String( msimRXN_FILE_FLG_DOS ) )
          return DOS_FLAGS;

     if ( rFileID == String( msimRXN_FILE_FLG_MAC ) )
          return MAC_FLAGS;

     if ( rFileID == String( msimRXN_FILE_FLG_PMAC ) )
          return PMAC_FLAGS;
		  
     if ( rFileID == String( msimRXN_FILE_FLG_OS2 ) )
          return OS2_FLAGS;

     if ( rFileID == String( msimRXN_FILE_FLG_AIX ) )
          return AIX_FLAGS;

     return UNKNOWN_FLAGS;

}

USHORT GetSourceFileFlags( String& rFilename )
{
     FILE *pf;
     char buffer[sizeof RxnInstance.msimID];

     if ( NULL == ( pf = fopen( rFilename, "rb" ) ) )
     {
          ErrorBox( NULL, ResId( ID_FILE_OPEN_ERROR ) ) .Execute( );
          return ERROR_FLAGS;
     }

     fread( &buffer, sizeof buffer, 1, pf );

     fclose( pf );

     return ( DetermineSourceFlags( String( buffer ) ) );
}



void ReverseByteOrder( char *pBuffer, size_t BufSize )
{
     char tempbuf[16];       // allocate buffer for > largest expected  size

     size_t size = BufSize;
     char *ptemp = pBuffer;

     while ( size-- > 0 )
          tempbuf[size] = *ptemp++;

     memcpy( pBuffer, &tempbuf, BufSize );
}



BOOL ConvertNotebookText( FILE *pFile, char *pBuffer, ULONG FileFlags )
{
     BOOL add_0x0A = (
          (
               ( ( TARGET_FLAGS & TERM_MASK ) == TERM_0D_0A )  // target needs 0x0A
               ||
               ( ( TARGET_FLAGS & TERM_MASK ) == TERM_0A )  // target needs 0x0A
          )
          &&
          (
               ( ( FileFlags & TERM_MASK ) != TERM_0D_0A )// source doesnt have it
               &&
               ( ( FileFlags & TERM_MASK ) != TERM_0A )// source doesnt have it
          )
     );

     BOOL add_0x0D = (
          (
               ( ( TARGET_FLAGS & TERM_MASK ) == TERM_0D_0A )  // target needs 0x0D
               ||
               ( ( TARGET_FLAGS & TERM_MASK ) == TERM_0D )  // target needs 0x0D
          )
          &&
          (
               ( ( FileFlags & TERM_MASK ) != TERM_0D_0A )// source doesnt have it
               &&
               ( ( FileFlags & TERM_MASK ) != TERM_0D )// source doesnt have it
          )
     );


     BOOL delete_0x0A = (
          (
               ( ( TARGET_FLAGS & TERM_MASK ) != TERM_0D_0A )// target doesn't use 0x0A
               &&
               ( ( TARGET_FLAGS & TERM_MASK ) != TERM_0A )// target doesn't use 0x0A
          )
          &&
          (
               ( ( FileFlags & TERM_MASK ) == TERM_0D_0A )// but source contains it
               ||
               ( ( FileFlags & TERM_MASK ) == TERM_0A )// but source contains it

          )
     );

     BOOL delete_0x0D = (
          (
               ( ( TARGET_FLAGS & TERM_MASK ) != TERM_0D_0A )// target doesn't use 0x0D
               &&
               ( ( TARGET_FLAGS & TERM_MASK ) != TERM_0D )// target doesn't use 0x0D
          )
          &&
          (
               ( ( FileFlags & TERM_MASK ) == TERM_0D_0A )// but source contains it
               ||
               ( ( FileFlags & TERM_MASK ) == TERM_0D )// but source contains it

          )
     );


     int num_chars_read = 0;
     int num_chars_saved = 0;

     char c;

     memset( pBuffer, '\0', msimNOTEBOOK_SIZE );

     while ( ( num_chars_read < msimNOTEBOOK_SIZE ) &&
               ( num_chars_saved < msimNOTEBOOK_SIZE )
          )
     {
          fread( &c, sizeof c, 1, pFile );

          if ( ferror( pFile ) )
               return FALSE;

          num_chars_read++;

          switch ( c )
          {

          case '\0' :

               *pBuffer = c;
               num_chars_saved++;
               RxnInstance.notebook_size = num_chars_saved;

               // advance the file ptr to end of notebook text block

               fseek( pFile, ( msimNOTEBOOK_SIZE - num_chars_read ), SEEK_CUR );
               return TRUE;

          default :

               *pBuffer++= c;
               num_chars_saved++;
               break;

          case '\x0D' :

               if ( delete_0x0D )
                    break;

               *pBuffer++= c;
               num_chars_saved++;

               if ( ( add_0x0A ) && ( num_chars_saved < msimNOTEBOOK_SIZE ) )
               {
                    *pBuffer++= '\x0A';
                    num_chars_saved++;
               }
               break;

          case '\x0A' :

               if ( delete_0x0A )
                    break;

               if ( add_0x0D )
               {
                    *pBuffer++= '\x0D';
                    num_chars_saved++;

                    if ( num_chars_saved < msimNOTEBOOK_SIZE )
                    {
                          *pBuffer++= c;
                          num_chars_saved++;
                    }
               }
               else
               {
                    *pBuffer++= c;
                    num_chars_saved++;
               }

          }
     }

     // if we get to here its becase we reached the buffer length limit
     // without finding the terminating zero
     // we add one now

     pBuffer--;
     *pBuffer = '\0';
     RxnInstance.notebook_size = msimNOTEBOOK_SIZE;
     return TRUE;

}




BOOL MainDialog::CreateNewFile( String& rNewFileName, String& rOldFileName )
{
     FILE *psource;
     FILE *ptarget;
     ULONG source_flags;
     ULONG i;

     aConvertApp.Wait( TRUE );

     // zero out the target struct

     memset( &RxnInstance, '\0', sizeof RxnInstance );

     // open the files and check for success

     psource = fopen( rOldFileName, "rb" );

     if ( ( psource == NULL ) )
     {
          ErrorBox( NULL, ResId( ID_FILE_OPEN_ERROR ) ) .Execute( );
          return FALSE;
     }

     // get the first 13 bytes of file and decode

     fread( &RxnInstance.msimID, sizeof RxnInstance.msimID, 1, psource );

     source_flags = DetermineSourceFlags( String( RxnInstance.msimID ) );

     BOOL has_alignment_words = ( 0 != ( source_flags & DBLWORD_ALIGNED ) );

     // if source has alignment words it also has alignment bytes

     BOOL has_alignment_bytes = ( ( 0 != ( source_flags & WORD_ALIGNED ) ) ||
                                 has_alignment_words ) ;

     BOOL byte_order_reversed = ( 0 != ( ( source_flags & BIG_ENDIAN ) ^ ( TARGET_FLAGS & BIG_ENDIAN ) ) );

     // now over-write with the msimID of the native system

     strcpy( RxnInstance.msimID, msimRXN_FILE_FLG );

     // skip alignment byte if necessary

     if ( has_alignment_bytes )
          fseek( psource, 1, SEEK_CUR );

     if ( has_alignment_words )
          fseek( psource, 2, SEEK_CUR );

     // read sim_data_file_offset

     fread( &RxnInstance.sim_data_file_offset, sizeof RxnInstance.sim_data_file_offset, 1,
          psource );

     // do we need to reverse byte ordering

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.sim_data_file_offset.offset,
               sizeof RxnInstance.sim_data_file_offset.offset );

     // read notebook_size

     fread( &RxnInstance.notebook_size, sizeof RxnInstance.notebook_size, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.notebook_size,
               sizeof RxnInstance.notebook_size );

     fread( &RxnInstance.ptr_to_rxnlist, sizeof RxnInstance.ptr_to_rxnlist, 1,
          psource );

     fread( &RxnInstance.ptr_to_species_list, sizeof RxnInstance.ptr_to_species_list, 1,
          psource );

     fread( &RxnInstance.ptr_to_notebook_text, sizeof RxnInstance.ptr_to_notebook_text, 1,
          psource );

     fread( &RxnInstance.listbox_selection, sizeof RxnInstance.listbox_selection, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.listbox_selection,
               sizeof RxnInstance.listbox_selection );


     fread( &RxnInstance.numsteps, sizeof RxnInstance.numsteps, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.numsteps,
               sizeof RxnInstance.numsteps );


     fread( &RxnInstance.nonzeroconcs, sizeof RxnInstance.nonzeroconcs, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.nonzeroconcs,
               sizeof RxnInstance.nonzeroconcs );


     fread( &RxnInstance.speciescount, sizeof RxnInstance.speciescount, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.speciescount,
               sizeof RxnInstance.speciescount );


     fread( &RxnInstance.variablepress, sizeof RxnInstance.variablepress, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.variablepress,
               sizeof RxnInstance.variablepress );

     fread( &RxnInstance.specieslist_altered, sizeof RxnInstance.specieslist_altered, 1,
          psource );

     RxnInstance.specieslist_altered = FALSE;// reset as in normal load of rxn file

     fread( &RxnInstance.sim_state_data_file_offset, sizeof RxnInstance.sim_state_data_file_offset, 1,
          psource );

     fread( &RxnInstance.data_altered_since_lastsave,
          sizeof RxnInstance.data_altered_since_lastsave, 1, psource );

     RxnInstance.data_altered_since_lastsave = FALSE;

     fread( &RxnInstance.enable_equil_detect, sizeof RxnInstance.enable_equil_detect, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.enable_equil_detect,
               sizeof RxnInstance.enable_equil_detect );

     fread( &RxnInstance.temp_prog_data_format, sizeof RxnInstance.temp_prog_data_format, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.temp_prog_data_format,
               sizeof RxnInstance.temp_prog_data_format );

     if ( has_alignment_words )
          fseek( psource, 2, SEEK_CUR );

     fread( &RxnInstance.reserved4bytes, sizeof RxnInstance.reserved4bytes, 1,
          psource );

     fread( &RxnInstance.num_simulation_data_records, sizeof RxnInstance.num_simulation_data_records, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.num_simulation_data_records,
               sizeof RxnInstance.num_simulation_data_records );

     fread( &RxnInstance.volume_option, sizeof RxnInstance.volume_option, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.volume_option,
               sizeof RxnInstance.volume_option );

     fread( &RxnInstance.temp_option, sizeof RxnInstance.temp_option, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.temp_option,
               sizeof RxnInstance.temp_option );

     fread( &RxnInstance.optionvalue, sizeof ( RxnInstance.optionvalue ), 1,
          psource );

     fread( &RxnInstance.p_plot_dialog, sizeof RxnInstance.p_plot_dialog, 1,
          psource );

     RxnInstance.p_plot_dialog = NULL;

     fread( &RxnInstance.p_notebook_window, sizeof RxnInstance.p_notebook_window, 1,
          psource );

     RxnInstance.p_notebook_window = NULL;

     fread( &RxnInstance.sim_return_code, sizeof RxnInstance.sim_return_code, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.sim_return_code,
               sizeof RxnInstance.sim_return_code );

     fread( &RxnInstance.notebook_altered,
          sizeof RxnInstance.notebook_altered, 1, psource );

     RxnInstance.notebook_altered = FALSE;

     fread( &RxnInstance.operating_sys_flag,
          sizeof RxnInstance.operating_sys_flag, 1, psource );

     RxnInstance.operating_sys_flag = msimOS_FLAG;

     fread( &RxnInstance.filename,
          sizeof RxnInstance.filename, 1, psource );

     msimStringCopy( RxnInstance.filename, rNewFileName, sizeof RxnInstance.filename );

     fread( &RxnInstance.print_target,
          sizeof RxnInstance.print_target, 1, psource );

     strcpy( RxnInstance.print_target, msimNULL_STR );

     fread( &RxnInstance.plot_target,
          sizeof RxnInstance.plot_target, 1, psource );

     strcpy( RxnInstance.plot_target, msimNULL_STR );

     if ( has_alignment_bytes )
          fseek( psource, 1, SEEK_CUR );

     fread( &RxnInstance.time_units, sizeof RxnInstance.time_units, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.time_units,
               sizeof RxnInstance.time_units );

     fread( &RxnInstance.energy_units, sizeof RxnInstance.energy_units, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.energy_units,
               sizeof RxnInstance.energy_units );


     fread( &RxnInstance.conc_units, sizeof RxnInstance.conc_units, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.conc_units,
               sizeof RxnInstance.conc_units );


     fread( &RxnInstance.pressure_units, sizeof RxnInstance.pressure_units, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.pressure_units,
               sizeof RxnInstance.pressure_units );

     fread( &RxnInstance.volume_units, sizeof RxnInstance.volume_units, 1,
          psource );

     if ( byte_order_reversed )
          ReverseByteOrder(( char * ) &RxnInstance.volume_units,
               sizeof RxnInstance.volume_units );


     fread( &RxnInstance.temp_data, sizeof RxnInstance.temp_data, 1,
          psource );

     fread( &RxnInstance.session_id, sizeof RxnInstance.session_id, 1,
          psource );
     RxnInstance.session_id = msimNO_SESSION;

     fread( &RxnInstance.equil_data, sizeof RxnInstance.equil_data, 1,
          psource );

     fread( &RxnInstance.temp_profile_data, sizeof RxnInstance.temp_profile_data, 1,
          psource );

     fread( &RxnInstance.elapsed_time_limit, sizeof RxnInstance.elapsed_time_limit, 1,
          psource );

     fread( &RxnInstance.reserved15bytes, sizeof RxnInstance.reserved15bytes, 1,
          psource );

     fread( &RxnInstance.execution_time_str, sizeof RxnInstance.execution_time_str, 1,
          psource );

     fread( &RxnInstance.base_filename, sizeof RxnInstance.base_filename, 1,
          psource );

     msimStringCopy( RxnInstance.base_filename, msimBaseFilename( RxnInstance.filename ),
          sizeof RxnInstance.base_filename );

     fread( &RxnInstance.reserved_for_future, sizeof RxnInstance.reserved_for_future, 1,
          psource );

     // more alignment byte(s) at end of header. Does it go here or before "reserved_for_future" ?
     // doesnt matter now but might if we use some of the reserved_for_future buffer

     if ( has_alignment_bytes )
          fseek( psource, 1, SEEK_CUR );

     if ( has_alignment_words )
          fseek( psource, 2, SEEK_CUR );


     // now give user a chance to enter temperature profile file name if necessary

     if ( ( RxnInstance.temp_option == msimPROGR_TEMP ) &&
               ( RxnInstance.temp_prog_data_format == msimFILE_TPROG )
          )
     {
          enum FSysPathStyle path_style;

          aConvertApp.Wait( FALSE );

          switch ( source_flags & FILE_SYSTEM_MASK )
          {

          case DOS_FILE_SYSTEM :

               path_style = FSYS_STYLE_FAT;
               break;

          case MAC_FILE_SYSTEM :

               path_style = FSYS_STYLE_MAC;
               break;

          case OS2_FILE_SYSTEM :

               path_style = FSYS_STYLE_HPFS;
               break;

          case UNIX_FILE_SYSTEM :

               path_style = FSYS_STYLE_UNX;
               break;

          default :

               path_style = FSYS_STYLE_DETECT;
               break;

          }


          String msg( ResId( ID_TEMP_PRO_NAME_STR1 ) );

          msg += DirEntry( String( RxnInstance.temp_profile_data ), path_style ) .GetName( );
          msg += String( ResId( ID_TEMP_PRO_NAME_STR2 ) );

          if ( RET_YES == QueryBox( this, WB_YES_NO | WB_DEF_YES, msg ) .Execute( ) )
          {
               // invoke filedlg

               FileDialog aDlg( this, WB_OPEN | WB_SVLOOK );

               aDlg.SetText( ResId( ID_TEMP_PRO_SELECT_STR ) );

               aDlg.RemoveAllFilter( );

               if ( RET_CANCEL != aDlg.Execute( ) )
                    msimStringCopy( RxnInstance.temp_profile_data, aDlg.GetPath( ),
                         sizeof RxnInstance.temp_profile_data );
          }

          aConvertApp.Wait( TRUE );
     }



     // now write to the output file

     ptarget = fopen( rNewFileName, "wb" );

     if ( ( ptarget == NULL ) )
     {
          ErrorBox( NULL, ResId( ID_FILE_OPEN_ERROR ) ) .Execute( );
          aConvertApp.Wait( FALSE );
          fclose( psource );
          return FALSE;
     }

     fwrite( &RxnInstance, sizeof RxnInstance, 1, ptarget );

     // AT THIS POINT WE HAVE FINISHED READING THE INSTANCE HEADER - NOW WE READ IN
     // THE REACTION STEP STRUCTS

     msimRXN rxn;


     i = RxnInstance.numsteps;

     while ( i-- )
     {
          memset( &rxn, '\0', sizeof rxn );

          fread( &rxn.equation, sizeof rxn.equation, 1, psource );

          if ( has_alignment_bytes )
               fseek( psource, 1, SEEK_CUR );

          fread( &rxn.numreactants, sizeof rxn.numreactants, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &rxn.numreactants,
                    sizeof rxn.numreactants );

          fread( &rxn.numproducts, sizeof rxn.numproducts, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &rxn.numproducts,
                    sizeof rxn.numproducts );

          USHORT j = 0;

          while ( j < msimMAX_NUMBER_OF_COMPONENTS )
          {
               fread( &( rxn.reactantlist[j].speciesindex ),
                    sizeof ( rxn.reactantlist[j].speciesindex ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) & ( rxn.reactantlist[j].speciesindex ),
                         sizeof ( rxn.reactantlist[j].speciesindex ) );

               fread( &( rxn.reactantlist[j].stoichcoeff ),
                    sizeof ( rxn.reactantlist[j].stoichcoeff ), 1, psource );

               if ( has_alignment_bytes )
                    fseek( psource, 1, SEEK_CUR );

               j++;
          }


          j = 0;

          while ( j < msimMAX_NUMBER_OF_COMPONENTS )
          {
               fread( &( rxn.productlist[j].speciesindex ),
                    sizeof ( rxn.productlist[j].speciesindex ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) & ( rxn.productlist[j].speciesindex ),
                         sizeof ( rxn.productlist[j].speciesindex ) );

               fread( &( rxn.productlist[j].stoichcoeff ),
                    sizeof ( rxn.productlist[j].stoichcoeff ), 1, psource );

               if ( has_alignment_bytes )
                    fseek( psource, 1, SEEK_CUR );

               j++;
          }

          if ( has_alignment_words )
               fseek( psource, 2, SEEK_CUR );

          fread( &rxn.next, sizeof rxn.next, 1, psource );
          fread( &rxn.prev, sizeof rxn.prev, 1, psource );

          fread( &rxn.reversible, sizeof rxn.reversible, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &rxn.reversible,
                    sizeof rxn.reversible );

          fread( &rxn.not_stoich, sizeof rxn.not_stoich, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &rxn.not_stoich,
                    sizeof rxn.not_stoich );

          fread( &rxn.singlerate, sizeof rxn.singlerate, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &rxn.singlerate,
                    sizeof rxn.singlerate );


          fread( &rxn.fwdA, sizeof rxn.fwdA, 1, psource );
          fread( &rxn.fwdM, sizeof rxn.fwdM, 1, psource );
          fread( &rxn.fwdEa, sizeof rxn.fwdEa, 1, psource );

          fread( &rxn.revA, sizeof rxn.revA, 1, psource );
          fread( &rxn.revM, sizeof rxn.revM, 1, psource );
          fread( &rxn.revEa, sizeof rxn.revEa, 1, psource );

          fread( &rxn.fwdexponent, sizeof rxn.fwdexponent, 1, psource );
          fread( &rxn.revexponent, sizeof rxn.revexponent, 1, psource );

          fwrite( &rxn, sizeof rxn, 1, ptarget );

          if ( has_alignment_words )
               fseek( psource, 2, SEEK_CUR );


     }


     // AT THIS POINT ALL THE RXN DATA IS READ - NOW READ SPECIES DATA

     msimSPECIES species;

     i = RxnInstance.speciescount;

     while ( i-- )
     {
          fread( &species.name, sizeof species.name, 1, psource );

          if ( has_alignment_bytes )
               fseek( psource, 1, SEEK_CUR );

          fread( &species.index, sizeof species.index, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &species.index,
                    sizeof species.index );

          fread( &species.nonzero, sizeof species.nonzero, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &species.nonzero,
                    sizeof species.nonzero );

          fread( &species.thermcoeff, sizeof species.thermcoeff, 1, psource );
          fread( &species.initialconc, sizeof species.initialconc, 1, psource );
          fread( &species.molardensity, sizeof species.molardensity, 1, psource );

          if ( has_alignment_bytes )
               fseek( psource, 1, SEEK_CUR );

          fread( &species.phys_state, sizeof species.phys_state, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &species.phys_state,
                    sizeof species.phys_state );

          fread( &species.next, sizeof species.next, 1, psource );

          fread( &species.prev, sizeof species.prev, 1, psource );



          fwrite( &species, sizeof species, 1, ptarget );
     }

     // AT THIS POINT ALL SPECIES DATA HAS BEEN TRANSFERRED
     // NOW DO THE NOTEBOOK TEXT

     char *pbuffer = new char[msimNOTEBOOK_SIZE];

     if ( pbuffer == NULL )
     {
          aConvertApp.Wait( FALSE );
          ErrorBox( this, ResId( ID_MEMORY_ERROR ) ) .Execute( );
          fclose( psource );
          fclose( ptarget );
          return FALSE;
     }

     if ( ! ConvertNotebookText( psource, pbuffer, source_flags ) )
     {
          aConvertApp.Wait( FALSE );
          ErrorBox( this, ResId( ID_FILE_READ_ERROR ) ) .Execute( );
          fclose( psource );
          fclose( ptarget );

          delete[]pbuffer;

          return FALSE;
     }


     fwrite( pbuffer, msimNOTEBOOK_SIZE, 1, ptarget );

     delete[]pbuffer;

     RxnInstance.sim_data_file_offset.offset = ftell( ptarget );

     if ( -1L == RxnInstance.sim_data_file_offset.offset )
     {
          ErrorBox( this, ResId( ID_SEEK_ERROR ) ) .Execute( );
          fclose( ptarget );
          fclose( psource );
          aConvertApp.Wait( FALSE );
          return FALSE;
     }


     // AT THIS POINT WE ARE AT THE SIMULATION DATA IS THERE IS ANY

     msimFLOAT tmpf;

     i = RxnInstance.num_simulation_data_records;

     while ( i-- )
     {
          // read the time value - it is ALWAYS there

          fread( &tmpf, sizeof tmpf, 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &tmpf,
                    sizeof tmpf );

          fwrite( &tmpf, sizeof tmpf, 1, ptarget );

          // next the temperature value - recorded only if variable or progr temp

          if ( ( RxnInstance.temp_option == msimVAR_TEMP ) ||
                    ( RxnInstance.temp_option == msimPROGR_TEMP ) )
          {
               fread( &tmpf, sizeof tmpf, 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &tmpf,
                         sizeof tmpf );

               fwrite( &tmpf, sizeof tmpf, 1, ptarget );
          }

          if ( RxnInstance.variablepress == TRUE )
          {
               fread( &tmpf, sizeof tmpf, 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &tmpf,
                         sizeof tmpf );

               fwrite( &tmpf, sizeof tmpf, 1, ptarget );
          }

          if ( RxnInstance.volume_option == msimVAR_VOL )
          {
               fread( &tmpf, sizeof tmpf, 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &tmpf,
                         sizeof tmpf );

               fwrite( &tmpf, sizeof tmpf, 1, ptarget );
          }

          USHORT j = RxnInstance.speciescount;
          ULONG tmp_ul;

          while ( j-- )
          {
               fread( &tmp_ul, sizeof tmp_ul, 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &tmp_ul,
                         sizeof tmp_ul );

               fwrite( &tmp_ul, sizeof tmp_ul, 1, ptarget );
          }


     }

     // AT THIS POINT WE HAVE READ ALL THE SIMULATION RESULTS
     // THE ONLY THING THAT MIGHT BE REMAINING IS SIMULATION
     // STATE DATA IF THE USER INTERRUIPTED THE SIMULATION

     if ( RxnInstance.sim_return_code == msimUSER_ABORT )
     {
          RxnInstance.sim_state_data_file_offset.offset = ftell( ptarget );

          if ( -1L == RxnInstance.sim_state_data_file_offset.offset )
          {
               ErrorBox( this, ResId( ID_SEEK_ERROR ) ) .Execute( );

               fclose( ptarget );
               fclose( psource );

               aConvertApp.Wait( FALSE );
               return FALSE;
          }
     }
     else
          RxnInstance.sim_state_data_file_offset.offset = 0L;

// now get all the data

     if ( RxnInstance.sim_return_code == msimUSER_ABORT )
     {
          char buffer[8];

          USHORT ctr;

          ctr = 8;

          // initial phase
          // tmep and press option
          // equili detect option
          // ProgTFileINdex
          // NewIndex
          // EventCount
          // EquilTestCycleCount
          // NumberOfRecordsWritten

          while ( ctr-- )
          {
               fread( &buffer, sizeof ( unsigned long ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( unsigned long ) );

               fwrite( &buffer, sizeof ( unsigned long ), 1, ptarget );
          }

          ctr = 12;

          // TotalProbasbility
          // TotalRxnProbability
          // PArticlesPerMol
          // V
          // P
          // T
          // TotalEnthalpy
          // TotalHeatCapacity
          // ConversionFactor
          // MinFrequency
          // PReviousTotalRxnProbability
          // ElapsedTime

          while ( ctr-- )
          {
               fread( &buffer, sizeof ( double ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( double ) );

               fwrite( &buffer, sizeof ( double ), 1, ptarget );
          }

          // NoOfProgTFilePts

          fread( &buffer, sizeof ( unsigned long ), 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &buffer, sizeof ( unsigned long ) );

          fwrite( &buffer, sizeof ( unsigned long ), 1, ptarget );

          ctr = 3;

          // the random number seeds - three short ints

          while ( ctr-- )
          {
               fread( &buffer, sizeof ( unsigned short ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( unsigned short ) );

               fwrite( &buffer, sizeof ( unsigned short ), 1, ptarget );
          }
		  
		  // deal with some alignment issues 

          // AIX has one alignment word here - if the source file is from AIX then
		  // move forward one word (2 bytes) at this point

          if ( AIX_FLAGS == source_flags )
               fseek( psource, 2, SEEK_CUR );
			   
          // PoweMac has three alignment words here - if the source file is from PowerMac then
		  // move forward three word (6 bytes) at this point

          if ( PMAC_FLAGS == source_flags)
               fseek( psource, 6, SEEK_CUR );

#if defined (__AIX__)

          // AIX target has an alignment word here, so we need to insert two bytes here
          // we just set them to zero

          buffer[0] = '\0'; buffer[1] = '\0';
          fwrite( &buffer, 2, 1, ptarget );
#endif


#if defined(__MAC__) && defined (__PPC__)

          // PowerMac target has 3 alignment words, so we need to insert SIX bytes here
          // we just set them to zero

          buffer[0] = '\0'; buffer[1] = '\0'; buffer[2] = '\0'; 
		  buffer[3] = '\0'; buffer[4] = '\0'; buffer[5] = '\0';
		  
          fwrite( &buffer, 6, 1, ptarget );
		  
#endif

          // now tfunction - part of STATE_STRUCT

          ctr = 5;

          // Finaltemp
          // MaxDeltaTemp
          // MaxDeltaTime
          // IdlingTimeStep
          // TRamp

          while ( ctr-- )
          {
               fread( &buffer, sizeof ( double ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( double ) );

               fwrite( &buffer, sizeof ( double ), 1, ptarget );
          }

          // finally the slope

          fread( &buffer, sizeof ( long ), 1, psource );

          if ( byte_order_reversed )
               ReverseByteOrder(( char * ) &buffer, sizeof ( long ) );

          fwrite( &buffer, sizeof ( long ), 1, ptarget );
		  
		  
		  // PowerMac has another two alignment words here - dela with it
		  // if the source file is from PowerMac
		  
		  if ( PMAC_FLAGS == source_flags)
               fseek( psource, 4, SEEK_CUR );


#if defined(__MAC__) && defined (__PPC__)

          // PowerMac target has two alignment words here, so we need to insert four bytes here
          // we just set them to zero

          buffer[0] = '\0'; buffer[1] = '\0'; buffer[2] = '\0'; buffer[3] = '\0';
		  
          fwrite( &buffer, 4, 1, ptarget );
		  
#endif



          // There is a bunch of other info appended after STATE_STRUCT

          // now read the whole array of concentrations one at a time

          i = RxnInstance.speciescount;

          while ( i-- )
          {
               fread( &buffer, sizeof ( unsigned long ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( unsigned long ) );

               fwrite( &buffer, sizeof ( unsigned long ), 1, ptarget );
          }


          // read the the current rxnprobs

          i = RxnInstance.numsteps * 2;

          while ( i-- )
          {
               fread( &buffer, sizeof ( double ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( double ) );

               fwrite( &buffer, sizeof ( double ), 1, ptarget );
          }

          // RxnEventCtr

          i = RxnInstance.numsteps * 2;

          while ( i-- )
          {
               fread( &buffer, sizeof ( long ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( long ) );

               fwrite( &buffer, sizeof ( long ), 1, ptarget );
          }


          // EquilRxns

          i = RxnInstance.numsteps;

          while ( i-- )
          {
               fread( &buffer, sizeof ( unsigned long ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( unsigned long ) );

               fwrite( &buffer, sizeof ( unsigned long ), 1, ptarget );
          }


          // the Rate Constant Values and UnitsConversionExponent

          i = RxnInstance.numsteps * 2;

          while ( i-- )
          {
               // the Rate Constant

               fread( &buffer, sizeof ( double ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( double ) );

               fwrite( &buffer, sizeof ( double ), 1, ptarget );

               // the UnitsConversionExponent

               fread( &buffer, sizeof ( double ), 1, psource );

               if ( byte_order_reversed )
                    ReverseByteOrder(( char * ) &buffer, sizeof ( double ) );

               fwrite( &buffer, sizeof ( double ), 1, ptarget );
          }

     }


     // now rewrite the first record to the file, which now
     // contains updated values for  file offsets

     rewind( ptarget );

     fwrite( &RxnInstance, sizeof ( RxnInstance ), 1, ptarget );

     // done - clean up and go home

     if ( ferror( psource ) )
     {
          aConvertApp.Wait( FALSE );
          ErrorBox( this, ResId( ID_FILE_READ_ERROR ) ) .Execute( );
          fclose( psource );
          return FALSE;
     }


     fclose( psource );
     fclose( ptarget );

#if defined(__MAC__)

     Sysdepen::SetFileInfo( rNewFileName, String( msimRXNFILE_TYPE),
                  String( msimCREATOR_NAME) );

#endif


     aConvertApp.Wait( FALSE );

     return TRUE;
}



void MainDialog::SelectHandler( PushButton PTR )
{
     FileDialog aDlg( this, WB_OPEN | WB_SVLOOK );

     aDlg.SetText( ResId( ID_FILE_DLG_TITLE ) );

     aDlg.RemoveAllFilter( );

     if ( RET_CANCEL == aDlg.Execute( ) )
          return;
     
	 String filename = aDlg.GetPath( );

     if ( DirEntry( filename ) .Exists( ) )
     {
          SourceFileName.SetText( filename );
          ok_to_convert = TRUE;
     }
     else
     {
          ok_to_convert = FALSE;
          return;
     }

     if ( ! DeriveNewFilename( filename ) )
     {
          ErrorBox( this, ResId( ID_NEW_NAME_ERROR ) ) .Execute( );
          ok_to_convert = FALSE;
          return;
     }

     StartConvPB.Enable( );
     HelpText.SetText( ResId( ID_READY_TO_GO_STRING ) );

     return;
}

BOOL MainDialog::DeriveNewFilename( String& rName )
{
     DirEntry new_name( rName );

     for ( int i = 0; i < 999; i++ )
     {
          new_name.ChangeExtension( String( i ) );

          if ( ! new_name.Exists( ) )
               break;
     }

     if ( i > 998 )
          return FALSE;

     else
     {
          RenamedFilename.SetText( new_name.GetFull( ) );
          return TRUE;
     }
}

// this declares the class InstallApp


ConvertApp::ConvertApp( ) : Application ( )
{
}


void ConvertApp::UserEvent( ULONG nEvent, void PTR )
{

     if ( nEvent == START_DLG )
     {
          MainDialog main_dlg( GetAppWindow( ) );
          main_dlg.Execute( );

          Quit( );
     }

}



// ------ definition of MainApp::Main() -------

void ConvertApp::Main( int argc, char *argv[] )
{
     // initialization

     EnableSVLook( );

     // creat app window, but we never actually display it

     WorkWindow AppWindow( NULL, WB_STDWORK | WB_SVLOOK | WB_APP );
	 
#if 0	
     // delta wdh 10.14.96 on pmac port
	// previously was
     WorkWindow AppWindow = WorkWindow( NULL, WB_STDWORK | WB_SVLOOK | WB_APP );
#endif	 

     // start the event/msg processing

#if defined(__MAC__)

     // on mac AppWindow must be shown to
     AppWindow.ChangeSizePixel( Size( 0, 0 ) );
     AppWindow.ChangePosPixel( Point( 0, 0 ) );
     AppWindow.Show();

#endif

     PostUserEvent( START_DLG, ( void * ) NULL );
     Execute( );
}



