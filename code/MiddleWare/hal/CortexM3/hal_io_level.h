/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           hal_io_level.h
** Last modified Date:  2011-04-16
** Last Version:        v1.00
** Description:         与处理器架构相关的一些硬件接口层必要函数
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo
** Created date:        2011-04-16
** Version:             v1.00
** Descriptions:        
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Description:         
*********************************************************************************************************/
#ifndef __HAL_IO_LEVEL_H__
#define __HAL_IO_LEVEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"

#define  hal_cpu_reset()   (HAL_WRITE_UINT32(0xE000ED0C,0x05FA0004)) // 启动系统复位
/*********************************************************************************************************
**  实现的外部函数声明
*********************************************************************************************************/
extern uint8 hal_enter_critical(uint8 u8BasePriMask);
extern void hal_exit_critical(uint8 u8Previous);
extern void hal_app_jump(uint32 u32AppAddr);

extern void hal_int_register(uint32 u32IntNo, void (*pfnHandler)(void));
extern void hal_int_unregister(uint32 u32IntNo);
extern void hal_int_enable(uint32 u32IntNo);
extern void hal_int_disable(uint32 u32IntNo);
extern void hal_int_priority_group_set(uint32 u32Bits);
extern uint32 hal_int_priority_group_get(uint32 u32Bits);
extern void hal_int_priority_set(uint32 u32IntNo, uint8 u8Priority);
extern uint8 hal_int_priority_get(uint32 u32IntNo);

#ifdef __cplusplus
    }
#endif      // __cplusplus
    
#endif      // __HAL_IO_LEVEL_H__
/*********************************************************************************************************
END FILE
*********************************************************************************************************/