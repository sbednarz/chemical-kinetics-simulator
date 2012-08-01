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
/*  FILE NAME : msimlogo.cxx                                                */
/*                                                                          */
/*  This simply implements a timed logo screen                              */
/*                                                                          */
/*  Version 1.0  started Nov 29 1993                                        */
/*                                                                          */
/*  CHANGE HISTROY : 5.24.95 modify to allow bitmpa logo dialog (wdh)       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop

#define LOGO_VIEW_PERIOD       3500

void LogoWindow::StartTimer( )
{
     aTimer.ChangeTimeout( LOGO_VIEW_PERIOD );
     aTimer.ChangeTimeoutHdl( LINK( this, LogoWindow, TimeOutHandler ) );
     aTimer.Start( );
}


void LogoWindow::TimeOutHandler( Timer PTR )
{
     Hide( );
     aMainApp.SetLogoWindow( NULL );
     delete this;
}


LogoWindow::LogoWindow( )
: WorkWindow ( NULL, ResId ( msimLOGO_DLG ) ),
aFixedBitmap1 ( this, ResId ( 1 ) )
{
     FreeResource( );

     aFixedBitmap1.Show( );

     msimCenterDialogWindowOnOwner( this, NULL );
     StartTimer( );
     Show( );
     Update( );
}


