/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_exti.h
** Last modified Date:  2013-04-04
** Last Version:        v1.0
** Description:         exti模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-04
** Version:             v1.0
** Descriptions:        The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#ifndef __ARCH_IO_EXTI_H__
#define __ARCH_IO_EXTI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void ExtiIntEnable(uint32 u32Line);
extern void ExtiIntDisable(uint32 u32Line);
extern void ExtiEventEnable(uint32 u32Line);
extern void ExtiEventDisable(uint32 u32Line);
extern void ExtiIntEventRisingEnable(uint32 u32Line);
extern void ExtiIntEventRisingDisable(uint32 u32Line);
extern void ExtiIntEventFallingEnable(uint32 u32Line);
extern void ExtiIntEventFallingDisable(uint32 u32Line);
extern void ExtiSoftInterruptTrigger(uint32 u32Line);
extern void ExtiIntEventClear(uint32 u32Line);


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_EXTI_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
