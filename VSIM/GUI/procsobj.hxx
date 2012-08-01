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
//  PROCSOBJ.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the ProcessObject class for
//  the VSIM user interface. This is the base class from which 
//  the Compartment and TransferPath class are derived. 
//  It includes info about drawing the object on the screen 
//
//  CREATED : 1.12.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------


#if !defined(__PROCSOBJ_HXX__)
#define __PROCSOBJ_HXX__



const enum ColorName  PO_NORMAL_BG_FILL_COLOR = COL_WHITE;


// PO_NORMAL = the normal state ( solid white fill )
// PO_SELECT = the state where the Object has been selected ( select brush fill )
// PO_OPEN   = the state where the Object has been open     ( diag black fill )

enum ProcObjState { PO_NORMAL, PO_OPEN, PO_SELECT };
enum ProcObjType  { PO_TYPE_NONE, PO_TYPE_COMPT, PO_TYPE_XFER, PO_TYPE_WRAPPED_XFER };

enum ProcObjRC { PO_RC_FAIL, PO_RC_CANCEL, PO_RC_ACCEPT };

class SchemeWindow;
class OutputDevice;
class SpeciesDatabase;




// ----------------------------------------------------------------------------
//                         class ProcessObject( )             
// ............................................................................
// 
//
// ----------------------------------------------------------------------------

class ProcessObject
{
protected:
     String         title;             // the name of the object
     ProcObjState   state;          // it's state
     BOOL           highlight;
     BOOL           is_modified;
     Rectangle      active_area;       // defines the region on the windo sensitive to mouse clicks
     SchemeWindow*  pparent;
     OutputDevice*  poutput_device;
     Font           std_font;
     Font           highlight_font;
     Brush          std_fill;
     Brush          select_fill;
     Brush          open_fill;
     short          char_width;
     short          char_height;


     void                SelectBrush();
     void                DrawTitle( );
     void                SetModified( BOOL NewState ) { is_modified = NewState; };


public:
                         // declared but not defined - prevents inadvertent use  under the covers
                         ProcessObject( );       

                         // this one IS defined and used 
                         ProcessObject( const String& rName, SchemeWindow* pParent );

                         // copy constructor 
                         ProcessObject( const ProcessObject& rObject );


     virtual            ~ProcessObject( );       

     ProcessObject&     operator = ( const ProcessObject& rObject );


     void                Highlight( BOOL NewState );

     SchemeWindow*       GetpParent( ) const { return pparent; }
     virtual void        SetpParent(  SchemeWindow* pParent );

     OutputDevice*       GetpOutputDevice( ) const { return poutput_device; }
     virtual void        SetpOutputDevice(  OutputDevice* pDevice ) { poutput_device = pDevice; }

     ProcObjState        ChangeState( ProcObjState NewState );

     virtual void        DrawObject( ) = 0;
     virtual void        DrawActiveArea( ) = 0;
     virtual void        SynchSpeciesListWithDatabase( SpeciesDatabase& rSDB ) = 0;
     virtual UINT16      GetIndex( ) = 0;

      ProcObjState       GetState( ) const { return state; };

     BOOL                ObjectHit( const Point& rPoint )  const 
                              { return active_area.IsInside( rPoint ); };

     void                SetActiveArea( Rectangle& rNewRect ) 
                              { active_area = rNewRect; };

     Rectangle           GetActiveArea( ) const { return active_area; };

     Color               ChangeObjectColor( const Color& rNewColor) { return std_fill.ChangeColor( rNewColor ); }
     Color               GetObjectColor( ) const { return std_fill.GetColor( ); }

     virtual ProcObjType GetType( ) const = 0;

     BOOL                IsModified( ) const { return is_modified; };
     virtual ProcObjRC   EditContents ( ) = 0;
     virtual BOOL        IsEmpty ( ) const = 0;
     const String&       GetName( ) const  { return title; };
     void                SetName( const String& rNewName ) { title = rNewName; };

     virtual BOOL8       IsVisibleWithin( const Rectangle& rRect ) = 0;
};


#endif
