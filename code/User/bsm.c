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
#include "phy_process.h"
#include "globalmacro.h"
/*********************************************************************************************************
** �Ƿ�ʹ�ܵ��Թ���
*********************************************************************************************************/
#define DEBUG 1
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
//������ӡ���Ա���
static struct ctimer ct2;

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
void addAliveNode(uint16_t addr)//���ID����ѡ�б�
{
  route_info *routeInfo = (route_info *)&__routeinfo ;
  
  PRINTF("addAliveNodeInfo %d %d\r\n",addr,routeInfo->AliveNodesCount);
  
  (routeInfo->node_info+routeInfo->AliveNodesCount)->source_addr = addr; //��ӵ�ַ
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
void addAliveNodeInfo(uint16_t pos,uint16_t addr,int16_t delay)//���ID����ѡ�б�
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
    PRINTF("noid %d recivecout %d delay %d\r\n",(routeInfo->node_info+i)->source_addr,
                                                (routeInfo->node_info+i)->receive_packet,
                                                  (routeInfo->node_info+i)->delay);
    }

}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
