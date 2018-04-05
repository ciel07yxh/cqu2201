/**************************************************
 *
 * This module contains the function `__low_level_init', a function
 * that is called before the `main' function of the program.  Normally
 * low-level initializations - such as setting the prefered interrupt
 * level or setting the watchdog - can be performed here.
 *
 * Note that this function is called before the data segments are
 * initialized, this means that this function cannot rely on the
 * values of global or static variables.
 *
 * When this function returns zero, the startup code will inhibit the
 * initialization of the data segments. The result is faster startup,
 * the drawback is that neither global nor static data will be
 * initialized.
 *
 * Copyright 1999-2004 IAR Systems. All rights reserved.
 *
 * $Revision: 21623 $
 *
 **************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * 包含需要的头文件
  */

#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_ints.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/CortexM3/hal_io_level.h"

  /**
   * 以下是IAR提供的模板代码
  */
#pragma language=extended

__interwork int __low_level_init(void);

__interwork int __low_level_init(void)
{
  /*==================================*/
  /*  Initialize hardware.            */
  /*==================================*/

  /**
   * Step1, 初始化系统时钟
  */
  //修改系统中断向量表起始位置为ROM开始的位置（BootLoader跳转的位置）
  HWREG32(0xE000ED08) = 0x08006000;
  SystemClockInit();

  /**
   * Step2, 开启使用的外设时钟
  */
  //  可以在这里开启需要的外设时钟

  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not.           */
  /* Return: 0 to omit seg_init       */
  /*         1 to run seg_init        */
  /*==================================*/
  return 1;
}

#pragma language=default

#ifdef __cplusplus
}
#endif
