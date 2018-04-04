/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_rcc.c
** Last modified Date:  2013-04-01
** Last Version:        v1.0
** Description:         rcc模块寄存器封装函数实现
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-01
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
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"

/*********************************************************************************************************
  时钟配置宏转变
*********************************************************************************************************/
#if (RCC_CLOCK_HCLK_DIV == 1)
#define RCC_CFGR_HPRE   (0<<4)
#elif (RCC_CLOCK_HCLK_DIV == 2)
#define RCC_CFGR_HPRE   (8<<4)
#elif (RCC_CLOCK_HCLK_DIV == 4)
#define RCC_CFGR_HPRE   (9<<4)
#elif (RCC_CLOCK_HCLK_DIV == 8)
#define RCC_CFGR_HPRE   (10<<4)
#elif (RCC_CLOCK_HCLK_DIV == 16)
#define RCC_CFGR_HPRE   (11<<4)
#elif (RCC_CLOCK_HCLK_DIV == 64)
#define RCC_CFGR_HPRE   (12<<4)
#elif (RCC_CLOCK_HCLK_DIV == 128)
#define RCC_CFGR_HPRE   (13<<4)
#elif (RCC_CLOCK_HCLK_DIV == 256)
#define RCC_CFGR_HPRE   (14<<4)
#elif (RCC_CLOCK_HCLK_DIV == 512)
#define RCC_CFGR_HPRE   (15<<4)
#endif

#if (RCC_CLOCK_PCLK1_DIV == 1)
#define RCC_CFGR_PPRE1  (0<<8)
#elif (RCC_CLOCK_PCLK1_DIV == 2)
#define RCC_CFGR_PPRE1  (4<<8)
#elif (RCC_CLOCK_PCLK1_DIV == 4)
#define RCC_CFGR_PPRE1  (5<<8)
#elif (RCC_CLOCK_PCLK1_DIV == 8)
#define RCC_CFGR_PPRE1  (6<<8)
#elif (RCC_CLOCK_PCLK1_DIV == 16)
#define RCC_CFGR_PPRE1  (7<<8)
#endif

#if (RCC_CLOCK_PCLK2_DIV == 1)
#define RCC_CFGR_PPRE2  (0<<11)
#elif (RCC_CLOCK_PCLK2_DIV == 2)
#define RCC_CFGR_PPRE2  (4<<11)
#elif (RCC_CLOCK_PCLK2_DIV == 4)
#define RCC_CFGR_PPRE2  (5<<11)
#elif (RCC_CLOCK_PCLK2_DIV == 8)
#define RCC_CFGR_PPRE2  (6<<11)
#elif (RCC_CLOCK_PCLK2_DIV == 16)
#define RCC_CFGR_PPRE2  (7<<11)
#endif

/*********************************************************************************************************
  局部变量定义
*********************************************************************************************************/
// 时钟相关定义
static __no_init uint32 __G_stm32_sysclk;
static __no_init uint32 __G_stm32_hclk;
static __no_init uint32 __G_stm32_pclk1;
static __no_init uint32 __G_stm32_pclk2;
static __no_init uint32 __G_cortexm_systick_clock;

// 外设复位寄存器描述
static const uint32 __GSysPeripheralResetReg[] = {
    0, // 处理器不支持AHB外设的复位
    RCC_O_APB1RSTR,
    RCC_O_APB2RSTR,
};

// 外设时钟时能寄存器描述
static const uint32 __GSysPeripheralEnableReg[] = {
    RCC_O_AHBENR,
    RCC_O_APB1ENR,
    RCC_O_APB2ENR,
};

/*********************************************************************************************************
** Function name:       rccClockSet
** Descriptions:        时钟配置，需要在头文件中配置相应的系数
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void SystemClockInit(void)
{
    unsigned long ulTemp;
    HWREG32(RCC_BASE + RCC_O_CR) = 0x0083;
    HWREG32(RCC_BASE + RCC_O_CFGR) = 0x0000;
    HWREG32(RCC_BASE + RCC_O_CIR) = 0x0000;
    
    HWREG32(RCC_BASE + RCC_O_CR) = 0x00000001;     // reset RCC
    
    // Start up HSE clock
    HWREG32(RCC_BASE + RCC_O_CR) &= ((1<<16) | (1<<18));
    HWREG32(RCC_BASE + RCC_O_CR) |= (1<<16);
    
    // Wait for HSE clock to startup
    while(!(HWREG32(RCC_BASE + RCC_O_CR) & (1<<17))) ; 
    
    // 使能flash预取指及等待周期2个wait
    HWREG32(FLASH_BASE + FLASH_O_ACR) |= ((1<<4) | 0x02);
    
    __G_stm32_sysclk = RCC_HSE_INPUT_CLOCK;
    ulTemp = 0;
#if defined (RCC_CLOCK_PLL_SOURCE_HSE)
    ulTemp |= (1<<16);
#elif defined (RCC_CLOCK_PLL_SOURCE_HSE_HALF)
    ulTemp |= ((1<<16) | (1<<17));
    __G_stm32_sysclk /= 2;
#elif defined (RCC_CLOCK_PLL_SOURCE_HSI_HALF)
    __G_stm32_sysclk /= 2;
#endif
    
    ulTemp |= ((RCC_CLOCK_PLL_MUL - 2) << 18);
    ulTemp |= RCC_CFGR_HPRE;
    ulTemp |= RCC_CFGR_PPRE1;
    ulTemp |= RCC_CFGR_PPRE2;

    HWREG32(RCC_BASE + RCC_O_CFGR) = ulTemp;

    // Enable the PLL and wait for it to lock
    HWREG32(RCC_BASE + RCC_O_CR) |= (1<<24);
    while(!(HWREG32(RCC_BASE + RCC_O_CR) & (1<<25))) ;

    // Now switch to use PLL as SYSCLK and wait it finish
    HWREG32(RCC_BASE + RCC_O_CFGR) |= 0x02;
    while(((HWREG32(RCC_BASE + RCC_O_CFGR) >> 2) & 0x03) != 0x02) ;

    //  根据需要配置MCO输出与USB模块的时钟，注意当使用USB时，时钟必须配置为48M或72M
    
    // Calculate clocks from configuration
    __G_stm32_sysclk *= RCC_CLOCK_PLL_MUL;
    __G_stm32_hclk = __G_stm32_sysclk / RCC_CLOCK_HCLK_DIV;
    __G_stm32_pclk1 = __G_stm32_sysclk / RCC_CLOCK_PCLK1_DIV;
    __G_stm32_pclk2 = __G_stm32_sysclk / RCC_CLOCK_PCLK2_DIV;
    __G_cortexm_systick_clock = __G_stm32_sysclk / 8;
}
/*********************************************************************************************************
** Function name:       SystemSysClockGet
** Descriptions:        获取系统主频
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      系统主频
*********************************************************************************************************/
uint32 SystemSysClockGet(void)
{
    return __G_stm32_sysclk;
}

