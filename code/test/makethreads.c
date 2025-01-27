#include "syscall.h"

// Teste les threads avec un certain nombre de threads affichant un caractère
// pour tester, lancez ./nachos -x ../test/makethreads depuis userprog
// doit afficher en sortie des 'a'

// on peut utiliser l'option -rs avec un seed pour avoir un ordre d'affichage aléatoire
// ./nachos -rs 1234 -x ../test/makethreads

// ce test permet de vérifier que les threads fonctionnent correctement et que plusieurs threads peuvent être créés
// permet de vérifier que la pile de threads est correctement gérée peu importe le nombre de threads
// donc qu'il doit afficher NumberOfThreads * 10 'a'


#define NumberOfThreads 12

void ThreadFunction() {
    volatile int a;  
    for (a = 0; a < 10; a++) {  
        PutChar('a');
    }
}

int main() {
    int i;
    // Créer plusieurs threads
    for (i = 0; i < NumberOfThreads; i++) {
        ThreadCreate(ThreadFunction, 0); 
    }
}
