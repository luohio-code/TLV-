/********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_client.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年07月12日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月12日 22时53分51秒"
 *                 
 ********************************************************************************/
 

#ifndef __SOCKET_CLIENT_H_
#define __SOCKET_CLIENT_H_

#include <stdio.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tlv_pack.h"
#include "database.h"
#include <unistd.h> 

enum
{
                DISCONECT,
                CONNECTED,
};
typedef struct socket_s
{
    int     clifd;
    int     status;

}socket_t;

int socket_client_connect(socket_t *sock, int port, char *servip);
int socket_send(socket_t *sock, tlv_send_data *tlv, int size);
int socket_send(socket_t *sock, tlv_send_data *tlv, int size);
#endif
