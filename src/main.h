#ifndef IBCONS_MAIN_H
#define IBCONS_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <time.h>

extern int MAX_X, MAX_Y;
extern WINDOW* boardlist;
extern WINDOW* commandline;
extern WINDOW* contentviewer;
extern struct timespec update_time;
extern unsigned IBCONS_MSTATE;
extern char inputbuf[80];
extern unsigned RUNNING;




#endif

