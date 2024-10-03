#include <stdio.h>
#include <string.h>
#include "disp_manager.h"

/* 管理底层LCD,WEB选择 */
static PDispOpr g_DispDevs = NULL;
static PDispOpr g_DispDefault = NULL;
static DispBuff g_tDispBuff; // 储存了显存地址
static int line_witdh;
static int pixel_witdh;

void DrawTextInRegionCentral(char *name, PRegion ptRegion, unsigned int dwColor){
  	int n = strlen(name);
	int iFontSize = ptRegion->iWidth / n / 2;
	FontBitMap tFontBitMap;
    RegionCartesian tRegionCar;

	int iOriginX, iOriginY;
	int i = 0;
	int error;

    /* cal outside frame */
    GetStringRegionCar(name, &tRegionCar);

    /* cal first font region */

	iOriginX = ptRegion->iLeftUpX + (ptRegion->iWidth - tRegionCar.iWidth)/2 - tRegionCar.iLeftUpX;
	iOriginY = ptRegion->iLeftUpY + (ptRegion->iHeigh - tRegionCar.iHeigh)/2 + tRegionCar.iLeftUpY;

    /* draw */
	while (name[i]){
		/* get bitmap */
		tFontBitMap.iCurOriginX = iOriginX;
		tFontBitMap.iCurOriginY = iOriginY;
		error = GetFontBitMap(name[i], &tFontBitMap);
		if (error){
			printf("SelectAndInitFont err\n");
			return;
		}

		/* draw on buffer */		
		DrawFontBitMap(&tFontBitMap, dwColor);		

		iOriginX = tFontBitMap.iNextOriginX;
		iOriginY = tFontBitMap.iNextOriginY;	
		i++;
	}  
}

void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor)
{
    int i, j, p, q;
    int x = ptFontBitMap->tRegion.iLeftUpX;
    int y = ptFontBitMap->tRegion.iLeftUpY;
    int x_max = x + ptFontBitMap->tRegion.iWidth;
    int y_max = y + ptFontBitMap->tRegion.iHeigh;
    int width = ptFontBitMap->tRegion.iWidth;
    unsigned char *buffer = ptFontBitMap->pucBuffer;

    for (j = y, q = 0; j < y_max; j++, q++){
        for (i = x, p = 0; i < x_max; i++, p++){
            if (i < 0 || j < 0 ||
                i >= g_tDispBuff.iXres || j >= g_tDispBuff.iYres)
                continue;

            if (buffer[q * width + p])
                PutPixel(i, j, dwColor);
        }
    }
}

void DrawRegion(PRegion ptRegion, unsigned int dwColor){
    int x = ptRegion->iLeftUpX;
    int y = ptRegion->iLeftUpY;
    int width = ptRegion->iWidth;
    int heigh = ptRegion->iHeigh;

    int i, j;
    for(j = y; j < y + heigh; j++){
        for (i = x; i < x + width; i++)
        {
            PutPixel(x, y, dwColor);
        }
        
    }
}

int PutPixel(int x, int y, unsigned int dwcolor)
{
    unsigned char *pen_8 = (unsigned char *)g_tDispBuff.buff + y * line_witdh + x * pixel_witdh;
    unsigned short *pen_16;
    unsigned int *pen_32;

    unsigned int red, green, blue;

    pen_16 = (unsigned short *)pen_8;
    pen_32 = (unsigned int *)pen_8;

    switch (g_tDispBuff.iBpp)
    {
    case 8:
    {
        *pen_8 = dwcolor;
        break;
    }
    case 16:
    {
        red = (dwcolor >> 16) & 0xff;
        green = (dwcolor >> 8) & 0xff;
        blue = (dwcolor >> 0) & 0xff;
        dwcolor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        *pen_16 = dwcolor;
        break;
    }
    case 32:
    {
        *pen_32 = dwcolor;
        break;
    }
    default:
    {
        printf("can't support %dbpp\n", g_tDispBuff.iBpp);
        return -1;
        break;
    }
    }
    return 0;
}

void RegisterDisplay(PDispOpr pyDispOpr)
{
    pyDispOpr->ptNext = g_DispDevs;
    g_DispDevs = pyDispOpr;
}

/**
 *
 */
int SelectDefaultDisplay(char *name)
{
    PDispOpr pTmp = g_DispDevs;
    while (pTmp)
    {
        if (strcmp(pTmp->name, name) == 0)
        {
            g_DispDefault = pTmp;
            return 0;
        }
        pTmp = pTmp->ptNext;
    }
    return -1;
}

int InitDefaultDisplay()
{
    int ret;

    ret = g_DispDefault->DeviceInit();
    if (ret)
    {
        printf("DeviceInit failed\n");
        return -1;
    }

    ret = g_DispDefault->GetBuffer(&g_tDispBuff);
    if (ret)
    {
        printf("GetBuffer failed\n");
        return -1;
    }

    line_witdh = g_tDispBuff.iXres * g_tDispBuff.iBpp / 8;
    pixel_witdh = g_tDispBuff.iBpp / 8;

    return 0;
}

PDispBuff GetDisplayBuffer()
{
    return &g_tDispBuff;
}

int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
    return g_DispDefault->FlushRegion(ptRegion, ptDispBuff);
}

/**调用底层初始化
 *
 */
void DisplayInit()
{
    extern void FramebufferInit(void);
    FramebufferInit();
    // WebInit();
}