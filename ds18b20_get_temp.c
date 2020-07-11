/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20_get_temp.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月09日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月09日 12时40分06秒"
 *                 
 ********************************************************************************/

#include "ds18b20_get_temperature.h"
int ds18b20_get_temp(float *temp)
{
    char    w1_path[50] = "/sys/bus/w1/devices/";
    char    chip[20];
    char    buf[128];
    DIR     *dirp;
    struct  dirent *direntp;
    int     fd = -1;
    char    *ptr;
    float   value;
    int     found = 0;

    if(!temp)
    {
        return -1;
    }

    if( (dirp = opendir(w1_path)) == NULL)
    {
        printf("opendir error: %s\n",strerror(errno));
        return -2;
    }

    while( (direntp = readdir(dirp)) !=NULL)
    {
        if(strstr(direntp->d_name,"28-"))
        {
            strcpy(chip, direntp->d_name);
            found = 1;
            break;
        }
    }
    closedir(dirp);
    if( !found )
    {
        printf("can not find ds18b20 in %s\n",w1_path);
        return -3;
    }
    strncat(w1_path,chip,sizeof(w1_path)-strlen(w1_path));
    strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
     if( (fd=open(w1_path,O_RDONLY)) < 0)
     {
          printf("open %s error:%s\n",w1_path,strerror(errno));
          return -4;
     }
     memset(buf, 0 ,sizeof(buf));

     if(read(fd, buf ,sizeof(buf)) <0)
          {
               printf("read into fd %s\n",strerror(errno));
               return -5;
          }
     ptr = strstr(buf, "t");
      if( !ptr)
      {
          printf("can not t \n");
          return -6;

      }
       ptr+=2;
       *temp = atof(ptr)/1000.0;
        close(fd);
         return 0;
}

