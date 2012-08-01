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

//--------------------------------------------------------------------------
//
//      Definition of base class MATRIX
//      Date last modified: August 21, 1995
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#include "vsimtype.hxx"
#include "vsimglob.hxx"

#ifndef MATRIX_HXX
#define MATRIX_HXX


class matrix          //for integers
{
  protected:
	UINT32 no_rows;
	UINT32 no_columns;
	UINT32 *Element;

  public:
	matrix ();
	matrix ( UINT32 nr, UINT32 nc );
	matrix ( const matrix& m );
	virtual ~matrix () { delete [] (Element); }
	void InitMatrix ( matrix g );
	UINT32 ElementValue (UINT32 row, UINT32 column)	{
		return Element[no_rows * row + column];
		}
	void SetValue (UINT32 n, UINT32 row, UINT32 column) {
		Element[no_rows*row + column] = n;
		}
	UINT32 GetNoRows () { return no_rows; }
	UINT32 GetNoColumns () { return no_columns; }
};

#endif