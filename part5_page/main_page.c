#include <stdio.h>
#include <math.h>
#include <string.h>

#include "page_manager.h"
#include "config.h"
#include "ui.h"

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];
static int g_tButtonsCnt;

static int MainPageOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;
    char name[100];
    char status[100];
    char *strButton;

    strButton = ptButton->name;

    /* 1 for touch event */
    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        /* 1.1 support touch? */
        if (GetItemCfgByName(ptButton->name)->bCanBeTouched == 0)
            return -1;

        /* 1.2 change button color */
        ptButton->status = !ptButton->status;
        if (ptButton->status)
        {
            dwColor = BUTTON_PRESSED_COLOR;
        }
    }
    /* 2 for net event */
    else if (ptInputEvent->iType == INPUT_TYPE_NET)
    {
        /* 2.1 change button color by str,  */
        sscanf(ptInputEvent->str, "%s %s", name, status);
        if (strcmp(status, "ok") == 0)
            dwColor = BUTTON_PRESSED_COLOR;
        else if (strcmp(status, "err") == 0)
            dwColor = BUTTON_DEFAULT_COLOR;
        else if (status[0] >= '0' && status[0] <= '9')
        {
            dwColor = BUTTON_PERCENT_COLOR;
            strButton = status;
        }
        else
            return -1;
    }
    else
    {
        return -1;
    }

    /* draw */
    DrawRegion(&ptButton->tRegion, dwColor);

    /* write text */
    DrawTextInRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    /* flush to lcd/web */
    FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);

    return 0;
}

static int GetFontSizeForAllButton()
{
    int i;
    int max_len = -1;
    int max_index = 0;
    int len;
    RegionCartesian tRegionCar;
    float k, kx, ky;

    /* find longest Button */
    for (i = 0; i < g_tButtonsCnt; i++)
    {
        len = strlen(g_tButtons[i].name);
        if (len > max_len)
        {
            max_len = len;
            max_index = i;
        }
    }

    /* cal button  */
    SetFontSize(100);
    GetStringRegionCar(g_tButtons[max_index].name, &tRegionCar);

    /* cal font frame */
    kx = (float)g_tButtons[max_index].tRegion.iWidth / tRegionCar.iWidth;
    ky = (float)g_tButtons[max_index].tRegion.iHeigh / tRegionCar.iHeigh;
    if (kx < ky)
        k = kx;
    else
        k = ky;

    /* cal font size */
    return k * 100;
}

static void GenerateButtons()
{
    int width, height;
    int n_per_line;
    int row, rows;
    int col;
    int n, i = 0;
    PDispBuff pDispBuff;
    int xres, yres;
    int start_x, start_y;
    int pre_start_x, pre_start_y;
    PButton ptButton;
    int iFontSize;

    /* cal a singal button size */
    g_tButtonsCnt = n = GetItemCfgCount(); // button num
    pDispBuff = GetDisplayBuffer();        // get fps
    xres = pDispBuff->iXres;
    yres = pDispBuff->iYres;
    width = sqrt(1.0 / 0.618 * (double)(xres * xres / n));
    n_per_line = xres / width + 1;
    width = xres / n_per_line;
    height = 0.618 * width;

    /* cal a singal button region */
    start_x = (xres - width * n_per_line) / 2;
    rows = n / n_per_line;
    if (rows * n_per_line < n)
        rows++;
    start_y = (yres - rows * height) / 2;

    /* cal each button region */
    for (row = 0; (row < rows) && (i < n); row++)
    {
        pre_start_y = start_y + row * height;
        pre_start_x = start_x - width;
        for (col = 0; (col < n_per_line) && (i < n); col++)
        {

            ptButton = &g_tButtons[i];
            ptButton->tRegion.iLeftUpX = pre_start_x + width;
            ptButton->tRegion.iLeftUpY = pre_start_y;
            ptButton->tRegion.iWidth = width - X_GAP;
            ptButton->tRegion.iHeigh = height - Y_GAP;
            pre_start_x = ptButton->tRegion.iLeftUpX;

            /* init button */
            InitButton(ptButton, NULL, GetItemCfgByIndex(i)->name, NULL, MainPageOnPressed);
            i++;
        }
    }

    iFontSize = GetFontSizeForAllButton();
    //SetFontSize(iFontSize);

    /* ondraw */
    for (i = 0; i < n; i++){
        g_tButtons[i].iFontSize = iFontSize;
        g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);
    }
        
}

static int isTouchPointRegion(int ix, int iy, PRegion ptRegion)
{
    if (ix < ptRegion->iLeftUpX || ix > ptRegion->iLeftUpX + ptRegion->iWidth)
        return 0;

    if (iy < ptRegion->iLeftUpY || iy > ptRegion->iLeftUpY + ptRegion->iHeigh)
        return 0;

    return 1;
}

static PButton GetButtonByName(char *name)
{
    int i;

    for (i = 0; i < g_tButtonsCnt; i++)
    {
        if (strcmp(name, g_tButtons[i].name) == 0)
            return &g_tButtons[i];
    }

    return NULL;
}

static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
    int i;
    char name[100];

    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    { // 触摸屏
        for (i = 0; i < g_tButtonsCnt; i++)
        {
            if (isTouchPointRegion(ptInputEvent->iX, ptInputEvent->iY, &g_tButtons[i].tRegion))
                return &g_tButtons[i];
        }
    }
    else if (ptInputEvent->iType == INPUT_TYPE_NET)
    { // 网络事件
        sscanf(ptInputEvent->str, "%s", name);
        return GetButtonByName(name);
    }

    return NULL;
}

static void MainPageRun(void *arg)
{
    int err;
    InputEvent tInputEvent;
    PButton ptButton;
    PDispBuff ptDispBuff = GetDisplayBuffer();

    /* load param */
    err = ParesConfigFile();
    if (err)
    {
        return;
    }

    /* generate button, GUI */
    GenerateButtons();

    while (1)
    {
        /* read task */
        err = GetInputEvent(&tInputEvent);
        if (err)
            continue;

        /* find button */
        ptButton = GetButtonByInputEvent(&tInputEvent);
        if (!ptButton)
            continue;

        /* use onpressed button */
        ptButton->OnPressed(ptButton, ptDispBuff, &tInputEvent);
    }
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run = MainPageRun,
};

void MainPageRegister()
{
    return PageRegister(&g_tMainPage);
}