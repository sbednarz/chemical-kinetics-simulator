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
/*  FILE NAME : msimutil.cxx                                                */
/*                                                                          */
/*  This module  contains various utility functions for msim                */
/*                                                                          */
/*  Written using the Starview C++ class libraries to provide common code   */
/*  for mutliple platforms                                                  */
/*                                                                          */
/*  Version 1.1  started Aug 13, 1993                                       */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma  hdrstop


#if defined(DEBUG)

#include <stdlib.h>

void msimAssertMsg( PCHAR Filename,  int Line )
{
     String  str("Assertion failure\n");
     str = str + String(Filename) + " ; line : " + String(Line) + "\nAborting...";
     ErrorBox( NULL, WB_OK | WB_DEF_OK, str).Execute();
     abort();
}
#endif


/*--------------------------------------------------------------------------*/
/*                            msimBeep()                                    */
/*..........................................................................*/
/*                                                                          */
/* Function for producing an audible beep upon request                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimBeep( void )

{
     Sound::Beep( );
     return;
}

/*--------------------------------------------------------------------------*/
/*                 msimCascadeWindowOnOwner( )                              */
/*..........................................................................*/
/*                                                                          */
/* function to reposition a child window on its owner in a "cascade" sort   */
/* of positioning. We first try to offset  left left,                       */
/* then right from right, then upper from upper, then lower from lower      */
/* The offsets are given in percent of the owner window dimensions          */
/*--------------------------------------------------------------------------*/

#define  VERTICAL_OFFSET                         10
#define  HORIZ_OFFSET                            10
#define  MIN_SPACE_PIXELS                        20

VOID msimCascadeWindowOnOwner( msimWID Dialog, msimWID Owner )

{
     SHORT desired_h_offset, desired_v_offset;

     Size dlg_size = Dialog->GetSizePixel( );
     Size owner_size = Owner->GetSizePixel( );
     Point owner_pos = Owner->GetPosPixel( );

     SHORT dlg_width = dlg_size.Width ( );
     SHORT dlg_height = dlg_size.Height ( );
     SHORT owner_width = owner_size.Width ( );
     SHORT owner_height = owner_size.Height ( );
     SHORT owner_x = owner_pos.X ( );
     SHORT owner_y = owner_pos.Y ( );

     Size screen = System::GetScreenSizePixel( );
     SHORT screen_width = screen.Width ( );
     SHORT screen_height = screen.Height ( );

     SHORT new_x, new_y;

     desired_h_offset = ( HORIZ_OFFSET * owner_width ) / 100;
     desired_v_offset = ( VERTICAL_OFFSET * owner_height ) / 100;

     // calculate a trial position for new location of dlg

     new_x = owner_x + desired_h_offset;

     // off the left side of screen ?? move to right a bit

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = MIN_SPACE_PIXELS;

     // if we are off the right end of the screen try offset from right side
     // of owner

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = owner_x + owner_width - desired_h_offset - dlg_width;

     // if we are still off the right side then move over a bit from right edge

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = screen_width - MIN_SPACE_PIXELS - dlg_width;

     /* if we are back off the left edge of screen then center on screen */

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = ( screen_width - dlg_width ) / 2;

     /* now try the vertical offsets : + because + moves downward           */

     new_y = owner_y + desired_v_offset;
     // are we off the bottom of the screen ?? if so try to offset up from
     // bottom of owner

     if ( ( new_y + dlg_height ) > ( screen_height - MIN_SPACE_PIXELS ) )
          new_y = owner_y + owner_height - dlg_height - desired_v_offset;
     // if we are still off the bottom of screen then move up a bit from bottom

     if ( ( new_y + dlg_height ) > ( screen_height - MIN_SPACE_PIXELS ) )
          new_y = screen_height - MIN_SPACE_PIXELS - dlg_height;

     /* are we now off the top of the screen ?                              */

     // if so then move down a bit from top

     if ( new_y < MIN_SPACE_PIXELS )
          new_y = MIN_SPACE_PIXELS;

     Dialog->ChangePosPixel( Point( new_x, new_y ) );

     return;
}

