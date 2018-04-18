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
#include "devs/spi/rhb_spi_bus_adapter.h"

#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_gpio.h"
#include "hal/arch/inc/arch_io_spi.h"

#include "hal/CortexM3/hal_io_level.h"

#include "contiki/stm32_def.h"

/*********************************************************************************************************
** 局部函数声明
*********************************************************************************************************/
static void spi_bus_init(struct rhb_spi_bus *bus);
static void spi_bus_lock(struct rhb_spi_bus *bus);
static void spi_bus_unlock(struct rhb_spi_bus *bus);
static uint8 spi_bus_send_recv_byte(struct rhb_spi_bus *bus, uint8 Data);
static boolean spi_bus_is_busy(struct rhb_spi_bus *bus);
static void spi_bus_flush(struct rhb_spi_bus *bus);

/*********************************************************************************************************
** 定义SPI总线驱动程序接口
*********************************************************************************************************/
rhb_spi_bus __GtSpiBusDriver = {
  .int_state = 0,
  .spi_bus_init = spi_bus_init,
  .spi_bus_lock = spi_bus_lock,
  .spi_bus_unlock = spi_bus_unlock,
  .spi_bus_send_recv_byte = spi_bus_send_recv_byte,
  .spi_bus_is_busy = spi_bus_is_busy,
  .spi_bus_flush = spi_bus_flush
};

/*********************************************************************************************************
** Function name:       spi_bus_init
** Descriptions:        SPI总线硬件初始化
** input parameters:    bus: SPI总线控制器
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
static void spi_bus_init(struct rhb_spi_bus *bus)
{
    SystemPeripheralEnable(RCC_PERIPHERAL_IOPA);
    SystemPeripheralEnable(RCC_PERIPHERAL_SPI1);
    
    // CLK、MOSI与片选引脚SS引脚设置为推挽复用输出
    GPIOConfigSet(GPIOA_BASE, GPIO_PIN_5 | GPIO_PIN_7, GPIO_AF_PP | GPIO_SPEED_50MHZ);
    // MISO设置为浮空输入
    GPIOConfigSet(GPIOA_BASE, GPIO_PIN_6, GPIO_DIR_IN_Floating);

    // 初始SPI接口
    SPIInit(SPI1_BASE, SPI_DIR_2Lines_FullDuplex | SPI_MODE_Master |
                       SPI_DATA_Len8 | SPI_CPOL_Low | SPI_CPHA_1Edge |
                       SPI_NSS_Soft | SPI_Baud_Prescaler_32 | SPI_First_MSB);
    SPI_SSOutEnable(SPI1_BASE);
    SPIEnable(SPI1_BASE);
}

/*********************************************************************************************************
** Function name:       spi_bus_init
** Descriptions:        锁定总线，避免多线程操作的冲突
** input parameters:    bus: SPI总线控制器
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
static void spi_bus_lock(struct rhb_spi_bus *bus)
{
    bus->int_state = hal_enter_critical(1);
}

/*********************************************************************************************************
** Function name:       spi_bus_unlock
** Descriptions:        接触锁定总线，其他SPI设备可以使用总线了
** input parameters:    bus: SPI总线控制器
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
static void spi_bus_unlock(struct rhb_spi_bus *bus)
{
   hal_exit_critical(bus->int_state);
}
/*********************************************************************************************************
** Function name:       spi_bus_send_recv_byte
** Descriptions:        SPI总线发送同时接收一个字节
** input parameters:    bus: SPI总线控制器
**                      Data: 总线发送的数据
** output parameters:   NONE
** Returned value:      接收到的数据
*********************************************************************************************************/
static uint8 spi_bus_send_recv_byte(struct rhb_spi_bus *bus, uint8 Data)
{
  uint8 temp;
  
  // 等待发送寄存器为空
  while(!(HWREG32(SPI1_BASE + SPI_O_SR) & 0x02));
  // 填充发送寄存器，开始发送
  HWREG32(SPI1_BASE + SPI_O_DR) = Data;
  // 等待接收寄存器不空
  while(!(HWREG32(SPI1_BASE + SPI_O_SR) & 0x01));
  // 读回返回数据
  temp = HWREG32(SPI1_BASE + SPI_O_DR);

  return temp;
}

/*********************************************************************************************************
** Function name:       spi_bus_is_busy
** Descriptions:        判断SPI总线是否繁忙
** input parameters:    bus: SPI总线控制器
** output parameters:   NONE
** Returned value:      true: 总线繁忙；false: 总线不忙
*********************************************************************************************************/
static boolean spi_bus_is_busy(struct rhb_spi_bus *bus)
{
  // 如果总线忙
  if(HWREG32(SPI1_BASE+SPI_O_SR) & 0x80) {
 // if((HWREG32(SPI1_BASE+SPI_O_SR) & 0x01)) {
      return true;
  } else {
      return false;
  }
}

/*********************************************************************************************************
** Function name:       spi_bus_flush
** Descriptions:        清空spi总线接收寄存器
** input parameters:    bus: SPI总线控制器
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
static void spi_bus_flush(struct rhb_spi_bus *bus)
{
  uint8 temp;

  // 等待总线不忙
  while(HWREG32(SPI2_BASE+SPI_O_SR) & 0x80);
  
  if((HWREG32(SPI1_BASE+SPI_O_SR) & 0x01)){
     temp = HWREG32(SPI1_BASE + SPI_O_DR);
     temp = temp;
  }
}

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
