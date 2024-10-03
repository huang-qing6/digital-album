#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>

#include "font_manager.h"

static PFontOpr g_ptFonts = NULL;
static PFontOpr g_ptDefaultFontOpr = NULL;

void RegisterFont(PFontOpr ptFontOpr){
    ptFontOpr->ptNext = g_ptFonts;
    g_ptFonts = ptFontOpr;
}

void FontSystemRegister(){
    extern void FreetypeRegister(void);
    FreetypeRegister();
}

int SelectAndInitFont(char *aFontOprName, char *aFontFileName){
    PFontOpr ptTmp = g_ptFonts;
    while(ptTmp){
        if(strcmp(ptTmp->name, aFontOprName) == 0)
            break;
        ptTmp = ptTmp->ptNext;
    }

    if(!ptTmp){
        printf("ptTmp is NULL\n");
        return -1;
    }
        
    
    g_ptDefaultFontOpr = ptTmp;
    return ptTmp->FontInit(aFontFileName);
}

int SetFontSize(int iFontSize){
    return g_ptDefaultFontOpr->SetFontSize(iFontSize);
}

int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap){
    return g_ptDefaultFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}

int GetStringRegionCar(char *str, PRegionCartesian ptRegionCar){
    return g_ptDefaultFontOpr->GetStringRegionCar(str, ptRegionCar);
}