/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           Main.c
** Last modified Date:  2012-06-21
** Last Version:        v1.0
** Description:         contiki下主函数的模板
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2012-06-21
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
#include "includes.h"

/*********************************************************************************************************
**  全局变量定义
*********************************************************************************************************/
#define DEBUG 1
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*********************************************************************************************************
**  外部函数声明
*********************************************************************************************************/
PROCESS_NAME(tcp_client_process);

/*********************************************************************************************************
**  内部函数声明
*********************************************************************************************************/
PROCESS(led_process, "Blink1");
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();
  sys_led_toggle(1);

  while(1)
  {
    static struct etimer et;

    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    sys_led_toggle(0);
    sys_led_toggle(1);
   // PRINTF("The LED is toggle!\r\n");
  }
   PROCESS_END();
}

/*********************************************************************************************************
**  自启动的线程
*********************************************************************************************************/
AUTOSTART_PROCESSES(&led_process);

/*********************************************************************************************************
**  操作系统需要的定义
*********************************************************************************************************/
PROCINIT(NULL);


/*********************************************************************************************************
** Function name:       main
** Descriptions:        主函数的模板，一般不要随意更改
** input parameters:    无
** output parameters:   无
** Returned value:      0
** Created by:          任海波
** Created Date:        2012-06-21
*********************************************************************************************************/
int main (void)
{
    // 初始化系统服务
    sys_services_init();

    // 初始化总线驱动
    __GtSpiBusDriver.spi_bus_init(&__GtSpiBusDriver);

    // 初始化控制台
    uart_stdio_init(115200);


    PRINTF("The Contiki System Start!\r\n");

    clock_init();
    rtimer_init();
    process_init();

    random_init(0);

    procinit_init();
    process_start(&etimer_process, NULL);
    ctimer_init();
    // 初始化网络协议栈
    moteid_init();
    contiki_net_init();

    energest_init();
    ENERGEST_ON(ENERGEST_TYPE_CPU);

    autostart_start(autostart_processes);
   // watchdog_init();
   // watchdog_start();
    process_start(&tcp_client_process, NULL);
    for(;;)
    {
        do
        {
            // 喂狗
            // watchdog_periodic();
        } while(process_run() > 0);
      // contiki_net_handle();
        //contiki_power_manager();

        // Stop processor clock
         //asm("wfi"::);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
