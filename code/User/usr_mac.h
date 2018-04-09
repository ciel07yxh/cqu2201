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
#ifndef _USR_MAC_H__ 
#define _USR_MAC_H__ 

#include "includes.h"
#include "usr_framer.h"
#include "radio_para.h"

#define NR_SLOTS        25                              //时隙分割数量
#define BSM_FRE_HZ      10                              //BSM 10Hz
#define PEROID_LENGTH   (RTIMER_SECOND/BSM_FRE_HZ)      //BSM 周期时间     
#define SLOT_LENGTH     (PEROID_LENGTH / NR_SLOTS)      //时隙长度
#define GUARD_PERIOD    (PEROID_LENGTH / (NR_SLOTS*2))  //保护间隔

void packet_input_arch(void);
void usr_mac_init(void);


#endif