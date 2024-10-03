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
#include "ui.h"

int main(int argc, char **argv)
{
	PDispBuff ptBuffer;
	int error;
	Button tButton;
	Region tRegion;

	if(argc != 2){
		printf("usage %s <font>\n",argv[0]);
		return -1;
	}

	DisplaySystemRegister();

	SelectDefaultDisplay("fb");

	InitDefaultDisplay();

	ptBuffer = GetDisplayBuffer();

	FontSystemRegister();
	
	error = SelectAndInitFont("freetype", argv[1]);
	if (error)
	{
		printf("SelectAndInitFont err\n");
		return -1;
	}

	tRegion.iLeftUpX = 200;
	tRegion.iLeftUpY = 200;
	tRegion.iWidth = 300;
	tRegion.iHeigh = 100;
	InitButton(&tButton, &tRegion, "test", NULL, NULL);

	tButton.OnDraw(&tButton, ptBuffer);
	//模拟点击
	while(1){
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);
	}

	return 0;	
}
