#ifndef IBCONS_POSTVIEW_H
#define IBCONS_POSTVIEW_H

#include <panel.h>

typedef struct POSTCONTENT_DEF
{
    unsigned long long no;   // number
    char*              sub;  // subject
    char*              com;  // comment
    char*              name; // name
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
    // unsigned long long fsize
    // char* filename
    // char* ext
    // char* tim
    // char* md5
    unsigned long long resto; // answers post #
} postcontent_t;

typedef enum POSTCONTENT_FLAGS_DEF
{
    POSTFLAG_IMAGES =  0x0001u,
    POSTFLAG_STICKY =  0x0002u,
    POSTFLAG_LOCKED =  0x0003u,
    POSTFLAG_CYCLYC =  0x0004u
} postflags_e;


typedef struct POSTVIEW_DEF
{
    PANEL*         panel;
    postcontent_t* content;
} postview_t;

// TODO: post list

void dispose_post(postcontent_t*);

#endif
