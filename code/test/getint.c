#include "syscall.h"

// Test de l'appel système GetInt
// depuis userprog, lancez ./nachos -x ../test/getint
// attends une chaine de caractères de l'utilisateur et l'affiche ensuite avec PutInt

int main(){
    int i;
    GetInt(&i);
    PutInt(i);
    PutChar('\n');
    Halt();
    //return 1;
}