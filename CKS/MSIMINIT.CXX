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
   |  msimInit.cxx     :  initialization routines for MSIM                  |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  file created   :   Aug 12 1993                                        |
   |                                                                        |
   |  this module contains various routines for initializing data structures|
   |  and application variables. Included is a routine for getting          |
   |  and putting application initialization to a file                      |
   |  Also included are the initialization and shutdown routines for the    |
   |  overall application                                                   |
   |                                                                        |
   []----------------------------------------------------------------------[]*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimfile.hxx"
#include "msimque.hxx"

#include <stdlib.h>
#include <string.h>

#if defined(__AIX__) || defined(__MAC__)
#include <errno.h>
#endif

/* the following define the position of the main msim window in percent     */
/* of screen dimensions offset from the origin at upper left                */

#define  MAINWIN_TOP                             5
#define  MAINWIN_LEFT                            5

/* the following define the size of the main window in percent              */
/* of screen dimensions    div 10                                           */

// currently 60 % x 70 %
#define  MAINWIN_WIDTH                           6
#define  MAINWIN_HEIGHT                          7

/* variables global to program                                              */

msimAPP_OPTIONS msimAppOptions;

msimAPP_OPTIONS msimC_APP_OPTIONS =
                {                      /* app_options                         */
                     msimCFG_FILE_FLG, /* msimID[];                           */
                     msimTIME_SECONDS, /* time_units,                         */
                     msimENERGY_CALS,  /* energy_units,                       */
                     msimCONC_MOLES_LITER,/* conc_units;                      */
                     msimPRESS_ATM,    /* pressure_units                      */
                     msimVOL_LITERS,   /* volume_units                        */
                     msimRXNFILE_TEMPLATE,/* data_template;                   */
                     msimTEXTOUT_FILE_TEMPLATE,/* text_output_template;       */
                     msimGRAPHOUT_FILE_TEMPLATE,/* graph_output_template;     */
                     msimEXTERN_FILE_TEMPLATE,/* external_file_template;      */
                     msimEXT_T_PROFILE_TEMPLATE,/* external_t_profile_template*/
                     TRUE,             /* save_plot_window_data;              */
                     TRUE,             /* confirm_on_overwrite;               */
                     msimDEFAULT_POINT_SIZE,/* fontsize                       */
                     msimDEFAULT_TYPEFACE,/* fonttype                         */
                     TRUE,             /* vary_colors;                        */
                     FALSE,            /* vary_linestyle;                     */
                     TRUE,             /* vary_markerstyle;                   */
                     TRUE,             /* show_lines;                         */
                     TRUE,             /* show_markers;                       */
                     FALSE,            /* show_filename;                      */
                     FALSE,            /* encapsulated_ps                     */
                     TRUE,             /* landscape                           */
                     TRUE,             /* plotfile_conv_color                 */
                     ( USHORT ) FALSE, /* show_grid                           */
                     msimPLOTFILE_TEXT,/* default_plotfile                    */
                     msimGRAY_BKGRD,   /*  default_colorset;                  */
                     {                 /* plot_window_position                */
                          0, 0, 0, 0
                     }
                     ,
                     {                 /* note_dialog_position                */
                          0, 0, 0, 0
                     }
                     ,
                     {                 /* help_window_position                */
                          0, 0, 0, 0
                     }
                     ,
                     {                 /* plot_dialog_position                */
                          0, 0, 0, 0
                     },
                     {                 /* main_window_position                */
                          0, 0, 0, 0
                     }
                };


