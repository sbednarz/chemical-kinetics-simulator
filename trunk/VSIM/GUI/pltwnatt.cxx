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
//  PLTWNATT.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the EditWindowAttributes dialog used in the plot routines
//  the VSIM user interface.
//
//  CREATED : 8.24.95
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------




#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include "pltwnatt.hxx"
#include "mainapp.hxx"

#include "assert.hxx"

#endif



#include "pltwnatt.hrc"

// ---------------------------------------------------------------------------
//                  class SampleWindow
// ............................................................................
//
//
//  for displaying sample text to show font/size/color selections to user 
//
// ----------------------------------------------------------------------------

class SampleWindow : public Window
{
public :

     SampleWindow( Window* pParent, const ResId& rResId );

     virtual void Paint( const Rectangle& );
};






// ---------------------------------------------------------------------------
//                  SampleWindow::SampleWindow()
// ............................................................................
//
//
//  constructor 
//
// ----------------------------------------------------------------------------

SampleWindow::SampleWindow( Window* pParent, const ResId& rResId ) :
Window( pParent, rResId )
{
     ChangeMapMode( MAP_POINT );
}




// ---------------------------------------------------------------------------
//                  SampleWindow::Paint()
// ............................................................................
//
//
// just show the text with selected attributes, centered in window 
//
// ----------------------------------------------------------------------------

void SampleWindow::Paint( const Rectangle& )
{
     INT16 x, y;
     Size text_size( GetTextSize( GetText( ) ) );
     Size window_size( GetOutputSize( ) );

     x = window_size.Width( ) / 2 - text_size.Width( ) / 2;
     y = window_size.Height( ) / 2 - text_size.Height( ) / 2;

     DrawText( Point( x, y ), GetText( ) );
}





// ---------------------------------------------------------------------------
//                  class WindowAttributesDlg
// ............................................................................
//
//
//    dialog for selecting fonts and color in window 
//
// ----------------------------------------------------------------------------

class WindowAttributesDlg : public ModalDialog
{
protected:
     RadioButton  black_bkgrd_rb;
     RadioButton  cyan_bkgrd_rb;
     RadioButton  gray_bkgrd_rb;
     RadioButton  white_bkgrd_rb;
     FixedBitmap  bitmap_1;
     FixedBitmap  bitmap_2;
     FixedBitmap  bitmap_3;
     FixedBitmap  bitmap_4;
     GroupBox     groupbox_1;
     SampleWindow sample_text_window;
     GroupBox     groupbox_2;
     OKButton     ok_btn;
     PushButton   undo_btn;
     HelpButton   help_btn;
     RadioButton  courier_rb;
     RadioButton  helvetica_rb;
     RadioButton  timesroman_rb;
     FixedText    fixed_text_1;
     ListBox      fontsize_lb;

     BOOL8        modified;
     enum  COLOR_SCHEME_TYPE current_color;

     void         InitializeDialog();
     void         InitializeListBox( const Font& rFont );
     void         SetModifiedFlag();
     void         SetFont( const Font& rFont );
     void         ChangeBackgroundColor();

public:
                   WindowAttributesDlg( DisplayWindow * pParent );

     void          UndoHandler( PushButton* );
     void          CloseHandler( OKButton* );
     void          RadioButtonClickHandler( RadioButton* pBtn );
     void          SelectFontHandler( RadioButton* pBtn );

     void          SelectSizeHandler( ListBox* pListBox );
};






// ---------------------------------------------------------------------------
//           WindowAttributesDlg::WindowAttributesDlg( )
// ............................................................................
//
//     constructor 
//
// ----------------------------------------------------------------------------

