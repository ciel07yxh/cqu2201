/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           phy_process.c
** Last modified Date:  2018-04-06
** Last Version:        v1.0
** Description:         �����˽��պͷ����������̣����������нӹ�
**
**--------------------------------------------------------------------------------------------------------
** Created By:          ԬС��
** Created date:        2018-04-06
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
#include "includes.h"
#include "phy_process.h"
#include "globalmacro.h"
#include "contiki.h"
#include <string.h>
#include "runtime/uartstdio.h"
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
**  ȫ�ֱ�������
*********************************************************************************************************/
//�����˶����ķ���802.15.4Э�������֡
//TODO:��̬װ֡������֡��ʽ������moteid���е�ַ��䡢Ӳ����ַ����
uint8_t send_buff[23]={ 0x61, 0x88, 1, 0x02, 0x03,0xff, 0xff, 0x02, 0x04,
'H', 'e', 'l', 'l', 'o', ' ', '8', '0', '2', '.', '1', '5', '.', '4'};
PhyRadioMsg frame;
macfct mac={
  0,
  0,
  0,
timeoffset_calc,
get_synch_time
};

static struct rtimer rt2;
/*********************************************************************************************************
**  �ڲ���������
*********************************************************************************************************/

/*********************************************************************************************************
** Process name:       phy_send_process
** Descriptions:        ���ݷ��ͽ��̶��壬�����Է�������֡�������ڴ�ӡ��sending��
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
PROCESS(phy_send_process, "Flooding");
PROCESS_THREAD(phy_send_process, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer et1;
  while(1){
   //frame_init(&frame,FRAME_TYPE_BSM);
   etimer_set(&et1, CLOCK_SECOND);
   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et1));
   //PRINTF("sending\r\n"); /*�򴮿ڴ�ӡ�ַ���"Hello,world"*/ 
    //NETSTACK_RADIO.send(&frame,sizeof(PhyRadioMsg));
  }
  PROCESS_END();
}

