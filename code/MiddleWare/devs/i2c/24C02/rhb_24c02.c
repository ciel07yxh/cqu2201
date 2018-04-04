/****************************************Copyright (c)****************************************************
**
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           rhb_24c02.c
** Last modified Date:  2010-11-9
** Last Version:        v1.0
** Description:         STM32串口驱动中间件函数实现
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2010-11-9
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
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_usart.h"
#include "hw_ints.h"
#include "hw_dma.h"
#include "dma.h"
#include "usart.h"
#include "interrupt.h"
#include "string.h"

#include <os_cpu.h>
#include <os_cfg.h>
#include <ucos_ii.h>

#include "io/rhb_io.h"

#include "dev/I2C/24C02/rhb_24C02.h"


rhb_24c02_info __tGrhb_24c02 = {NULL, NULL};
/*********************************************************************************************************
** Function name:       rhb_24c02_init
** Descriptions:        初始化I2C设备，由系统内核调用
** input parameters:    handle: 外设特性描述符
**                      NONE
** output parameters:   NONE
** Returned value:      1:初始化设备成功 
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_init(rhb_io_handle handle)
{
   rhb_24c02_info *i2c_handle = handle;
   
   i2c_handle->sem_I2C = OSSemCreate(1);
   i2c_handle->i2c_operate = rhb_i2c_read;
   
   return 1;
}

/*********************************************************************************************************
** Function name:       rhb_24c02_fread
** Descriptions:        从24C02指定存储单元开始读取指定直接的数据
** input parameters:    handle:   外设特性描述符
**                      ulAddr：  24C02读取起始地址
**                      key：     读取特性，这里只能为I2C_24C02_Normal
**                      buf:      读取数据存放地址
**                      len:      读取数据长度指针
** output parameters:   NONE
** Returned value:      调用正确，返回1；反之，返回0
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_fread(rhb_io_handle handle, unsigned long ulAddr, 
                          unsigned char key, void *buf, unsigned long *len) 
{
    INT8U err;
    unsigned long ulCount = *len;
    rhb_24c02_info *i2c_handle = handle;
    
    if((buf == NULL)|| ((*len) <= 0) || (len==NULL))
      return 0;
    
    if(key != I2C_24C02_Normal)
    {
        *len = 0;
        return 0;
    } else {
        i2c_handle->i2c_operate = rhb_i2c_read;
    }
    
    OSSemPend(i2c_handle->sem_I2C,0,&err);
    if(err == OS_ERR_NONE)
    {
        if(ulAddr > DEVICE_ADDR_MAX) {
            *len = 0;
            OSSemPost(i2c_handle->sem_I2C);
            return 0;                                   //   读取地址超过器件存储单元最大地址，返回0
        }
        if((ulAddr + ulCount - 1) > DEVICE_ADDR_MAX) {  //   指定地址的剩余字节数不够要求读取的字节数
            ulCount = DEVICE_ADDR_MAX - ulAddr + 1;     //   则仅仅读取完剩余的字节
            *len = ulCount;
        }
        //  开始读取数据
        i2c_handle->i2c_operate(I2C_24C02_ADDR, ulAddr, 1, buf, &ulCount);
        ulAddr += ulCount;
    }
    OSSemPost(i2c_handle->sem_I2C);
    
    return 1;
}

/*********************************************************************************************************
** Function name:       rhb_24c02_fwrite
** Descriptions:        从24C02指定存储单元开始写入一定的数据
** input parameters:    handle:   外设特性描述符
**                      ulAddr：  24C02写入起始地址
**                      key：     写入特性，可以取下列三个值
**                      I2C_24C02_Normal            //  正常操作24C02
**                      I2C_24C02_AllSet            //  将指定地址的数据置为0xFF
**                      I2C_24C02_AllReset          //  将指定地址的数据置为0x00
**                      buf:      写入数据起始地址
**                      len:      写入数据长度指针
** output parameters:   NONE
** Returned value:      调用正确，返回1；反之，返回0
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_fwrite(rhb_io_handle handle, unsigned long ulAddr, 
                          unsigned char key, void *buf, unsigned long *len)
{
    INT8U err;
    unsigned long ulCount = *len;
    unsigned long ulNumOnce = 0;
    unsigned char *data = buf;
    rhb_24c02_info *i2c_handle = handle;
    
    if(((*len) <= 0) || (len==NULL))
      return 0;
    
    OSSemPend(i2c_handle->sem_I2C,0,&err);
    if(err == OS_ERR_NONE)
    {
        if(ulAddr > DEVICE_ADDR_MAX) {
            *len = 0;
            OSSemPost(i2c_handle->sem_I2C);
            return 0;                                   //   写取地址超过器件存储单元最大地址，返回0
        }
        if((ulAddr + ulCount - 1) > DEVICE_ADDR_MAX) {  //   指定地址的剩余字节数不够要求读取的字节数
            ulCount = DEVICE_ADDR_MAX - ulAddr + 1;     //   则仅仅写完剩余的字节
            *len = ulCount;
        }
        
        switch(key)                                     //     选择操作方式
        {
            case I2C_24C02_Normal:
                i2c_handle->i2c_operate = rhb_i2c_write;
                break;
            case I2C_24C02_AllSet:
                i2c_handle->i2c_operate = rhb_i2c_set;
                break;
            case I2C_24C02_AllReset:
                i2c_handle->i2c_operate = rhb_i2c_reset;
                break;
            default:
                *len = 0;
                OSSemPost(i2c_handle->sem_I2C);
                return 0;
        }
        
        ulNumOnce = (I2C_24C02_PAGE_SEZE - (ulAddr % I2C_24C02_PAGE_SEZE));
        if(ulNumOnce > 0)                           //  如果写地址没有按页对齐，则写到对齐的地址
        {
            if(ulCount > ulNumOnce) {
              i2c_handle->i2c_operate(I2C_24C02_ADDR, ulAddr, 1, data, &ulNumOnce);
              data += ulNumOnce;
              ulAddr += ulNumOnce;
              ulCount -= ulNumOnce;
              OSTimeDly(2);
            } else {
              i2c_handle->i2c_operate(I2C_24C02_ADDR, ulAddr, 1, data, &ulCount);
              ulAddr += ulNumOnce;
              OSTimeDly(2);
              OSSemPost(i2c_handle->sem_I2C);
              return 1;
            }
        }
        while((ulCount / I2C_24C02_PAGE_SEZE) > 0)      //  按页对齐写数据
        {
            ulNumOnce = I2C_24C02_PAGE_SEZE;
            i2c_handle->i2c_operate(I2C_24C02_ADDR, ulAddr, 1, data, &ulNumOnce);
            data += ulNumOnce;
            ulAddr += I2C_24C02_PAGE_SEZE;
            ulCount -= I2C_24C02_PAGE_SEZE;
            OSTimeDly(2);
        }
        if(ulCount > 0)                             //  写剩下不到一页的数据
        {
            i2c_handle->i2c_operate(I2C_24C02_ADDR, ulAddr, 1, data, &ulCount);
            ulAddr += ulNumOnce;
            OSTimeDly(2);
        }
    }
    OSSemPost(i2c_handle->sem_I2C);
    
    return 1;
}

/*********************************************************************************************************
** Function name:       rhb_24c02_IsBusy
** Descriptions:        I2C总线是否繁忙
** input parameters:    handle: 外设特性描述符
**                      NONE
** output parameters:   NONE
** Returned value:      true: 数据可以发送，正在发送中；反之false: 发送正在进行中，不允许再次发送
注意：因为由信号量的保护，可以认为外设总是不忙的。这是读取时，将会挂起任务，直到有信号量可用。
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_IsBusy(rhb_io_handle handle)
{
    return 1;
}

/*********************************************************************************************************
** Function name:       rhb_24c02_setConfig
** Descriptions:        运行时重新配置串口参数
** input parameters:    handle: 外设特性描述符
**                      data：  获取参数
**                      len：   参数长度
** output parameters:   NONE
** Returned value:      0，该设备不可配置
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_setConfig(rhb_io_handle handle, void *data, unsigned long *len)
{
    
    return 0;
}

/*********************************************************************************************************
** Function name:       rhb_24c02_getConfig
** Descriptions:        运行时重新配置串口参数
** input parameters:    handle: 外设特性描述符
**                      data：  获取参数
**                      len：   参数长度
**                      NONE
** output parameters:   NONE
** Returned value:      0，该设备不可获取配置
*********************************************************************************************************/
rhb_ErrNo rhb_24c02_getConfig(rhb_io_handle handle, void *data, unsigned long *len)
{
    return 0;
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

