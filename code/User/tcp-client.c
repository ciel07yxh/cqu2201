/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           udp-client.c
** Last modified Date:  2014-05-16
** Last Version:        V1.0
** Description:         contiki下主函数的模板
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2014-05-16
** Version:             V1.0
** Descriptions:        The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "contiki-net.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moteid.h"

/*********************************************************************************************************
**  定义调试功能，是否打开串口打印功能
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((u8_t *)addr)[0], ((u8_t *)addr)[1], ((u8_t *)addr)[2], ((u8_t *)addr)[3], ((u8_t *)addr)[4], ((u8_t *)addr)[5], ((u8_t *)addr)[6], ((u8_t *)addr)[7], ((u8_t *)addr)[8], ((u8_t *)addr)[9], ((u8_t *)addr)[10], ((u8_t *)addr)[11], ((u8_t *)addr)[12], ((u8_t *)addr)[13], ((u8_t *)addr)[14], ((u8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF(" %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x ",lladdr.u8[0], lladdr.u8[1], lladdr.u8[2], lladdr.u8[3],lladdr.u8[4], lladdr.u8[5], lladdr.u8[6], lladdr.u8[7])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

/*********************************************************************************************************
**  定义服务器端口号，本地端口号随机
*********************************************************************************************************/
#define SERVER_PORT      8003

/*********************************************************************************************************
**  定义TCP的socket
*********************************************************************************************************/
static struct tcp_socket socket;

/*********************************************************************************************************
**  定义tcp服务器的地址
*********************************************************************************************************/
static uip_ipaddr_t server_ipaddr;

/*********************************************************************************************************
**  定义数据缓冲区
*********************************************************************************************************/
#define INPUTBUFSIZE     400
static uint8_t inputbuf[INPUTBUFSIZE];

#define OUTPUTBUFSIZE    400
static uint8_t outputbuf[OUTPUTBUFSIZE];


/*********************************************************************************************************
** Function name:       SetServerAddress
** Descriptions:        Set the Server address
** input parameters:    none
** output parameters:   none
** Returned value:      none
*********************************************************************************************************/
static void SetServerAddress(void)
{ 
  // 发送到IPv4的主机192.168.123.225
   uip_ip6addr(&server_ipaddr, 0x3ffe, 0, 0, 0, 0, 0, 0xc0a8, 0x7be1);
}

/*********************************************************************************************************
**  接收数据回调函数处理   接收到数据之后就把数据传回去
*********************************************************************************************************/
static int input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
  PRINTF("Reveive a packet with %d bytes!\r\n", inputdatalen);

  tcp_socket_send(&socket, inputptr, inputdatalen);
  
  return 0;
}

/*********************************************************************************************************
**  回调函数，可以显示TCP的连接状态
*********************************************************************************************************/
static void event(struct tcp_socket *s, void *ptr,
      tcp_socket_event_t ev)
{
  if(ev == TCP_SOCKET_CONNECTED) {
    PRINTF("TCP Conetct OK!\r\n");
  } else if(ev ==  TCP_SOCKET_CLOSED) {
    PRINTF("TCP Closed, need connect again!\r\n");
    tcp_socket_connect(&socket, &server_ipaddr, SERVER_PORT);
  }
}

/*********************************************************************************************************
**  TCP线程
*********************************************************************************************************/
PROCESS(tcp_client_process, "TCP client echo process");
PROCESS_THREAD(tcp_client_process, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer et;
  SetServerAddress();
  
  tcp_socket_register(&socket, NULL,
               inputbuf, sizeof(inputbuf),
               outputbuf, sizeof(outputbuf),
               input, event);
  while(tcp_socket_connect(&socket, &server_ipaddr, SERVER_PORT) < 0) {
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  
    
  //tcp_socket_listen(&socket, SERVER_PORT);

  while(1) {
    //PROCESS_PAUSE();
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
