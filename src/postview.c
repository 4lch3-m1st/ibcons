#include "postview.h"
#include "networking.h"
#include <jansson.h>
#include <string.h>
#include <assert.h>
#include <ncurses.h>

#define free_existing(ptr) \
    if(ptr) free(ptr)

void post_dispose(postcontent_t* content)
{
    free_existing(content->sub);
    free_existing(content->com);
    free_existing(content->name);
    free_existing(content->filename);
    free_existing(content->ext);
    free_existing(content->capcode);
    // free(md5);
}


void ibthread_init(ibthread_t* thread)
{
    assert(thread != NULL);
    thread->first = thread->last = NULL;
    thread->numposts = 0;
}

void ibthread_dispose(ibthread_t* thread)
{
    assert(thread != NULL);
    while(thread->first != NULL) {
        ibthread_node_t* aux = thread->first;
        thread->first = thread->first->next;
        post_dispose(&aux->content);
        free(aux);
    }
}

void ibthread_add(ibthread_t* thread, postcontent_t* content)
{
    assert(thread != NULL);
    if(!thread->first) {
        thread->first = malloc(sizeof(ibthread_node_t));
        thread->last = thread->first;
        thread->first->next = NULL;
        memcpy(&thread->first->content, content, sizeof(postcontent_t));
        thread->numposts = 1llu;
        return;
    }
    ibthread_node_t* aux = malloc(sizeof(ibthread_node_t));
    memcpy(&aux->content, content, sizeof(postcontent_t));
    thread->last->next = aux;
    thread->last = aux;
    aux->next = NULL;
    thread->numposts++;
}

void ibthread_populate(ibthread_t* thread, const char* uri)
{
    json_t*      root;
    json_error_t error;
    char*        jsonfile;

    jsonfile = curl_request(uri);
    if(!jsonfile) {
        // Handle error here. Couldn't fetch JSON file.
        return;
    }

    root = json_loads(jsonfile, 0, &error);
    free(jsonfile);
    if(!root) {
        // Handle error here. Couldn't load JSON file
        return;
    }

    // Here's how it works.
    // The posts are the same whether you're dealing with
    // a board or a thread.

    // When dealing with a board, you'll receive an object
    // with a field called "threads". Each "thread" object
    // will have an array of "posts", which are something
    // like an overview. We only care about OP, though.
    // Also, all of these posts carry a number of replies.

    // When dealing with a thread, you'll receive an object
    // with a field called "posts", which will contain an array
    // of posts. First one being OP, next ones being anons.

    // At this function, we'll only be collecting posts from a
    // single thread, not a board. We can de-simplify stuff later.

    // 1. We're receiving an object "posts".
    if(!json_is_object(root)) {
        // Handle error
        json_decref(root);
        return;
    }

    // 2. "posts" is an array of actual objects.
    const json_t* posts;
    posts = json_object_get(root, "posts");
    if(!json_is_array(posts)) {
        // Handle
        json_decref(root);
        return;
    }

    // 3. We now loop through it and fetch info. We'll keep adding it
    // to our thread.
    ibthread_init(thread);
    {
        json_t* current_post,
            *current_field;
        size_t numposts = json_array_size(posts),
            i;
        for(i = 0; i < numposts; i++) {
            current_post = json_array_get(posts, i);
            // TODO: Check if we're actually dealing with
            // an OBJECT!
            // I suppose we don't need to do that at this
            // point, but who knows.
            postcontent_t content;
            memset(&content, 0, sizeof(content));

            // General values
            current_field = json_object_get(current_post, "no"); //  Always
            content.no = json_integer_value(current_field);

            current_field = json_object_get(current_post, "sub"); // Optional
            if(current_field)
                content.sub   = strdup(json_string_value(current_field));

            current_field = json_object_get(current_post, "com"); // Always??
            if(current_field) {
                content.com   = strdup(json_string_value(current_field));
                // Tidy up HTML tags
                
            }

            current_field = json_object_get(current_post, "name"); // Always
            content.name  = strdup(json_string_value(current_field));

            current_field = json_object_get(current_post, "capcode"); // Sometimes
            if(current_field)
                content.capcode = strdup(json_string_value(current_field));

            current_field = json_object_get(current_post, "time"); // Always
            content.time  = json_integer_value(current_field);

            current_field = json_object_get(current_post, "omitted_posts"); // OP only
            if(current_field)
                content.omitted_posts = json_integer_value(current_field);

            current_field = json_object_get(current_post, "omitted_images"); // OP only
            if(current_field)
                content.omitted_images = json_integer_value(current_field);

            // Flags
            current_field = json_object_get(current_post, "sticky"); // Always
            content.flags |= json_integer_value(current_field) ? POSTFLAG_STICKY : 0u;

            current_field = json_object_get(current_post, "locked"); // Always
            content.flags |= json_integer_value(current_field) ? POSTFLAG_STICKY : 0u;

            current_field = json_object_get(current_post, "cyclical"); // Always
            content.flags |= strcmp(json_string_value(current_field), "0")
                ? 0u : POSTFLAG_CYCLIC;


            // Attachment. These fields only appear if there are attachments.
            current_field = json_object_get(current_post, "filename");
            if(current_field) {
                content.filename = strdup(json_string_value(current_field));
                // File size
                current_field = json_object_get(current_post, "fsize");
                content.fsize = json_integer_value(current_field);
                // Extension
                current_field = json_object_get(current_post, "ext");
                content.ext   = strdup(json_string_value(current_field));
            }

            // Replies to...
            current_field = json_object_get(current_post, "resto");
            content.resto = json_integer_value(current_field);

            // Thread-only stuff
            // Those are only needed for threads. Leaving for future purposes
            //current_field = json_object_get(current_post, "replies");
            //content.replies = json_integer_value(current_field);
            
            //current_field = json_object_get(current_post, "images");
            //content.flags |= json_integer_value(current_field) ? POSTFLAG_IMAGES : 0u;


            // And now, add the post to the list.
            ibthread_add(thread, &content);
        }
    }
}

