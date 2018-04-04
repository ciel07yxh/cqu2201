/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           arch_io_ethernet.c
** Last modified Date:  2013-04-04
** Last Version:        v1.0
** Description:         ethernet模块寄存器封装函数实现声明
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
#include "hal/arch/inc/arch_io_ethernet.h"


/*********************************************************************************************************
  局部变量定义
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       EthernetInit
** Descriptions:        Ethernet模块配置
** input parameters:    u32Base:    ethernet模块基地址
**                      u32Clock:   提供给以太网模块的时钟，即AHB的时钟频率
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetInit(uint32 u32Base, uint32 u32Clock)
{
    uint32 u32Temp;
    uint32 u32PhyRegValue;
    uint32 u32AutoValue;

    // 根据u32Clock的值来确定MDC模块的时钟
    u32Temp = HWREG32(u32Base + ETH_MACMIIAR);
    u32Temp &= 0xFFFFFFE3;
    if((u32Clock >= 20000000) && (u32Clock < 35000000)) {
        u32Temp |= (0x00000002 << 2);
    } else if((u32Clock >= 35000000) && (u32Clock < 60000000)) {
        u32Temp |= (0x00000003 << 2);
    } else if((u32Clock >= 60000000) && (u32Clock < 100000000)) {
        u32Temp |= (0x00000000 << 2);
    } else {  // 如果都不是，时钟就是100M-120M
        u32Temp |= (0x00000001 << 2);
    }
    HWREG32(u32Base + ETH_MACMIIAR) = u32Temp;

    /*
    ** 在这里插入代码，完成PHY相关的初始化工作
    */
    // 读取PHY LAN8700的ID，如果错误，直接返回
    u32PhyRegValue = EthernetPHYRead(u32Base, PHYAddress, 16);
    if(u32PhyRegValue != 0x0040) {
      return;
    }

    EthernetPHYWrite(u32Base, PHYAddress, 0, 0x8000);           //  复位PHY
    while(EthernetPHYRead(u32Base, PHYAddress, 0) & 0x8000);   //  等待复位完成

    //u32PhyRegValue = EthernetPHYRead(u32Base, PHYAddress, 18);

    //  开启PHY的自协商功能
    u32Temp = EthernetPHYRead(u32Base, PHYAddress, 0);
    u32Temp |= 0x1000;
    EthernetPHYWrite(u32Base, PHYAddress, 0, u32Temp);

//    while(!((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0004)));
//    while(!((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0020)));
#if 0
    //  读取连接状态，如果连接完成
    if ((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0004)) {
        // 如果自协商完成了
        if((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0020)) {
            //  读取自协商的结果
            u32PhyRegValue = EthernetPHYRead(u32Base, PHYAddress, 0);

            if (u32PhyRegValue & 0x0100) {
                //  全双工通信模式
                u32AutoValue = 0x00000800;
            } else {
                //  半双工通信模式
                u32AutoValue = 0x00000000;
            }

            if (u32PhyRegValue & 0x2000) {
                //  100M以太网通信模式
                u32AutoValue |= 0x00004000;
            } else {
                //  10M以太网通信模式
                u32AutoValue |= 0x00000000;
            }
        } else {
            u32AutoValue = 0x00000000;
        }
    } else {
        u32AutoValue = 0x00000000;
    }
#endif

    // 将MAC直接配置成100M，全双工的通信模式
    u32AutoValue = 0x00004800;

    /*  PHY初始化配置已完成，自协商结果保存在变量u32AutoValue中，等待稍后配置MAC寄存器 */

    /*  以太网MAC配置 */
    /* 配置以太网MAC设置寄存器，配置基本工作模式 */
    u32Temp = HWREG32(u32Base + ETH_MACCR);
    // Clear WD, PCE, PS, TE and RE bits
    u32Temp &= 0xFF20810F;
    u32Temp |= (uint32)(0x00000000 |    //  不关闭看门狗
                        0x00000000 |    //  不关闭啰嗦定时器
                        0x00000000 |    //  设置两个帧之间的最短间隙为96位的时间
                        0x00000000 |    //  不关闭载波侦听功能
                        0x00000000 |    //　不关闭自接受功能，在全双工模式下无意义
                        0x00000000 |    //  不开启自循环模式
                        0x00000000 |    //  尝试发送尝试功能，发送冲突后再延时后重发
                        0x00000000 |    //  MAC转发所有的帧，不改变帧的内容，无论帧长度是否小于1500字节
                        0x00000000 |    //  设置退后限制
                        0x00000000      //  MAC关闭顺延检验
                        );
