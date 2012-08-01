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
   |  instcfg.cxx   :   a component of the MSIM installation utility        |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    :   this module builds an initial msim.CNF file con-   |
   |                     taining the directories initialized to appropriate |
   |                     settings.                                          |
   |                                                                        |
   |  author         :   Bill Hinsberg                    IBM Almaden       |
   |                                                                        |
   |  file created   :   Feb 15, 1994                                       |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop

#include <stdlib.h>
#include <string.h>

#undef __MSDOS__
#include "install.hxx"


#define DEMOS_DIRECTORY         "\\demos\\"
#define TEXT_OUTPUT_DIRECTORY   "\\output\\"
#define GRAPH_OUTPUT_DIRECTORY  "\\output\\"
#define EXTDATA_DIRECTORY       "\\extdata\\"
#define TPROFILES_DIRECTORY     "\\tprofile\\"


     //
static msimAPP_OPTIONS AppOptions =
                       {               /* app_options                         */
                            msimCFG_FILE_FLG,/* msimID[];                     */
                            msimTIME_SECONDS,/* time_units,                   */
                            msimENERGY_CALS,/* energy_units,                  */
                            msimCONC_MOLES_LITER,/* conc_units;               */
                            msimPRESS_ATM,/* pressure_units                   */
                            msimVOL_LITERS,/* volume_units                    */
                            msimRXNFILE_TEMPLATE,/* data_template;            */
                            msimTEXTOUT_FILE_TEMPLATE,/* text_output_template;*/
                            msimGRAPHOUT_FILE_TEMPLATE,/* graph_output_template;*/
                            msimEXTERN_FILE_TEMPLATE,/* external_file_template;*/
                            msimEXT_T_PROFILE_TEMPLATE,/* external_t_profile_template*/
                            TRUE,      /* save_plot_window_data;              */
                            TRUE,      /* confirm_on_overwrite;               */
                            msimDEFAULT_POINT_SIZE,/* fontsize                */
                            msimDEFAULT_TYPEFACE,/* fonttype                  */
                            TRUE,      /* vary_colors;                        */
                            FALSE,     /* vary_linestyle;                     */
                            TRUE,      /* vary_markerstyle;                   */
                            TRUE,      /* show_lines;                         */
                            TRUE,      /* show_markers;                       */
                            FALSE,     /* show_filename;                      */
                            FALSE,     /* encapsulated_ps                     */
                            TRUE,      /* landscape                           */
                            TRUE,      /* plotfile_conv_color                 */
                            ( USHORT ) FALSE,/* show_grid                     */
                            msimPLOTFILE_TEXT,/* default_plotfile             */
                            msimGRAY_BKGRD,/*  default_colorset;              */
                            {          /* plot_window_position                */
                                 0, 0, 0, 0
                            }
                            ,
                            {          /* note_dialog_position                */
                                 0, 0, 0, 0
                            }
                            ,
                            {          /* help_window_position                */
                                 0, 0, 0, 0
                            }
                            ,
                            {          /* plot_dialog_position                */
                                 0, 0, 0, 0
                            },
                            {          /* main_window_position                */
                                 0, 0, 0, 0
                            }
                       };


int BuildConfigFile( const PCHAR ConfigFilename, const PCHAR HomeDir )
{
     FILE *f;

     // update appropriate fields in struct

     strcpy( AppOptions.data_template, HomeDir );
     strcat( AppOptions.data_template, DEMOS_DIRECTORY );
     strcat( AppOptions.data_template, msimRXNFILE_TEMPLATE );

     strcpy( AppOptions.text_output_template, HomeDir );
     strcat( AppOptions.text_output_template, TEXT_OUTPUT_DIRECTORY );
     strcat( AppOptions.text_output_template, msimTEXTOUT_FILE_TEMPLATE );

     strcpy( AppOptions.graph_output_template, HomeDir );
     strcat( AppOptions.graph_output_template, GRAPH_OUTPUT_DIRECTORY );
     strcat( AppOptions.graph_output_template, msimGRAPHOUT_FILE_TEMPLATE );

     strcpy( AppOptions.external_file_template, HomeDir );
     strcat( AppOptions.external_file_template, EXTDATA_DIRECTORY );
     strcat( AppOptions.external_file_template, msimEXTERN_FILE_TEMPLATE );

     strcpy( AppOptions.external_t_profile_template, HomeDir );
     strcat( AppOptions.external_t_profile_template, TPROFILES_DIRECTORY );
     strcat( AppOptions.external_t_profile_template, msimEXT_T_PROFILE_TEMPLATE );


     // now write the struct out to cfg file in appropriate directory

     // make the file name


     f = fopen( ConfigFilename, "wb" );

     if ( ! f )
     {
          return ( FAILURE );
     }


     fwrite( &AppOptions, sizeof ( msimAPP_OPTIONS ), 1, f );

     if ( ferror( f ) )
     {
          return ( FAILURE );
     }

     if ( EOF == fclose( f ) )
     {
          return ( FAILURE );
     }

     return ( SUCCESS );

}



// argv[1] is full name of configfile to update, argv[2] is fully
// qualified home directory

void main( int argc, char *argv[] )
{
     if ( argc != 3 )
          exit(1);


     if ( FAILURE == BuildConfigFile( argv[1], argv[2] ) )
          exit (2);

     exit( 0 );
}


