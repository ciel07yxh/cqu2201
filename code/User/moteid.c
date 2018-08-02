#include "moteid.h"
#include "cfs-coffee-arch.h"
uint16_t moteid;

void moteid_init(void)
{
  stm32f10xx_flash_read(MOTE_ID_FLASH_ADDR,&moteid,2);
}

uint16_t get_moteid(void)
{
  return moteid;
}