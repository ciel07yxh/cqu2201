/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               rtime-arch.h
** Last modified Date:      2012-07-03
** Last Version:            v1.0
** Description:             Hardware-dependent function declarations used to support the 
**                          contiki rtimer module.
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              任海波
** Created date:            2012-07-03
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
#ifndef __RTIMER_ARCH_H__
#define __RTIMER_ARCH_H__

#include "contiki-conf.h"
#include "sys/rtimer.h"
#include "sys/clock.h"

#include "infra/rhb_type.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_ints.h"
#include "hal/CortexM3/hal_io_level.h"


#define RTIMER_ARCH_USE_BASE  TIM2_BASE   // 使用TIM1作为rtimer定时器
#define RTIMER_INT_NO         INT_TIM2    // 中断编号
#define RT_PRESCALER          639        // 定时器的预分频值，PCLK1/4096 = 32M / (639 + 1) = 50000 Hz
#define RTIMER_ARCH_SECOND    50000        // One tick: 20 us. 

void rtimer_arch_set(rtimer_clock_t t);

rtimer_clock_t rtimer_arch_now(void);


#endif // __RTIMER_ARCH_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/