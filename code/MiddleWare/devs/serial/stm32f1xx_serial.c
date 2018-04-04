/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           stm32f1xx_serial.c
** Last modified Date:  2013-08-28
** Last Version:        v1.0
** Description:         基于contiki的stm32的串口驱动程序。发送和接受都基于中断方式
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-08-28
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
/*********************************************************************************************************
  头文件包含
*********************************************************************************************************/
#include "devs/serial/stm32f1xx_serial.h"


/*********************************************************************************************************
  局部静态声明
*********************************************************************************************************/
// 串口1
stm32_serial_info tUsart1Info  =  {
    USART1_BASE,            // 模块基地址
    RCC_PERIPHERAL_USART1,  //  外设总线属性，用于确定外设时钟
    ARCH_INT_USART1,        // 中断序号
    0,                      // 串口是否处于发送状态，0，没有正在发送；1，正在发送
    {0},                    // 发送缓冲区，会用在contiki的环形缓冲区中
    19200,                  // 默认波特率
    {0},                    // 发送环形缓冲区
    NULL                    // 默认数据输入函数
};

// 串口2
stm32_serial_info tUsart2Info  =  {
    USART2_BASE,            // 模块基地址
    RCC_PERIPHERAL_USART2,  //  外设总线属性，用于确定外设时钟
    ARCH_INT_USART2,        // 中断序号
    0,                      // 串口是否处于发送状态，0，没有正在发送；1，正在发送
    {0},                    // 发送缓冲区，会用在contiki的环形缓冲区中
    19200,                  // 默认波特率
    {0},                    // 发送环形缓冲区
    NULL                    // 默认数据输入函数
};
// 串口3
stm32_serial_info tUsart3Info  =  {
    USART3_BASE,            // 模块基地址
    RCC_PERIPHERAL_USART3,  //  外设总线属性，用于确定外设时钟
    ARCH_INT_USART3,        // 中断序号
    0,                      // 串口是否处于发送状态，0，没有正在发送；1，正在发送
    {0},                    // 发送缓冲区，会用在contiki的环形缓冲区中
    19200,                  // 默认波特率
    {0},                    // 发送环形缓冲区
    NULL                    // 默认数据输入函数
};

/*********************************************************************************************************
** Function name:       serial_set_input
** Descriptions:        设置串口输入处理函数
** input parameters:    handle: 外设特性描述符
**                      input:  指定处理函数
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void serial_set_input(void *handle, int (*input)(unsigned char c))
{
    stm32_serial_info *serial_handle = handle;
    
    serial_handle->serial_input_handler = input;
}

/*********************************************************************************************************
** Function name:       serial_writeb
** Descriptions:        串口发送数据，发送一个字节的数据
** input parameters:    handle: 外设特性描述符
**                      c:      待发送的数据
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void serial_writeb(void *handle, unsigned char c)
{
    stm32_serial_info *serial_handle = handle;
    //watchdog_periodic();
    
    //　将数据放入发送缓冲区
    while(ringbuf_put(&serial_handle->txbuf, c) == 0);

    if(0 == serial_handle->transmitting) {
        serial_handle->transmitting = 1;
        while(!(UsartSpaceAvial(serial_handle->base)));
        UsartCharPut(serial_handle->base, ringbuf_get(&serial_handle->txbuf));
        UsartIntEnable(serial_handle->base,USART_INT_TX_EMPTY);
    }
}

/*********************************************************************************************************
** Function name:       serial_init
** Descriptions:        串口初始化
** input parameters:    handle: 外设特性描述符
**                      u32Buad: 串口波特率
**                      pfIsrHandler: 中断服务函数
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void serial_init(void *handle, uint32 u32Buad, void(*pfIsrHandler)(void))
{
    uint32 u32Clock;
    stm32_serial_info *serial_handle = handle;
    
    // 开启外设时钟
    SystemPeripheralEnable(serial_handle->u8WhichBus, serial_handle->u32Ctrl);
    // 获取外设时钟
    u32Clock = SystemPeripheralClockGet(serial_handle->u8WhichBus, serial_handle->u32Ctrl);

    serial_handle->transmitting = 0;
    ringbuf_init(&serial_handle->txbuf, serial_handle->sndBuf, UART_SEND_BUFFER_SIZE); 

    UsartConfigSet(serial_handle->base,u32Clock,u32Buad,
                       (USART_WLEN_8 | USART_PAR_NONE | USART_STOP_1));
   
    serial_handle->Baud = u32Buad;
    UsartIntEnable(serial_handle->base,USART_INT_RX);
    hal_int_register(serial_handle->int_num, pfIsrHandler);
    hal_int_priority_set(serial_handle->int_num, 0x00);
    hal_int_enable(serial_handle->int_num);
    UsartEnable(serial_handle->base);
}

/*********************************************************************************************************
** Function name:       stm32_serial_ISR
** Descriptions:        串口中断处理函数，该函数需要再中断里调用
** input parameters:    handle
**                      NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void stm32_serial_ISR(void *handle)
{
   unsigned char c;
   stm32_serial_info *serial_handle = handle;
   
   if(!(HWREG32(serial_handle->base + USART_O_CR1) & USART_INT_TX_EMPTY)) // 如果发送空中断未使能
   {
     goto STM32_USART_RECV_HANDLER;
   }
   if(HWREG32(serial_handle->base + USART_O_SR) & 0x0080)                 //  如果是发送中断
   {
        if(1 == serial_handle->transmitting) {
            if(0 == ringbuf_elements(&serial_handle->txbuf)) {
                UsartIntDisable(serial_handle->base,USART_INT_TX_EMPTY);
                while(!(HWREG32(serial_handle->base + USART_O_SR) & 0x0040)); //  等待发送完成
                serial_handle->transmitting = 0;
            } else {
                HWREG32(serial_handle->base + USART_O_DR) = ringbuf_get(&serial_handle->txbuf);
            }
        } else {
            UsartIntDisable(serial_handle->base,USART_INT_TX_EMPTY);
        }
    }

   
STM32_USART_RECV_HANDLER:
    if(HWREG32(serial_handle->base + USART_O_SR) & 0x0020)            //  如果是接受中断
    {
        c = (unsigned char)(HWREG32(serial_handle->base + USART_O_DR) & 0x01FF);
        if(NULL != serial_handle->serial_input_handler) {
            serial_handle->serial_input_handler(c);
        }
    }
   
   if(HWREG32(serial_handle->base + USART_O_SR) & 0x0008)             //  如果是数据过载了
   {
        c = (unsigned char)(HWREG32(serial_handle->base + USART_O_DR) & 0x01FF);
        if(NULL != serial_handle->serial_input_handler) {
            serial_handle->serial_input_handler(c);
        }
   } 
}

// 串口的中断服务函数，需要注册在中断向量表中
void Usart1_ISR(void)
{
   stm32_serial_ISR(&tUsart1Info);
}

void Usart2_ISR(void)
{
   stm32_serial_ISR(&tUsart2Info);
}

void Usart3_ISR(void)
{
   stm32_serial_ISR(&tUsart3Info);
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
