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
//  CREATE2D.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This module defines the Create2DComptScheme function 
//  used to create a new reaction scheme containing a two-dimensional
//  array of compartments 
//
//  CREATED : 6.11.96
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------

#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "math.hxx"
#include "numstrng.hxx"
#include "mainapp.hxx"
#include "create2d.hxx"

#endif



#include "create2d.hrc"
#include "strings.hrc"

const UINT16  INIT_NUM_ROWS    = 5;
const UINT16  INIT_NUM_COLUMNS = 5;



// ----------------------------------------------------------------------------
//                         class SchematicWindow 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class Schematic2DWindow : public Window
{
protected :

     Rectangle  frame_rect;
     FLOAT64    frame_width_flt;
     FLOAT64    frame_height_flt;
     FLOAT64    vertical_interval;
     FLOAT64    horizontal_interval;
     UINT16     num_rows;
     UINT16     num_columns;

public :

     Schematic2DWindow( Window* pParent, const ResId& rResId );

     virtual void Paint( const Rectangle& );

     void SetNumRows( UINT16 NewValue );
     void SetNumColumns( UINT16 NewValue );
};






// ----------------------------------------------------------------------------
//                         Schematic2DWindow::SetNumRows()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic2DWindow::SetNumRows( UINT16 NewValue ) 
{ 
     num_rows = NewValue; 
     vertical_interval = frame_height_flt / (FLOAT64) num_rows;
}





// ----------------------------------------------------------------------------
//                         Schematic2DWindow::SetNumColumns()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic2DWindow::SetNumColumns( UINT16 NewValue ) 
{ 
     num_columns = NewValue; 
     horizontal_interval = frame_width_flt / (FLOAT64) num_columns;
}





// ----------------------------------------------------------------------------
//                         Schematic2DWindow::Schematic2DWindow()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

Schematic2DWindow::Schematic2DWindow( Window* pParent, const ResId& rResId ) :
Window ( pParent, rResId ),
frame_rect(),
frame_width_flt( 0.0 ),
frame_height_flt( 0.0 ),
num_rows(1),
num_columns(1),
vertical_interval(0),
horizontal_interval(0)
{
     Size window_size = GetOutputSize();

     Point origin = Point( ( 10 * window_size.Width() ) / 200, (10 * window_size.Height() ) / 200 );
     Size frame_size = Size( ( 9 * window_size.Width()) / 10, (9 * window_size.Height()) / 10 );

     frame_height_flt = (FLOAT64) frame_size.Height();
     frame_width_flt  = (FLOAT64) frame_size.Width();

     frame_rect = Rectangle( origin, frame_size );
}





// ----------------------------------------------------------------------------
//                         Schematic2DWindow::Paint()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic2DWindow::Paint( const Rectangle& )
{
     DrawRect( frame_rect );

     // draw rows first 

     Point start_pt;
     Point end_pt;
     Point step;

     for ( UINT16 i = 1 ; i < num_rows; i++ )
     {
          step = Point( 0, Round( i * vertical_interval ) ); 
          start_pt = frame_rect.TopLeft() + step;
          end_pt   = frame_rect.TopRight() + step;

          DrawLine( start_pt, end_pt );
     }

     for ( i = 1 ; i < num_columns; i++ )
     {
          step = Point(  Round( i * horizontal_interval ), 0 );
          start_pt = frame_rect.TopLeft() + step;
          end_pt   = frame_rect.BottomLeft() + step;

          DrawLine( start_pt, end_pt );
     }
}





