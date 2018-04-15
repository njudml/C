#include "head.h"
#include "workque.h"
#include "factory.h"
void * thread_func(void *p)
{
    pfac f=(pfac)p;
    pnode pn=(pnode)calloc(1,sizeof(node));
    while(1)
    {
        pthread_mutex_lock(&f->que.mutex);
        if(!f->que.size)
            pthread_cond_wait(&f->cond,&f->que.mutex);
        que_get(&f->que,&pn);
        pthread_mutex_unlock(&f->que.mutex);
        child(pn->new_fd);
        close(pn->new_fd);
    }
}
int main(int argc,char *argv[])
{
    int fd=open(argv[1],O_RDONLY);
    char ip[20]={0};
    char port[20]={0};
    char c;
    int i=0;
    while(read(fd,&c,1))
    {
        if(c=='\n')
            break;
        else
            ip[i++]=c;
    }
    i=0;
    while(read(fd,&c,1))
    {
        if(c=='\n')
            break;
        else
            port[i++]=c;
    }
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    int reuse=1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    fac f;
    bzero(&f,sizeof(f));
    fac_init(&f,thread_func,20);
    fac_start(&f);
    struct sockaddr_in ser;
    ser.sin_family=AF_INET;
    ser.sin_addr.s_addr=inet_addr(ip);
    ser.sin_port=htons(atoi(port));
    bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    listen(sfd,100);
    int new_fd;
    while(1)
    {
        new_fd=accept(sfd,NULL,NULL);
        pnode p=(pnode)malloc(sizeof(node));
        p->new_fd=new_fd;
        p->next=NULL;
        pthread_mutex_lock(&f.que.mutex);
        que_add(&f.que,p);
        pthread_mutex_unlock(&f.que.mutex);
        pthread_cond_signal(&f.cond);
    }
}
