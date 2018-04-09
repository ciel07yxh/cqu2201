/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               bsm.h
** Last modified Date:      2018-04-09
** Last Version:            v1.0
** Description:             BSM 发送以及丢包率时延测试
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
#include "bsm.h"
#include "usr_mac.h"
#include "globalmacro.h"
#include "radio_para.h"
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
 全局变量
*********************************************************************************************************/
route_info __routeinfo;
//用来进行时间同步
static struct ctimer ct1;
//用来打印测试报告
static struct ctimer ct2;
//用来发送丢包率统计帧
static struct ctimer ct3;
static struct rtimer rt1;
#if BSM_FREAM_TEST
uint16_t send_time_count = BSM_FRAME_TEST_TIMES;
#endif
uint8_t pdr_frame_count;
uint8_t maxelement;
uint8_t pdr_send_count;
/*********************************************************************************************************
** Function name:       pacet_info_statistics_init
** Descriptions:        初始化地址池
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void pacet_info_statistics_init()
{
    route_info *routeInfo = (route_info *)&__routeinfo ;
    routeInfo->AliveNodesCount=1;
    maxelement = (127-sizeof(pdr_info_head))/sizeof(pdr_info_node);
    //ctimer_set(&ct2, PACKET_INTO_REPORT_TIME*CLOCK_SECOND,report_node_info, NULL); 
}

/*********************************************************************************************************
** Function name:       pacet_info_statistics
** Descriptions:        对接受到的BSM消息进行统计
** input parameters:    源地址 时延
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/
void pacet_info_statistics(uint16_t addr,int16_t delay)//判断是否ID是否存在
{
      uint8_t pos;
      pos = CheckIfExist(addr);
      if(pos)
      {
        addAliveNodeInfo(pos,addr,delay);
      }
      else
      {
        addAliveNode(addr);
      }
}


/*********************************************************************************************************
** Function name:       CheckIfExist
** Descriptions:        判断地址池中是否存在源地址
** input parameters:    源地址
** output parameters:   存在的位置
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/
uint8_t CheckIfExist(uint16_t addr)//判断是否ID是否存在
{
        route_info *routeInfo = (route_info *)&__routeinfo ;
	uint8_t exist = FALSE;
	uint8_t j;
        //对所有节点遍历
	for(j = 0; j <= routeInfo->AliveNodesCount; j++)
        {
	
		if((routeInfo->node_info+j)->source_addr == addr)
		{
                  exist = j;
		}
        }
return exist;
}

/*********************************************************************************************************
** Function name:       addAliveNode
** Descriptions:        增加地址到地址池
** input parameters:    源地址
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/
void addAliveNode(uint16_t addr)//添加ID进候选列表
{
    route_info *routeInfo = (route_info *)&__routeinfo ;
  
    PRINTF("addAliveNodeInfo %d %d\r\n",addr,routeInfo->AliveNodesCount);
  
    (routeInfo->node_info+routeInfo->AliveNodesCount)->source_addr = addr; //添加地址
     routeInfo->AliveNodesCount++;
}

/*********************************************************************************************************
** Function name:       addAliveNodeInfo
** Descriptions:        对传进来的帧类型进行装帧
** input parameters:    缓存地址 源地址 延时
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/
void addAliveNodeInfo(uint16_t pos,uint16_t addr,int16_t delay)//添加ID进候选列表
{
  route_info *routeInfo = (route_info *)&__routeinfo ;
  
  PRINTF("addAliveNodeInfo %d %d\r\n",pos,addr);
  
  if((routeInfo->node_info+pos)->source_addr = addr)
  {
    (routeInfo->node_info+pos)->receive_packet++;
    (routeInfo->node_info+pos)->delay_sum+=delay;
  }
  else
  {
    PRINTF("address not match %d %d %d\r\r",pos,addr);
  }
}
/*********************************************************************************************************
** Function name:       report_node_info
** Descriptions:        对传进来的帧类型进行装帧
** input parameters:    帧地址 帧类型
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void report_node_info()
{
  route_info *routeInfo = (route_info *)&__routeinfo;
  uint8_t i;
  
  for(i=1;i<routeInfo->AliveNodesCount;i++)
  {
    //计算时延
    (routeInfo->node_info+i)->delay =  (routeInfo->node_info+i)->delay_sum/(routeInfo->node_info+i)->receive_packet;
    PRINTF("info %d recivecout %d delay %d\r\n",(routeInfo->node_info+i)->source_addr,
                                                (routeInfo->node_info+i)->receive_packet,
                                                  (routeInfo->node_info+i)->delay);
   }

}


/*********************************************************************************************************
** Function name:       bsm_transmit_csma_ca
** Descriptions:        使用Ctimer 进行csma_ca 周期回调发送
** input parameters:    none
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void bsm_transmit_csma_ca(void *ptr)
{
    static struct ctimer ct3;
    PhyRadioMsg bsm;
      
    PRINTF("send_time_count %d\r\n",(*(uint16_t *)ptr));
#if BSM_FREAM_TEST
    route_info *routeinfo = (route_info *)&__routeinfo;
    uint8_t nodes_to_send =routeinfo->AliveNodesCount-1;
    PRINTF("send_time_count %d\r\n",(*(uint16_t *)ptr));
    send_time_count--;
    if(!send_time_count)
    {
        report_radio_statistics();
        report_node_info();
        
        if( nodes_to_send <= maxelement)
          pdr_frame_count = 1;
        if(nodes_to_send>maxelement)
          pdr_frame_count = (nodes_to_send/maxelement)+1;
        if(nodes_to_send>maxelement && (nodes_to_send%maxelement==0) )
          pdr_frame_count-=1;      
        ctimer_set(&ct3,CLOCK_SECOND*10,pdr_info_send,NULL); 
        return;
    }
#endif
    frame_init(&bsm,FRAME_TYPE_BSM);
    NETSTACK_RADIO.send(&bsm,sizeof(PhyRadioMsg));
    
    ctimer_set(&ct3, CLOCK_SECOND/10,bsm_transmit_csma_ca,NULL); 
}


/*********************************************************************************************************
** Function name:       bsm_transmit_tdma
** Descriptions:        使用Rtimer 进行TDMA 周期回调发送
** input parameters:    none
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void bsm_transmit_tdma(void *ptr)
{
  
    uint32_t now;
    int32_t temp;
    int r;
    uint16_t id = get_moteid();
    macfct *macpara = &mac;
    
    PhyRadioMsg bsm;
   
    
#if BSM_FREAM_TEST
    route_info *routeinfo = (route_info *)&__routeinfo;
    uint8_t nodes_to_send =routeinfo->AliveNodesCount-1;
    PRINTF("send_time_count %d\r\n",(*(uint16_t *)ptr));
    send_time_count--;
    if(!send_time_count)
    {
        report_radio_statistics();
        report_node_info();
        
        if( nodes_to_send <= maxelement)
          pdr_frame_count = 1;
        if(nodes_to_send>maxelement)
          pdr_frame_count = (nodes_to_send/maxelement)+1;
        if(nodes_to_send>maxelement && (nodes_to_send%maxelement==0) )
          pdr_frame_count-=1;
        ctimer_set(&ct3,CLOCK_SECOND*10,pdr_info_send,NULL); 
        return;
    }
#endif
    
   
   frame_init(&bsm,FRAME_TYPE_BSM);
   NETSTACK_RADIO.send(&bsm,sizeof(PhyRadioMsg));
   //获取现在时钟
   now = RTIMER_NOW();
   //获取下一次发送时钟
   temp = PEROID_LENGTH-(now+macpara->time_offset_period_align)%PEROID_LENGTH+(id%NR_SLOTS)*SLOT_LENGTH ;
   //定时 传递回调函数
   r=rtimer_set(&rt1, (rtimer_clock_t)(now+temp+GUARD_PERIOD),1,(rtimer_callback_t)bsm_transmit_tdma,NULL);
   
   PRINTF("tdma send %d %d %d %d %d \r\n",temp,now,GUARD_PERIOD,SLOT_LENGTH,
                                                    macpara->time_offset_period_align
                                                     );
   if(r){
      PRINTF("rtimer error\r\n");
    }


}


/*********************************************************************************************************
** FFunction name:       时间同步
** Descriptions:        进行GPS时间同步信号模拟
** input parameters:    同步帧发送次数
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/
void time_synch_gps(void *ptr)
{
    uint8_t time_to_synch;
    if(ptr!=NULL)
    {
      PRINTF("time_synch_gps %d\r\n",(*(uint8_t *)ptr));
      time_to_synch=(*(uint8_t *)ptr)--;
      if(!time_to_synch)
          return;
    }

      
    PhyRadioMsg time_synch_frame;
    frame_init(&time_synch_frame,FRAME_TYPE_TIME_SYNCH);
    NETSTACK_RADIO.send(&time_synch_frame,sizeof(PhyRadioMsg));
    ctimer_set(&ct1,CLOCK_SECOND,time_synch_gps, ptr); 
  
}
/*********************************************************************************************************
** FFunction name:       pdr_info_send
** Descriptions:        PDR统计信息帧发送
** input parameters:    无
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-10
*********************************************************************************************************/


void pdr_info_send(void *ptr)
{
    uint8_t buffer[128];
    uint8_t bufferSize=0;
    
    if(pdr_send_count>=pdr_frame_count)
        pdr_send_count=0;
    
    bufferSize=pdr_info_frame_create(FRAME_TYPE_PDR_INFO,
                                                    buffer,
                                                    pdr_send_count,maxelement,
                                                    &__routeinfo);

    NETSTACK_RADIO.send(buffer,bufferSize);
    pdr_send_count++;
    ctimer_set(&ct3,CLOCK_SECOND/10,pdr_info_send,NULL); 
  
}



/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/




