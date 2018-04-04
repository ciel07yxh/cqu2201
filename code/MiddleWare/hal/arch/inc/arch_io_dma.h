/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_dma.h
** Last modified Date:  2013-04-23
** Last Version:        v1.0
** Description:         DMA模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-23
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
#ifndef __ARCH_IO_DMA_H__
#define __ARCH_IO_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 需要的宏定义
*********************************************************************************************************/
//  DMA配置相关参数，为函数DMAModeSet的参数ulConfig
#define DMA_DIR_M_READ      0x00000010      //  从存储器读，反之为外设读

#define DMA_CIRC_EN         0x00000020      //  执行循环操作模式
#define DMA_PINC_EN         0x00000040      //  使能外设地址增量模式
#define DMA_MINC_EN         0x00000080      //  使能存储器地址增量模式

#define DMA_PSIZE_8         0x00000000      //  外设数据宽度为8位
#define DMA_PSIZE_16        0x00000100      //  外设数据宽度为16位
#define DMA_PSIZE_32        0x00000200      //  外设数据宽度为32位
#define DMA_PSIZE_RESERVED  0x00000300      //  外设数据宽度保留

#define DMA_MSIZE_8         0x00000000      //  存储器数据宽度为8位
#define DMA_MSIZE_16        0x00000400      //  存储器数据宽度为16位
#define DMA_MSIZE_32        0x00000800      //  存储器数据宽度为32位
#define DMA_MSIZE_RESERVED  0x00000C00      //  存储器数据宽度保留

#define DMA_PL_LOW          0x00000000      //  通道优先级为低
#define DMA_PL_MEDIUM       0x00001000      //  通道优先级为中
#define DMA_PL_HIGH         0x00002000      //  通道优先级为高
#define DMA_PL_HITHEST      0x00003000      //  通道优先级为最高

#define DMA_MTOM_EN         0x00004000      //  存储器到存储器传输模式使能
//  相关的中断信号定义，为中断相关函数的参数或返回值
#define DMA_INT_TC          0x00000002      //  传输完成中断
#define DMA_INT_HT          0x00000004      //  传输一半时的中断
#define DMA_INT_TE          0x00000008      //  传输错误中断
#define DMA_INT_GLOBAL      0x00000001      //  全局中断，只要上面三个有一个成立，则就成立

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void DMAModeSet(unsigned long ulBase, unsigned char ucChannel,unsigned long ulConfig);
extern void DMAAddrSet(unsigned long ulBase, unsigned char ucChannel, 
                unsigned long ulMemAddr, unsigned long ulPeriAddr);
extern void DMABufferSizeSet(unsigned long ulBase, unsigned char ucChannel, unsigned short usSize);
extern void DMAEnable(unsigned long ulBase, unsigned char ucChannel);
extern void DMADisable(unsigned long ulBase, unsigned char ucChannel);
extern void DMAIntEnable(unsigned long ulBase, unsigned char ucChannel,unsigned long ulFlags);
extern void DMAIntDisable(unsigned long ulBase, unsigned char ucChannel,unsigned long ulFlags);
extern unsigned long DMAIntStatus(unsigned long ulBase, unsigned char ucChannel);
extern void DMAIntClear(unsigned long ulBase, unsigned char ucChannel,unsigned long ulIntFlags);

#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_DMA_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
