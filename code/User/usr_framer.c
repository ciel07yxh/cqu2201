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
#include <string.h>

/*********************************************************************************************************
** �Ƿ�ʹ�ܵ��Թ���
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#include <stdio.h>
#define PRINTF(...)   uart_printf(__VA_ARGS__)  //����ʹ��uart_printf��������
#else
#define PRINTF(...)
#endif
/*********************************************************************************************************
 ȫ�ֱ���
*********************************************************************************************************/
macfct mac={
  0,
  0,
  0,
  0,
timeoffset_calc,
get_synch_time
};
pdr_addr_poll __pdr_addr_poll;

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

/*********************************************************************************************************
** Function name:       pdr_info_frame_create
** Descriptions:        �Դ�������֡���ͽ���װ֡
** input parameters:    ֡��ַ ֡����
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

uint8_t pdr_info_frame_create(uint16_t frametype,uint8_t *buf,uint8_t count,uint8_t max_ele,route_info *routeinfo)
{     
      
      uint8_t bufferSize=0;
      uint8_t i;
      radio_para *radio = (radio_para *)&radiopara;
      pdr_info_head pdrinfohead;
      
      
      //ͷ����ʼ����
      pdr_info_head *msg = &pdrinfohead;
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
      msg->addr_count=routeinfo->AliveNodesCount-(max_ele*count+1);
      if(msg->addr_count>max_ele)
          msg->addr_count=max_ele;
      //TODO find the bug
      if(count)
         msg->addr_count++;
      
      //Σ�ճ̶�
      
      if(frametype == FRAME_TYPE_PDR_INFO) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
      }
      
      memcpy(buf,msg,sizeof(pdr_info_head));
      bufferSize+=sizeof(pdr_info_head);
      
      
      for(i=1;i<=msg->addr_count;i++){
        node_info *nodeinfo = (routeinfo->node_info+(i+count*max_ele));
        buf[bufferSize++] = (nodeinfo->source_addr&0x00FF);
        buf[bufferSize++] = ((nodeinfo->source_addr >> 8) & 0x00FF);
        buf[bufferSize++] = (nodeinfo->receive_packet&0x00FF);
        buf[bufferSize++] = ((nodeinfo->receive_packet >> 8) & 0x00FF);
        buf[bufferSize++] = (nodeinfo->delay&0x00FF);
        buf[bufferSize++] = ((nodeinfo->delay >> 8) & 0x00FF);
      }
      PRINTF("%d addr_count %d  max_ele %d count %d i %d \r\n",routeinfo->AliveNodesCount,msg->addr_count,max_ele,count,i);
      return bufferSize;
}


/*********************************************************************************************************
** Function name:       pdr_info_frame_create
** Descriptions:        �Դ�������֡���ͽ���װ֡
** input parameters:    ֡��ַ ֡����
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void pdr_info_frame_pase(uint8_t *buf,uint8_t len)
{     
  uint8_t bufferSize;
  uint8_t i;
  uint8_t calc_len;
  pdr_info_head *pdrinfohead = (pdr_info_head *)buf;
  radio_para *radio = (radio_para *)&radiopara;
  pdr_addr_poll *pdraddrpoll = (pdr_addr_poll *)&__pdr_addr_poll;
      
  if(pdrinfohead->frame_type == FRAME_TYPE_PDR_INFO)
  {
    calc_len=(pdrinfohead->addr_count*sizeof(pdr_info_node) + sizeof(pdr_info_head));
    if(calc_len!=len)
    {
      PRINTF("len is %d calc is %d\r\n" ,len,calc_len);
      return;
    }
    bufferSize=sizeof(pdr_info_head); 
    //�԰��е���Ϣ���б���
    for(i=0;i<pdrinfohead->addr_count;i++)
    {
      pdr_info_node *pdrinfonode = (pdr_info_node *)(buf+bufferSize);
      bufferSize+=sizeof(pdr_info_node);
      //����յ�ͳ����Ϣ���ǸĽڵ㷢�ͳ�ȥ�İ�
      if(pdrinfonode->source_addr == radio->shortaddr)
      {
        //����Ƿ�֮ǰ�յ������֡
        if(check_exist(pdraddrpoll->node_count,pdraddrpoll->buf,pdrinfohead->src_addrl))
        {
          //�յ���ֱ�ӷ���
          return;
        }else{
          //û���յ�����ӵ�ַ��
          add_pdr_addr_poll(pdrinfohead->src_addrl);
        }
        //��ӡ������ַ
        uart_printf("source_addr%d  desc %d receive_packet %d delay %d \r\n",radio->shortaddr,
                                                            pdrinfohead->src_addrl,
                                                            pdrinfonode->receive_packet,
                                                            pdrinfonode->delay);
      }
    }
  
  }
    
    
  
}


void add_pdr_addr_poll(uint8_t id)
{
  pdr_addr_poll *pdraddrpoll = (pdr_addr_poll *)&__pdr_addr_poll;
  pdraddrpoll->buf[pdraddrpoll->node_count]=id;
  pdraddrpoll->node_count++;
}

uint8_t check_exist(uint8_t len,uint8_t *buf,uint8_t id)
{
  uint8_t i;
  //������鳤��Ϊ0 ֱ�ӷ���
  if(!len)
    return 0;
  //���buffer �д������֡�򷵻ش���
  for(i=0;i<len;i++)
  {
    if(buf[i]==id)
      return 1;
  }
  //��û���ҵ� ���ز�����
  return 0;
  
}
