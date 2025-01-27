#include "userthread.h"
#include "system.h"


int do_ThreadCreate(int f, int arg, int exitAddr)
{
    Thread *newThread = new Thread("UserThread");

    if (newThread == NULL)
    {
        return -1;
    }
 
    newThread->space=currentThread->space;

    schmurtzStruct *schmurtz = new schmurtzStruct;
    schmurtz->f = f;
    schmurtz->arg = arg;
    schmurtz->exit = exitAddr; //adresse de ThreadExit

    currentThread->space->addThread(currentThread);
    newThread->Start(StartUserThread, (void *)schmurtz);  
    return 0;
}


//fonction auxiliaire pour lancer un thread utilisateur

 void StartUserThread(void *schmurtz) {
    schmurtzStruct *args = (schmurtzStruct *)schmurtz;

    
    int f = args->f;
    int arg = args->arg;

    for (int i = 0; i < NumTotalRegs; i++) //intialiser les registres
        machine->WriteRegister (i, 0);


    // Exécuter la fonction utilisateur
    machine->WriteRegister(PCReg, f);  //adresse de la fonction
    machine->WriteRegister(NextPCReg, f + 4);  //adresse de la prochaine instruction
    machine->WriteRegister(4, arg); // passer l'agument à la fonction
    machine->WriteRegister(RetAddrReg, args->exit); // passer l'adresse de retour à la fonction ThreadExit

    // initialiser le registre de la pile
    int stackTop;
    while((stackTop = currentThread->space->AllocateUserStack()) == -1){ //on attend que la pile soit disponible
        currentThread->Yield();
    }
    machine->WriteRegister (StackReg, stackTop);

    DEBUG('x', "StartUserThread: stackTop = %d\n", stackTop);
    DEBUG('x', "StartUserThread: PCReg = %d, NextPCReg = %d, StackReg = %d\n", f, f + 4, stackTop);

    delete args; // libérer schmurtz
    //machine->DumpMem("threads.svg"); // quand activé avec plusieurs threads cause des erreurs de mémoire
    machine->Run();  
}



//fonction pour terminer un thread utilisateur
void do_ThreadExit(void) {
    int stackTop = machine->ReadRegister(StackReg);  // Obtient l'adresse de la pile
    currentThread->space->freeUserStack(stackTop);
    currentThread->space->removeThread();
    if (currentThread->space->getThreadCount() == 0) {
        do_ProcessExit();
    }
    currentThread->Finish();
    
}