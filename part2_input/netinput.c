#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#include "input_manager.h"

#define SERVER_PORT 8888

static int g_iSocketServer;

static int NetInputGetInputEvent(PInputEvent ptInputEvent){
    int iRecvLen;
    struct sockaddr_in tSocketClientAddr;
    unsigned int iAddrLen = sizeof(struct sockaddr);
    char aRecvBuf[1000];

    iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
    if(iRecvLen > 0){
        aRecvBuf[iRecvLen] = '\0';
        ptInputEvent->iType = INPUT_TYPE_NET; // 网络模式
        gettimeofday(&ptInputEvent->tTime, NULL);
        strncpy(ptInputEvent->str, aRecvBuf, 1000);  
        ptInputEvent->str[999] = '\0';  
        return 0;
    }
    return -1;
}

static int NetInputDeviveInit(void){
    struct sockaddr_in tSocketServerAddr;
    int iRet;

    g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if(g_iSocketServer == -1){
        printf("socket err\n");
        return -1;
    }

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(SERVER_PORT);
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;

    iRet = bind(g_iSocketServer, (struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if(iRet == -1){
        printf("bind err\n");
        return -1;
    }

    return 0;
}

static int NetInputDeviveExit(void){
    close(g_iSocketServer);
    return 0;
}

static InputDevice g_tNetInputDev ={
    .name = "NetInput",
    .GetInputEvent = NetInputGetInputEvent,
    .DeviveInit = NetInputDeviveInit,
    .DeviveExit = NetInputDeviveExit,
};

void NetinputRegister(){
    RegisterInputDevice(&g_tNetInputDev); 
}

/*
#if 1
int main(int argc, char **argv){
    InputEvent event;
    int ret;

    g_tNetInputDev.DeviveInit();

    while(1){
        ret = g_tNetInputDev.GetInputEvent(&event);
        if(ret){
            printf("getinputevent err\n");
            return -1;
        } else {
            printf("type : %d\n", event.iType);
            printf("str  : %s\n", event.str);
        }
    }

}
#endif
*/