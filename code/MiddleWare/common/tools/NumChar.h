/****************************************Copyright (c)****************************************************
**
**                          一切为理想而作，打造嵌入式WEB节省资源的最优化应用
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           NumChar.h
** Last modified Date:  2008.11.20
** Last Version:        v1.0
** Description:         字符串与数字之间的一些处理函数的头文件声明
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2008.11.20
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
#ifndef __NumChar_h
#define __NumChar_h

/***********************************函数原型声明********************************************************/

extern unsigned long PowerCall(unsigned long ulSrc, unsigned long ulPower);
extern void strToNumArray(unsigned char *pucDest, const unsigned char *pscSrc, unsigned long ulLen);
extern unsigned long strToLNum(unsigned char *pscSrc, unsigned long ulLen);
extern unsigned char charToHex(unsigned char *Buffer);

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

