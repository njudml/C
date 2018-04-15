#include "workque.h"
void que_init(pque_t p,int cap)
{
    p->head=p->tail=NULL;
    p->capacity=cap;
    p->size=0;
    pthread_mutex_init(&p->mutex,NULL);
}
void que_add(pque_t que,pnode p)
{
    if(que->head==NULL)
        que->head=que->tail=p;
    else
    {
        que->tail->next=p;
        que->tail=p;
    }
    que->size++;
}
void que_get(pque_t que,pnode *p)
{
    *p=que->head;
    que->head=que->head->next;
    que->size--;
}























