#ifndef __FOLD_H__ 
#define __FOLD_H__ 

#include "includes.h"
#include "radio_para.h"

#define FRAME_TYPE_TIME_SYNCH   0x01
#define FRAME_TYPE_BSM          0x02
#define FRAME_TYPE_WARN_ACK     0x03
#define FRAME_TYPE_WARN         0x04//簇间通信类型



#define NR_SLOTS        25                              //时隙分割数量
#define BSM_FRE_HZ      10                              //BSM 10Hz
#define PEROID_LENGTH   (RTIMER_SECOND/BSM_FRE_HZ)      //BSM 周期时间     
#define SLOT_LENGTH     (PEROID_LENGTH / NR_SLOTS)      //时隙长度
#define GUARD_PERIOD    (PEROID_LENGTH / (NR_SLOTS*2))  //保护间隔



void packet_input_arch(void);

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
rtimer_clock_t get_synch_time();
void timeoffset_calc(macfct *macpara,uint32_t time);
void tdmasend(void *ptr);
#endif