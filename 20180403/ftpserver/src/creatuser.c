#include "head.h"
int creatuser(int sfd)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[512]="select username from users where username='";
    char buf[30]={0};
    int t,r;
    recv(sfd,buf,sizeof(buf),0);
    strcat(query,buf);
    strcat(query,"'");
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,usr,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else
    {
        res=mysql_use_result(conn);
        if(res)
        {
            if(mysql_fetch_row(res))
            {
                r=0;
                send(sfd,&r,sizeof(r),0);
                goto lable;
            }
        }
    }
    r=1;
    send(sfd,&r,sizeof(r),0);
    char passwd[20]={0};
    char salt[13]={0};
    char *shadow;
    recv(sfd,passwd,sizeof(passwd),0);
    randstr(salt,12);
    shadow=crypt(passwd,salt);
    strcpy(query,"insert into users(username,salt,shadow) values(");
    char buf2[200]={0};
    sprintf(buf2,"'%s','%s','%s')",buf,salt,shadow);
    strcat(query,buf2);
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        r=1;
        send(sfd,&r,sizeof(r),0);
    }

lable:
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
