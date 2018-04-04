/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_dma.c
** Last modified Date:  2013-04-23
** Last Version:        v1.0
** Description:         dma模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-23
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
#include "hal/arch/inc/arch_io_dma.h"


/*********************************************************************************************************
** Function name:       DMAModeSet
** Descriptions:        DMA模块工作模式配置
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      ucConfig        DMA配置参数，可以取下列几组参数的或
**                      DMA_DIR_M_READ            //  从存储器读，反之为外设读
**                      DMA_CIRC_EN               //  执行循环操作模式
**                      DMA_PINC_EN               //  使能外设地址增量模式
**                      DMA_MINC_EN               //  使能存储器地址增量模式
**                      //  下列四个必须取其中的一个与其它值进行或运算
**                      DMA_PSIZE_8               //  外设数据宽度为8位
**                      DMA_PSIZE_16              //  外设数据宽度为16位
**                      DMA_PSIZE_32              //  外设数据宽度为32位
**                      DMA_PSIZE_RESERVED        //  外设数据宽度保留
**                      //  下列四个必须取其中的一个与其它值进行或运算
**                      DMA_MSIZE_8               //  存储器数据宽度为8位
**                      DMA_MSIZE_16              //  存储器数据宽度为16位
**                      DMA_MSIZE_32              //  存储器数据宽度为32位
**                      DMA_MSIZE_RESERVED        //  存储器数据宽度保留
**                      //  下列四个必须取其中的一个与其它值进行或运算
**                      DMA_PL_LOW                //  通道优先级为低
**                      DMA_PL_MEDIUM             //  通道优先级为中
**                      DMA_PL_HIGH               //  通道优先级为高
**                      DMA_PL_HITHEST            //  通道优先级为最高
**
**                      DMA_MTOM_EN               //  存储器到存储器传输模式使能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAModeSet(unsigned long ulBase, unsigned char ucChannel,unsigned long ulConfig)
{
    unsigned long ulTemp;
    ulTemp = HWREG32(ulBase + DMA_O_CCR1 + (ucChannel-1)*0x14);
    ulTemp &= 0xFFFF800F;
    ulTemp |= ulConfig;
    HWREG32(ulBase + DMA_O_CCR1+ (ucChannel-1)*0x14)= ulTemp;
}

/*********************************************************************************************************
** Function name:       DMAAddrSet
** Descriptions:        DMA传输存储器地址及外设地址设置
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      ulMemAddr       指定DMA传输的存储器地址
**                      ulPeriAddr      指定DMA传输的外设地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAAddrSet(unsigned long ulBase, unsigned char ucChannel, 
                unsigned long ulMemAddr, unsigned long ulPeriAddr)
{
    HWREG32(ulBase + DMA_O_CPAR1 + (ucChannel-1)*0x14)= ulPeriAddr;
    HWREG32(ulBase + DMA_O_CMAR1 + (ucChannel-1)*0x14)= ulMemAddr;
}

/*********************************************************************************************************
** Function name:       DMABufferSizeSet
** Descriptions:        DMA模块传输数据缓冲区大小设置
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      usSize          指定传输的大小
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMABufferSizeSet(unsigned long ulBase, unsigned char ucChannel, unsigned short usSize)
{
    HWREG32(ulBase + DMA_O_CNDTR1 + (ucChannel-1)*0x14)= usSize;
}

/*********************************************************************************************************
** Function name:       DMAEnable
** Descriptions:        DMA模块使能
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAEnable(unsigned long ulBase, unsigned char ucChannel)
{
    HWREG32(ulBase + DMA_O_CCR1 + (ucChannel-1)*0x14) |= 0x00000001;
}

/*********************************************************************************************************
** Function name:       DMAModeSet
** Descriptions:        DMA模块禁能
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMADisable(unsigned long ulBase, unsigned char ucChannel)
{
    HWREG32(ulBase + DMA_O_CCR1 + (ucChannel-1)*0x14) &= 0xFFFFFFFE;
}

/*********************************************************************************************************
** Function name:       DMAIntEnable
** Descriptions:        DMA中断使能
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      ulFlags         指定使能的中断，可以取下列值的或
**                      DMA_INT_TC      //  传输完成中断
**                      DMA_INT_HT      //  传输一半时的中断
**                      DMA_INT_TE      //  传输错误中断
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAIntEnable(unsigned long ulBase, unsigned char ucChannel,unsigned long ulFlags)
{
    HWREG32(ulBase + DMA_O_CCR1 + (ucChannel-1)*0x14) |= ulFlags;
}

/*********************************************************************************************************
** Function name:       DMAIntDisable
** Descriptions:        DMA中断禁能
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      ulFlags         指定使能的中断，可以取下列值的或
**                      DMA_INT_TC      //  传输完成中断
**                      DMA_INT_HT      //  传输一半时的中断
**                      DMA_INT_TE      //  传输错误中断
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAIntDisable(unsigned long ulBase, unsigned char ucChannel,unsigned long ulFlags)
{
    HWREG32(ulBase + DMA_O_CCR1 + (ucChannel-1)*0x14) &= ~ulFlags;
}

/*********************************************************************************************************
** Function name:       DMAIntStatus
** Descriptions:        DMA中断状态获取
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
** output parameters:   NONE
** Returned value:      当前通道的中断状态，可以为下列三个值或者其或运算后，再与DMA_INT_GLOBAL的或，
**                      DMA_INT_GLOBAL，表示全局中断状态，只要下列三个值有一个成立，则就成立
**                      DMA_INT_TC      //  传输完成中断
**                      DMA_INT_HT      //  传输一半时的中断
**                      DMA_INT_TE      //  传输错误中断
*********************************************************************************************************/
unsigned long DMAIntStatus(unsigned long ulBase, unsigned char ucChannel)
{
    unsigned long ulTemp;
    ulTemp = HWREG32(ulBase + DMA_O_ISR);
    ulTemp >>= (ucChannel - 1);
    
    return (ulTemp);
}

/*********************************************************************************************************
** Function name:       DMAIntStatus
** Descriptions:        DMA中断禁能
** input parameters:    ulBase          DMA模块的基地址
**                      ucChannel       DMA通道编号，DMA1为1-7，DMA2为1-5
**                      ulIntFlags      当前通道的中断状态，可以为下列三个值的或后，再与DMA_INT_GLOBAL的或，
**                      DMA_INT_GLOBAL，表示全局中断状态，只要下列三个值有一个成立，则就成立
**                      DMA_INT_TC      //  传输完成中断
**                      DMA_INT_HT      //  传输一半时的中断
**                      DMA_INT_TE      //  传输错误中断
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void DMAIntClear(unsigned long ulBase, unsigned char ucChannel,unsigned long ulIntFlags)
{
    unsigned long ulTemp = (ulIntFlags << (ucChannel - 1));
    
    HWREG32(ulBase + DMA_O_IFCR) |= ulTemp;
    
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
