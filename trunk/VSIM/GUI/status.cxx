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

// ---------------------------------------------------------------------------
//  STATUS.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the StatusDialog dialog used in the
//  the VSIM user interface.
//
//  CREATED : 1.18.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "schemwin.hxx"
#include "mainapp.hxx"
#include "status.hxx"

#endif


#include "status.hrc"
#include "strings.hrc"



#if defined(OS2)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MSDOS__)
#define FILE_END_OF_LINE     "\r"
#define MLE_END_OF_LINE     "\r\n"
#endif

#if defined(__AIX__)
#define FILE_END_OF_LINE     "\n"
#define MLE_END_OF_LINE     "\n"
#endif

#if defined(__MAC__)
#define FILE_END_OF_LINE   "\r\n\xOD"
#define MLE_END_OF_LINE    "\n"
#endif


const static String MLE_EOL( MLE_END_OF_LINE );





// ----------------------------------------------------------------------------
//                             class StatusDialog
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class StatusDialog : public ModalDialog
{
protected:
     ListBox              status_type_lb;
     MultiLineEdit        details_mle;
     OKButton             ok_btn;
     HelpButton           help_btn;
     SchemeFrameWindow*   pparent;

     International  intl; 

	void            ChangeDetails();

     ReactionScheme& GetScheme() { return pparent->GetReactionScheme(); }

     void            FormatFileStatistics( String& rText );
     void            FormatSchemeStatistics( String& rText );
     void            FormatSimulationStatistics( String& rText );

     const String&   ElapsedTime( const Time& rTime );
     const String&   GetTimeStamp( const DateTime& rTimeStamp );

public:
                     StatusDialog( SchemeFrameWindow* pParent );

     void            SelectHandler( ListBox* );
};





// ----------------------------------------------------------------------------
//                   StatusDialog::StatusDialog( )
// ............................................................................
//
// constructor 
//
// ----------------------------------------------------------------------------

