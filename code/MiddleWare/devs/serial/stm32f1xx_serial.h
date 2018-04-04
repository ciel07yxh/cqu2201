/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           stm32f1xx_serial.h
** Last modified Date:  2013-08-28
** Last Version:        v1.0
** Description:         基于contiki的stm32的串口驱动程序。发送和接受都基于中断方式
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-08-28
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
#ifndef __STM32F1XX_SERIAL_H__
#define __STM32F1XX_SERIAL_H__

/*********************************************************************************************************
** 头文件包含
*********************************************************************************************************/
#include "infra/rhb_type.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_usart.h"
#include "hal/arch/inc/arch_io_gpio.h"
#include "hal/CortexM3/hal_io_level.h"
#include "hal/arch/inc/arch_io_ints.h"

#include "contiki.h"
#include "lib/ringbuf.h"


/*********************************************************************************************************
** 基本宏定义
*********************************************************************************************************/
#define BAUD2UBR(baud)          baud
// 定义发送缓冲区大小, 最大255
#define UART_SEND_BUFFER_SIZE   128

/*********************************************************************************************************
** 串口特性定义
*********************************************************************************************************/
typedef struct {
    uint32   base;                           //  模块基地址
    uint8   u8WhichBus; uint32 u32Ctrl;      //  外设总线属性，用于确定外设时钟
    uint8   int_num;                         //  中断序号
    uint8   transmitting;                    //  串口是否处于发送状态，0，没有正在发送；1，正在发送
    uint8   sndBuf[UART_SEND_BUFFER_SIZE];   //  发送缓冲区，会用在contiki的环形缓冲区中
    uint32   Baud;                           //  波特率
    struct   ringbuf txbuf;                         //  发送环形缓冲区定义
    int (*serial_input_handler)(unsigned char c);   //  数据输入函数指针
} stm32_serial_info;

/*********************************************************************************************************
** 外部函数声明
*********************************************************************************************************/
extern stm32_serial_info tUsart1Info;
extern stm32_serial_info tUsart2Info;
extern stm32_serial_info tUsart3Info;
extern void Usart1_ISR(void);
extern void Usart2_ISR(void);
extern void Usart3_ISR(void);
extern void serial_set_input(void *handle, int (*input)(unsigned char c));
extern void serial_writeb(void *handle, unsigned char c);
extern void serial_init(void *handle, uint32 u32Buad, void(*pfIsrHandler)(void));

// 串口1初始化
                                /*AFIORemapEnable(AFIO_REMAP_USART1_REAMP);                               \*/
#define hal_uart1_init()        do{                 \
                                SystemPeripheralEnable(RCC_PERIPHERAL_IOPA);                            \
                                SystemPeripheralEnable(RCC_PERIPHERAL_AFIO);                            \
                                SystemPeripheralEnable(RCC_PERIPHERAL_USART1);                          \
                                GPIOConfigSet(GPIOA_BASE, GPIO_PIN_9, GPIO_AF_PP | GPIO_SPEED_50MHZ);   \
                                GPIOConfigSet(GPIOA_BASE, GPIO_PIN_10, GPIO_DIR_IN_Floating);            \
                                }while(0);
#define uart1_set_input(a)      serial_set_input(&tUsart1Info , a)
#define uart1_writeb(a)         serial_writeb(&tUsart1Info, a)
#define uart1_init(a)           serial_init(&tUsart1Info, a, Usart1_ISR)

// 串口2初始化
#define hal_uart2_init()        do{                 \
                                SystemPeripheralEnable(RCC_PERIPHERAL_IOPA);                            \
                                SystemPeripheralEnable(RCC_PERIPHERAL_USART2);                          \
                                GPIOConfigSet(GPIOA_BASE, GPIO_PIN_2, GPIO_AF_PP | GPIO_SPEED_50MHZ);   \
                                GPIOConfigSet(GPIOA_BASE, GPIO_PIN_3, GPIO_DIR_IN_Floating);            \
                                }while(0);
#define uart2_set_input(a)      serial_set_input(&tUsart2Info , a)
#define uart2_writeb(a)         serial_writeb(&tUsart2Info, a)
#define uart2_init(a)           serial_init(&tUsart2Info, a, Usart2_ISR)

// 串口3初始化
#define hal_uart3_init()        do{                 \
                                SystemPeripheralEnable(RCC_PERIPHERAL_IOPB);                            \
                                SystemPeripheralEnable(RCC_PERIPHERAL_USART3);                          \
                                GPIOConfigSet(GPIOB_BASE, GPIO_PIN_10, GPIO_AF_PP | GPIO_SPEED_50MHZ);   \
                                GPIOConfigSet(GPIOB_BASE, GPIO_PIN_11, GPIO_DIR_IN_Floating);            \
                                }while(0);
#define uart3_set_input(a)      serial_set_input(&tUsart3Info , a)
#define uart3_writeb(a)         serial_writeb(&tUsart3Info, a)
#define uart3_init(a)           serial_init(&tUsart3Info, a, Usart3_ISR)

#endif /* __STM32F1XX_SERIAL_H__ */
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
