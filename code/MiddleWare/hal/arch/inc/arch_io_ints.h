/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_ints.h
** Last modified Date:  2013-04-01
** Last Version:        v1.0
** Description:         中断源相关定义
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-01
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

#ifndef __ARCH_IO_INTS_H__
#define __ARCH_IO_INTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal/CortexM3/hal_io_ints.h"

/*********************************************************************************************************
**  本平台的中断向量表个数
*********************************************************************************************************/
#define ARCH_NUM_INTERRUPTS     76

/*********************************************************************************************************
**  本平台的中断优先级划分
*********************************************************************************************************/
#define ARCH_NUM_PRIORITY       8
#define ARCH_NUM_PRIORITY_BITS  4

/*********************************************************************************************************
** 本平台支持的外部中断编号定义
*********************************************************************************************************/
#define ARCH_INT_WWDG                (HAL_EXTERN_INT_BASE + 0)          //  WWDG_IRQHandler
#define ARCH_INT_PVD                 (HAL_EXTERN_INT_BASE + 1)          //  PVD_IRQHandler
#define ARCH_INT_TAMPER              (HAL_EXTERN_INT_BASE + 2)          //  TAMPER_IRQHandler
#define ARCH_INT_RTC                 (HAL_EXTERN_INT_BASE + 3)          //  RTC_IRQHandler
#define ARCH_INT_FLASH               (HAL_EXTERN_INT_BASE + 4)          //  FLASH_IRQHandler
#define ARCH_INT_RCC                 (HAL_EXTERN_INT_BASE + 5)          //  RCC_IRQHandler
#define ARCH_INT_EXTI0               (HAL_EXTERN_INT_BASE + 6)          //  EXTI0_IRQHandler
#define ARCH_INT_EXTI1               (HAL_EXTERN_INT_BASE + 7)          //  EXTI1_IRQHandler
#define ARCH_INT_EXTI2               (HAL_EXTERN_INT_BASE + 8)          //  EXTI2_IRQHandler
#define ARCH_INT_EXTI3               (HAL_EXTERN_INT_BASE + 9)          //  EXTI3_IRQHandler
#define ARCH_INT_EXTI4               (HAL_EXTERN_INT_BASE + 10)          //  EXTI4_IRQHandler
#define ARCH_INT_DMA1_Channel1       (HAL_EXTERN_INT_BASE + 11)          //  DMA1_Channel1_IRQHandler
#define ARCH_INT_DMA1_Channel2       (HAL_EXTERN_INT_BASE + 12)          //  DMA1_Channel2_IRQHandler
#define ARCH_INT_DMA1_Channel3       (HAL_EXTERN_INT_BASE + 13)          //  DMA1_Channel3_IRQHandler
#define ARCH_INT_DMA1_Channel4       (HAL_EXTERN_INT_BASE + 14)          //  DMA1_Channel4_IRQHandler
#define ARCH_INT_DMA1_Channel5       (HAL_EXTERN_INT_BASE + 15)          //  DMA1_Channel5_IRQHandler
#define ARCH_INT_DMA1_Channel6       (HAL_EXTERN_INT_BASE + 16)          //  DMA1_Channel6_IRQHandler
#define ARCH_INT_DMA1_Channel7       (HAL_EXTERN_INT_BASE + 17)          //  DMA1_Channel7_IRQHandler
#define ARCH_INT_ADC1_2              (HAL_EXTERN_INT_BASE + 18)          //  ADC1_2_IRQHandlerr
#define ARCH_INT_USB_HP_CAN_TX       (HAL_EXTERN_INT_BASE + 19)          //  USB_HP_CAN_TX_IRQHandler
#define ARCH_INT_USB_LP_CAN_RX0      (HAL_EXTERN_INT_BASE + 20)          //  USB_LP_CAN_RX0_IRQHandler
#define ARCH_INT_CAN_RX1             (HAL_EXTERN_INT_BASE + 21)          //  CAN_RX1_IRQHandler
#define ARCH_INT_CAN_SCE             (HAL_EXTERN_INT_BASE + 22)          //  CAN_SCE_IRQHandler
#define ARCH_INT_EXTI9_5             (HAL_EXTERN_INT_BASE + 23)          //  EXTI9_5_IRQHandler
#define ARCH_INT_TIM1_BRK            (HAL_EXTERN_INT_BASE + 24)          //  TIM1_BRK_IRQHandler
#define ARCH_INT_TIM1_UP             (HAL_EXTERN_INT_BASE + 25)          //  TIM1_UP_IRQHandler
#define ARCH_INT_TIM1_TRG_COM        (HAL_EXTERN_INT_BASE + 26)          //  TIM1_TRG_COM_IRQHandler
#define ARCH_INT_TIM1_CC             (HAL_EXTERN_INT_BASE + 27)          //  TIM1_CC_IRQHandler
#define ARCH_INT_TIM2                (HAL_EXTERN_INT_BASE + 28)          //  TIM2_IRQHandler
#define ARCH_INT_TIM3                (HAL_EXTERN_INT_BASE + 29)          //  TIM3_IRQHandler
#define ARCH_INT_TIM4                (HAL_EXTERN_INT_BASE + 30)          //  TIM4_IRQHandler
#define ARCH_INT_I2C1_EV             (HAL_EXTERN_INT_BASE + 31)          //  I2C1_EV_IRQHandler
#define ARCH_INT_I2C1_ER             (HAL_EXTERN_INT_BASE + 32)          //  I2C1_ER_IRQHandler
#define ARCH_INT_I2C2_EV             (HAL_EXTERN_INT_BASE + 33)          //  I2C2_EV_IRQHandler
#define ARCH_INT_I2C2_ER             (HAL_EXTERN_INT_BASE + 34)          //  I2C2_ER_IRQHandler
#define ARCH_INT_SPI1                (HAL_EXTERN_INT_BASE + 35)          //  SPI1_IRQHandler
#define ARCH_INT_SPI2                (HAL_EXTERN_INT_BASE + 36)          //  SPI2_IRQHandler
#define ARCH_INT_USART1              (HAL_EXTERN_INT_BASE + 37)          //  USART1_IRQHandler
#define ARCH_INT_USART2              (HAL_EXTERN_INT_BASE + 38)          //  USART2_IRQHandler
#define ARCH_INT_USART3              (HAL_EXTERN_INT_BASE + 39)          //  USART3_IRQHandler
#define ARCH_INT_EXTI15_10           (HAL_EXTERN_INT_BASE + 40)          //  EXTI15_10_IRQHandler
#define ARCH_INT_RTCAlarm            (HAL_EXTERN_INT_BASE + 41)          //  RTCAlarm_IRQHandler
#define ARCH_INT_USBWakeUp           (HAL_EXTERN_INT_BASE + 42)          //  USBWakeUp_IRQHandler
#define ARCH_INT_TIM8_BRK            (HAL_EXTERN_INT_BASE + 43)          //  TIM8_BRK_IRQHandler
#define ARCH_INT_TIM8_UP             (HAL_EXTERN_INT_BASE + 44)          //  TIM8_UP_IRQHandler
#define ARCH_INT_TIM8_TRG_COM        (HAL_EXTERN_INT_BASE + 45)          //  TIM8_TRG_COM_IRQHandler
#define ARCH_INT_TIM8_CC             (HAL_EXTERN_INT_BASE + 46)          //  TIM8_CC_IRQHandler
#define ARCH_INT_ADC3                (HAL_EXTERN_INT_BASE + 47)          //  ADC3_IRQHandler
#define ARCH_INT_FSMC                (HAL_EXTERN_INT_BASE + 48)          //  FSMC_IRQHandler
#define ARCH_INT_SDIO                (HAL_EXTERN_INT_BASE + 49)          //  SDIO_IRQHandler
#define ARCH_INT_TIM5                (HAL_EXTERN_INT_BASE + 50)          //  TIM5_IRQHandler
#define ARCH_INT_SPI3                (HAL_EXTERN_INT_BASE + 51)          //  SPI3_IRQHandler
#define ARCH_INT_UART4               (HAL_EXTERN_INT_BASE + 52)          //  UART4_IRQHandler
#define ARCH_INT_UART5               (HAL_EXTERN_INT_BASE + 53)          //  UART5_IRQHandler
#define ARCH_INT_TIM6                (HAL_EXTERN_INT_BASE + 54)          //  TIM6_IRQHandler
#define ARCH_INT_TIM7                (HAL_EXTERN_INT_BASE + 55)          //  TIM7_IRQHandler
#define ARCH_INT_DMA2_Channel1       (HAL_EXTERN_INT_BASE + 56)          //  DMA2_Channel1_IRQHandler
#define ARCH_INT_DMA2_Channel2       (HAL_EXTERN_INT_BASE + 57)          //  DMA2_Channel2_IRQHandler
#define ARCH_INT_DMA2_Channel3       (HAL_EXTERN_INT_BASE + 58)          //  DMA2_Channel3_IRQHandler
#define ARCH_INT_DMA2_Channel4       (HAL_EXTERN_INT_BASE + 59)          //  DMA2_Channel4_IRQHandler


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_INTS_H__
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