msimINSTANCE msimC_INSTANCE =
             {
                  msimRXN_FILE_FLG,    /* msimID[];                           */
                  {
                       {
                            0L, 0L
                       }
                  }
                  ,                    /* sim_data_file_offset;               */
                  0L,                  /* notebook_size;                      */
                  NULL,                /* ptr_to_rxnlist;                     */
                  NULL,                /* ptr_to_species_list;                */
                  NULL,                /* ptr_to_notebook_text;               */
                  0,                   /* listbox_selection;                  */
                  0,                   /* numsteps,                           */
                  0,                   /* nonzeroconcs,                       */
                  0,                   /* speciescount;                       */
                  FALSE,               /* variablepress,                      */
                  FALSE,               /* rxnschemealtered,                   */
                  {
                       {
                            0L, 0L
                       }
                  }
                  ,                    /* sim_state_data_file_offset;         */
                  FALSE,               /* data_altered_since_lastsave;        */
                  TRUE,                /* enable equil detect                 */
                  msimANALYTIC_TPROG,  /* temp_prog_data_format;              */
                  0L,                  /* num_temp_pro_data_pts;              */
                  0L,                  /* num_simulation_data_records;        */
                  msimDONT_CARE_VOL,   /* volume option                       */
                  msimCONST_TEMP,      /* temperature option                  */
                  {
                       msimINIT_PARTICLES,/* optionvalue; msimPARTICLES       */
                       msimINIT_EVENTS,/* msimEVENTS                          */
                       msimINIT_INTERVAL,/* msimINTERVAL                      */
                       msimINIT_SEED   /* msimSEED                            */
                  }
                  , msimWID_INVALID,   /* p_plot_dialog                       */
                  msimWID_INVALID,     /* p_notebook_window                   */
                  msimNO_ERROR,        /* sim_return_code;                    */
                  ( USHORT ) FALSE,    /* notebook_altered                    */
                  msimOS_FLAG,         /* operating_sys_flag;                 */
                  msimINIT_FILENAME,   /* filename,                           */
                  msimNULL_STR,        /* print_target,                       */
                  msimNULL_STR,        /* plot_target;                        */
                  msimTIME_SECONDS,    /* time_units,                         */
                  msimENERGY_CALS,     /* energy_units,                       */
                  msimCONC_MOLES_LITER,/* conc_units;                         */
                  msimPRESS_ATM,       /* pressure_units                      */
                  msimVOL_LITERS,      /* volume_units                        */
                  {
                       msimINIT_TEMP_VAL,/* const_temp;                       */
                       msimINIT_TOLERANCE,/* convergence_std;                 */
                       msimREAL_ONE_STRING,/* progr_coeffA;                   */
                       msimINIT_TEMP_VAL,/* prog_initial_temp                 */
                       msimINIT_TEMP_VAL,/* initial_temp;                     */
                       msimREAL_ZERO_STRING,/* prog_coeffD                    */
                       msimINIT_TEMP_STEP_SIZE,/* max_step_size;              */
                       msimMAX_TEMP_VAL
                  }
                  , msimNO_SESSION,    /* session_id;                         */
                  {
                       msimINIT_CYCLE_COUNT,/* cycle_length                   */
                       msimINIT_MIN_EFF/* min_eff;                            */
                  }
                  , msimNULL_STR,      /* temp_profile_data                   */
                  msimREAL_ZERO_STRING,/* elapsed_time_limit */
                  msimNULL_STR,        /* reserved15bytes    */
                  msimNULL_STR,        /* execution_time_str */
                  msimINIT_FILENAME,   /* base_filename                       */
                  msimNULL_STR         /* reserved_for_future[]               */
             };

msimSPECIES msimC_SPECIES =
            {
                 msimNULL_STR,         /* name;                               */
                 0,                    /* index;                              */
                 FALSE,                /* nonzero;                            */
                 {                     /* thermcoeff[];                       */
                      msimREAL_ZERO_STRING, msimREAL_ZERO_STRING, msimREAL_ONE_STRING,
                      msimREAL_ZERO_STRING, msimREAL_ZERO_STRING
                 }
                 , msimREAL_ZERO_STRING,/* initialconc;                       */
                 msimDEFAULT_DENSITY,  /* 3;                                  */
                 msimSOLID,            /* phys_state;                         */
                 NULL,                 /* next,                               */
                 NULL                  /* prev;                               */
            };

// declarations of local classes

class SetUnitsDialog : public ModalDialog
{
private :
     FixedText UnitsFilenameLabel;
     OKButton CloseBtn;
     PushButton DefaultsBtn;
     HelpButton HelpBtn;
     ListBox ConcUnitsLB;
     FixedText aFixedText3;
     FixedText aFixedText4;
     ListBox TimeUnitsLB;
     FixedText aFixedText5;
     ListBox PressUnitsLB;
     FixedText aFixedText6;
     ListBox EnergyUnitsLB;
     GroupBox aGroupBox1;
     GroupBox aGroupBox2;

     msimPINSTANCE instance;

public :
     SetUnitsDialog( Window PTR pParent, msimPINSTANCE Instance );
     void DefaultsHandler( PushButton PTR pButton );
     void InitializeUnitsPanel( msimPINSTANCE Instance );
     void CloseHandler( PushButton PTR pButton );

};

