/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_usart.h
** Last modified Date:  2013-04-02
** Last Version:        v1.0
** Description:         usart及uart模块寄存器封装函数实现声明
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
#ifndef __ARCH_IO_USART_H__
#define __ARCH_IO_USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 需要的宏定义
*********************************************************************************************************/
//  串口配置函数UsartConfigSet的参数定义,该宏左移12位后可以判断写入寄存器
#define USART_STOP_0_5      0x0001
#define USART_STOP_1        0x0000
#define UASRT_STOP_1_5      0x0003
#define USART_STOP_2        0x0002

#define USART_WLEN_8        0x0000
#define USART_WLEN_9        0x1000

#define USART_PAR_NONE      0x0000
#define USART_PAR_EVEN      0x0400
#define USART_PAR_ODD       0x0600

#define USART_DMA_TX        0x0080    //  DMA发送功能
#define USART_DMA_RX        0x0040    //  DMA接受功能
//  常用中断源，为函数UsartIntEnable()的参数
#define USART_INT_TX_EMPTY  0x0080      //  发送缓冲区空中断
#define USART_INT_TX_FINISH 0x0040      //  发送完成中断
#define USART_INT_RX        0x0020      //  接收中断使能，说明接收寄存器中有数据了
#define USART_INT_IDLE      0x0010      //  IDLE中断使能
/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void UsartConfigSet(unsigned long ulBase, unsigned long ulClock, 
                            unsigned long ulBaud, unsigned long ulConfig);
extern void UsartEnable(unsigned long ulBase);
extern void UsartDisable(unsigned long ulBase);
extern void UsartDMAEnable(unsigned long ulBase,unsigned long ulFlags);
extern void UsartDMADisable(unsigned long ulBase, unsigned long ulFlags);
extern void UsartIntEnable(unsigned long ulBase,unsigned long ulIntFlags);
extern void UsartIntDisable(unsigned long ulBase,unsigned long ulIntFlags);
extern boolean UsartSpaceAvial(unsigned long ulBase);
extern void UsartCharPut(unsigned long ulBase,unsigned char ucData);
extern boolean UsartCharsAvial(unsigned long ulBase);
extern unsigned long  UsartCharGet(unsigned long ulBase);
extern void UsartPutS(unsigned long ulBase,const char *s);
extern void UsartPutN(unsigned long ulBase, const  char  *pStr , unsigned short  usNum);



#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_USART_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
