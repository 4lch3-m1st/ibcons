#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <time.h>

static int MAX_X, MAX_Y;
static WINDOW* boardlist;
static WINDOW* commandline;
static WINDOW* contentviewer;
static struct timespec update_time;
static unsigned IBCONS_MSTATE;
static char inputbuf[80];
static unsigned RUNNING;

#define BORDERFLAG_BOLD 0x0001u

typedef enum IBCONS_STATE_DEF
{
    STATE_NONE      = 0u,
    STATE_INPUTCOMM = 1u
} IBCONS_STATE;

#define centertext(text) \
    mvprintw(MAX_Y/2, (MAX_X/2) - (strlen(text)/2), text)

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

void clear_all(void)
{
    wclear(stdscr);
    wclear(boardlist);
    wclear(commandline);
    wclear(contentviewer);
}

void refresh_all(void)
{
    // print dummy text
    mvwprintw(boardlist, 1, 1, "[board1 / board2 / board3]");
    mvwprintw(commandline, 1, 1, "> ");
    wcentertext(contentviewer, "Choose a board to start browsing.");
    
    wrefresh(stdscr);
    wrefresh(contentviewer);
    wrefresh(boardlist);
    wrefresh(commandline);
}

void dispose_all(void)
{
    delwin(boardlist);
    delwin(commandline);
}

void inputhandler(void)
{
    if(IBCONS_MSTATE == STATE_NONE) {
        char c = getch();
        switch(c) {
        case 'q':
            RUNNING = FALSE;
            break;
        case '\t':
            IBCONS_MSTATE = STATE_INPUTCOMM; // FALLING THROUGH ON PURPOSE FOR NOW. MIND THIS.
        default: break;
        };
    } else if(IBCONS_MSTATE == STATE_INPUTCOMM) {
        memset(inputbuf, 0, 80 * sizeof(char));
        curs_set(1);
        echo();
        wscanw(commandline, "%s", inputbuf);
        wclear(commandline);
        drawborders(commandline, 0);
        mvwprintw(commandline, 1, 1, "> ");
        wrefresh(commandline);
        noecho();
        curs_set(FALSE);
        IBCONS_MSTATE = STATE_NONE;
    }
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, MAX_Y, MAX_X);
    update_time.tv_sec = 0;
    update_time.tv_nsec = 400000;
    memset(inputbuf, 0, 80 * sizeof(char));

    RUNNING = TRUE;
    IBCONS_MSTATE = STATE_NONE;

    // Create boards window 
    boardlist = newwin(3, MAX_X, 0, 0);
    drawborders(boardlist, BORDERFLAG_BOLD);
    
    commandline = newwin(3, MAX_X, MAX_Y - 3, 0);
    drawborders(commandline, 0);

    contentviewer = newwin(MAX_Y - 6, MAX_X, 3, 0);
    //drawborders(contentviewer, 0);

    refresh_all();

    while(RUNNING) {
        int new_y, new_x;
        getmaxyx(stdscr, new_y, new_x);
        if((new_y != MAX_Y) || (new_x != MAX_X)) {
            // Redraw windows
            MAX_Y = new_y; MAX_X = new_x;
            
            clear_all();

            wresize(boardlist, 3, MAX_X);
            wresize(commandline, 3, MAX_X);
            wresize(contentviewer, MAX_Y - 6, MAX_X);
            mvwin(commandline, MAX_Y - 3, 0);

            drawborders(boardlist, BORDERFLAG_BOLD);
            drawborders(commandline, 0);
            //drawborders(contentviewer, 0);

            // TODO: fill windows again?
            refresh_all();
        }

        inputhandler();
                
        nanosleep(&update_time, NULL);
    }

    dispose_all();
    endwin();
    return 0;
}
