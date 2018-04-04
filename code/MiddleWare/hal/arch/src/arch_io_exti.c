/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_exti.c
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
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_exti.h"


/*********************************************************************************************************
  局部变量定义
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       ExtiIntEnable
** Descriptions:        EXTI中断使能
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiIntEnable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_IMR) |= ((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntDisable
** Descriptions:        EXTI中断禁止
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiIntDisable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_IMR) &= ~((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiEventEnable
** Descriptions:        EXTI事件使能
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiEventEnable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_EMR) |= ((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiEventDisable
** Descriptions:        EXTI事件禁止
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiEventDisable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_EMR) &= ~((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntEventRisingEnable
** Descriptions:        EXTI中断及事件上升沿使能
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
** 注意：当上升沿和下降沿同时使能的时候，则同时有效
*********************************************************************************************************/
void ExtiIntEventRisingEnable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_RTSR) |= ((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntEventRisingDisable
** Descriptions:        EXTI中断及事件上升沿禁止
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
** 注意：当上升沿和下降沿同时使能的时候，则同时有效
*********************************************************************************************************/
void ExtiIntEventRisingDisable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_RTSR) &= ~((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntEventFallingEnable
** Descriptions:        EXTI中断及事件下降沿使能
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
** 注意：当上升沿和下降沿同时使能的时候，则同时有效
*********************************************************************************************************/
void ExtiIntEventFallingEnable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_FTSR) |= ((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntEventFallingDisable
** Descriptions:        EXTI中断及事件下降沿禁止
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
** 注意：当上升沿和下降沿同时使能的时候，则同时有效
*********************************************************************************************************/
void ExtiIntEventFallingDisable(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_FTSR) &= ~((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiSoftInterruptTrigger
** Descriptions:        EXTI中断及事件软件触发
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiSoftInterruptTrigger(uint32 u32Line)
{
    HWREG32(EXTI_BASE + EXTI_O_SWIER) |= ((uint32)1 << u32Line);
}

/*********************************************************************************************************
** Function name:       ExtiIntEventClear
** Descriptions:        EXTI中断及事件清除
** input parameters:    u32Line:    外部硬件中断号，取0-22
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void ExtiIntEventClear(uint32 u32Line)
{
     HWREG32(EXTI_BASE + EXTI_O_PR) |= ((uint32)1 << u32Line);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
