#include "syscall.h"

// Test de l'appel système GetChar

// depuis userprog, lancez ./nachos -x ../test/getchar
// attends un caractère de l'utilisateur et l'affiche ensuite avec PutChar
// quitte le test avec q ou CTRL+D (EOF)

int main(){
    while (1){
        char c = GetChar();
        if (c == -1 || c == 'q'){   //EOF
            break;
        }
        PutChar(c);
    }
    Halt();
    //return 1;
}