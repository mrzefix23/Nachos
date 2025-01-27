#include "syscall.h"

// Teste la création de threads avec argument
// Chaque thread affiche un entier correspondant à son numéro et un dernier thread affiche un caractère

// pour tester, lancez ./nachos -x ../test/threadsArgument depuis userprog
// doit afficher en sortie les entiers de 0 à 9 et un caractère 'a'

// on peut utiliser l'option -rs avec un seed pour avoir un ordre d'affichage aléatoire
// ./nachos -rs 1234 -x ../test/threadsArgument

// ce test permet de vérifier que les arguments passés aux threads sont bien transmis et peuvent être utilisés
// par les threads


// fonction exécutée par les threads affichant leur numéro
void ThreadFunctionWithInt(void *arg) {
    int n = (int)arg;
    PutInt(n);
    PutChar('\n');
    //ThreadExit();
}

// fonction exécutée par les threads affichant un caractère passé en argument
void ThreadFunctionWithChar(void *arg) {
    char c = *(char *)arg;
    PutChar(c);
    PutChar('\n');
    //ThreadExit();
}


int main() {
    int i; char c;
    // Créer plusieurs threads
    for(i = 0; i < 10; i++)
        ThreadCreate(ThreadFunctionWithInt, (void *)i); 
    
    c = 'a';
    ThreadCreate(ThreadFunctionWithChar, (void *)&c);
    ThreadExit(); // on attend que tous les threads se terminent

    return 0;
}
