/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_client_1.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月26日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月26日 10时36分13秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP          "127.0.0.1"
#define SERVER_PORT        8889
#define MSG_STR            "Hello, Unix Network Program World!"

int main(int argc, char **argv)
{
    int                     conn_fd = -1;
    int                     rv = -1;
    char                    buf[1024]; 
    struct sockaddr_in      serv_addr;

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0)
    {
        printf("create socket failure: %s\n", strerror(errno));
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_aton( SERVER_IP, &serv_addr.sin_addr );

    if( connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))  < 0)
    {
        printf("connect to server [%s:%d] failure: %s\n", SERVER_IP, SERVER_PORT, strerror(errno));
        return 0;
    }

    if( write(conn_fd, MSG_STR, strlen(MSG_STR)) < 0 )
    {
        printf("Write data to server [%s:%d] failure: %s\n", SERVER_IP, SERVER_PORT, strerror(errno));
        goto cleanup;
    }

    memset(buf, 0, sizeof(buf));
    rv = read(conn_fd, buf, sizeof(buf)); 
    if(rv < 0)
    {
        printf("Read data from server failure: %s\n", strerror(errno));
        goto cleanup;
    }
    else if( 0 == rv )
    {
        printf("Client connect to server get disconnected\n");
        goto cleanup;
    }

    printf("Read %d bytes data from server: '%s'\n", rv, buf);

cleanup:
    close(conn_fd);
}
