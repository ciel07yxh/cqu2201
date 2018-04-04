/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_usart.c
** Last modified Date:  2013-04-02
** Last Version:        v1.0
** Description:         usart及uart模块寄存器封装函数实现声明
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
#include "hal/arch/inc/arch_io_usart.h"


/*********************************************************************************************************
** Function name:       UsartConfigSet
** Descriptions:        usart模块配置
** input parameters:    ulBase  usart模块基地址
**                      ulClock 提供给usart模块的时钟，usart1为APB2的时钟，最高72M，其它为APB1的时钟，最高36M
**                      ulBaud  通信波特率,直接指定即可
**                      ulConfig 配置信息，可以取下列三组值的或
**                      USART_STOP_0_5          //  0.5位停止位
**                      USART_STOP_1            //  1位停止位
**                      UASRT_STOP_1_5          //  1.5位停止位
**                      USART_STOP_2            //  2位停止位

**                      USART_WLEN_8            //  字长为8
**                      USART_WLEN_9            //  字长为9

**                      USART_PAR_NONE          //  无校验
**                      USART_PAR_EVEN          //  偶校验
**                      USART_PAR_ODD           //  奇校验
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartConfigSet(unsigned long ulBase, unsigned long ulClock, 
                    unsigned long ulBaud, unsigned long ulConfig)
{
    unsigned long ulTemp;
    unsigned long ulIntergerDiv;
    unsigned long ulFloatDiv;
    
    //  设置停止位，即配置寄存器CR2
    ulTemp = HWREG32(ulBase + USART_O_CR2);
    ulTemp &= 0xCFFF;
    ulTemp |= (ulConfig << 12);
    HWREG32(ulBase + USART_O_CR2) = ulTemp;
    
    //  配置寄存器CR1，即配置字长及校验
    ulTemp = HWREG32(ulBase + USART_O_CR1);
    ulTemp &= 0xE9F3;
    ulTemp |= ulConfig;
    ulTemp |= 0x000C;    //  使能发送和接收
    HWREG32(ulBase + USART_O_CR1) = ulTemp;
    
    //  注意：流控功能这里暂时不配置，需要的话在这里添加代码
    
    //  根据波特率计算BRR寄存器的值，并写入其中
    //  先计算整数部分
    if((HWREG32(ulBase + USART_O_CR1)) & 0x8000) {
        ulIntergerDiv = ((25 * ulClock) / (2 * ulBaud));
    } else {
        ulIntergerDiv = ((25 * ulClock) / (4 * ulBaud));
    }
    ulTemp = ((ulIntergerDiv / 100) << 4);
    //  计算小数部分
    ulFloatDiv = ulIntergerDiv - (100*(ulTemp >> 4));
    if((HWREG32(ulBase + USART_O_CR1)) & 0x8000) {
        ulTemp |= ((((ulFloatDiv * 8) + 50) / 100)) & ((unsigned char)0x07);
    } else {
        ulTemp |= ((((ulFloatDiv * 16) + 50) / 100)) & ((unsigned char)0x0F);
    }
    
    HWREG32(ulBase + USART_O_BRR) = ulTemp;
}

/*********************************************************************************************************
** Function name:       UsartEnable
** Descriptions:        usart模块使能，使能后模块才能正常工作
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartEnable(unsigned long ulBase)
{
    HWREG32(ulBase + USART_O_CR1) |= 0x2000;
}

/*********************************************************************************************************
** Function name:       UsartDisable
** Descriptions:        usart模块禁能，禁能后模块才能停止工作，在重新配置usart前通常先调用该函数
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartDisable(unsigned long ulBase)
{
    HWREG32(ulBase + USART_O_CR1) &= 0xDFFF;
}


/*********************************************************************************************************
** Function name:       UsartDMAEnable
** Descriptions:        usart模块DMA功能使能
** input parameters:    ulBase  usart模块基地址
**                      ulFlags 指定使能的DMA功能，可以取下列值
**                      USART_DMA_TX    //  DMA发送功能
**                      USART_DMA_RX    //  DMA接受功能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartDMAEnable(unsigned long ulBase,unsigned long ulFlags)
{
  HWREG32(ulBase + USART_O_CR3) |= ulFlags;
}
/*********************************************************************************************************
** Function name:       UsartDMADisable
** Descriptions:        usart模块DMA功能禁能
** input parameters:    ulBase  usart模块基地址
**                      ulFlags 指定禁能的DMA功能，可以取下列值
**                      USART_DMA_TX    //  DMA发送功能
**                      USART_DMA_RX    //  DMA接受功能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartDMADisable(unsigned long ulBase, unsigned long ulFlags)
{
  HWREG32(ulBase + USART_O_CR3) &= ~ulFlags;
}
/*********************************************************************************************************
** Function name:       UsartIntEnable
** Descriptions:        usart模块中断使能
** input parameters:    ulBase:     usart模块基地址
**                      ulIntFlags: 需要使能的中断, 这里仅仅列出常用的中断，其它的可以通过寄存器设置，可以
**                                  取下列值的或
**                      USART_INT_TX_EMPTY          //  发送缓冲区空中断
**                      USART_INT_TX_FINISH         //  发送完成中断
**                      USART_INT_RX                //  接收中断使能，说明接收寄存器中有数据了
**                      USART_INT_IDLE              //  IDLE中断使能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartIntEnable(unsigned long ulBase,unsigned long ulIntFlags)
{
    HWREG32(ulBase + USART_O_CR1) |= ulIntFlags;
}

/*********************************************************************************************************
** Function name:       UsartIntEnable
** Descriptions:        usart模块中断禁能
** input parameters:    ulBase:     usart模块基地址
**                      ulIntFlags: 需要禁能的中断, 这里仅仅列出常用的中断，其它的可以通过寄存器设置，可以
**                                  取下列值的或
**                      USART_INT_TX_EMPTY          //  发送缓冲区空中断
**                      USART_INT_TX_FINISH         //  发送完成中断
**                      USART_INT_RX                //  接收中断使能，说明接收寄存器中有数据了
**                      USART_INT_IDLE              //  IDLE中断使能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartIntDisable(unsigned long ulBase,unsigned long ulIntFlags)
{
    HWREG32(ulBase + USART_O_CR1) &= ~ulIntFlags;
}

/*********************************************************************************************************
** Function name:       UsartSpaceAvial
** Descriptions:        判断发送寄存器是否为空，即是否可以发送下一个数据
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      true: 可以发送下一个数据 ；false：不能发送下一个数据
*********************************************************************************************************/
boolean UsartSpaceAvial(unsigned long ulBase)
{
    if (HWREG32(ulBase + USART_O_SR) & 0x0080)
        return true;
    else
        return false;
}

