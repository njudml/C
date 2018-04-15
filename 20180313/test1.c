#include "head.h"
int main()
{
    int n;
    char buf[120]={0};
    while((n=read(0,buf,120))>0)
        if(write(1,buf,n)!=n)
            perror("write");
    if(n<0)
        perror("read");

    return 0;
}
