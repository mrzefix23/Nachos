#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"
#include "machine.h"
class Lock;

class PageProvider {
public:
    PageProvider(int numPages);
    ~PageProvider();

    int GetEmptyPage();
    void ReleasePage(int pageNumber);
    bool ReservePage(int numPage, int* pages);
    int NumAvailPage() const;

private:
    BitMap *pageMap;  // Bitmap pour gérer les pages
    int totalPages;   // TNombre total de pages
    Lock *lock;       // Verrou pour protéger l'accès à la bitmap
};

#endif // PAGEPROVIDER_H
