#include "userinit.h"
#include "moteid.h"
#define DEBUG 1
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


/*********************************************************************************************************
** Function name:       userinit
** Descriptions:        用户初始化函数，自定义需要初始化的功能（单独物理层）
** input parameters:    无
** output parameters:   无
** Returned value:      无
** Created by:          张校源
** Created Date:        2018-04-05
*********************************************************************************************************/
void userinit(void){
  //初始化Radio 之前 初始化各项参数
  moteid_init();
  parainit(get_moteid());
  NETSTACK_RADIO.init();
}

int parainit(uint16_t moteid){
  PRINTF("time is up");
   PRINTF("time is up");
    PRINTF("time is up"); PRINTF("time is up"); PRINTF("time is up"); PRINTF("time is up"); PRINTF("time is up"); PRINTF("time is up"); PRINTF("time is up");
    return 1;
}