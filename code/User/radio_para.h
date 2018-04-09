/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               radio_para.h
** Last modified Date:      2018-04-07
** Last Version:            v1.0
** Description:             根据节点号初始化物理层参数
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              张校源
** Created date:            2018-04-07
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
#ifndef __RADIO_PARA_H__ 
#define __RADIO_PARA_H__ 

#include "includes.h"
#include "moteid.h"

/*********************************************************************************************************
  设备类型相关定义
*********************************************************************************************************/
typedef struct radio_para{
  uint16_t pan_id;              
  uint16_t shortaddr;
  uint8_t max_frame_retrise;             /*0x00 - 0xFF*/
  uint8_t max_csma_retries;             /*0x0-0x5 csma_ca repet time, 0x01代表只传输一次
                                          0x07代表不做csma_ca，直接发送
                                          0x06 无意义*/
  uint8_t tx_power;                     /*0x0-0xF*/
  uint8_t channel;                      /* 取值范围为  11-26
                                          15, 16, 21, 22是与WIFI不干扰的信道 */
  uint8_t panid_filter;                 //是否开启混杂模式       
  uint8_t seq;                          //帧系列号
  
  //统计信息
  uint16_t transmit_times;              //发送次数
  uint16_t badcrc_times;                
  uint16_t transmit_failed_times;
  uint16_t channel_access_failed_times;
  uint16_t no_ack_times;
  uint16_t transmit_success_times;
}radio_para;


    /*
    ** Step 10, 发送功率设置
    **  AT86RF231                      AT86RF233
    **  0x0 --------->3.0db            0x0 --------->4.0db
    **  0x1 --------->2.8db            0x1 --------->3.7db
    **  0x2 --------->2.3db            0x2 --------->3.4db
    **  0x3 --------->1.8db            0x3 --------->3.0db
    **  0x4 --------->1.3db            0x4 --------->2.5db
    **  0x5 --------->0.7db            0x5 --------->2.0db
    **  0x6 --------->0.0db            0x6 --------->1.0db
    **  0x7 --------->-1db             0x7 --------->0.0db
    **  0x8 --------->-2db             0x8 --------->-1db
    **  0x9 --------->-3db             0x9 --------->-2db
    **  0xA --------->-4db             0xA --------->-3db
    **  0xB --------->-5db             0xB --------->-4db
    **  0xC --------->-7db             0xC --------->-6db
    **  0xD --------->-9db             0xD --------->-8db
    **  0xE --------->-12db            0xE --------->-12db
    **  0xF --------->-17db            0xF --------->-17db
    */


//默认参数
#define DEFAULT_MAX_FRAME_RETRIES       0x00
#define DEFAULT_MAX_CSMA_RETRIES        0x03
#define DEFAULT_TX_POWER                0xD
#define DEFAULT_CHANNEL                 16
#define DEEFAULT_COODER                 0x00

#define NO_CSMA_CA_PERFORM              0x07
/*********************************************************************************************************
  外部函数及变量定义
*********************************************************************************************************/
extern radio_para radiopara;
void radio_para_init();
void report_radio_statistics(void);


#endif
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
