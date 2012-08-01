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
//  CREATE3D.CXX - a module for the VSIM User interface 
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
#include "create3d.hxx"

#endif



#include "create3d.hrc"
#include "strings.hrc"



const UINT16  INIT_NUM_X    = 5;
const UINT16  INIT_NUM_Y    = 5;
const UINT16  INIT_NUM_Z    = 1;



// ----------------------------------------------------------------------------
//                         class SchematicWindow 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class Schematic3DWindow : public Window
{
protected :

     Rectangle  frame_rect;
     FLOAT64    frame_x_flt;
     FLOAT64    frame_y_flt;
     FLOAT64    frame_z_flt;
     FLOAT64    x_interval;
     FLOAT64    y_interval;
     FLOAT64    z_interval;
     FLOAT64    actual_length_x;
     FLOAT64    actual_length_y;
     FLOAT64    actual_length_z;
     UINT16     num_x_divns;
     UINT16     num_y_divns;
     UINT16     num_z_divns;

public :

     Schematic3DWindow( Window* pParent, const ResId& rResId );

     virtual void Paint( const Rectangle& );

     void SetNumXDivisions( UINT16 NewValue );
     void SetNumYDivisions( UINT16 NewValue );
     void SetNumZDivisions( UINT16 NewValue );

     void SetXDimension( FLOAT64 NewValue ) { actual_length_x = NewValue; }
     void SetYDimension( FLOAT64 NewValue ) { actual_length_y = NewValue; } 
     void SetZDimension( FLOAT64 NewValue ) { actual_length_z = NewValue; } 
};







// ----------------------------------------------------------------------------
//                    Schematic3DWindow::SetNumXDivisions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic3DWindow::SetNumXDivisions( UINT16 NewValue ) 
{ 
     num_x_divns = NewValue; 
     x_interval = frame_x_flt / (FLOAT64) num_x_divns;
}





// ----------------------------------------------------------------------------
//                    Schematic3DWindow::SetNumYDivisions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic3DWindow::SetNumYDivisions( UINT16 NewValue ) 
{ 
     num_y_divns = NewValue; 
     y_interval = frame_y_flt / (FLOAT64) num_y_divns;
}






// ----------------------------------------------------------------------------
//                    Schematic3DWindow::SetNumZDivisions()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic3DWindow::SetNumZDivisions( UINT16 NewValue ) 
{ 
     num_z_divns = NewValue; 
     z_interval = frame_z_flt / (FLOAT64) num_z_divns;
}







// ----------------------------------------------------------------------------
//                         Schematic3DWindow::Schematic3DWindow()
// ............................................................................
//
//   constructor
//
// ----------------------------------------------------------------------------

Schematic3DWindow::Schematic3DWindow( Window* pParent, const ResId& rResId ) :
Window ( pParent, rResId ),
frame_rect(),
frame_x_flt( 0.0 ),
frame_y_flt( 0.0 ),
frame_z_flt( 0.0 ),
x_interval(0.0),
y_interval(0.0),
z_interval(0.0),
actual_length_x(0.0),
actual_length_y(0.0), 
actual_length_z(0.0), 
num_x_divns(1), 
num_y_divns(1), 
num_z_divns(1)
{
     Size window_size = GetOutputSize();

     Point origin = Point( ( 10 * window_size.Width() ) / 200, (10 * window_size.Height() ) / 200 );
     Size frame_size = Size( ( 9 * window_size.Width()) / 10, (9 * window_size.Height()) / 10 );

     frame_y_flt = (FLOAT64) frame_size.Height();
     frame_x_flt  = (FLOAT64) frame_size.Width();

     frame_rect = Rectangle( origin, frame_size );
}





// ----------------------------------------------------------------------------
//                         Schematic2DWindow::Paint()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Schematic3DWindow::Paint( const Rectangle& )
{
     DrawRect( frame_rect );

     // draw rows first 

     Point start_pt;
     Point end_pt;
     Point step;

     for ( UINT16 i = 1 ; i < num_y_divns; i++ )
     {
          step = Point( 0, Round( i * y_interval ) ); 
          start_pt = frame_rect.TopLeft() + step;
          end_pt   = frame_rect.TopRight() + step;

          DrawLine( start_pt, end_pt );
     }

     for ( i = 1 ; i < num_x_divns; i++ )
     {
          step = Point(  Round( i * x_interval ), 0 );
          start_pt = frame_rect.TopLeft() + step;
          end_pt   = frame_rect.BottomLeft() + step;

          DrawLine( start_pt, end_pt );
     }
}








