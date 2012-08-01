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
   |  msim2.hxx      :   main header file for msim                          |
   |                                                                        |
   |                     IBM INTERNAL USE ONLY                              |
   |      Copyright International Business Machines Corp., 1993             |
   |                                                                        |
   |  Version number :  1.0                                                 |
   |                                                                        |
   |  description    : contains global declarations for msim                |
   |                                                                        |
   |  authors        :   Bill Hinsberg and Frances Houle, IBM Almaden       |
   |                                                                        |
   |  created        :   Aug 11,1993                                        |
   |                                                                        |
   | This is the second main header file for MSIM. It contains struct,      |
   | constants, #defines and function prototypes                            |
   |                                                                        |
   []----------------------------------------------------------------------[]*/


#ifndef  MSIM2_HXX
#define  MSIM2_HXX
#include <sv.hxx>

#define  msimDEFAULT_TYPEFACE                    FAMILY_SWISS

// declare a couple class names used below

class MainPlotDialog;

#include "msim1.hxx"

struct APP_OPTION_STRUCT
{
     CHAR msimID[msimCFG_FILE_FLG_LENGTH];
     USHORT time_units, energy_units;
     USHORT conc_units, pressure_units, volume_units;
     msimFILE_STRING data_template;
     msimFILE_STRING text_output_template;
     msimFILE_STRING graph_output_template;
     msimFILE_STRING external_file_template;
     msimFILE_STRING external_t_profile_template;
     msimBOOL save_window_positions;
     msimBOOL confirm_on_overwrite;
     USHORT fontsize;                  /* a bunch of stuff for plot attri     */
     enum FontFamily fonttype;
     msimBOOL vary_colors;
     msimBOOL vary_linestyle;
     msimBOOL vary_markerstyle;
     msimBOOL show_lines;
     msimBOOL show_markers;
     msimBOOL show_filename;
     msimBOOL encapsulated_ps;
     msimBOOL landscape;
     msimBOOL plotfile_conv_color;
     USHORT show_grid;                 /* boolean  flag                       */
     enum msimPLOT_FILE_TYPE default_plotfile;
     enum msimCOLORSET_TYPE default_colorset;
     msimWIN_GEOMETRY plot_window_position;
     msimWIN_GEOMETRY note_dialog_position;
     msimWIN_GEOMETRY help_window_position;
     msimWIN_GEOMETRY plot_dialog_position;
     msimWIN_GEOMETRY main_window_position;
};

typedef struct APP_OPTION_STRUCT msimAPP_OPTIONS;
typedef struct APP_OPTION_STRUCT PTR msimPAPP_OPTIONS;

#if !defined(__SIMEQUIL_BUILD__)


// global class declarations ...
// used in msimmain.cxx

class msimPushButton : public PushButton
{
protected :
     virtual void MouseMove( const MouseEvent & );

public :
     msimPushButton( Window PTR pParent, const ResId& rResId );

};

class msimAddPushButton : public msimPushButton
{
public :
     msimAddPushButton( Window PTR pParent, const ResId& rResId );
};

class msimEditPushButton : public msimPushButton
{
public :
     msimEditPushButton( Window PTR pParent, const ResId& rResId );
};

class msimDeletePushButton : public msimPushButton
{

public :
     msimDeletePushButton( Window PTR pParent, const ResId& rResId );
};


class msimRxnListBox : public ListBox
{
protected :
     virtual void KeyInput( const KeyEvent& rKeyEvt );

public :
     virtual void MouseMove( const MouseEvent& rMEvt );

     // constructor
     msimRxnListBox( Window PTR pParent, const ResId& rResId );

};



class LogoWindow : public WorkWindow
{

protected :

     FixedBitmap aFixedBitmap1;
     Timer aTimer;

public :

     LogoWindow( );
     void StartTimer( );
     void TimeOutHandler( Timer PTR );
};



// --- class MainWindow ----------------------------------------------

class MainWindow : public WorkWindow
{
protected :
     FixedText aActiveFileName;
     FixedText aActiveFileStatus;
     FixedText aNumReactionsStatus;
     FixedText aNumSpeciesStatus;
     GroupBox aFileStatsGB;

