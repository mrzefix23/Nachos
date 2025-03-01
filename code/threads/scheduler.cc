// scheduler.cc
//      Routines to choose the next thread to run, and to dispatch to
//      that thread.
//
//      These routines assume that interrupts are already disabled.
//      If interrupts are disabled, we can assume mutual exclusion
//      (since we are on a uniprocessor).
//
//      NOTE: We can't use Locks to provide mutual exclusion here, since
//      if we needed to wait for a lock, and the lock was busy, we would
//      end up calling FindNextToRun(), and that would put us in an
//      infinite loop.
//
//      Very simple implementation -- no priorities, straight FIFO.
//      Might need to be improved in later assignments.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "scheduler.h"
#include "system.h"
#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#endif

//----------------------------------------------------------------------
// Scheduler::Scheduler
//      Initialize the list of ready but not running threads to empty.
//----------------------------------------------------------------------

Scheduler::Scheduler ()
{
    readyList = new List;
    halted = FALSE;
}

//----------------------------------------------------------------------
// Scheduler::Stop
//      Prevent further context switches, used when halting the system
//----------------------------------------------------------------------

void
Scheduler::Stop ()
{
    halted = TRUE;
}

//----------------------------------------------------------------------
// Scheduler::~Scheduler
//      De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler ()
{
    delete readyList;
    readyList = NULL;
}

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
//      Mark a thread as ready, but not running.
//      Put it on the ready list, for later scheduling onto the CPU.
//
//      "thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread * thread)
{
    DEBUG ('t', "Putting thread %p %s on ready list.\n", thread, thread->getName ());

    thread->setStatus (READY);
    readyList->Append ((void *) thread);
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
//      Return the next thread to be scheduled onto the CPU.
//      If there are no ready threads, return NULL.
// Side effect:
//      Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    if (halted)
        return NULL;
    return (Thread *) readyList->Remove ();
}

//----------------------------------------------------------------------
// Scheduler::Run
//      Dispatch the CPU to nextThread.  Save the state of the old thread,
//      and load the state of the new thread, by calling the machine
//      dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//      already been changed from running to blocked or ready (depending).
// Side effect:
//      The global variable currentThread becomes nextThread.
//
//      "nextThread" is the thread to be put into the CPU.
//----------------------------------------------------------------------

void
Scheduler::Run (Thread * nextThread)
{
    Thread *oldThread = currentThread;

    // LB: For safety...
    ASSERT (interrupt->getLevel () == IntOff);
    // End of addition

#ifdef USER_PROGRAM		// ignore until running user programs
    if (currentThread->space != NULL)
      {				// if this thread is a user program,
          currentThread->SaveUserState ();        // save the user's CPU registers
          currentThread->space->SaveState ();
      }
#endif

    oldThread->CheckOverflow ();	// check if the old thread
    // had an undetected stack overflow

    currentThread = nextThread;	// switch to the next thread
    currentThread->setStatus (RUNNING);	// nextThread is now running

    DEBUG ('t', "Switching from thread %p \"%s\" to thread %p \"%s\"\n",
           oldThread, oldThread->getName (), nextThread, nextThread->getName ());

    // This is a machine-dependent assembly language routine defined
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

#ifdef __SANITIZE_ADDRESS__
    if (threadToBeDestroyed == oldThread)
        __sanitizer_start_switch_fiber (NULL, nextThread->stack, nextThread->stack_size);
    else
        __sanitizer_start_switch_fiber (&oldThread->fake_stack, nextThread->stack, nextThread->stack_size);
#endif
    SWITCH (oldThread, nextThread);
#ifdef __SANITIZE_ADDRESS__
    __sanitizer_finish_switch_fiber (currentThread->fake_stack, NULL, NULL);
#endif

    DEBUG ('t', "Now in thread %p \"%s\"\n", currentThread, currentThread->getName ());

    // If the old thread gave up the processor because it was finishing,
    // we need to delete its carcass.  Note we cannot delete the thread
    // before now (for example, in Thread::Finish()), because up to this
    // point, we were still running on the old thread's stack!
    if (threadToBeDestroyed != NULL)
      {
          Thread *destroying = threadToBeDestroyed;
          threadToBeDestroyed = NULL;
          delete destroying;
      }

#ifdef USER_PROGRAM
    if (currentThread->space != NULL)
      {				// if there is an address space
          currentThread->RestoreUserState ();        // to restore, do it.
          currentThread->space->RestoreState ();
      }

    #ifdef CHANGED
   
    if(currentThread->stopThread){
        currentThread->space->removeThread();
        currentThread->Finish();
    }
    #endif //CHANGED
#endif
}

//----------------------------------------------------------------------
// Scheduler::Print
//      Print the scheduler state -- in other words, the contents of
//      the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print ()
{
    printf ("Ready list contents:\n");
    readyList->Mapcar (ThreadPrint);
}
