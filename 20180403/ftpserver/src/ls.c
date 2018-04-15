#include "head.h"
void ls(int sfd,int ui,int fi)
{
    char buf[1000]={0};
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[200]="select filename from userspace where userID=";
    sprintf(query,"%s%d and dirID=%d",query,ui,fi);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,usr,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
    }
    t=mysql_query(conn,query);
    if(t)
    {
    printf("Error making query:%s\n",        mysql_error(conn));
    }else
    {
        res=mysql_use_result(conn);
        if(res)
        {
            while((row=mysql_fetch_row(res))!=NULL)
            {
                sprintf(buf,"%s%s\t",buf,row[0]);
            }
        }
        mysql_free_result(res);
    }
    sprintf(buf,"%s\t",buf);
    mysql_close(conn);
    send(sfd,buf,strlen(buf),0);
    return;
}
