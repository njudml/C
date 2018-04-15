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
    struct sigaction act;
    act.sa_handler=sighandler;
    act.sa_flags=SA_NODEFER;
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    while(1);
}
