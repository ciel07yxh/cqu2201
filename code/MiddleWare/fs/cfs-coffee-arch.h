/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           cfs-coffee-arch.h
** Last modified Date:  2012-08-03
** Last Version:        coffee文件系统模块移植代码实现
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2012-08-03
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

#ifndef __CFS_COFFEE_ARCH_H__
#define __CFS_COFFEE_ARCH_H__

/*********************************************************************************************************
** 头文件包含
*********************************************************************************************************/
#include "contiki.h"

#include "infra/rhb_type.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_ints.h"
#include "hal/arch/inc/arch_io_flash.h"
#include "hal/CortexM3/hal_io_level.h"

/*********************************************************************************************************
** STM32F103RBT6有128个page，每个page1024字节，它是最小的擦除单元。最小的写单元是一个16位的值(半字)
*********************************************************************************************************/
// 指定Flash起始地址
#define FLASH_START               0x8000000
//　指定页大小，即最小擦除的单元
#define FLASH_PAGE_SIZE           1024
// 保留最后三个扇区为其它存储
#define FLASH_PAGES               253
// Coffee文件系统的最小保留单元，可以由用户重新定义
#define COFFEE_PAGE_SIZE          (FLASH_PAGE_SIZE/4)


/* If using IAR, COFFEE_ADDRESS reflects the static value in the linker script
   iar-cfg-coffee.icf, so it can't be passed as a parameter for Make.*/
#ifdef __ICCARM__
#define COFFEE_ADDRESS            0x08020000
#endif
#if (COFFEE_ADDRESS & 0x3FF) !=0
 #error "COFFEE_ADDRESS not aligned to a 1024-bytes page boundary."
#endif

#define COFFEE_PAGES              ((FLASH_PAGES*FLASH_PAGE_SIZE-(COFFEE_ADDRESS-FLASH_START))/COFFEE_PAGE_SIZE)
#define COFFEE_START              (COFFEE_ADDRESS & ~(COFFEE_PAGE_SIZE-1))
#define COFFEE_SIZE               (COFFEE_PAGES*COFFEE_PAGE_SIZE)

/* These must agree with the parameters passed to makefsdata */
#define COFFEE_SECTOR_SIZE        FLASH_PAGE_SIZE
#define COFFEE_NAME_LENGTH        20

/* These are used internally by the coffee file system */
/* Micro logs are not needed for single page sectors */
#define COFFEE_MAX_OPEN_FILES     4
#define COFFEE_FD_SET_SIZE        8
#define COFFEE_DYN_SIZE           (COFFEE_PAGE_SIZE*1)
#define COFFEE_MICRO_LOGS         0
#define COFFEE_LOG_TABLE_LIMIT    16    // It doesnt' matter as
#define COFFEE_LOG_SIZE           128   // COFFEE_MICRO_LOGS is 0.


#if COFFEE_PAGES <= 127
#define coffee_page_t int8_t
#elif COFFEE_PAGES <= 0x7FFF
#define coffee_page_t int16_t
#endif


#define COFFEE_WRITE(buf, size, offset) \
        stm32f10xx_flash_write(COFFEE_START + offset, buf, size)

#define COFFEE_READ(buf, size, offset) \
        stm32f10xx_flash_read(COFFEE_START + offset, buf, size)

#define COFFEE_ERASE(sector) \
        stm32f10xx_flash_erase(sector)


void stm32f10xx_flash_read(uint32_t address, void * data, uint32_t length);
void stm32f10xx_flash_write(uint32_t address, const void * data, uint32_t length);
void stm32f10xx_flash_erase(uint32_t sector);

#endif /* __CFS_COFFEE_ARCH_H__ */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
