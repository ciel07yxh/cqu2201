/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_flash.h
** Last modified Date:  2013-04-04
** Last Version:        v1.0
** Description:         flash模块寄存器封装函数实现声明
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
#ifndef __ARCH_IO_FLASH_H__
#define __ARCH_IO_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"
  
/*********************************************************************************************************
**  定义设备是否是STM32F10xx XL-density系列.1:是XL系列；0：不是XL系列
*********************************************************************************************************/
#define STM32F10XX_XL_Density       1

/*********************************************************************************************************
**  定义BANK1的最大地址，用于判断BANK2的处理
*********************************************************************************************************/
#define FLASH_BANK1_END_ADDRESS   ((uint32)0x807FFFF)

/*********************************************************************************************************
** 外部函数使用的声明
*********************************************************************************************************/
extern void flashUnlock(void);
extern void flashLock(void);
extern boolean flashPageErase(unsigned long ulPageAddr);
extern boolean flashMassErase(void);
extern boolean flashWriteHalfWord(unsigned long ulAddr, unsigned short usData);
extern boolean flashProgram(unsigned short *pusData, unsigned long ulAddr, unsigned long ulSize);
extern boolean flashConstDataProgram(unsigned short usData, unsigned long ulAddr, unsigned long ulSize);


#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_FLASH_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
