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

ibthread_t* thread;


int main(int argc, char** argv)
{
    puts("Quickly testing stuff here.");
    puts("Fetching something.");
    thread = malloc(sizeof(ibthread_t));
    //ibthread_populate(thread, "https://27chan.org/b/res/166189.json");
    ibthread_populate(thread, "https://27chan.org/b/res/167124.json");
    
    init_all();

    // Print ONE post!
    unsigned char_y = 2,
        char_y_begin;
    ibthread_node_t* post;
    for(post = thread->first; post != NULL; post = post->next)
    {
        char_y_begin = char_y;
        int y, x;
        unsigned xcaret = 1;
        getmaxyx(contentviewer, y, x);

        // Post header
        if(post->content.sub) {
            mvwprintw(contentviewer, char_y, xcaret, "%s", post->content.sub);
            xcaret += strlen(post->content.sub);
            mvwprintw(contentviewer, char_y, xcaret, " - "); xcaret += 3;
        }
        mvwprintw(contentviewer, char_y, xcaret, "%s", post->content.name);
        xcaret += strlen(post->content.name) - 1;
        mvwprintw(contentviewer, char_y, xcaret, ", No. "); xcaret += 6;
        mvwprintw(contentviewer, char_y, xcaret, "%llu", post->content.no);
        char_y++;

        // Comments begin on line 2. Images too.
        // Images are 8x18 characters wide.
        size_t currchar = 0;
        unsigned char_return = post->content.fsize ? 20 : 1;
        unsigned char_x = char_return;
        for(currchar = 0; currchar < strlen(post->content.com); currchar++) {
            mvwaddch(contentviewer, char_y, char_x, post->content.com[currchar]);
            char_x++;
            if(char_x >= x - 2) {
                char_y++;
                char_x = char_return;
            }
            if(char_y_begin + 1 == 9) char_return = 1;
        }

        // Separator
        for(currchar = 1; currchar < x - 2; currchar++)
            mvwaddch(contentviewer, (char_y < 10 ? 10 : char_y + 1), currchar, '-');
        
        char_y += (char_y < 10) ? (12 - char_y) : 3;
    }
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

    ibthread_dispose(thread);
    free(thread);

    dispose_all();
    endwin();
    return 0;
}
