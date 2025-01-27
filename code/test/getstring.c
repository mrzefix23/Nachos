#include "syscall.h"


// Test de l'appel système GetString
// depuis userprog, lancez ./nachos -x ../test/getstring
// attends une chaine de caractères de l'utilisateur et récupère les 2 premiers caractères
// PutString renvoie ensuite les caractères de la chaines

int main(){
    char buffer[3];
    GetString(buffer, 3);
    PutString(buffer);
    PutChar('\n');
    Halt();
    //return 1;
}