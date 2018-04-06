#ifndef __FOLD_H__ 
#define __FOLD_H__ 

#include "includes.h"


#define FRAME_TYPE_TIME_SYNCH   0x0001
#define FRAME_TYPE_BSM          0x0002
#define FRAME_TYPE_WARN_ACK     0x0003
#define FRAME_TYPE_WARN         0x0004//簇间通信类型

void packet_input_arch(void);

typedef struct PhyRadioMsg {
	uint16_t fcf;
	uint16_t seq;
        uint16_t dest_pid;              
        uint16_t src_pid;               
        uint16_t frame_type;
        uint16_t danger;
        uint16_t moteid;
        uint16_t network_id;
        uint32_t time_stamp;
}PhyRadioMsg;

typedef struct mac {
        uint8_t  IsSyched;
        uint32_t time_stamp;
        int32_t  time_offset;
        void (*timeoffset)(struct mac *macpara,uint32_t time);
        rtimer_clock_t (*get_synch_time)(struct mac *macpara);
}macfct;

extern macfct mac;
uint16_t get_cluster_name(uint16_t moteid);
void frame_init(PhyRadioMsg * msg,uint16_t frametype);   
rtimer_clock_t get_synch_time();
void timeoffset_calc(macfct *macpara,uint32_t time);
#endif