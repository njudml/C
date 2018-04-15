#include "head.h"
void sighandler(int sig)
{
    printf("I am %d,sleep\n",sig);
    sleep(3);
    sigset_t set;
    sigemptyset(&set);
    sigpending(&set);
    if(sigismember(&set,SIGQUIT))
        printf("signal SIGQUIT is pending\n");
    else
        printf("is not pending\n");
    printf("I am %d,awake\n",sig);
    
}
int main()
{
    struct sigaction act;
    act.sa_handler=sighandler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);
    sigaction(SIGINT,&act,NULL);
    while(1);
}
