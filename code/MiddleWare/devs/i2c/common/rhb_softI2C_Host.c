/****************************************Copyright (c)****************************************************
**
**
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               rhb_softI2C_Host.c
** Last modified Date:      2010-8-05
** Last Version:            v1.0
** Description:             软件模拟I2C主机的实现软件包
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Renhaibo
** Created date:            2010-8-05
** Version:                 v1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "rhb_softI2C_Host.h"
#include "target.h"

#if (rhb_I2C_Delay > 0)
#define I2C_Delay()      usdelay(rhb_I2C_Delay)
#else
#define I2C_Delay()
#endif

unsigned long stm32_SDA_Read(void)       
{
   unsigned long ulTemp;
   HWREG(GPIOC_BASE + GPIO_O_CRH) &= 0x0FFFFFFF;
   HWREG(GPIOC_BASE + GPIO_O_CRH) |= 0x40000000;
   ulTemp = (HWREG(GPIOC_BASE + GPIO_O_IDR) & 0x8000);     //  读SDA线的状态
   HWREG(GPIOC_BASE + GPIO_O_CRH) &= 0x0FFFFFFF;
   HWREG(GPIOC_BASE + GPIO_O_CRH) |= 0x70000000;
   
   return ulTemp;  
}
#define rhb_SDA_Read() stm32_SDA_Read()
/*********************************************************************************************************
** Function name:       rhb_I2C_Start
** Descriptions:        IO模拟I2C产生起始条件
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      true:起始条件正确产生；fasle：总线忙。
*********************************************************************************************************/
void rhb_I2C_Start(void)
{
    rhb_SDA_H();
    rhb_SCL_H();
    I2C_Delay();
    rhb_SDA_L();
    I2C_Delay();
    rhb_SCL_L();
    I2C_Delay();
}

/*********************************************************************************************************
** Function name:       rhb_I2C_Stop
** Descriptions:        IO模拟I2C产生停止条件
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_I2C_Stop(void)
{
    rhb_SCL_L();
    rhb_SDA_L();
    I2C_Delay();
    rhb_SCL_H();
    I2C_Delay();
    rhb_SDA_H();
    I2C_Delay();
}
/*********************************************************************************************************
** Function name:       rhb_I2C_Ack
** Descriptions:        IO模拟I2C产生Ack信号
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_I2C_Ack(void)
{ 
    rhb_SCL_L();
    I2C_Delay();
    rhb_SDA_L();
    I2C_Delay();
    rhb_SCL_H();
    I2C_Delay();
    rhb_SCL_L();
    I2C_Delay();
}

/*********************************************************************************************************
** Function name:       rhb_I2C_NoAck
** Descriptions:        IO模拟I2C产生NOAck信号
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_I2C_NoAck(void)
{ 
    rhb_SCL_L();
    I2C_Delay();
    rhb_SDA_H();
    I2C_Delay();
    rhb_SCL_H();
    I2C_Delay();
    rhb_SCL_L();
    I2C_Delay();
}

/*********************************************************************************************************
** Function name:       rhb_I2C_WaitAck
** Descriptions:        IO模拟I2C产生WaitAck信号
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      true:有Ack信号；false:无Ack信号
*********************************************************************************************************/
tBoolean rhb_I2C_WaitAck(void)
{
    rhb_SCL_L();
    I2C_Delay();
    rhb_SDA_H();
    I2C_Delay();
    rhb_SCL_H();
    I2C_Delay();
    if(rhb_SDA_Read())
    {
      rhb_SCL_L();
      return false;
    }
    rhb_SCL_L();
    return true;
}

/*********************************************************************************************************
** Function name:       rhb_I2C_WaitAck
** Descriptions:        IO模拟I2C发送一个字节
** input parameters:    ucByte: 发送的数据，数据先发送MSB，最后发送LSB
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_I2C_SendByte(unsigned char ucByte)
{
    unsigned char i=8;
    while(i--)
    {
        rhb_SCL_L();
        I2C_Delay();
        if(ucByte&0x80)
            rhb_SDA_H(); 
        else 
            rhb_SDA_L();   
        ucByte<<=1;
        I2C_Delay();
        rhb_SCL_H();
        I2C_Delay();
    }
    rhb_SCL_L();
}

/*********************************************************************************************************
** Function name:       rhb_I2C_RecvByte
** Descriptions:        IO模拟I2C接收一个字节的数据
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      接收到的数据，数据从高位到低位接收。
*********************************************************************************************************/
unsigned char rhb_I2C_RecvByte(void)
{ 
    unsigned char i=8;
    unsigned char ucByte=0;

    rhb_SDA_H();
    while(i--)
    {
      ucByte <<= 1;
      rhb_SCL_L();
      I2C_Delay();
      rhb_SCL_H();
      I2C_Delay(); 
      if(rhb_SDA_Read())
      {
        ucByte|=0x01;
      }
    }
    rhb_SCL_L();
    return ucByte;
}