/* declarations of local functions                                          */

static msimBOOL OKToSaveNotebookText( PCHAR Filename, msimWID Owner );
static msimBOOL OKToSaveRxnScheme( PCHAR Filename, msimWID Owner );

static msimFILE_STRING ConfigFilename = msimINITIALIZATION_FILE;

/*--------------------------------------------------------------------------*/
/*                        msimMemoryError( )                                */
/*..........................................................................*/
/*                                                                          */
/* Notify user of memory error. Provides info regarding location of the     */
/* bad operation                                                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimMemoryError( USHORT ErrType, PCHAR Filename, PCHAR Timestamp, int LineNum
          , msimWID Owner )
{
     String message;
     String msg1;

     switch ( ErrType )
     {
     case msimMEM_ALLOC_ERROR :

          message = "Memory allocation error";

          break;

     case msimMEM_FREE_ERROR :

          message = "Memory release error";

          break;

     default :

          message = "Unknown memory error";

          break;

     }                                 /* endswitch                           */
     msg1 = " in file \"" + String( Filename ) + "\"\n" + "Line number (" + String( LineNum
     ) + ")\n" + "Time stamp (" + String( Timestamp ) + ")\n" + "sys err msg : " + strerror
     ( errno ) + ".\nAborting current action";

     message += msg1;

     ErrorBox( Owner, WB_OK | WB_DEF_OK, message ) .Execute( );

     return;
}

/*--------------------------------------------------------------------------*/
/*                        msimCreateRxnInstance()                           */
/*..........................................................................*/
/*                                                                          */
/* Function for allocating memory for a reaction instance. If memory is     */
/* successfully allocated then the structure is initialized                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPINSTANCE msimCreateRxnInstance( msimWID Owner )
{
     msimPINSTANCE ptr;

     ptr = new msimINSTANCE;

     if ( ! ptr )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, Owner );
          return NULL;
     }

     memset( ptr, '\0', sizeof ( msimINSTANCE ) );
     return ptr;
}

void msimInitRxnInstance( msimPINSTANCE tmp, msimWID Owner, PCHAR Filename )

{

     /* structure initialization                                         */

     *tmp = msimC_INSTANCE;
     msimStringCopy( tmp->filename, Filename, sizeof tmp->filename );
     msimStringCopy( tmp->base_filename, msimBaseFilename( Filename ),
          sizeof tmp->base_filename );

/* set initial values for units options based on those in the last initialization*/

     tmp->time_units = msimAppOptions.time_units;
     tmp->energy_units = msimAppOptions.energy_units;
     tmp->conc_units = msimAppOptions.conc_units;
     tmp->pressure_units = msimAppOptions.pressure_units;
     tmp->volume_units = msimAppOptions.volume_units;
     // create and execute Set Units dialog

     SetUnitsDialog dialog( Owner, tmp );

     dialog.Execute( );

     tmp->data_altered_since_lastsave = TRUE;
     return;
}

