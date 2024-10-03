#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include "common.h"

typedef struct FontBitMap{ // 支持freetype / 固定大小 的字符
    Region tRegion;
    int iCurOriginX;
    int iCurOriginY;
    int iNextOriginX;
    int iNextOriginY;    
    unsigned char *pucBuffer; 
}FontBitMap, *PFontBitMap;

typedef struct FontOpr{
    char *name;
    int (*FontInit)(char *aFileName);
    int (*SetFontSize)(int iFontSize);
    int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);
    int (*GetStringRegionCar)(char *str, PRegionCartesian ptRegionCar);
    struct FontOpr *ptNext;
}FontOpr, *PFontOpr;

// font_manager.c
void RegisterFont(PFontOpr g_ptFonts);
void FontsRegister(void);
int SelectAndInitFont(char *aFontOprName, char *aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);
int GetStringRegionCar(char *str, PRegionCartesian ptRegionCar);

#endif