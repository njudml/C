#ifndef __FACTORY_H__
#define __FACTORY_H__ 
#include "workque.h"
typedef void *(*pfunc)(void *);
typedef struct
{
    pthread_t *pth_ID;
    int num;
    que_t que;
    pthread_cond_t cond;
    int start_flag;
    pfunc thread_func;
}fac,*pfac;
void fac_init(pfac,pfunc,int);
void fac_start(pfac);

#endif