#if (TCPIP_CHECKSUM_EN > 0)
    u32Temp |= 0x00000400;                      //  如果使能了TCP/IP的硬件校验和功能，则配置
#endif

    u32Temp |= u32AutoValue;

    HWREG32(u32Base + ETH_MACCR) = u32Temp;    //  写回配置结果

    /* 滤波模式设置，即以太网控制器如何过滤接受数据的MAC地址 */
    HWREG32(u32Base + ETH_MACFFR) = (0x00000000 |   //  禁止接受全部功能，即采用MAC地址过滤
                                 0x00000000 |   //  设置HASH或完美过滤器位为0，这样就不采用完美过滤
                                 0x00000000 |   //  源地址过滤禁能
                                 0x00000040 |   //  MAC转发所有的控制帧给应用程序
                                 0x00000000 |   //  运行所有的广播帧通过
                                 0x00000010 |   //  接受所有的多播帧，如果需要支持IPv6，就必须配置该项
                                 0x00000000 |   //  目的地址过滤器正常工作
                                 0x00000000 |   //  多播HASH设置，即寄存器的HM位
                                 0x00000000 |   //  单播HASH设置，即寄存器的HU位
                                 0x00000000     //  混杂模式设置，使能接受过滤
                                );
    /* 设置HASH列表寄存器 */
    HWREG32(u32Base + ETH_MACHTHR) = 0x00000000;
    HWREG32(u32Base + ETH_MACHTLR) = 0x00000000;

    /* 设置以太网MAC流控寄存器 */
    u32Temp = HWREG32(u32Base + ETH_MACFCR);
    u32Temp &= 0x0000FF41;                       //  清空必要的位
    u32Temp |= ((0x00 << 16) |                   //  PAUSE时间
               0x00000080   |                   //  关闭自动零值PAUSE帧的自动生成
               0x00000000   |                   //  Pause时间为4时间间隙
               0x00000000   |                   //  单播Pause帧检测
               0x00000000   |                   //  关闭接受流控
               0x00000000                       //  关闭发送流控
                );
    HWREG32(u32Base + ETH_MACFCR) = u32Temp;

    /* 设置VLAN  */
    HWREG32(u32Base + ETH_MACVLANTR) = 0x00000000;

    /* DMA配置，配置DMA的控制器的工作模式 */
    /* 以太网DMA OMR寄存器配置 */
    u32Temp = HWREG32(u32Base + ETH_DMAOMR);
    u32Temp &= 0xF8DE3F23;                       //  清除必要的位，重新设置
    u32Temp |= (0x00000000 |                     //  丢弃所有TCP/IP的CRC校验错误的帧
               0x02000000 |                     //  设置接受存储转发
               0x00000000 |                     //  清空接受帧，以处理FIFO溢出
               0x00200000 |                     //  设置发送存储转发
               0x00000000 |                     //  发送阀值控制，只有TSF位为0时有效，这里无效
               0x00000000 |                     //  丢弃错误的帧
               0x00000000 |                     //  丢弃长度不够的帧，即丢弃过短帧
               0x00000000 |                     //  接受阀值控制，只有在RSF为0时有效，这里无效
               0x00000004                       //  允许发送下一帧
                );
    HWREG32(u32Base + ETH_DMAOMR) = u32Temp;

    /*  设置DMA BMR寄存器 */
    HWREG32(u32Base + ETH_DMABMR) =(0x02000000 |   //  传输地址对齐
                                    0x00010000 |   //  固定长度突发
                                    0x00400000 |   //  /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                                    0x00002000 |
                                   (0x00000000 << 2) |   //  描述符跳跃长度
                                    0x00004000 |   //  DMA仲裁设置
                                    0x00800000     // Use separate PBL
                                );
