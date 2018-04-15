#ifndef __WORKQUE_H__
#define __WORKQUE_H__ 
#include "head.h"
typedef struct node{
    int new_fd;
    struct node *next;
}node,*pnode;
typedef struct{
    pnode quehead,quetail;
    int capacity;
    int size;
    pthread_mutex_t que_mutex;
}que_t,*pque_t;
void que_init(pque_t,int);
void que_set(pque_t,pnode);
void que_get(pque_t,pnode *);

#endif
