/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_gpio.h
** Last modified Date:  2013-04-02
** Last Version:        v1.0
** Description:         gpio模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-02
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
#ifndef __ARCH_IO_GPIO_H__
#define __ARCH_IO_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 需要的宏定义
*********************************************************************************************************/
//  引脚定义
#define GPIO_PIN_0      0x0001
#define GPIO_PIN_1      0x0002
#define GPIO_PIN_2      0x0004
#define GPIO_PIN_3      0x0008
#define GPIO_PIN_4      0x0010
#define GPIO_PIN_5      0x0020
#define GPIO_PIN_6      0x0040
#define GPIO_PIN_7      0x0080
#define GPIO_PIN_8      0x0100
#define GPIO_PIN_9      0x0200
#define GPIO_PIN_10     0x0400
#define GPIO_PIN_11     0x0800
#define GPIO_PIN_12     0x1000
#define GPIO_PIN_13     0x2000
#define GPIO_PIN_14     0x4000
#define GPIO_PIN_15     0x8000


/*********************************************************************************************************
** IO配置参数定义
*********************************************************************************************************/
// IO设置为输入时取下列值中的一个
#define GPIO_DIR_IN_Analog      0x0000
#define GPIO_DIR_IN_Floating    0x0004
#define GPIO_DIR_IN_UpDown      0x0008

// IO设置为输出时设置为下列两组值的或
#define GPIO_OUT_PP             0x0000
#define GPIO_OUT_OD             0x0004
#define GPIO_AF_PP              0x0008
#define GPIO_AF_OD              0x000C

#define GPIO_SPEED_2MHZ         0x0001
#define GPIO_SPEED_10MHZ        0x0002
#define GPIO_SPEED_50MHZ        0x0003

/*********************************************************************************************************
** 外设引脚重映射定义
*********************************************************************************************************/
#define AFIO_REMAP_SPI1_REMAP               ((uint32)1 << 0)
#define AFIO_REMAP_I2C1_REAMP               ((uint32)1 << 1)
#define AFIO_REMAP_USART1_REAMP             ((uint32)1 << 2)
#define AFIO_REMAP_USART2_REMAP             ((uint32)1 << 3)
#define AFIO_REMAP_USART3_REAMP_HALF        ((uint32)1 << 4)
#define AFIO_REMAP_USART3_REAMP_FULL        ((uint32)3 << 4)
#define AFIO_REMAP_TIM1_REAMP_HALF          ((uint32)1 << 6)
#define AFIO_REMAP_TIM1_REMAP_FULL          ((uint32)3 << 6)
#define AFIO_REMAP_TIM2_REMAP_HALF          ((uint32)1 << 8)
#define AFIO_REMAP_TIM2_REAMP_FULL          ((uint32)3 << 8)
#define AFIO_REMAP_TIM3_REAMP_HALF          ((uint32)1 << 10)
#define AFIO_REMAP_TIM3_REAMP_FULL          ((uint32)1 << 10)
#define AFIO_REMAP_TIM4_REMAP               ((uint32)1 << 12)
#define AFIO_REMAP_CAN_REMAP_HALF           ((uint32)1 << 13)
#define AFIO_REMAP_CAN_REMAP_FULL           ((uint32)3 << 13)
#define AFIO_REMAP_PD01_REMAP               ((uint32)1 << 15)
#define AFIO_REMAP_TIM5_CH4_REMAP           ((uint32)1 << 16)
#define AFIO_REMAP_ADC1_ETRGINJ_REMAP       ((uint32)1 << 17)
#define AFIO_REMAP_ADC1_ETRGREG_REMAP       ((uint32)1 << 18)
#define AFIO_REMAP_ADC2_ETRGINJ_REMAP       ((uint32)1 << 19)
#define AFIO_REMAP_ADC2_ETRGRGE_REMAP       ((uint32)1 << 20)
#define AFIO_REMAP_ETH_REMAP                ((uint32)1 << 21)
#define AFIO_REMAP_CAN2_REMAP               ((uint32)1 << 22)
#define AFIO_REMAP_RMII_EN                  ((uint32)1 << 23)
#define AFIO_REMAP_SPI3_REMAP               ((uint32)1 << 28)
#define AFIO_REMAP_TIM2ITR1_REMAP           ((uint32)1 << 29)
#define AFIO_REMAP_PTP_PPS_REMAP            ((uint32)1 << 30)

/*********************************************************************************************************
** EXTI中断Line接口定义
*********************************************************************************************************/
#define EXTI_GPIO_A                 0x00000000
#define EXTI_GPIO_B                 0x00000001
#define EXTI_GPIO_C                 0x00000002
#define EXTI_GPIO_D                 0x00000003
#define EXTI_GPIO_E                 0x00000004
#define EXTI_GPIO_F                 0x00000005
#define EXTI_GPIO_G                 0x00000006

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void GPIOConfigSet(unsigned long ulBase, unsigned short usPin, unsigned long ulConfig);
extern void GPIODataPut(unsigned long ulBase, unsigned short usData);
extern unsigned short GPIODataGet(unsigned long ulBase);
extern void GPIOBitSet(unsigned long ulBase,unsigned short usPin);
extern void GPIOBitReset(unsigned long ulBase,unsigned short usPin);
extern void GPIOToggleBit(uint32 u32Base, uint32 u32Pins);
extern void AFIORemapEnable(uint32 u32Remap);
extern void AFIORemapReset(uint32 u32Remap);
extern void AFIOExtiLineConfig(uint32 u32GPIOLinex, uint32 u32Pin);


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_GPIO_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
