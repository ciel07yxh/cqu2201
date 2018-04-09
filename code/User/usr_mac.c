/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               usr_mac.c
** Last modified Date:      2018-04-09
** Last Version:            v1.0
** Description:             用户自定义接管mac层回调函数
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              张校源
** Created date:            2018-04-09
** Version:                 v1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
/*********************************************************************************************************
** 是否使能调试功能
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#include <stdio.h>
#define PRINTF(...)   uart_printf(__VA_ARGS__)  //必须使用uart_printf（）函数
#else
#define PRINTF(...)
#endif
/*********************************************************************************************************
**  全局变量定义
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
** Descriptions:        自定义mac层初始化
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
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
** Descriptions:        自定义mac层接受函数
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/

void packet_input_arch(void)
{
    macfct *macpara = &mac;
    radio_para *radio = (radio_para *)&radiopara;
    
    //打印帧内容
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
    
    //收到符合长度的帧
    if(packetbuf_datalen()==sizeof(PhyRadioMsg))
    {
      PhyRadioMsg *recmeg =(PhyRadioMsg *)packetbuf_dataptr();
      //BUG 来自自己的帧会被接受
      if(recmeg->src_addrl == radio->shortaddr)
        return;
      
      //统计接收节点信息
#if PACKET_INFO_STATISTICS
      int16_t delay = (macpara->time_stamp- recmeg->time_stamp-macpara->time_offset);
      if(recmeg->src_addrl!=TIME_SYNCH_NODE)
        pacet_info_statistics(recmeg->src_addrl,delay);
#endif
      //开启时延测试功能，如果是使能了时间同步进程
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
       //对收到的帧进行相应
      switch(recmeg->frame_type){
      
      case FRAME_TYPE_TIME_SYNCH:
        //只同步一次
        if(mac.IsSyched){
          break;
        }
        
        
        macpara->IsSyched = true;
        //计算时间偏置
        macpara->timeoffset(macpara,recmeg->time_stamp);
        PRINTF("time-offset is %d the time is  %d \r\n",mac.time_offset,macpara->get_synch_time(macpara));
        //开启BSM发送

        
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