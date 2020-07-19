/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月11日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月11日 12时11分40秒"
 *                 
 ********************************************************************************/

#include "socket_client.h"

#define tlv_size  30

int socket_client_connect(socket_t *sock, int port, char *servip)
{
    char       buf[1024];
    struct  sockaddr_in  servaddr;
    struct  hostent *host =NULL;
    char    *hostip=NULL;
    sock->clifd = socket(AF_INET, SOCK_STREAM, 0);
     
     if( !sock || !servip  )
     {
                printf("invalid input argument\n");
                return -1;
     }
      if(sock->clifd < 0)
      {
          printf("create socket failure:%s\n",strerror(errno));
          return -2;
      }
      host = gethostbyname(servip);//域名解析函数
       if(!host)
       {
           printf("cannot get host by hostname\n");
           return -3;
       }
       hostip = inet_ntoa( *((struct in_addr*)host->h_addr));
       printf("host ip=%s\n", hostip);
       printf("create socket successfuly\n");
       memset(&servaddr, 0, sizeof(servaddr));
       servaddr.sin_family = AF_INET;
       servaddr.sin_port = htons(port);
       inet_aton(servip, &servaddr.sin_addr);

       if( connect(sock->clifd, (struct sockaddr *)&servaddr,sizeof(servaddr)) <0)
       {
           printf("connect to server%s %d failure:%s\n",servip,port, strerror(errno));
           sock->status = DISCONECT;
           close(sock->clifd);
           return -2;
       }
       printf("connect to server successfuly\n");

       sock->status = CONNECTED;
       return 0;
}
int socket_send(socket_t *sock, tlv_send_data *tlv, int size)
{
     int        rv=0;

     if(!tlv || !sock)
          {
              printf("Invalid input arguments\n");
              return -1;
          }
     rv= write(sock->clifd, &tlv->sn,size);
     printf("write from buf bytes:%d\n", rv);
     
     if(rv <=0)
     {
         printf("send message to server failure:%s\n",strerror(errno));
         sock->status=CONNECTED;
         return -2;
     }
     else if(rv > 0)
     {
         printf("send message to server successfuly\n");//打印发送成功
         return 0;
     }
        
}
int socket_recv(socket_t *sock, char *buf, int size, int timeout)
{
    int rv=0;
    int i=0;
    int convert;
    fd_set fdsr;
    if(!buf)
    {
        printf("Invalid input argument\n");
        return -1;
    }
    memset(buf, 0,sizeof(buf));
    FD_ZERO(&fdsr);
    FD_SET(sock->clifd,&fdsr);
    if(timeout <=0)
    {
        rv=select(sock->clifd+1,&fdsr,NULL,NULL,NULL);
    }
    else
    {
        struct timeval tv;
        tv.tv_sec =timeout;
        tv.tv_usec=0;
        rv=select(sock->clifd+1,&fdsr,NULL,NULL, &tv);
    }
    if(rv < 0)
    {
        printf("select read from socket[%d] failulre:%s\n",sock->clifd,strerror(errno));
        return  -1;
    }
    else if(rv ==0 )
    {
        printf("select read from socket[%d] get timeout\n", sock->clifd);
        return 0;
    }
    else if(rv >0)
    {
        rv=read(sock->clifd, buf, size);
        if(rv<0)
        {
            printf("read data from client socket[%d] failure:%s\n",sock->clifd, strerror(errno));
            close(sock->clifd);
            return -2;
        }
        else if( rv ==0 )
        {
            printf("clietn socket [%d] disconnected\n", sock->clifd);
            close(sock->clifd);
            return -3;
        }
        else if(rv >0)
        {
            printf("start receive from start\n");
            dump_buf(buf, 6);
            printf("read from data bytes:%d\n",rv);
            return 1;
        }
    }
}

