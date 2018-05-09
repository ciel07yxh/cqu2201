/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           Main.c
** Last modified Date:  2012-06-21
** Last Version:        v1.0
** Description:         contiki����������ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created By:          �κ���
** Created date:        2012-06-21
** Version:             v1.0
** Descriptions:        The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "includes.h"
#include "P2P.h"
/*********************************************************************************************************
**  ȫ�ֱ�������
*********************************************************************************************************/
#define DEBUG 1
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*********************************************************************************************************
**  �ⲿ��������
*********************************************************************************************************/
PROCESS_NAME(udp_client_process);

/*********************************************************************************************************
**  �ڲ���������
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
    //PRINTF("The moteid is %d \r\n",get_moteid());
  }
   PROCESS_END();
}

/*********************************************************************************************************
**  ���������߳�
*********************************************************************************************************/
AUTOSTART_PROCESSES(&led_process);

/*********************************************************************************************************
**  ����ϵͳ��Ҫ�Ķ���
*********************************************************************************************************/
PROCINIT(NULL);


/*********************************************************************************************************
** Function name:       main
** Descriptions:        ��������ģ�壬һ�㲻Ҫ�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0
** Created by:          �κ���
** Created Date:        2012-06-21
*********************************************************************************************************/
int main (void)
{
    // ��ʼ��ϵͳ����
    sys_services_init();

    // ��ʼ����������
    __GtSpiBusDriver.spi_bus_init(&__GtSpiBusDriver);

    // ��ʼ������̨
    uart_stdio_init(115200);
    moteid_init();

    PRINTF("The Contiki System Start!\r\n");

    clock_init();
    rtimer_init();
    process_init();

    random_init(0);

    procinit_init();
    process_start(&etimer_process, NULL);
    ctimer_init();
    /****************�Զ��x��ʼ��************/
    // ��ʼ������Э��ջ
    //contiki_net_init();
    //ֻ��ʼ�������
      NETSTACK_RADIO.init();
      NETSTACK_RADIO.on();
      //��ʼ��moteid
      moteid_init();
      /*
               PRINTF("CCC");                           //ΪʲôCDû�д�ӡ������
      if(get_moteid() == SRC_ADDR){
               PRINTF("DDD");
      p2p_frame_send(NULL);
      }
      */
     static uint8_t times = TIME_SYNCH_TIMES;

     if(get_moteid()==TIME_SYNCH_NODE)
       time_synch_gps((void *)&times);      
      
     /*****************************************/    
        
    energest_init();
    ENERGEST_ON(ENERGEST_TYPE_CPU);

    autostart_start(autostart_processes);
   // watchdog_init();
   // watchdog_start();
    process_start(&udp_client_process, NULL);
    for(;;)
    {
        do
        {
            // ι��
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