// ----------------------------------------------------------------------------
//                         class Create3DArrayDialog 
// ............................................................................
//
//
// ----------------------------------------------------------------------------

class Create3DArrayDialog : public ModalDialog
{

protected:

     OKButton        ok_btn;
     CancelButton    cancel_btn;
     HelpButton      help_btn;
     PushButton      defaults_btn;
     Schematic3DWindow schematic_window;

     CheckBox        vert_wrap_cb;
     CheckBox        horiz_wrap_cb;
     CheckBox        depth_wrap_cb;

     FixedText       fixed_text1;
     FixedText       fixed_text2;
     FixedText       fixed_text3;
     Edit            array_x_length;
     Edit            array_y_length;
     Edit            array_z_length;
     FixedText       length_units_text;
	ListBox         units_selection_lb;

     GroupBox        group_box2;
     GroupBox        group_box4;

     FixedText       fixed_text4;
     FixedText       fixed_text5;
     FixedText       fixed_text6;
     Edit            array_x_number;
     Edit            array_y_number;
     Edit            array_z_number;

     GroupBox        group_box3;

     FixedText       fixed_text_statistics1;
     FixedText       fixed_text9;
     FixedText       fixed_text_cell_stats_x;
     FixedText       fixed_text_cell_stats_y;
     FixedText       fixed_text_cell_stats_z;

     GroupBox        group_box1;

     String          length_units;

     UINT16         num_compts_x;
     UINT16         num_compts_y;
     UINT16         num_compts_z;

     UINT32         num_compartments;
     UINT32         num_transfer_paths;

     FLOAT64        array_x_length_flt;
     FLOAT64        array_y_length_flt;
     FLOAT64        array_z_length_flt;

     FLOAT64        cell_x;
     FLOAT64        cell_y;
     FLOAT64        cell_z;

     BOOL8          x_wrap;
     BOOL8          y_wrap;
     BOOL8          z_wrap;

     void           RecalcArrayData();
     void           InitializeDlg();
     BOOL8          ValidInput();

     void           DimensionInputError( const String& rEntry, const String& rId );
     void           NumberInputError( const String& rEntry, const String& rId );


public:

     Create3DArrayDialog( Window * pParent );

     void LengthModifiedHandler( Edit* pEdit );
     void NumberModifiedHandler( Edit* pEdit );

     void DefaultsBtnHandler( PushButton* );
     void OKBtnHandler( OKButton* );
     void CancelBtnHandler( CancelButton* );
     void SelectUnitsHandler( ListBox* );

     UINT16 GetNumComptsX() const { return num_compts_x; }
     UINT16 GetNumComptsY() const { return num_compts_y; }
     UINT16 GetNumComptsZ() const { return num_compts_z; }

     FLOAT64 GetCellXDimension( ) const { return cell_x; }
     FLOAT64 GetCellYDimension( ) const { return cell_y; }
     FLOAT64 GetCellZDimension( ) const { return cell_z; }

     BOOL8   IsXWrapRequested( ) const { return x_wrap; }
     BOOL8   IsYWrapRequested( ) const { return y_wrap; }
     BOOL8   IsZWrapRequested( ) const { return z_wrap; }
};








// ----------------------------------------------------------------------------
//                 Create3DArrayDialog::ValidInput()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

BOOL8 Create3DArrayDialog::ValidInput()
{
     NumericString test_str;

     test_str = array_x_length.GetText();

     if (FALSE == test_str.IsValidPositiveFLOAT64() )
     {
          DimensionInputError( fixed_text1.GetText(), test_str );
          return FALSE;
     }

     test_str = array_y_length.GetText();

     if (FALSE == test_str.IsValidPositiveFLOAT64() )
     {
          DimensionInputError( fixed_text2.GetText(), test_str );
          return FALSE;
     }

     test_str = array_z_length.GetText();

     if (FALSE == test_str.IsValidPositiveFLOAT64() )
     {
          DimensionInputError( fixed_text3.GetText(), test_str );
          return FALSE;
     }


     UINT16 testval; 

     test_str = array_x_number.GetText();

     if (FALSE == test_str.IsValidUINT16( &testval) || 0 == testval )
     {
          NumberInputError( fixed_text4.GetText(), test_str );
          return FALSE;
     }

     test_str = array_y_number.GetText();

     if (FALSE == test_str.IsValidUINT16( &testval) || 0 == testval )
     {
          NumberInputError( fixed_text5.GetText(), test_str );
          return FALSE;
     }

     test_str = array_z_number.GetText();

     if (FALSE == test_str.IsValidUINT16( &testval) || 0 == testval )
     {
          NumberInputError( fixed_text6.GetText(), test_str );
          return FALSE;
     }

     x_wrap = horiz_wrap_cb.IsChecked();
     y_wrap = vert_wrap_cb.IsChecked();

     return TRUE;
}





