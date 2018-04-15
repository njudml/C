#include "head.h"
#include "workque.h"
#include "factory.h"
void * thread_func(void *p)
{
    pfac f=(pfac)p;
    pnode pn=(pnode)calloc(1,sizeof(node));
    while(1)
    {
        pthread_mutex_lock(&f->que.que_mutex);
        if(!f->que.size)
        {
            pthread_cond_wait(&f->cond,&f->que.que_mutex);
        }
        que_get(&f->que,&pn);
        pthread_mutex_unlock(&f->que.que_mutex);
        tran_file(pn->new_fd);
    }
}
int main(int argc,char *argv[])
{
    if(argc!=5)
    {
        printf("error args\n");
        return -1;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    int reuse=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    fac f;
    bzero(&f,sizeof(f));
    f.th_num=atoi(argv[3]);
    int cap=atoi(argv[4]);
    struct sockaddr_in ser;
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(argv[2]));
    bind(sfd,(struct sockaddr *)&ser,sizeof(ser));
    fac_init(&f,thread_func,cap);
    fac_start(&f);
    int new_fd;
    listen(sfd,100);
    while(1)
    {
        new_fd=accept(sfd,NULL,NULL);
        pnode p=(pnode)malloc(sizeof(node));
        p->new_fd=new_fd;
        p->next=NULL;
        pthread_mutex_lock(&f.que.que_mutex);
        que_set(&f.que,p);
        pthread_mutex_unlock(&f.que.que_mutex);
        pthread_cond_signal(&f.cond);
    }
}
