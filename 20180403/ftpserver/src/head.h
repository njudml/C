#ifndef __HEAD_H__
#define __HEAD_H__
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>
#include <shadow.h>
#include <crypt.h>
#include <sys/sendfile.h>
int randstr(char *,int);
int creatuser(int);
int login(int,char *);
void ls(int,int,int);
int recv_n(int,char *,int);
void child(int);
int makedir(char *,int,int);
int cd(char*,int,int);
int cd2(int,int);
void putfile(int,char*,int,int);
void getfile(int,char*,int,int);
void removefile(char*,int,int);
char* pwd(int sfd,int ui,int fi);
#define file "log.txt"

#endif
