/****************************************Copyright (c)****************************************************
**
**                          一切为理想而作，打造嵌入式WEB节省资源的最优化应用
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               memsearch.h
** Last modified Date:      2008.11.22
** Last Version:            v1.0
** Description:             在指定的内存中查找字符或字符串等的处理函数的头文件声明
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              任海波
** Created date:            2008.11.20
** Version:                 v1.0
** Descriptions:            The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Description:             
**
*********************************************************************************************************/
#ifndef	 __memsearch_h
#define	 __memsearch_h

extern void *memSearch(void *mem, unsigned long ulmemSize, 
                       unsigned char ucData, unsigned long ulDataSize);
extern void *memchrExt(void *mem, unsigned long ulmemSize, unsigned char ucCh);
extern unsigned char bMemStr(void *mem, unsigned long ulmemSize, 
                             unsigned char *pucStr, unsigned long ulStrSize);
extern void *pMemStr(void *mem, unsigned long ulmemSize,
                     unsigned char *pucStr, unsigned long ulStrSize);

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

