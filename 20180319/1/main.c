#include "head.h"
void *start(void *p)
{
    int i=*(int *)p;
    printf("I am child thread %d\n",i);
    pthread_exit((void *)2);    
}
int main()
{
    pthread_t thr;
    int p=1;

    pthread_create(&thr,NULL,start,&p);
    long ret;
    pthread_join(thr,(void **)&ret);
    printf("I am main thread %ld\n",ret);
}
