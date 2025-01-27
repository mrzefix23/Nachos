#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;


static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out){          
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    lock = new Lock("console lock");
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

ConsoleDriver::~ConsoleDriver(){
    delete console;
    delete writeDone;
    delete readAvail;
    delete lock;    
}

void ConsoleDriver::PutChar(int ch){
    lock->Acquire();
     if(ch == (char) -1){
        //printf ("Au revoir!\n");
        lock->Release();
        return;
    }
    if (ch == '\n') {
        console->TX (ch);
        writeDone->P ();
    } 
    else{
        console->TX (ch);     
        writeDone->P ();      
    }
    lock->Release();
}

int ConsoleDriver::GetChar(){
    lock->Acquire();
    readAvail->P ();        // wait for character to arrive
    int ch = console->RX ();
    if(ch == EOF){
        lock->Release();
        return -1;  //retourne la même valeur que fgets quand un EOF arrive
    }
    lock->Release();
    return ch;
}

void ConsoleDriver::PutString(const char *s){
    lock->Acquire();
    while (*s != '\0') {
        console->TX (*s);   
        writeDone->P ();;
        s++;
    }
    lock->Release();
}

void ConsoleDriver::GetString(char *s, int n){
    lock->Acquire();
    char ch;
    int i = 0;
    while(i < n - 1){
        readAvail->P ();        // wait for character to arrive
        ch = console->RX ();
        if(ch == EOF || ch == '\0' || ch == '\n'){   //Lit une chaine de caractéres de taille n
            s[i] = '\0';                //jusqu'à rencontrer un '\0' ou un '\n'
            lock->Release();
            return;                     
        }
        s[i] = ch;
        i++;
    }
    s[i] = '\0';    //EOF
    lock->Release();
}

void ConsoleDriver::PutInt(int n){
    int size = 12;
    char* buffer = new char[size];
    snprintf(buffer, size, "%d", n);
    PutString(buffer);
    delete[] buffer;
}

void ConsoleDriver::GetInt(int *n){
    int size = 12;
    char* buffer = new char[size];
    GetString(buffer, size);
    char ch;
    if((int) strlen(buffer) == size - 1){
        while((ch = GetChar()) != '\n' && ch != EOF); // vide le flux d'entrée
    }
    sscanf(buffer, "%d", n);
    //printf("n = : %d\n", *n);
    delete[] buffer;
}


unsigned ConsoleDriver::copyStringFromMachine(int from, char *to, unsigned size){
    unsigned i = 0;    
    int ch;            

    while (i < size - 1 ){
        if (!machine->ReadMem(from + i, 1, &ch)){ 
            return i; 
        }
        to[i] = (char)ch;  
        if (to[i] == '\0'){ 
            return i;
        }
        i++; 
    }
    // On force la fin de fichier
    to[i] = '\0';
    
    return i;
}

void ConsoleDriver::copyStringToMachine(char *from, int to, unsigned size){
    unsigned i = 0;
    int ch;            

    while (i < size - 1){
        ch = (int)from[i]; 
        if(!machine->WriteMem(to + i, 1, ch)) { 
            return; 
        }
        if(from[i] == '\0'){ 
            return;
        }
        i++; 
    }

    machine->WriteMem(to + i, 1, '\0');     
}

#endif // CHANGED