/*********************************************************************************************************
** Function name:       rhb_i2c_read
** Descriptions:        IO主机从从机读取一定字节的数据
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_i2c_read(unsigned char I2CAddr, unsigned long ulAddr, 
                  unsigned char addrLen, void *buf, unsigned long *len)
{
    unsigned char i = 0;
    unsigned char *data = buf;
    unsigned long ulCount = *len;
    
    rhb_I2C_Start();                                //  等待产生停止条件
    rhb_I2C_SendByte(I2CAddr | rhb_I2C_WRITE);      //  发送器件地址
    for(i=0; i< addrLen; i++)
    {
        rhb_I2C_Ack();
        rhb_I2C_SendByte((char)(ulAddr>>((addrLen-i-1)*8)));    //  发送读取存储器单元地址
    }
    rhb_I2C_Ack();
    rhb_I2C_Start();                                //  等待产生停止条件
    rhb_I2C_SendByte(I2CAddr | rhb_I2C_READ);       //  发送器件地址，并标志读
    while(ulCount--){
        rhb_I2C_Ack();                              //  开始读取数据，并放入缓存
        *data++ = rhb_I2C_RecvByte();
    }
    rhb_I2C_NoAck();
    rhb_I2C_Stop();
}

/*********************************************************************************************************
** Function name:       rhb_i2c_write
** Descriptions:        IO主机向从机写入一定字节的数据
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_i2c_write(unsigned char I2CAddr, unsigned long ulAddr, 
                   unsigned char addrLen, void *buf, unsigned long *len)
{
    unsigned char i = 0;
    unsigned char *data = buf;
    unsigned long ulCount = *len;
    
    rhb_I2C_Start();                            //  等待产生停止条件
    rhb_I2C_SendByte(I2CAddr | rhb_I2C_WRITE);  //  发送器件地址
    for(i=0; i< addrLen; i++)
    {
        rhb_I2C_Ack();
        rhb_I2C_SendByte((char)(ulAddr>>((addrLen-i-1)*8)));    //  发送读取存储器单元地址
    }
    while(ulCount--){
        rhb_I2C_Ack();                          //  开始读取数据，并放入缓存
        rhb_I2C_SendByte(*data++);             //  写入一个字节的数据
    }
    rhb_I2C_Ack();
    rhb_I2C_Stop();
}

/*********************************************************************************************************
** Function name:       rhb_i2c_set
** Descriptions:        I2C指定地址的数据设置为0xFF
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_i2c_set(unsigned char I2CAddr, unsigned long ulAddr, 
                   unsigned char addrLen, void *buf,unsigned long *len)
{
    unsigned char i = 0;
    unsigned long ulCount = *len;
    
    rhb_I2C_Start();                            //  等待产生停止条件
    rhb_I2C_SendByte(I2CAddr | rhb_I2C_WRITE);  //  发送器件地址
    for(i=0; i< addrLen; i++)
    {
        rhb_I2C_Ack();
        rhb_I2C_SendByte((char)(ulAddr>>((addrLen-i-1)*8)));    //  发送读取存储器单元地址
    }
    while(ulCount--){
        rhb_I2C_Ack();                          //  开始读取数据，并放入缓存
        rhb_I2C_SendByte(0xFF);                //  写入一个字节的数据
    }
    rhb_I2C_Ack();
    rhb_I2C_Stop();
}

/*********************************************************************************************************
** Function name:       rhb_i2c_set
** Descriptions:        I2C指定地址的数据设置为0x00
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void rhb_i2c_reset(unsigned char I2CAddr, unsigned long ulAddr, 
                   unsigned char addrLen, void *buf,unsigned long *len)
{
    unsigned char i = 0;
    unsigned long ulCount = *len;
    
    rhb_I2C_Start();                            //  等待产生停止条件
    rhb_I2C_SendByte(I2CAddr | rhb_I2C_WRITE);  //  发送器件地址
    for(i=0; i< addrLen; i++)
    {
        rhb_I2C_Ack();
        rhb_I2C_SendByte((char)(ulAddr>>((addrLen-i-1)*8)));    //  发送读取存储器单元地址
    }
    while(ulCount--){
        rhb_I2C_Ack();                          //  开始读取数据，并放入缓存
        rhb_I2C_SendByte(0x00);                 //  写入一个字节的数据
    }
    rhb_I2C_Ack();
    rhb_I2C_Stop();
}

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
