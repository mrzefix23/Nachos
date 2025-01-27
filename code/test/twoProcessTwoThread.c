#include "syscall.h"

// Créer 2 processus chacun avec 2 threads

//pour tester, lancez ./nachos -x ../test/twoProcessTwoThread depuis userprog
// doit afficher une série d'une même lettre débutant par un 'x' pour chaque thread

// ce test permet de vérifier que les threads fonctionnent correctement avec des processus différents et que les processus sont bien isolés



int main(){
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
}