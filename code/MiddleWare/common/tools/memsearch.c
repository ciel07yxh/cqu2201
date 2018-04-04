/****************************************Copyright (c)****************************************************
**
**                          一切为理想而作，打造嵌入式WEB节省资源的最优化应用
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               memsearch.c
** Last modified Date:      2008.11.20
** Last Version:            v1.0
** Description:             在指定的内存中查找字符或字符串等的处理函数
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
#include  "includes.h"
#include  "memsearch.h"

/*********************************************************************************************************
** Function name:       memSearch
** Descriptions:        在指定大小的内存区域查找数据，该函数可以查找任意类型的数据
** input parameters:    mem:        指定的一个内存区的指针
**                      ulmemSize:  指定的内存区的大小
**                      ucData:     指定要查找的数据
**                      ulDataSize: 指定查找数据的大小(以字节为单位)
** output parameters:   无
** Returned value:      如果数据找到，返回位置指针；反之，返回NULL
** Created by:          任海波
** Created Date:        2008.11.22
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void *memSearch(void *mem, unsigned long ulmemSize, unsigned char ucData, unsigned long ulDataSize)
{
    unsigned char *__pAddr = (unsigned char *)mem;
    unsigned char *__pData = &ucData;
    unsigned long   i;
    unsigned char   __blIsMatch;

    do{
        __pAddr = memchrExt(__pAddr, (unsigned long)(((unsigned char *)mem + ulmemSize) - __pAddr), *__pData);
        if(__pAddr != NULL)
        {
            __blIsMatch = 1;
            for(i=1; i<ulDataSize; i++)
            {
                if(*(__pAddr + i) != *(__pData + i))
                {
                    __blIsMatch = 0;
                    break;
                }
            }

            if(__blIsMatch)
                return (void *)__pAddr;
                __pAddr++;
            } else break;
    }while(1);

    return NULL;
}

/*********************************************************************************************************
** Function name:       memchrExt  
** Descriptions:        在指定大小的内存区域查找字符
** input parameters:    mem: 指定的一个内存区的指针
**                      ulmemSize:  指定的内存区的大小
**                      ucCh:   指定要查找的字符
** output parameters:   无
** Returned value:      如果数据找到，返回位置指针；反之，返回NULL
** Created by:          任海波
** Created Date:        2008.11.22
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void *memchrExt(void *mem, unsigned long ulmemSize, unsigned char ucCh)
{
    unsigned char *__pbNext = (unsigned char *)mem;
    unsigned long    i;

    for(i=0; i<ulmemSize; i++)
    {
        if(*(__pbNext+i) == ucCh)
            return (__pbNext + i);
    }
    
    return NULL;
}

/*********************************************************************************************************
** Function name:       bMemStr  
** Descriptions:        在指定大小的内存区域查找字符串
** input parameters:    mem: 指定的一个内存区的指针
**                      ulmemSize:  指定的内存区的大小
**                      pucStr:   指定要查找的字符串
**                      ulStrSize：指定字符串的大小(以字节为单位)
** output parameters:   无
** Returned value:      如果数据找到，返回位置逻辑值1；反之，返回逻辑值0
** Created by:          任海波
** Created Date:        2008.11.22
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
unsigned char bMemStr(void *mem, unsigned long ulmemSize, unsigned char *pucStr, unsigned long ulStrSize)
{
        unsigned char  *__pbNext =(unsigned char*)mem;
    unsigned long   i;
    unsigned char   __blIsMatch;
    
    do{
           __pbNext = memchrExt(__pbNext, 
                                    (unsigned long)(((unsigned char*)mem + ulmemSize) - __pbNext),
                                    *pucStr);
           if(__pbNext != NULL)
           {
            __blIsMatch = 1;
            for(i=1; i<ulStrSize; i++)
            {
                if(*(__pbNext + i) != *(pucStr + i))
                {
                    __blIsMatch = 0;
                    break;
                }
            }

            if(__blIsMatch)
                return 1;

            __pbNext++;
           }
        else
            break;
    }while(1);

        return 0;
}

/*********************************************************************************************************
** Function name:       pMemStr 
** Descriptions:        在指定大小的内存区域查找字符串
** input parameters:    mem: 指定的一个内存区的指针
**                      ulmemSize:  指定的内存区的大小
**                      pucStr:   指定要查找的字符串
**                      ulStrSize：指定字符串的大小(以字节为单位)
** output parameters:   无
** Returned value:      如果数据找到，返回位置指针；反之，返回NULL
** Created by:          任海波
** Created Date:        2008.11.22
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void *pMemStr(void *mem, unsigned long ulmemSize, unsigned char *pucStr, unsigned long ulStrSize)
{
    unsigned char  *__pbNext =(unsigned char*)mem;
    unsigned long   i;
    unsigned char   __blIsMatch;
    
    do{
          __pbNext = memchrExt(__pbNext, 
                               (unsigned long)(((unsigned char*)mem + ulmemSize) - __pbNext),
                               *pucStr);
           if(__pbNext != NULL)
           {
            __blIsMatch = 1;
            for(i=1; i<ulStrSize; i++)
            {
                if(*(__pbNext + i) != *(pucStr + i))
                {
                    __blIsMatch = 0;
                    break;
                }
            }

            if(__blIsMatch)
                return (void *)__pbNext;
                __pbNext++;
           }
        else
            break;
    }while(1);

       return NULL;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

