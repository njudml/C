#include "head.h"
void sighandler(int sig)
{
    if(sig==SIGINT)
    {
        printf("before sleep SIGINT\n");
        sleep(3);
        printf("after sleep SIGINT\n");
    }else if(sig==SIGQUIT)
    {
        printf("before sleep SIGQUIT\n");
        sleep(3);
        printf("after sleep SIGQUIT\n");
    }
}
int main()
{
    if(signal(SIGINT,sighandler)==SIG_ERR)
    {
        perror("signal");
        return -1;
    }
    if(signal(SIGQUIT,sighandler)==SIG_ERR)
    {
        perror("signal");
        return -1;
    }
    
    while(1);
}