#if (ENHANCED_DMA_DESCRIPTORS_EN > 0)
    /* Enable the Enhanced DMA descriptors */
    HWREG32(u32Base + ETH_DMABMR)  |= (uint32)0x00000080;
#endif /* ENHANCED_DMA_DESCRIPTORS_EN */

#if 0
    // 开启PHY中断，自协商完成中断
    u32Temp = EthernetPHYRead(u32Base, PHYAddress, 30);
    u32Temp |= 0x0040;
    EthernetPHYWrite(u32Base, PHYAddress, 30, u32Temp);
#endif
}

/*********************************************************************************************************
** Function name:       EthernetPHYIntISR
** Descriptions:        PHY中断中调用该函数，重新配置PHY和MAC
** input parameters:    u32Base:        以太网模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetPHYIntISR(uint32 u32Base)
{
    uint32 u32Temp;
    uint32 u32PhyRegValue;
    uint32 u32AutoValue;

    u32Temp = HWREG32(u32Base + ETH_MACCR);

    // 清楚10M/100M标志和半双工、全双工的标志位
    u32Temp &= ~((uint32)0x00004800);

     //  读取中断状态
    u32PhyRegValue = EthernetPHYRead(u32Base, PHYAddress, 29);
    // 如果自协商完成中断产生
    if((u32PhyRegValue & 0x0040)) {
//      while(!((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0004)));
//      while(!((EthernetPHYRead(u32Base, PHYAddress, 1) & 0x0020)));
        //  读取自协商的结果
        u32PhyRegValue = EthernetPHYRead(u32Base, PHYAddress, 0);

        if (u32PhyRegValue & 0x0100) {
            //  全双工通信模式
            u32AutoValue = 0x00000800;
        } else {
            //  半双工通信模式
            u32AutoValue = 0x00000000;
        }

        if (u32PhyRegValue & 0x2000) {
            //  100M以太网通信模式
            u32AutoValue |= 0x00004000;
        } else {
            //  10M以太网通信模式
            u32AutoValue |= 0x00000000;
        }

        u32Temp |= u32AutoValue;
        HWREG32(u32Base + ETH_MACCR) = u32Temp;
    }
}
/*********************************************************************************************************
** Function name:       EthernetPHYRead
** Descriptions:        读取指定地址的PHY寄存器
** input parameters:    u32Base:        以太网模块基地址
**                      u8PHYDevAddr:   PHY器件的地址
**                      u8PHYRegAddr:   PHY寄存器的地址
** output parameters:   NONE
** Returned value:      读取的PHY寄存器的数据
*********************************************************************************************************/
uint32 EthernetPHYRead(uint32 u32Base, uint8 u8PHYDevAddr, uint8 u8PHYRegAddr)
{
    uint32 u32Temp;

    //  等待MAC MII地址寄存器和MAC MII数据寄存器可写
    while(HWREG32(u32Base + ETH_MACMIIAR) & 0x01);

    u32Temp = HWREG32(u32Base + ETH_MACMIIAR);
    u32Temp &= ~(0xFFFFFFE3);                               // 仅仅保持CR[2:0]，其它位清零
    u32Temp |= (((uint32)u8PHYDevAddr << 11) & 0x0000F800); //  写PHY器件地址
    u32Temp |= (((uint32)u8PHYRegAddr << 6)  & 0x000007C0); //  写PHY寄存器地址
    u32Temp &= ~(0x00000002);                               //  设置为读PHY寄存器模式
    u32Temp |= 0x00000001;                                  //  设置MII忙，开始读操作
    HWREG32(u32Base + ETH_MACMIIAR) = u32Temp;

    //  等读PHY寄存器操作完成
    while(HWREG32(u32Base + ETH_MACMIIAR) & 0x01);
    return (HWREG32(u32Base + ETH_MACMIIDR) & 0xFFFF);
}

