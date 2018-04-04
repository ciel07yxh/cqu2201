/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               clock.c
** Last modified Date:      2012-06-21
** Last Version:            v1.0
** Description:             操作系统时钟tick初始化，以及tick处理函数实现
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              任海波
** Created date:            2012-06-21
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
** 头文件包含
*********************************************************************************************************/
#include <stdio.h>
#include "sys/clock.h"
#include "sys/cc.h"
#include "sys/etimer.h"
#include "sys/energest.h"

#include "hal/arch/inc/arch_io_rcc.h"

/*********************************************************************************************************
** 与Cortex-M3内核相关的宏定义
*********************************************************************************************************/
#define  OS_CPU_CM3_NVIC_ST_CTRL    (*((volatile uint32_t *)0xE000E010))   /* SysTick Ctrl & Status Reg. */
#define  OS_CPU_CM3_NVIC_ST_RELOAD  (*((volatile uint32_t *)0xE000E014))   /* SysTick Reload  Value Reg. */
#define  OS_CPU_CM3_NVIC_ST_CURRENT (*((volatile uint32_t *)0xE000E018))   /* SysTick Current Value Reg. */
#define  OS_CPU_CM3_NVIC_ST_CAL     (*((volatile uint32_t *)0xE000E01C))   /* SysTick Cal     Value Reg. */
#define  OS_CPU_CM3_NVIC_PRIO_ST    (*((volatile uint8_t  *)0xE000ED23))   /* SysTick Handler Prio  Reg. */

#define  OS_CPU_CM3_NVIC_ST_CTRL_COUNT                    0x00010000     /* Count flag.                */
#define  OS_CPU_CM3_NVIC_ST_CTRL_CLK_SRC                  0x00000000     /* Clock Source, 使用外部的   */
#define  OS_CPU_CM3_NVIC_ST_CTRL_INTEN                    0x00000002     /* Interrupt enable.          */
#define  OS_CPU_CM3_NVIC_ST_CTRL_ENABLE                   0x00000001     /* Counter mode.              */
#define  OS_CPU_CM3_NVIC_PRIO_MIN                               0xFF     /* Min handler prio.          */

/*********************************************************************************************************
** 全局变量定义
*********************************************************************************************************/
static volatile clock_time_t current_clock = 0;
static volatile clock_time_t current_seconds = 0;
static clock_time_t second_countdown = CLOCK_SECOND;

/*********************************************************************************************************
** Function name:       clock_delay
** Descriptions:        时钟延时控制,一次延时len*2 + 1个时钟
** Input parameters:    len： 延时参数
** Output parameters:   无
** Returned value:      无
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
void clock_delay(unsigned int len)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   clock_delay\n"
          "    bx      lr");
}

/*********************************************************************************************************
** Function name:       clock_wait
** Descriptions:        Wait for a multiple of ~8 ms (a tick)
** Input parameters:    t: 等待参数
** Output parameters:   无
** Returned value:      无
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
void clock_wait(clock_time_t t)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)t);
}

/*********************************************************************************************************
** Function name:       clock_time
** Descriptions:        获取时钟计数
** Input parameters:    无
** Output parameters:   无
** Returned value:      当前时钟计数值
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
CCIF clock_time_t clock_time(void)
{
  return current_clock;
}

/*********************************************************************************************************
** Function name:       clock_seconds
** Descriptions:        获取当前时钟秒数
** Input parameters:    无
** Output parameters:   无
** Returned value:      当前时钟计数值，以秒为单位
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
CCIF unsigned long clock_seconds(void)
{
  return current_seconds;
}

/*********************************************************************************************************
** Function name:       clock_init
** Descriptions:        时钟初始化
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
void clock_init(void)
{
    uint32_t  cnts;

    cnts = SystemSysTickClockGet() / CLOCK_SECOND;

    OS_CPU_CM3_NVIC_ST_RELOAD = (cnts - 1);
                                                 /* Set prio of SysTick handler to min prio.           */
    OS_CPU_CM3_NVIC_PRIO_ST   = OS_CPU_CM3_NVIC_PRIO_MIN;
                                                 /* Enable timer.                                      */
    OS_CPU_CM3_NVIC_ST_CTRL  |= OS_CPU_CM3_NVIC_ST_CTRL_CLK_SRC | OS_CPU_CM3_NVIC_ST_CTRL_ENABLE;
                                                 /* Enable timer interrupt.                            */
    OS_CPU_CM3_NVIC_ST_CTRL  |= OS_CPU_CM3_NVIC_ST_CTRL_INTEN;
}

/*********************************************************************************************************
** Function name:       OS_CPU_SysTickHandler
** Descriptions:        时钟tick中断服务函数
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
void  OS_CPU_SysTickHandler (void)
{
  current_clock++;
  
  if(etimer_pending() && etimer_next_expiration_time() <= current_clock) {
    etimer_request_poll();
    /* printf("%d,%d\n", clock_time(),etimer_next_expiration_time  	()); */

  }
  if (--second_countdown == 0) {
    current_seconds++;
    second_countdown = CLOCK_SECOND;
  }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

