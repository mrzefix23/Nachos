#include "syscall.h"

#define THIS "aaa"
#define THAT "bbb"

const int N = 10; // Choose it large enough!

// Fonction pour afficher une chaîne de caractères
void puts(const char *s) {
    const char *p;
    for (p = s; *p != '\0'; p++) {
        PutChar(*p);
    }
}

// Fonction exécutée par les threads
void f(void *arg) {
    const char *s = arg;
    int i;

    PutChar('x'); // Affiche 'x' au début de l'exécution
    for (i = 0; i < N; i++) {
        puts(s); // Affiche la chaîne de caractères passée en argument
    }
   
}

// Point d'entrée principal
int main() {
    ThreadCreate(f, THIS); // Crée un thread exécutant la fonction f avec THIS comme argument
    f(THAT);               // Exécute la fonction f dans le thread principal avec THAT comme argument
}
