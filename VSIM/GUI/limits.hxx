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
//  LIMITS.HXX - a header file for the VSIM User interface 
// ............................................................................
// 
//  This header file declares the Limits class, to be used to specify limits of
//  a range of floating numbers 
//
//  CREATED : 6.27.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------



#if !defined(__LIMITS_HXX__)
#define __LIMITS_HXX__

#include "vsimtype.hxx"


class Limits
{
private:
     FLOAT64       minval;
     FLOAT64       maxval;

public:
     Limits() : minval( 0.0 ), maxval (0.0 ){}
     Limits( FLOAT64 Min, FLOAT64 Max ): minval( Min ), maxval ( Max ){} 
     Limits( const Limits& rObject ): minval( rObject.minval ), maxval ( rObject.maxval ){} 

     FLOAT64     Min() const { return minval; }
     FLOAT64     Max() const { return maxval; }

     FLOAT64&    Min() { return minval; }
     FLOAT64&    Max() { return maxval; }

     FLOAT64     Range() const { return (maxval-minval); }

     BOOL8       Inside( FLOAT64 Val ) const { return ( (Val >= minval) && ( Val <= maxval )  ); }

     // returns limits that completely enclose object and Limits passed as parameter 
     
     void        Union( const Limits& rLimits )
                 { if ( rLimits.Min() < minval ) minval = rLimits.Min();
                   if ( rLimits.Max() > maxval ) maxval = rLimits.Max(); }


     Limits&     operator = ( const Limits& rObject ) 
                 { minval = rObject.minval; maxval = rObject.maxval; return *this; }
};

#endif