// ----------------------------------------------------------------------------
//                 Create3DArrayDialog::NumberInputError()
// ............................................................................
//
//
// ----------------------------------------------------------------------------
void Create3DArrayDialog::NumberInputError( const String& rId, const String& rEntry )
{
     String message = String( ResId( vID_STR_CREATE_3D_INPUT_ERR1 ) ) + rEntry + 
      String( ResId( vID_STR_CREATE_3D_INPUT_ERR2 ) ) + rId +
      String( ResId( vID_STR_CREATE_3D_INPUT_ERR3 ) );

     WarningBox( GetParent(), WB_OK | WB_DEF_OK, message ).Execute( );
}




// ----------------------------------------------------------------------------
//                 Create3DArrayDialog::DimensionInputError()
// ............................................................................
//
//
// ----------------------------------------------------------------------------
void Create3DArrayDialog::DimensionInputError( const String& rId, const String& rEntry )
{
     String message = String( ResId( vID_STR_CREATE_3D_INPUT_ERR4 ) ) + rEntry + 
      String( ResId( vID_STR_CREATE_3D_INPUT_ERR5 ) ) + rId +
      String( ResId( vID_STR_CREATE_3D_INPUT_ERR6 ) );

     WarningBox( GetParent(), WB_OK | WB_DEF_OK, message ).Execute( );
}


// ----------------------------------------------------------------------------
//                 Create2DArrayDialog::Create2DArrayDialog()
// ............................................................................
//
//  constructor 
//
// ----------------------------------------------------------------------------

Create3DArrayDialog::Create3DArrayDialog( Window * pParent ) :
ModalDialog( pParent, ResId(vID_DLG_CREATE_3D_ARRAY )  ),
ok_btn( this, ResId( vID_OK_BTN ) ),
cancel_btn( this, ResId( vID_CANCEL_BTN ) ),
help_btn( this, ResId( vID_HELP_BTN ) ),
defaults_btn( this, ResId( vID_DEFAULTS_BTN ) ),
schematic_window( this, ResId( vID_WINDOW_SCHEMATIC ) ),
vert_wrap_cb( this, ResId( vID_VERTICAL_WRAP_CB ) ), 
horiz_wrap_cb( this, ResId( vID_HORIZONTAL_WRAP_CB ) ), 
depth_wrap_cb( this, ResId( vID_DEPTH_WRAP_CB ) ),  
fixed_text1( this, ResId( 1 ) ),
fixed_text2( this, ResId( 2 ) ),
fixed_text3( this, ResId( 3 ) ),
array_x_length( this, ResId( vID_EDIT_ARRAY_X_LENGTH ) ), 
array_y_length( this, ResId( vID_EDIT_ARRAY_Y_LENGTH ) ),  
array_z_length( this, ResId( vID_EDIT_ARRAY_Z_LENGTH ) ),  
length_units_text( this, ResId( vID_STEXT_LENGTH_UNITS ) ),
units_selection_lb( this, ResId( vID_LB_LENGTH_UNITS_SELECTION ) ),
group_box2( this, ResId( 2 ) ), 
fixed_text4( this, ResId( 4 ) ),
fixed_text5( this, ResId( 5 ) ),
fixed_text6( this, ResId( 6 ) ),
array_x_number( this, ResId( vID_EDIT_ARRAY_X_NUMBER ) ), 
array_y_number( this, ResId( vID_EDIT_ARRAY_Y_NUMBER ) ), 
array_z_number( this, ResId( vID_EDIT_ARRAY_Z_NUMBER ) ), 
group_box3( this, ResId( 3 ) ),
fixed_text_statistics1( this, ResId( vID_STEXT_STATISTICS_1 ) ), 
fixed_text9( this, ResId( 9 ) ), 
fixed_text_cell_stats_x( this, ResId( vID_STEXT_STATISTICS_CELL_X ) ),
fixed_text_cell_stats_y( this, ResId( vID_STEXT_STATISTICS_CELL_Y ) ), 
fixed_text_cell_stats_z( this, ResId( vID_STEXT_STATISTICS_CELL_Z ) ), 
group_box1( this, ResId( 1 ) ),
group_box4( this, ResId( 4 ) ), 
num_compts_x(INIT_NUM_X),
num_compts_y(INIT_NUM_Y),
num_compts_z(INIT_NUM_Z),
num_compartments(0),
num_transfer_paths(0),
array_x_length_flt(0.0),
array_y_length_flt(0.0),
array_z_length_flt(0.0),
cell_x(0.0),
cell_y(0.0),
cell_z(0.0),
x_wrap( FALSE ),
y_wrap( FALSE ),
z_wrap( FALSE )
{
     FreeResource();

     ok_btn.ChangeClickHdl( LINK( this, Create3DArrayDialog, OKBtnHandler ) );
     cancel_btn.ChangeClickHdl( LINK( this, Create3DArrayDialog, CancelBtnHandler ) );
     defaults_btn.ChangeClickHdl( LINK( this, Create3DArrayDialog, DefaultsBtnHandler ) );

     array_x_length.ChangeModifyHdl( LINK( this, Create3DArrayDialog, LengthModifiedHandler ) );  
     array_y_length.ChangeModifyHdl( LINK( this, Create3DArrayDialog, LengthModifiedHandler ) );  
     array_z_length.ChangeModifyHdl( LINK( this, Create3DArrayDialog, LengthModifiedHandler ) );  

     array_x_number.ChangeModifyHdl( LINK( this, Create3DArrayDialog, NumberModifiedHandler ) );  
     array_y_number.ChangeModifyHdl( LINK( this, Create3DArrayDialog, NumberModifiedHandler ) );  
     array_z_number.ChangeModifyHdl( LINK( this, Create3DArrayDialog, NumberModifiedHandler ) );  

     units_selection_lb.ChangeSelectHdl( LINK( this, Create3DArrayDialog, SelectUnitsHandler ) );   
     InitializeDlg();

     GetpMainApp()->CenterWindowOnParent( this, pParent );
}




