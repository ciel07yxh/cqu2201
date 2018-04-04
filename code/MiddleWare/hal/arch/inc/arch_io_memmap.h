/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_memmap.h
** Last modified Date:  2013-04-01
** Last Version:        v1.0
** Description:         IO模块内存映射定义
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

#ifndef __ARCH_IO_MEMMAP_H__
#define __ARCH_IO_MEMMAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** 外设映射基地址定义
*********************************************************************************************************/
#define RCC_BASE         0x40021000              //  葩弇迵奀笘諷秶耀輸(RCC)價華硊
#define IWDG_BASE        0x40003000              //  黃蕾艘藷僩耀輸價硊
#define AFIO_BASE        0x40010000              //  AFIO耀輸價華硊
#define EXTI_BASE        0x40010400              //  EXTI耀輸價華硊
#define GPIOA_BASE       0x40010800              //  BS_GPIOA耀輸價華硊
#define GPIOB_BASE       0x40010C00              //  BS_GPIOB耀輸價華硊
#define GPIOC_BASE       0x40011000              //  BS_GPIOC耀輸價華硊
#define GPIOD_BASE       0x40011400              //  BS_GPIOD耀輸價華硊
#define GPIOE_BASE       0x40011800              //  BS_GPIOE耀輸價華硊
#define GPIOF_BASE       0x40011C00              //  BS_GPIOF耀輸價華硊
#define GPIOG_BASE       0x40012000              //  BS_GPIOG耀輸價華硊
#define ETH0_BASE        0x40028000              //  眕怮厙MAC耀輸腔價華硊
#define USART1_BASE      0x40013800              //  Usart1耀輸價華硊
#define USART2_BASE      0x40004400              //  Usart2耀輸價華硊
#define USART3_BASE      0x40004800              //  Usart3耀輸價華硊
#define USART4_BASE      0x40004C00              //  Usart4耀輸價華硊
#define USART5_BASE      0x40005000              //  Usart5耀輸價華硊
#define FLASH_BASE       0x40022000              //  Flash諷秶耀輸價華硊
#define DMA1_BASE        0x40020000              //  DMA1耀輸價華硊
#define DMA2_BASE        0x40020400              //  DMA2耀輸價華硊
#define SPI1_BASE        0x40013000              //  SPI1耀輸價華硊
#define SPI2_BASE        0x40003800              //  SPI2耀輸價華硊
#define SPI3_BASE        0x40003C00             //  SPI3耀輸價華硊

#define TIM2_BASE        0x40000000             // TIM2耀輸價華硊
#define TIM3_BASE        0x40000400             // TIM3耀輸價華硊
#define TIM4_BASE        0x40000800             // TIM4耀輸價華硊
#define TIM5_BASE        0x40000C00             // TIM5耀輸價華硊
#define TIM6_BASE        0x40001000             // TIM6耀輸價華硊
#define TIM7_BASE        0x40001400             // TIM7耀輸價華硊
#define TIM12_BASE       0x40001800             // TIM12耀輸價華硊
#define TIM13_BASE       0x40001C00             // TIM13耀輸價華硊
#define TIM14_BASE       0x40002000             // TIM14耀輸價華硊
#define TIM1_BASE        0x40012C00             // TIM1耀輸價華硊
#define TIM8_BASE        0x40013400             // TIM8耀輸價華硊
#define TIM15_BASE       0x40014000             // TIM12耀輸價華硊
#define TIM16_BASE       0x40014400             // TIM13耀輸價華硊
#define TIM17_BASE       0x40014800             // TIM14耀輸價華硊
#define TIM9_BASE        0x40014C00             // TIM12耀輸價華硊
#define TIM10_BASE       0x40015000             // TIM13耀輸價華硊
#define TIM11_BASE       0x40015400             // TIM14耀輸價華硊


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_MEMMAP_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
