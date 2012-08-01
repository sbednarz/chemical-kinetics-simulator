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

/*--------------------------------------------------------------------------*/
/*                     MSIM Chemical Reaction Simulator                     */
/*                            -----------------                             */
/*                                                                          */
/*                      by W. Hinsberg and F. Houle                         */
/*                      IBM Almaden Research Center                         */
/*                                  ----                                    */
/*                                                                          */
/*  FILE NAME : msimque.hxx                                                 */
/*                                                                          */
/*  This header file declares global functions for implementing a queue     */
/*  which contains an ordered list of simulations to be run                 */
/*                                                                          */
/*  Written using the Starview class libraries to provide common code for   */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started January 1994                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

struct QUEUE_ENTRY_STRUCT
{
     msimPINSTANCE p_instance;
     msimRC saved_rc;
     msimFILE_STRING runparm_file;
};

typedef struct QUEUE_ENTRY_STRUCT msimQUEUE_ENTRY, PTR msimPQUEUE_ENTRY;



DECLARE_LIST( InstanceList, msimPQUEUE_ENTRY )

class SimQueue : public InstanceList
{
protected :
     msimBOOL queue_locked;            // to be used during queue manipulations

public :
     SimQueue( );

     void LockQueue( )
     {
          queue_locked = TRUE;
     };
     void UnlockQueue( )
     {
          queue_locked = FALSE;
     };
     msimBOOL IsAnotherSimulationReady( );
     msimBOOL AddToQueue( msimPINSTANCE pInstance, PCHAR RunParmFilename );
     void ClearQueue( );
     msimBOOL RetrieveNextFromQueue( msimPINSTANCE PTR ppInstance,
          PCHAR RunParmFilename, size_t BufSize );
     msimBOOL RemoveFromQueue( msimPINSTANCE pInstance );
};


extern SimQueue msimSimQueue;

void msimQueueDialog( msimWID Owner );

