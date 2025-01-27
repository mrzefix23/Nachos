// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
#include "userfork.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

#ifdef CHANGED
// assure que le flux est vide
void clearStdin(){
  DEBUG('s', "Clearing stdin\n");
    char ch;
    while ((ch = consoledriver->GetChar()) != EOF && ch != '\n'){
     // printf("ch = %c\n", ch);
    }
}
#endif // CHANGED

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------


#ifdef CHANGED
bool checkflux = false;  //mettre à true quand on utilise GetString
#endif // CHANGED

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->ReadRegister (BadVAddrReg);

    switch (which)
      {
        case SyscallException:
          {
            switch (type)
              {
                case SC_Halt:
                  {
                    DEBUG ('s', "Shutdown, initiated by user program.\n");
                    // on supprime les caractères restants dans le flux avant de terminer le programme
                    if(checkflux){
                      clearStdin();
                    }
                    interrupt->Powerdown ();
                    break;
                  }
                  #ifdef CHANGED
                  case SC_PutChar:
                  {
                    DEBUG ('s', "Putchar.\n");
                    char ch = (char)machine->ReadRegister(4);
                    consoledriver->PutChar(ch);
                    break;
                  }
                  case SC_PutString:
                  {
                     DEBUG ('s', "PutString.\n");
                     char buffer[MAX_STRING_SIZE];
                     int addr = machine->ReadRegister(4);
                     int read;            // Nombre de caractères lus
                     int totalRead = 0;   // Nombre total de caractères lus
                     while(true){
                      read = consoledriver->copyStringFromMachine(addr + totalRead, buffer, MAX_STRING_SIZE);
                      if(read == 0){  // Aucun caractère lu
                        break;
                      }
                      consoledriver->PutString(buffer);
                      if(read < MAX_STRING_SIZE -1){   // La fin de la chaîne est atteinte
                        break;
                      }
                      totalRead += read;
                      
                     }
                    break;
                  }
                  case SC_GetChar:
                  {
                    DEBUG ('s', "GetChar.\n");
                    char ch = consoledriver->GetChar();
                    machine->WriteRegister(2, ch);
                    if(ch == EOF){
                      checkflux = false; // On a atteint la fin du flux
                    }
                    else{
                      checkflux = true; // Il reste des caractères dans le flux
                    }
                    break;
                  }
                  case SC_GetString:
                  {
                    DEBUG ('s', "GetString.\n");
                    int addr = machine->ReadRegister(4);
                    int size = machine->ReadRegister(5);
                    char buffer[size];
                    consoledriver->GetString(buffer, size);
                    consoledriver->copyStringToMachine(buffer, addr, size);
                    if((int) strlen(buffer) < size - 1){
                      checkflux = false; // On a lu toute la chaîne
                    }
                    else{
                      checkflux = true; // Il reste des caractères dans le flux
                    }
                    break;
                  }
                  case SC_Exit: {
                    DEBUG('s', "Exit.\n");
                    // on supprime les caractères restants dans le flux avant de terminer le programme
                    if(checkflux){
                      clearStdin();
                    }
                    do_ProcessExit();
                    break;
                    }
                  case SC_PutInt:
                  {
                    DEBUG ('s', "PutInt.\n");
                    int n = machine->ReadRegister(4);
                    consoledriver->PutInt(n);
                    break;
                  }
                  case SC_GetInt:
                  {
                    DEBUG ('s', "GetInt.\n");
                    int n;
                    consoledriver->GetInt(&n);
                    machine->WriteMem(machine->ReadRegister(4), 4, n);
                    break;
                  }
                  case SC_ThreadCreate:
                  {
                    DEBUG ('s', "ThreadCreate.\n");
                    int f = machine->ReadRegister(4);
                    int arg = machine->ReadRegister(5);
                    int exitAddr = machine->ReadRegister(6);
                    DEBUG('s', "exitAddr = %d\n", exitAddr);
                    int returnValue = do_ThreadCreate(f, arg, exitAddr);
                    machine->WriteRegister(2, returnValue);
                    break;
                  }
                  case SC_ThreadExit:
                  {
                    DEBUG ('s', "ThreadExit.\n");
                    if(checkflux && currentThread->space->getThreadCount() == 1){
                      clearStdin();
                    }
                    do_ThreadExit();
                    
                    break;
                  }
                  case SC_ForkExec:
                  {
                    DEBUG ('s', "ForkExec.\n");
                    int addr = machine->ReadRegister(4);
                    char buffer[256]; // Taille du buffer augmentée pour stocker la chaîne entière
                    
                    int read = consoledriver->copyStringFromMachine(addr, buffer, sizeof(buffer));

                    if (read == 0) {
                        printf("Erreur : la chaîne est vide ou n'a pas pu être lue.\n");
                    } else {
                        buffer[read] = '\0';  // Assurez-vous que la chaîne est bien terminée
                       // printf("buffer = %s\n", buffer);
                    }
                    int returnValue = do_ForkExec(buffer);
                    machine->WriteRegister(2, returnValue);
                    
                    break;
                  }
                  case SC_SemP:
                  {
                    DEBUG('s', "Semaphore P\n");
                    //int semAddr = machine->ReadRegister(4);
                    //Semaphore *sem = (Semaphore *)semAddr;
                    //sem->P();
                    break;
                  }
                  case SC_SemV:
                  {
                    DEBUG('s', "Semaphore V\n");
                    //int semAddr = machine->ReadRegister(4);
                    //Semaphore *sem = (Semaphore *)semAddr;
                    //sem->V();
                    break;
                  }
                  case SC_SemInit:
                  {
                    DEBUG('s', "Semaphore initialization\n");
                    //int semAddr = machine->ReadRegister(4);
                    //int initialValue = machine->ReadRegister(5);

                    // Crée le sémaphore
                    //Semaphore *sem = new Semaphore("UserSemaphore", initialValue);
                    //*(Semaphore **)semAddr = sem; // associer l'adresse utilisateur au sémaphore
                    break;
                  }
                  #endif
                default:
                  {
                    ASSERT_MSG(FALSE, "Unimplemented system call %d\n", type);
                  }
              }

            // Do not forget to increment the pc before returning!
            // This skips over the syscall instruction, to continue execution
            // with the rest of the program
            UpdatePC ();
            break;
          }

        case PageFaultException:
          if (!address) {
            ASSERT_MSG (FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
          } else {
            // For now
            ASSERT_MSG (FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
          }
          break;

        case ReadOnlyException:
          // For now
          ASSERT_MSG (FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case BusErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case AddressErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case OverflowException:
          // For now
          ASSERT_MSG (FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
          break;

        case IllegalInstrException:
          // For now
          ASSERT_MSG (FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
          break;

        default:
          ASSERT_MSG (FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
          break;
      }
}