// ----------------------------------------------------------------------------
//                         class Create2DArrayDialog 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class Create2DArrayDialog : public ModalDialog
{

protected:

     OKButton        ok_btn;
     CancelButton    cancel_btn;
     HelpButton      help_btn;
     PushButton      defaults_btn;
     FixedText       fixed_text1;
     Schematic2DWindow schematic_window;
     Edit            height_length;
     FixedText       height_length_units_text;
     FixedText       fixed_text3;
     Edit            width_length;
     FixedText       width_length_units_text;
     FixedText       fixed_text5;
     Edit            width_number;
     FixedText       fixed_text6;
     Edit            height_number;
     FixedText       fixed_text_statistics1;
     FixedText       fixed_text_statistics2;
     GroupBox        group_box1;
     FixedText       fixed_text9;
     FixedText       fixed_text10;

     UINT16         num_rows;
     UINT16         num_columns;

     UINT32         num_compartments;
     UINT32         num_transfer_paths;

     FLOAT64        array_ht;
     FLOAT64        array_width;

     NumericString  cell_ht;
     NumericString  cell_width;

     void           RecalcArrayData();
     void           InitializeDlg();


public:

     Create2DArrayDialog( Window * pParent );

     void HtLengthModifiedHandler( Edit* );
     void WidthLengthModifiedHandler( Edit* );
     void WidthNumberModifiedHandler( Edit* );
     void HtNumberModifiedHandler( Edit* );

     void DefaultsBtnHandler( PushButton* );
     void OKBtnHandler( OKButton* );
     void CancelBtnHandler( CancelButton* );

     UINT16 GetNumRows() const { return num_rows; }
     UINT16 GetNumColumns() const { return num_columns; }

};





// ----------------------------------------------------------------------------
//                 Create2DArrayDialog::Create2DArrayDialog()
// ............................................................................
//
//  constructor 
//
// ----------------------------------------------------------------------------

Create2DArrayDialog::Create2DArrayDialog( Window * pParent ) :
ModalDialog( pParent, ResId(vID_DLG_CREATE_2D_ARRAY )  ),
ok_btn( this, ResId( vID_OK_BTN ) ),
cancel_btn( this, ResId( vID_CANCEL_BTN ) ),
help_btn( this, ResId( vID_HELP_BTN ) ),
defaults_btn( this, ResId( vID_DEFAULTS_BTN ) ),
fixed_text1( this, ResId( 1 ) ),
schematic_window( this, ResId( vID_WINDOW_SCHEMATIC ) ),
height_length( this, ResId( vID_EDIT_ARRAY_HEIGHT_LENGTH ) ),
height_length_units_text( this, ResId( vID_STEXT_HEIGHT_LENGTH_UNITS ) ),
fixed_text3( this, ResId( 3 ) ),
width_length( this, ResId( vID_EDIT_ARRAY_WIDTH_LENGTH ) ),
width_length_units_text( this, ResId( vID_STEXT_WIDTH_LENGTH_UNITS ) ),
fixed_text5( this, ResId( 5 ) ),
width_number( this, ResId( vID_EDIT_ARRAY_WIDTH_NUMBER ) ),
fixed_text6( this, ResId( 6 ) ),
height_number( this, ResId( vID_EDIT_ARRAY_HEIGHT_NUMBER ) ),
fixed_text_statistics2( this, ResId( vID_STEXT_STATISTICS_2 ) ),
group_box1( this, ResId( 1 ) ),
fixed_text_statistics1( this, ResId( vID_STEXT_STATISTICS_1 ) ),
fixed_text9( this, ResId( 9 ) ),
fixed_text10( this, ResId( 10 ) ),
num_rows( INIT_NUM_ROWS ),
num_columns( INIT_NUM_COLUMNS ),
num_compartments( 0 ),
num_transfer_paths( 0 ),
array_ht(),
array_width(),
cell_ht(),
cell_width()
{
     FreeResource();

     ok_btn.ChangeClickHdl( LINK( this, Create2DArrayDialog, OKBtnHandler ) );
     cancel_btn.ChangeClickHdl( LINK( this, Create2DArrayDialog, CancelBtnHandler ) );
     defaults_btn.ChangeClickHdl( LINK( this, Create2DArrayDialog, DefaultsBtnHandler ) );

     height_length.ChangeModifyHdl( LINK( this, Create2DArrayDialog, HtLengthModifiedHandler ) );  
     width_length.ChangeModifyHdl( LINK( this, Create2DArrayDialog, WidthLengthModifiedHandler ) );  
     height_number.ChangeModifyHdl( LINK( this, Create2DArrayDialog, HtNumberModifiedHandler ) );  
     width_number.ChangeModifyHdl( LINK( this, Create2DArrayDialog, WidthNumberModifiedHandler ) );  

     InitializeDlg();

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}





// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::InitializeDlg()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::InitializeDlg( )
{
     height_length.SetText( String( ResId( vID_STR_DEFAULT_HT_LENGTH ) ) );  
     width_length.SetText( String( ResId( vID_STR_DEFAULT_WIDTH_LENGTH ) ) );   
     height_number.SetText( String( num_rows ) );   
     width_number.SetText( String( num_columns ) );   

     schematic_window.SetNumRows( num_rows );
     schematic_window.SetNumColumns( num_columns );

     RecalcArrayData();
}





