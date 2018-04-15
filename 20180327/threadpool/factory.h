#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "workque.h"
typedef void* (*pfunc)(void *);
typedef struct inode{
    pthread_t *pth_id;
    pthread_cond_t cond;
    int th_num;
    que_t que;
    int start_flag;
    pfunc thread_func;
}fac,*pfac;
typedef struct{
    int len;
    char buf[1000];
}train;
#define FILENAME "file"
void fac_init(pfac,pfunc,int);
void fac_start(pfac);
int send_n(int,char *,int);
void tran_file(int);

#endif
