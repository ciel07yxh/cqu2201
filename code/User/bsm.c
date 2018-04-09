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
#include "phy_process.h"
#include "globalmacro.h"
/*********************************************************************************************************
** 是否使能调试功能
*********************************************************************************************************/
#define DEBUG 1
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
//用来打印测试报告
static struct ctimer ct2;

/*********************************************************************************************************
** Function name:       pacet_info_statistics_init
** Descriptions:        初始化地址池
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void pacet_info_statistics_init(){
    route_info *routeInfo = (route_info *)&__routeinfo ;
    routeInfo->AliveNodesCount=1;
    ctimer_set(&ct2, PACKET_INTO_REPORT_TIME*CLOCK_SECOND,report_node_info, NULL); 
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
** Descriptions:        对传进来的帧类型进行装帧
** input parameters:    帧地址 帧类型
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-09
*********************************************************************************************************/

void report_node_info(){
  route_info *routeInfo = (route_info *)&__routeinfo;
  uint8_t i;
  
  for(i=1;i<routeInfo->AliveNodesCount;i++)
  {
    //计算时延
    (routeInfo->node_info+i)->delay =  (routeInfo->node_info+i)->delay_sum/(routeInfo->node_info+i)->receive_packet;
    PRINTF("noid %d recivecout %d delay %d\r\n",(routeInfo->node_info+i)->source_addr,
                                                (routeInfo->node_info+i)->receive_packet,
                                                  (routeInfo->node_info+i)->delay);
    }

}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
