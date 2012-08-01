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
//  EVENTS.HXX - a header file of the VSIM User interface 
// ............................................................................
//
//  This header file defines the constants for UserEvents
//
//  CREATED : 1.22.96 
//  AUTHOR  : Bill Hinsberg
//
// ----------------------------------------------------------------------------

#if !defined(__EVENTS_HXX__)
#define __EVENTS_HXX__


typedef unsigned long EVENT_TYPE;

const EVENT_TYPE  vEVT_UPDATE_AFTER_SIMULATION = 1000;
const EVENT_TYPE  vEVT_UPDATE_WINDOW_ASYNC     = 1001;
const EVENT_TYPE  vEVT_UPDATE_AFTER_ABORT      = 1002;

#endif
