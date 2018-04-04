/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_reg.h
** Last modified Date:  2013-04-01
** Last Version:        v1.0
** Description:         IO外设寄存器定义
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-01
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

#ifndef __ARCH_IO_REG_H__
#define __ARCH_IO_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
** RCC模块寄存器定义
*********************************************************************************************************/
#define RCC_O_CR            0x0000
#define RCC_O_CFGR          0x0004
#define RCC_O_CIR           0x0008
#define RCC_O_APB2RSTR      0x000C
#define RCC_O_APB1RSTR      0x0010
#define RCC_O_AHBENR        0x0014
#define RCC_O_APB2ENR       0x0018
#define RCC_O_APB1ENR       0x001C
#define RCC_O_BDCR          0x0020
#define RCC_O_CSR           0x0024

/*********************************************************************************************************
** GPIO模块寄存器定义
*********************************************************************************************************/
#define GPIO_O_CRL          0x0000
#define GPIO_O_CRH          0x0004
#define GPIO_O_IDR          0x0008
#define GPIO_O_ODR          0x000C
#define GPIO_O_BSRR         0x0010
#define GPIO_O_BRR          0x0014
#define GPIO_O_LCKR         0x0018

/* AFIO模块的寄存器定义 */
#define AFIO_O_EVCR         0x0000
#define AFIO_O_MAPR         0x0004
#define AFIO_O_EXTICR1      0x0008
#define AFIO_O_EXTICR2      0x000C
#define AFIO_O_EXTICR3      0x0010
#define AFIO_O_EXTICR4      0x0014

/*********************************************************************************************************
** EXTI模块寄存器定义
*********************************************************************************************************/
#define EXTI_O_IMR          0x0000
#define EXTI_O_EMR          0x0004
#define EXTI_O_RTSR         0x0008
#define EXTI_O_FTSR         0x000C
#define EXTI_O_SWIER        0x0010
#define EXTI_O_PR           0x0014

/*********************************************************************************************************
** DMA模块寄存器定义
*********************************************************************************************************/
#define DMA_O_ISR       0x0000    //  中断状态寄存器
#define DMA_O_IFCR      0x0004    //  中断清除寄存器
#define DMA_O_CCR1      0x0008    //  DMA通道x配置寄存器，这里为通道1，其它通道地址为该值+(0x14*(通道号-1))
#define DMA_O_CNDTR1    0x000C    //  DMA通道x传输数量寄存器，这里为通道1，其它通道地址为该值+(0x14*(通道号-1))
#define DMA_O_CPAR1     0x0010    //  DMA通道x外设地址寄存器，这里为通道1，其它通道地址为该值+(0x14*(通道号-1))
#define DMA_O_CMAR1     0x0014    //  DMA通道x存储器地址寄存器，这里为通道1，其它通道地址为该值+(0x14*(通道号-1))

/*********************************************************************************************************
** USART模块寄存器定义
*********************************************************************************************************/
#define USART_O_SR          0x0000
#define USART_O_DR          0x0004
#define USART_O_BRR         0x0008
#define USART_O_CR1         0x000C
#define USART_O_CR2         0x0010
#define USART_O_CR3         0x0014
#define USART_O_GTPR        0x0018

/*********************************************************************************************************
** Ethernet模块寄存器定义
*********************************************************************************************************/
#define ETH_MACCR           0x0000
#define ETH_MACFFR          0x0004
#define ETH_MACHTHR         0x0008
#define ETH_MACHTLR         0x000C
#define ETH_MACMIIAR        0x0010
#define ETH_MACMIIDR        0x0014
#define ETH_MACFCR          0x0018
#define ETH_MACVLANTR       0x001C
#define ETH_MACRWUFFR       0x0028
#define ETH_MACPMTSCR       0x002C
#define ETH_MACDBGR         0x0034    // STM32F107没有的寄存器
#define ETH_MACSR           0x0038
#define ETH_MACIMR          0x003C
#define ETH_MACA0HR         0x0040
#define ETH_MACA0LR         0x0044
#define ETH_MACA1HR         0x0048
#define ETH_MACA1LR         0x004C
#define ETH_MACA2HR         0x0050
#define ETH_MACA2LR         0x0054
#define ETH_MACA3HR         0x0058
#define ETH_MACA3LR         0x005C

/*  下面是MMC控制寄存器的定义  */
#define ETH_MMCCR           0x0100
#define ETH_MMCRIR          0x0104
#define ETH_MMCTIR          0x0108
#define ETH_MMCRIMR         0x010C
#define ETH_MMCTIMR         0x0110
#define ETH_MMCTGFSCCR      0x014C
#define ETH_MMCTGFMSCCR     0x0150
#define ETH_MMCTGFCR        0x0168
#define ETH_MMCRFCECR       0x0194
#define ETH_MMCRFAECR       0x0198
#define ETH_MMCRGUFCR       0x01C4

/*  下面是PTP模块寄存器的定义  */
#define ETH_PTPTSCR         0x0700
#define ETH_PTPSSIR         0x0704
#define ETH_PTPTSHR         0x0708
#define ETH_PTPTSLR         0x070C
#define ETH_PTPTSHUR        0x0710
#define ETH_PTPTSLUR        0x0714
#define ETH_PTPTSAR         0x0718
#define ETH_PTPTTHR         0x071C
#define ETH_PTPTTLR         0x0720
#define ETH_PTPTSSR         0x0728      // STM32F107没有的寄存器
#define ETH_PTPPPSCR        0x072C      // STM32F107没有的寄存器

