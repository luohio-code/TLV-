/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_time.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月09日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月09日 13时15分16秒"
 *                 
 ********************************************************************************/
#include "get_time.h"
int get_time(char  *datatime)
{
    struct tm *p;
    time_t timep;
    time(&timep);
    p=localtime(&timep);

    datatime[0] = p->tm_year;
    datatime[1] = p->tm_mon+1;
    datatime[2] = p->tm_mday;
    datatime[3] = p->tm_hour;
    datatime[4] = p->tm_min;
    datatime[5] = p->tm_sec;
    return 6 ; 
}

    



