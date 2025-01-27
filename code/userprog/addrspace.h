// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "noff.h"
#include "list.h"
#include "pageprovider.h"

#ifdef CHANGED  
#include "bitmap.h"
class Thread;
class Lock;

#endif //CHANGED

#define UserStacksAreaSize		1024	// increase this as necessary!

class AddrSpace:public dontcopythis
{
  public:
    AddrSpace (OpenFile * executable); // Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();              // De-allocate an address space

    

    void InitRegisters (void);  // Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState (void);      // Save/restore address space-specific
    void RestoreState (void);   // info on a context switch

    unsigned Dump(FILE *output, unsigned addr_s, unsigned sections_x, unsigned virtual_x, unsigned virtual_width,
                    unsigned physical_x, unsigned virtual_y, unsigned y,
                    unsigned blocksize);
                                // Dump program layout as SVG
    unsigned NumPages(void) { return numPages; }

    #ifdef CHANGED

    int AllocateUserStack();   // Allocate a stack for the user program
    void freeUserStack(int stackTop); // Free the user stack
    void decrementThreadCount(); // Decrement the number of threads using this address space
    void incrementThreadCount(); // Increment the number of threads using this address space
    int getThreadCount();       // Get the number of threads using this address space
    static void ReadAtVirtual(OpenFile *executable, int virtualAddr, int numBytes, int position, TranslationEntry *pageTable, unsigned numPages);
    void addThread(Thread* thread); // Add a thread to the list of threads using this address space
    void removeThread(); // Remove a thread from the list of threads using this address space
    int getThreadCountMax();    // Get the maximum number of threads that can use this address space
    Thread** getThreadList();   // Get the list of threads using this address space

    #endif // CHANGED

  private:
    NoffHeader noffH;           // Program layout

    TranslationEntry * pageTable; // Page table
    unsigned int numPages;      // Number of pages in the page table
    #ifdef CHANGED
    int threadCount;            // Number of threads using this address space
    Lock *lock;                 // Lock to protect the address space
    BitMap *stackMap;           // Bitmap to keep track of the user stacks
    Thread** threads;           // Threads using this address space
    int threadCountMax;         // Maximum number of threads that can use this address space
    #endif //CHANGED
};

extern List AddrspaceList;

#endif // ADDRSPACE_H