SetUnitsDialog::SetUnitsDialog( Window PTR pParent, msimPINSTANCE Instance ) :
ModalDialog ( pParent, ResId ( msimUNITS_PANEL ) ),
UnitsFilenameLabel ( this, ResId ( msimUNITS_FILENAME_STR ) ),
CloseBtn ( this, ResId ( msimUNITS_CLOSE ) ),
DefaultsBtn ( this, ResId ( msimUNITS_DEFAULTS ) ),
HelpBtn ( this, ResId ( msimUNITS_HELP ) ),
ConcUnitsLB ( this, ResId ( msimUNITS_CONC_LISTBOX ) ),
aFixedText3 ( this, ResId ( 3 ) ),
aFixedText4 ( this, ResId ( 4 ) ),
TimeUnitsLB ( this, ResId ( msimUNITS_TIME_LISTBOX ) ),
aFixedText5 ( this, ResId ( 5 ) ),
PressUnitsLB ( this, ResId ( msimUNITS_PRESS_LISTBOX ) ),
aFixedText6 ( this, ResId ( 6 ) ),
EnergyUnitsLB ( this, ResId ( msimUNITS_ENERGY_LISTBOX ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aGroupBox2 ( this, ResId ( 2 ) )
{
     FreeResource( );

     instance = Instance;

     CloseBtn.ChangeClickHdl( LINK( this, SetUnitsDialog,
               CloseHandler ) );

     DefaultsBtn.ChangeClickHdl( LINK( this, SetUnitsDialog,
               DefaultsHandler ) );

     UnitsFilenameLabel.SetText( String( instance->filename ) );

     InitializeUnitsPanel( instance );

     msimCascadeWindowOnOwner( this, pParent );

}

/*--------------------------------------------------------------------------*/
/*                SetUnitsDialog::DefaultsHandler( )                        */
/*..........................................................................*/
/*                                                                          */
/* This function resets the Units panel to the application-defined          */
/* default settings                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void SetUnitsDialog::DefaultsHandler( PushButton PTR )
{
     InitializeUnitsPanel( &msimC_INSTANCE );
     return;
}

void SetUnitsDialog::InitializeUnitsPanel( msimPINSTANCE Instance )
{
     USHORT position;
     //  set the listbox initial selections

     switch ( Instance->conc_units )
     {

     default :
     case msimCONC_MOLES_LITER :
          position = 0;
          break;

     case msimCONC_MOLES_CM3 :
          position = 1;
          break;

     case msimCONC_MOLES_CM2 :
          position = 2;
          break;

     case msimCONC_MOL_LITER :
          position = 3;
          break;

     case msimCONC_MOL_CM3 :
          position = 4;
          break;

     case msimCONC_MOL_CM2 :
          position = 5;
          break;
     }

     ConcUnitsLB.SelectEntryPos( position, TRUE );

     switch ( Instance->time_units )
     {

     default :
     case msimTIME_SECONDS :
          position = 0;
          break;

     case msimTIME_MINUTES :
          position = 1;
          break;

     case msimTIME_HOURS :
          position = 2;
          break;

     }
     TimeUnitsLB.SelectEntryPos( position, TRUE );

     if ( Instance->energy_units == msimENERGY_JOULES )
          position = 1;
     else
          position = 0;

     EnergyUnitsLB.SelectEntryPos( position, TRUE );

     switch ( Instance->pressure_units )
     {

     default :
     case msimPRESS_ATM :
          position = 0;
          break;

     case msimPRESS_TORR :
          position = 1;
          break;

     case msimPRESS_PASCALS :
          position = 2;
          break;

     }
     PressUnitsLB.SelectEntryPos( position, TRUE );

     return;
}

/*--------------------------------------------------------------------------*/
/*                SetUnitsDialog::CloseHandler( )                           */
/*..........................................................................*/
/*                                                                          */
/* This function reads the four dropdown listbox selections                 */
/* and set the appropriate values in the instance                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void SetUnitsDialog::CloseHandler( PushButton PTR )
{

     switch ( ConcUnitsLB.GetSelectEntryPos( ) )
     {

     default :
     case 0 :
          instance->conc_units = msimCONC_MOLES_LITER;
          break;

     case 1 :
          instance->conc_units = msimCONC_MOLES_CM3;
          break;

     case 2 :
          instance->conc_units = msimCONC_MOLES_CM2;
          break;

     case 3 :
          instance->conc_units = msimCONC_MOL_LITER;
          break;

     case 4 :
          instance->conc_units = msimCONC_MOL_CM3;
          break;

     case 5 :
          instance->conc_units = msimCONC_MOL_CM2;
          break;
     }

     switch ( TimeUnitsLB.GetSelectEntryPos( ) )
     {

     default :
     case 0 :
          instance->time_units = msimTIME_SECONDS;
          break;

     case 1 :
          instance->time_units = msimTIME_MINUTES;
          break;

     case 2 :
          instance->time_units = msimTIME_HOURS;
          break;

     }

     switch ( PressUnitsLB.GetSelectEntryPos( ) )
     {

     default :
     case 0 :
          instance->pressure_units = msimPRESS_ATM;
          break;

     case 1 :
          instance->pressure_units = msimPRESS_TORR;
          break;

     case 2 :
          instance->pressure_units = msimPRESS_PASCALS;
          break;

     }

     if ( EnergyUnitsLB.GetSelectEntryPos( ) == 1 )
          instance->energy_units = msimENERGY_JOULES;
     else
          instance->energy_units = msimENERGY_CALS;

     // save the current options for next time we call the dialog

     msimAppOptions.time_units = instance->time_units;
     msimAppOptions.energy_units = instance->energy_units;
     msimAppOptions.conc_units = instance->conc_units;
     msimAppOptions.pressure_units = instance->pressure_units;
     msimAppOptions.volume_units = instance->volume_units;

     /* then get the choice for volume */

     if ( instance->conc_units == msimCONC_MOLES_LITER ||
               instance->conc_units == msimCONC_MOL_LITER
          )

          instance->volume_units = msimVOL_LITERS;

     else if ( instance->conc_units == msimCONC_MOLES_CM3 ||
               instance->conc_units == msimCONC_MOL_CM3
          )

          instance->volume_units = msimVOL_CC;

     else
          instance->volume_units = msimVOL_CM;

     EndDialog( );
}

/*--------------------------------------------------------------------------*/
/*                        msimAddReaction()                                 */
/*..........................................................................*/
/*                                                                          */
/* Function for adding another reaction step to a growing reaction scheme   */
/* memory is allocated and the various fields on the reaction step struc-   */
/* ture are initialized. A NULL ptr is returned if the memory allocation    */
/* fails.                                                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPRXN msimAddReaction( msimPRXN current, msimPINSTANCE Instance )
{
     msimPRXN newptr;
     USHORT i;

     newptr = new msimRXN;

     if ( ! newptr )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__, __TIMESTAMP__
               , __LINE__, msimWID_NONE );
          return NULL;
     }

     /* first initialize new structure                                      */

     strcpy( newptr->equation, msimNULL_EQN );

     newptr->numreactants = newptr->numproducts = 0;
     newptr->next = newptr->prev = ( msimPRXN ) NULL;
     newptr->reversible = newptr->not_stoich = FALSE;
     newptr->singlerate = TRUE;

     strcpy( newptr->fwdA, msimNULL_STR );
     strcpy( newptr->fwdM, msimNULL_STR );
     strcpy( newptr->fwdEa, msimNULL_STR );
     strcpy( newptr->revA, msimNULL_STR );
     strcpy( newptr->revM, msimNULL_STR );
     strcpy( newptr->revEa, msimNULL_STR );

     for ( i = 0; i < msimMAX_NUMBER_OF_COMPONENTS; i++ )
     {
          strcpy( newptr->reactantlist[i].stoichcoeff, msimNULL_STR );
          strcpy( newptr->productlist[i].stoichcoeff, msimNULL_STR );
          newptr->reactantlist[i].speciesindex = 0;
          newptr->productlist[i].speciesindex = 0;
          strcpy( newptr->fwdexponent[i], msimNULL_STR );
          strcpy( newptr->revexponent[i], msimNULL_STR );
     }                                 /* endfor                              */

     /* now the confusing part - dealing with pointers in the linked list   */

     if ( current == NULL )            /* is this is the start of a list      */
     {
          newptr->next = NULL;
          newptr->prev = NULL;
          Instance->ptr_to_rxnlist = newptr;
     }
     else
     {

          /* insert the new reaction struc in between the current and its follower*/

          newptr->next = current->next;
          newptr->prev = current;

          /* make the preceding reaction structure point to the one we just made*/

          current->next = newptr;

          /* finally if there is an existing RXN struct we have inserted    */
          /* in front of, be sure it points back to the new one             */

          if ( newptr->next != NULL )
               newptr->next->prev = newptr;

     }                                 /* endif                               */
     Instance->numsteps++;
     return newptr;
}

