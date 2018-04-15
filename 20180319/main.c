#include "head.h"
#define N 20000000
typedef struct{
    int p;
    pthread_mutex_t mutex;
}data;
void *start(void *p)
{
    data *t=(data *)p;
   for(int i=0;i<N;i++)
   {
    pthread_mutex_lock(&t->mutex);
    t->p++;
    pthread_mutex_unlock(&t->mutex);
   }
   pthread_exit(NULL);
}
int main()
{
    pthread_t thr;
    data t;
    t.p=0;
    pthread_mutex_init(&t.mutex,NULL);
    pthread_create(&thr,NULL,start,&t);
    for(int i=0;i<N;i++)
    {
    pthread_mutex_lock(&t.mutex);
    t.p++;
    pthread_mutex_unlock(&t.mutex);
    }
    printf("%d\n",t.p);

}
