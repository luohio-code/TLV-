/********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  temp_client_main.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年07月19日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月19日 11时18分08秒"
 *                 
 ********************************************************************************

#ifndef _TEMP_CLIENT_MAIN_H
#define _TEMP_CLIENT_MAIN_H


#include "crc-itu-t.h"
#include "get_time.h"
#include "socket_client.h"
#include "tlv_pack.h"
#include "ds18b20_get_temperature.h"
#include "database.h"

#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sqlite3.h>
#include "tlv_unpack.h"

#define PROG_VERSION "1.1.0"
#define BUF_SIZE 6
#define TLV_SIZE 30

void banner(void);
void print_usage(char *progname);
void    signal_stop(int signum);
int log_system(int daemon_run);
int socket_send_tlv_pack(socket_t *sock, tlv_send_data *tlv_data, int timeout, int retry_times);
#endif 
