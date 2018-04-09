/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           framer.c
** Last modified Date:  2018-04-09
** Last Version:        v1.0
** Description:         装帧与帧定义
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
#include "usr_framer.h"
#include "radio_para.h"
#include "globalmacro.h"

macfct mac={
  0,
  0,
  0,
  0,
timeoffset_calc,
get_synch_time
};


/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        获取同步时间
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/


rtimer_clock_t get_synch_time(macfct *macpara)
{
  return rtimer_arch_now()-macpara->time_offset;
} 

/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        计算时间偏置
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/


void timeoffset_calc(macfct *macpara,uint32_t time)
{
  macpara->time_offset = RTIMER_NOW() -time;
  //对周期取与
  macpara->time_offset_period_align=macpara->time_offset%PEROID_LENGTH;
  //若果为负数
  if(macpara->time_offset_period_align<0)
      macpara->time_offset_period_align+=PEROID_LENGTH;
} 

/*********************************************************************************************************
** Function name:       frame_init
** Descriptions:        对传进来的帧类型进行装帧
** input parameters:    帧地址 帧类型
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/

void frame_init(PhyRadioMsg * msg,uint16_t frametype)
{
      radio_para *radio = (radio_para *)&radiopara;
      macfct *macpara = &mac;
      msg->fcfl=0x61;
      msg->fcfh=0x88;
      msg->seq=radio->seq;              //帧系列号
      msg->dest_pidl=(radio->pan_id & 0x00FF) ;
      msg->dest_pidh=((radio->pan_id >> 8) & 0x00FF) ;
      msg->des_addrl=0x40;
      msg->des_addrh=0x12;
      msg->src_addrl=(radio->shortaddr & 0x00FF);
      msg->src_addrh=((radio->shortaddr >> 8) & 0x00FF) ;
      msg->frame_type=frametype;
      //危险程度
      msg->danger=random_rand()%200;
      msg->moteid=get_moteid();
      msg->network_id = get_cluster_name(get_moteid());
      //装载进同步时间
      msg->time_stamp=macpara->get_synch_time(macpara);
      
      //广播帧设置Frame Control Field 以及目的地址
      if(frametype == FRAME_TYPE_BSM) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
      }
       //PANID 广播帧设置Frame Control Field 以及目的地址 目的PAIID
      if(frametype ==FRAME_TYPE_TIME_SYNCH ) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
        msg->dest_pidh = 0xFF;
        msg->dest_pidl = 0xFF;
      }
}
