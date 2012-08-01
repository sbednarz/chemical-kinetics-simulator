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
//  FILQUERY.CXX - a module of the VSIM User interface
// ............................................................................
//
//  This module defines the InvokeFillQueryDialog function
//
//  CREATED : 10.23.96
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if defined(_USE_MERGED_HEADER_)

#include "vsim.hxx"
#pragma hdrstop

#else

#include <sv.hxx>

#include "filquery.hxx"
#include "mainapp.hxx"

#endif


#include "filquery.hrc"












// ----------------------------------------------------------------------------
//                               class FillQueryDialog
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

class FillQueryDialog : public ModalDialog
{
protected:
    PushButton    use_clip_dims_btn;
    PushButton    use_target_dims_btn;
    CancelButton  cancel_btn;
    HelpButton    help_btn;
    FixedText     text1;
    FixedText     text_x_dim;
    FixedText     text_y_dim;
    FixedText     text_z_dim;
    FixedText     text5;

public:
                  FillQueryDialog( SchemeFrameWindow * pParent, 
                                   const CompartmentObject* const pClipObject );


     void         CancelHandler( CancelButton * );
     void         UseTargetDimsHandler( PushButton * );
     void         UseClippedDimsHandler( PushButton * );

};







// ----------------------------------------------------------------------------
//                         FillQueryDialog::FillQueryDialog( )
// ............................................................................
//
//
//   constructor
//
// ----------------------------------------------------------------------------

FillQueryDialog::FillQueryDialog( SchemeFrameWindow * pParent, const CompartmentObject* const pClipObject ) :
ModalDialog( pParent, ResId( vID_FILL_QUERY_DLG ) ),
use_clip_dims_btn( this, ResId( vID_BTN_USE_CLIP_DIMS ) ),
use_target_dims_btn( this, ResId( vID_BTN_USE_TARGET_DIMS ) ),
cancel_btn( this, ResId( vID_BTN_CANCEL ) ),
help_btn( this, ResId( vID_BTN_HELP ) ),
text1( this, ResId( 1 ) ),
text_x_dim( this, ResId( vID_STEXT_X ) ),
text_y_dim( this, ResId( vID_STEXT_Y ) ),
text_z_dim( this, ResId( vID_STEXT_Z ) ),
text5( this, ResId( 5 ) )
{
    FreeResource();

     use_clip_dims_btn.ChangeClickHdl( LINK( this, FillQueryDialog, UseClippedDimsHandler ) );
     use_target_dims_btn.ChangeClickHdl( LINK( this, FillQueryDialog, UseTargetDimsHandler ) );
     cancel_btn.ChangeClickHdl( LINK( this, FillQueryDialog, CancelHandler ) );

     
     String length_units = String( ResId( vID_STR_SYMBOL_BLANK_SPACE ) ) +
                           pParent->GetSchemeWindow().GetScheme().GetDistanceUnits(); 

     text_x_dim.SetText( 
      String( ResId( vID_STR_3D_STATX ) ) +
      NumericString( pClipObject->GetXDimension()  ) +
      length_units );

     text_y_dim.SetText( 
      String( ResId( vID_STR_3D_STATY ) ) +
      NumericString( pClipObject->GetYDimension() ) +
      length_units );


     text_z_dim.SetText( 
      String( ResId( vID_STR_3D_STATZ ) ) +
      NumericString( pClipObject->GetZDimension() ) +
      length_units );


     GetpMainApp()->CenterWindowOnParent( this, GetParent() );
}






// ----------------------------------------------------------------------------
//                  FillQueryDialog::CancelHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void FillQueryDialog::CancelHandler( CancelButton * )
{
          EndDialog(  FO_CANCEL );
}





// ----------------------------------------------------------------------------
//                  FillQueryDialog::UseTargetDimsHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void FillQueryDialog::UseTargetDimsHandler( PushButton * )
{
          EndDialog(  FO_USE_EXISTING_CONTENTS );
}





// ----------------------------------------------------------------------------
//                  FillQueryDialog::UseClippedDimsHandler()
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

void FillQueryDialog::UseClippedDimsHandler( PushButton * )
{
          EndDialog(  FO_USE_CLIP_CONTENTS );
}





// ----------------------------------------------------------------------------
//                               InvokeFillQueryDialog( )
// ............................................................................
//
//
//
// ----------------------------------------------------------------------------

enum FILL_OPERATION InvokeFillQueryDialog( SchemeFrameWindow* pParent, const CompartmentObject* const pClipObject  )
{
     FillQueryDialog* pdlg = new FillQueryDialog( pParent, pClipObject );

     if ( pdlg )
     {
          enum FILL_OPERATION rc = ( enum FILL_OPERATION ) pdlg->Execute();
          delete pdlg;
          return rc;
     }
     else
     {
          GetpMainApp()->OutOfMemory();
          return FO_FAIL;
     }
}




