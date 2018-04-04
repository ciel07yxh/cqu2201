/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           hal_io_ints.c
** Last modified Date:  2013-04-01
** Last Version:        v1.00
** Description:         中断相关函数实现
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo
** Created date:        2013-04-01
** Version:             v1.00
** Descriptions:        
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Description:         
*********************************************************************************************************/
#include "infra/rhb_type.h"
#include "hal/CortexM3/hal_io_ints.h"
#include "hal/arch/inc/arch_io_ints.h"

/*********************************************************************************************************
  全局变量声明 
*********************************************************************************************************/
// 中断向量表定义，该表需要放在RAM起始地址
static __no_init void (*__GfnRAMVectors[ARCH_NUM_INTERRUPTS])(void) @ "VTABLE";
// 中断优先级分组值定义
static const uint32 __GpulPriority[] =
{
    HAL_NVIC_APINT_PRIGROUP_0_8, HAL_NVIC_APINT_PRIGROUP_1_7, HAL_NVIC_APINT_PRIGROUP_2_6,
    HAL_NVIC_APINT_PRIGROUP_3_5, HAL_NVIC_APINT_PRIGROUP_4_4, HAL_NVIC_APINT_PRIGROUP_5_3,
    HAL_NVIC_APINT_PRIGROUP_6_2, HAL_NVIC_APINT_PRIGROUP_7_1
};


/*********************************************************************************************************
** Function name:       IntDefaultHandler
** Descriptions:        默认的中断服务函数定义
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
static void IntDefaultHandler(void)
{
    // 进入死循环
    while(1)
    {
    }
}
/*********************************************************************************************************
** Function name:       hal_int_register
** Descriptions:        注册中断服务函数
** input parameters:    u32IntNo:   中断服务编号
**                      pfnHandler: 待注册的中断服务函数
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_register(uint32 u32IntNo, void (*pfnHandler)(void))
{
    uint32 i;
    uint32 u32Temp;
    
    if(HWREG32(HAL_NVIC_VTABLE) != (uint32)__GfnRAMVectors) {
        // 将Flash中的向量表复制到RAM区的向量表中
        u32Temp = HWREG32(HAL_NVIC_VTABLE);
        for(i=0; i<ARCH_NUM_INTERRUPTS; i++) {
            __GfnRAMVectors[i] = (void (*)(void))HWREG32((i * 4) + u32Temp);
        }
        // 复制完毕，将向量表映射到RAM区
        HWREG32(HAL_NVIC_VTABLE) = (uint32)__GfnRAMVectors;
    }
    
    // 注册中断服务函数
    __GfnRAMVectors[u32IntNo] = pfnHandler;
}

/*********************************************************************************************************
** Function name:       hal_int_unregister
** Descriptions:        注销中断服务函数
** input parameters:    u32IntNo:   中断服务编号
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_unregister(uint32 u32IntNo)
{
    __GfnRAMVectors[u32IntNo] = IntDefaultHandler;
}

/*********************************************************************************************************
** Function name:       hal_int_enable
** Descriptions:        使能中断
** input parameters:    u32IntNo:   中断服务编号
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_enable(uint32 u32IntNo)
{
    // 如果是内部中断
    if(u32IntNo < 16) {
        if(u32IntNo == HAL_INT_MPU){
            // Enable the MemManage interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) |= HAL_NVIC_SYS_HND_CTRL_MEM;
        } else if(u32IntNo == HAL_INT_BUS) {
            // Enable the bus fault interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) |= HAL_NVIC_SYS_HND_CTRL_BUS;
        } else if(u32IntNo == HAL_INT_USAGE) {
            // Enable the usage fault interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) |= HAL_NVIC_SYS_HND_CTRL_USAGE;
        } else if(u32IntNo == HAL_INT_SYSTICK) {
            // Enable the System Tick interrupt.
            HWREG32(HAL_NVIC_ST_CTRL) |= HAL_NVIC_ST_CTRL_INTEN;
        }
        return;
    }
    
    if(u32IntNo > 255) {
        return;
    }
    u32IntNo -= HAL_EXTERN_INT_BASE;
    HWREG32(HAL_NVIC_EN_BASE + ((u32IntNo / 32) * 4)) |= (1 << (u32IntNo % 32));
}

/*********************************************************************************************************
** Function name:       hal_int_disable
** Descriptions:        禁止中断
** input parameters:    u32IntNo:   中断服务编号
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_disable(uint32 u32IntNo)
{
    // 如果是内部中断
    if(u32IntNo < 16) {
        if(u32IntNo == HAL_INT_MPU){
            // Enable the MemManage interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) &= ~HAL_NVIC_SYS_HND_CTRL_MEM;
        } else if(u32IntNo == HAL_INT_BUS) {
            // Enable the bus fault interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) &= ~HAL_NVIC_SYS_HND_CTRL_BUS;
        } else if(u32IntNo == HAL_INT_USAGE) {
            // Enable the usage fault interrupt.
            HWREG32(HAL_NVIC_SYS_HND_CTRL) &= ~HAL_NVIC_SYS_HND_CTRL_USAGE;
        } else if(u32IntNo == HAL_INT_SYSTICK) {
            // Enable the System Tick interrupt.
            HWREG32(HAL_NVIC_ST_CTRL) &= ~HAL_NVIC_ST_CTRL_INTEN;
        }
        return;
    }
    
    if(u32IntNo > 255) {
        return;
    }
    
    u32IntNo -= HAL_EXTERN_INT_BASE;
    HWREG32(HAL_NVIC_DIS_BASE + ((u32IntNo / 32) * 4)) |= (1 << (u32IntNo % 32));
}

/*********************************************************************************************************
** Function name:       hal_int_priority_group_set
** Descriptions:        设置中断优先级分组
** input parameters:    u32Bits:   可抢占优先级占用的位数
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_priority_group_set(uint32 u32Bits)
{
    HWREG32(HAL_NVIC_APINT) = HAL_NVIC_APINT_VECTKEY | __GpulPriority[u32Bits];
}

/*********************************************************************************************************
** Function name:       hal_int_priority_group_get
** Descriptions:        获取中断优先级分组
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      可抢占优先级占用的位数
*********************************************************************************************************/
uint32 hal_int_priority_group_get(uint32 u32Bits)
{
    uint32 u32Temp, i;
    
    u32Temp = HWREG32(HAL_NVIC_APINT) & HAL_NVIC_APINT_PRIGROUP_M;
    for(i=0; i<ARCH_NUM_PRIORITY; i++) {
        if(u32Temp == __GpulPriority[i]) {
            break;
        }
    }
    
    return i;
}

