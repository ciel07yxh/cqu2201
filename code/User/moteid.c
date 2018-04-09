#include "moteid.h"
#include "cfs-coffee-arch.h"
uint16_t moteid;


/*********************************************************************************************************
** Function name:       moteid_init
** Descriptions:        moteid 初始化
** input parameters:    0
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/
void moteid_init(void)
{
  stm32f10xx_flash_read(MOTE_ID_FLASH_ADDR,&moteid,2);
}

/*********************************************************************************************************
** Function name:       get_moteid
** Descriptions:        获取moteid
** input parameters:    0
** output parameters:   无
** Returned value:      moteid
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/
uint16_t get_moteid(void)
{
  return moteid;
}

/*********************************************************************************************************
** Function name:       get_cluster_name
** Descriptions:        获取分簇编号
** input parameters:    moteid
** output parameters:   无
** Returned value:      0
** Created by:          张校源
** Created Date:        2018-04-07
*********************************************************************************************************/
uint16_t get_cluster_name(uint16_t moteid)
{
  return (((moteid-1)/5)+1);
}
