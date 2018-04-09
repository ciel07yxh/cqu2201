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
#ifndef _USR_MAC_H__ 
#define _USR_MAC_H__ 

#include "includes.h"
#include "usr_framer.h"
#include "radio_para.h"

#define NR_SLOTS        25                              //ʱ϶�ָ�����
#define BSM_FRE_HZ      10                              //BSM 10Hz
#define PEROID_LENGTH   (RTIMER_SECOND/BSM_FRE_HZ)      //BSM ����ʱ��     
#define SLOT_LENGTH     (PEROID_LENGTH / NR_SLOTS)      //ʱ϶����
#define GUARD_PERIOD    (PEROID_LENGTH / (NR_SLOTS*2))  //�������

void packet_input_arch(void);
void usr_mac_init(void);


#endif