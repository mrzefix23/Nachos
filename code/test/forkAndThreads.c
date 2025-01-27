#include "syscall.h"

// Créer aux maximum 12 processus exécutant chacun le programme makethreads
// dépendant du nombre de processus que le système peut gérer en simultané
// pour tester, lancez ./nachos -x ../test/forkAndThreads depuis userprog
// chaque processus créé exécute le programme makethreads qui crée 12 threads affichant 10 fois la lettre a

// ce test permet de vérifier qu'il est possible de créer plusieurs processus en parallèle avec un grand nombre de threads

int main(){
    int i;
    for(i = 0; i < 12; i++){
        ForkExec("../test/makethreads");   
    }
    return 0;
}
