#include "winctrl.h"
#include "aesthetics.h"

void init_all(void)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(FALSE);
    getmaxyx(stdscr, MAX_Y, MAX_X);
    update_time.tv_sec = 0;
    update_time.tv_nsec = 400000;
    memset(inputbuf, 0, 80 * sizeof(char));
    inputbuf[0] = '\0';

    RUNNING = TRUE;
    IBCONS_MSTATE = STATE_NONE;

    // Create boards window 
    boardlist = newwin(3, MAX_X, 0, 0);
    drawborders(boardlist, BORDERFLAG_BOLD);
    
    commandline = newwin(3, MAX_X, MAX_Y - 3, 0);
    drawborders(commandline, 0);

    contentviewer = newwin(MAX_Y - 6, MAX_X, 3, 0);
    scrollok(contentviewer, TRUE);
    //drawborders(contentviewer, 0);
}

void dispose_all(void)
{
    delwin(boardlist);
    delwin(commandline);
    delwin(contentviewer);
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
    //wcentertext(contentviewer, "Choose a board to start browsing.");
    
    wrefresh(stdscr);
    wrefresh(contentviewer);
    wrefresh(boardlist);
    wrefresh(commandline);
}

void inputhandler(void)
{
    if(IBCONS_MSTATE == STATE_NONE) {
        if(strcmp(inputbuf, ""));
        
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
