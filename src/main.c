#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

static int MAX_X, MAX_Y;

#define centertext(text) \
    mvprintw(MAX_Y/2, (MAX_X/2) - (strlen(text)/2), text)

int main(int argc, char** argv)
{
    initscr();
    noecho();
    curs_set(FALSE);

    getmaxyx(stdscr, MAX_Y, MAX_X);
    centertext("Hello, world!");
    refresh();

    sleep(1);
    
    endwin();
    return 0;
}
