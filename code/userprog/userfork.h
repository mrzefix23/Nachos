#ifndef USER_FORK_H
#define USER_FORK_H

#ifdef CHANGED

extern int do_ForkExec(const char* s);
void StartUserProg(void *schmurtz);
void do_ProcessExit();

#endif

#endif // USER_FORK_H