/*--------------------------------------------------------------------------*/
/*                        msimDeleteReaction()                              */
/*..........................................................................*/
/*                                                                          */
/* Function for removing a  reaction step from a reaction scheme            */
/* Pointers of other mebers of the linked list are adjusted and memory is   */
/* delaoocated. The standard return value is a pointer to the rxn step      */
/* immedialtey follwing the deleted one. If the deleted step was at the end */
/* of the list then a ptr to the preceeding rxn step is returned. If the    */
/* deleted rxn step  was the only rxn step in the scheme then a NULL ptr is */
/* returned                                                                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimPRXN msimDeleteReaction( msimPRXN rxn, msimPINSTANCE Instance )
{
     msimPRXN ret;

     if ( rxn == NULL )
          return NULL;                 /* get out immed if NULL ptr passed    */

     /* if we are not at the end of the list then make the following        */
     /* member point at the one preceding the one we are about to delete    */

     if ( rxn->next != NULL )
     {
          ret = rxn->next;
          rxn->next->prev = rxn->prev;
     }
     else
          ret = rxn->prev;

     /* if we are not at the beginning of the list then make the            */
     /* preceding member point to the one after the one to be deleted       */

     if ( rxn->prev != NULL )
          rxn->prev->next = rxn->next;
     else
          Instance->ptr_to_rxnlist = rxn->next;

     delete rxn;

     Instance->numsteps--;

     return ret;
}

