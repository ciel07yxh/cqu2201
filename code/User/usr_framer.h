/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           framer.h
** Last modified Date:  2018-04-09
** Last Version:        v1.0
** Description:         װ֡��֡���壬�Լ���֡�������
**
**--------------------------------------------------------------------------------------------------------
** Created By:          ��УԴ
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
#define FRAME_TYPE_WARN         0x04//�ؼ�ͨ������

//Ԥ�趨��֡�ṹ(�̶�֡)
//TODO ��̬װ֡
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
//Ԥ�趨��mac��Ĳ���
typedef struct mac {
        uint8_t  IsSyched;
        uint32_t time_stamp;
        int32_t  time_offset;
        int32_t  time_offset_period_align;         //����ʱ��ͬ�����ڶ���
        void (*timeoffset)(struct mac *macpara,uint32_t time);
        rtimer_clock_t (*get_synch_time)(struct mac *macpara);
}macfct;

extern macfct mac;
void frame_init(PhyRadioMsg * msg,uint16_t frametype);   
void timeoffset_calc(macfct *macpara,uint32_t time);
rtimer_clock_t get_synch_time();

#endif