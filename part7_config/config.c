#include <stdio.h>
#include <string.h>

#include "config.h"

static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];
static int g_iTemCfgCount = 0;

int ParesConfigFile(){
    FILE *fp;
    char buf[100];
    char *p = buf;

    /* open config file */
    fp = fopen(CFG_FILE, "r");
    if(!fp){
        printf("read config file failed!\n");
        return -1;
    }

    /* read each line */
    while(fgets(buf, 100, fp)){//读入一行
        
        //buf[99] = '\0';
    
        /* ignore space/TAB */
        p = buf;
        while(*p == ' ' || *p == '\t')
            p++;

        /* ignore label line */
        if(*p == '#')
            continue;

        /* save data */
        g_tItemCfgs[g_iTemCfgCount].command[0] = '\0';
        g_tItemCfgs[g_iTemCfgCount].index = g_iTemCfgCount;
        sscanf(p, "%s %d %s", g_tItemCfgs[g_iTemCfgCount].name, 
                            &g_tItemCfgs[g_iTemCfgCount].bCanBeTouched, 
                            g_tItemCfgs[g_iTemCfgCount].command);
        g_iTemCfgCount++;
    }
    return 0;    
}


int GetItemCfgCount(void){
    return g_iTemCfgCount;
}

PItemCfg GetItemCfgByIndex(int index){
    if(index < g_iTemCfgCount){
        return &g_tItemCfgs[index];
    } else {
        return  NULL;
    }
}

PItemCfg GetItemCfgByName(char *name){
    int i;
    for (i = 0; i < g_iTemCfgCount; i++)
    {
        if(strcmp(name, g_tItemCfgs[i].name) == 0)
            return &g_tItemCfgs[i];
    }
    
        return  NULL;
}