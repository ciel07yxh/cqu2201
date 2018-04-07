/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               radio_para.c
** Last modified Date:      2018-04-07
** Last Version:            v1.0
** Description:             根据节点号初始化物理层参数
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              张校源
** Created date:            2018-04-07
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
#include "radio_para.h"
#include "globalmacro.h"
/*********************************************************************************************************
  全局变量定义
*********************************************************************************************************/
radio_para radiopara;

/*********************************************************************************************************
** Function name:       radio_para_init
** Descriptions:        物理层参数初始化
** input parameters:    0
** output parameters:   无
** Returned value:      moteid
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/
//TODO 根据节点号设置不同的功率

void radio_para_init(){
    radiopara.shortaddr = get_moteid();
    radiopara.pan_id = get_cluster_name(radiopara.shortaddr);
    radiopara.max_frame_retrise = DEFAULT_MAX_FRAME_RETRIES;
    radiopara.max_csma_retries = 0x03;
    radiopara.tx_power = 0x0E;
    radiopara.channel = DEFAULT_CHANNEL;
    radiopara.pancooder =DEEFAULT_COODER;
    if(get_moteid() == TIME_SYNCH_NODE )
      radiopara.tx_power = 0x02;
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
