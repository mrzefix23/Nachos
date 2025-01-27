#include "syscall.h"

// Test de l'appel système PutString
// Fait appel à la fonction PutString plusieurs fois
// avec une chaine de caractères différente dont la taille varie
// pour voir son comportement dans chaque cas

// appel système PutString
// depuis userprog, lancez ./nachos -x ../test/putstring
// doit afficher en sortie:
// Hello
// HelloWorld!
// Machine 
//
// ...

void print(char *c){
    PutString(c);
}

int main(){
    print("Hello");     //test PutString sans dépasser le buffer
    PutChar('\n'); 
    print("HelloWorld!\n"); //test PutString en dépassant le buffer 
    print("Machine \0going down!\n"); //test avec un EOF au milieu de la chaine
    PutChar('\n');
    print("");  //test avec une chaine vide
    Halt();
}