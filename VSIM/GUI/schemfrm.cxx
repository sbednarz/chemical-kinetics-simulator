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
//  SCHEMFRM.CXX - a module of the VSIM User interface 
// ............................................................................
//
//  This modules defines the SchemeFrameWindow class, an MDI window containing a view 
//  of a reaction scheme.
//
//  CREATED : 1.24.96 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "schemfrm.hxx"
#include "cschemwn.hxx"
#include "editspdb.hxx"
#include "simopts.hxx"
#include "notebook.hxx"
#include "status.hxx"
#include "mainapp.hxx"

#include "fileutil.hxx"
#include "showrslt.hxx"
#include "assert.hxx"


#endif


#include "mainmenu.hrc"
#include "strings.hrc"
#include "icons.hrc"
#include "popup.hrc"
#include "help.hhc"



const USHORT       SchemeFrameWindow::NUM_COMPTS_STR_INDEX     = 2;
const USHORT       SchemeFrameWindow::NUM_XFERPATHS_STR_INDEX  = 3;



















// ----------------------------------------------------------------------------
//                       SchemeFrameWindow::SchemeFrameWindow()
// ............................................................................
//
//   constructor 
//
// ----------------------------------------------------------------------------

SchemeFrameWindow::SchemeFrameWindow( enum SCHEMWIN_TYPE SchemeType ) :
ScrollingMDIWindow( GetpApp()->GetAppWindow(), NULL, ResId( vID_TOOLBOX_SCHEME) ),
scheme_type( SchemeType ),
p_notebook_dlg( NULL )
{
     switch ( SchemeType )
     {
          case SW_TYPE_COMPARTMENT :

               pscheme_win = new CompartmentSchemeWindow( this );
               break;

          default :
          case SW_TYPE_UNKNOWN:

               pscheme_win = NULL;
               break;

          case SW_TYPE_3D:

               pscheme_win = new ThreeDArraySchemeWindow( this );
               break;
     }


     if ( NULL != pscheme_win)
     {
          SetpDrawingArea( pscheme_win );
     
          SetStandardIcon();

          SetWindowTitle( );
      
         // set up status bar
     
         GetStatusBar().InsertItem( NUM_COMPTS_STR_INDEX, GetTextSize(
          String( ResId(vID_STR_000)) + String( ResId( vID_STR_NUM_COMPTS ))).Width() );
     
         GetStatusBar().InsertItem( NUM_XFERPATHS_STR_INDEX, GetTextSize(
          String( ResId(vID_STR_000)) + String( ResId( vID_STR_NUM_XFERPATHS ))).Width() );
     
         UpdateStatusBar();
     
         // initialize tool bar
     
         GetToolBar().ChangeHighlightHdl(  LINK( this, SchemeFrameWindow, ToolBarHighlight ) );
         GetToolBar().ChangeSelectHdl( LINK( this, SchemeFrameWindow, ToolBarSelect ) );
     
          // disable access to Species Properties since no species are defined yet
     
         GetToolBar().EnableItem( vID_TI_SPECIES_PROPERTIES, FALSE );
     }
}







// ----------------------------------------------------------------------------
//                       SchemeFrameWindow::~SchemeFrameWindow()
// ............................................................................
//
//   destructor 
//
// ----------------------------------------------------------------------------

SchemeFrameWindow::~SchemeFrameWindow( )
{
     if ( pscheme_win )
          delete pscheme_win;

     if ( p_notebook_dlg )
          delete p_notebook_dlg;

     GetpMainApp()->RemoveFromWindowList( this );
}



// ----------------------------------------------------------------------------
//                       SchemeFrameWindow::SetWindowTitle()
// ............................................................................
//
//   
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::SetWindowTitle( ) 
{ 
     SetText( GetReactionScheme().GetNickName() + String( ResId( vID_STR_SCHEME_TITLE_SUFFIX)) ); 
}





// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::Activate( )
// ............................................................................
//
//  called upon activation of MDI Window. We activate/deactivate ToolButtons
//  as necessary
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::Activate( )
{
     UpdateToolbarActivations( );
     GetpMainApp()->UpdateMenuActivation( this );
     GetSchemeWindow().Activate();


}




// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::Deactivate( )
// ............................................................................
//
//  called upon deactivation of MDI Window
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::Deactivate()
{
     GetSchemeWindow().Deactivate();
}








// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::ToolBarSelect( )
// ............................................................................
//
//  handler for ToolBar select events
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::ToolBarSelect( ToolBox* pBox )
{
     switch ( pBox->GetCurItemId() )
     {
          case vID_TI_SCHEMEWIN_HELP :


            GetpMainApp()->GetHelp()->Start( HLP_SCHEMWIN );
               break;


          case vID_TI_SPECIES_PROPERTIES :

               if ( ED_RC_ACCEPT == EditSpeciesDatabase( this ) )
                    GetReactionScheme().SetStatus( SIM_DATA_MODIFIED );

               break;


          case vID_TI_USER_NOTES :

               EditNotebook( this );

               break;


          case vID_TI_SIM_OPTIONS:

               if ( SO_RC_ACCEPT == EditSimulationOptions( this ) )
                    GetReactionScheme().SetStatus( SIM_DATA_MODIFIED );

               break;


          case vID_TI_RESULTS:

               ViewResults( );

               break;


          case vID_TI_START_SIMULATION:

               StartSimulation();
               UpdateToolbarActivations();

               break;


          case vID_TI_ABORT_SIMULATION:

               if ( OKToAbortSimulation() )
               {
                    if ( !GetReactionScheme().AbortSimulation() )
                       ErrorBox( NULL, vID_POPUP_SIMULATOR_ABORT_FAILED ).Execute();

                    UpdateToolbarActivations();
               }

               break;


          case vID_TI_SUMMARY :

               ShowStatusDialog( this );

               break;
     }

     UpdateStatusBar();
}





// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::SetName( )
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::SetName( const String& rName )
{
     GetReactionScheme().SetName( rName );
     SetWindowTitle( );
}








// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::ViewResults( )
// ............................................................................
//
// 
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::ViewResults( )
{
     if ( FALSE == GetReactionScheme().LoadResults() )
     {
          GetpMainApp()->OutOfMemory();
          return;
     }

     ShowViewResultDlg( this );
}








// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::ToolBarHighlight( )
// ............................................................................
//
//  Set help text appropriately
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::ToolBarHighlight( ToolBox* pBox )
{
     String text = pBox->GetHelpText( pBox->GetCurItemId( ) );

     if ( ! text )
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( GetpMainApp()->GetAppWindow()->GetHelpText( ) );
     else
          GetpMainApp()->GetMainWindow()->GetStatusBar( ).SetText( text );

    GetpMainApp()->SetWindowUnderPointer( NULL );
}




// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::OKToAbortSimulation()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 SchemeFrameWindow::OKToAbortSimulation() 
{
     String msg( ResId( vID_STR_SCHEME_SIM_ABORT_1) );


              msg = msg + GetReactionScheme().GetNickName( ) + String( ResId( vID_STR_SCHEME_SIM_ABORT_2 ) );
     
              return ( RET_YES == QueryBox( NULL, WB_YES_NO, msg ).Execute() );
}
     




// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::SaySimulationIsRunning()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::SaySimulationIsRunning()
{
     String msg( ResId( vID_STR_SCHEME_SIM_RUNNING_1) );


              msg = msg + GetReactionScheme().GetNickName( ) + String( ResId( vID_STR_SCHEME_SIM_RUNNING_2 ) );
     
              InfoBox( NULL, msg ).Execute();
}
     




// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::Close( )
// ............................................................................
//
//  check with user that it is ok to close window. If the file name is empty,
//  the present file dialog to get a name
//
// ----------------------------------------------------------------------------

