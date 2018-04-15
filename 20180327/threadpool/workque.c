#include "head.h"
#include "workque.h"
void que_init(pque_t que,int cap)
{
    que->quehead=que->quetail=NULL;
    que->capacity=cap;
    que->size=0;
    pthread_mutex_init(&que->que_mutex,NULL);
}
void que_set(pque_t que,pnode p)
{
    if(que->quehead==NULL)
    {
        que->quehead=que->quetail=p;
    }else
    {
        que->quetail->next=p;
        que->quetail=p;
    }
    que->size++;
}
void que_get(pque_t que,pnode *p)
{
    *p=que->quehead;
    que->quehead=que->quehead->next;
    que->size--;
}