// ----------------------------------------------------------------------------
//                     Create3DArrayDialog::SelectUnitsHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::SelectUnitsHandler( ListBox* )
{
     length_units = String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) + units_selection_lb.GetSelectEntry();
     RecalcArrayData();
}




// ----------------------------------------------------------------------------
//                     Create2DArrayDialog::InitializeDlg()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::InitializeDlg( )
{
     length_units = units_selection_lb.GetSelectEntry();

     array_x_length.SetText( String( ResId( vID_STR_DEFAULT_X_LENGTH ) ) );  
     array_y_length.SetText( String( ResId( vID_STR_DEFAULT_Y_LENGTH ) ) );  
     array_z_length.SetText( String( ResId( vID_STR_DEFAULT_Z_LENGTH ) ) );  

     array_x_number.SetText( String( num_compts_x ) );    
     array_y_number.SetText( String( num_compts_y ) );
     array_z_number.SetText( String( num_compts_z ) );

     schematic_window.SetNumXDivisions( num_compts_x );
     schematic_window.SetNumYDivisions( num_compts_y );     
     schematic_window.SetNumZDivisions( num_compts_z );     

     vert_wrap_cb.Check( FALSE );
     horiz_wrap_cb.Check( FALSE ); 

     SelectUnitsHandler( &units_selection_lb );
}





// ----------------------------------------------------------------------------
//                 Create3DArrayDialog::LengthModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::LengthModifiedHandler( Edit* pEdit )
{
     NumericString text = pEdit->GetText();
     FLOAT64  value;

     if (text.IsValidPositiveFLOAT64( &value ) )
     {
          if ( pEdit == &array_x_length )
          {
               array_x_length_flt = value;
               schematic_window.SetXDimension( value );
          }

          if ( pEdit == &array_y_length )
          {
               array_y_length_flt = value;
               schematic_window.SetYDimension( value );
          }

          if ( pEdit == &array_z_length )
          {
               array_z_length_flt = value;
               schematic_window.SetZDimension( value );
          }

          schematic_window.Invalidate();
          RecalcArrayData();
     }
}







