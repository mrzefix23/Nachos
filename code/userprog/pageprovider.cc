#include "pageprovider.h"
#include <cstring>  // memset
#include "system.h"

PageProvider::PageProvider(int numPages) {
    totalPages = numPages; // nombre total de pages
    pageMap = new BitMap(totalPages);  // initialiser la bitmap
    lock = new Lock("PageProvider lock");
}

PageProvider::~PageProvider() {
    delete pageMap;
    delete lock;
}

int PageProvider::GetEmptyPage() {
    int page = pageMap->Find();  // trouver une page libre (vide)
    if (page != -1) { 
        memset(&machine->mainMemory[page * PageSize], 0, PageSize); // initialiser la page à 0
    }
    return page; // retourner le numéro de la page ou -1 si aucune page n'est trouvée
}

void PageProvider::ReleasePage(int pageNumber) {
    pageMap->Clear(pageNumber);  // libérer la page
}

// Réserver un certain nombre de pages
// Retourne vrai si le nombre de pages est réservé avec succès, faux sinon
// Si la réservation échoue, les pages déjà réservées sont libérées
bool PageProvider::ReservePage(int numPages, int* pages) {
    lock->Acquire();
    if(numPages > NumAvailPage()){
        lock->Release();
        return false;
    }
    for(int i = 0; i < numPages; i++){
        pages[i] = GetEmptyPage();
        if(pages[i] == -1){
            for(int j = 0; j <= i; j++){
                ReleasePage(pages[j]);
            }
            lock->Release();
            return false;
        }
    }
    lock->Release();
    return true;
}


int PageProvider::NumAvailPage() const {
    return pageMap->NumClear();  // retourner le nombre de pages libres
}
