#include "userfork.h"
#include "system.h"

int numberOfProcess = 1;

int do_ForkExec(const char* s){
    OpenFile *executable = fileSystem->Open (s);
    AddrSpace *space;

    if (executable == NULL)
        {
            SetColor (stdout, ColorRed);
            SetBold (stdout);
            printf ("Unable to open file %s\n", s);
            ClearColor (stdout);
            return -1;
        }

    try{
        space = new AddrSpace (executable);
    }catch(std::bad_alloc const&){
        delete executable;
        return -1;
    }

    Thread *newThread = new Thread("Forked thread");
    if (newThread == NULL)
    {
        return -1;
    }
    
    newThread->space = space;
    delete executable;		// close file

    numberOfProcess++;
    newThread->Start(StartUserProg, (void *)space);
    
    return 0;
}

void StartUserProg(void *space){
    AddrSpace *space1 = (AddrSpace *)space;
    
    space1->InitRegisters ();	// set the initial register values
    space1->RestoreState ();	// load page table register

    //machine->DumpMem ("fork.svg");
    machine->Run ();		// jump to the user progam
    ASSERT_MSG (FALSE, "Machine->Run returned???\n");	// machine->Run never returns;

}

void do_ProcessExit(){
    numberOfProcess--;

    // stop all threads except the current one
    Thread **threads = currentThread->space->getThreadList();
    // set stopThread to true for all threads except the current one
    for(int i = 0; i < currentThread->space->getThreadCount(); i++){
        if(threads[i] != NULL && threads[i] != currentThread){
            threads[i]->stopThread = true;
        }
    }

    // wait for all threads to finish except the current one
    //while(currentThread->space->getThreadCount() > 1){
    //    currentThread->Yield();
    //}
    // doesn't work, never exiting the loop

    //stop the process
    if(numberOfProcess == 0){
        interrupt->Powerdown();
    }
    else{
        AddrSpace *space = currentThread->space;
        delete space;
        currentThread->Finish();
    }
}