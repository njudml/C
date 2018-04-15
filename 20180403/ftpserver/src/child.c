#include "factory.h"
void gettime(char* p)
{
    time_t now;
    struct tm* timenow;
    time(&now);
    timenow = localtime(&now);
    strcpy(p, asctime(timenow));
}
void child(int new_fd)
{
    int i;
    int ret;
    int userID;
    int dirID = 0;
    char filename[20] = { 0 };
    int fd;
    char tim[50] = { 0 };
    char username[20] = { 0 };
    char log[100] = { 0 };
lable1:
    recv(new_fd, &i, sizeof(int), 0);
    if (i == 1) {
        userID = login(new_fd, username);
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        bzero(tim, sizeof(tim));
        gettime(tim);
        sprintf(log, "user %s long in\t%s\n", username, tim);
        write(fd, log, strlen(log));
        close(fd);
    } else if (i == 2) {
        creatuser(new_fd);
        goto lable1;
    }
    while (1) {
        i = 0;
        bzero(filename, sizeof(filename));
        ret = recv(new_fd, &i, sizeof(int), 0);
        if (ret == 0) {
            break;
        }
        if (i == 3) {
            ls(new_fd, userID, dirID);
            printf("%d\n", dirID);
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :ls\t%s\n", username, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
        if (i == 4) {
            if (dirID == 0)
                send(new_fd, "/", 1, 0);
            else {
                char* p;
                p = pwd(new_fd, userID, dirID);
                send(new_fd, p, strlen(p), 0);
                free(p);
                fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
                bzero(tim, sizeof(tim));
                gettime(tim);
                sprintf(log, "user %s :pwd\t%s\n", username, tim);
                write(fd, log, strlen(log));
                close(fd);
            }
        }
        if (i == 5) {
            recv(new_fd, filename, sizeof(filename), 0);
            if (strcmp(filename, "..") == 0)
                dirID = cd2(userID, dirID);
            else
                dirID = cd(filename, userID, dirID);
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :cd %s\t%s\n", username, filename, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
        if (i == 6) {
            recv(new_fd, filename, sizeof(filename), 0);
            puts(filename);
            printf("start puts\n");
            putfile(new_fd, filename, userID, dirID);
            printf("end puts\n");
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :puts %s\t%s\n", username, filename, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
        if (i == 7) {
            recv(new_fd, filename, sizeof(filename), 0);
            char signal = '1';
            send(new_fd, &signal, 1, 0);
            getfile(new_fd, filename, userID, dirID);
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :gets %s\t%s\n", username, filename, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
        if (i == 8) {
            recv(new_fd, filename, sizeof(filename), 0);
            removefile(filename, userID, dirID);
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :remove %s\t%s\n", username, filename, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
        if (i == 9) {
            recv(new_fd, filename, sizeof(filename), 0);
            makedir(filename, userID, dirID);
            fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            bzero(tim, sizeof(tim));
            gettime(tim);
            sprintf(log, "user %s :mkdir %s\t%s\n", username, filename, tim);
            write(fd, log, strlen(log));
            close(fd);
        }
    }
}
