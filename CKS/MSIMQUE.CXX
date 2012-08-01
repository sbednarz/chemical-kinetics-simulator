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
/*  FILE NAME : msimque.cxx                                                 */
/*                                                                          */
/*  This module implements the class SimulationQueue to be used for         */
/*  setting up a list of simulations to be run                              */
/*                                                                          */
/*  Written using the Starview class libraries to provide common code for   */
/*  multiple platforms                                                      */
/*                                                                          */
/*  Version 1.0  started January 1994                                       */
/*                                                                          */
/*  variable naming conventions :                                           */
/*     variables to a function all in lower case                            */
/*     variables global to a module but not to application in mixed case    */
/*     variables global to the application in mixed case with "msim" prefix */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "msim2.hxx"
#pragma hdrstop

#include "msimque.hxx"
#include "msimstrg.hxx"


void SimQueue::ClearQueue( )
{
     msimPQUEUE_ENTRY p_cur_entry;

     msimBOOL save_state = queue_locked;

     queue_locked = TRUE;

     while ( Count( ) > 0 )
     {
          p_cur_entry = Remove( );

          if ( p_cur_entry )
          {
               remove( p_cur_entry->runparm_file );
               p_cur_entry->p_instance->sim_return_code = p_cur_entry->saved_rc;
               delete p_cur_entry;
          }
     }
     queue_locked = save_state;
}


msimBOOL SimQueue::AddToQueue( msimPINSTANCE pInstance, PCHAR RunParmFilename )
{
     // are parameters valid
     msimPQUEUE_ENTRY p_new_entry;

     if ( ! pInstance || ! RunParmFilename )
          return FALSE;

     // allocate memory for new member of queue
     p_new_entry = new msimQUEUE_ENTRY;

     if ( ! p_new_entry )
     {
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, NULL );
          return FALSE;
     }

     // fill in new struct members
     p_new_entry->p_instance = pInstance;
     msimStringCopy( p_new_entry->runparm_file, RunParmFilename,
          sizeof p_new_entry->runparm_file );
     p_new_entry->saved_rc = pInstance->sim_return_code;

     pInstance->sim_return_code = msimWAITING_IN_QUEUE;

     // now add to queue

     Insert( p_new_entry, LIST_APPEND );
     return TRUE;
}

msimBOOL SimQueue::RetrieveNextFromQueue( msimPINSTANCE PTR ppInstance,
              PCHAR RunParmFilename, size_t BufSize )
{
     msimPQUEUE_ENTRY p_cur_entry;

     if ( ppInstance == NULL || RunParmFilename == NULL )
          return FALSE;

     p_cur_entry = Remove( );

     if ( p_cur_entry == NULL )
          return FALSE;

     // retrieve the data
     p_cur_entry->p_instance->sim_return_code = p_cur_entry->saved_rc;//reset field

     *ppInstance = p_cur_entry->p_instance;

     msimStringCopy( RunParmFilename, p_cur_entry->runparm_file, BufSize );

     delete p_cur_entry;
     return TRUE;
}


// constructor for SimQueue...

SimQueue::SimQueue( ) : InstanceList ( msimMAX_NO_INSTANCES, msimMAX_NO_INSTANCES )
{
     queue_locked = FALSE;
}

msimBOOL SimQueue::IsAnotherSimulationReady( )
{
     return ( ( queue_locked == FALSE ) && ( Count( ) > 0 ) );
}


msimBOOL SimQueue::RemoveFromQueue( msimPINSTANCE pInstance )
{
     msimBOOL save_state = queue_locked;
     queue_locked = TRUE;
     msimBOOL found_it = FALSE;


     ULONG count = Count ( );
     for ( ULONG pos = 0; pos < count; pos++ )
     {
          if ( GetObject( pos ) ->p_instance == pInstance )
          {
               found_it = TRUE;
               break;
          }
     }

     if ( found_it )
     {
          msimPQUEUE_ENTRY obj = GetObject( pos );

          obj->p_instance->sim_return_code = obj->saved_rc;

          remove( obj->runparm_file );

          Remove( obj );
     }

     queue_locked = save_state;
     return found_it;
}


// instantiate queue

SimQueue msimSimQueue;



class QueueDlg : public ModalDialog
{
protected :
     ListBox QueueLB;
     PushButton DeleteOnePB;
     PushButton DeleteAllPB;
     OKButton ClosePB;
     PushButton ToTopPB;
     HelpButton HelpPB;
     FixedText FixedText1;
     GroupBox aGroupBox1;


     void InitializeDialog( );

public :
     QueueDlg( Window PTR pParent );
     ~QueueDlg( );

