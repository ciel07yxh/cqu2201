/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_gpio.c
** Last modified Date:  2013-04-02
** Last Version:        v1.0
** Description:         gpio模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-02
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
#include "hal/arch/inc/arch_io_gpio.h"


/*********************************************************************************************************
** Function name:       GPIOConfigSet
** Descriptions:        GPIO引脚配置
** input parameters:    ulBase:     GPIO模块的基地址
**                      usPin:      指定配置的引脚
**                      ulConfig:   配置的参数,可以取下列值
**                      // IO设置为输入时取下列值中的一个
**                      GPIO_DIR_IN_Analog      //  模拟输入模式
**                      GPIO_DIR_IN_Floating    //  浮空输入模式
**                      GPIO_DIR_IN_UpDown      //  上拉下拉输入模式
**
**                      // IO设置为输出时设置为下列两组值的或
**                      GPIO_OUT_PP             //  通用推挽模式输出
**                      GPIO_OUT_OD             //  通用开漏模式输出
**                      GPIO_AF_PP              //  复用推挽模式输出
**                      GPIO_AF_OD              //  复用开漏模式输出

**                      GPIO_SPEED_2MHZ         //  输出速度为2MHz
**                      GPIO_SPEED_10MHZ        //  输出速度为10MHz
**                      GPIO_SPEED_50MHZ        //  输出速度为50MHz
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void GPIOConfigSet(unsigned long ulBase, unsigned short usPin, unsigned long ulConfig)
{
    unsigned char i;
    for(i=0; i<8; i++)
    {
        if(usPin & (1<<i))
        {
            HWREG32(ulBase + GPIO_O_CRL) &= ~(0x0F<<(4*i));
            HWREG32(ulBase + GPIO_O_CRL) |= (ulConfig<<(4*i));
            usPin &= ~(1<<i);
        }
        if(!(usPin & 0xFF))
            break;
    }
    
    usPin >>= 8;
    for(i=0; i<8; i++)
    {
        if(usPin & (1<<i))
        {
            HWREG32(ulBase + GPIO_O_CRH) &= ~(0x0F<<(4*i));
            HWREG32(ulBase + GPIO_O_CRH) |= (ulConfig<<(4*i));
            usPin &= ~(1<<i);
        }
        if(!(usPin & 0xFF))
            break;
    }
}

/*********************************************************************************************************
** Function name:       GPIODataPut
** Descriptions:        GPIO数据输出，该函数以16位的形式输出数据
** input parameters:    ulBase:     GPIO模块的基地址
**                      usData:     输出的数据
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void GPIODataPut(unsigned long ulBase, unsigned short usData)
{
    HWREG32(ulBase + GPIO_O_ODR) = usData;
}

/*********************************************************************************************************
** Function name:       GPIODataGet
** Descriptions:        GPIO数据输出，该函数以16位的形式读入数据
** input parameters:    ulBase:     GPIO模块的基地址
** output parameters:   NONE
** Returned value:      读取到的GPIO端口的数据
*********************************************************************************************************/
unsigned short GPIODataGet(unsigned long ulBase)
{
    return (HWREG32(ulBase + GPIO_O_IDR));
}

/*********************************************************************************************************
** Function name:       GPIOBitSet
** Descriptions:        GPIO位设置
** input parameters:    ulBase:     GPIO模块的基地址
**                      usPin:      指定IO引脚
** output parameters:   NONE
** Returned value:      读取到的GPIO端口的数据
*********************************************************************************************************/
void GPIOBitSet(unsigned long ulBase,unsigned short usPin)
{
    HWREG32(ulBase + GPIO_O_BSRR) = usPin;
}

/*********************************************************************************************************
** Function name:       GPIOBitReset
** Descriptions:        GPIO位清除
** input parameters:    ulBase:     GPIO模块的基地址
**                      usPin:      指定IO引脚
** output parameters:   NONE
** Returned value:      读取到的GPIO端口的数据
*********************************************************************************************************/
void GPIOBitReset(unsigned long ulBase,unsigned short usPin)
{
    HWREG32(ulBase + GPIO_O_BRR) = usPin;
}

/*********************************************************************************************************
** Function name:       GPIOToggleBit
** Descriptions:        翻转指定的IO引脚
** input parameters:    u32Base:  指定操作的GPIO模块基地址
**                      u32Pins： 指定操作的GPIO引脚
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void GPIOToggleBit(uint32 u32Base, uint32 u32Pins)
{
    HWREG32(u32Base + GPIO_O_ODR) ^= u32Pins;
}
/*********************************************************************************************************
** Function name:       AFIORemapConfig
** Descriptions:        AFIO外设重映射设置
** input parameters:    u32Base:     GPIO模块的基地址
**                      u32Remap:    Remap功能
** output parameters:   NONE
** Returned value:      读取到的GPIO端口的数据
*********************************************************************************************************/
void AFIORemapEnable(uint32 u32Remap)
{
    HWREG32(AFIO_BASE + AFIO_O_MAPR) |= u32Remap;
}
/*********************************************************************************************************
** Function name:       AFIORemapReset
** Descriptions:        AFIO外设重映射关闭
** input parameters:    u32Base:     GPIO模块的基地址
**                      u32Remap:    Remap功能
** output parameters:   NONE
** Returned value:      读取到的GPIO端口的数据
*********************************************************************************************************/
void AFIORemapReset(uint32 u32Remap)
{
    HWREG32(AFIO_BASE + AFIO_O_MAPR) &= ~u32Remap;
}

/*********************************************************************************************************
** Function name:       AFIOExtiLineConfig
** Descriptions:        AFIO的中断连接线控制
** input parameters:    u32GPIOLinex:   EXTI_GPIO_A....EXTI_GPIO_G
**                      u32Pin:         指定配置的GPIO引脚，取0-15
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void AFIOExtiLineConfig(uint32 u32GPIOLinex, uint32 u32Pin)
{
    uint32 u32Temp = u32Pin / 4;
    uint32 u32Value = u32Pin % 4;

    u32Temp *= 4;
    u32Value *= 4;
    HWREG32(AFIO_BASE + AFIO_O_EXTICR1 + u32Temp) &= ~(((uint32)0x0000000F) << u32Value);
    HWREG32(AFIO_BASE + AFIO_O_EXTICR1 + u32Temp) |= (u32GPIOLinex << u32Value);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