BOOL SchemeFrameWindow::Close()
{
     switch ( GetReactionScheme().GetStatus() )
     {
          case SIM_NEW :

               // an empty scheme - throw it away 
               break; 


          case SIM_RUNNING :
#if 0 
          {
              if ( OKToAbortSimulation() )
              {
                  if ( !GetReactionScheme().AbortSimulation() )
                  {
                       ErrorBox( NULL, vID_POPUP_SIMULATOR_ABORT_FAILED ).Execute();
                       return FALSE;
                  }
              }
              else
              {
                   return FALSE;
              }
          }
          break;
#endif
               SaySimulationIsRunning();
               return FALSE;
               break;
     
          case SIM_DATA_MODIFIED :
          {
              String msg( ResId( vID_STR_SCHEME_WIN_CLOSE1 ) );
     
              msg = msg + GetReactionScheme().GetNickName( ) + String( ResId( vID_STR_SCHEME_WIN_CLOSE2 ) );
     
              QueryBox aBox( NULL, WB_YES_NO_CANCEL, msg );
     
               switch ( aBox.Execute() )
               {
               case RET_CANCEL :
     
                    return FALSE;
     
               case RET_YES :
     
                    // if full name is a null string then reaction scheme is untitled - give it a name
     
                    if ( !GetReactionScheme().GetName() )
                    {
                         String name;
     
                         if ( SaveAsFileDialog( name, RXNSCHEME_FILETYPE ) )
                         {
                              GetpMainApp()->RemoveFromWindowList( this );
                              SetName( name );
                              GetpMainApp()->AddToWindowList( this );
                              SaveScheme( );
                         }
                         else
                              // do not continue with Close operation if user cancelled 'save as'
     
                              return FALSE;
                    }
                    else
                         SaveScheme();
     
                    break;
     
               case RET_NO :
     
                    break;
               }
          }

          break;
     
     
          default:

          {
               if ( FALSE == GetSchemeWindow().DeleteResultsWindows() )
                    return FALSE;

               if ( GetReactionScheme().IsNotebookTextModified()) 
               {
                    String msg( ResId( vID_STR_SCHEME_WIN_CLOSE3 ) );
           
                    msg = msg + GetReactionScheme().GetNickName( ) + String( ResId( vID_STR_SCHEME_WIN_CLOSE4 ) );
           
                    QueryBox aBox( NULL, WB_YES_NO_CANCEL, msg );
           
                     switch ( aBox.Execute() )
                     {
                     case RET_CANCEL :
           
                          return FALSE;
           
                     case RET_YES :
           
                          GetReactionScheme().WriteNotebookTextToFile();
                          break;
           
                     case RET_NO :
           
                          break;
                     }
               }

               SaveWindowGeometry();
          }

          break;
     }

    GetpMainApp()->RemoveFromWindowList( this );
    GetpMainApp()->AddNameToReloadMenu( GetReactionScheme().GetName() );

    // is this the last window ?? if so then deactivate all Toolbar fcns

    if ( GetpMainApp()->GetWindowCount() == 1 )
    {
         MainWindow*  pmainwin= GetpMainApp()->GetMainWindow();

         pmainwin->ActivateEditTools( FALSE );
         pmainwin->ActivateZoomTools( FALSE );
         pmainwin->ActivatePaste( FALSE );
         pmainwin->ActivateAddCompartment( FALSE );
         pmainwin->ActivateAddTransferObject( FALSE );
         pmainwin->ActivateSelectAllTools( FALSE );
         pmainwin->Activate3DSpecialEditTools( FALSE );                                     

         GetpMainApp()->UpdateMenuActivation( NULL );
    }

    GetpMainApp()->Wait( TRUE );

    delete this;

    GetpMainApp()->Wait( FALSE );


    return TRUE;
}