/*  下面是DMA模块寄存器的定义  */
#define ETH_DMABMR          0x1000
#define ETH_DMATPDR         0x1004
#define ETH_DMARPDR         0x1008
#define ETH_DMARDLAR        0x100C
#define ETH_DMATDLAR        0x1010
#define ETH_DMASR           0x1014
#define ETH_DMAOMR          0x1018
#define ETH_DMAIER          0x101C
#define ETH_DMAMFBOCR       0x1020
#define ETH_DMARSWTR        0x1024　　　// STM32F107没有的寄存器
#define ETH_DMACHTDR        0x1048
#define ETH_DMACHRDR        0x104C
#define ETH_DMACHTBAR       0x1050
#define ETH_DMACHRBAR       0x1054

/*********************************************************************************************************
** Flash控制模块寄存器定义
*********************************************************************************************************/
#define FLASH_O_ACR         0x0000
#define FLASH_O_KEYR        0x0004
#define FLASH_O_OPTKEYR     0x0008
#define FLASH_O_SR          0x000C
#define FLASH_O_CR          0x0010
#define FLASH_O_AR          0x0014
#define FLASH_O_OBR         0x001C
#define FLASH_O_WRPRR       0x0020

#define FLASH_O_KEYR2       0x0044
#define FLASH_O_SR2         0x004C
#define FLASH_O_CR2         0x0050
#define FLASH_O_AR2         0x0054

/*  常用的常量定义  */
#define FLASH_UNLOCK_KER1   0x45670123
#define FLASH_UNLOCK_KER2   0xCDEF89AB

/*********************************************************************************************************
** FSMC模块寄存器定义
*********************************************************************************************************/
#define FSMC_BCR1           0x0000
#define FSMC_BCR2           0x0008
#define FSMC_BCR3           0x0010
#define FSMC_BCR4           0x0018
#define FSMC_BTR1           0x0004
#define FSMC_BTR2           0x000C
#define FSMC_BTR3           0x0014
#define FSMC_BTR4           0x001C
#define FSMC_BWTR1          0x0104
#define FSMC_BWTR2          0x010C
#define FSMC_BWTR3          0x0114
#define FSMC_BWTR4          0x011C
#define FSMC_PCR2           0x0060
#define FSMC_PCR3           0x0080
#define FSMC_PCR4           0x00A0
#define FSMC_SR2            0x0064
#define FSMC_SR3            0x0084
#define FSMC_SR4            0x00A4
#define FSMC_PMEM2          0x0068
#define FSMC_PMEM3          0x0088
#define FSMC_PMEM4          0x00A8
#define FSMC_PATT2          0x006C
#define FSMC_PATT3          0x008C
#define FSMC_PATT4          0x00AC
#define FSMC_PIO4           0x00B0
#define FSMC_ECCR2          0x0074
#define FSMC_ECCR3          0x0094

/*********************************************************************************************************
** SPI及I2S模块寄存器定义
*********************************************************************************************************/
#define SPI_O_CR1           0x0000
#define SPI_O_CR2           0x0004
#define SPI_O_SR            0x0008
#define SPI_O_DR            0x000C
#define SPI_O_CRCPR         0x0010
#define SPI_O_RXCRCR        0x0014
#define SPI_O_TXCRCR        0x0018
#define SPI_O_I2SCFGR       0x001C
#define SPI_O_I2SPR         0x0020

/*********************************************************************************************************
** TIM模块的寄存器定义
*********************************************************************************************************/
#define TIMx_O_CR1            0x0000
#define TIMx_O_CR2            0x0004
#define TIMx_O_SMCR           0x0008
#define TIMx_O_DIER           0x000C
#define TIMx_O_SR             0x0010
#define TIMx_O_EGR            0x0014
#define TIMx_O_CCMR1          0x0018
#define TIMx_O_CCMR2          0x001C
#define TIMx_O_CCER           0x0020
#define TIMx_O_CNT            0x0024
#define TIMx_O_PSC            0x0028
#define TIMx_O_ARR            0x002C
#define TIMx_O_CCR1           0x0034
#define TIMx_O_CCR2           0x0038
#define TIMx_O_CCR3           0x003C
#define TIMx_O_CCR4           0x0040
#define TIMx_O_DCR            0x0048
#define TIMx_O_DMAR           0x004C
  
/*********************************************************************************************************
** SDIO模块寄存器定义
*********************************************************************************************************/
#define SDIO_POWER          0x0000
#define SDIO_CLKCR          0x0004
#define SDIO_ARG            0x0008
#define SDIO_CMD            0x000C
#define SDIO_RESPCMD        0x0010

#define SDIO_RESP_BASE      0x0014
#define SDIO_RESP1          0x0000
#define SDIO_RESP2          0x0004
#define SDIO_RESP3          0x0008
#define SDIO_RESP4          0x000C

#define SDIO_DTIMER         0x0024
#define SDIO_DLEN           0x0028
#define SDIO_DCTRL          0x002C
#define SDIO_DCOUNT         0x0030
#define SDIO_STA            0x0034
#define SDIO_ICR            0x0038
#define SDIO_MASK           0x003C
#define SDIO_FIFOCNT        0x0048
#define SDIO_FIFO           0x0080




#ifdef __cplusplus
    }
#endif      // __cplusplus

#endif // endif of __ARCH_IO_REG_H__
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
