#ifndef USER_THREAD_H
#define USER_THREAD_H

#ifdef CHANGED
#include "userfork.h"

extern int do_ThreadCreate(int f, int arg, int exitAddr);
void StartUserThread(void *schmurtz);
void do_ThreadExit(void);
void ThreadFunction(void *schmurtz);

struct schmurtzStruct {
    int f;
    int arg;
    int exit;
};

#endif

#endif // USER_THREAD_H