void msimClearSpeciesList( msimPSPECIES ListPtr )
{
     msimPSPECIES next_ptr;

     while ( ListPtr != NULL )
     {

          /* delete all the substructures                                   */

          next_ptr = ListPtr->next;
          delete ListPtr;

          ListPtr = next_ptr;
     }
     return;
}

msimBOOL msimClearMemory( msimPINSTANCE Instance, msimWID Owner, msimBOOL
              CheckFirst )

{

     /* if Instance has not yet been allocated then quit                    */

     if ( Instance == NULL )
          return FALSE;

     /* if notebook window is opened for this instance, read it             */
     /* before deciding whether buffer is NULL                              */

     if ( Instance->p_notebook_window != msimWID_INVALID )
          msimReadNotebookMLE( Instance );

#if 0
     /* if memory is clear to start with just return                        */

     if ( ( Instance->ptr_to_rxnlist == NULL ) && ( Instance->ptr_to_species_list
               == NULL ) && ( Instance->ptr_to_notebook_text == NULL ) )
          return TRUE;

#endif

     /* otherwise give the user a chance to change their mind               */
     /* and to save any modified data to file                               */

     if ( CheckFirst )
     {
#if 0
          String message = "The reaction file named \"";

          message += Instance->base_filename;
          message += "\" is currently loaded. This "
          "operation will remove it from memory. Is this OK?";

          QueryBox box( Owner, WB_YES_NO | WB_DEF_NO, message );

          msimCenterDialogWindowOnOwner( &box, Owner );

          if ( RET_NO == box.Execute( ) )
               return FALSE;
#endif

          msimSimQueue.RemoveFromQueue( Instance );

          if ( Instance->data_altered_since_lastsave )
          {
               if ( OKToSaveRxnScheme( Instance->filename, Owner ) )
               {
                    if ( msimNO_ERROR != msimSaveRxnScheme( Instance, Owner, msimCHECK_FOR_OVERWRITE ) )
                         return FALSE;
               }
          }

          if ( Instance->notebook_altered )
          {
               if ( OKToSaveNotebookText( Instance->filename, Owner ) )
                    if ( msimNO_ERROR != msimSaveNotebookText( Instance, Owner ) )
                         return FALSE;
          }

     }                                 /* end if (CheckFirst)                 */

     /* if notebook window is open and it belongs to existing               */
     /* rxn instance then close it                                          */

     if ( Instance->p_notebook_window )
          msimCloseNotebookWindow( Instance );
     if ( Instance->p_plot_dialog )
          msimClosePlotDialogWindow( Instance );

     /* check if notebook window is open and belongs to the                 */
     /* first delete all the reaction steps                                 */

     if ( Instance->ptr_to_rxnlist != NULL )
     {
          msimPRXN rxnptr, nextrxnptr;

          rxnptr = Instance->ptr_to_rxnlist;
          while ( rxnptr != NULL )
          {

               /* delete all the substructures                              */

               nextrxnptr = rxnptr->next;
               delete rxnptr;          /* then delete the rxn                 */

               rxnptr = nextrxnptr;
          }                            /* endwhile                            */
     }

     /* next delete the list of species                                     */

     msimClearSpeciesList( Instance->ptr_to_species_list );

     /* then delete the notebook text buffer                                */

     if ( Instance->ptr_to_notebook_text != NULL )
          delete[]Instance->ptr_to_notebook_text;

     /* initialize to default values                                        */

     *Instance = msimC_INSTANCE;

     return TRUE;

}                                      /* end function                        */

