#ifndef IBCONS_POSTVIEW_H
#define IBCONS_POSTVIEW_H

#include <ncurses.h>
#include <panel.h>

// Post data
typedef struct POSTCONTENT_DEF
{
    unsigned long long no;   // number
    char*              sub;  // subject
    char*              com;  // comment
    char*              name; // name
    char*              capcode; // capcode
    unsigned long long time;
    unsigned long      omitted_posts;
    unsigned long      omitted_images;
    unsigned long long replies;
    unsigned           flags;
    // unsigned long long last_modified
    // unsigned tn_h
    // unsigned tn_w
    // unsigned h
    // unsigned w
    unsigned long long fsize;
    char*              filename; 
    char*              ext;
    // char* tim
    // char* md5
    unsigned long long resto; // answers post #
} postcontent_t;

typedef enum POSTCONTENT_FLAGS_DEF
{
    POSTFLAG_IMAGES =  0x0001u,
    POSTFLAG_STICKY =  0x0002u,
    POSTFLAG_LOCKED =  0x0003u,
    POSTFLAG_CYCLIC =  0x0004u
} postflags_e;

// These functions assume an already allocated postcontent_t.
// You'll also need to free() it yourself (not needed when using
// an actual thread)
void post_dispose(postcontent_t* content);


// Thread data
// Linked list for handling thread data
typedef struct IBTHREAD_NODE
{
    postcontent_t         content;
    struct IBTHREAD_NODE* next;
} ibthread_node_t;

typedef struct IBTHREAD_T
{
    ibthread_node_t*   first;
    ibthread_node_t*   last;
    unsigned long long numposts;
} ibthread_t;

// These functions assume an already allocated ibthread_t.
// You'll also need to free() it yourself
void ibthread_init(ibthread_t* thread);
void ibthread_dispose(ibthread_t* thread);
void ibthread_add(ibthread_t* thread, postcontent_t* content);
void ibthread_populate(ibthread_t* thread, const char* uri);
void ibthread_populate_board(ibthread_t* board, const char* uri);

char* ibthread_parse_comment(char* com);
void ibthread_print(WINDOW* w, const ibthread_t* thread);
#endif
