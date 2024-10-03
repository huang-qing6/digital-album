#ifndef _UI_H
#define _UI_H

#include "common.h"
#include "disp_manager.h"
#include "input_manager.h"

#define BUTTON_DEFAULT_COLOR 0Xff0000
#define BUTTON_PRESSED_COLOR 0X00ff00
#define BUTTON_PERCENT_COLOR 0X0000ff
#define BUTTON_TEXT_COLOR    0X000000

struct Button;

typedef int (*ONDRAW_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff);
typedef int (*ONPRESSED_FUNC)(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent);

typedef struct Button
{
    char *name;
    int iFontSize;    
    int status;
    Region tRegion;
    ONDRAW_FUNC OnDraw;
    ONPRESSED_FUNC OnPressed;
}Button, *PButton;

// ui.c
void InitButton(PButton ptButton, PRegion ptRegion, char *name, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed);

#endif