void msimCenterDialogWindowOnOwner( msimWID Dialog, msimWID Owner )
{
     Size screen = System::GetScreenSizePixel( );
     SHORT screen_width = screen.Width ( );
     SHORT screen_height = screen.Height ( );

     Size owner_size;
     Point owner_pos;

     if ( ! Dialog )
          return;

     Size dlg_size = Dialog->GetSizePixel( );

     // deal with NULL owner
     if ( ! Owner )
     {
          owner_size = screen;// center on screen          owner_pos = Point) 0, 0);
     }
     else
     {
          owner_size = Owner->GetSizePixel( );
          owner_pos = Owner->GetPosPixel( );
     }


     SHORT dlg_width = dlg_size.Width ( );
     SHORT dlg_height = dlg_size.Height ( );
     SHORT owner_width = owner_size.Width ( );
     SHORT owner_height = owner_size.Height ( );
     SHORT owner_x = owner_pos.X ( );
     SHORT owner_y = owner_pos.Y ( );

     SHORT new_x, new_y;

     //  recalc coords for Dialog
     // center horizontally first

     new_x = ( owner_width - dlg_width ) / 2 + owner_x;

     if ( new_x < MIN_SPACE_PIXELS )
          new_x = MIN_SPACE_PIXELS;
     // are we off right side of screen ??
     // if so then move back such that we are MIN_SPACE_PIXELS from right edge
     // of screen

     if ( ( new_x + dlg_width ) > ( screen_width - MIN_SPACE_PIXELS ) )
          new_x = screen_width - MIN_SPACE_PIXELS - dlg_width;
     // now center vertically

     new_y = ( owner_height - dlg_height ) / 2 + owner_y;
     if ( new_y < MIN_SPACE_PIXELS )
          new_y = MIN_SPACE_PIXELS;
     // are we off bottom ?

     if ( new_y + dlg_height > screen_height - MIN_SPACE_PIXELS )
          new_y = screen_height - MIN_SPACE_PIXELS - dlg_height;

     Dialog->ChangePosPixel( Point( new_x, new_y ) );

     return;
}

/*--------------------------------------------------------------------------*/
/*                 msimOKToInvalidatePlotData( )                            */
/*..........................................................................*/
/*                                                                          */
/*  checks if plot window exists. If so then any changes to the rxn scheme  */
/*  could change/invalidate the data being plotted. We ask the user whether */
/*  is is OK to close                                                       */
/*                                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

msimBOOL msimOKToInvalidateRxnData( msimWID Owner, msimPINSTANCE Instance )
{
     // return w/o query if plot dialog does not exist

     if ( Instance->p_plot_dialog )
     {
          if ( RET_OK == WarningBox( Owner, ResId( msimINVALIDATE_DATA_MSG ) ) .Execute( ) )
          {
               msimClosePlotDialogWindow( Instance );
               return TRUE;
          }
          else
               return FALSE;
     }
     else
          return TRUE;
}

void msimSaveCurrentWindowGeometry( msimWID pWindow, msimPWIN_GEOMETRY WinGeom )
{
     Point win_position = pWindow->GetPosPixel( );
     Size win_size = pWindow->GetSizePixel( );

     WinGeom->x = win_position.X( );
     WinGeom->y = win_position.Y( );
     WinGeom->height = win_size.Height( );
     WinGeom->width = win_size.Width( );

     return;
}

void msimRestoreCurrentWindowGeometry( msimWID pWindow, msimPWIN_GEOMETRY
          WinGeom )
{

     pWindow->SetPosSizePixel( Point( WinGeom->x, WinGeom->y ), Size
          ( WinGeom->width, WinGeom->height ) );

     return;
}

void msimRestoreCurrentWindowPosition( msimWID pWindow, msimPWIN_GEOMETRY
          WinGeom )
{
     pWindow->ChangePosPixel( Point( WinGeom->x, WinGeom->y ) );

     return;
}




/*--------------------------------------------------------------------------*/
/*                          msimEntryFieldErrorMsg( )                       */
/*..........................................................................*/
/*                                                                          */
/* creates a dialog box notifying user of an error in user input            */
/* in an entry field                                                        */
/*                                                                          */
/* returns nothing                                                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/

void msimEntryFieldErrorMsg( msimWID Owner )
{
     WarningBox( Owner, ResId( msimENTRY_FIELD_ERROR ) ) .Execute( );
     return;
}




#if defined(__MAC__)

void SetInactive( Control& rText)
{
     Font f = rText.GetFont( );

         f.ChangeColor( COL_GRAY );
         rText.ChangeFont( f );

}

void SetActive( Control& rText)
{
     Font f = rText.GetFont( );

         f.ChangeColor( COL_BLACK );
         rText.ChangeFont( f );
}



#endif
