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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimsetp.cxx                                                */
/*                                                                          */
/*  This module implements a print dialog for sending the current           */
/*  plot of simulation results to the printer                               */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for multiple platforms                                                  */
/*                                                                          */
/*  Version 1.0  started Nov  1993                                          */
/*                                                                          */
/*  Authors : Fumiko Allen and Bill Hinsberg                                */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#include "msimstrg.hxx"
#include "msimplot.hxx"

#include <string.h>
#include <stdlib.h>

class PrintDlg : public ModalDialog
{

protected :
     FixedText PlotLabel1;
     FixedText PlotLabel5;
     FixedText PlotLabel2;
     FixedText PlotLabel3;
     FixedText PrinterNameLabel;
     FixedText DriverNameLabel;
     FixedText PlotFilenameLabel;
     FixedText plotStatusLabel;
     PushButton PrinterSetupPB;
     GroupBox aGroupBox1;
     CheckBox aPrintAsMonochromeCB;
     OKButton ClosePB;
     PushButton StartPrintPB;
     PushButton CancelPrintPB;
     HelpButton HelpPB;

     msimPINSTANCE pSimInstance;
     PPLOT_SPECS pplot_specs;
     PLOT_SPECS print_plotspecs;
     msimBOOL CancelFlag;
     Window PTR owner;
     XYPrint PTR pPrinter;
     msimBOOL initialized;

     void InitializeEntry( );
     void ResetButtons( );

public :

     PrintDlg( Window PTR pParent,
          msimPINSTANCE PSimInstance, PPLOT_SPECS pPlotSpecs );
     ~PrintDlg( );

     void CloseHandler( PushButton PTR );
     void StartHandler( PushButton PTR );
     void CancelHandler( PushButton PTR );
     void SetUpHandler( PushButton PTR );

     msimBOOL IsInitialized( )
     {
          return initialized;
     };
};



// destructor -- frees allocated memory

PrintDlg::~PrintDlg( )
{
     if ( pPrinter )
          delete pPrinter;
}


// constructor

PrintDlg::
PrintDlg( Window PTR pParent,
     msimPINSTANCE PSimInstance, PPLOT_SPECS pPlotSpecs )
: ModalDialog ( pParent, ResId ( msimSETUP_PLOT_PANEL ) ),
PlotLabel1 ( this, ResId ( msimSETUP_PLOT_LABEL1 ) ),
PlotLabel5 ( this, ResId ( msimSETUP_PLOT_LABEL5 ) ),
PlotLabel2 ( this, ResId ( msimSETUP_PLOT_LABEL2 ) ),
PlotLabel3 ( this, ResId ( msimSETUP_PLOT_LABEL3 ) ),
PrinterNameLabel ( this, ResId ( msimSETUP_PLOT_PRINTERNAME ) ),
DriverNameLabel ( this, ResId ( msimSETUP_PLOT_DRIVERNAME ) ),
PlotFilenameLabel ( this, ResId ( msimSETUP_PLOT_FILENAME ) ),
plotStatusLabel ( this, ResId ( msimSETUP_PLOT_STATUS ) ),
PrinterSetupPB ( this, ResId ( msimSETUP_PLOT_PRINTER_SETUP ) ),
aGroupBox1 ( this, ResId ( 1 ) ),
aPrintAsMonochromeCB ( this, ResId ( msimSETUP_PLOT_MONOCHROME ) ),

ClosePB ( this, ResId ( msimSETUP_PLOT_CLOSE ) ),
StartPrintPB ( this, ResId ( msimSETUP_PLOT_START_PRINT ) ),
CancelPrintPB ( this, ResId ( msimSETUP_PLOT_CANCEL_PRINT ) ),
HelpPB ( this, ResId ( msimSETUP_PLOT_HELP ) )
{
     FreeResource( );

     owner = pParent;
     pSimInstance = PSimInstance;
     pplot_specs = pPlotSpecs;
     print_plotspecs = *pplot_specs;
     CancelFlag = FALSE;

     pPrinter = new XYPrint(this, &print_plotspecs );

     if ( ! pPrinter )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, this );

          initialized = FALSE;
          return;
     }
     else
          initialized = TRUE;


     // Initialize Entry panel -- may set the initialized flag to FALSE

     InitializeEntry( );

     ClosePB.ChangeClickHdl(
          LINK( this, PrintDlg, CloseHandler ) );
     StartPrintPB.ChangeClickHdl(
          LINK( this, PrintDlg, StartHandler ) );
     CancelPrintPB.ChangeClickHdl(
          LINK( this, PrintDlg, CancelHandler ) );
     PrinterSetupPB.ChangeClickHdl(
          LINK( this, PrintDlg, SetUpHandler ) );

     msimCascadeWindowOnOwner( this, pParent );
}


