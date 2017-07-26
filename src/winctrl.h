#ifndef IBCONS_WINCTRL_H
#define IBCONS_WINCTRL_H

#include "main.h"

typedef struct POSTVIEW_DEF
{
    PANEL* panel;
} postview_t;

typedef enum IBCONS_STATE_DEF
{
    STATE_NONE      = 0u,
    STATE_INPUTCOMM = 1u
} IBCONS_STATE;


void init_all(void);
void dispose_all(void);
void clear_all(void);
void refresh_all(void);
void inputhandler(void);

#endif
