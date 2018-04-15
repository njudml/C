#include "head.h"
void routine(void *p)
{
    free(p);
}
void *start(void *p)
{
    int *p1=(int *)malloc(20);
    char buf[10]={0};
    read(0,buf,sizeof(buf));
    pthread_cleanup_push(routine,p1);
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}
int main()
{
    pthread_t thr;
    pthread_create(&thr,NULL,start,NULL);
    pthread_cancel(thr);
}