// ----------------------------------------------------------------------------
//                   SchemeFrameWindow::UpdateToolbarActivations()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::UpdateToolbarActivations()
{
    enum SIM_STATUS_CODE current_status = GetReactionScheme().GetStatus();

     const BOOL8 SIM_IS_NOT_RUNNING = (SIM_RUNNING != current_status);

    // if there are no compartments left then deactivate the edit
    // buttons on the toolbar

    MainWindow* pmw = GetpMainApp()->GetMainWindow();

    enum SCHEMWIN_TYPE  scheme_type = GetSchemeWindow().GetSchemeType();

    ASSERT ( NULL != pmw && scheme_type != SW_TYPE_UNKNOWN );

    pmw->ActivateEditTools( SIM_IS_NOT_RUNNING && ( GetReactionScheme().GetNumCompartments() > 0 )  &&
     GetSchemeWindow().IsObjectSelected() && !IsMinimized() );

    pmw->ActivatePaste( SIM_IS_NOT_RUNNING && !IsMinimized() );

    // similarly de-activate AddTransferObject btn of there is less than
    // two compartments

    pmw->ActivateAddTransferObject( SIM_IS_NOT_RUNNING && ( SW_TYPE_COMPARTMENT == scheme_type ) && 
                                      !IsMinimized() && 
                                    ( GetReactionScheme().GetNumCompartments() > 1 ) 
                                  );

    pmw->ActivateAddCompartment( SIM_IS_NOT_RUNNING && ( SW_TYPE_COMPARTMENT == scheme_type ) && !IsMinimized( ) );
    pmw->ActivateZoomTools( SIM_IS_NOT_RUNNING && !IsMinimized( ) );

    GetToolBar().EnableItem( vID_TI_SPECIES_PROPERTIES, SIM_IS_NOT_RUNNING && (GetReactionScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() > 0 ) );

 
    switch( current_status )
    {

    case SIM_TERMINATE_ZERO_PROBABILITY   :
    case SIM_TERMINATE_EVENT_LIMIT        :
    case SIM_TERMINATE_RECORD_LIMIT       :
    case SIM_TERMINATE_ELAPSED_TIME_LIMIT :
    case SIM_TERMINATE_FINAL_TEMP_REACHED :
    case SIM_TERMINATE_USER_ABORT         :
 
         GetToolBar().EnableItem( vID_TI_RESULTS, ( GetReactionScheme().GetNumSimulationDataPoints() > 0 ) );
 
         break;
 
 
    default :
 
          // otherwise turn it off 

         GetToolBar().EnableItem( vID_TI_RESULTS, FALSE );
 
         break;
    }                     
                        
    GetToolBar().EnableItem( vID_TI_ABORT_SIMULATION, !SIM_IS_NOT_RUNNING );
    GetToolBar().EnableItem( vID_TI_START_SIMULATION, SIM_IS_NOT_RUNNING && (GetReactionScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() > 0 ) );
    GetToolBar().EnableItem( vID_TI_SIM_OPTIONS,       SIM_IS_NOT_RUNNING );
    GetToolBar().EnableItem( vID_TI_USER_NOTES,        SIM_IS_NOT_RUNNING );
    GetToolBar().EnableItem( vID_TI_SUMMARY,           SIM_IS_NOT_RUNNING );
}







// ----------------------------------------------------------------------------
//                     SchemeFrameWindow::UpdateStatusBar()
// ............................................................................
//
//  a utility function to update the data shown on the SchemeFrameWindow Status Bar
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::UpdateStatusBar()
{
    GetStatusBar().SetText( GetReactionScheme().GetStatusStr() );

    GetStatusBar().SetItemText( NUM_COMPTS_STR_INDEX, String( GetReactionScheme().GetNumCompartments() ) +
     String( ResId(
     1 == GetReactionScheme().GetNumCompartments() ? vID_STR_NUM_COMPT_SINGULAR : vID_STR_NUM_COMPTS ) ) );

    GetStatusBar().SetItemText( NUM_XFERPATHS_STR_INDEX, String( GetReactionScheme().GetNumTransferObjects() ) +
    String( ResId( 1 == GetReactionScheme().GetNumTransferObjects() ? vID_STR_NUM_XFERPATH_SINGULAR :
    vID_STR_NUM_XFERPATHS ) ) );

//wdh    GetpMainApp()->UpdateMenuActivation( GetpMainApp()->GetActiveWindow() );
}





// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::KeyInput( )
// ............................................................................
//
//  intercept key input and deal with it
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::KeyInput( const KeyEvent& rKeyEvt )
{

     if ( KEY_ESCAPE ==  rKeyEvt.GetKeyCode( ) .GetCode( ) )
     {
          switch ( GetSchemeWindow().GetOperation() )
          {
           case SW_CMPTOBJ_ADD :

               GetSchemeWindow().CancelAddCompartmentObject();
               break;

           case SW_XFEROBJ_ADD :

               GetSchemeWindow().CancelAddTransferObject();
               break;


           default :

               return;
          }
     }
     else
          MDIWindow::KeyInput( rKeyEvt );

}





// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::StartSimulation( )
// ............................................................................
//
//  this function is called to start a simulation
//  returns TRUE if the save is successful, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8  SchemeFrameWindow::StartSimulation( )
{
	// do we have a name yet? if not then request one from user 

     if ( !GetReactionScheme().GetName() )
     {
          String name;
     
          if ( SaveAsFileDialog( name, RXNSCHEME_FILETYPE ) )
          {
               GetpMainApp()->RemoveFromWindowList( this );
               SetName( name );
               GetpMainApp()->AddToWindowList( this );
          }
          else 
             return FALSE;
     }

     // check for nonzero concentrations 

     if ( 0.0 == GetReactionScheme().GetTotalInitialMoles() )
     {
          InfoBox( NULL, ResId( vID_POPUP_ALL_CONCENTRATIONS_ZERO ) ).Execute();
          return FALSE;
     }

     BOOL8 result;

     // if we successfully save the scheme to file

     if( SaveScheme() )
     {
          //  create the runparm file using a temporary name 

// wdh not yet           DirEntry runparm_filename( FSYS_FLAG_TEMPORARY );
          DirEntry runparm_filename( "runparm.tst" );
          runparm_filename.ToAbs();

          TextOutputStream outfile( runparm_filename.GetFull() );


          if ( !outfile.IsOpen() )
          {
               GetpMainApp( )->FileOpenError( outfile.GetName() );
               return FALSE;
          }

          GetReactionScheme().SetRunParameterFileName( runparm_filename.GetFull() );

          outfile << GetReactionScheme();
 
          result = !outfile.FileError( );
     }
     else
          result = FALSE;

     if ( result )
     {
          GetReactionScheme().SetStatus( SIM_READY_TO_RUN );
          UpdateStatusBar();

          SetRunningIcon();
          Minimize();
          GetpDrawingArea()->Disable();

          GetReactionScheme().RunSimulation();      


     }

     return result;
}





// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::SaveWindowGeometry()
// ............................................................................
//
//  this function is called to save the current window position and size to 
//  an existing file
//
// ----------------------------------------------------------------------------

void  SchemeFrameWindow::SaveWindowGeometry( )
{
     BinaryOutputStream outfile( GetReactionScheme().GetName(), FILE_MUST_EXIST );

     if ( outfile.IsOpen() )
     {
          outfile.MoveTo( SIMFRAME_WIN_GEOMETRY_POSITION );

             if ( IsMinimized() )
             {
                  outfile << Point();
                  outfile << Size();
             }
             else
             {
                  outfile << GetPosPixel();
                  outfile << GetSizePixel();
             }

             Fraction scale = GetSchemeWindow().GetScale();

             outfile << scale.GetNumerator();
             outfile << scale.GetDenominator();
     }
}



// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::SaveScheme( )
// ............................................................................
//
//  this function is called to save the entire reaction scheme to file. It
//  returns TRUE if the save is successful, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8  SchemeFrameWindow::SaveScheme( )
{
      
     BinaryOutputStream outfile( GetReactionScheme().GetName(), REPLACE_IF_EXISTS );

     if ( !outfile.IsOpen() )
     {
          GetpMainApp( )->FileOpenError( GetReactionScheme().GetName() );
          return FALSE;
     }

	// ask if ok to destroy results windows if so then xontinue with save 

     if ( GetSchemeWindow().DeleteResultsWindows() )
     {
          outfile << *this;

          // how did we do?

          BOOL8 result = !outfile.FileError();

          if ( result)
               GetReactionScheme().SetStatus( SIM_DATA_UNMODIFIED );

          UpdateStatusBar();

          return result;
     }
     else
          return FALSE;
}







// ----------------------------------------------------------------------------
//                         SchemeFrameWindow::LoadScheme( )
// ............................................................................
//
//  this function is called to load an entire reaction scheme from a file. It
//  returns TRUE if the load is successful, FALSE otherwise
//
// ----------------------------------------------------------------------------

BOOL8  SchemeFrameWindow::LoadScheme( const String& rFilename )
{
     BinaryInputStream infile( rFilename );

     if ( !infile.IsOpen() )
     {
          GetpMainApp( )->FileOpenError( GetReactionScheme().GetName() );
          return FALSE;
     }

     infile >> *this;

     UpdateStatusBar();
     GetSchemeWindow().RecalcTotalSize();
     RecalcWindowLayout();
     UpdateToolbarActivations( );    
     Invalidate();


     return !infile.FileError( );
}



// ----------------------------------------------------------------------------
//                       SchemeFrameWindow::SetStandardIcon()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void SchemeFrameWindow::SetStandardIcon( )
{
     switch ( GetSchemeType() )
     {
          case SW_TYPE_COMPARTMENT :

               ChangeIcon( Icon( ResId( vID_ICO_SCHEME_ICON ) ) );
               break;

          default :
          case SW_TYPE_UNKNOWN:

               ChangeIcon( Icon( ResId( vID_ICO_SCHEME_ICON ) ) );
               break;

          case SW_TYPE_3D:

               ChangeIcon( Icon( ResId( vID_ICO_SCHEME2D_ICON ) ) );
               break;
     }
}




// ----------------------------------------------------------------------------
//                       SchemeFrameWindow::SetRunningIcon()
// ............................................................................
//
//
// ----------------------------------------------------------------------------
void SchemeFrameWindow::SetRunningIcon( ) 
{              
     switch (  GetSchemeType() )
     {
          case SW_TYPE_COMPARTMENT :

               ChangeIcon( Icon( ResId( vID_ICO_RUNNING_ICON ) ) );
               break;

          default :
          case SW_TYPE_UNKNOWN:

               ChangeIcon( Icon( ResId( vID_ICO_RUNNING_ICON ) ) );
               break;

          case SW_TYPE_3D:

               ChangeIcon( Icon( ResId( vID_ICO_RUNNING_ICON ) ) );
               break;
     }
}





// ----------------------------------------------------------------------------
//                  operator <<   SchemeFrameWindow
// ............................................................................
//
//  this function writes the internal data of a SchemeFrameWindow to a
//  BinaryOutputStream object
//
// ----------------------------------------------------------------------------

BinaryOutputStream& operator << ( BinaryOutputStream& rBOS, SchemeFrameWindow& rObject )
{
     // first write some general info as a header with fixed size

     rBOS << IDType( SCHEME_FRAME_WINDOW );                 // 4 bytes
     rBOS << IDType( CURRENT_OS );                          // 4 bytes
     rBOS << GetpMainApp()->GetMajorVersion( );             // 2 bytes
     rBOS << GetpMainApp()->GetMinorVersion( );             // 2 bytes
     rBOS << GetpMainApp()->GetBuildLevel( );               // 4 bytes
     rBOS << (UINT16) rObject.GetSchemeWindow( ).GetSchemeType();  // 2 bytes

     // save actual size and position only if we are not minimzed
     // otherwise save default point and size ( all equal to zero
     // 8 bytes total for Point and Size

     if ( rObject.IsMinimized() )
     {
          rBOS << Point();
          rBOS << Size();
     }
     else
     {
          rBOS << rObject.GetPosPixel();
          rBOS << rObject.GetSizePixel();
     }

     // save current scale;

     Fraction scale = rObject.GetSchemeWindow().GetScale();

     INT32 num = scale.GetNumerator(); 
     INT32 den = scale.GetDenominator(); 

     // another 8 bytes for scale info 

     rBOS << num;
     rBOS << den;

     // offset = 34d at this point

     rBOS << rObject.GetReactionScheme();

     return rBOS;
}