void ibthread_populate_board(ibthread_t* board, const char* uri)
{
}

void ibthread_print(WINDOW* w, const ibthread_t* thread)
{
    unsigned char_y = 2, char_y_begin;
    ibthread_node_t* post;
    
    for(post = thread->first; post != NULL; post = post->next)
    {
        char_y_begin = char_y;
        int y, x;
        unsigned xcaret = 1;
        getmaxyx(w, y, x);

        // Post header
        if(post->content.sub) {
            mvwprintw(w, char_y, xcaret, "%s", post->content.sub);
            xcaret += strlen(post->content.sub);
            mvwprintw(w, char_y, xcaret, " - "); xcaret += 3;
        }
        // Name
        mvwprintw(w, char_y, xcaret, "%s", post->content.name);
        xcaret += strlen(post->content.name);

        // Capcode, if existing
        if(post->content.capcode) {
            mvwprintw(w, char_y, xcaret, " ## %s", post->content.capcode);
            xcaret += strlen(post->content.capcode) + 4;
        }
        
        mvwprintw(w, char_y, xcaret, " No. "); xcaret += 5;
        mvwprintw(w, char_y, xcaret, "%llu", post->content.no);
        char_y++;

        // Comments begin on line 2. Images too.
        // Images are 8x18 characters wide.
        size_t currchar = 0;
        unsigned char_return = post->content.fsize ? 20 : 1;
        unsigned char_x = char_return;
        if(post->content.com) {
            for(currchar = 0; currchar < strlen(post->content.com); currchar++) {
                // Halt everything if we reached the end of the window (for now)
                if(char_y == y - 1)
                    return;
            
                char current = post->content.com[currchar];
                if(current == '\n') {
                    char_y++;
                    char_x = char_return;
                } else {
                    // Temporary unicode fix
                    if(current > 255 || current < 0)
                         current = ' ';
                    
                    mvwaddch(w, char_y, char_x, current);
                    char_x++;
                    if(char_x >= x - 2) {
                        char_y++;
                        char_x = char_return;
                    }
                    if(char_y_begin + 1 == 9) char_return = 1;
                }
            }

            if(post->content.fsize) {
                // Separator
                for(currchar = 1; currchar < x - 2; currchar++)
                    mvwaddch(w, (char_y < (char_y_begin + 10)
                                 ? (char_y_begin + 10) : char_y + 1), currchar, '-');
        
                char_y += (char_y < (char_y_begin + 10)) ? ((char_y_begin + 12) - char_y) : 3;
            } else {
                for(currchar = 1; currchar < x - 2; currchar++)
                    mvwaddch(w, char_y + 1, currchar, '-');
        
                char_y += 3;
            }
        }
    }
}

char* ibthread_parse_comment(char* com)
{
    if(!com) return NULL;
    return com;
}
