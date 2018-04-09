#include "moteid.h"
#include "cfs-coffee-arch.h"
uint16_t moteid;


/*********************************************************************************************************
** Function name:       moteid_init
** Descriptions:        moteid ��ʼ��
** input parameters:    0
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/
void moteid_init(void)
{
  stm32f10xx_flash_read(MOTE_ID_FLASH_ADDR,&moteid,2);
}

/*********************************************************************************************************
** Function name:       get_moteid
** Descriptions:        ��ȡmoteid
** input parameters:    0
** output parameters:   ��
** Returned value:      moteid
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/
uint16_t get_moteid(void)
{
  return moteid;
}

/*********************************************************************************************************
** Function name:       get_cluster_name
** Descriptions:        ��ȡ�ִر��
** input parameters:    moteid
** output parameters:   ��
** Returned value:      0
** Created by:          ��УԴ
** Created Date:        2018-04-07
*********************************************************************************************************/
uint16_t get_cluster_name(uint16_t moteid)
{
  return (((moteid-1)/5)+1);
}
