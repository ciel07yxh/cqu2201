/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_sdio.h
** Last modified Date:  2013-05-21
** Last Version:        v1.0
** Description:         SDIO模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-05-21
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
#ifndef __ARCH_IO_SDIO_H__
#define __ARCH_IO_SDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
**  相关参数的宏定义
*********************************************************************************************************/
/*********************************************************************************************************
**  函数SdioConfigSet的参数
*********************************************************************************************************/
// sdio 时钟的产生原则
#define SDIO_CLOCK_EDGE_RISING      ((uint32)0x00000000)
#define SDIO_CLOCK_EDGE_FALLING     ((uint32)0x00002000)

// SDIO时钟分频是否旁路
#define SDIO_CLOCK_BYPASS_DISABLE   ((uint32)0x00000000)
#define SDIO_CLOCK_BYPASS_ENABLE    ((uint32)0x00000400) 

// SDIO是否处于省电模式
#define SDIO_POWER_SAVE_DISABLE     ((uint32)0x00000000)
#define SDIO_POWER_SAVE_ENABLE      ((uint32)0x00000200) 

// SDIO总线宽度
#define SDIO_BUS_WIDE_1B            ((uint32)0x00000000)
#define SDIO_BUS_WIDE_4B            ((uint32)0x00000800)
#define SDIO_BUS_WIDE_8B            ((uint32)0x00001000)

// SDIO 流控
#define SDIO_HW_FLOW_CTRL_DISABLE    ((uint32)0x00000000)
#define SDIO_HW_FLOW_CTRL_ENABLE     ((uint32)0x00004000)

// SDIO分频系数
#define SDIO_CLOCK_DIV_NUM(x)        ((uint32)x)

/*********************************************************************************************************
**  函数SdioCMDSend的参数
*********************************************************************************************************/
// 命令号
#define SDIO_CMD_INDEX(x)           ((uint32)x)

// 回应类型
#define SDIO_RESPONSE_NO            ((uint32)0x00000000)
#define SDIO_RESPONSE_SHORT         ((uint32)0x00000040)
#define SDIO_RESPONSE_LONG          ((uint32)0x000000C0)

// SDIO等待条件
#define SDIO_WAIT_NO                ((uint32)0x00000000)
#define SDIO_WAIT_INT               ((uint32)0x00000100)
#define SDIO_WAIT_PEND              ((uint32)0x00000200)

// SDIO CPSM状态
#define SDIO_CPSM_DISABLE           ((uint32)0x00000000)
#define SDIO_CPSM_ENABLE            ((uint32)0x00000400)

/*********************************************************************************************************
**  函数SdioDataConfigInit的参数
*********************************************************************************************************/
// SDIO数据块大小
#define SDIO_DATABLOCK_SIZE_1B      ((uint32)0x00000000)
#define SDIO_DATABLOCK_SIZE_2B      ((uint32)0x00000010)
#define SDIO_DATABLOCK_SIZE_4B      ((uint32)0x00000020)
#define SDIO_DATABLOCK_SIZE_8B      ((uint32)0x00000030)
#define SDIO_DATABLOCK_SIZE_16B     ((uint32)0x00000040)
#define SDIO_DATABLOCK_SIZE_32B     ((uint32)0x00000050)
#define SDIO_DATABLOCK_SIZE_64B     ((uint32)0x00000060)
#define SDIO_DATABLOCK_SIZE_128B    ((uint32)0x00000070)
#define SDIO_DATABLOCK_SIZE_256B    ((uint32)0x00000080)
#define SDIO_DATABLOCK_SIZE_512B    ((uint32)0x00000090)
#define SDIO_DATABLOCK_SIZE_1024B   ((uint32)0x000000A0)
#define SDIO_DATABLOCK_SIZE_2048B   ((uint32)0x000000B0)
#define SDIO_DATABLOCK_SIZE_4096B   ((uint32)0x000000C0)
#define SDIO_DATABLOCK_SIZE_8192B   ((uint32)0x000000D0)
#define SDIO_DATABLOCK_SIZE_16384B  ((uint32)0x000000E0)

// SDIO数据传输方向
#define SDIO_TRANSFERDIR_TO_CARD    ((uint32)0x00000000)
#define SDIO_TRANSFERDIR_TO_SDIO    ((uint32)0x00000002)

// SDIO传输类型
#define SDIO_TRANSFER_MODE_BLOCK    ((uint32)0x00000000)
#define SDIO_TRANSFER_MODE_STREAM   ((uint32)0x00000004)

// SDIO_DPSM_State 
#define SDIO_DPSM_DISABLE           ((uint32)0x00000000)
#define SDIO_DPSM_ENABLE            ((uint32)0x00000001)

/*********************************************************************************************************
**  函数SdioReadWaitModeSet的参数
*********************************************************************************************************/
#define SDIO_READ_WAIT_MODE_CLK     ((uint32)0x00000400)
#define SDIO_READ_WAIT_MODE_DATA2  ~((uint32)0x00000400)

