#include <tslib.h>
#include <stdio.h>

#include "input_manager.h"

static struct tsdev *g_ts;

static int TouchscreenGetInputEvent(PInputEvent ptInputEvent){
    struct ts_sample samp;
    int ret;

    ret = ts_read(g_ts, &samp, 1);
    if(!ret){
        return -1;
    }

    ptInputEvent->iType = INPUT_TYPE_TOUCH; // 触摸屏模式
    ptInputEvent->iX = samp.x;
    ptInputEvent->iY = samp.y;
    ptInputEvent->iPresure = samp.pressure;
    ptInputEvent->tTime = samp.tv;

    return 0;
}

static int TouchscreenDeviveInit(void){
    g_ts = ts_setup(NULL, 0);
    if(!g_ts){
        printf("ts_setup err\n");
        return -1;
    }
    return 0;
}

static int TouchscreenDeviveExit(void){
    ts_close(g_ts);
    return 0;
}

static InputDevice g_tTouchscreenDev ={
    .name = "touchscreen",
    .GetInputEvent = TouchscreenGetInputEvent,
    .DeviveInit = TouchscreenDeviveInit,
    .DeviveExit = TouchscreenDeviveExit,
};


void TouchscreenRegister(){
    RegisterInputDevice(&g_tTouchscreenDev); 
}


/*
#if 1

int main(int argc, char **argv){
    InputEvent event;
    int ret;

    g_tTouchscreenDev.DeviveInit();

    while(1){
        ret = g_tTouchscreenDev.GetInputEvent(&event);
        if(ret){
            printf("getinputevent err\n");
            return -1;
        } else {
            printf("type     : %d\n", event.iType);
            printf("index x  : %d\n", event.iX);
            printf("index y  : %d\n", event.iY);
            printf("pressure : %d\n", event.iPresure);
        }
    }

}

#endif
*/