/*********************************************************************************************************
** Function name:       hal_int_priority_set
** Descriptions:        设置中断优先级
** input parameters:    u32IntNo:   中断编号
**                      u8Priority: 待设置的中断优先级
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void hal_int_priority_set(uint32 u32IntNo, uint8 u8Priority)
{
    // 该函数不设置内核的中断，仅仅设置大于16的外部中断
    if((u32IntNo < 16) || (u32IntNo > 255)) {
        return;
    }
    u32IntNo -= HAL_EXTERN_INT_BASE;
    HWREG8(HAL_NVIC_PRI_BASE + u32IntNo) = u8Priority;
}

/*********************************************************************************************************
** Function name:       hal_int_priority_get
** Descriptions:        获取中断优先级
** input parameters:    u32IntNo:   中断编号
** output parameters:   NONE
** Returned value:      获取的中断优先级
*********************************************************************************************************/
uint8 hal_int_priority_get(uint32 u32IntNo)
{
    // 该函数不设置内核的中断，仅仅设置大于16的外部中断
    if((u32IntNo < 16) || (u32IntNo > 255)) {
        return 0;
    }
    u32IntNo -= HAL_EXTERN_INT_BASE;
    return (HWREG8(HAL_NVIC_PRI_BASE + u32IntNo));
}
/*********************************************************************************************************
END FILE
*********************************************************************************************************/