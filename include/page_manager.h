#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

#include "common.h"

typedef struct PageAction{
    char *name;
    void (*Run)(void *arg);
    struct PageAction *ptNext;
}PageAction, *PPageAction;

// page_manager.c
void PageRegister(PPageAction ptPageAction);
PPageAction Page(char *name);
void PagesRegister(void);

#endif