/*********************************************************************************************************
** Function name:       EthernetPHYRead
** Descriptions:        读取指定地址的PHY寄存器
** input parameters:    u32Base:        以太网模块基地址
**                      u8PHYDevAddr:   PHY器件的地址
**                      u8PHYRegAddr:   PHY寄存器的地址
**                      u32Data:        写入指定PHY寄存器的数据
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetPHYWrite(uint32 u32Base, uint8 u8PHYDevAddr, uint8 u8PHYRegAddr, uint32 u32Data)
{
    uint32 u32Temp;

    //  等待MAC MII地址寄存器和MAC MII数据寄存器可写
    while(HWREG32(u32Base + ETH_MACMIIAR) & 0x01);

    u32Temp = HWREG32(u32Base + ETH_MACMIIAR);
    u32Temp &= ~(0xFFFFFFE3);                               // 仅仅保持CR[2:0]，其它位清零
    u32Temp |= (((uint32)u8PHYDevAddr << 11) & 0x0000F800); //  写PHY器件地址
    u32Temp |= (((uint32)u8PHYRegAddr << 6)  & 0x000007C0); //  写PHY寄存器地址
    u32Temp |= 0x00000002;                                  //  设置为读PHY寄存器模式
    u32Temp |= 0x00000001;                                  //  设置MII忙，开始读操作
    HWREG32(u32Base + ETH_MACMIIDR) = u32Data;
    HWREG32(u32Base + ETH_MACMIIAR) = u32Temp;

    //  等读PHY寄存器操作完成
    while(HWREG32(u32Base + ETH_MACMIIAR) & 0x01);
}

/*********************************************************************************************************
** Function name:       EthernetEnable
** Descriptions:        使能以太网控制器，包括发送和接受等
** input parameters:    u32Base      以太网模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetEnable(uint32 u32Base)
{
    HWREG32(u32Base + ETH_MACCR) |= 0x00000008;     //  使能MAC发送状态机
    HWREG32(u32Base + ETH_DMAOMR) |= 0x00100000;    //  清空发送FIFO
    HWREG32(u32Base + ETH_MACCR) |= 0x00000004;     //  使能MAC接受状态机
    HWREG32(u32Base + ETH_DMAOMR) |= 0x00002000;    //  开启DMA传输
    HWREG32(u32Base + ETH_DMAOMR) |= 0x00000002;    //  开启DMA接收
}

/*********************************************************************************************************
** Function name:       EthernetMACAddrSet
** Descriptions:        设置以太网的MAC地址，这里仅仅使用MAC0，MAC1-MAC3不使用
** input parameters:    u32Base      以太网模块基地址
**                      pu8MacAddr  需要设定的MAC地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetMACAddrSet(uint32 u32Base, uint8 *pu8MacAddr)
{
    uint32 u32Temp;

    u32Temp = (((uint32)pu8MacAddr[5] << 8) | (uint32)pu8MacAddr[4]);
    HWREG32(u32Base + ETH_MACA0HR) = u32Temp;

    u32Temp = (((uint32)pu8MacAddr[3] << 24) | ((uint32)pu8MacAddr[2] << 16) |
             ((uint32)pu8MacAddr[1] << 8) | (uint32)pu8MacAddr[0]);

    HWREG32(u32Base + ETH_MACA0LR) = u32Temp;
}

/*********************************************************************************************************
** Function name:       EthernetMACAddrGet
** Descriptions:        获取以太网的MAC地址，这里仅仅使用MAC0，MAC1-MAC3不使用
** input parameters:    u32Base      以太网模块基地址
**                      pu8MacAddr  获取的MAC地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetMACAddrGet(uint32 u32Base, uint8 *pu8MacAddr)
{
    uint32 u32Temp;

    u32Temp = HWREG32(u32Base + ETH_MACA0HR);
    pu8MacAddr[5] = ((u32Temp >> 8) & (uint8)0xFF);
    pu8MacAddr[4] = (u32Temp & (uint8)0xFF);

    u32Temp = HWREG32(u32Base + ETH_MACA0LR);
    pu8MacAddr[3] = ((u32Temp >> 24) & (uint8)0xFF);
    pu8MacAddr[2] = ((u32Temp >> 16) & (uint8)0xFF);
    pu8MacAddr[1] = ((u32Temp >> 8 ) & (uint8)0xFF);
    pu8MacAddr[0] = (u32Temp & (uint8)0xFF);
}

/*********************************************************************************************************
** Function name:       EthernetReset
** Descriptions:        软件复位以太网控制器
** input parameters:    u32Base          以太网模块基地址
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetReset(uint32 u32Base)
{
    HWREG32(u32Base + ETH_DMABMR) |= 0x00000001;
}
/*********************************************************************************************************
** Function name:       EthernetResetStatusGet
** Descriptions:        判断以太网控制器是否处于复位状态
** input parameters:    u32Base          以太网模块基地址
** output parameters:   NONE
** Returned value:      true:  以太网控制器处于复位状态
**                      false：以太网控制器复位完成
*********************************************************************************************************/
boolean EthernetResetStatusGet(uint32 u32Base)
{
    if((HWREG32(u32Base + ETH_DMABMR)) & 0x00000001)
        return true;
    else
        return false;
}
/*********************************************************************************************************
** Function name:       EthernetDMAIntEnable
** Descriptions:        使能以太网控制器的DMA中断
** input parameters:    u32Base          以太网模块基地址
**                      u32Config        使能的中断源，可以取下列值或下列值的或
**                      DMA_INTRUPT_NISE    //  正常中断汇总使能
**                      DMA_INTRUPT_AISE    //  异常中断汇总使能
**                      DMA_INTRUPT_ERS     //  早接受中断使能
**                      DMA_INTRUPT_FBES    //  总线致命错误中断使能
**                      DMA_INTRUPT_ETIE    //  早发送中断使能
**                      DMA_INTRUPT_RWITE   //  接受看门狗超时中断使能
**                      DMA_INTRUPT_RPSIE   //  接受流程停止中断使能
**                      DMA_INTRUPT_RBUIE   //  接受缓存不可用中断使能
**                      DMA_INTRUPT_RIE     //  接受中断使能
**                      DMA_INTRUPT_TUIE    //  发送下益中断使能
**                      DMA_INTRUPT_ROIE    //  接受益处中断使能
**                      DMA_INTRUPT_TITIE   //  发送啰嗦超时中断使能
**                      DMA_INTRUPT_TBUIE   //  发送缓存不可用中断使能
**                      DMA_INTRUPT_TPSIE   //  发送流程停止中断使能
**                      DMA_INTRUPT_TIE     //  发送中断使能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetDMAIntEnable(uint32 u32Base, uint32 u32Config)
{
    HWREG32(u32Base + ETH_DMAIER) |= u32Config;
}

/*********************************************************************************************************
** Function name:       EthernetDMAIntDisable
** Descriptions:        禁能以太网控制器的DMA中断
** input parameters:    u32Base          以太网模块基地址
**                      u32Config        禁能的中断源，可以取下列值或下列值的或
**                      DMA_INTRUPT_NISE    //  正常中断汇总使能
**                      DMA_INTRUPT_AISE    //  异常中断汇总使能
**                      DMA_INTRUPT_ERS     //  早接受中断使能
**                      DMA_INTRUPT_FBES    //  总线致命错误中断使能
**                      DMA_INTRUPT_ETIE    //  早发送中断使能
**                      DMA_INTRUPT_RWITE   //  接受看门狗超时中断使能
**                      DMA_INTRUPT_RPSIE   //  接受流程停止中断使能
**                      DMA_INTRUPT_RBUIE   //  接受缓存不可用中断使能
**                      DMA_INTRUPT_RIE     //  接受中断使能
**                      DMA_INTRUPT_TUIE    //  发送下益中断使能
**                      DMA_INTRUPT_ROIE    //  接受益处中断使能
**                      DMA_INTRUPT_TITIE   //  发送啰嗦超时中断使能
**                      DMA_INTRUPT_TBUIE   //  发送缓存不可用中断使能
**                      DMA_INTRUPT_TPSIE   //  发送流程停止中断使能
**                      DMA_INTRUPT_TIE     //  发送中断使能
** output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
void EthernetDMAIntDisable(uint32 u32Base, uint32 u32Config)
{
    HWREG32(u32Base + ETH_DMAIER) &= ~u32Config;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
