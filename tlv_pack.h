/********************************************************************************
 *      Copyright:  (C) 2020 Luo Hui<1091992698@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  tlv_pack.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年07月12日)
 *         Author:  Luo Hui <1091992698@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月12日 14时01分22秒"
 *                 
 ********************************************************************************/

#ifndef _TLV_PACH_H
#define _TLV_PACH_H

/*   TLV Packet format: 
 *    
 *   +------------+---------+------------+-----------+-----------+
 *   | Header(1B) | Tag(1B) | Length(1B) | Value(1B) | CRC16(2B) |
 *   +------------+---------+------------+-----------+-----------+
 *
 *    */

enum
{
    NOSAVB,
    SAVEDB,

};

#pragma pack(1)
typedef struct tlv_data_s
{   
        unsigned    char sn[12];
        unsigned    char temp[7];
        unsigned    char datatime[11];
        int  save_db_flag;
        int  id; 
}tlv_send_data;
#pragma pack()

/* packet TLV message for SN */
int pack_sn(char *buf, int size);

/* packet TLV message for temperature*/
int pack_temp(char *buf, int size);

/* packet TLV message for data time*/
int pack_time(char *buf, int size);

/* printf buf data in hex and string mode*/
void    dump_buf(char *data,int len);  


#endif


