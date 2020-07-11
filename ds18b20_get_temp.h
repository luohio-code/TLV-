/********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20_get_temp.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年07月09日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月09日 12时51分56秒"
 *                 
 ********************************************************************************/
#ifndef __DS15B20_GET_TEMPERATURE_H
#define __DS15B20_GET_TEMPERATURE_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int ds18b20_get_temperature(float *temp);

#endif

