#include "main.h"
#include "aesthetics.h"
#include "winctrl.h"

#include "postview.h"

int MAX_X, MAX_Y;
WINDOW* boardlist;
WINDOW* commandline;
WINDOW* contentviewer;
struct timespec update_time;
unsigned IBCONS_MSTATE;
char inputbuf[80];
unsigned RUNNING;

ibthread_t* test_thread;


int main(int argc, char** argv)
{
    puts("Quickly testing stuff here.");
    puts("Fetching something.");
    test_thread = malloc(sizeof(ibthread_t));
    ibthread_populate(test_thread, "https://27chan.org/b/res/193195.json");
    
    init_all();

    ibthread_print(contentviewer, test_thread);
    
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

            ibthread_print(contentviewer, test_thread);

            // TODO: fill windows again?
            refresh_all();
        }

        inputhandler();
                
        nanosleep(&update_time, NULL);
    }

    ibthread_dispose(test_thread);
    free(test_thread);

    dispose_all();
    endwin();
    return 0;
}
