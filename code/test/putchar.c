#include "syscall.h"

//Test de l'appel système PutChar

// depuis userprog, lancez ./nachos -x ../test/putchar

// doit afficher en sortie:
// abcd
// Machine going down!
// ...

void print(char c, int n){
    int i;
    //#if 0
    for (i = 0; i < n; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
    //#endif
}

int main(){
    print('a',4);
    //while(1);
    //Halt();
    return 5;
    
}