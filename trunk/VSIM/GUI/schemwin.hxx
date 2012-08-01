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
//  SCHEMWIN.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file descibes the SchemeWindow class, MDI window containing a view 
//  of a reaction scheme. This is the base class for the various varieties of 
//  SchemeWindows
//
//  CREATED : 1.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------



#if !defined(__SCHEMWIN_HXX__)
#define __SCHEMWIN_HXX__

#include "binstrem.hxx"
#include "rxnschem.hxx"

#include "scalwin.hxx"


class TextOutputStream;
class NotebookDialog;
class SchemeFrameWindow;


DECLARE_LIST( ResultsWindowList, Window* )
DECLARE_LIST( ProcessObjectList, ProcessObject* )


enum SCHEMWIN_OP { SW_NO_OP, SW_CMPTOBJ_ADD, SW_XFEROBJ_ADD, SW_COMPOBJ_MOVE, SW_RUBBER_BAND_SELECT };


enum SCHEMWIN_TYPE { SW_TYPE_UNKNOWN, SW_TYPE_COMPARTMENT, SW_TYPE_3D };






class SchemeWindow :  public ScalableWindow
{

private:

    ResultsWindowList   results_window_list;

    ReactionScheme      scheme;

    INT16               char_width;
    INT16               char_height;

    enum SCHEMWIN_OP    current_operation;
    BOOL8               in_edit_mode;

protected:

    ProcessObjectList   highlight_object_list;
    ProcessObjectList   select_object_list;
    ProcessObjectList   move_object_list;

    Rectangle           previous_rect;
    Rectangle           current_rect;

    Pen                 drag_pen;
    Brush               drag_brush;
    Pen                 std_pen;
    Brush               std_brush;
    Point               prev_position;
    Point               drag_offset;
    Pointer             original_ptr;

    Size                CalcTotalSize( ) const;

    void                SetOperation( enum SCHEMWIN_OP NewOp ) { current_operation = NewOp; };
    void                ActivateDragAttributes( );
    void                DeactivateDragAttributes( );

    ProcessObjectList&  GetHighlightObjectList() { return highlight_object_list; }
    ProcessObjectList&  GetSelectObjectList()   { return  select_object_list; }
    ProcessObjectList&  GetMoveObjectList()     { return move_object_list; }

    void                SetSelectObject ( ProcessObject* pPO = NULL );
    ProcessObject*      GetSelectObject ();

    void                SetHighlightObject ( ProcessObject* pPO = NULL );
    ProcessObject*      GetHighlightObject ();

    void                EditObject( Point& rPos );

    virtual void        Paint( const Rectangle& );
    virtual void        Resize();


public:
  
                        SchemeWindow( Window* pParent );
                       ~SchemeWindow();

    enum SCHEMWIN_OP    GetOperation( ) { return current_operation; };

    INT16               GetDefaultCharWidth() const { return char_width;}
    INT16               GetDefaultCharHeight() const { return  char_height; }

    void                ReconstructSpeciesDatabase( );
    BOOL8               DeleteResultsWindows();
    void                RecalcTotalSize() { SetTotalSize( CalcTotalSize() ); }

    ReactionScheme&     GetScheme() { return scheme; }
    const ReactionScheme&    GetScheme() const { return scheme; }
    SchemeFrameWindow*  GetpFrame() { return (SchemeFrameWindow*) GetParent(); }
         

    void                AddResultsWindow( Window* pWin ) { results_window_list.Insert( pWin, LIST_APPEND ); }
    void                RemoveResultsWindow( Window* pWin ) { results_window_list.Remove( pWin  ); }

    BOOL8               IsObjectSelected() { return ( 0 != GetSelectObjectList().Count() ); }
    void                SelectObject( Point& rPos );

    virtual void        Activate();
    virtual void        Deactivate();

    virtual void        StartAddCompartment(  ) {}
    virtual void        StartAddTransferObject( ) {}
    virtual void        CancelAddTransferObject( ) {}
    virtual void        CancelAddCompartmentObject( ) {}
    virtual BOOL8       PasteObject( const ProcessObject* const pClipObject ) { return FALSE; }
    virtual void        DeleteSelectedObject( ) {}
    virtual void        ChangeCompartmentDimensions() {}
    virtual void        ChangeSchemeTemperature() {}
    virtual void        ChangeSpeciesConcentration() {}

    virtual BOOL8       CopySelectedObject( ProcessObject*& rpClipObject );

    virtual enum SCHEMWIN_TYPE GetSchemeType( ) const { return SW_TYPE_UNKNOWN; }

    static  enum SCHEMWIN_TYPE QueryRxnFileSchemeType( const String& rFileName );
};





#endif

