#include "factory.h"
void getfile(int sfd, char* p, int ui, int fi)
{
    long len;
    recv(sfd, &len, sizeof(long), 0);
    char buf[33] = { 0 };
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    char* server = "localhost";
    char* usr = "root";
    char* password = "3412";
    char* database = "wangpan";
    char query[200] = "select MD5 from userspace where filename=";
    sprintf(query, "%s'%s' and userID=%d and dirID=%d", query, p, ui, fi);
    int t;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, usr, password, database, 0, NULL, 0)) {
        printf("Error connecting to database:%s\n", mysql_error(conn));
    }
    t = mysql_query(conn, query);
    if (t) {
        printf("Error making query:%s\n", mysql_error(conn));
    } else {
        res = mysql_use_result(conn);
        if (res) {
            if ((row = mysql_fetch_row(res)) != NULL) {
                strcpy(buf, row[0]);
            } else {
                printf("can't find file");
                return;
            }
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    int fd = open(buf, O_RDONLY);
    struct stat st;
    stat(buf, &st);
    send(sfd, &st.st_size, sizeof(off_t), 0);
    long len1 = st.st_size - len;
    lseek(fd, len, SEEK_SET);
    printf("%ld %ld\n", len, len1);
    if (sendfile(sfd, fd, 0, len1) < 0) {
        perror("sendfile");
    }
    close(fd);
}
void putfile(int sfd, char* p, int ui, int fi)
{
    char signal = '1';
    send(sfd, &signal, 1, 0);
    int sesig = 0;
    char realname[33] = { 0 };
    recv(sfd, realname, sizeof(realname), 0);
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    char* server = "localhost";
    char* usr = "root";
    char* password = "3412";
    char* database = "wangpan";
    char query[200] = "select MD5 from userspace where MD5=";

    sprintf(query, "%s'%s'", query, realname);
    int t;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, usr, password, database, 0, NULL, 0)) {
        printf("Error connecting to database:%s\n", mysql_error(conn));
    }
    t = mysql_query(conn, query);
    if (t) {
        printf("Error making query1:%s\n", mysql_error(conn));
    } else {
        res = mysql_use_result(conn);
        if (res) {
            if ((row = mysql_fetch_row(res)) != NULL) {
                sesig = 1;
                send(sfd, &sesig, sizeof(int), 0);
                goto lable1;
            }
        }
    }
    mysql_free_result(res);
    int fd = open(realname, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd == -1) {
        sesig = 1;
        send(sfd, &sesig, sizeof(int), 0);
        goto lable1;
    }
    send(sfd, &sesig, sizeof(int), 0);
    char buffer[4096] = { 0 };
    off_t filelen = 0;
    recv(sfd, &filelen, sizeof(off_t), 0);
    printf("start puts\n");
    off_t total = 0;
    off_t rec = 0;
    while (total < filelen) {
        rec = 0;
        bzero(buffer, sizeof(buffer));

        rec = recv(sfd, buffer, sizeof(buffer), 0);
        total += rec;
        write(fd, buffer, rec);
    }
    printf("end puts\n");

    strcpy(query, "insert into userspace (dirID,MD5,userID,filename,realname) values (");
    sprintf(query, "%s%d,'%s',%d,'%s','%s')", query, fi, realname, ui, p, p);
    t = mysql_query(conn, query);
    if (t) {
        printf("Error making query2:%s\n", mysql_error(conn));
    }
    return;
lable1:
    mysql_free_result(res);
    strcpy(query, "select filename from userspace where filename='");
    sprintf(query, "%s%s' and userID=%d and dirID=%d ", query, p, ui, fi);
    t = mysql_query(conn, query);
    if (t) {
        printf("Error making query3:%s\n", mysql_error(conn));
    } else {
        res = mysql_use_result(conn);
        if (res) {
            if ((row = mysql_fetch_row(res)) != NULL) {
                printf("file exist\n");
                return;
            }
        }
    }
    mysql_free_result(res);
    strcpy(query, "insert into userspace (dirID,MD5,userID,filename,realname) values (");
    sprintf(query, "%s%d,'%s',%d,'%s','%s')", query, fi, realname, ui, p, p);
    t = mysql_query(conn, query);
    if (t) {
        printf("Error making query4:%s\n", mysql_error(conn));
    }
    mysql_close(conn);
}
