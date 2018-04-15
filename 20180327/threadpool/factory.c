#include "factory.h"
#include "workque.h"
void fac_init(pfac p,pfunc thread_func,int cap)
{
    p->pth_id=(pthread_t *)calloc(p->th_num,sizeof(pthread_t));
    p->thread_func=thread_func;
    pthread_cond_init(&p->cond,NULL);
    que_init(&p->que,cap);
}
void fac_start(pfac p)
{
    if(!p->start_flag)
    {
        int i;
        for(i=0;i<p->th_num;i++)
        {
            pthread_create(p->pth_id+i,NULL,p->thread_func,p);
        }
    }
}
