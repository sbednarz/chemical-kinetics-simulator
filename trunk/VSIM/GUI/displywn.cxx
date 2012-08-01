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
//  DISPLYWN.CXX - a module for use in the VSIM User interface
// ............................................................................
//
//  This module defines a Window specialized for presenting plots of numeric data
//  This is a base class
//
//  CREATED : 9.01.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "displywn.hxx"

#endif


#include "strings.hrc"
#include "popup.hrc"





// ----------------------------------------------------------------------------
//                         DisplayWindow::DisplayWindow( )
// ............................................................................
//
//
//    constructor 
//
// ----------------------------------------------------------------------------

DisplayWindow::DisplayWindow( Window* pParent ) :
ScalableWindow( pParent )
{
     ChangeMapMode( MAP_POINT );  
}





// ----------------------------------------------------------------------------
//                         DisplayWindow::ShowPrintDlg( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

void DisplayWindow::ShowPrintDlg()
{
    Printer     printer;

    PrinterSetupDialog print_setup_dlg( GetParent(), WB_SVLOOK | WB_STDMODAL );
    PrintDialog        print_dlg( GetParent(), WB_SVLOOK | WB_STDMODAL );

    print_dlg.ChangePrinter( &printer );
    print_dlg.ChangeSetupDialog( &print_setup_dlg );
    print_dlg.ChangeCopyCount( 1 );
    print_dlg.ChangeFirstPage( 1 );
    print_dlg.ChangeLastPage( 1 );
    print_dlg.EnablePageFields( FALSE );
    print_dlg.EnableSelection( FALSE );
    print_dlg.EnableCollate( FALSE );

    if ( print_dlg.Execute() )
    {
        printer.SetCopyCount( print_dlg.GetCopyCount() );

        // translate screen font into printer font

        Font screen_font = GetFont();

        Size font_size = LogicToPixel( screen_font.GetSize(), GetMapMode() );

        font_size = PixelToLogic( font_size, MAP_POINT );
        font_size = printer.LogicToPixel( font_size, MAP_POINT );

        screen_font.ChangeSize( printer.PixelToLogic( font_size, printer.GetMapMode() ) );
        printer.ChangeFont( screen_font );

        if ( !printer.StartJob( GetText() ) )
             WarningBox( this, ResId( vID_POPUP_PRINTER_BUSY ) ).Execute( );
        else
        {
             // send the plot to printer

             PrintPlot( &printer );

             printer.EndPage();

             if ( !printer.EndJob() )
                  ErrorBox( this, ResId( vID_POPUP_PRINTJOB_FAILED_TO_END ) ).Execute();
        }

        Invalidate();

    }
}






// ----------------------------------------------------------------------------
//                   DisplayWindow::RetrieveSavedPlotAttributes()
// ............................................................................
//
//   retrieves info from application Config file
//
// ----------------------------------------------------------------------------

void DisplayWindow::RetrieveSavedPlotAttributes()
{
     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     // first retrieve font info

     String saved_font_name   = config.ReadKey( String( ResId( vID_STR_SAVED_FONT_FAMILY   )) );
     INT16 saved_font_height  = config.ReadKey( String( ResId( vID_STR_SAVED_FONT_HEIGHT )), String( 0 ) );

     Font font;

     // if there is no valid font info in Config file then use default font

     if ( ( saved_font_name.Len() > 0 ) && ( saved_font_height  >  0  ) )
     {
          font = Font( GetFontFamilyFromName( saved_font_name ), Size( 0, saved_font_height ) );
     }
     else
          font = GetFont();

     // resolve logical to physical font - needed for correct spacing/sizing

     FontMetric metric = GetFontMetric( font );
     font =  metric.GetPhysicalFont( );

     ChangeFont( font );

     String saved_color_scheme =  config.ReadKey(
      String( ResId( vID_STR_SAVED_COLOR_SCHEME ) ), String() );

     SetColorScheme( GetColorSchemeFromName( saved_color_scheme ) );
}




// ----------------------------------------------------------------------------
//                   DisplayWindow::SavePlotAttributes()
// ............................................................................
//
//   saves info in Config file for later recall
//
// ----------------------------------------------------------------------------

void DisplayWindow::SavePlotAttributes()
{
     Config config;

     // deal with settings in the 'Plot' group

     config.SetGroup( String( ResId( vID_STR_PLOTGROUP ) ) );

     // first save font info

     Font font = GetFont();

     config.WriteKey( String( ResId( vID_STR_SAVED_FONT_FAMILY ) ), 
      GetNameFromFontFamily( font.GetFamily() ) );

     config.WriteKey( String( ResId( vID_STR_SAVED_FONT_HEIGHT  )), font.GetSize().Height() );

     // then color scheme info 

     config.WriteKey( String( ResId( vID_STR_SAVED_COLOR_SCHEME  )),
      GetNameFromColorScheme( GetColorScheme() ) );
}






// ----------------------------------------------------------------------------
//                   DisplayWindow::GetFontFamilyFromName()
// ............................................................................
//
//   converts String to corresponding enum
//
// ----------------------------------------------------------------------------

enum FontFamily DisplayWindow::GetFontFamilyFromName( const String& rName )
{
     if ( rName.Len() )
     {
          if ( String( ResId( vID_STR_FONT_FAMILY_ROMAN )) == rName )
               return FAMILY_ROMAN;
          else
          if ( String( ResId( vID_STR_FONT_FAMILY_SWISS )) == rName )
               return FAMILY_SWISS;
           else
               return FAMILY_MODERN;

     }
     else
          return FAMILY_MODERN;
}








// ----------------------------------------------------------------------------
//                   DisplayWindow::GetNameFromFontFamily()
// ............................................................................
//
//   converts enum into corresponding String
//
// ----------------------------------------------------------------------------

String DisplayWindow::GetNameFromFontFamily( enum FontFamily Type )
{
     String name;

     switch ( Type )
     {
     case  FAMILY_ROMAN :

          name = String( ResId( vID_STR_FONT_FAMILY_ROMAN ));
          break;

     case FAMILY_SWISS :

          name = String( ResId( vID_STR_FONT_FAMILY_SWISS ));
          break;

     default :
     case FAMILY_MODERN :

          name = String( ResId( vID_STR_FONT_FAMILY_MODERN ));
          break;
     }

     return name;
}







