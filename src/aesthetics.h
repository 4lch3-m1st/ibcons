#ifndef IBCONS_AESTHETICS_H
#define IBCONS_AESTHETICS_H

#include <ncurses.h>
#include <string.h>

#define BORDERFLAG_BOLD 0x0001u

void centertext(const char* text);
void wcentertext(WINDOW* w, const char* text);
void drawborders(WINDOW* w, unsigned flags);

#endif
