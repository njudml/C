#include "factory.h"
void fac_init(pfac p,pfunc f,int i)
{
    p->pth_ID=(pthread_t *)calloc(p->num,sizeof(pthread_t));
    pthread_cond_init(&p->cond,NULL);
    p->num=10;
    que_init(&p->que,i);
    p->start_flag=0;
    p->thread_func=f;
}
void fac_start(pfac p)
{
    int i;
    for(i=0;i<p->num;i++)
    {
        pthread_create(p->pth_ID+i,NULL,p->thread_func,p);
    }
}
