/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               usr_mac.c
** Last modified Date:      2018-04-09
** Last Version:            v1.0
** Description:             �û��Զ���ӹ�mac��ص�����
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              ��УԴ
** Created date:            2018-04-09
** Version:                 v1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
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
#include "globalmacro.h"
#include "radio_para.h"
#include "usr_framer.h"
#include "usr_mac.h"
#include "bsm.h"
static struct ctimer ct;
uint16_t bsm_send_times = BSM_FRAME_TEST_TIMES;
/*********************************************************************************************************
** Function name:       usr_mac_init
** Descriptions:        �Զ���mac���ʼ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void usr_mac_init(void)
{
    moteid_init();
#if PACKET_INFO_STATISTICS
    pacet_info_statistics_init();
#endif  
    NETSTACK_RADIO.init();
    NETSTACK_RADIO.on();
#if TIME_STAMP
    static uint8_t times = TIME_SYNCH_TIMES;

     if(get_moteid()==TIME_SYNCH_NODE)
       time_synch_gps((void *)&times);
#endif
}
/*********************************************************************************************************
** Function name:       packet_input_arch
** Descriptions:        �Զ���mac����ܺ���
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/

void packet_input_arch(void)
{
    macfct *macpara = &mac;
    radio_para *radio = (radio_para *)&radiopara;
    
    //��ӡ֡����
#if PRINT_FRAME
      uint8_t *buf;
      uint8_t i;
    buf=(uint8_t *)packetbuf_dataptr();
   
    for(i=0;i<packetbuf_datalen();i++){
       uart_printf(" %X ",*(buf+i));
    }
      uart_printf("\r\n");
    
#endif 
    
      pdr_info_frame_pase((uint8_t *)packetbuf_dataptr(),packetbuf_datalen());
    
    //�յ����ϳ��ȵ�֡
    if(packetbuf_datalen()==sizeof(PhyRadioMsg))
    {
      PhyRadioMsg *recmeg =(PhyRadioMsg *)packetbuf_dataptr();
      //BUG �����Լ���֡�ᱻ����
      if(recmeg->src_addrl == radio->shortaddr)
        return;
      
      //ͳ�ƽ��սڵ���Ϣ
#if PACKET_INFO_STATISTICS
      int16_t delay = (macpara->time_stamp- recmeg->time_stamp-macpara->time_offset);
      if(recmeg->src_addrl!=TIME_SYNCH_NODE)
        pacet_info_statistics(recmeg->src_addrl,delay);
#endif
      //����ʱ�Ӳ��Թ��ܣ������ʹ����ʱ��ͬ������
#if PACKET_DELAY
#if TIME_STAMP
        
        if(macpara->IsSyched && recmeg->src_addrl!=TIME_SYNCH_NODE)
        {
          
          PRINTF("receive %d to %d delay is %d \r\n",recmeg->src_addrl,
                                                    radio->shortaddr,
                                                    (macpara->time_stamp- recmeg->time_stamp-macpara->time_offset));
        };
#endif
#endif
       //���յ���֡������Ӧ
      switch(recmeg->frame_type){
      
      case FRAME_TYPE_TIME_SYNCH:
        //ֻͬ��һ��
        if(mac.IsSyched){
          break;
        }
        
        
        macpara->IsSyched = true;
        //����ʱ��ƫ��
        macpara->timeoffset(macpara,recmeg->time_stamp);
        PRINTF("time-offset is %d the time is  %d \r\n",mac.time_offset,macpara->get_synch_time(macpara));
        //����BSM����

        
#if TDMA_BSM_ENABLE
        PRINTF("BSM TDMA send  \r\n");
        ctimer_set(&ct, BSM_START_TIME*CLOCK_SECOND,bsm_transmit_tdma,(void *)&bsm_send_times);
#else
        PRINTF("BSM CSMA/CA send\r\n");
        ctimer_set(&ct, BSM_START_TIME*CLOCK_SECOND,bsm_transmit_csma_ca,(void *)&bsm_send_times);
        
#endif 
    
         
        break;
        
        case FRAME_TYPE_BSM:
        PRINTF("receive bsm from %d time %d \r\n",recmeg->moteid,macpara->get_synch_time(macpara));
        break;
      }
      
     
    }
    
 
}