void PrintDlg::InitializeEntry( )
{
     String defaultprintername;

     defaultprintername = System::GetDefaultPrinterName( );

     if ( defaultprintername == String( msimNULL_STR ) )
     {
          // there is no default printer installed

          initialized = FALSE;

          WarningBox( this, ResId( msimNO_DEF_PRINTER ) ) .Execute( );

          return;
     }

     PrinterNameLabel.SetText( defaultprintername );
     DriverNameLabel.SetText( pPrinter->GetDriverName( ) );
     PlotFilenameLabel.SetText( String( pSimInstance->base_filename ) );
     CancelPrintPB.Disable( );
     aPrintAsMonochromeCB.Check( TRUE );

     return;
}





void PrintDlg::CloseHandler( PushButton PTR )
{
     EndDialog( );
}


void PrintDlg::StartHandler( PushButton PTR )
{
     // enable the Cancel button and disable the rest

     ClosePB.Disable( );
     StartPrintPB.Disable( );
     CancelPrintPB.Enable( );
     PrinterSetupPB.Disable( );
     CancelFlag = FALSE;

     // select set of colors as requested by user

     pPrinter->ChangeColorSet( aPrintAsMonochromeCB.IsChecked( ) ?
               msimBLACK_ON_WHITE : pplot_specs->current_colorset );

     // then take care of some settings which depend on the ColorSet setting


     pPrinter->ChangeFillInBrush( Brush( BRUSH_NULL ) );// suppresses fillin of rectangle

     pPrinter->ChangeBackgroundBrush( Brush( pplot_specs->background_color, BRUSH_SOLID ) );

     pPrinter->InitializeGeometricData( );

     pPrinter->InitializeFontData( pplot_specs->typeface, pplot_specs->pointsize );

     pPrinter->SetCopyCount( 1 );

     // OK - start the print job !

     if ( ! pPrinter->StartJob( String( pSimInstance->filename ) ) )
     {
          // the printer is already active

          WarningBox( this, ResId( msimPRINTER_ACTIVE ) ) .Execute( );

          // disable the Cancel button and enable the rest

          ResetButtons( );

          return;
     }
     plotStatusLabel.SetText( "Printing..." );

     pPrinter->StartPage( );
     pPrinter->DrawSelectedPlots( );
     pPrinter->EndPage( );
     if ( ! pPrinter->EndJob( ) )
     {
          // check to see if the EndJob() ended unsuccessfully due to
          // user intervention

          if ( ! CancelFlag )
          {
               // print job has not been ended successfully

               ErrorBox box( this, ResId( msimFAILED_TO_END ) );
               box.Execute( );

               // disable the Cancel button and enable the rest

               ResetButtons( );

               plotStatusLabel.SetText( "Failed" );

               return;
          }
     }

     // print job was completed successfully
     // disable the Cancel button and enable the rest

     ResetButtons( );

     plotStatusLabel.SetText( "Completed" );

     return;
}


void PrintDlg::CancelHandler( PushButton PTR )
{
     pPrinter->AbortJob( );

     // disable the Cancel button and enable the rest

     ResetButtons( );
     plotStatusLabel.SetText( "Canceled" );
     CancelFlag = TRUE;

     return;
}


void PrintDlg::SetUpHandler( PushButton PTR )
{
     if ( ! pPrinter->Setup( ) )
          if ( pPrinter->IsPrinting( ) )
          {
               // the printer is already active

               WarningBox( this, ResId( msimPRINTER_ACTIVE ) ) .Execute( );

               return;
          }

     return;
}



void PrintDlg::ResetButtons( )
{
     // disable the Cancel button and enable the rest

     ClosePB.Enable( );
     StartPrintPB.Enable( );
     CancelPrintPB.Disable( );
     PrinterSetupPB.Enable( );

     return;
}

void msimPrinterDialog( msimWID Owner, msimPINSTANCE pInstance, PPLOT_SPECS pPlotSpecs )
{
     PrintDlg PTR pdlg =
                  new PrintDlg( Owner, pInstance, pPlotSpecs );

     if ( pdlg )
     {
          if ( pdlg->IsInitialized( ) )
               pdlg->Execute( );

          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;
}
 