// ----------------------------------------------------------------------------
//                 Create2DArrayDialog::HtLengthModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::HtLengthModifiedHandler( Edit* )
{
     NumericString text = height_length.GetText();
     FLOAT64  value;

     if (text.IsValidPositiveFLOAT64( &value ) )
     {
          array_ht = value;
          RecalcArrayData();
     }
     else
          Sound::Beep();
}






// ----------------------------------------------------------------------------
//                 Create2DArrayDialog::WidthLengthModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::WidthLengthModifiedHandler( Edit* )
{
     NumericString text = width_length.GetText();
     FLOAT64  value;

     if (text.IsValidPositiveFLOAT64( &value ) )
     {
          array_width = value;
          RecalcArrayData();
     }
     else
          Sound::Beep();
}






// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::RecalcArrayData()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::RecalcArrayData( )
{
     FLOAT64 cell_ht_float    = ( (FLOAT64) NumericString( height_length.GetText() ) ) / num_rows;
     FLOAT64 cell_width_float = ( (FLOAT64) NumericString( width_length.GetText() ) ) / num_columns;

     cell_ht = NumericString( cell_ht_float );
     cell_width = NumericString( cell_width_float );
     
     num_compartments   = num_rows * num_columns;
     num_transfer_paths = ( num_rows * (num_columns-1)) + ( num_columns * (num_rows-1) );

     fixed_text_statistics1.SetText( 
      String( ResId( vID_STR_2D_STAT1_1 ) ) +
      String( num_compartments ) + 
      String( ResId( vID_STR_2D_STAT1_2 ) )+
      String( num_transfer_paths ) +
      String( ResId( vID_STR_2D_STAT1_3 ) ) );

     fixed_text_statistics2.SetText( 
      String( ResId( vID_STR_2D_STAT2_1 ) ) +
      cell_width + 
      String( ResId( vID_STR_2D_STAT2_2 ) ) +
      cell_ht + 
      String( ResId( vID_STR_2D_STAT2_3 ) ) );
}






// ----------------------------------------------------------------------------
//                Create2DArrayDialog::WidthNumberModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::WidthNumberModifiedHandler( Edit* )
{
     NumericString text = width_number.GetText();
     UINT16 value;

     if (text.IsValidUINT16( &value ) )
     {
          num_columns = value;
          schematic_window.SetNumColumns( value );
          schematic_window.Invalidate();
          RecalcArrayData();
     }
     else
          Sound::Beep();
     
}




// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::HtNumberModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::HtNumberModifiedHandler( Edit* )
{
     NumericString text = height_number.GetText();
     UINT16 value;

     if (text.IsValidUINT16( &value ) )
     {
          num_rows = value;
          schematic_window.SetNumRows( value );
          schematic_window.Invalidate();
          RecalcArrayData();
     }
     else
          Sound::Beep();
}




// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::DefaultsBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::DefaultsBtnHandler( PushButton* )
{
     num_rows    = INIT_NUM_ROWS;
     num_columns = INIT_NUM_COLUMNS;

     InitializeDlg();
     schematic_window.Invalidate();
}






// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::OKBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::OKBtnHandler( OKButton* )
{
          EndDialog( C2D_RC_ACCEPT );
}





// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::CancelBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create2DArrayDialog::CancelBtnHandler( CancelButton* )
{
     EndDialog( C2D_RC_CANCEL );
}




// ----------------------------------------------------------------------------
//                     Create2DComptScheme()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

enum Create2D_RC Create2DComptScheme( Window* pParent, SchemeFrameWindow* pSchemeFrameWin )
{
     Create2DArrayDialog* pdlg = new Create2DArrayDialog( pParent );

     if ( pdlg )
     {
           enum Create2D_RC rc = (Create2D_RC) pdlg->Execute();

           if ( C2D_RC_ACCEPT == rc )
           {
                if ( FALSE == pSchemeFrameWin->GetSchemeWindow().Add2DArray( pdlg->GetNumRows(), pdlg->GetNumColumns() ) )
                     GetpMainApp()->OutOfMemory();
           }

           delete pdlg;

           return rc;
     }

     return C2D_RC_FAIL;
}