     msimAddPushButton aAddReactionPB;
     msimEditPushButton aEditReactionPB;
     msimDeletePushButton aDeleteReactionPB;

     StatusBar aHelpStatus;

     msimRxnListBox aRxnStepLB;
     msimPRXN rxn_cut_buffer;

     virtual void KeyInput( const KeyEvent& rKEvent );

     void CutSelectedRxnStep( );
     void CopySelectedRxnStep( );
     void PasteRxnStep( );
     void ProcessSelectedRxnStep( );
     void TransferSelectedRxnData( msimPRXN, msimPRXN );

public :

     MainWindow( );
     ~MainWindow( );

     virtual BOOL Close( );

     virtual void Resize( );
     virtual void MouseMove( const MouseEvent& rMEvt );

     long HighlightMenuHdl( Menu PTR pMenu );
     long SelectMenuHdl( Menu PTR pMenu );
     void AddRxnStepHandler( PushButton PTR );
     void EditRxnStepHandler( PushButton PTR );
     void DeleteRxnStepHandler( PushButton PTR );
     void ListboxSelectHandler( ListBox PTR pListBox );
     void ListboxDoubleClickHandler( ListBox PTR );

     StatusBar& GetStatus( )
     {
          return aHelpStatus;
     }
     void UpdateMainWinData( msimBOOL RefreshRxnBuffer );

     msimPRXN DeleteReactionFromListBox( msimPRXN RxnPtr, USHORT
          Position,
          msimPINSTANCE Instance
     );

     msimPRXN AddReactionToListBox( msimPRXN RxnPtr,
          msimPINSTANCE Instance );
     ListBox& GetListBox( )
     {
          return aRxnStepLB;
     };

     void HandOffKeyInput( const KeyEvent& rKEvent )
     {
          KeyInput( rKEvent );
     };

     msimBOOL OpenRxnFile( PCHAR FileName );

};


class MainApp : public Application
{
protected :
     virtual BOOL QueryExit( );
     virtual void AppEvent( const ApplicationEvent& rAppEvent );
     virtual void UserEvent( ULONG nEvent, void PTR pEventData );
     msimPINSTANCE ActiveInstance;
     USHORT ActiveInstanceIndex;
     msimPINSTANCE Instance[msimMAX_NO_INSTANCES];
     MainWindow PTR pAppWindow;
     LogoWindow PTR pLogoWindow;

     LogoWindow PTR ShowLogo( );


public :
     MainApp( );
     msimBOOL OKToStartSimulation;
     msimFILE_STRING ResourceFileName;

     USHORT GetNextEmptyRxnInstance( );
     void ActivateRxnInstance( USHORT NewIndex );
     void UpdateRxnSchemeListMenu( USHORT Index, PCHAR FileName );

     msimPINSTANCE GetActiveInstance( )
     {
          return ActiveInstance;
     };
     void SetActiveInstance( msimPINSTANCE pI )
     {
          Instance[ActiveInstanceIndex] = ActiveInstance = pI;
     };
     USHORT GetActiveInstanceIndex( )
     {
          return ActiveInstanceIndex;
     };
     void SetActiveInstanceIndex( USHORT i )
     {
          ActiveInstanceIndex = i;
     };
     msimPINSTANCE PTR GetInstanceArray( )
     {
          return Instance;
     };


     MainWindow PTR GetAppWindow( )
     {
          return pAppWindow;
     };


     LogoWindow PTR GetLogoWindow( )
     {
          return pLogoWindow;
     };

     void SetLogoWindow( LogoWindow PTR pWin )
     {
          pLogoWindow = pWin;
     };

     virtual void Main( int argc, char *argv[] );
};




/* global variable definitions                                              */
/* defined in msiminit.c                                                    */

extern msimINSTANCE msimC_INSTANCE;
extern msimSPECIES msimC_SPECIES;
extern msimAPP_OPTIONS msimC_APP_OPTIONS;
extern msimAPP_OPTIONS msimAppOptions;

/* defined in msimmain.cxx                                                  */

extern MainApp aMainApp;

// end if   #if !defined(__SIMEQUIL_BUILD__)
#endif


// ----- global function declarations -----

// defined in msimappo.cxx

void msimAppOptionDialog( msimWID Owner );


// defined in msimrxde.cxx