/*********************************************************************************************************
** Process name:       phy_receive_process
** Descriptions:        ���ݽ��ս��̶���,���յ����ݺ󼴽���ת��
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
PROCESS(phy_receive_process, "phy_receive_process");
PROCESS_THREAD(phy_receive_process, ev, data)
{
  PROCESS_BEGIN();

  PRINTF("start process\r\n");
  while(1){
      PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);
      PRINTF("recving\r\n");
      
     
    }
  PROCESS_END();
}

/*********************************************************************************************************
** Function name:       packet_input_arch
** Descriptions:        �������յ�֡���д���Ļص�����
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void packet_input_arch(void){
    macfct *macpara = &mac;
    radio_para *radio = (radio_para *)&radiopara;
    
    
#if PRINT_FRAME
      uint8_t *buf;
      uint8_t i;
    buf=(uint8_t *)packetbuf_dataptr();
   
    for(i=0;i<packetbuf_datalen();i++){
       uart_printf(" %X ",*(buf+i));
    }
      uart_printf("\r\n");
    
#endif 
    

    //PhyRadioMsg *recmeg =(PhyRadioMsg *)packetbuf_dataptr();
    

     
    if(packetbuf_datalen()==sizeof(PhyRadioMsg))
    {
      PhyRadioMsg *recmeg =(PhyRadioMsg *)packetbuf_dataptr();
      //BUG �����Լ���֡�ᱻ����
      if(recmeg->src_addrl == radio->shortaddr)
        return;
    
#if PACKET_DELAY
#if TIME_STAMP
        
        if(macpara->IsSyched && recmeg->src_addrl!=TIME_SYNCH_NODE)
          uart_printf("receive %d to %d delay is %d \r\n",recmeg->src_addrl,
                                                    radio->shortaddr,
                                                    (macpara->time_stamp- recmeg->time_stamp-macpara->time_offset));
#endif
#endif
      switch(recmeg->frame_type){
      
      case FRAME_TYPE_TIME_SYNCH:
        if(mac.IsSyched){
          //tdmasend(NULL); 
          break;
        }
        
        
        macpara->IsSyched = true;
        //����ʱ��ƫ��
        macpara->timeoffset(macpara,recmeg->time_stamp);
        PRINTF("time-offset is %d the time is  %d \r\n",mac.time_offset,macpara->get_synch_time(macpara));
        tdmasend(NULL);      
        break;
        
        case FRAME_TYPE_BSM:
        //uart_printf("receive bsm from %d time %d \r\n",recmeg->moteid,macpara->get_synch_time(macpara));
        break;
      }
      
     
    }
    
 
}

/*********************************************************************************************************
** Function name:       get_synch_time
** Descriptions:        ��ȡͬ��ʱ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/


rtimer_clock_t get_synch_time(macfct *macpara){
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


void timeoffset_calc(macfct *macpara,uint32_t time){
  macpara->time_offset = RTIMER_NOW() -time;
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

void frame_init(PhyRadioMsg * msg,uint16_t frametype){
      radio_para *radio = (radio_para *)&radiopara;
      macfct *macpara = &mac;
      msg->fcfl=0x61;
      msg->fcfh=0x88;
      msg->seq=1;
      msg->dest_pidl=(radio->pan_id & 0x00FF) ;
      msg->dest_pidh=((radio->pan_id >> 8) & 0x00FF) ;
      msg->des_addrl=0x40;
      msg->des_addrh=0x12;
      msg->src_addrl=(radio->shortaddr & 0x00FF);
      msg->src_addrh=((radio->shortaddr >> 8) & 0x00FF) ;
      msg->frame_type=frametype;
      msg->danger=random_rand()%200;
      msg->moteid=get_moteid();
      msg->network_id = get_cluster_name(get_moteid());
      msg->time_stamp=macpara->get_synch_time(macpara);
      
      //�㲥֡����Frame Control Field �Լ�Ŀ�ĵ�ַ
      if(frametype == FRAME_TYPE_BSM ) 
      {
        msg->fcfl = 0x41;   
        msg->des_addrl = 0xFF;
        msg->des_addrh = 0xFF;
      }
      
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
** Process name:        ʱ��ͬ������
** Descriptions:        �Թ涨�Ľڵ����GPSʱ��ͬ���ź�ģ��
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
PROCESS(time_synch_process, "time_synch");
PROCESS_THREAD(time_synch_process, ev, data)
{
  PROCESS_BEGIN();
  while(1)
   {
      static struct etimer et2;
      etimer_set(&et2, CLOCK_SECOND/2);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et2));
      if(get_moteid()==TIME_SYNCH_NODE){
      uart_printf("time_synch_process\r\n");
      frame_init(&frame,FRAME_TYPE_TIME_SYNCH);
     
      NETSTACK_RADIO.send(&frame,sizeof(PhyRadioMsg));

    }
  }
 
   PROCESS_END();
}

/*********************************************************************************************************
** Function name:       tdmasend
** Descriptions:        �ڹ涨��ʱ�����ڽ���TDMA֡����
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void tdmasend(void *ptr){
   uint32_t now, temp;

   int r;
   
   macfct *macpara = &mac;
   
   frame_init(&frame,FRAME_TYPE_BSM);
   NETSTACK_RADIO.send(&frame,sizeof(PhyRadioMsg));
   now =macpara->get_synch_time(macpara);
   
   temp = PEROID_LENGTH-(now%PEROID_LENGTH)+(get_moteid()%10)*SLOT_LENGTH;
   r=rtimer_set(&rt2, (rtimer_clock_t)(now+temp+GUARD_PERIOD),1,(rtimer_callback_t)tdmasend,NULL);
   
   if(r){
      uart_printf("rtimer error\r\n");
    }

   //uart_printf("tdma send %d %d\r\n",moteid,time_synch_rec((uint8_t *)send_buff));
}

