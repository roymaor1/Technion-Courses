#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <iostream>
#include "commands.h"
using namespace std;

int SIGNALS_setSignalHandlers(void);
bool SIGNALS_kill(pid_t pid, int signum);
void SIGNALS_sigChldHandler(int sig_num);
void SIGNALS_waitpid(list<Job>::iterator j, int options);
void catch_tstp(int sig_num);
void catch_int(int sig_num);

#endif

