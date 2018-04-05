/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               uartstdio.c
** Last modified Date:      2012-06-30
** Last Version:            v1.0
** Description:             串口控制台函数实现
**
**--------------------------------------------------------------------------------------------------------
** Created By:              任海波
** Created date:            2012-06-30
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
#include <stdarg.h>
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_usart.h"
#include "hal/arch/inc/arch_io_gpio.h"
#include "hal/CortexM3/hal_io_level.h"
#include "hal/arch/inc/arch_io_ints.h"
#include "runtime/uartstdio.h"

#include "dev/serial-line.h"

/*********************************************************************************************************
**  配置
*********************************************************************************************************/
#define UART_STDIO_BASE        USART1_BASE
#define RCC_UART_PERIPHERAL    RCC_PERIPHERAL_USART1
#define STDIO_UART_INT         ARCH_INT_USART1

/*********************************************************************************************************
**  int类型十六进制0-f的对应字符串表
*********************************************************************************************************/
static const char * const __GpcHexTable = "0123456789abcdef";

/*********************************************************************************************************
** Function name:       uart_byte_write
** Descriptions:        向串口发送一个字节的数据
** Input parameters:    c: 发送的数据
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint16 uart_byte_write(uint16 ch)
{
    // 等待有发送空间
    while(!(HWREG32(UART_STDIO_BASE + USART_O_SR) & 0x0080)) ;
    HWREG32(UART_STDIO_BASE + USART_O_DR) = ch;
    //while(!(HWREG(UART_STDIO_BASE + USART_O_SR) & 0x0040));   //  等待发送完成

    return ch;
}

/*********************************************************************************************************
** Function name:       uart_byte_read
** Descriptions:        从串口读取一个字节的数据
** Input parameters:    c: 发送的数据
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint16 uart_byte_read(void)
{
    // 等待收到数据
    while(!(HWREG32(UART_STDIO_BASE + USART_O_SR) & 0x0020));  
    // 读取数据并返回
    return (HWREG32(UART_STDIO_BASE + USART_O_DR) & 0x01FF);
}

/*********************************************************************************************************
** Function name:       uart_stdio_isr
** Descriptions:        中断服务函数
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void uart_stdio_isr(void)
{
   uint8 c;
   
   if(HWREG32(UART_STDIO_BASE + USART_O_SR) & 0x0020)             //  如果是接受中断
   {
      c = (unsigned char)(HWREG32(UART_STDIO_BASE + USART_O_DR) & 0x01FF);
      serial_line_input_byte(c);
     //如果接受到了从笔记本电脑发送的 0x4c 代表需要进入Bootloader 下载固件 （重启进入BootLoader）
       if(0x4c==c){
        hal_cpu_reset();
      }
   }
   
   if(HWREG32(UART_STDIO_BASE + USART_O_SR) & 0x0008)             //  如果是数据过载了
   {
      c = (unsigned char)(HWREG32(UART_STDIO_BASE + USART_O_DR) & 0x01FF);
      serial_line_input_byte(c);
   } 
}

/*********************************************************************************************************
** Function name:       uart_stdio_init
** Descriptions:        串口控制台初始化
** Input parameters:    u32Baud: 初始化的串口波特率
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart_stdio_init(uint32 u32Baud)
{
    uint32 u32Clock;
    
    // 开启外设时钟
    SystemPeripheralEnable(RCC_PERIPHERAL_IOPA);
    SystemPeripheralEnable(RCC_PERIPHERAL_AFIO);
    SystemPeripheralEnable(RCC_UART_PERIPHERAL);
    
    // 设置外设重映射
    //AFIORemapEnable(AFIO_REMAP_USART1_REAMP);
    
    // 配置IO功能为复用功能
    GPIOConfigSet(GPIOA_BASE, GPIO_PIN_9, GPIO_AF_PP | GPIO_SPEED_50MHZ);
    GPIOConfigSet(GPIOA_BASE, GPIO_PIN_10, GPIO_DIR_IN_Floating);

    // 获取外设时钟
    u32Clock = SystemPeripheralClockGet(RCC_UART_PERIPHERAL);
    // 配置串口
    UsartConfigSet(UART_STDIO_BASE,u32Clock,u32Baud,
                       (USART_WLEN_8 | USART_PAR_NONE | USART_STOP_1));
    hal_int_register(STDIO_UART_INT, uart_stdio_isr);
    hal_int_priority_set(STDIO_UART_INT, 0x00);
    hal_int_enable(STDIO_UART_INT);
    UsartIntEnable(UART_STDIO_BASE,USART_INT_RX);
    UsartEnable(UART_STDIO_BASE);
}

/*********************************************************************************************************
** Function name:       uart_write_string
** Descriptions:        向串口发送指定字节的数据
** Input parameters:    pcBuf:  发送数据地址
**                      u32Len: 发送数据长度
** Output parameters:   无
** Returned value:      实际发送的数据长度
*********************************************************************************************************/
int uart_write_string(const char *pcBuf, uint32 u32Len)
{
    int i;

    for(i=0; i<u32Len; i++)
    {
        if(pcBuf[i] == '\n') {
            uart_byte_write('\r');
        }
        uart_byte_write(pcBuf[i]);
    }

    return i;
}

