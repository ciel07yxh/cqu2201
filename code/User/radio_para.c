/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���                         
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               radio_para.c
** Last modified Date:      2018-04-07
** Last Version:            v1.0
** Description:             ���ݽڵ�ų�ʼ����������
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              ��УԴ
** Created date:            2018-04-07
** Version:                 v1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
#include "radio_para.h"
#include "globalmacro.h"
/*********************************************************************************************************
  ȫ�ֱ�������
*********************************************************************************************************/
radio_para radiopara;

/*********************************************************************************************************
** Function name:       radio_para_init
** Descriptions:        ����������ʼ��
** input parameters:    0
** output parameters:   ��
** Returned value:      moteid
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/
//TODO ���ݽڵ�����ò�ͬ�Ĺ���

void radio_para_init(){
    radiopara.shortaddr = get_moteid();
    radiopara.pan_id = get_cluster_name(radiopara.shortaddr);
    radiopara.max_frame_retrise = DEFAULT_MAX_FRAME_RETRIES;
    radiopara.max_csma_retries = 0x03;
    radiopara.tx_power = 0x0E;
    radiopara.channel = DEFAULT_CHANNEL;
    radiopara.pancooder =DEEFAULT_COODER;
    if(get_moteid() == TIME_SYNCH_NODE )
      radiopara.tx_power = 0x02;
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
