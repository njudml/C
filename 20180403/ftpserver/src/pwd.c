#include "head.h"
char* pwd(int sfd,int ui,int fi)
{
    char *buf=(char *)calloc(1000,1);
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[200]="select dirID from userspace where userID=";
    sprintf(query,"%s%d and fileID=%d",query,ui,fi);
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
            row=mysql_fetch_row(res);
        }
    }
    int dirID=atoi(row[0]);
    if(dirID!=0)
    {
        sprintf(buf,"%s%s",buf,pwd(sfd,ui,dirID));
    }
    mysql_free_result(res);
    strcpy(query,"select filename from userspace where userID=");
    sprintf(query,"%s%d and fileID=%d",query,ui,fi);
    t=mysql_query(conn,query);
    if(t)
    {
    printf("Error making query:%s\n",        mysql_error(conn));
    }else
    {
        res=mysql_use_result(conn);
        if(res)
        {
            row=mysql_fetch_row(res);
        }
    }
    sprintf(buf,"%s/%s",buf,row[0]);
    mysql_close(conn);
    return buf;

}
