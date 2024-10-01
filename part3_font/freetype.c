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
#include <ft2build.h>
#include <freetype/freetype.h>

#include "font_manager.h"

static FT_Face g_tFace;
static int g_iDefaultFontSize;

static int FreeTypeFontInit(char *aFileName){
    FT_Library library;
    int ret;

    ret = FT_Init_FreeType(&library);
    if(ret){
        printf("FT_Init_FreeType err\n");
        return -1;
    }
    ret = FT_New_Face(library, aFileName, 0, &g_tFace);
    if(ret){
        printf("FT_New_Face err\n");
        return -1;        
    }

    FT_Set_Pixel_Sizes(g_tFace, g_iDefaultFontSize, 0);

    return 0;
}

static int FreeTypeSetFontSize(int iFontSize){
    FT_Set_Pixel_Sizes(g_tFace, iFontSize, 0);
    return 0;
}

static int FreeTypeGetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap){
    int ret;
    FT_Vector pen;
    FT_GlyphSlot slot = g_tFace->glyph;

    pen.x = ptFontBitMap->iCurOriginX * 64; /* 单位1/64像素 */
    pen.x = ptFontBitMap->iCurOriginY * 64; /* 单位1/64像素 */

    /* trans */
    FT_Set_Transform(g_tFace, 0, &pen);

    /* 加载位图 */
    ret = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);
    if(ret){
        printf("FT_Load_Char err\n");
        return -1;        
    }

    /* 初始化 */
    ptFontBitMap->pucBuffer = slot->bitmap.buffer;
    
	ptFontBitMap->tRegion.iLeftUpX = slot->bitmap_left;
	ptFontBitMap->tRegion.iLeftUpY = ptFontBitMap->iCurOriginY*2 - slot->bitmap_top;
	ptFontBitMap->tRegion.iWidth = slot->bitmap.width;
	ptFontBitMap->tRegion.iHeigh = slot->bitmap.rows;
	ptFontBitMap->iNextOriginX = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
	ptFontBitMap->iNextOriginY = ptFontBitMap->iCurOriginY;

    return 0;
}

static FontOpr g_tFreetypeOpr = {
    .name = "freetype",
    .FontInit = FreeTypeFontInit,
    .SetFontSize = FreeTypeSetFontSize,
    .GetFontBitMap = FreeTypeGetFontBitMap,
};

void FreetypeRegister(){
    RegisterFont(&g_tFreetypeOpr);
}