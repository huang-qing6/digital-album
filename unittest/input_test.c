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

int main(int argc, char **argv)
{
    int ret;
    InputEvent event;

    InputSystemRegister();
    InputDeviceInit();

    while(1){
        ret = GetInputEvent(&event);
        if(ret == -1){
            printf("Getinputevent err\n");
            return -1;
        } else {
            if(event.iType == INPUT_TYPE_TOUCH){
                printf("type     : %d\n", event.iType);
                printf("index x  : %d\n", event.iX);
                printf("index y  : %d\n", event.iY);
                printf("pressure : %d\n", event.iPresure);
            } else if(event.iType == INPUT_TYPE_NET){
                printf("type : %d\n", event.iType);
                printf("str  : %s\n", event.str);
            }
        }
    }

	return 0;	
}