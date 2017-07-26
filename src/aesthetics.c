#include "main.h"
#include "aesthetics.h"

void centertext(const char* text)
{
    mvprintw(MAX_Y/2, (MAX_X/2) - (strlen(text)/2), text);
}

void wcentertext(WINDOW* w, const char* text)
{
    int y, x;
    getmaxyx(w, y, x);
    mvwprintw(w, y/2, (x/2) - (strlen(text)/2), text);
}

// TODO: Add border flags
void drawborders(WINDOW* w, unsigned flags)
{
    int y, x, i;
    getmaxyx(w, y, x);
    y -= 1;
    x -= 1;

    unsigned bold = (flags & BORDERFLAG_BOLD);

    // Corners
    mvwprintw(w, 0, 0, bold ? "╔" : "┌");
    mvwprintw(w, 0, x, bold ? "╗" : "┐");
    mvwprintw(w, y, 0, bold ? "╚" : "└");
    mvwprintw(w, y, x, bold ? "╝" : "┘");

    // Sides
    for(i = 1; i < y; i++) {
        mvwprintw(w, i, 0, bold ? "║" : "│");
        mvwprintw(w, i, x, bold ? "║" : "│");
    }

    // Bases
    for(i = 1; i <  x; i++)  {
        mvwprintw(w, 0, i, bold ? "═" : "─");
        mvwprintw(w, y, i, bold ? "═" : "─");
    }
}
