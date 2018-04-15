#include "head.h"
typedef struct{
    pthread_mutex_t lock;
    pthread_cond_t cond;
}data;
void *start(void *p)
{
    data *t=(data*)p;
    pthread_mutex_lock(&t->lock);
    pthread_cond_wait(&t->cond,&t->lock);
    pthread_mutex_unlock(&t->lock);
}
int main()
{
    data t;
    pthread_t thr;
    pthread_mutex_init(&t.lock,NULL);
    pthread_cond_init(&t.cond,NULL);
    pthread_create(&thr,NULL,start,&t);
    sleep(1);
    pthread_cond_signal(&t.cond);
    int k=1;
    pthread_join(thr,NULL);
    printf("%d\n",k);


}
