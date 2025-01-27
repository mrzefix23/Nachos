#include "syscall.h"

// Teste les threads avec un certain nombre de threads affichant un caractère
// pour tester, lancez ./nachos -x ../test/threadAndExit depuis userprog
// doit afficher en sortie des 'a' puis se terminer dès que un thread se termine

// on peut utiliser l'option -rs avec un seed pour avoir un ordre d'affichage aléatoire
// ./nachos -rs 1234 -x ../test/makethreads

// à tester surtout avec valgrind pour vérifier que la mémoire est correctement libérée
// valgrind ./nachos -x ../test/threadAndExit

// ce test permet de vérifier que lorsqu'un thread d'un processus appelle Exit, le processus se termine correctement en terminant tous les threads proprement et en libérant les ressources
// doit afficher à minima 5 'a' puis le programme se termine


#define NumberOfThreads 3

void ThreadFunction() {
    volatile int a;  
    for (a = 0; a < 5; a++) {  
        PutChar('a');
    } 
    Exit(0);        // Test exit with threads
}

int main() {
    int i;
    // Créer plusieurs threads
    for (i = 0; i < NumberOfThreads; i++) {
        ThreadCreate(ThreadFunction, 0); 
    }
}