/*********************************************************************************************************
** Function name:       SystemAHBClockGet
** Descriptions:        获取AHB总线时钟频率
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      AHB时钟频率
*********************************************************************************************************/
uint32 SystemAHBClockGet(void)
{
    return __G_stm32_hclk;
}

/*********************************************************************************************************
** Function name:       SystemAPB1ClockGet
** Descriptions:        获取APB1总线时钟频率
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      APB1时钟频率
*********************************************************************************************************/
uint32 SystemAPB1ClockGet(void)
{
    return __G_stm32_pclk1;
}

/*********************************************************************************************************
** Function name:       SystemAPB2ClockGet
** Descriptions:        获取APB2总线时钟频率
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      APB2时钟频率
*********************************************************************************************************/
uint32 SystemAPB2ClockGet(void)
{
    return __G_stm32_pclk2;
}

/*********************************************************************************************************
** Function name:       SystemSysTickClockGet
** Descriptions:        获取System tick总线时钟频率
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      APB2时钟频率
*********************************************************************************************************/
uint32 SystemSysTickClockGet(void)
{
  return __G_cortexm_systick_clock;
}

/*********************************************************************************************************
** Function name:       SystemPeripheralReset
** Descriptions:        复位外设
** input parameters:    u8WhichBus:  外设位于那条总线
**                      u32Ctrl：外设控制位
** output parameters:   NONE
** Returned value:      NONE
注意：两个参数有一个统一的宏定义，具体取值可以参考arch_io_rcc.h中的定义
*********************************************************************************************************/
void SystemPeripheralReset(uint8 u8WhichBus, uint32 u32Ctrl)
{
    // AHB外设不支持复位
    if(u8WhichBus == 0) {
        return;
    }
    HWREG32(RCC_BASE + __GSysPeripheralResetReg[u8WhichBus]) |= u32Ctrl;
    HWREG32(RCC_BASE + __GSysPeripheralResetReg[u8WhichBus]) &= ~u32Ctrl;
}

/*********************************************************************************************************
** Function name:       SystemPeripheralEnable
** Descriptions:        使能外设时钟
** input parameters:    u8WhichBus:  外设位于那条总线
**                      u32Ctrl：外设控制位
** output parameters:   NONE
** Returned value:      NONE
注意：两个参数有一个统一的宏定义，具体取值可以参考arch_io_rcc.h中的定义
*********************************************************************************************************/
void SystemPeripheralEnable(uint8 u8WhichBus, uint32 u32Ctrl)
{
    HWREG32(RCC_BASE + __GSysPeripheralEnableReg[u8WhichBus]) |= u32Ctrl;
}

/*********************************************************************************************************
** Function name:       SystemPeripheralDisable
** Descriptions:        禁能外设时钟
** input parameters:    u8WhichBus:  外设位于那条总线
**                      u32Ctrl：外设控制位
** output parameters:   NONE
** Returned value:      NONE
注意：两个参数有一个统一的宏定义，具体取值可以参考arch_io_rcc.h中的定义
*********************************************************************************************************/
void SystemPeripheralDisable(uint8 u8WhichBus, uint32 u32Ctrl)
{
    HWREG32(RCC_BASE + __GSysPeripheralEnableReg[u8WhichBus]) &= ~u32Ctrl;
}

/*********************************************************************************************************
** Function name:       SystemPeripheralClockGet
** Descriptions:        获取指定的外设时钟
** input parameters:    u8WhichBus:  外设位于那条总线
**                      u32Ctrl：外设控制位
** output parameters:   NONE
** Returned value:      NONE
注意：两个参数有一个统一的宏定义，具体取值可以参考arch_io_rcc.h中的定义
*********************************************************************************************************/
uint32 SystemPeripheralClockGet(uint8 u8WhichBus, uint32 u32Ctrl)
{
    uint32 u32Clock;
    switch(u8WhichBus) {
        case 0:
            u32Clock = SystemAHBClockGet();
            break;
        case 1:
            u32Clock = SystemAPB1ClockGet();
            break;
        case 2:
            u32Clock = SystemAPB2ClockGet();
            break;
        default:
            u32Clock = 0;
            break;
    }
    return u32Clock;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