/*--------------------------------------------------------------------------*/
/*                        msimInitializeApp()                               */
/*..........................................................................*/
/*                                                                          */
/* Function  loads in an initial application configuration file if a        */
/* valid config file is found.                                              */
/* function also loads in rxn file if specified on command line, and it     */
/* exists and it is a valid msim rxn file.                                  */
/* Returns msimNO_ERROR if successful.                                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimInitializeApp( int Argc, PCHAR Argv[], msimPINSTANCE Instance[],
            msimWID Owner )
{
     FILE *init_file;
     int i, j;
     msimFILE_STRING current_file;

     /* first take care of initialization of application options            */

     // construct cnf file name - looks in curent working directory only

     msimConstructFullyQualifiedFilename( ConfigFilename, sizeof ConfigFilename );

     if ( msimFileExists( ConfigFilename ) )
     {
          if ( NULL == ( init_file = fopen( ConfigFilename, "rb" ) ) )
          {
               msimFileError( ConfigFilename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
               msimAppOptions = msimC_APP_OPTIONS;
          }
          else
          {

               /* if file was opened without error                          */

               fread( &msimAppOptions, sizeof ( msimAPP_OPTIONS ), 1, init_file );

               if ( ferror( init_file ) )
               {
                    msimFileError( ConfigFilename, Owner, ( USHORT ) msimREAD_ERROR );
                    msimAppOptions = msimC_APP_OPTIONS;
               }
               else
               {

                    /* if no error reading the file                         */

                    if ( strcmp( msimAppOptions.msimID, msimCFG_FILE_FLG ) )
                    {
                         msimFileError( ConfigFilename, Owner, ( USHORT )
                              msimNOT_MSIM_FILE );
                         msimAppOptions = msimC_APP_OPTIONS;
                    }
               }
               fclose( init_file );
          }
     }                                 /* endif                               */
     else

          /* if no init file found then set msimAppOptions to defaults      */

          msimAppOptions = msimC_APP_OPTIONS;

     /* set main window size info if the current info is invalid            */
     /* eg from using the defaultAppOptions settings                        */

     if ( msimINVALID_WINDOW_GEOMETRY( msimAppOptions.main_window_position ) )
     {
          msimAppOptions.main_window_position.x = ( MAINWIN_LEFT * System::
               GetScreenSizePixel( ) .Width( ) ) / 100;
          msimAppOptions.main_window_position.y = ( MAINWIN_TOP * System::
               GetScreenSizePixel( ) .Height( ) ) / 100;
          msimAppOptions.main_window_position.width = ( MAINWIN_WIDTH * System::
               GetScreenSizePixel( ) .Width( ) ) / 10;
          msimAppOptions.main_window_position.height = ( MAINWIN_HEIGHT * System
               :: GetScreenSizePixel( ) .Height( ) ) / 10;
     }

     /* now load the first rxn scheme if a valid reaction file name was     */
     /* specified on the command line                                       */
     /* first initialize the Instance arry of ptrs to NULL                  */

     for ( i = 0; i < msimMAX_NO_INSTANCES; i++ )
          Instance[i] = NULL;

     /*  if command line args were specified then try to load them          */

     for ( i = 1, j = 0; ( i < Argc ) && ( j < msimMAX_NO_INSTANCES ); i++ )
     {
          msimStringCopy( current_file, Argv[i], sizeof current_file );
          msimConstructFullyQualifiedFilename( current_file, sizeof current_file );
          if ( msimFileExists( current_file ) )
          {

               if ( NULL == ( Instance[j] = msimCreateRxnInstance( Owner ) ) )
                    return msimMEM_ALLOC_ERROR;

               if ( msimNO_ERROR != msimLoadRxnScheme( Instance[j], current_file,
                              Owner ) )
               {
                    String message = "Error loading file \"";

                    message += msimBaseFilename( current_file );
                    message += "\"\nExit application?";

                    ErrorBox box( Owner, WB_YES_NO | WB_DEF_NO, message
                    );

                    if ( RET_YES == box.Execute( ) )
                         return msimUSER_ABORT;
                    else
                    {
                         msimClearMemory( Instance[j], Owner, FALSE );
                         delete Instance[j];

                         Instance[j] = NULL;
                         return msimNO_ERROR;
                    }
               }
               else
               {
                    aMainApp.UpdateRxnSchemeListMenu( j, Instance[j]->filename );
                    j++;               /* move to next Instance               */
               }
          }
          else
          {

               /* filename specified on command line was not found          */
               /* just notify - do not abort app                            */

               String message = "File \"";

               message += msimBaseFilename( current_file );
               message += "\" was not found.";

               WarningBox box( Owner, WB_OK | WB_DEF_OK, message );

               box.Execute( );
          }
     }

#if defined(__OS2__)
     if ( msimNO_ERROR != msimCreateQueue( ) )
     {
          ErrorBox( Owner, WB_OK | WB_DEF_OK,
               String( "Unable to create message queue. Exiting" )
          ) .Execute( );
          return msimQUEUE_ERROR;
     }


#endif


     if ( ! msimBuildSimulatorName( ) )
     {
          ErrorBox( Owner, ResId( msimLOST_SIM_ENGINE ) )
          .Execute( );
          return msimFILENAME_ERROR;
     }


     return msimNO_ERROR;
}

