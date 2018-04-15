#include "head.h"
int randstr(char *p,int num)
{
    const char *str="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len =strlen(str);
    int i;
    char buf[10]={0};
    srand(time(NULL));
    for(i=0;i<num-4;i++)
    {
        buf[i]=str[rand()%len];
    }
    sprintf(p,"%s%s%c","$6$",buf,'$');

}
