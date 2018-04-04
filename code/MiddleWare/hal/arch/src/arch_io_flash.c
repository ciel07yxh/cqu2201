/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_flash.c
** Last modified Date:  2013-04-04
** Last Version:        v1.0
** Description:         flash模块寄存器封装函数实现声明
**
**--------------------------------------------------------------------------------------------------------
** Created By:          Renhaibo任海波
** Created date:        2013-04-04
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
#include "hal/arch/inc/arch_io_flash.h"

extern void watchdog_periodic(void);
/*********************************************************************************************************
** Function name:       flashUnlock
** Descriptions:        对flash模块解锁，解锁后可以擦除及编程Flash
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void flashUnlock(void)
{
    HWREG32(FLASH_BASE + FLASH_O_KEYR) = FLASH_UNLOCK_KER1;
    HWREG32(FLASH_BASE + FLASH_O_KEYR) = FLASH_UNLOCK_KER2;
    
#if (STM32F10XX_XL_Density > 0)
    HWREG32(FLASH_BASE + FLASH_O_KEYR2) = FLASH_UNLOCK_KER1;
    HWREG32(FLASH_BASE + FLASH_O_KEYR2) = FLASH_UNLOCK_KER2;
#endif
}

/*********************************************************************************************************
** Function name:       flashLock
** Descriptions:        对flash模块加锁，加锁后flash不允许擦除及编程
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void flashLock(void)
{
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000080;
    
#if (STM32F10XX_XL_Density > 0)
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000080;
#endif
}

/*********************************************************************************************************
** Function name:       flashPageErase
** Descriptions:        擦除指定的一页
** input parameters:    ulPageAddr：指定擦除页的地址，不同的系列芯片页大小可能不同
** output parameters:   NONE
** Returned value:      true：擦除成功； false: 出现了错误，擦除失败
*********************************************************************************************************/
boolean flashPageErase(unsigned long ulPageAddr)
{
#if (STM32F10XX_XL_Density > 0)
  if(ulPageAddr < FLASH_BANK1_END_ADDRESS) {
      while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){   //  等待器件不忙
        watchdog_periodic();
      }
      
      if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
      {
          HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000002;       //  选择页擦除
          HWREG32(FLASH_BASE + FLASH_O_AR)  = ulPageAddr;       //  写入页地址
          HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000040;       //  启动擦除
          while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){ //  等待擦除完成
            watchdog_periodic();
          }
          HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFD;       //  页擦除完成，清除页擦除标志
          if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
          {
            return true;
          } else {
            return false;
          }
      } else {
          return false;
      }
  } else {
      while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001){   //  等待器件不忙
        watchdog_periodic();
      }
      
      if (!((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
      {
          HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000002;       //  选择页擦除
          HWREG32(FLASH_BASE + FLASH_O_AR2)  = ulPageAddr;       //  写入页地址
          HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000040;       //  启动擦除
          while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001){ //  等待擦除完成
            watchdog_periodic();
          }
          HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFD;       //  页擦除完成，清除页擦除标志
          if (!((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
          {
            return true;
          } else {
            return false;
          }
      } else {
          return false;
      }
  }
#else
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){   //  等待器件不忙
      watchdog_periodic();
    }
      
    if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
    {
        HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000002;       //  选择页擦除
        HWREG32(FLASH_BASE + FLASH_O_AR)  = ulPageAddr;       //  写入页地址
        HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000040;       //  启动擦除
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){ //  等待擦除完成
          watchdog_periodic();
        }
        HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFD;       //  页擦除完成，清除页擦除标志
        if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
        {
          return true;
        } else {
          return false;
        }
    } else {
        return false;
    }
#endif
}

/*********************************************************************************************************
** Function name:       flashMassErase
** Descriptions:        全片擦除flash
** input parameters:    NONE
** output parameters:   NONE
** Returned value:      true：擦除成功； false: 出现了错误，擦除失败
*********************************************************************************************************/
boolean flashMassErase(void)
{
#if (STM32F10XX_XL_Density > 0)
    // 先擦除Bank1
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){   //  等待器件不忙
      watchdog_periodic();
    }
    
    if (((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014)) {      //  如果上次有错误发生,包括编程错误或写保护错误
       return false;
    }
      
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000004;       //  选择全片擦除
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000040;       //  启动擦除
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001) { //  等待擦除完成
      watchdog_periodic();
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFB;       //  全片擦除完成，清除擦除标志
    if (((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014)) {      //  如果有错误发生,包括编程错误或写保护错误
       return false;
    }
    
    // 再擦除Bank2
    while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001){   //  等待器件不忙
      watchdog_periodic();
    }
    
    if (((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014)) {      //  如果上次有错误发生,包括编程错误或写保护错误
       return false;
    }
      
    HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000004;       //  选择全片擦除
    HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000040;       //  启动擦除
    while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001){ //  等待擦除完成
      watchdog_periodic();
    }
    HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFB;       //  全片擦除完成，清除擦除标志
    if (((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014)) {      //  如果有错误发生,包括编程错误或写保护错误
       return false;
    }
    
    return true;
  
  
#else
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){   //  等待器件不忙
      watchdog_periodic();
    }
    
    if (((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014)) {      //  如果上次有错误发生,包括编程错误或写保护错误
       return false;
    }
      
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000004;       //  选择全片擦除
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000040;       //  启动擦除
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001){ //  等待擦除完成
       watchdog_periodic();
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFB;       //  全片擦除完成，清除擦除标志
    if (((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014)) {      //  如果有错误发生,包括编程错误或写保护错误
       return false;
    }
    
    return true;
#endif
}

/*********************************************************************************************************
** Function name:       flashWriteHalfWord
** Descriptions:        向Flash的指定地址写如一个16位的数据
** input parameters:    ulAddr:   指定写入的地址
**                      usData:   指定要写入的数据
** output parameters:   NONE
** Returned value:      true：写成功； false: 出现了错误，写失败
*********************************************************************************************************/
boolean flashWriteHalfWord(unsigned long ulAddr, unsigned short usData)
{
#if (STM32F10XX_XL_Density > 0)
   if(ulAddr < FLASH_BANK1_END_ADDRESS) {
      while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
      
      if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
      {
          HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;        //  选择flash编程模式
          HWREG16(ulAddr) = usData;                            //  写入编程的半字数据
          while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
          HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;        // 编程完成，清除编程标志
          if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
          {
            return true;
          } else {
            return false;
          }
      }
      return false;
   }else {
      while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
      
      if (!((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
      {
          HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000001;        //  选择flash编程模式
          HWREG16(ulAddr) = usData;                            //  写入编程的半字数据
          while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
          HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFE;        // 编程完成，清除编程标志
          if (!((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
          {
            return true;
          } else {
            return false;
          }
      }
      return false;
   }
#else 
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
    
    if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果上次没有错误发生,包括编程错误或写保护错误
    {
        HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;        //  选择flash编程模式
        HWREG16(ulAddr) = usData;                            //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
        HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;        // 编程完成，清除编程标志
        if (!((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x0014))       //  如果没有错误发生,包括编程错误或写保护错误
        {
          return true;
        } else {
          return false;
        }
    }
    return false;
#endif
}

/*********************************************************************************************************
** Function name:       flashProgram
** Descriptions:        向Flash的指定地址编程指定字节的数据
** input parameters:    pusData:   编程数据缓冲区指针
**                      ulAddr:    指定编程地址
**                      ulSize:    指定编程的字节数，必须是2的倍数
** output parameters:   NONE
** Returned value:      true：写成功； false: 出现了错误，写失败
*********************************************************************************************************/
boolean flashProgram(unsigned short *pusData, unsigned long ulAddr, unsigned long ulSize)
{
    if ((ulSize < 2) || (ulAddr < 0x08000000) || ((ulSize % 2) != 0))
        return false;
    
#if (STM32F10XX_XL_Density > 0)
    if(ulAddr < FLASH_BANK1_END_ADDRESS) {
      while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
      
      HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;            //  选择flash编程模式
    } else {
      goto FlashProgramBank2;
    }
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = *pusData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
        pusData++;
        ulAddr += 2;
        ulSize -= 2;
        if(ulAddr > FLASH_BANK1_END_ADDRESS) {
          HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;       // 编程完成，清除编程标志
          goto FlashProgramBank2;
        }
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;            // 编程完成，清除编程标志
    
FlashProgramBank2:
    while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
      
    HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000001;            //  选择flash编程模式
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = *pusData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
        pusData++;
        ulAddr += 2;
        ulSize -= 2;
    }
    HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFE;            // 编程完成，清除编程标志
    
    return true;
#else
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
    
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;            //  选择flash编程模式
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = *pusData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
        pusData++;
        ulAddr += 2;
        ulSize -= 2;
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;            // 编程完成，清除编程标志
    return true;
#endif
}

/*********************************************************************************************************
** Function name:       flashConstDataProgram
** Descriptions:        向Flash的指定地址编程写入固定的数据
** input parameters:    usData:    编程的固定数据
**                      ulAddr:    指定编程地址
**                      ulSize:    指定编程的字节数，必须是2的倍数
** output parameters:   NONE
** Returned value:      true：写成功； false: 出现了错误，写失败
*********************************************************************************************************/
boolean flashConstDataProgram(unsigned short usData, unsigned long ulAddr, unsigned long ulSize)
{
    if ((ulSize < 2) || (ulAddr < 0x08000000) || ((ulSize % 2) != 0))
        return false;
#if (STM32F10XX_XL_Density > 0)
    if(ulAddr < FLASH_BANK1_END_ADDRESS) {
      while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
      
      HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;            //  选择flash编程模式
    } else {
      goto FlashProgramConstBank2;
    }
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = usData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
        ulAddr += 2;
        ulSize -= 2;
        if(ulAddr > FLASH_BANK1_END_ADDRESS) {
          HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;            // 编程完成，清除编程标志
          goto FlashProgramConstBank2;
        }
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;            // 编程完成，清除编程标志
    
FlashProgramConstBank2:
    while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
      
    HWREG32(FLASH_BASE + FLASH_O_CR2) |= 0x00000001;            //  选择flash编程模式
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = usData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR2)) & 0x00000001);   //  等待器件不忙
        ulAddr += 2;
        ulSize -= 2;
    }
    HWREG32(FLASH_BASE + FLASH_O_CR2) &= 0x00001FFE;            // 编程完成，清除编程标志
    
    return true;
#else 
    while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
    
    HWREG32(FLASH_BASE + FLASH_O_CR) |= 0x00000001;            //  选择flash编程模式
    while(ulSize)
    {
        if((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x001)       //  如果上次有错误发生,包括编程错误或写保护错误
        {
            HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;    // 编程完成，清除编程标志
            return false;
        }
        HWREG16(ulAddr) = usData;                          //  写入编程的半字数据
        while((HWREG32(FLASH_BASE + FLASH_O_SR)) & 0x00000001);   //  等待器件不忙
        ulAddr += 2;
        ulSize -= 2;
    }
    HWREG32(FLASH_BASE + FLASH_O_CR) &= 0x00001FFE;            // 编程完成，清除编程标志
    return true;
#endif
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
