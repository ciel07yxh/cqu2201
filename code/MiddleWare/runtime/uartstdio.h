/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               uartstdio.h
** Last modified Date:      2012-06-30
** Last Version:            v1.0
** Description:             串口控制台函数实现
**
**--------------------------------------------------------------------------------------------------------
** Created By:              任海波
** Created date:            2012-06-30
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
#ifndef __UARTSTDIO_H__
#define __UARTSTDIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
  外部函数及变量定义
*********************************************************************************************************/
extern void uart_stdio_init(uint32 u32Baud);
extern int uart_write_string(const char *pcBuf, uint32 u32Len);
extern int uart_read_string(char *pcBuf, uint32 u32Len);
extern unsigned char uart_getc(void);
extern void uart_putc(unsigned char ch);
extern void uart_printf(const char *pcString, ...);


#ifdef __cplusplus
}
#endif

#endif // __UARTSTDIO_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/