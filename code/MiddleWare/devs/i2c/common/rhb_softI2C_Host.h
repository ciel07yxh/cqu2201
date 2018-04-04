/****************************************Copyright (c)****************************************************
**
**
**                                 http://www.ourtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           rhb_softI2C_Host.h
** Last modified Date:  2010-8-05
** Last Version:        v1.0
** Description:         软件模拟I2C主机的实现软件包配置及函数声明文件
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo
** Created date:        2010-8-05
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
#ifndef  __RHB_SOFTI2C_HOST_H__
#define  __RHB_SOFTI2C_HOST_H__

#ifdef __cplusplus
extern "C" {
#endif

  
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
/*********************************************************************************************************
  软件模拟I2C实现配置函数
*********************************************************************************************************/
#define rhb_I2C_Delay        10   //  定义延时，决定I2C总线的速度，为0时不使用延时函数。大于0时需要提供一个
                                  //  delay(a)的延时函数

#define rhb_SCL_H()          (HWREG(GPIOC_BASE + GPIO_O_BSRR) = 0x4000)     //  将时钟线SCL置高
#define rhb_SCL_L()          (HWREG(GPIOC_BASE + GPIO_O_BRR) = 0x4000)      //  将时钟线SCL置低

#define rhb_SDA_H()          (HWREG(GPIOC_BASE + GPIO_O_BSRR) = 0x8000)     //  将数据线SDA置高
#define rhb_SDA_L()          (HWREG(GPIOC_BASE + GPIO_O_BRR) = 0x8000)      //  将数据项SDA置低

#define rhb_I2C_READ      0x01
#define rhb_I2C_WRITE     0x00

//  当读取数据时，key取值只能为I2C_24c16_Normal
#define  rhb_I2C_Normal         0x00    //  正常操作
#define  rhb_I2C_AllSet         0x01    //  将指定地址的数据置为0xFF
#define  rhb_I2C_AllReset       0x02    //  将指定地址的数据置为0x00

//  如果没有tBoolean类型，需要用户定义，如果有逻辑类型，则定义为逻辑类型；反正定义为unsigned char。如下：
//  typedef unsigned char tBoolean;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/*********************************************************************************************************
  实现外部函数声明
*********************************************************************************************************/
extern void rhb_I2C_Start(void);
extern void rhb_I2C_Stop(void);
extern void rhb_I2C_Ack(void);
extern void rhb_I2C_NoAck(void);
extern tBoolean rhb_I2C_WaitAck(void);
extern void rhb_I2C_SendByte(unsigned char ucByte);
extern unsigned char rhb_I2C_RecvByte(void);
extern void rhb_i2c_read(unsigned char I2CAddr, unsigned long ulAddr, 
                         unsigned char addrLen, void *buf, unsigned long *len);
extern void rhb_i2c_write(unsigned char I2CAddr, unsigned long ulAddr, 
                          unsigned char addrLen, void *buf, unsigned long *len);
extern void rhb_i2c_set(unsigned char I2CAddr, unsigned long ulAddr, 
                        unsigned char addrLen, void *buf, unsigned long *len);
extern void rhb_i2c_reset(unsigned char I2CAddr, unsigned long ulAddr, 
                          unsigned char addrLen, void *buf, unsigned long *len);
#ifdef __cplusplus
    }
#endif      //  END OF __cplusplus

#endif      // END OF __RHB_SOFTI2C_M_H__

/*********************************************************************************************************
** End of File
*********************************************************************************************************/