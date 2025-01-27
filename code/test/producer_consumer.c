/*#include "syscall.h"

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Déclarations des sémaphores
sem_t empty;
sem_t full;
sem_t mutex;

// Fonction Producteur
void Producer() {
    int item = 1; 
    while (1) {
        P(&empty);     // attendre
        P(&mutex);     // entrer dans la section critique

        // inserer l'item
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        PutString("Item produit : "); 
        PutInt(item);
        PutChar('\\n');

        V(&mutex);     // sortir de la section critique
        V(&full);      // signal

        item++;
    }
}


void Consumer() {
    while (1) {
        P(&full);      // attendre
        P(&mutex);     // entrer dans section critique

        // retirer l'item 
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        PutString("Item consommé : "); 
        PutInt(item);
        PutChar('\\n');

        V(&mutex);     // sortir de la section critique
        V(&empty);     // signal
    }
}

int main() {
    sem_init(&empty, BUFFER_SIZE);
    sem_init(&full, 0);           
    sem_init(&mutex, 1);         

    // créer les threads producteur et consommateur
    ThreadCreate(Producer, 0);
    ThreadCreate(Consumer, 0);

    ThreadExit(); // Terminer le thread principal
    return 0;
}
*/

int main() {
    return 0;
}