/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           globalmacro.h
** Last modified Date:  2018-04-06
** Last Version:        v1.0
** Description:         全局宏定义
**
**--------------------------------------------------------------------------------------------------------
** Created By:          张校源
** Created date:        2018-04-06
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

//定义是否启用自己定义的线程接管物理层
#define RECIVE_SELF                     1
//开启时间同步进程
#define TIME_STAMP                      1
//打印物理层接受的数据
#define PRINT_FRAME                     0
//时间同步节点
#define TIME_SYNCH_NODE                 15
//帧延迟测试打印
#define PACKET_DELAY                    1
//BSM发送制定帧 与发送次数
#define BSM_FREAM_TEST                  1
#define BSM_FRAME_TEST_TIMES            200
//开启或关闭 混杂模式 开启PADID过滤
#define ENABLE_PANID_FILTER             0
//同步之后多少时间开始发送 BSM
#define BSM_START_TIME                  5
#define PACKET_INFO_STATISTICS          1
//发送完成之后定时开启的时间
#define PACKET_INTO_REPORT_TIME         50