/*********************************************************************************************************
** Function name:       UsartCharPut
** Descriptions:        发送数据
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartCharPut(unsigned long ulBase,unsigned char ucData)
{
    HWREG32(ulBase + USART_O_DR) = ucData;           //  将数据写入发送寄存器
}

/*********************************************************************************************************
** Function name:       UsartCharsAvial
** Descriptions:        判断接收寄存器中是否有可用数据
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      true: 接受寄存器中有可用数据，可以读出; false: 接收寄存器中无可用数据
*********************************************************************************************************/
boolean UsartCharsAvial(unsigned long ulBase)
{
    if(HWREG32(ulBase + USART_O_SR) & 0x0020)
        return true;
    else
        return false;
}

/*********************************************************************************************************
** Function name:       UsartCharPut
** Descriptions:        接收数据，即从接收寄存器中读出数据
** input parameters:    ulBase  usart模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
unsigned long  UsartCharGet(unsigned long ulBase)
{
    return (HWREG32(ulBase + USART_O_DR) & 0x01FF);   //  读出数据，低9为有效
}

/*********************************************************************************************************
** Function name:       UsartPutS
** Descriptions:        发送一个指定的字符串
** input parameters:    ulBase: usart模块基地址
**                      s:      指定发送的字符串
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartPutS(unsigned long ulBase,const char *s)
{
    while (*s != '\0') {
       while(!(UsartSpaceAvial(ulBase))) ;          //  等待发送寄存器可用
       UsartCharPut(ulBase,*(s++));
    }
}
/*********************************************************************************************************
** Function name:       UsartPutS
** Descriptions:        发送一个指定长度的字符串
** input parameters:    ulBase: usart模块基地址
**                      s:      指定发送字符串的基地址
**                      usNum:  指定发送字符串的长度
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void UsartPutN(unsigned long ulBase, const  char  *pStr , unsigned short  usNum)
{
    while(usNum) 
    {
        if(UsartSpaceAvial(ulBase))                 //  如果发送缓冲区有空间，则填充发送寄存器
        {  
            UsartCharPut(ulBase,*pStr++);
            usNum--;
        }
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
