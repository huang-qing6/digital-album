#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include "input_manager.h"

static PInputDevice g_InputDevs = NULL;

static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tConVar = PTHREAD_COND_INITIALIZER;

#define BUFFER_LEN 20
static int g_iRead = 0;
static int g_iWrite = 0;
static InputEvent g_atInputEvents[BUFFER_LEN];

static int isInputBufferFull(){
    return (g_iRead == ((g_iWrite + 1) % BUFFER_LEN));
}

static int isInputBufferEmpty(){
    return (g_iRead == g_iWrite);
}

static void PutInputEventToBuffer(PInputEvent ptInputEvent){
    if(!isInputBufferFull()){
        g_atInputEvents[g_iWrite] = *ptInputEvent;
        g_iWrite = (g_iWrite + 1) % BUFFER_LEN;
    }
}

static int GetInputEventFromBuffer(PInputEvent ptInputEvent){
    if(!isInputBufferEmpty()){
        *ptInputEvent = g_atInputEvents[g_iRead];
        g_iRead = (g_iRead + 1) % BUFFER_LEN;
        return 1;
    }

    return 0;
}

void RegisterInputDevice(PInputDevice pInputDevice){
    pInputDevice->ptNext = g_InputDevs;
    g_InputDevs = pInputDevice;
}

/* init */
void InputSystemRegister(){
    /* register touchscreen */
    extern void TouchscreenRegister(void);
    TouchscreenRegister();
    
    /* register netinput */
    extern void NetinputRegister(void);
    NetinputRegister();
}

static void *input_recv_thread_func(void *arg){
    PInputDevice ptInputDev = (PInputDevice)arg;
    InputEvent tEvent;
    int ret;

    while(1){
        /* read data */
        ret = ptInputDev->GetInputEvent(&tEvent);
        if(!ret){
            /* save data */
            pthread_mutex_lock(&g_tMutex);
            PutInputEventToBuffer(&tEvent);

            /* wake other pthread */
            pthread_cond_signal(&g_tConVar);
            pthread_mutex_unlock(&g_tMutex);            
        }
    }
    return NULL;
}

void InputDeviceInit(){
    int ret;
    pthread_t tid;

    /* for each inputdevice, init and create pthread */
    PInputDevice ptTmp = g_InputDevs;
    while(ptTmp){
        /* init device */
        ret = ptTmp->DeviveInit();

        /* pthread create */
        if(!ret){
            ret = pthread_create(&tid, NULL, input_recv_thread_func, ptTmp);
        }
        
        ptTmp = ptTmp->ptNext;
    }
}

int GetInputEvent(PInputEvent ptInputEvent){
    InputEvent tEvent;
    int ret;
    /* 无数据则休眠 */
    pthread_mutex_lock(&g_tMutex);
    if(GetInputEventFromBuffer(&tEvent)){
        *ptInputEvent = tEvent;
        pthread_mutex_unlock(&g_tMutex);
        return 0;
    } else {
        /* sleep */
        pthread_cond_wait(&g_tConVar, &g_tMutex);
        if(GetInputEventFromBuffer(&tEvent)){
            *ptInputEvent = tEvent;
            ret = 0;
        }else{
            ret = -1;
        }
        pthread_mutex_unlock(&g_tMutex);
        return ret;
    }
    
    /* 返回数据 */
}