#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#include "disp_manager.h"
#include "font_manager.h"
#include "page_manager.h"
#include "input_manager.h"

int main(int argc, char **argv)
{
	int error;
	
    if(argc != 2){
        printf("Usage: %s <font_type>\n", argv[0]);
    }

    /* Disp system init */
	DisplaySystemRegister();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();

    /* Input system init */
    InputSystemRegister();
    InputDeviceInit();

    /* Font system init */
	FontSystemRegister();
	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

    /* Page system init */
	PagesRegister();

    /* start main GUI */
    Page("main")->Run(NULL);
	
	return 0;	
}
