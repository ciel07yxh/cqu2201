/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_spi.h
** Last modified Date:  2013-04-20
** Last Version:        v1.0
** Description:         spi及i2s模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-20
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
#ifndef __ARCH_IO_SPI_H__
#define __ARCH_IO_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 需要的宏定义
*********************************************************************************************************/
// 函数SPI_Init参数ulConfig宏定义,可以取下列几组参数的或
// 工作模式方向选择
#define SPI_DIR_2Lines_FullDuplex       0x0000
#define SPI_DIR_2Lines_RxOnly           0x0400
#define SPI_DIR_1Lines_Rx               0x8000
#define SPI_DIR_1Lines_Tx               0xC000

//  工作主从选择
#define SPI_MODE_Master                 0x0104
#define SPI_MODE_Slave                  0x0000

//  数据长度选择
#define SPI_DATA_Len8                   0x0000
#define SPI_DATA_Len16                  0x0800

// 无数据收发时，SPI_CLK极性选择
#define SPI_CPOL_Low                    0x0000
#define SPI_CPOL_High                   0x0002

// 时钟采样的相位选择
#define SPI_CPHA_1Edge                  0x0000
#define SPI_CPHA_2Edge                  0x0001

// 从机片选管理
#define SPI_NSS_Soft                    0x0200
#define SPI_NSS_Hard                    0x0000

// SPI波特率分频器选择
#define SPI_Baud_Prescaler_2            0x0000
#define SPI_Baud_Prescaler_4            0x0008
#define SPI_Baud_Prescaler_8            0x0010
#define SPI_Baud_Prescaler_16           0x0018
#define SPI_Baud_Prescaler_32           0x0020
#define SPI_Baud_Prescaler_64           0x0028
#define SPI_Baud_Prescaler_128          0x0030
#define SPI_Baud_Prescaler_256          0x0038

// 发送数据位从MSB开始，还是从LSB开始选择
#define SPI_First_MSB                   0x0000
#define SPI_First_LSB                   0x0080

// 函数SPI_I2S_IntEnable或SPI_I2S_IntDisable的参数定义，指定使能的中断源
#define SPI_I2S_INT_TXE                 (1<<7)
#define SPI_I2S_INT_RXNE                (1<<6)
#define SPI_I2S_INT_ERR                 (1<<5)

// 函数SPI_I2S_DMAEnable或SPI_I2S_DMADisable的参数定义
#define SPI_I2S_DMA_TX                  (1<<1)
#define SPI_I2S_DMA_RX                  (1<<0)

/*********************************************************************************************************
** 宏函数的一些定义
*********************************************************************************************************/
#define SPI_Busy(ulBase)                (HWREG(ulBase+SPI_O_SR) & 0x80)  // 为0，表示SPI总线不忙，非0，表示忙
#define SPISpaceAvial(ulBase)           (!(HWREG(ulBase+SPI_O_SR) & 0x02))// 0，发送缓冲区空；非0，发送缓冲区不空
#define SPICharsAvial(ulBase)           (HWREG(ulBase+SPI_O_SR) & 0x01)  // 0,接收缓冲区空，非0，接收缓冲区非空

// 下面两个宏通过软件设置片选引脚输出，只有片选引脚设置为软件控制时才有效
#define SPI_SSSet(ulBase)               (HWREG(ulBase+SPI_O_CR1) |= 0x0100)
#define SPI_SSReset(ulBase)             (HWREG(ulBase+SPI_O_CR1) &= 0xFEFF)
/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void SPIInit(unsigned long ulBase,unsigned long ulConfig);
extern void SPI_SSOutEnable(unsigned long ulBase);
extern void SPI_SSOutDisable(unsigned long ulBase);
extern void SPIEnable(unsigned long ulBase);
extern void SPIDisable(unsigned long ulBase);
extern void SPI_I2S_IntEnable(unsigned long ulBase,unsigned long ulIntFlag);
extern void SPI_I2S_IntEDisable(unsigned long ulBase,unsigned long ulIntFlag);
extern void SPI_I2S_DMAEnable(unsigned long ulBase,unsigned long ulDMAFlag);
extern void SPI_I2S_DMADisable(unsigned long ulBase,unsigned long ulDMAFlag);
extern void SPI_I2S_DataPut(unsigned long ulBase,unsigned short usData);
extern unsigned short SPI_I2S_DataGet(unsigned long ulBase);



#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_SPI_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