/*********************************************************************************************************
** Function name:       uart_read_string
** Descriptions:        从串口中读取一定的数据
** Input parameters:    pcBuf:  读取数据地址
**                      u32Len: 读取数据长度
** Output parameters:   无
** Returned value:      实际读取的数据长度
*********************************************************************************************************/
int uart_read_string(char *pcBuf, uint32 u32Len)
{
    uint32 u32Count;
    char ch;
    static char bLastWasCR = 0;

    u32Len--;

    // 处理字符，直到新的一行数据收到
    while(1)
    {
        // 读取一个数据
        ch = (char)uart_byte_read();

        // 如果按键是Backspace
        if(ch == '\b')
        {
            // 如果buffer有数据，删除最后一个数据
            if(u32Count)
            {
                // 发送之前的数据
                uart_write_string("\b \b", 3);

                // 减少缓冲区的数据个数
                u32Count--;
            }

            // 继续读下一个数据
            continue;
        }

        // 如果字符是LF，上一个字符是CR，则处理EQL
        if((ch == '\n') && bLastWasCR)
        {
            bLastWasCR = 0;
            continue;
        }

        // 是否要求回车换行
        if((ch == '\r') || (ch == '\n') || (ch == 0x1b))
        {
            // 回车换行
            if(ch == '\r')
            {
                bLastWasCR = 1;
            }

            // 结束处理，换行
            break;
        }

        // 处理收到的字符
        if(u32Count < u32Len)
        {
            // 存储收到的字符
            pcBuf[u32Count] = ch;

            // 增加收到的字符个数计数
            u32Count++;

            // 回显字符串
            uart_byte_write(ch);
        }
    }

    // 添加'\0'的字符串结束标志
    pcBuf[u32Count] = 0;

    // 发送一个回车换行
    uart_write_string("\r\n", 2);

    // 返回buffer中的字符个数，不包括'\0'
    return(u32Count);
}

/*********************************************************************************************************
** Function name:       uart_getc
** Descriptions:        从串口中读取一个字节的数据，本函数将阻塞CPU，直到收到数据
** Input parameters:    无
** Output parameters:   无
** Returned value:      读取到的数据
*********************************************************************************************************/
unsigned char uart_getc(void)
{
    // 从串口读取一个字符
    return(uart_byte_read());
}

