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
//  DISPLYWN.HXX - a header file for use in the VSIM User interface
// ............................................................................
//
//  This module declares the DisplayWindow class for use in displaying simulation
//  results in VSIM
//
//  CREATED : 8.31.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__DISPLYWN_HXX__)
     #define __DISPLYWN_HXX__


#include "scalwin.hxx"
#include "colors.hxx"


class DisplayWindow : public ScalableWindow
{
protected:
    void               RetrieveSavedPlotAttributes();
    void               SavePlotAttributes();

    enum FontFamily    GetFontFamilyFromName( const String& rName );
    String             GetNameFromFontFamily( enum FontFamily Type );

    virtual void       PrintPlot( Printer* pPrinter ) = 0;

public:

     DisplayWindow( Window* pParent );

     void                       ShowPrintDlg( );
     virtual COLOR_SCHEME_TYPE  GetColorScheme() const { return DEFAULT_COLOR_SCHEME; }
     virtual void               SetColorScheme( enum COLOR_SCHEME_TYPE NewScheme ) {}
};


#endif