     void CloseHandler( PushButton PTR pButton );
     void DeleteOneHandler( PushButton PTR pButton );
     void DeleteAllHandler( PushButton PTR pButton );
     void ToTopHandler( PushButton PTR pButton );

};

// constructor

QueueDlg::QueueDlg( Window PTR pParent ) :
ModalDialog ( pParent, ResId ( msimQUEUE_DLG ) ),
QueueLB ( this, ResId ( msimQUEUE_LIST ) ),
DeleteOnePB ( this, ResId ( msimQUEUE_DELETE_ONE ) ),
DeleteAllPB ( this, ResId ( msimQUEUE_DELETE_ALL ) ),
ClosePB ( this, ResId ( msimQUEUE_CLOSE ) ),
ToTopPB ( this, ResId ( msimQUEUE_UNDO ) ),
HelpPB ( this, ResId ( msimQUEUE_HELP ) ),
FixedText1 ( this, ResId ( 1 ) ),
aGroupBox1 ( this, ResId ( 1 ) )
{
     FreeResource( );
     msimSimQueue.LockQueue( );
     InitializeDialog( );

     DeleteOnePB.ChangeClickHdl(
          LINK( this, QueueDlg, DeleteOneHandler ) );

     DeleteAllPB.ChangeClickHdl(
          LINK( this, QueueDlg, DeleteAllHandler ) );

     ClosePB.ChangeClickHdl(
          LINK( this, QueueDlg, CloseHandler ) );

     ToTopPB.ChangeClickHdl(
          LINK( this, QueueDlg, ToTopHandler ) );

     msimCascadeWindowOnOwner( this, pParent );

}


QueueDlg::~QueueDlg( )
{
     msimSimQueue.UnlockQueue( );
     msimStartDeferredSimulation( );
}

void QueueDlg::CloseHandler( PushButton PTR )
{
     EndDialog( );
     msimUpdateMainWinData( msimUSR_EVT_UPDATE_WIN_ONLY );
}

void QueueDlg::DeleteOneHandler( PushButton PTR )
{
     // get the queue pointer corresponding to the selected listbox entry

     msimPQUEUE_ENTRY obj =
     msimSimQueue.GetObject( QueueLB.GetSelectEntryPos( ) );

     remove( obj->runparm_file );// delete the temporary file associated with it

     obj->p_instance->sim_return_code = obj->saved_rc;//reset flag in Instance struct

     msimSimQueue.Remove( obj );
     msimSimQueue.First( );            // set first object to be current object

     InitializeDialog( );
}

void QueueDlg::DeleteAllHandler( PushButton PTR )
{
     msimSimQueue.ClearQueue( );
     InitializeDialog( );
}

void QueueDlg::ToTopHandler( PushButton PTR )
{
     // cut selected item from list
     msimPQUEUE_ENTRY obj =
     msimSimQueue.Remove( QueueLB.GetSelectEntryPos( ) );

     // insert it back in frm of the current object

     msimSimQueue.Insert( obj, ( ULONG ) 0 );// insert at position 0
     msimSimQueue.First( );            // set first object to be current object

     InitializeDialog( );
}


void QueueDlg::InitializeDialog( )
{

     // turn off update to prevent flicker

     QueueLB.ChangeUpdateMode( FALSE );

     QueueLB.Clear( );

     // fill the listbox

     ULONG count = msimSimQueue.Count ( );

     if ( count )
     {
          QueueLB.Enable( );
          DeleteOnePB.Enable( );
          DeleteAllPB.Enable( );
          ToTopPB.Enable( );

          for ( ULONG pos = 0; pos < count; pos++ )
               QueueLB.InsertEntry( String( msimSimQueue.GetObject( pos ) ->p_instance->base_filename ) );

          QueueLB.SelectEntryPos( 0 );
     }
     else
     {
          QueueLB.Disable( );
          DeleteOnePB.Disable( );
          DeleteAllPB.Disable( );
          ToTopPB.Disable( );
     }

     QueueLB.ChangeUpdateMode( TRUE );
     QueueLB.Update( );

#if defined(__MAC__)
     QueueLB.Invalidate( );
#endif

}


void msimQueueDialog( msimWID Owner )
{
     QueueDlg PTR pdlg =
                  new QueueDlg( Owner );

     if ( pdlg )
     {
          pdlg->Execute( );
          delete pdlg;
     }
     else
          msimMemoryError(( USHORT ) msimMEM_ALLOC_ERROR, __FILE__,
               __TIMESTAMP__, __LINE__, Owner );

     return;


}


