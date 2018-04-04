/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_spi.c
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
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_spi.h"


/*********************************************************************************************************
** Function name:       SPIInit
** Descriptions:        SPI模块工作模式配置
** input parameters:    ulBase      SPI模块的基地址
**                      ulConfig    SPI模块工作模块初始化配置，可以去下列几组数字的或
**                      // 工作模式方向选择
**                      SPI_DIR_2Lines_FullDuplex       //  两线全双工模式
**                      SPI_DIR_2Lines_RxOnly           //  两线只接收模式
**                      SPI_DIR_1Lines_Rx               //  单线只接受
**                      SPI_DIR_1Lines_Tx               //  单线只发送
**
**                      //  工作主从选择
**                      SPI_MODE_Master                 //  SPI主机模式
**                      SPI_MODE_Slave                  //  SPI从机模式
**
**                      //  数据长度选择
**                      SPI_DATA_Len8                   //  数据长度长度为8
**                      SPI_DATA_Len16                  //  数据长度为16
**
**                      // 无数据收发时，SPI_CLK极性选择
**                      SPI_CPOL_Low                    //  SPI_CLK极性为低电平
**                      SPI_CPOL_High                   //  SPI_CLK极性为高电平
**
**                      // 时钟采样的相位选择
**                      SPI_CPHA_1Edge                  //  在第一个时钟边沿采样
**                      SPI_CPHA_2Edge                  //  在第二个时钟边沿采样
**
**                      // 从机片选管理
**                      SPI_NSS_Soft                    //  片选信号由软件控制
**                      SPI_NSS_Hard                    //  片选信号由硬件控制
**
**                      // SPI波特率分频器选择
**                      SPI_Baud_Prescaler_2            //  波特率分配系数为2
**                      SPI_Baud_Prescaler_4            //  波特率分配系数为4
**                      SPI_Baud_Prescaler_8            //  波特率分配系数为8
**                      SPI_Baud_Prescaler_16           //  波特率分配系数为16
**                      SPI_Baud_Prescaler_32           //  波特率分配系数为32
**                      SPI_Baud_Prescaler_64           //  波特率分配系数为64
**                      SPI_Baud_Prescaler_128          //  波特率分配系数为128
**                      SPI_Baud_Prescaler_256          //  波特率分配系数为256
**
**                      // 发送数据位从MSB开始，还是从LSB开始选择
**                      SPI_First_MSB                   //  MSB先发送
**                      SPI_First_LSB                   //  LSB先发送
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPIInit(unsigned long ulBase,unsigned long ulConfig)
{
    unsigned long ulTemp;
    
    ulTemp = HWREG32(ulBase + SPI_O_CR1);
    
    ulTemp &= 0x3040; //　先清除在配置
    ulTemp |= ulConfig;
    
    /* Write to SPIx CR1 */
    HWREG32(ulBase + SPI_O_CR1) = ulTemp;
  
    /* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
    HWREG32(ulBase + SPI_O_I2SCFGR) &= ~(0x0800);
    
   /* CRC校验的多项式配置 ,暂时采用复位的配置 */
    HWREG32(ulBase + SPI_O_CRCPR) = 0x0007;
}

/*********************************************************************************************************
** Function name:       SPI_SSOutEnable
** Descriptions:        SPI的片选信号输出使能
** input parameters:    ulBase      SPI模块的基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_SSOutEnable(unsigned long ulBase)
{
    HWREG32(ulBase + SPI_O_CR2) |= (1<<2);
}

/*********************************************************************************************************
** Function name:       SPI_SSOutDisable
** Descriptions:        SPI的片选信号输出禁能
** input parameters:    ulBase      SPI模块的基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_SSOutDisable(unsigned long ulBase)
{
    HWREG32(ulBase + SPI_O_CR2) &= ~(1<<2);
}

/*********************************************************************************************************
** Function name:       SPIEnable
** Descriptions:        SPI工作使能
** input parameters:    ulBase      SPI模块的基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPIEnable(unsigned long ulBase)
{
    HWREG32(ulBase + SPI_O_CR1) |= (1<<6);
}

/*********************************************************************************************************
** Function name:       SPIDisable
** Descriptions:        SPI工作禁能
** input parameters:    ulBase      SPI模块的基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPIDisable(unsigned long ulBase)
{
    HWREG32(ulBase + SPI_O_CR1) &= ~(1<<6);
}

/*********************************************************************************************************
** Function name:       SPI_I2S_IntEnable
** Descriptions:        SPI、I2S中断使能
** input parameters:    ulBase      SPI模块的基地址
**                      ulIntFlag   使能的中断源，可以取下列三个数值的的或
**                      SPI_I2S_INT_TXE             //  发送缓冲区空中断
**                      SPI_I2S_INT_RXNE            //  接收缓冲区有数据中断
**                      SPI_I2S_INT_ERR             //  错误中断
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_I2S_IntEnable(unsigned long ulBase,unsigned long ulIntFlag)
{
    HWREG32(ulBase + SPI_O_CR2) |= ulIntFlag;
}

/*********************************************************************************************************
** Function name:       SPI_I2S_IntEDisable
** Descriptions:        SPI、I2S中断禁能
** input parameters:    ulBase      SPI模块的基地址
**                      ulIntFlag   禁能的中断源，可以取下列三个数值的的或
**                      SPI_I2S_INT_TXE             //  发送缓冲区空中断
**                      SPI_I2S_INT_RXNE            //  接收缓冲区有数据中断
**                      SPI_I2S_INT_ERR             //  错误中断
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_I2S_IntEDisable(unsigned long ulBase,unsigned long ulIntFlag)
{
    HWREG32(ulBase + SPI_O_CR2) &= ~ulIntFlag;
}

/*********************************************************************************************************
** Function name:       SPI_I2S_DMAEnable
** Descriptions:        SPI、I2SDMA模式使能
** input parameters:    ulBase      SPI模块的基地址
**                      ulDMAFlag   指定使能的DMA功能是发送还是接收，可以去下列两个值的或
**                      SPI_I2S_DMA_TX               //  发送DMA
**                      SPI_I2S_DMA_RX               //  接受DMA
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_I2S_DMAEnable(unsigned long ulBase,unsigned long ulDMAFlag)
{
    HWREG32(ulBase + SPI_O_CR2) |= ulDMAFlag;
}

/*********************************************************************************************************
** Function name:       SPI_I2S_DMADisable
** Descriptions:        SPI、I2SDMA模式禁能
** input parameters:    ulBase      SPI模块的基地址
**                      ulDMAFlag   指定禁能的DMA功能是发送还是接收，可以去下列两个值的或
**                      SPI_I2S_DMA_TX               //  发送DMA
**                      SPI_I2S_DMA_RX               //  接受DMA
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_I2S_DMADisable(unsigned long ulBase,unsigned long ulDMAFlag)
{
    HWREG32(ulBase + SPI_O_CR2) &= ~ulDMAFlag;
}

/*********************************************************************************************************
** Function name:       SPI_I2S_DataPut
** Descriptions:        SPI、I2S发送数据
** input parameters:    ulBase      SPI模块的基地址
**                      usData      发送的数据，当工作在8位模式时，只有低8位有效
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SPI_I2S_DataPut(unsigned long ulBase,unsigned short usData)
{
    HWREG32(ulBase + SPI_O_DR) = usData;
}

/*********************************************************************************************************
** Function name:       SPI_I2S_DataPut
** Descriptions:        SPI、I2S接收数据
** input parameters:    ulBase      SPI模块的基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
unsigned short SPI_I2S_DataGet(unsigned long ulBase)
{
    return (HWREG32(ulBase + SPI_O_DR));
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
