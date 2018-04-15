#include "head.h"
void removefile(char *p,int ui,int fi)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[200]="update userspace set userID=NULL where userID=";
    sprintf(query,"%s%d and filename='%s' and dirID=%d",query,ui,p,fi);
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
        printf("remove sucess\n");
    }
    mysql_close(conn);
    return;
}
