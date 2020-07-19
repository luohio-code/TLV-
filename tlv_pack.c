/*********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  tlv_pack.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月19日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月19日 11时19分25秒"
 *                 
 ********************************************************************************/

#include "tlv_pack.h"
#include <stdio.h>
#include <string.h>
#include "temp_client_main.h"
#define HEAD 0xFD  //定义一个报文头
#define TLV_FIXED_SIZE  5 //固定TLV字节流大小，不包含value的值
#define TLV_MINI_SIZE  (TLV_FIXED_SIZE+1)  //TLV字节流的最小值，value的最小值为12
#define BUFSIZE 256

enum  //使用枚举Tag的值会自加
{
    TAG_SN=0x01,//树莓派上的ID
    TAG_TEMP=0x02,//温度
    TAG_TIME=0x03,//时间
};

/* 树莓派的编号ID=RPI0001报文
 *  * H      T             L               Value    CRC1    CRC2
 *  * 0xFD  0x01         0x01              0x07      0x12    0x34
 */

int pack_sn(char *buf, int size)//声明id封装函数
{
    unsigned short crc16 = 0;
    char           *id = "RPI0001";
    int     ofset = 0;
    int         pack_len = 0;
    int     data_len = 0;

    if(!buf || size < TLV_MINI_SIZE)
    {
        printf("Invalid input argument \n");
        return 0;
    }

    buf[ofset]=HEAD;
    ofset+=1;

    buf[ofset]=TAG_SN;
    ofset +=1;

    data_len=strlen(id);
    pack_len = data_len + TLV_FIXED_SIZE;
    buf[ofset]=pack_len;
    ofset +=1;

    memcpy(&buf[ofset], id, data_len);

    ofset += data_len;

    crc16=crc_itu_t(MAGIC_CRC, buf, ofset);
    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;

    return ofset; //返回索引值
}
int  pack_temp(char *buf, int size)
{
    unsigned short crc16=0;
    int     pack_len = 0;
    int     data_len = 0;
    int     ofset = 0;
    float   temp;
    char    datatime[32];
    int     i,j;

    if(!buf || size < TLV_MINI_SIZE)
    {
        printf("Invalid input adgument\n");
        return 0;
    }
    buf[ofset]=HEAD;
    ofset +=1;

    buf[ofset]=TAG_TEMP;
    ofset +=1;

    ds18b20_get_temperature(&temp);
    i=(int)temp; //整数部分
    j=(int) ((temp-i) *100);//小数部分

    data_len = 2;
    pack_len= data_len + TLV_FIXED_SIZE;
    buf[ofset]=pack_len;
    ofset +=1;

    buf[ofset]=i;
    ofset+=1;
    buf[ofset]=j;
    ofset+=1;

    crc16=crc_itu_t(MAGIC_CRC, buf, ofset);
    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;
    return ofset;//返回索引值
}

int pack_time (char *buf, int size)
{
    unsigned short crc16=0;
    int  ofset = 0;
    int  pack_len = 0;
    int  data_len = 0;
    char  datatime[32];

    if( !buf || size < TLV_MINI_SIZE)
    {
        printf("Invald input argument\n");
        return 0;
    }

    buf[ofset]=HEAD;
    ofset +=1;

    buf[ofset]=TAG_TIME;
    ofset +=1;

    memset(datatime, 0, sizeof(datatime));
    int time=get_time(datatime);

    data_len = time;
    pack_len =data_len + TLV_FIXED_SIZE;
    buf[ofset]=pack_len;
    ofset +=1;

    buf[ofset]=datatime[0];
    ofset +=1;

    buf[ofset]=datatime[1];
    ofset +=1;

    buf[ofset]=datatime[2];
    ofset +=1;

    buf[ofset]=datatime[3];
    ofset +=1;

    buf[ofset]=datatime[4];
    ofset +=1;

    buf[ofset]=datatime[5];
    ofset +=1;

    crc16=crc_itu_t(MAGIC_CRC,buf, ofset);
    ushort_to_bytes(&buf[ofset], crc16);
    ofset +=2;
    return ofset;//返回索引值
}

void dump_buf( char *data, int len)//data指针，指向buf的首地址，len是buf的长度
{
    int i;
    for(i=0; i<len; i++)
    {
        printf("0x%02x ",(unsigned char)data[i]);
        if( 0 == (i+1)%16 )
            printf("\n");
    }
    printf("\n");
}
