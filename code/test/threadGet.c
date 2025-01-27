#include "syscall.h"

// Teste l'utilisation de thread avec GetChar et GetString

// pour tester, lancez ./nachos -x ../test/threadGet depuis userprog
// doit afficher en sortie le caractère et la chaine de caractère entrés par l'utilisateur

// on peut utiliser l'option -rs avec un seed pour avoir un ordre d'affichage aléatoire
// ./nachos -rs 1234 -x ../test/threadsArgument

// ce test permet de vérifier que les appels système GetChar et GetString fonctionnent correctement
// et qu'il n'y ait pas de conflit entre les threads et que les appels système fonctionnent correctement avec les threads
// vérifie aussi que les débordements de buffer sont correctement gérés

void ThreadGetChar(){
    char c = GetChar();
    PutChar(c);
    //ThreadExit();
}

void ThreadGetStr(){
    char buffer[10];
    GetString(buffer, 10);
    PutString(buffer);
    //ThreadExit();
}

int main() {
    ThreadCreate(ThreadGetChar,0);
    ThreadCreate(ThreadGetStr,0); 
    ThreadExit(); // on attend que tous les threads se terminent
    return 0; // termine directement le processus
}
