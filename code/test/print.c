#include "syscall.h"

/* 

** executer le programme print.c avec la commande suivante:
** ./nachos -x ../test/print
** doit afficher en sortie "Test d'adressage virtuel par table de pages."

*/

int main() {
    // ecriture d'un message simple à l'écran
    PutString("Test d'adressage virtuel par table de pages.\n");
    return 0;
}
