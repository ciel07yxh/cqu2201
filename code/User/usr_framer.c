/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           framer.c
** Last modified Date:  2018-04-09
** Last Version:        v1.0
** Description:         װ֡��֡����
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
** Descriptions:        ��ȡͬ��ʱ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/


rtimer_clock_t get_synch_time(macfct *macpara)
{
  return rtimer_arch_now()-macpara->time_offset;
} 

/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        ����ʱ��ƫ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/


void timeoffset_calc(macfct *macpara,uint32_t time)
{
  macpara->time_offset = RTIMER_NOW() -time;
  //������ȡ��
  macpara->time_offset_period_align=macpara->time_offset%PEROID_LENGTH;
  //����Ϊ����
  if(macpara->time_offset_period_align<0)
      macpara->time_offset_period_align+=PEROID_LENGTH;
} 

/*********************************************************************************************************
** Function name:       frame_init
** Descriptions:        �Դ�������֡���ͽ���װ֡
** input parameters:    ֡��ַ ֡����
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void frame_init(PhyRadioMsg * msg,uint16_t frametype)
{
      radio_para *radio = (radio_para *)&radiopara;
      macfct *macpara = &mac;
      msg->fcfl=0x61;
      msg->fcfh=0x88;
      msg->seq=radio->seq;              //֡ϵ�к�
      msg->dest_pidl=(radio->pan_id & 0x00FF) ;
      msg->dest_pidh=((radio->pan_id >> 8) & 0x00FF) ;
      msg->des_addrl=0x40;
      msg->des_addrh=0x12;
      msg->src_addrl=(radio->shortaddr & 0x00FF);
      msg->src_addrh=((radio->shortaddr >> 8) & 0x00FF) ;
      msg->frame_type=frametype;
      //Σ�ճ̶�
      msg->danger=random_rand()%200;
      msg->moteid=get_moteid();
      msg->network_id = get_cluster_name(get_moteid());
      //װ�ؽ�ͬ��ʱ��
      msg->time_stamp=macpara->get_synch_time(macpara);
      
      //�㲥֡����Frame Control Field �Լ�Ŀ�ĵ�ַ
      if(frametype == FRAME_TYPE_BSM) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
      }
       //PANID �㲥֡����Frame Control Field �Լ�Ŀ�ĵ�ַ Ŀ��PAIID
      if(frametype ==FRAME_TYPE_TIME_SYNCH ) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
        msg->dest_pidh = 0xFF;
        msg->dest_pidl = 0xFF;
      }
}