WindowAttributesDlg::WindowAttributesDlg( DisplayWindow * pParent ) :
ModalDialog( pParent, ResId( vID_DLG_PLOTWIN_ATTR ) ),
black_bkgrd_rb( this, ResId( vID_RB_BLACK_BKGRD ) ),
cyan_bkgrd_rb( this, ResId( vID_RB_CYAN_BKGRD ) ),
gray_bkgrd_rb( this, ResId( vID_RB_GRAY_BKGRD ) ),
white_bkgrd_rb( this, ResId( vID_RB_WHITE_BKGRD ) ),
bitmap_1( this, ResId( 1 ) ),
bitmap_2( this, ResId( 2 ) ),
bitmap_3( this, ResId( 3 ) ),
bitmap_4( this, ResId( 4 ) ),
groupbox_1( this, ResId( 1 ) ),
sample_text_window( this, ResId( vID_WIN_SAMPLE_TEXT ) ),
groupbox_2( this, ResId( 2 ) ),
ok_btn( this, ResId( vID_BTN_OK ) ),
undo_btn( this, ResId( vID_BTN_UNDO ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
courier_rb( this, ResId( vID_RB_COURIER_FONT ) ), 
helvetica_rb( this, ResId( vID_RB_HELVETICA_FONT ) ),  
timesroman_rb( this, ResId( vID_RB_TIMESROMAN_FONT ) ),  
fixed_text_1( this, ResId( 1 ) ),
fontsize_lb( this, ResId( vID_FONTSIZE_LB ) ), 
modified( FALSE ),
current_color( DEFAULT_COLOR_SCHEME )
{
     FreeResource();

     InitializeDialog();

     ok_btn.ChangeClickHdl( LINK( this, WindowAttributesDlg, CloseHandler ) );
     undo_btn.ChangeClickHdl( LINK( this, WindowAttributesDlg, UndoHandler ) );

     fontsize_lb.ChangeSelectHdl( LINK( this, WindowAttributesDlg, SelectSizeHandler ) );

     courier_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, SelectFontHandler ) );
     helvetica_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, SelectFontHandler ) );
     timesroman_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, SelectFontHandler ) );

     black_bkgrd_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, RadioButtonClickHandler ) );
     cyan_bkgrd_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, RadioButtonClickHandler ) );
     gray_bkgrd_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, RadioButtonClickHandler ) );
     white_bkgrd_rb.ChangeClickHdl( LINK( this, WindowAttributesDlg, RadioButtonClickHandler ) );

     GetpMainApp()->CenterWindowOnParent( this, GetParent() );

}





// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::InitializeDialog()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::InitializeDialog( )
{
     current_color = ((DisplayWindow*)GetParent())->GetColorScheme();

     ChangeBackgroundColor();

     black_bkgrd_rb.Check( current_color == COL_SCHEME_BLACK );
     cyan_bkgrd_rb.Check( current_color == COL_SCHEME_CYAN );
     gray_bkgrd_rb.Check( current_color == COL_SCHEME_GRAY );
     white_bkgrd_rb.Check( current_color == COL_SCHEME_WHITE );

     Font font = GetParent()->GetFont();
     SetFont( font );

     enum FontFamily family = font.GetFamily();

     timesroman_rb.Check( family == FAMILY_ROMAN ); 
     helvetica_rb.Check(  family == FAMILY_SWISS ); 
     courier_rb.Check(    family == FAMILY_MODERN ); 

     InitializeListBox( font );
}




void WindowAttributesDlg::InitializeListBox( const Font& rFont )
{
     String old_size = fontsize_lb.GetSelectEntry( );

     fontsize_lb.ChangeUpdateMode( FALSE );
     fontsize_lb.Clear( );

     INT16 height = GetDevFontSize ( rFont, 0 ) .Height( );

     if ( ! height )
     {
          for ( int i = 1; i < 128; i++ )
               fontsize_lb.InsertEntry( i );
     }
     else
     {
          USHORT n = GetDevFontSizeCount ( rFont );

          for ( UINT16 i = 0; i < n; i++ )
               fontsize_lb.InsertEntry( GetDevFontSize( rFont, i ) .Height( ) );
     }

     fontsize_lb.SelectEntry( String( rFont.GetSize().Height() ) );
     fontsize_lb.ChangeUpdateMode( TRUE );
     fontsize_lb.Invalidate( );
}








// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::CloseHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::CloseHandler( OKButton* )
{
     if ( modified )
     {
         GetParent()->ChangeFont( sample_text_window.GetFont() );

          if ( black_bkgrd_rb.IsChecked( ) )
               ((DisplayWindow*)GetParent())->SetColorScheme( COL_SCHEME_BLACK );

          if ( cyan_bkgrd_rb.IsChecked( ) )
               ((DisplayWindow*)GetParent())->SetColorScheme( COL_SCHEME_CYAN );

          if ( gray_bkgrd_rb.IsChecked( ) )
               ((DisplayWindow*)GetParent())->SetColorScheme( COL_SCHEME_GRAY );

          if ( white_bkgrd_rb.IsChecked( ) )
               ((DisplayWindow*)GetParent())->SetColorScheme( COL_SCHEME_WHITE );

          EndDialog( TRUE );
     }
     else
          EndDialog( FALSE );
}








// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::ChangeBackgroundColor()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::ChangeBackgroundColor()
{
     Brush brush = sample_text_window.GetBackgroundBrush();
     brush.ChangeColor( Color( ColorScheme[ current_color ].GetBackgroundColor() ) );
     sample_text_window.ChangeBackgroundBrush( brush );
}




// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::RadioButtonClickHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::RadioButtonClickHandler( RadioButton* pBtn )
{
     if ( !modified )
          SetModifiedFlag();

     if ( pBtn == &black_bkgrd_rb )
          current_color = COL_SCHEME_BLACK;

     if ( pBtn == &cyan_bkgrd_rb )
          current_color = COL_SCHEME_CYAN;

     if ( pBtn == &gray_bkgrd_rb )
          current_color = COL_SCHEME_GRAY;

     if ( pBtn == &white_bkgrd_rb )
          current_color = COL_SCHEME_WHITE;

     ChangeBackgroundColor();

     Font font = sample_text_window.GetFont();
     font.ChangeColor( Color( ColorScheme[ current_color ].GetTextColor() ) );
     SetFont( font );

}






// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::SelectSizeHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::SelectSizeHandler( ListBox* pListBox )
{
     // get newly selected size 

     USHORT point_size = pListBox->GetSelectEntry( );


     // modify current font 

     Font temp_font = sample_text_window.GetFont();

     temp_font.ChangeSize( Size( 0, point_size ) );

     SetFont( temp_font );

     SetModifiedFlag();
}








// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::SelectFontHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::SelectFontHandler( RadioButton* pBtn )
{
     enum FontFamily family; 
     
     // what was selected ?

     if ( pBtn == &timesroman_rb )
        family = FAMILY_ROMAN;
     else
     {
          if ( pBtn == &helvetica_rb )
               family = FAMILY_SWISS;
          else
		{
               if ( pBtn == &courier_rb )
                    family = FAMILY_MODERN;
               else
                    ASSERT( 0 );
          }
     }

     USHORT point_size = fontsize_lb.GetSelectEntry( );

     Font new_font = Font( family, Size( 0, point_size ) );

     // modify current font 

     SetFont( new_font );

     SetModifiedFlag();

     InitializeListBox( new_font );
}





// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::SetFont()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::SetFont( const Font& rFont )
{
     // resolve logical font to physical font - needed for correct plot layout

     Font temp_font( rFont );

     FontMetric ametric = GetFontMetric( temp_font );
     temp_font =  ametric.GetPhysicalFont( );
     temp_font.ChangeColor( Color( ColorScheme[ current_color ].GetTextColor() ) );
     temp_font.ChangeTransparent( TRUE );

     sample_text_window.ChangeFont( temp_font );
     sample_text_window.Invalidate();
}




// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::UndoHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::UndoHandler( PushButton* )
{
     modified = FALSE;
     InitializeDialog();
     undo_btn.Disable();
}




// ----------------------------------------------------------------------------
//                 WindowAttributesDlg::SetModifiedFlag()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void WindowAttributesDlg::SetModifiedFlag( )
{
     modified = TRUE;
     undo_btn.Enable();
}



BOOL8 EditWindowAttributes( DisplayWindow* pWindow )
{
     WindowAttributesDlg* pdlg = new WindowAttributesDlg( pWindow );

          if ( pdlg )
     {
          BOOL8 rc = pdlg->Execute();
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FALSE;
     }
}


