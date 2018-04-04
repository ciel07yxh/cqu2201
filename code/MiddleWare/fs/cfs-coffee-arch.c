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

/*********************************************************************************************************
** 头文件包含
*********************************************************************************************************/
#include <string.h>
#include "cfs-coffee-arch.h"

/*********************************************************************************************************
** Allocates a buffer of FLASH_PAGE_SIZE bytes statically (rather than on the stack).
*********************************************************************************************************/
#ifndef STATIC_FLASH_BUFFER
#define STATIC_FLASH_BUFFER     1
#endif

/*********************************************************************************************************
** Function name:       stm32f10xx_flash_read
** Descriptions:        读取指定地址flash中的内容
** Input parameters:    u32Addr:  指定读写的flash地址
**                      data:     读取数据存放地址
**                      len:      读取数据长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void stm32f10xx_flash_read(uint32_t u32Addr, void *data, uint32_t len)
{
    uint8_t *pSrcData = (uint8_t *)u32Addr;
    ENERGEST_ON(ENERGEST_TYPE_FLASH_READ);
    memcpy(data, pSrcData, len);
    ENERGEST_OFF(ENERGEST_TYPE_FLASH_READ);
}

/*********************************************************************************************************
** Function name:       stm32f10xx_flash_erase
** Descriptions:        擦除flash指定扇区的内容
** Input parameters:    sector:  待擦除的扇区号
** Output parameters:   无
** Returned value:      无
** 注意：这里的擦除是将Flash清零，对于Flash一般要先擦除，然后在写入0
*********************************************************************************************************/
void stm32f10xx_flash_erase(uint32_t sector)
{
    uint32_t addr = COFFEE_START + (sector) * COFFEE_SECTOR_SIZE;
  
    flashUnlock();
    flashPageErase(COFFEE_START + (sector) * COFFEE_SECTOR_SIZE);
    flashConstDataProgram(0, addr, COFFEE_SECTOR_SIZE);
    flashLock();
}


/*********************************************************************************************************
** Function name:       stm32f10xx_flash_write
** Descriptions:        向Flash的指定地址写入数据
** Input parameters:    u32Addr:  指定写的flash地址
**                      data:     写数据存放地址
**                      len:      写取数据长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void stm32f10xx_flash_write(uint32_t u32Addr, const void *data, uint32_t len)
{
    const uint32_t end = u32Addr + len;
    uint32_t i;
    uint32_t next_page, curr_page;
    uint16_t offset;
  
#if STATIC_FLASH_BUFFER
    static uint8_t buf[FLASH_PAGE_SIZE];
#else
    uint8_t buf[FLASH_PAGE_SIZE];
#endif
  
    for(i = u32Addr; i<end;)
    {
        next_page = (i | (FLASH_PAGE_SIZE-1)) + 1;
        curr_page = i & ~(FLASH_PAGE_SIZE-1);
        offset = i - curr_page;
        if(next_page > end) {
          next_page = end;
        }
        // 读取一页的数据到指定的缓冲区
        stm32f10xx_flash_read(curr_page, buf, FLASH_PAGE_SIZE);
        // 更新数据
        memcpy(buf + offset, data, next_page - i);
        // 擦除扇区，并写入新数据
        ENERGEST_ON(ENERGEST_TYPE_FLASH_WRITE);
        flashUnlock();
        flashPageErase(i);
        flashProgram((uint16_t *)buf, curr_page, FLASH_PAGE_SIZE);
        flashLock();
        ENERGEST_OFF(ENERGEST_TYPE_FLASH_WRITE);
        
        data = (u8_t *)data + next_page - i;
        i = next_page;
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
