/****************************************Copyright (c)****************************************************
**
**                                     
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           rhb_24c16.h
** Last modified Date:  2011-1-11
** Last Version:        v1.0
** Description:         操作24C16实现函数
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2011-1-11
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
#ifndef  __RHB_24C16_H__
#define  __RHB_24C16_H__

#include "dev/I2C/common/rhb_softI2C_Host.h"
#include "dev/I2C/24C02/rhb_24c02.h"

/*********************************************************************************************************
  24c16的相关宏定义
*********************************************************************************************************/
#define  rhb_I2C_24C16_ADDR         0xA0    //  器件在总线上的地址
#define  rhb_24C16_DEVICE_ADDR_MAX  0x7FF   //  器件内部存储单元最大地址,器件共16K位，即2K个字节，最大地址为0x7FF
#define  rhb_I2C_24C16_PAGE_SEZE    16      //  页大小

//  当读取数据时，key取值只能为I2C_24c16_Normal
#define  I2C_24C16_Normal   rhb_I2C_Normal      //  正常操作24c16
#define  I2C_24C16_AllSet   rhb_I2C_AllSet      //  将指定地址的数据置为0xFF
#define  I2C_24C16_AllReset rhb_I2C_AllReset    //  将指定地址的数据置为0x00

/*********************************************************************************************************
** 表征串口的结构体定义
*********************************************************************************************************/
typedef struct
{
    void (*i2c_operate)(unsigned char I2CAddr, unsigned long ulAddr, 
                   unsigned char addrLen, void *buf,unsigned long *len);
    OS_EVENT        *sem_I2C;                         // 保护信号量，避免多线程操作总线
} rhb_24c16_info;

/*********************************************************************************************************
** 外部函数的声明
*********************************************************************************************************/
extern rhb_24c16_info __tGrhb_24c16;
extern rhb_ErrNo rhb_24c16_init(rhb_io_handle handle);
extern rhb_ErrNo rhb_24c16_fread(rhb_io_handle handle, unsigned long ulAddr, 
                                 unsigned char key, void *buf, unsigned long *len);
extern rhb_ErrNo rhb_24c16_fwrite(rhb_io_handle handle, unsigned long ulAddr, 
                          unsigned char key, void *buf, unsigned long *len);
extern rhb_ErrNo rhb_24c16_IsBusy(rhb_io_handle handle);
extern rhb_ErrNo rhb_24c16_setConfig(rhb_io_handle handle, void *data, unsigned long *len);
extern rhb_ErrNo rhb_24c16_getConfig(rhb_io_handle handle, void *data, unsigned long *len);

#endif // endif of __RHB_24C16_H__

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
