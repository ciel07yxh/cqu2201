/****************************************Copyright (c)****************************************************
**
**                          一切为理想而作，打造嵌入式WEB节省资源的最优化应用
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           stringExt.h
** Last modified Date:  2008.11.22
** Last Version:        v1.0
** Description:         标准C语言中一些字符串处理函数的扩充的头文件声明
** 
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2008.11.22
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
#ifndef __stringExt_h
#define __stringExt_h

extern unsigned char *strcatExt(unsigned char *pDest, unsigned long ulDestLen, 
                                const unsigned char *pSrc, unsigned long ulSrcLen);
extern unsigned long strlenExt(const unsigned char *pucSrc);

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

