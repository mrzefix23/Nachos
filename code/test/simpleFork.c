#include "syscall.h"

// Créer 2 processus exécutant un code simple avec l'appel système PutChar

//pour tester, lancez ./nachos -x ../test/simpleFork depuis userprog
// doit afficher 2 fois 'abcd' puis se terminer

// ce test permet de vérifier avec un exemple simple que les processus sont bien isolés et que les appels systèmes fonctionnent correctement avec les processus


int main(){
    ForkExec("../test/putchar");
    ForkExec("../test/putchar");
}