// ----------------------------------------------------------------------------
//                Create3DArrayDialog::NumberModifiedHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::NumberModifiedHandler( Edit* pEdit )
{
     NumericString text = pEdit->GetText();
     UINT16 value;

     if (text.IsValidUINT16( &value ) )
     {
          if ( pEdit == &array_x_number )
          {
               num_compts_x = value;
               schematic_window.SetNumXDivisions( value );
          }

          if ( pEdit == &array_y_number )
          {
               num_compts_y = value;
               schematic_window.SetNumYDivisions( value );
          }

          if ( pEdit == &array_z_number )
          {
               num_compts_z = value;
               schematic_window.SetNumZDivisions( value );
          }


          schematic_window.Invalidate();
          RecalcArrayData();
     }
     
}






// ----------------------------------------------------------------------------
//                     Create3DArrayDialog::RecalcArrayData()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::RecalcArrayData( )
{    
     cell_x  = ( (FLOAT64) NumericString( array_x_length.GetText() ) ) / num_compts_x;
     cell_y  = ( (FLOAT64) NumericString( array_y_length.GetText() ) ) / num_compts_y;
     cell_z  = ( (FLOAT64) NumericString( array_z_length.GetText() ) ) / num_compts_z;

     num_compartments   = num_compts_x * num_compts_y * num_compts_z;

     UINT32 num_xfer_paths_per_z_layer = ( num_compts_x * (num_compts_y-1)) + ( num_compts_y * (num_compts_x-1) );

     num_transfer_paths = ( num_compts_z * num_xfer_paths_per_z_layer ) + 
      ( (num_compts_z-1) * (num_compts_x * num_compts_y ) );

     fixed_text_statistics1.SetText( 
      String( ResId( vID_STR_3D_STAT1_1 ) ) +
      String( num_compartments ) + 
      String( ResId( vID_STR_3D_STAT1_2 ) )+
      String( num_transfer_paths ) +
      String( ResId( vID_STR_3D_STAT1_3 ) ) );


     fixed_text_cell_stats_x.SetText( 
      String( ResId( vID_STR_3D_STATX ) ) +
      NumericString( cell_x ) + length_units );

     fixed_text_cell_stats_y.SetText( 
      String( ResId( vID_STR_3D_STATY ) ) +
      NumericString( cell_y ) + length_units ); 

     fixed_text_cell_stats_z.SetText( 
      String( ResId( vID_STR_3D_STATZ ) ) +
      NumericString( cell_z ) + length_units ); 

}







// ----------------------------------------------------------------------------
//                     Create3DArrayDialog::DefaultsBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::DefaultsBtnHandler( PushButton* )
{
     num_compts_x = INIT_NUM_X;
     num_compts_y = INIT_NUM_Y;
     num_compts_z = INIT_NUM_Z;

     InitializeDlg();
     schematic_window.Invalidate();
}






// ----------------------------------------------------------------------------
//                     Create3DArrayDialog::OKBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::OKBtnHandler( OKButton* )
{
     if ( ValidInput() )
          EndDialog( C3D_RC_ACCEPT );
}





// ----------------------------------------------------------------------------
//                     Create3DArrayDialog::CancelBtnHandler()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

void Create3DArrayDialog::CancelBtnHandler( CancelButton* )
{
     EndDialog( C3D_RC_CANCEL );
}




// ----------------------------------------------------------------------------
//                     Create3DComptScheme()
// ............................................................................
//
//
// ----------------------------------------------------------------------------

enum Create3D_RC Create3DComptScheme( Window* pParent, SchemeFrameWindow* pSchemeFrameWin )
{
     Create3DArrayDialog* pdlg = new Create3DArrayDialog( pParent );

     if ( pdlg )
     {
           enum Create3D_RC rc = (Create3D_RC) pdlg->Execute();

           if ( C3D_RC_ACCEPT == rc )
           {
                if ( FALSE == ( (ThreeDArraySchemeWindow*) (pSchemeFrameWin->GetpSchemeWindow()))->Add3DArray( 
                 pdlg->GetNumComptsX(), pdlg->GetNumComptsY(), pdlg->GetNumComptsZ(),
                 pdlg->GetCellXDimension(), pdlg->GetCellYDimension(), pdlg->GetCellZDimension(),
                 pdlg->IsXWrapRequested( ), 
                 pdlg->IsYWrapRequested( ),
                 pdlg->IsZWrapRequested( )
                 ) )
                 {
                     GetpMainApp()->OutOfMemory();
                 }
           }

           delete pdlg;

           return rc;
     }

     return C3D_RC_FAIL;
}

