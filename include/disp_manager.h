#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

#include "common.h"
#include "font_manager.h"

typedef struct DispBuff{
    int iXres;
    int iYres;
    int iBpp;
    char *buff;
}DispBuff, *PDispBuff;

typedef struct DispOpr{
    char *name;
    int (*DeviceInit)(void);
    int (*DeviceExit)(void);
    int (*GetBuffer)(PDispBuff ptDispBuff);
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);
    struct DispOpr *ptNext;
}DispOpr, *PDispOpr;

// disp_manager.c
void RegisterDisplay(PDispOpr pyDispOpr);
void DisplaySystemRegister(void);
int SelectDefaultDisplay(char *name);
int InitDefaultDisplay(void);
void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor);
void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwcolor);
void DrawRegion(PRegion ptRegion, unsigned int dwColor);
int PutPixel(int x, int y, unsigned int dwColor);
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff);
PDispBuff GetDisplayBuffer();

#endif