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

//---------------------------------------------------------------------------
//
//      Declaration of OUTPUT_OBJECT
//      This object manages data printing to file
//      Date last modified: November 7, 1995
//      Written by: Frances A. Houle
//      IBM  
//
//---------------------------------------------------------------------------

#ifndef OUT_OBJ_HXX
#define OUT_OBJ_HXX

#include "vsimtype.hxx"
#include "txtstrem.hxx"
#include "binstrem.hxx"
#include "idtypes.hxx"
#include "vsimglob.hxx"


class output_object {

private:
	enum ELAPSED_TIME_UNITS TimeUnits;
	UINT32 FileOffset;
	String OutFileName;

public:
     output_object ();
	output_object ( const output_object& o_o );
	~output_object () {}

	ELAPSED_TIME_UNITS GetTimeUnits () { return TimeUnits; }
	void SetTimeUnits ( ELAPSED_TIME_UNITS t_u ) { TimeUnits = t_u; }
	void SetFileOffset ( UINT32 f ) { FileOffset = f; }
	void SetFileName ( String fn ) { OutFileName = fn; }
	void OpenFile ();
	void WriteState ();

	friend TextInputStream& operator >> ( TextInputStream& rTIS, output_object& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, output_object& rObject );


#endif



