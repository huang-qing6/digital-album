#ifndef _IPUT_MANAGER_H
#define _IPUT_MANAGER_H

#include <sys/time.h>

#ifndef NULL
#define NULL (void*)0
#endif

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET   2

typedef struct InputEvent{
    struct timeval tTime;
    int iType;
    int iX;
    int iY;
    int iPresure;
    char str[1000];
}InputEvent, *PInputEvent;

typedef struct InputDevice{
    char *name;
    int (*GetInputEvent)(PInputEvent ptInputEvent);
    int (*DeviveInit)(void);
    int (*DeviveExit)(void);
    struct InputDevice *ptNext;
}InputDevice, *PInputDevice;


void RegisterInputDevice(PInputDevice pyInputDevice);
void InputInit(void);
void InputDeviceInit(void);
int GetInputEvent(PInputEvent ptInputEvent);

#endif