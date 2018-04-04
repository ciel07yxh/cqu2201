/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           rhb_spi_bus_adapter.h
** Last modified Date:  2013-07-08
** Last Version:        V1.0
** Description:         SPI总线适配层
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2013-07-08
** Version:             V1.0
** Descriptions:        The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#ifndef  __RHB_SPI_BUS_ADAPTER_H__
#define  __RHB_SPI_BUS_ADAPTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

/*********************************************************************************************************
  总线驱动描述符
*********************************************************************************************************/
typedef struct rhb_spi_bus {
    // 中断保护状态
    uint8 int_state;
    // 总线硬件初始化
    void (*spi_bus_init)(struct rhb_spi_bus *bus); 
    // 锁定总线，避免多线程操作的冲突
    void (*spi_bus_lock)(struct rhb_spi_bus *bus);
    // 接触锁定总线，其他SPI设备可以使用总线了
    void (*spi_bus_unlock)(struct rhb_spi_bus *bus);  
    // 总线发送一个字节的数据
    uint8 (*spi_bus_send_recv_byte)(struct rhb_spi_bus *bus, uint8 Data); 
    // 判断总线是否繁忙，true: 总线繁忙；false: 总线不忙
    boolean (*spi_bus_is_busy)(struct rhb_spi_bus *bus);  
    // 清空SPI总线接收寄存器
    void (*spi_bus_flush)(struct rhb_spi_bus *bus);                
} rhb_spi_bus;

/*********************************************************************************************************
  实现外部函数声明
*********************************************************************************************************/
extern rhb_spi_bus __GtSpiBusDriver;

#ifdef __cplusplus
    }
#endif      //  END OF __cplusplus

#endif      // END OF __RHB_SPI_BUS_ADAPTER_H__

/*********************************************************************************************************
** End of File
*********************************************************************************************************/