StatusDialog::StatusDialog( SchemeFrameWindow * pParent ) :
ModalDialog( pParent, ResId( vID_DLG_STATUS ) ),
status_type_lb( this, ResId( vID_LB_STATUS_TYPE ) ),
details_mle( this, ResId( vID_MLE_DETAILS ) ),
ok_btn( this, ResId( vID_BTN_OK ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
pparent( pParent ),
intl()
{
     FreeResource();
     intl.ChangeNumDigits( 0 );

     status_type_lb.SelectEntryPos( 0 );

     ChangeDetails();

     status_type_lb.ChangeSelectHdl( LINK( this, StatusDialog, SelectHandler ) );

     GetpMainApp()->CenterWindowOnParent( this, GetpMainApp()->GetAppWindow() );
}







// ----------------------------------------------------------------------------
//                   StatusDialog::SelectHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StatusDialog::SelectHandler( ListBox* )
{
     ChangeDetails();
}




// ----------------------------------------------------------------------------
//                   StatusDialog::ElapsedTime()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

const String& StatusDialog::ElapsedTime( const Time& rTime )
{
     static String str; 

     str = String();

     if ( rTime.GetHour() )
     {
          str = str + String( rTime.GetHour() ) + String( ResId( vID_STR_HOURS ) ); 
     }

     if ( rTime.GetMin() )
     {
          str = str + String( rTime.GetMin() ) + String( ResId( vID_STR_MINUTES) ); 
     }

     str = str + String( rTime.GetSec() ) + String( ResId( vID_STR_SECONDS) ); 

     return str;
}





// ----------------------------------------------------------------------------
//                   StatusDialog::ElapsedTime()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

const String& StatusDialog::GetTimeStamp( const DateTime& rTimeStamp )
{
     static String str; 

     if ( rTimeStamp == INVALID_DATE )
     {
          str = String( ResId( vID_STR_INVALID_TIME_STAMP ) );
     }
     else
     {
          str = intl.GetLongDate( rTimeStamp ) + 
          String( ResId( vID_STR_TIMESTAMP_SEP  ) ) + 
          intl.GetTime( rTimeStamp );
     }

     return str;
}





// ----------------------------------------------------------------------------
//                   StatusDialog::FormatSchemeStatistics()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StatusDialog::FormatSchemeStatistics( String& rText )
{
     rText = String( ResId( vID_STR_SCHEMESTATS_1 ) ) +
          GetTimeStamp( GetScheme().GetFileCreationDate() ) + MLE_EOL +
          String( ResId( vID_STR_SCHEMESTATS_3 ) ) +
          GetTimeStamp( GetScheme().GetSchemeModificationDate() ) + MLE_EOL +
          String( ResId( vID_STR_SCHEMESTATS_5 ) ) +
          GetTimeStamp( GetScheme().GetNotebookModificationDate() ) + MLE_EOL;


     rText = rText + String( ResId( vID_STR_SCHEMESTATS_7 ) ) + 
          String(  GetScheme().GetNumCompartments() ) + MLE_EOL +
          String( ResId( vID_STR_SCHEMESTATS_8 ) ) +  
          String(  GetScheme().GetNumTransferObjects() ) + MLE_EOL +
          String( ResId( vID_STR_SCHEMESTATS_9 ) ) +  
          String( GetScheme().GetpSpeciesDatabase()->GetNumberOfSpecies() );
}






// ----------------------------------------------------------------------------
//                   StatusDialog::FormatSimulationStatistics()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StatusDialog::FormatSimulationStatistics( String& rText )
{
     rText = String( ResId( vID_STR_SIMULATIONSTATS_1 ) ) +  
          GetTimeStamp( GetScheme().GetSimulationStartDate() ) + MLE_EOL +
          String( ResId( vID_STR_SIMULATIONSTATS_3 ) ) + 
          ElapsedTime( GetScheme().GetCumulativeSimulationTime() ) +
          String( ResId( vID_STR_SIMULATIONSTATS_4 ) ) + MLE_EOL +
          String( ResId( vID_STR_SIMULATIONSTATS_5 ) ) + 
          ElapsedTime( GetScheme().GetLastStepSimulationTime() ) +
          String( ResId( vID_STR_SIMULATIONSTATS_6 ) ) + MLE_EOL +
          String( ResId( vID_STR_SIMULATIONSTATS_7 ) ) + 
          intl.GetNum( GetScheme().GetNumSimulationDataPoints() ) ;
}







// ----------------------------------------------------------------------------
//                   StatusDialog::FormatFileStatistics()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StatusDialog::FormatFileStatistics( String& rText )
{
     DirEntry dir_entry( GetScheme().GetName() );

     if ( dir_entry.Exists() )
     {
          FileStat file_stat( dir_entry );
     
          INT32  file_size = file_stat.GetSize();
          INT32  sim_data_offset = GetScheme().GetSimulationDataFilePosition();
          INT32  state_data_offset =  GetScheme().GetSimulationStateFileOffset();
      
          INT32  scheme_size = sim_data_offset;
          INT32  state_data_size = ( 0 == state_data_offset ) ? 0 : file_size - state_data_offset;
      
          INT32  simdata_size = file_size - state_data_size - scheme_size;
      
          Date mod_date = file_stat.DateModified();
     
     #if defined(OS2)
     
          // correct for incorrect year under OS/2
     
          const USHORT BASE_YEAR = 1980;
     
          mod_date.ChangeYear( mod_date.GetYear() + BASE_YEAR );
     
     #endif
          
          rText = String( ResId( vID_STR_FILESTATS_1 ) ) + GetScheme().GetName() + MLE_EOL +
            String( ResId( vID_STR_FILESTATS_2 ) ) + intl.GetNum( file_size ) + 
            String( ResId( vID_STR_FILESTATS_3 ) ) + MLE_EOL;
     
     
          if ( GetScheme().GetNumSimulationDataPoints() )
          {
               //we have simulation data points 
     
               rText = rText + String( ResId( vID_STR_FILESTATS_6 ) ) + 
                intl.GetNum( scheme_size ) + 
                String( ResId( vID_STR_FILESTATS_7 ) ) + MLE_EOL +
                String( ResId( vID_STR_FILESTATS_8 ) ) +
                intl.GetNum( simdata_size ) +
                String( ResId( vID_STR_FILESTATS_9 ) ) + MLE_EOL +
                String( ResId( vID_STR_FILESTATS_10 ) ) + 
                intl.GetNum( state_data_size ) + 
                String( ResId( vID_STR_FILESTATS_11 ) ) + MLE_EOL;
          }
          else
          {
               rText = rText + String( ResId( vID_STR_FILESTATS_12 ) ) + MLE_EOL;;
          }
     
            rText = rText + String( ResId( vID_STR_FILESTATS_4 ) ) + intl.GetLongDate( mod_date ) + 
            String( ResId( vID_STR_FILESTATS_5 ) ) + intl.GetTime( file_stat.TimeModified() );
     }
     else
     {
          rText = String( ResId( vID_STR_FILESTATS_13 ) );
     }
}






// ----------------------------------------------------------------------------
//                   StatusDialog::ChangeDetails()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void StatusDialog::ChangeDetails()
{
	String         text;

     switch ( status_type_lb.GetSelectEntryPos() ) 
     {

          case 0 :           // "File Statistics" 

               FormatFileStatistics( text );
               break;

     
          case 1 :           // "Scheme Statistics" 

               FormatSchemeStatistics( text );
               break;
     
          case 2 :           // "Simulation Statistics" 

               FormatSimulationStatistics( text );
               break;

     }

	details_mle.SetText( text );

}






// ----------------------------------------------------------------------------
//                   ShowStatusDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void ShowStatusDialog( SchemeFrameWindow* pWindow )
{
     StatusDialog* pdlg = new StatusDialog( pWindow );

     if ( pdlg )
     {
          BOOL8 rc = pdlg->Execute();
          delete pdlg;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
     }
}


