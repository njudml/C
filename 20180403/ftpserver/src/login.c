#include "head.h"
int login(int sfd,char *filename)
{
    printf("i am login\n");
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[512]="select username from users where username='";
    int t,r;
    char buf[30]={0};
    recv(sfd,buf,sizeof(buf),0);
    strcpy(filename,buf);
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
            if((row=mysql_fetch_row(res))==NULL)
            {
                r=0;
                send(sfd,&r,sizeof(int),0);
                goto lable;
            }
        }
    }
    r=1;
    send(sfd,&r,sizeof(int),0);
    mysql_free_result(res);
    strcpy(query,"select salt from users where username='");
    strcat(query,buf);
    strcat(query,"'");
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
        }
    }
    char passwd[20]={0};
    char salt[13]={0};
    char *shadow;
    recv(sfd,passwd,sizeof(passwd),0);
    strcpy(salt,row[0]);
    shadow=crypt(passwd,salt);
    strcpy(query,"select shadow from users where username='");
    strcat(query,buf);
    strcat(query,"'");
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
        }
    }
    if(strcmp(row[0],shadow)==0)
    {  
        r=1;
        send(sfd,&r,sizeof(int),0);
    }
    else 
    {
        r=0;
        send(sfd,&r,sizeof(int),0);
        goto lable;
    }
    mysql_free_result(res);
    strcpy(query,"select userID from users where username='");
    sprintf(query,"%s%s%s",query,buf,"'");
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }else{
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
        }
    }
    return atoi(row[0]);

lable:
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}