/*********************************************************************************************************
**  SDIO中断源
*********************************************************************************************************/
#define SDIO_INT_CCRCFAIL           ((uint32)0x00000001)
#define SDIO_INT_DCRCFAIL           ((uint32)0x00000002)
#define SDIO_INT_CTIMEOUT           ((uint32)0x00000004)
#define SDIO_INT_DTIMEOUT           ((uint32)0x00000008)
#define SDIO_INT_TXUNDERR           ((uint32)0x00000010)
#define SDIO_INT_RXOVERR            ((uint32)0x00000020)
#define SDIO_INT_CMDREND            ((uint32)0x00000040)
#define SDIO_INT_CMDSENT            ((uint32)0x00000080)
#define SDIO_INT_DATAEND            ((uint32)0x00000100)
#define SDIO_INT_STBITERR           ((uint32)0x00000200)
#define SDIO_INT_DBCKEND            ((uint32)0x00000400)
#define SDIO_INT_CMDACT             ((uint32)0x00000800)
#define SDIO_INT_TXACT              ((uint32)0x00001000)
#define SDIO_INT_RXACT              ((uint32)0x00002000)
#define SDIO_INT_TXFIFOHE           ((uint32)0x00004000)
#define SDIO_INT_RXFIFOHF           ((uint32)0x00008000)
#define SDIO_INT_TXFIFOF            ((uint32)0x00010000)
#define SDIO_INT_RXFIFOF            ((uint32)0x00020000)
#define SDIO_INT_TXFIFOE            ((uint32)0x00040000)
#define SDIO_INT_RXFIFOE            ((uint32)0x00080000)
#define SDIO_INT_TXDAVL             ((uint32)0x00100000)
#define SDIO_INT_RXDAVL             ((uint32)0x00200000)
#define SDIO_INT_SDIOIT             ((uint32)0x00400000)
#define SDIO_INT_CEATAEND           ((uint32)0x00800000)

/*********************************************************************************************************
**  SDIO Flag的参数值，与寄存器SDIO_STA对应
*********************************************************************************************************/
#define SDIO_FLAG_CCRCFAIL          ((uint32)0x00000001)
#define SDIO_FLAG_DCRCFAIL          ((uint32)0x00000002)
#define SDIO_FLAG_CTIMEOUT          ((uint32)0x00000004)
#define SDIO_FLAG_DTIMEOUT          ((uint32)0x00000008)
#define SDIO_FLAG_TXUNDERR          ((uint32)0x00000010)
#define SDIO_FLAG_RXOVERR           ((uint32)0x00000020)
#define SDIO_FLAG_CMDREND           ((uint32)0x00000040)
#define SDIO_FLAG_CMDSENT           ((uint32)0x00000080)
#define SDIO_FLAG_DATAEND           ((uint32)0x00000100)
#define SDIO_FLAG_STBITERR          ((uint32)0x00000200)
#define SDIO_FLAG_DBCKEND           ((uint32)0x00000400)
#define SDIO_FLAG_CMDACT            ((uint32)0x00000800)
#define SDIO_FLAG_TXACT             ((uint32)0x00001000)
#define SDIO_FLAG_RXACT             ((uint32)0x00002000)
#define SDIO_FLAG_TXFIFOHE          ((uint32)0x00004000)
#define SDIO_FLAG_RXFIFOHF          ((uint32)0x00008000)
#define SDIO_FLAG_TXFIFOF           ((uint32)0x00010000)
#define SDIO_FLAG_RXFIFOF           ((uint32)0x00020000)
#define SDIO_FLAG_TXFIFOE           ((uint32)0x00040000)
#define SDIO_FLAG_RXFIFOE           ((uint32)0x00080000)
#define SDIO_FLAG_TXDAVL            ((uint32)0x00100000)
#define SDIO_FLAG_RXDAVL            ((uint32)0x00200000)
#define SDIO_FLAG_SDIOIT            ((uint32)0x00400000)
#define SDIO_FLAG_CEATAEND          ((uint32)0x00800000)

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern void SdioInit(uint32 u32Base, uint32 u32Config);
extern void SdioEnable(uint32 u32Base);
extern void SdioDisable(uint32 u32Base);
extern void SdioPowerOn(uint32 u32Base);
extern void SdioPowerOff(uint32 u32Base);
extern boolean SdioPowerStatus(uint32 u32Base);
extern void SdioCMDSend(uint32 u32Base, uint32 u32SdioArg, uint32 u32SendConfig);
extern uint8 SdioCommandResponseGet(uint32 u32Base);
extern uint32 SdioResponseGet(uint32 u32Base, uint32 u32RESP);
extern void SdioDataConfigInit(uint32 u32Base, uint32 u32DataTimeOut, uint32 u32DataLen, uint32 u32DataCtrl);
extern uint32 SdioDataCounterGet(uint32 u32Base);
extern uint32 SdioDataRead(uint32 u32Base);
extern void SdioDataWrite(uint32 u32Base, uint32 u32Data);
extern uint32 SdioFIFOCounterGet(uint32 u32Base);
extern void SdioReadWaitSdioStart(uint32 u32Base, boolean bIsSet);
extern void SdioReadWaitSdioStop(uint32 u32Base, boolean bIsSet);
extern void SdioReadWaitSdioModeSet(uint32 u32Base, uint32 u32Mode);
extern void SdioOperationSdioSet(uint32 u32Base, boolean bIsSet);
extern void SdioSendSDIOSuspendCmd(uint32 u32Base, boolean bIsSet);
extern void SdioCommandCompletionSet(uint32 u32Base, boolean bIsSet);
extern void SdioCEATAIntSet(uint32 u32Base, boolean bIsSet);
extern void SdioSendCEATASet(uint32 u32Base, boolean bIsSet);
extern void SdioDmaEnable(uint32 u32Base);
extern void SdioDmaDisable(uint32 u32Base);
extern void SdioIntEnable(uint32 u32Base, uint32 u32IntFlag);
extern void SdioIntDisable(uint32 u32Base, uint32 u32IntFlag);
extern uint32 SdioFlagStatus(uint32 u32Base);
extern void SdioFlagClear(uint32 u32Base, uint32 u32Flag);
extern uint32 SdioIntStatus(uint32 u32Base);
extern void SdioIntClear(uint32 u32Base, uint32 u32IntFlag);



#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_SDIO_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
