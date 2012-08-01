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
//  FLOTARRY.CXX - a module for the VSIM User interface 
// ............................................................................
// 
//  This file defines the FloatArray class, to be used for 
//  containing one-dimensional arrays of floating point values 
//
//  CREATED : 6.21.95 
//  AUTHOR  : Bill Hinsberg  
//
// ----------------------------------------------------------------------------


#include <float.h>

#include "flotarry.hxx"



// ----------------------------------------------------------------------------
//                        FloatArray::FloatArray()
// ............................................................................
// 
//  default constructor
//
// ----------------------------------------------------------------------------

FloatArray::FloatArray() : 
array_size(0),
ptr( 0 )
{
}

// ----------------------------------------------------------------------------
//                        FloatArray::~FloatArray()
// ............................................................................
// 
//  destructor
//
// ----------------------------------------------------------------------------

FloatArray::~FloatArray()
{
     if ( ptr )
          delete[] ptr;
}


// ----------------------------------------------------------------------------
//                        FloatArray::FloatArray()
// ............................................................................
// 
//  constructor - allocates memory and initializes array 
//
// ----------------------------------------------------------------------------

FloatArray::FloatArray( const UINT32 ArraySize, const FLOAT64 InitialValue ) :
array_size(0),
ptr( 0 )
{ 
     Initialize( ArraySize, InitialValue );
}





// ----------------------------------------------------------------------------
//                        FloatArray::Initialize()
// ............................................................................
// 
//  allocates memory, and initializes array 
//
// ----------------------------------------------------------------------------

BOOL8 FloatArray::Initialize( const UINT32 ArraySize, const FLOAT64 InitialValue )
{
     // delete any memory currently allocated 

     if ( ptr )
          delete[] ptr;

     // now allocate memory 

     if (ArraySize)
          ptr = new FLOAT64[ ArraySize ];
     else
     {
          ptr = 0;
          array_size = 0;
          return FALSE;
     }

     if ( ptr )
     {
          UINT32 i = array_size = ArraySize; 

          while (i--)
               ptr[i] = InitialValue;

          return TRUE;
     }
     else
     {
          array_size = 0;
          return FALSE;
     }
}


// ----------------------------------------------------------------------------
//                        FloatArray::GetMinimumValue()
// ............................................................................
// 
//  scans through array to find minimum value
//
// ----------------------------------------------------------------------------

FLOAT64 FloatArray::GetMinimumValue() const
{
     FLOAT64 minimum;

     // if invalid object state then return DBL_MIN

     if ( ( 0 == ptr ) || ( 0 == array_size ) )
          minimum = DBL_MIN;
     else
     {
          // scan through the entire array looing for the minimum value 
          // start at the end of the array, initialize temp value 

          minimum = ptr[array_size-1];

          UINT32 i = array_size; 

          // work toward the front. comparing every value

          while (i--)
               if ( ptr[i] < minimum )
                    minimum = ptr[i];
     }

     return minimum;
}





// ----------------------------------------------------------------------------
//                        FloatArray::GetMaximumValue()
// ............................................................................
// 
//  scans through array to find maximum value
//
// ----------------------------------------------------------------------------

FLOAT64 FloatArray::GetMaximumValue() const
{
     FLOAT64 maximum;

     // if invalid object state then return DBL_MIN

     if ( ( 0 == ptr ) || ( 0 == array_size ) )
          maximum = DBL_MAX;
     else
     {
          // scan through the entire array looing for the maximum value 
          // start at the end of the array, initialize temp value 

          maximum = ptr[array_size-1];

          UINT32 i = array_size; 

          // work toward the front. comparing every value

          while (i--)
               if ( ptr[i] > maximum )
                    maximum = ptr[i];
     }

     return maximum;
}







#if defined(__UNIT_TEST__)

#include <iostream.h>
#include <iomanip.h>

void main()
{
     FloatArray array1;
     FloatArray array2(10);
     FloatArray array3(10, 1.2345e6 );

     FloatArray array4(0);

     cout << "testing constructors" << endl;

     cout << "array1 valid = " << (int) array1.IsValid() << endl;
     cout << "array2 valid = " << (int) array2.IsValid() << endl;
     cout << "array3 valid = " << (int) array3.IsValid() << endl;
     cout << "array4 valid = " << (int) array4.IsValid() << endl;

     cout << "array1 size = " << array1.GetArraySize() << endl;
     cout << "array2 size = " << array2.GetArraySize() << endl;

     cout << "array3 size = " << array3.GetArraySize() << endl;
     cout << "array4 size = " << array4.GetArraySize() << endl;

     UINT32 i = array3.GetArraySize();

     cout << "testing initialization" << endl;

     cout << "maxvalue1 = " << array1.GetMaximumValue() << endl;
     cout << "minvalue1 = " << array1.GetMinimumValue() << endl;
     while (i--)
          cout << "array3[" << i <<"] = " << array3[i] << endl;

     cout << "maxvalue3 = " << array3.GetMaximumValue() << endl;
     cout << "minvalue3 = " << array3.GetMinimumValue() << endl;

     cout << "testing initialization" << endl;

     array1.Initialize( 5 ); 
     cout << "array1 valid = " << (int) array1.IsValid() << endl;

     array1[0] = 0.9;
     array1[1] = 1.9;
     array1[2] = 2.9;
     array1[3] = 3.9;
     array1[4] = 4.9;

     i = array1.GetArraySize();

     while (i--)
          cout << "array1[" << i <<"] = " << array1[i] << endl;


     array3.Initialize( 5 ); 
     cout << "array3 valid = " << (int) array3.IsValid() << endl;
     array3[0] = 0.9;
     array3[1] = 1.9;
     array3[2] = 2.9;
     array3[3] = 3.9;
     array3[4] = 4.9;

     i = array3.GetArraySize();

     while (i--)
          cout << "array3[" << i <<"] = " << array3[i] << endl;

     cout << "maxvalue = " << array3.GetMaximumValue() << endl;
     cout << "minvalue = " << array3.GetMinimumValue() << endl;

     array3[0] = 0.9;
     array3[1] = -100.9;
     array3[2] = 2.9;
     array3[3] = -3.9e-20;
     array3[4] = 4.9e7;

     cout << "maxvalue = " << array3.GetMaximumValue() << endl;
     cout << "minvalue = " << array3.GetMinimumValue() << endl;

     array3[0] = -0.9;
     array3[1] = -100.9;
     array3[2] = -2.9;
     array3[3] = -3.9e-20;
     array3[4] = -4.9e7;

     cout << "maxvalue = " << array3.GetMaximumValue() << endl;
     cout << "minvalue = " << array3.GetMinimumValue() << endl;

}



#endif


