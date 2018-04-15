#include "head.h"
void sighandler(int sig)
{
    printf("%d sleep\n",sig);
    sleep(1);
    printf("%d awake\n",sig);
}
int main()
{
    struct sigaction act;
    act.sa_handler=sighandler;
    sigaction(SIGINT,&act,NULL);
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,NULL);
    sleep(5);
    sigset_t set1;
    sigemptyset(&set1);
    sigpending(&set1);
    if(sigismember(&set1,SIGINT))
        printf("is pending\n");
    else
        printf("is not pending\n");
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    while(1);

}
