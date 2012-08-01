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
//      Declaration of class K_INFO
//      base class
//	   This object is a generic rate constant object. It handles all
//		rate constant formats.
//      Date last modified: January 23, 1997
//      Written by Frances A. Houle
//      IBM  
//
//--------------------------------------------------------------------------

#ifndef K_INFO_HXX
#define K_INFO_HXX

#include <math.h>
#include "vsimtype.hxx"
#include "vsimcons.hxx"
#include "datastru.hxx"
#include "idtypes.hxx"
#include "profile.hxx"

class process_area;
class process;

class k_info {

protected:
	process_area*   		pProcessArea;
	process*			pStep;
	rate_coefficient_info 		RateConstant;
	profile*			kProfile;

public:
	k_info();
	k_info( const k_info& k_i );
	virtual ~k_info() {}

	virtual FLOAT64	Get_k ( FLOAT64 T );
	void				Set_k ( rate_coefficient_info& k);
	enum RATE_CONST_FORMAT	GetFormat() { return RateConstant.Format; }
	virtual void 			Update_k ( FLOAT64 SizeRatio ) {}


     // the following are used to initialize the rate constant object
	void		SetAreaPointer( process_area* r ) { pProcessArea = r; }
	void		SetStepPointer( process* r ) { pStep = r; }
     profile*  GetkProfilePointer() { return kProfile; }
	void		SetFormat( enum RATE_CONST_FORMAT f ) { RateConstant.Format = f; }
	virtual void   	Initialize_k() {}

	friend TextInputStream& operator >> ( TextInputStream& rTIS, k_info& rObject );

};

TextInputStream& operator >> ( TextInputStream& rTIS, k_info& rObject );

#endif

