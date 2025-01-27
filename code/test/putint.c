#include "syscall.h"

//Test de l'appel système PutInt

// depuis userprog, lancez ./nachos -x ../test/putint

// doit afficher en sortie:
// 1234
// Machine going down!
// ...

void print(int c, int n){
    int i;
    //#if 0
    for (i = 0; i < n; i++) {
        PutInt(c + i);
        PutChar('\n');
    }
    PutChar('\n');
    //#endif
}

int main(){
    print(1,4);
    Halt();
}