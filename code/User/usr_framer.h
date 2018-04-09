/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           framer.h
** Last modified Date:  2018-04-09
** Last Version:        v1.0
** Description:         装帧与帧定义，以及与帧内容相关
**
**--------------------------------------------------------------------------------------------------------
** Created By:          张校源
** Created date:        2018-04-09
** Version:             v1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#ifndef _USR_FRAMER_H__
#define _USR_FRAMER_H__
#include "includes.h"
#include "usr_mac.h"
#define FRAME_TYPE_TIME_SYNCH   0x01
#define FRAME_TYPE_BSM          0x02
#define FRAME_TYPE_WARN_ACK     0x03
#define FRAME_TYPE_WARN         0x04//簇间通信类型

//预设定义帧结构(固定帧)
//TODO 动态装帧
typedef struct PhyRadioMsg {
	uint8_t fcfl;
        uint8_t fcfh;
	uint8_t seq;
        
        uint8_t dest_pidl;
        uint8_t dest_pidh; 

        uint8_t des_addrl;
        uint8_t des_addrh;
        
        uint8_t src_addrl;
        uint8_t src_addrh;

        uint8_t frame_type;

        uint16_t danger;
        uint16_t moteid;
        uint16_t network_id;
        uint32_t time_stamp;
}PhyRadioMsg;
//预设定义mac层的参数
typedef struct mac {
        uint8_t  IsSyched;
        uint32_t time_stamp;
        int32_t  time_offset;
        int32_t  time_offset_period_align;         //用于时间同步周期对齐
        void (*timeoffset)(struct mac *macpara,uint32_t time);
        rtimer_clock_t (*get_synch_time)(struct mac *macpara);
}macfct;

extern macfct mac;
void frame_init(PhyRadioMsg * msg,uint16_t frametype);   
void timeoffset_calc(macfct *macpara,uint32_t time);
rtimer_clock_t get_synch_time();

#endif