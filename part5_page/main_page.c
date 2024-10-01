#include "page_manager.h"

void MainPageRun(void *arg){
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

static PageAction g_tMainPage = {
    .name = "main",
    .Run = MainPageRun,
};

void MainPageRegister(){
    return PageRegister(&g_tMainPage);
}