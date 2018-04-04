/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_ethernet.h
** Last modified Date:  2013-04-04
** Last Version:        v1.0
** Description:         ethernet模块寄存器封装函数实现声明
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
#ifndef __ARCH_IO_ETHERNET_H__
#define __ARCH_IO_ETHERNET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
** 驱动配置宏定义
*********************************************************************************************************/
#define PHYAddress                  31               // 使用的PHY的地址
#define TCPIP_CHECKSUM_EN           1                // 是否使能TCP/IP的硬件校验和功能
#define ENHANCED_DMA_DESCRIPTORS_EN 1                // 是否使能增强的DMA描述符

/*********************************************************************************************************
** DMA中断源定义
*********************************************************************************************************/
#define DMA_INTRUPT_NISE            0x00010000
#define DMA_INTRUPT_AISE            0x00008000
#define DMA_INTRUPT_ERS             0x00004000
#define DMA_INTRUPT_FBES            0x00002000
#define DMA_INTRUPT_ETIE            0x00000400
#define DMA_INTRUPT_RWITE           0x00000200
#define DMA_INTRUPT_RPSIE           0x00000100
#define DMA_INTRUPT_RBUIE           0x00000080
#define DMA_INTRUPT_RIE             0x00000040
#define DMA_INTRUPT_TUIE            0x00000020
#define DMA_INTRUPT_ROIE            0x00000010
#define DMA_INTRUPT_TITIE           0x00000008
#define DMA_INTRUPT_TBUIE           0x00000004
#define DMA_INTRUPT_TPSIE           0x00000002
#define DMA_INTRUPT_TIE             0x00000001

/*********************************************************************************************************
** 需要的数据结构定义
*********************************************************************************************************/
typedef struct  {
  uint32  Status;                   // < Status
  uint32  ControlBufferSize;        // < Control and Buffer1, Buffer2 lengths
  uint32  Buffer1Addr;              // < Buffer1 address pointer
  uint32  Buffer2NextDescAddr;      // < Buffer2 or next descriptor address pointer
  // Enhanced ETHERNET DMA PTP Descriptors
#if (ENHANCED_DMA_DESCRIPTORS_EN > 0)
  uint32  ExtendedStatus;           // Extended status for PTP receive descriptor
  uint32  Reserved1;                // Reserved
  uint32  TimeStampLow;             // Time Stamp Low value for transmit and receive
  uint32  TimeStampHigh;            // Time Stamp High value for transmit and receive
#endif                              // USE_ENHANCED_DMA_DESCRIPTORS
} tEthDMADesciptor;

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void EthernetInit(uint32 u32Base, uint32 u32Clock);
extern void EthernetPHYIntISR(uint32 u32Base);
extern uint32 EthernetPHYRead(uint32 u32Base, uint8 u8PHYDevAddr, uint8 u8PHYRegAddr);
extern void EthernetPHYWrite(uint32 u32Base, uint8 u8PHYDevAddr, uint8 u8PHYRegAddr, uint32 u32Data);
extern void EthernetEnable(uint32 u32Base);
extern void EthernetMACAddrSet(uint32 u32Base, uint8 *pu8MacAddr);
extern void EthernetMACAddrGet(uint32 u32Base, uint8 *pu8MacAddr);
extern void EthernetReset(uint32 u32Base);
extern boolean EthernetResetStatusGet(uint32 u32Base);
extern void EthernetDMAIntEnable(uint32 u32Base, uint32 u32Config);
extern void EthernetDMAIntDisable(uint32 u32Base, uint32 u32Config);


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_ETHERNET_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