/*--------------------------------------------------------------------------*/
/*                          msimCloseApp()                                  */
/*..........................................................................*/
/*                                                                          */
/* Function  saves the current application configuration  to file if        */
/* possible.                                                                */
/* function also makes sure user has saved all the rxn schemes in memory    */
/* to file if desired and makes sure all memory is released                 */
/* Returns msimNO_ERROR if no errors occur                                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimRC msimCloseApp( msimPINSTANCE Instance[], msimWID Owner )
{
     FILE *init_file;
     USHORT i;

     /* check for unsaved/modified rxn schemes                           */
     /* free dynamically allocated memory while we are at it                */

     for ( i = 0; i < msimMAX_NO_INSTANCES; i++ )
     {
          if ( ! Instance[i] )
               continue;               /* skip NULL instances                 */

          /* destroy any windows that may be open                                */

          if ( Instance[i]->p_notebook_window )
               msimCloseNotebookWindow( Instance[i] );

          if ( Instance[i]->p_plot_dialog )
               msimClosePlotDialogWindow( Instance[i] );


          if ( Instance[i]->data_altered_since_lastsave )
          {
               if ( OKToSaveRxnScheme( Instance[i]->filename, Owner ) )
                    msimSaveRxnSchemeAs( Instance[i], Owner, msimNO_SHOW_WARNING );
          }

          if ( Instance[i]->notebook_altered )
          {
               if ( OKToSaveNotebookText( Instance[i]->filename, Owner ) )
                    msimSaveNotebookText( Instance[i], Owner );
          }


          msimClearMemory( Instance[i], Owner, FALSE );
          delete Instance[i];

          Instance[i] = NULL;

     }

     /* take care of saving the current application options                 */

     if ( NULL == ( init_file = fopen( ConfigFilename, "wb" ) ) )
          msimFileError( ConfigFilename, Owner, ( USHORT ) msimFILE_OPEN_ERROR );
     else
     {

          /* if file opened ok                                              */
          /* first reset window data if necessary                           */

          if ( ! msimAppOptions.save_window_positions )
          {

               /* set all window attach structs to default vals             */

               msimAppOptions.plot_window_position =
               msimC_APP_OPTIONS.plot_window_position;

               msimAppOptions.note_dialog_position =
               msimC_APP_OPTIONS.note_dialog_position;

               msimAppOptions.help_window_position =
               msimC_APP_OPTIONS.help_window_position;

               msimAppOptions.plot_dialog_position =
               msimC_APP_OPTIONS.plot_dialog_position;

               msimAppOptions.main_window_position =
               msimC_APP_OPTIONS.main_window_position;
          }

          fwrite( &msimAppOptions, sizeof ( msimAPP_OPTIONS ), 1, init_file );

          if ( ferror( init_file ) )
               msimFileError( ConfigFilename, Owner, ( USHORT ) msimWRITE_ERROR );

          if ( EOF == fclose( init_file ) )
               msimFileError( ConfigFilename, Owner, ( USHORT ) msimFILE_CLOSE_ERROR );

     }

#if defined(__OS2__)
     if ( msimNO_ERROR != msimCloseQueue( ) )
          return msimQUEUE_ERROR;
#endif


     return msimNO_ERROR;
}

msimBOOL OKToSaveNotebookText( PCHAR Filename, msimWID Owner )
{
     String message( ResId( msimSAVE_NB_QUERY1 ) );

     message += msimBaseFilename( Filename );

     message += String( ResId( msimSAVE_NB_QUERY2 ) );

     QueryBox box( Owner, WB_YES_NO | WB_DEF_YES, message );

     return ( RET_YES == box.Execute( ) );
}

msimBOOL OKToSaveRxnScheme( PCHAR Filename, msimWID Owner )
{
     String message( ResId( msimSAVE_RXN_QUERY1 ) );

     message += msimBaseFilename( Filename );

     message += String( ResId( msimSAVE_RXN_QUERY2 ) );

     QueryBox box( Owner, WB_YES_NO | WB_DEF_YES, message );

     return ( RET_YES == box.Execute( ) );
}
