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
#include "bsm.h"
#define FRAME_TYPE_TIME_SYNCH   0x01
#define FRAME_TYPE_BSM          0x02
#define FRAME_TYPE_WARN_ACK     0x03
#define FRAME_TYPE_WARN         0x04//�ؼ�ͨ������
#define FRAME_TYPE_PDR_INFO     0x05

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

//Ԥ��֡ͷ
typedef struct pdr_info_head{
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
        uint8_t addr_count;
}pdr_info_head;


typedef struct pdr_info_node{
        uint16_t source_addr;
        uint16_t receive_packet;
        uint16_t delay;
}pdr_info_node;


//Ԥ�趨��mac��Ĳ���
typedef struct mac {
        uint8_t  IsSyched;
        uint32_t time_stamp;
        int32_t  time_offset;
        int32_t  time_offset_period_align;         //����ʱ��ͬ�����ڶ���
        void (*timeoffset)(struct mac *macpara,uint32_t time);
        rtimer_clock_t (*get_synch_time)(struct mac *macpara);
}macfct;

typedef struct pdr_addr_poll{
        uint8_t  node_count;
        uint8_t  buf[50];
}pdr_addr_poll;        
        
        
extern macfct mac;
extern uint8_t pdr_frame_count;
void frame_init(PhyRadioMsg * msg,uint16_t frametype);   
void timeoffset_calc(macfct *macpara,uint32_t time);
rtimer_clock_t get_synch_time();
uint8_t pdr_info_frame_create(uint16_t frametype,uint8_t *buf,uint8_t count,uint8_t pos,route_info *routeinfo);
void pdr_info_frame_pase(uint8_t *buf,uint8_t len);
void add_pdr_addr_poll(uint8_t id);
uint8_t check_exist(uint8_t len,uint8_t *buf,uint8_t id);
#endif