void msimRxnDataEntryDialog( msimWID Owner, msimPINSTANCE Instance );
msimPRXN msimGetRxnPtrFromLineNumber( USHORT Selection, msimPRXN RxnPtr );


// defined in msimopts.cxx

USHORT msimValidTPVCombo( msimBOOL VariablePress, USHORT TempOption, USHORT
            VolOption );
VOID msimSimulationOptionDialog( msimWID Owner, msimPINSTANCE Instance );


// defined in msimrxnc.cxx

VOID msimSetRxnConditions( msimWID Owner, msimPINSTANCE pInstance );


// defined in msimspec.cxx

VOID msimSpeciesDataDialog( msimWID Owner, msimPINSTANCE pInstance );


// defined in msimrxde.cxx

void msimDisplayAnalysisError( msimRC rc, msimWID Owner );


// defined in msimvspd.cxx

msimRC msimVerifySpeciesDataDlg( msimWID Owner, msimPINSTANCE pInstance );

// defined in msimedrl.cxx

void msimEditRateLawDialog( msimWID Owner, msimPINSTANCE pInstance,
          msimPRXN Rxnptr, PCHAR Equation );


// defined in msimnb.cxx

VOID msimNotebookDialog( msimWID Owner, msimPINSTANCE Instance );
VOID msimReadNotebookMLE( msimPINSTANCE Instance );


// defined in msimanal.cxx

msimRC msimAnalyzeEqn( msimEQUATION_STRING Eqn, msimNAME_ARRAY Reactants,
            msimEXPONENT_ARRAY R_Expnt, USHORT PTR numreactants,
            msimNAME_ARRAY Products, msimEXPONENT_ARRAY P_Expnt,
            USHORT *numproducts );

PCHAR msimAnalysisError( msimRC rc );

msimRC msimAnalyzeRxnScheme( msimPINSTANCE Instance, msimPRXN PTR BadRxnData, USHORT
            PTR LineNumber, msimWID Owner );

USHORT msimFindSpeciesIndex( PCHAR SearchString, msimPINSTANCE Instance );


// defined in msiminit.cxx

VOID msimInitRxnInstance( msimPINSTANCE tmp, msimWID Owner, PCHAR Filename );
msimPRXN msimAddReaction( msimPRXN rxn, msimPINSTANCE Instance );
msimPRXN msimDeleteReaction( msimPRXN rxn, msimPINSTANCE Instance );
msimBOOL msimClearMemory( msimPINSTANCE Instance, msimWID Owner, msimBOOL
              CheckFirst );

msimRC msimInitializeApp( int Argc, PCHAR Argv[], msimPINSTANCE Instance[],
            msimWID Owner );

msimRC msimCloseApp( msimPINSTANCE Instance[], msimWID Owner );
VOID msimClearSpeciesList( msimPSPECIES ListPtr );
msimPINSTANCE msimCreateRxnInstance( msimWID Owner );
VOID msimMemoryError( USHORT ErrType, PCHAR Filename, PCHAR Timestamp, int LineNum
          , msimWID Owner );

msimRC msimSaveNotebookText( msimPINSTANCE Instance, msimWID Owner );


// defined in msim_sim.cxx

VOID msimStartSimulation( msimWID Owner, msimPINSTANCE Instance, msimBOOL Resume );
msimBOOL msimCheckForActiveSimulation( msimPINSTANCE Instance[], msimWID Owner );
void msimAbortSimulation( msimWID Owner, msimPINSTANCE Instance );
msimBOOL msimBuildSimulatorName( VOID );
msimRC msimUpdateAfterSimulation( msimWID Owner, const ApplicationEvent& rAppEvent );
USHORT msimNumberNonZero( msimPSPECIES StartOfList );
void msimStartDeferredSimulation( );


#if defined(__PPC__)
void ChildDied( );
#endif


// defined in msimunit.cxx

