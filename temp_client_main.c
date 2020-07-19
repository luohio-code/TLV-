/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  temp_client_main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月19日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月19日 09时34分50秒"
 *                 
 ********************************************************************************/
#include "temp_client_main.h"

#define BUFSIZE  35
static int     g_sigstop = 0;

void print_usage(char *progname)
{
    printf("%s usage:\n",progname);
    printf("-v(--veision):display the program version\n");
    printf("-v(--veision):display the program version\n");
    printf("-i(--ipaddr):sepcify server ip address\n");
    printf("-i(--ipaddr):sepcify server ip address\n");
    printf("-d(--daemon):run it in the background\n");
    printf("-h(--help):print this help information.\n");
    return ;
}

void banner(void)
{
    printf("Version %s build on %s %s\n",PROG_VERSION,__DATE__,__TIME__);
    printf("copyright (c) 2020 makun\n");
    return ;
}

int main (int argc, char **argv)
{
    int                     ch;
    int                     rv = -1;
    int                     port;
    char                    *servip;
    int                     daemon_run;
    int                     clifd =-1;
    float                   temp;
    char                    buf[1024];
    time_t                  curr_time,last_time=0;
    tlv_send_data           tlv_data;
    int                     send_times;
    sqlite3                *db;
    int                     read_num =0;
    tlv_send_data           read_db;
    char                    read_buf[6];
    int                     timeout=3;
    int                     retry_times=3;
    socket_t                sock;

    struct option opts[]={
        {"port",required_argument,NULL,'p'},
        {"ipaddr",required_argument,NULL,'i'},
        {"version",no_argument,NULL,'v'},
        {"daemon",required_argument,NULL,'d'},
        {"help",no_argument,NULL,'h'},
        {NULL,0,NULL,0}
    };

    while( (ch=getopt_long(argc,argv,"p:i:b:hv",opts,NULL)) != -1)
    {
        switch(ch)
        {
            case 'p':
                port=atoi(optarg);  
                break;
            case 'i':
                servip=optarg;
                break;
            case 'b':
                daemon_run=1;//如果daemoon为1则放在后台
                break;
            case 'h':
                print_usage(argv[0]);
            case 'v':
                banner();
                return EXIT_SUCCESS;
        }

    }
    if(!port || !servip)//如果没指定端口和ip则打印帮助信息
    {
        print_usage(argv[0]);
        return 0;
    }

     /* 简单的日志系统*/ 
    log_system(daemon_run);
    /* 安装信号*/ 
    signal(SIGTERM, signal_stop);
    signal(SIGINT, signal_stop);

    //初始化数据库
    if(database_init(&db) <0)
    {
        return 0;
    }
    
    memset(&tlv_data,0,sizeof(tlv_data));
    while( !g_sigstop)
    {
        curr_time=time(&curr_time);
        if(curr_time-last_time > 5)//设置采样时间，如果大于3秒就采样温度
        {
            pack_sn(tlv_data.sn,sizeof(tlv_data.sn));
            pack_temp(tlv_data.temp,sizeof(tlv_data.temp));
            pack_time(tlv_data.datatime,sizeof(tlv_data.datatime));
            dump_buf(tlv_data.sn,TLV_SIZE);//以十六进制输出
            tlv_data.save_db_flag = SAVEDB;//设置保存数据库标识
            last_time = curr_time;//最后一次的时间为当前的时间

            if( CONNECTED != sock.status )
            {
                if( (rv= socket_client_connect(&sock, port, servip )) <0 )
                {
                    if(tlv_data.save_db_flag)
                    {
                        if(!(save_into_db(db,&tlv_data)))
                        {
                            printf("write into data db successfuly\n");
                            tlv_data.save_db_flag = NOSAVB;
                        }
                    }
                }
            }
            
            if( sock.status == CONNECTED )
            {
                printf("连接服务器成功\n");    
                printf("connect to server [%s:%d]successfuly!\n",servip, port);
                rv=socket_send_tlv_pack(&sock, &tlv_data,  timeout,  retry_times);
                if(rv < 0)
                {
                    printf("send message failure%s\n", strerror(errno));
                    if(tlv_data.save_db_flag)
                    {
                        if(!(save_into_db(db,&tlv_data)))
                        {
                            printf("write into data db successfuly\n");
                            tlv_data.save_db_flag = NOSAVB;
                        }
                    }
                }
                close(sock.clifd);
                return -1;
            }

            read_num = get_database_count(db);
            printf("打印数据库中的总数%d\n",read_num);
            if(read_num >0 )
            {
                for(int i=0; i<5; i++)
                {
                    rv = read_from_db(db,&tlv_data);
                    if(rv == -1)
                    {
                        break;
                    }
                    if(rv == 0)
                    {
                        rv=socket_send_tlv_pack(&sock, &tlv_data, timeout, retry_times);
                        if(rv < 0)
                        {
                            printf("send message failure%s\n", strerror(errno));
                            close(sock.clifd);
                            continue;
                        }
                        if(rv == 0 )
                        {
                            delete_db(db);
                            printf("delete successfuly from db\n")
                        }
                    }
                }
            }
        }
    }
}

sqlite3_close(db);
close(sock.clifd);
return 0;
}


int socket_send_tlv_pack(socket_t *sock, tlv_send_data *tlv_data, int timeout, int retry_times)
{
    int     i;
    int     rv=0;
    char    read_buf[128];

    memset(read_buf, 0, sizeof(read_buf));
    if(!sock || !tlv_data)
    {
        printf("Invalid input argument\n");
    }

    for(i=0; i<retry_times; i++)
    {
        if(  ( rv=socket_send(sock,tlv_data,TLV_SIZE) ) <0 )
        {
            printf("send tlv packet failure:%s\n",strerror(errno));
            return -1;

        }
        if (  (rv=socket_recv(sock,read_buf, sizeof(read_buf), timeout)) <0 )
        {
            printf("read ACK from server failure rv=%d\n", rv);
            sock->status = CONNECTED;
            break;
        }
        else 
        {
            rv=tlv_unpack(read_buf,BUF_SIZE);
            if(rv < 0)
            {
                close(sock->clifd);
                sock->status = CONNECTED;
                return -2;
            }
            if( rv == 0)
            {
                tlv_data->save_db_flag=NOSAVB;
                return 0;
            }
            if( rv == 1)
            {
                continue;
            }
        }
    }
}

void    signal_stop(int signum)
{
    if( SIGTERM == signum )
    {
        printf("SIGTERM signal detected\n");
        g_sigstop = 1;
    }
    else if( SIGINT == signum )
    {
        printf("SIGINT signal detected\n");
        g_sigstop = 1;
    }
}


/* 简单的日志，程序放在后台运行*/
int log_system(int daemon_run)
{
    int   log_fd = -1;
    log_fd=open("receive_temper.log", O_CREAT|O_RDWR, 0666);
    if (log_fd < 0)
    {
        printf("open log failure:%s\n",strerror(errno));
        return 0;
    }
    dup2(log_fd,STDOUT_FILENO);
    dup2(log_fd,STDERR_FILENO);

    if( (daemon(1,1))<0)
    {
        printf("daemon failure :%s\n", strerror(errno));
        return 0;
    }
}

