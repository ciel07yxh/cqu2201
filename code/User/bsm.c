/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               bsm.h
** Last modified Date:      2018-04-09
** Last Version:            v1.0
** Description:             BSM �����Լ�������ʱ�Ӳ���
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
#include "bsm.h"
#include "usr_mac.h"
#include "globalmacro.h"
#include "radio_para.h"
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
route_info __routeinfo;
//��������ʱ��ͬ��
static struct ctimer ct1;
//������ӡ���Ա���
static struct ctimer ct2;
static struct rtimer rt1;
/*********************************************************************************************************
** Function name:       pacet_info_statistics_init
** Descriptions:        ��ʼ����ַ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/

void pacet_info_statistics_init(){
    route_info *routeInfo = (route_info *)&__routeinfo ;
    routeInfo->AliveNodesCount=1;
    ctimer_set(&ct2, PACKET_INTO_REPORT_TIME*CLOCK_SECOND,report_node_info, NULL); 
}

/*********************************************************************************************************
** Function name:       pacet_info_statistics
** Descriptions:        �Խ��ܵ���BSM��Ϣ����ͳ��
** input parameters:    Դ��ַ ʱ��
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/
void pacet_info_statistics(uint16_t addr,int16_t delay)//�ж��Ƿ�ID�Ƿ����
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
** Descriptions:        �жϵ�ַ�����Ƿ����Դ��ַ
** input parameters:    Դ��ַ
** output parameters:   ���ڵ�λ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/
uint8_t CheckIfExist(uint16_t addr)//�ж��Ƿ�ID�Ƿ����
{
        route_info *routeInfo = (route_info *)&__routeinfo ;
	uint8_t exist = FALSE;
	uint8_t j;
        //�����нڵ����
	for(j = 0; j <= routeInfo->AliveNodesCount; j++){
	
		if((routeInfo->node_info+j)->source_addr == addr)
		{
                  exist = j;
		}
        }
return exist;
}

/*********************************************************************************************************
** Function name:       addAliveNode
** Descriptions:        ���ӵ�ַ����ַ��
** input parameters:    Դ��ַ
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/
void addAliveNode(uint16_t addr)//����ID����ѡ�б�
{
  route_info *routeInfo = (route_info *)&__routeinfo ;
  
  PRINTF("addAliveNodeInfo %d %d\r\n",addr,routeInfo->AliveNodesCount);
  
  (routeInfo->node_info+routeInfo->AliveNodesCount)->source_addr = addr; //���ӵ�ַ
  routeInfo->AliveNodesCount++;
}

/*********************************************************************************************************
** Function name:       addAliveNodeInfo
** Descriptions:        �Դ�������֡���ͽ���װ֡
** input parameters:    �����ַ Դ��ַ ��ʱ
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/
void addAliveNodeInfo(uint16_t pos,uint16_t addr,int16_t delay)//����ID����ѡ�б�
{
  route_info *routeInfo = (route_info *)&__routeinfo ;
  
  PRINTF("addAliveNodeInfo %d %d\r\n",pos,addr);
  
  if((routeInfo->node_info+pos)->source_addr = addr){
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
** Descriptions:        �Դ�������֡���ͽ���װ֡
** input parameters:    ֡��ַ ֡����
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/

void report_node_info(){
  route_info *routeInfo = (route_info *)&__routeinfo;
  uint8_t i;
  
  for(i=1;i<routeInfo->AliveNodesCount;i++)
  {
    //����ʱ��
    (routeInfo->node_info+i)->delay =  (routeInfo->node_info+i)->delay_sum/(routeInfo->node_info+i)->receive_packet;
    PRINTF("node %d recivecout %d delay %d\r\n",(routeInfo->node_info+i)->source_addr,
                                                (routeInfo->node_info+i)->receive_packet,
                                                  (routeInfo->node_info+i)->delay);
    }

}


/*********************************************************************************************************
** Function name:       bsm_transmit_csma_ca
** Descriptions:        ʹ��Ctimer ����csma_ca ���ڻص�����
** input parameters:    none
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/

void bsm_transmit_csma_ca(){


}


/*********************************************************************************************************
** Function name:       bsm_transmit_tdma
** Descriptions:        ʹ��Rtimer ����TDMA ���ڻص�����
** input parameters:    none
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-09
*********************************************************************************************************/

void bsm_transmit_tdma(){
  
   uint32_t now;
   int32_t temp;
   int r;
   uint16_t id = get_moteid();
   macfct *macpara = &mac;
   radio_para *radio = (radio_para *)&radiopara;
   PhyRadioMsg bsm;
#if BSM_FREAM_TEST
   
   if(radio->transmit_times >BSM_FRAME_TEST_TIMES)
   {
     report_radio_statistics();
     return;
     
   }
#endif
   
   frame_init(&bsm,FRAME_TYPE_BSM);
   NETSTACK_RADIO.send(&bsm,sizeof(PhyRadioMsg));
   //��ȡ����ʱ��
   now = RTIMER_NOW();
   //��ȡ��һ�η���ʱ��
   temp = PEROID_LENGTH-(now%PEROID_LENGTH)+(id%NR_SLOTS)*SLOT_LENGTH +macpara->time_offset_period_align;
   //��ʱ ���ݻص�����
   r=rtimer_set(&rt1, (rtimer_clock_t)(now+temp+GUARD_PERIOD),1,(rtimer_callback_t)bsm_transmit_tdma,NULL);
   
   PRINTF("tdma send %d %d %d %d \r\n",temp,now,GUARD_PERIOD,SLOT_LENGTH);
   if(r){
      PRINTF("rtimer error\r\n");
    }


}


/*********************************************************************************************************
** FFunction name:       ʱ��ͬ��
** Descriptions:        ����GPSʱ��ͬ���ź�ģ��
** input parameters:    ͬ��֡���ʹ���
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/
void time_synch_gps(void *ptr)
{
    uint8_t time_to_synch;
    if(ptr!=NULL)
    {
      PRINTF("time_synch_gps %d\r\n",(*(uint8_t *)ptr));
      time_to_synch=(*(uint8_t *)ptr)--;
      if(time_to_synch==0)
          return;
    }

      
    PhyRadioMsg time_synch_frame;
    frame_init(&time_synch_frame,FRAME_TYPE_TIME_SYNCH);
    NETSTACK_RADIO.send(&time_synch_frame,sizeof(PhyRadioMsg));
    ctimer_set(&ct1,CLOCK_SECOND,time_synch_gps, ptr); 
  
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/