PCHAR msimConcUnits( USHORT UnitType, USHORT VolumeOption );
PCHAR msimEnergyUnits( USHORT UnitType );
PCHAR msimTimeUnits( USHORT UnitType );
PCHAR msimPressureUnits( USHORT UnitType );
PCHAR msimVolumeUnits( USHORT UnitType );
msimFLOAT msimConvFactorVolumeFromLiters( USHORT VolumeUnits );
msimFLOAT msimConvFactorTimeFromSec( USHORT TimeUnits );
msimFLOAT msimConvFactorTimeToSec( USHORT TimeUnits );
msimFLOAT msimConvFactorEnergyToCal( USHORT EnergyUnits );
msimFLOAT msimConvFactorPressFromAtm( USHORT PressUnits );
msimFLOAT msimConvFactorPressToAtm( USHORT PressUnits );
msimFLOAT msimConvFactorConcToMolar( USHORT ConcUnits );
msimFLOAT msimConvFactorConcFromMolar( USHORT ConcUnits );
msimFLOAT msimConvFactorAmtToMoles( USHORT ConcUnits );
PCHAR msimMolarDensityInMolarUnits( PCHAR Str, USHORT CurrentUnits );
PCHAR msimAFactorUnits( msimFLOAT NumSpecies, USHORT ConcUnits, USHORT
           VolumeUnits, USHORT TimeUnits );
PCHAR msimEaUnits( USHORT EnergyUnits );
void msimCalcAFactorConvExponent( msimPRXN Reaction, msimPFLOAT FwdExponent,
          msimPFLOAT RevExponent );


// defined in msimtext.cxx

msimRC msimCreateTextSummary( msimPINSTANCE Instance, msimWID Owner );


// defined in msimutil.cxx

VOID msimBeep( VOID );
VOID msimCenterDialogWindowOnOwner( msimWID Dialog, msimWID Owner );
msimBOOL msimOKToInvalidateRxnData( msimWID Owner, msimPINSTANCE Instance );
VOID msimCascadeWindowOnOwner( msimWID Dialog, msimWID Owner );
void msimSaveCurrentWindowGeometry( msimWID pWindow, msimPWIN_GEOMETRY WinGeom );
void msimRestoreCurrentWindowGeometry( msimWID pWindow, msimPWIN_GEOMETRY
          WinGeom );
void msimRestoreCurrentWindowPosition( msimWID pWindow, msimPWIN_GEOMETRY );
void msimClosePlotDialogWindow( msimPINSTANCE Instance );
void msimShowPlotDialogWindow( msimPINSTANCE Instance );
void msimHidePlotDialogWindow( msimPINSTANCE Instance );

void msimCloseNotebookWindow( msimPINSTANCE Instance );
VOID msimEntryFieldErrorMsg( msimWID Owner );


// defined in msimvrfy.cxx

msimRC msimVerifySpeciesDataDialog( msimWID Owner, msimPINSTANCE Instance );


// defined in msimvvol.cxx

msimRC msimChangePhaseDataDlg( msimWID Owner, msimPINSTANCE pInstance );
msimBOOL msimCheckInitialPhase( msimPINSTANCE pInstance);

// defined in msimconc.cxx

msimRC msimSetConcentrations( msimPINSTANCE Instance, msimWID Owner );

// defined in msimextd.cxx

msimRC msimSelectExternalDataTypesDlg( msimWID Owner, USHORT VolumeOption,
        USHORT& rXDataType, USHORT& rYDataType, PCHAR Filename );



#if      defined(__OS2__)
                                        // defined in msimos2.cxx
msimBOOL msimStopSession( ULONG Session );
ULONG msimRunSimulation( ULONG PTR pSessionID, PCHAR SimulatorFilename, PCHAR RunparmFilename, PCHAR SimOwnerName );
ULONG msimWaitForSimCompletion( void );
msimRC msimCloseQueue( void );
msimRC msimCreateQueue( void );

#endif

#if defined(__MAC__)

void SetInactive( Control& rText);
void SetActive( Control& rText);

#endif



/* macros defined here                                                      */

#define  msimSeeIfReversible(x)                  (msimBOOL) (NULL != strstr( (\
                  x), "<=>") )

#define  msimUpdateMainWinData(flag)         aMainApp.PostUserEvent( (ULONG) (flag), (void*) (NULL) )


#define  msimINVALID_WINDOW_GEOMETRY(geometry)   ( (geometry.width == 0) || (\
                  geometry.height == 0) )
#define  STR(x)                                  #x
#define  msimMACRO_AS_STR(v)                     STR(v)


#endif