/*********************************************************************************************************
** Function name:       uart_putc
** Descriptions:        从串口中发送一个字节的数据
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart_putc(unsigned char ch)
{
    uart_byte_write(ch);
}

/*********************************************************************************************************
** Function name:       uart_printf
** Descriptions:        通过串口实现printf函数，目前支持\%c, \%d, \%p, \%s, \%u, \%x, and \%X.
**                      \%c to print a character
**                      \%d to print a decimal value
**                      \%s to print a string
**                      \%u to print an unsigned decimal value
**                      \%x to print a hexadecimal value using lower case letters
**                      \%X to print a hexadecimal value using lower case letters (not upper case
**                      letters as would typically be used)
**                      \%p to print a pointer as a hexadecimal value
**                      \%\% to print out a \% character
** Input parameters:    和标准IO函数printf相同
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uart_printf(const char *pcString, ...)
{
    uint32 u32Idx, u32Value, u32Pos,u32Count,u32Base,u32Neg;
    char *pcStr, pcBuf[16], cFill;
    va_list vaArgP;

    // 开始参数处理
    va_start(vaArgP, pcString);

    // 循环直到字符串结束
    while(*pcString)
    {
        // 寻找第一个'%'的位置或者字符串结束'\0'的位置
        for(u32Idx = 0; (pcString[u32Idx] != '%') && (pcString[u32Idx] != '\0');
            u32Idx++)
        {
        }

        // 发送%之前的字符
        uart_write_string(pcString, u32Idx);

        // 字符指针后移
        pcString += u32Idx;

        // 如果下一个字符是'%'
        if(*pcString == '%')
        {
            // 跳过%
            pcString++;

            // 将计数值设置为0，默认已空格填充
            u32Count = 0;
            cFill = ' ';

            // 循环处理更多的字符，这里使用goto实现，虽然不建议，但是很高效
uart_printf_again:

            // 决定如何处理下一个字符
            switch(*pcString++)
            {
                // 处理数字
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    // 如果第一个数字是0，则替换空格，用0填充
                    if((pcString[-1] == '0') && (u32Count == 0))
                    {
                        cFill = '0';
                    }

                    // 计数值加10
                    u32Count *= 10;
                    u32Count += pcString[-1] - '0';

                    // 获取下一个字符
                    goto uart_printf_again;
                }

                // 处理%c命令
                case 'c':
                {
                    // 读取参数
                    //u32Value = va_arg(vaArgP, uint32);
                    u32Value = (uint32)va_arg(vaArgP, int);

                    // 发送字符
                    uart_write_string((char *)&u32Value, 1);

                    break;
                }

                // 处理%d命令
                case 'd':
                {
                    // 读取参数
                    //u32Value = va_arg(vaArgP, uint32);
                    u32Value = (uint32)va_arg(vaArgP, int);

                    // 复位buffer位置指针
                    u32Pos = 0;

                    // 如果值是负数，变成整数，并且加一个负号标志
                    if((int32)u32Value < 0)
                    {
                        // 取相反数
                        u32Value = -(int32)u32Value;

                        // 标志负号
                        u32Neg = 1;
                    } else {
                        // 标志正数
                        u32Neg = 0;
                    }

                    // 设置10进制
                    u32Base = 10;

                    // 转换为ASCII.
                    goto uart_printf_convert;
                }

                // 处理%s命令
                case 's':
                {
                    // 获取字符串参数指针
                    pcStr = va_arg(vaArgP, char *);

                    // 判断字符串长度
                    for(u32Idx = 0; pcStr[u32Idx] != '\0'; u32Idx++)
                    {
                    }

                    // 发送字符串
                    uart_write_string(pcStr, u32Idx);

                    // 写需要的空格
                    if(u32Count > u32Idx)
                    {
                        u32Count -= u32Idx;
                        while(u32Count--)
                        {
                            uart_write_string(" ", 1);
                        }
                    }

                    break;
                }

                //
                // Handle the %u command.
                // 处理%u
                case 'u':
                {
                    // 获取参数
                    //u32Value = va_arg(vaArgP, uint32);
                    u32Value = (uint32)va_arg(vaArgP, int);

                    // 复位buffer位置指针
                    u32Pos = 0;

                    // 设置十进制
                    u32Base = 10;

                    // 标志是正数
                    u32Neg = 0;

                    // 转成为ASCII
                    goto uart_printf_convert;
                }

                // 处理%x、%X、%p, 暂时%p和%x做相同的处理.
                case 'x':
                case 'X':
                case 'p':
                {
                    // 获取参数
                   // u32Value = va_arg(vaArgP, uint32);
                    u32Value = va_arg(vaArgP, int);

                    // 复位buffer位置指针
                    u32Pos = 0;

                    // 设置十六进制
                    u32Base = 16;

                    // 指示正数
                    u32Neg = 0;

                    // 转换ASCII实现
uart_printf_convert:
                    for(u32Idx = 1;
                        (((u32Idx * u32Base) <= u32Value) &&
                         (((u32Idx * u32Base) / u32Base) == u32Idx));
                        u32Idx *= u32Base, u32Count--)
                    {
                    }

                    // 如果是负数，减少字符计数
                    if(u32Neg)
                    {
                        u32Count--;
                    }

                    // 如果是负数，并且用0填充
                    if(u32Neg && (cFill == '0'))
                    {
                        // 填充缺少的符号位
                        pcBuf[u32Pos++] = '-';

                        // 填充完符号位，关闭符号标志
                        u32Neg = 0;
                    }

                    // 如果需要，增加额外的填充
                    if((u32Count > 1) && (u32Count < 16))
                    {
                        for(u32Count--; u32Count; u32Count--)
                        {
                            pcBuf[u32Pos++] = cFill;
                        }
                    }

                    // 如果是负数在，在数值前加一个负号
                    if(u32Neg)
                    {
                        // 添加符号
                        pcBuf[u32Pos++] = '-';
                    }

                    // 将数值转换为string
                    for(; u32Idx; u32Idx /= u32Base)
                    {
                        pcBuf[u32Pos++] = __GpcHexTable[(u32Value / u32Idx) % u32Base];
                    }

                    // 发送string
                    uart_write_string(pcBuf, u32Pos);

                    break;
                }

                // 处理%%
                case '%':
                {
                    // 简单地发送一个string
                    uart_write_string(pcString - 1, 1);
                    break;
                }

                // 处理器其它命令
                default:
                {
                    // 指示错误提示
                    uart_write_string("ERROR", 5);
                    break;
                }
            }
        }
    }

    // 参数处理结束
    va_end(vaArgP);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
