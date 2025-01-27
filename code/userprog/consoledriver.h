#ifdef CHANGED

#ifndef CONSOLEDRIVER_H
#define CONSOLEDRIVER_H

#include "copyright.h"
#include "synch.h" 
#include "utility.h"
#include "console.h"
class ConsoleDriver:dontcopythis {
  public:
                                // initialize the hardware console device
    ConsoleDriver(const char *readFile, const char *writeFile);
    ~ConsoleDriver();           // clean up
    void PutChar(int ch);       // Behaves like putchar(3S)
    int GetChar();              // Behaves like getchar(3S)
    unsigned copyStringFromMachine(int from, char *to, unsigned size);
    void copyStringToMachine(char *from, int to, unsigned size);
    void PutString(const char *s); // Behaves like fputs(3S)
    void GetString(char *s, int n); // Behaves like fgets(3S)
    void PutInt(int n);
    void GetInt(int *n);
  private:
    Lock* lock;                 // Only one user can access the console at a time
    
    Console *console;
  
};

#endif // CONSOLEDRIVER_H


#endif // CHANGED