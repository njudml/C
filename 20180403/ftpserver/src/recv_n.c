#include "factory.h"
int recv_n(int sfd,char* buf,int len)
{
    int total=0;
    int ret;
    while(total<len)
    {
        ret=recv(sfd,buf+total,len-total,0);
        total=total+ret;
    }
    return 0;
}

