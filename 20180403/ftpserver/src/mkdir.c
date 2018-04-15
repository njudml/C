#include "head.h"
int makedir(char *p,int ui,int fi)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server="localhost";
    char *usr="root";
    char *password="3412";
    char *database="wangpan";
    char query[200]="select filename from userspace where filename='";
    sprintf(query,"%s%s' and userID=%d",query,p,ui);
    int t;
    conn=mysql_init(NULL);
    if(!mysql_real_connect(conn,server,usr,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(conn));
    }
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        res=mysql_use_result(conn);
        if(res)
        {
            if((row=mysql_fetch_row(res))!=NULL)
            {
                return 0;
            }
        }
    }
    mysql_free_result(res); 
    strcpy(query,"insert into userspace (dirID,userID,filename) values (");
    sprintf(query,"%s%d,%d,'%s')",query,fi,ui,p);
    t=mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
    }
    mysql_close(conn);
    
}
