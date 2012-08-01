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
//  SCALWIN.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file describes the ScalableWindow  class, which contains a
//  scaling mechanism for creating drawings on screen 
//
//  CREATED : 10.10.95 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__SCALWIN_HXX__)
#define __SCALWIN_HXX__


#include <sv.hxx>

class ScalableWindow : public Window 
{
protected:

    Point               win_offset_pixels;         // offset to virtual window (pixel)
    Size                virtual_win_size_logic;    // total size of virtual window
    Size                output_size_pixels;        // size of actual output area of window w/o tool, status, scrollbars
    Point               output_area_origin_pixels;
    Fraction            current_window_scale;

public:

                        ScalableWindow( Window* pParent );
    Size                GetTotalSizePixels( ) const { return LogicToPixel( virtual_win_size_logic ); }
    Size                GetTotalSizeLogic( ) const { return virtual_win_size_logic; }

    void                SetTotalSize( const Size& rNewSize ){virtual_win_size_logic = rNewSize;}
    Fraction            GetScale() const { return current_window_scale; }
    virtual Fraction    ChangeScale( const Fraction& rNewScale );

  
    INT16               ConcealedWidth() const { return ( GetTotalSizePixels().Width() - GetOutputSizePixel().Width() ); }
    INT16               ConcealedHeight() const { return ( GetTotalSizePixels().Height() - GetOutputSizePixel().Height() ); }

    INT16               HorizontalOffsetPixels() { return win_offset_pixels.X(); }
    INT16               VerticalOffsetPixels() { return win_offset_pixels.Y(); }

    void                SetPosSizePixel( const Point& rNewPos, const Size& rNewSize );

    virtual void        Scroll( INT16 nDeltaX, INT16 nDeltaY );



    static const Fraction      DECREMENT_SCALE;
    static const Fraction      INCREMENT_SCALE;
    static const Fraction      MAXIMUM_SCALE;
    static const Fraction      MINIMUM_SCALE;
    static const Fraction      FULL_SCALE;

};



#endif

