/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           sys_services.c
** Last modified Date:  2012-11-14
** Last Version:        v1.0
** Description:         系统服务函数实现头文件
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2012-11-14
** Version:             v1.0
** Descriptions:        初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/

/*********************************************************************************************************
  需要的头文件包含
*********************************************************************************************************/
#include "infra/rhb_type.h"
#include "hal/CortexM3/hal_io_level.h"

#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_gpio.h"
/*********************************************************************************************************
** Function name:           sys_clock_get
** Descriptions:            获取系统主频
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
uint32 sys_clock_get(void)
{
   return SystemSysClockGet();
}

/*********************************************************************************************************
** Function name:       sys_beep_init
** Descriptions:        beep蜂鸣器初始化
** Input parameters:    None 无
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
static void sys_beep_init(void)
{

}

/*********************************************************************************************************
** Function name:       sys_beep_up
** Descriptions:        beep蜂鸣器初始化
** Input parameters:    u32time: 鸣叫时间，以10ms为单位
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
void sys_beep_up(uint32 u32time)
{
  
}

/*********************************************************************************************************
** Function name:       sys_led_init
** Descriptions:        led灯控制初始化
** Input parameters:    None 无
** Output parameters:   None 无
** Returned value:      None 无
** 硬件连接： LED0 ---->PB10
**            LED1 ---->PB11
*********************************************************************************************************/
static void sys_led_init(void)
{
    SystemPeripheralEnable(RCC_PERIPHERAL_IOPB);
    SystemPeripheralEnable(RCC_PERIPHERAL_IOPD);
    GPIOConfigSet(GPIOB_BASE, GPIO_PIN_5, GPIO_OUT_PP | GPIO_SPEED_50MHZ);
    GPIOConfigSet(GPIOD_BASE, GPIO_PIN_2, GPIO_OUT_PP | GPIO_SPEED_50MHZ);
    GPIOBitSet(GPIOB_BASE,GPIO_PIN_5);
    GPIOBitSet(GPIOD_BASE,GPIO_PIN_2);
}

/*********************************************************************************************************
** Function name:       sys_led_on
** Descriptions:        点亮指定的LED
** Input parameters:    led: 指定的led灯
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
void sys_led_on(uint8 led)
{
    switch(led) {
        case 0:
            GPIOBitReset(GPIOB_BASE,GPIO_PIN_5);
            break;
        case 1:
            GPIOBitReset(GPIOD_BASE,GPIO_PIN_2);
            break;
        default:
            break;
    }
}

/*********************************************************************************************************
** Function name:       sys_led_off
** Descriptions:        熄灭指定的LED
** Input parameters:    led: 指定的led灯
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
void sys_led_off(uint8 led)
{
    switch(led) {
        case 0:
            GPIOBitSet(GPIOB_BASE,GPIO_PIN_5);
            break;
        case 1:
            GPIOBitSet(GPIOD_BASE,GPIO_PIN_2);
            break;
        default:
            break;
    }
}

/*********************************************************************************************************
** Function name:       sys_led_toggle
** Descriptions:        翻转指定的LED
** Input parameters:    led: 指定的led灯
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
void sys_led_toggle(uint8 led)
{
    switch(led) {
        case 0:
            GPIOToggleBit(GPIOB_BASE, GPIO_PIN_5);
            break;
        case 1:
            GPIOToggleBit(GPIOD_BASE, GPIO_PIN_2);
            break;
        default:
            break;
    }
}

/*********************************************************************************************************
** Function name:       sys_services_init
** Descriptions:        系统服务初始化，主要完成系统服务的初始化
** Input parameters:    None 无
** Output parameters:   None 无
** Returned value:      None 无
*********************************************************************************************************/
void sys_services_init(void)
{
    // 蜂鸣器初始化
    sys_beep_init();
    // LED灯控制初始化
    sys_led_init();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
