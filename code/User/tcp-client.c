/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           udp-client.c
** Last modified Date:  2014-05-16
** Last Version:        V1.0
** Description:         contiki����������ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created By:          �κ���
** Created date:        2014-05-16
** Version:             V1.0
** Descriptions:        The original version ��ʼ�汾
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
**  ������Թ��ܣ��Ƿ�򿪴��ڴ�ӡ����
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
**  ����������˿ںţ����ض˿ں����
*********************************************************************************************************/
#define SERVER_PORT      8003

/*********************************************************************************************************
**  ����TCP��socket
*********************************************************************************************************/
static struct tcp_socket socket;

/*********************************************************************************************************
**  ����tcp�������ĵ�ַ
*********************************************************************************************************/
static uip_ipaddr_t server_ipaddr;

/*********************************************************************************************************
**  �������ݻ�����
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
  // ���͵�IPv4������192.168.123.225
   uip_ip6addr(&server_ipaddr, 0x3ffe, 0, 0, 0, 0, 0, 0xc0a8, 0x7be1);
}

/*********************************************************************************************************
**  �������ݻص���������   ���յ�����֮��Ͱ����ݴ���ȥ
*********************************************************************************************************/
static int input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
  PRINTF("Reveive a packet with %d bytes!\r\n", inputdatalen);

  tcp_socket_send(&socket, inputptr, inputdatalen);
  
  return 0;
}

/*********************************************************************************************************
**  �ص�������������ʾTCP������״̬
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
**  TCP�߳�
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