// ----------------------------------------------------------------------------
//                  operator >>   SchemeFrameWindow
// ............................................................................
//
//  this function sets the internal state of a SchemeFrameWindow using data from a
//  BinaryInputStream object
//
// ----------------------------------------------------------------------------

BinaryInputStream& operator >> ( BinaryInputStream& rBIS, SchemeFrameWindow& rObject )
{
     // first write some general info from the fixed-size header

     IDType id_type;

     rBIS >> id_type;

     if ( id_type != IDType( SCHEME_FRAME_WINDOW ) )
     {
          GetpMainApp()->InvalidInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag();
          return rBIS;
     }

     rBIS >> id_type;

     if ( id_type != IDType( CURRENT_OS ) )
     {
          GetpMainApp()->ForeignInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }

     // the VSIM version info is imbedded in the file but not yet used
     // so read in and discard

     UINT16 major;
     UINT16 minor;

     rBIS >> major;             // GetpMainApp()->GetMajorVersion( );
     rBIS >> minor;             // GetpMainApp()->GetMinorVersion( );

     UINT32 build_level;

     rBIS >> build_level;       // GetpMainApp()->GetBuildLevel( );

     if ( ( major != GetpMainApp()->GetMajorVersion( ) ) ||
          ( minor != GetpMainApp()->GetMinorVersion( ) ) ||
          ( build_level != GetpMainApp()->GetBuildLevel() )
       )
     {
          GetpMainApp()->ForeignInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }

     UINT16 temp;

     rBIS >> temp;

     // cast to enum 

     if (  (enum SCHEMWIN_TYPE) temp != rObject.GetSchemeWindow( ).GetSchemeType() )
     {
          GetpMainApp()->InvalidInputFile( rBIS.GetName() );
          rBIS.SetExtErrorFlag( );
          return rBIS;
     }


     Point position;
     rBIS >> position;

     Size size;
     rBIS >> size;

     // only set window size and position to saved values if
     // the size is not equil to zero and is totally within main window
     // otherwise adjust accordingly

     Rectangle parent_area = GetpMainApp()->GetMainWindow()->GetActiveArea();

     Rectangle new_rect    = Rectangle( position + parent_area.TopLeft(), size );
     Rectangle union_rect  = parent_area.GetUnion( new_rect );

     if ( ( parent_area != union_rect ) || ( size == Size() ) )
     {
          size.Width()  = ( 3 * parent_area.GetWidth() )/4;
          size.Height() = ( 3 * parent_area.GetHeight() )/4;

          position.X() = ( parent_area.GetWidth() - size.Width() ) /2;
          position.Y() = ( parent_area.GetHeight() - size.Height() ) /2;
     }

     rObject.SetPosSizePixel( position, size );

     // get the previous scale 

     INT32 num;
     INT32 den;

     rBIS >> num;
     rBIS >> den;

     Fraction scale( num, den );

     rObject.GetSchemeWindow().ChangeScale( scale );

     if ( rBIS.FileError() )
     {
          GetpMainApp()->FileReadError( rBIS.GetName() ); 
          return rBIS;
     }
        
     rBIS >> rObject.GetReactionScheme();          

     if ( rBIS.FileError() )
          return rBIS;

                  
     // allow access to species database dialog is appropriate

     rObject.GetToolBar().EnableItem( vID_TI_SPECIES_PROPERTIES,
      ( rObject.GetReactionScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() > 0 ) );

     rObject.SetName( rObject.GetReactionScheme().GetName( ) );

     return rBIS;
}



