#include "head.h"
int cd2(int ui,int di)
{
    int id;
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[200]="select dirID from userspace where userID=";
    sprintf(query,"%s%d and fileID=%d",query,ui,di);
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
            if((row=mysql_fetch_row(res))!=NULL)
            {
                id=atoi(row[0]);
            }
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return id;
}
    
