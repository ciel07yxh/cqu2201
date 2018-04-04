/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           at86rf231_drv.c
** Last modified Date:  2013-09-12
** Last Version:        V1.0
** Description:         AT86RF231的射频驱动程序, 工作在扩展操作模式
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2013-09-12
** Version:             V1.0
** Descriptions:        The original version 初始版本
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "at86rf21x_drv.h"
#include "contiki.h"
#include "contiki-net.h"
#include "contiki-conf.h"

#include "services/sys_services.h"

/*********************************************************************************************************
** 是否使能调试功能
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#include <stdio.h>
//#define PRINTF(...)   uart_printf(__VA_ARGS__)
#define PRINTF(...)     printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*********************************************************************************************************
  用到的宏函数定义
*********************************************************************************************************/
#define BUSYWAIT_UNTIL(cond, max_time)                                  \
  do {                                                                  \
    rtimer_clock_t t0;                                                  \
    t0 = RTIMER_NOW();                                                  \
    while(!(cond) && RTIMER_CLOCK_LT(RTIMER_NOW(), t0 + (max_time)));   \
  } while(0)

/*********************************************************************************************************
  函数声明
*********************************************************************************************************/
static void rf_spi_sel_enable(void);
static void rf_spi_sel_disable(void);
static void rf_slp_tr_high(void);
static void rf_slp_tr_low(void);
static int rf_slp_tr_state(void);
static void rf_wait_idle(void);
static void rf_reset_state_machine(void);

static void at86rf231_isr(void);
static int channel_clear(void);
static int on(void);
static int off(void);
static uint8 * u8SndBuffer = NULL;
PROCESS(at86rf231_process, "AT86RF231 driver");

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
tAt86RFInfo __GtAt86RF231_Drv = {
  &__GtSpiBusDriver,            // 定义SPI总线适配器选择
  rf_spi_sel_enable,            // spi设备的片选使能
  rf_spi_sel_disable,           // spi设备的片选禁止
  rf_slp_tr_high,               // 引脚SLP_TR设置为高
  rf_slp_tr_low,                // 引脚SLP_TR设置为低
  rf_slp_tr_state               // 引脚SLP_TR当前电平状态
};

// 模块状态，标志是否有数据接收到
static uint8_t bIsReceive = 0;
// 定义模块是否打开标志变量
static uint8_t radio_receive_on = 0;

/*********************************************************************************************************
** Function name:       rf_spi_sel_enable
** Descriptions:        使能SPI总线
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void rf_spi_sel_enable(void)
{
    RF_SPI_SEL_L();
}

/*********************************************************************************************************
** Function name:       rf_spi_sel_disable
** Descriptions:        禁止SPI总线
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void rf_spi_sel_disable(void)
{
    RF_SPI_SEL_H();
}

/*********************************************************************************************************
** Function name:       rf_slp_tr_high
** Descriptions:        slp_tr引脚设置为高
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void rf_slp_tr_high(void)
{
    RF_SLP_TR_H();
}

/*********************************************************************************************************
** Function name:       rf_slp_tr_low
** Descriptions:        slp_tr引脚设置为低
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void rf_slp_tr_low(void)
{
    RF_SLP_TR_L();
}


/*********************************************************************************************************
** Function name:       rf_slp_tr_state
** Descriptions:        slp_tr引脚当前状态
** Input parameters:    无
** Output parameters:   无
** Returned value:      1
*********************************************************************************************************/
static int rf_slp_tr_state(void)
{
    return RF_SLP_TR_STATE();
}

/*********************************************************************************************************
** Function name:       trx_reg_read
** Descriptions:        读一个寄存器的值
** Input parameters:    addr:     待读取的寄存器地址
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint8 trx_reg_read(uint8 addr)
{
    uint8 u8Rtn;
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
    
    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    ptRFInfo->spi_sel_enable();

    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, (addr & CMD_REG_MASK) | CMD_REG);
    u8Rtn = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);

    ptRFInfo->spi_sel_disable();
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);

    return u8Rtn;
 
}

/*********************************************************************************************************
** Function name:       trx_reg_write
** Descriptions:        写一个寄存器的值
** Input parameters:    addr:     指定写的寄存器地址
**                      u8Value:  待写入的值
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void trx_reg_write(uint8 addr, uint8 u8Value)
{
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
    
    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    ptRFInfo->spi_sel_enable();

    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, (addr & CMD_REG_MASK) | CMD_REG | CMD_WRITE);
    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, u8Value);

    ptRFInfo->spi_sel_disable();
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
}

/*********************************************************************************************************
** Function name:       trx_bit_read
** Descriptions:        读一个寄存器的一位
** Input parameters:    addr:   待读取的寄存器地址
**                      mask:   寄存器屏蔽值
**                      pos：   读取的位的位置
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint8 trx_bit_read(uint8 addr, uint8 mask, uint8 pos)
{
    uint8 u8Rtn;
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
    
    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    ptRFInfo->spi_sel_enable();

    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, (addr & CMD_REG_MASK) | CMD_REG);
    u8Rtn = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);

    ptRFInfo->spi_sel_disable();

    u8Rtn &= mask;

    u8Rtn >>= pos;
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);

    return u8Rtn;
 
}

/*********************************************************************************************************
** Function name:       trx_bit_write
** Descriptions:        写一个寄存器的一位
** Input parameters:    addr:   待写的寄存器地址
**                      mask:   寄存器屏蔽值
**                      pos：   读取的位的位置
**                      u8Value: 待写入的值
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void trx_bit_write(uint8 addr, uint8 mask, uint8 pos, uint8 u8Value)
{
    uint8 u8Rtn;
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;

    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    // 先读该寄存器
    ptRFInfo->spi_sel_enable();

    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, (addr & CMD_REG_MASK) | CMD_REG);
    u8Rtn = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);

    ptRFInfo->spi_sel_disable();
    
    // 处理器该寄存器的位
    u8Rtn &= ~mask;
    u8Rtn |= ((u8Value << pos) & mask);

    // 将修改好位的值，写回寄存器
    ptRFInfo->spi_sel_enable();

    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, (addr & CMD_REG_MASK) | CMD_REG | CMD_WRITE);
    u8Rtn = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, u8Rtn);

    ptRFInfo->spi_sel_disable();
    
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);

}

/*********************************************************************************************************
** Function name:       trx_state_get
** Descriptions:        在FIFO中读取一帧数据
** Input parameters:    无
** Output parameters:   无
** Returned value:      获取到的状态
*********************************************************************************************************/
static uint8 trx_state_get(void)
{
  return(trx_bit_read(SR_TRX_STATUS));
}

/*********************************************************************************************************
** Function name:       trx_state_set
** Descriptions:        设置工作状态
** Input parameters:    lowpanif: lowpan网络属性结构
**                      new_state:   设置的状态
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void trx_state_set(uint8 new_state)
{
    uint8_t desired_status;
    uint8_t temp;
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
     
    if(ptRFInfo->dev_slp_tr_state()) {
        return;
    }
    
    /* Wait for radio to finish previous operation */
    rf_wait_idle();
    
    if (new_state == STATE_FORCE_TX_ON)
        desired_status = STATE_TX_ON;
    else if (new_state == STATE_FORCE_TRX_OFF)
        desired_status = STATE_TRX_OFF;
    else
        desired_status = new_state;

    // 如果和当前状态一致，直接返回
    temp = trx_state_get();
    if(desired_status == temp) {
        return;
    }

    // 写入待设置的状态
    trx_bit_write(SR_TRX_CMD, desired_status);
    rf_wait_idle();
    temp = trx_state_get();
    //PRINTF("The current state is 0x%02X\r\n", temp);
}

/*********************************************************************************************************
** Function name:       trx_rand_get
** Descriptions:        根据射频收发器产生随机数
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint16 trx_rand_get(void)
{
  uint16 rnd = 0;
  uint8 i, rndValue, state;

  trx_reg_write(RG_IRQ_MASK, 0x00);
  state = trx_state_get();
  trx_state_set(STATE_RX_ON);
  for (i = 0; i < 16; i += 2)
  {
    BUSYWAIT_UNTIL(0, RTIMER_SECOND / 10000);
    rndValue = trx_bit_read(SR_RND_VALUE) & 3;
    rnd |= rndValue << i;
  }

  trx_state_set(state);

  trx_reg_read(RG_IRQ_STATUS);
  trx_reg_write(RG_IRQ_MASK, IRQ_TRX_END);

  return rnd;
}

/*********************************************************************************************************
** Function name:       trx_frame_write
** Descriptions:        向FIFO中写入一帧的数据
** Input parameters:    length:  帧长度
**                      frame:   侦数据地址
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void trx_frame_write(uint8 length, uint8 *frame)
{
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;

    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    ptRFInfo->spi_sel_enable();
    // 发送写frame标志
    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, CMD_FB | CMD_WRITE);
    // 发送帧长度
    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, length + 2 /*crc*/);
    // 发送帧内容
    while(length--){
        ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, *frame++);
    }
    ptRFInfo->spi_sel_disable();
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
}

/*********************************************************************************************************
** Function name:       trx_frame_read
** Descriptions:        在FIFO中读取一帧数据
** Input parameters:    frame:   侦数据地址
** Output parameters:   无
** Returned value:      读取到的侦长度
*********************************************************************************************************/
static uint8 trx_frame_read(uint8 *frame, uint8 *lqi)
{
    uint8 u8Rtn, i;
    tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
    
    ptRFInfo->spi_bus->spi_bus_lock(ptRFInfo->spi_bus);
    ptRFInfo->spi_sel_enable();
    // 发送读frame标志
    ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, CMD_FB);
//    // 如果接收FIFO为空
//    if(RF_IRQ_STATE()) {
//       ptRFInfo->spi_sel_disable();
//       ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
//       RIMESTATS_ADD(tooshort);
//       return 0;
//    }
    // 读取帧长度
    u8Rtn = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);
    if(u8Rtn <= 3) 
    {
       ptRFInfo->spi_sel_disable();
       ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
       RIMESTATS_ADD(tooshort);
       return 0;
    }

    if((u8Rtn - 3) > 127) {
       ptRFInfo->spi_sel_disable();
       ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
       RIMESTATS_ADD(toolong);
       return 0;
    }
    
    // 读取帧内容
    u8Rtn -= 2;
    for(i=0; i<u8Rtn; i++) 
    {
        *frame++ = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);
    }
    
    // 读取最后两个字节的CRC
    i = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);
    i = ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);
    
    // 读取LQI的值
    *lqi =  ptRFInfo->spi_bus->spi_bus_send_recv_byte(ptRFInfo->spi_bus, 0xFF);
    ptRFInfo->spi_sel_disable();
    
    ptRFInfo->spi_bus->spi_bus_unlock(ptRFInfo->spi_bus);
    //　返回值去掉crc的长度
    return (u8Rtn);
}
/*********************************************************************************************************
** Function name:       rf_is_idle
** Descriptions:        判断射频是否空闲
** Input parameters:    无
** Output parameters:   无
** Returned value:      1
*********************************************************************************************************/
static char rf_is_idle(void)
{
    uint8 radio_state;
    tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
    
    if(ptRFInfo->dev_slp_tr_state()) {
      return 1;
    } else {
      radio_state = trx_state_get();
      if (radio_state != STATE_BUSY_TX_ARET &&
          radio_state != STATE_BUSY_RX_AACK &&
          radio_state != STATE_TRANSITION_IN_PROGRESS &&
          radio_state != STATE_BUSY_RX && 
          radio_state != STATE_BUSY_TX) {
        return(1);
      } else {
        return(0);
      }
    }
}
/*********************************************************************************************************
** Function name:       rf_wait_idle
** Descriptions:        等待射频空闲
** Input parameters:    无
** Output parameters:   无
** Returned value:      1
*********************************************************************************************************/
static void rf_wait_idle(void)
{
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
  
  /* TX_ARET with multiple csma retries can take a very long time to finish */
  while (1) {
    if(ptRFInfo->dev_slp_tr_state()) {
      return;
    }
    if (rf_is_idle()) break;
  }
}

/*********************************************************************************************************
** Function name:       rf_reset_state_machine
** Descriptions:        Reset the state machine (to TRX_OFF) from any of ts states, except for the SLEEP state.
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void rf_reset_state_machine(void)
{
    tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
    
      /* If radio is off (slptr high), turn it on */
    if(ptRFInfo->dev_slp_tr_state()) {
      ptRFInfo->dev_slp_tr_low();
      BUSYWAIT_UNTIL(0, RTIMER_SECOND * TIME_SLEEP_TO_TRX_OFF / 1000000);
    }

    trx_state_set(STATE_FORCE_TRX_OFF);
    BUSYWAIT_UNTIL(0, RTIMER_SECOND * TIME_PLL_ON_TO_TRX_OFF / 1000000);
}
// 下面开始是RF的网络驱动函数，对应的结构可以参看"core/dev/radio.h"
/*********************************************************************************************************
** Function name:       init
** Descriptions:        初始化
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int init(void)
{
   uint32 temp;
   tAt86RFInfo *ptRFInfo = &__GtAt86RF231_Drv;
   
    /*
    ** Step 1, 延时，等待芯片正常上电
    */
   BUSYWAIT_UNTIL(0, RTIMER_SECOND * TIME_TO_ENTER_P_ON / 1000000);
   
   /*
    ** Step 2, 初始化外设接口
    */
   SystemPeripheralEnable(RF_CTRL_PERIPHERAL1);
   SystemPeripheralEnable(RF_CTRL_PERIPHERAL2);
   SystemPeripheralEnable(RF_CTRL_PERIPHERAL3);
   SystemPeripheralEnable(RF_CTRL_PERIPHERAL4);
   
   GPIOConfigSet(RF_RSTN_BASE, RF_RSTN_PIN, GPIO_OUT_PP | GPIO_SPEED_50MHZ);
   GPIOConfigSet(RF_SPI_SEL_BASE, RF_SPI_SEL_PIN, GPIO_OUT_PP | GPIO_SPEED_50MHZ);
   GPIOConfigSet(RF_SLP_TR_BASE, RF_SLP_TR_PIN, GPIO_OUT_PP | GPIO_SPEED_50MHZ);

   // RF DIG2设置为输入，用来检查帧起始标志
   GPIOConfigSet(RF_DIG2_BASE, RF_DIG2_PIN, GPIO_DIR_IN_Floating);
   
   RF_SPI_SEL_H();

   // 初始化中断引脚
   GPIOConfigSet(RF_IRQ_BASE, RF_IRQ_PIN, GPIO_DIR_IN_Floating); \
   AFIOExtiLineConfig(RF_IRQ_LINE_PORT, RF_IRQ_LINE_NO);
   ExtiIntEventClear(RF_IRQ_LINE_NO);
   ExtiIntEventFallingDisable(RF_IRQ_LINE_NO);
   ExtiIntEventRisingEnable(RF_IRQ_LINE_NO);
   ExtiIntEnable(RF_IRQ_LINE_NO);
   hal_int_register(RF_IRQ_INT_NUM, at86rf231_isr);
   hal_int_priority_set(RF_IRQ_INT_NUM, (0x03 << 4));
   hal_int_enable(RF_IRQ_INT_NUM);

    /*
    ** Step 3, 复位模块，之后使模块进入TRX_OFF状态
    */
   RF_RSTN_L();
   ptRFInfo->dev_slp_tr_low();
   BUSYWAIT_UNTIL(0, RTIMER_SECOND *  2 * TIME_SLEEP_TO_TRX_OFF / 1000000);
   RF_RSTN_H();
    /* Force transition to TRX_OFF */
   trx_state_set(STATE_FORCE_TRX_OFF);
   BUSYWAIT_UNTIL(0, RTIMER_SECOND * TIME_PLL_ON_TO_TRX_OFF / 1000000);
   
    /*
    ** Step 4, 读射频芯片ID
    */
   temp = 0;
   temp = trx_reg_read(RG_PART_NUM);
   temp |= (((uint32)trx_reg_read(RG_VERSION_NUM)) << 8);
   temp |= (((uint32)trx_reg_read(RG_MAN_ID_0)) << 16);
   temp |= (((uint32)trx_reg_read(RG_MAN_ID_1)) << 24);
   
read_id_again:
   switch(temp){
     case 0x001F0203:
       PRINTF("The Device ID is 0x%08X, it is AT86RF231\r\n", temp);
       break;
     case 0x001F010B:
       PRINTF("The Device ID is 0x%08X, it is AT86RF233\r\n", temp);
       break;
     case 0x001F0107:
       PRINTF("The Device ID is 0x%08X, it is AT86RF212\r\n", temp);
       break;
     case 0x001F0307:
       PRINTF("The Device ID is 0x%08X, it is AT86RF212B\r\n", temp);
       break;
     default:
       PRINTF("The Device ID is 0x%08X, it isn't support!\r\n", temp);
       goto read_id_again;
   }
   
    /*
    ** Step 5, 配置中国频段（780M-786M）
    */   
    trx_bit_write(SR_PHY_MODE, 0x1c);
    trx_bit_write(SR_GC_TX_OFFS, 2);
    trx_bit_write(SR_CC_BAND, 4);
    
    /*
    ** Step 5, 中断配置, 中断设置为高电平，上升沿有效
    */   
//    trx_bit_write(SR_IRQ_POLARITY, 1); // 如果这配置，则为低电平有效
    trx_bit_write(SR_IRQ_2_EXT_EN, 1);
    trx_reg_write(RG_IRQ_MASK, IRQ_TRX_END /*| IRQ_RX_START */);
    // 不设置SR_IRQ_MASK_MODE， RG_IRQ_STATUS仅仅显示使能的中断状态
    trx_bit_write(SR_IRQ_MASK_MODE, 0);
    trx_reg_read(RG_IRQ_STATUS);
   
   

    /*
    ** Step 6, 扩展工作模式配置
    */  
    trx_bit_write(SR_MAX_FRAME_RETRIES, 3);
    trx_bit_write(SR_MAX_CSMA_RETRIES, 5);
    temp = trx_rand_get();
    trx_bit_write(SR_CSMA_SEED_1, ((temp >> 8) & 0x07));
    trx_reg_write(RG_CSMA_SEED_0, (temp & 0xff));
    trx_bit_write(SR_MIN_BE, 0);
    trx_bit_write(SR_MAX_BE, 8);
   
    /*
    ** Step 7, 接收灵敏度设置, 
    ** rxpdtlvl 	This parameter controls the receiver sensitivity threshold.
    ** {0} : The radio transceiver operates with maximum receiver sensitivity and receives 
    **       all frames at and below RSSI_BASE_VAL.
    ** {1,...,15} : The radio transceiver operates with reduced receiver sensitivity and 
    **       ignores all frames with a RSSI value, below rxthres.
    ** The receiver sensitivity threshold rxthres is computed according to this formula:
    ** rxthres = RSSI_BASE_VAL + rxpdtlvl * 3
    */
    //trx_bit_write(SR_RX_PDT_LEVEL, 0);
   
    /*
    ** Step 8, 自动产生CRC配置以及收发指示LED灯控制
    */
    trx_bit_write(SR_TX_AUTO_CRC_ON, 1);
    trx_bit_write(SR_PA_EXT_EN, 1);
   
   
    /*
    ** Step 9, 设置射频通信波特率， 可以设置为下列值
    ** 0--------->250Kbps(IEEE 802.15.4默认)
    ** 1--------->500Kbps(芯片扩展支持)
    ** 2--------->1000Kbps(芯片扩展支持)
    ** 3--------->2000Kbps(芯片扩展支持)
    */  
    //trx_bit_write( SR_OQPSK_DATA_RATE, 3);
   
/*
    ** Step 10, 发送功率设置
    **  AT86RF212 & AT86RF212B
    **  0xC1 --------->11dBm
    **  0xE3 --------->10dBm
    **  0xE4 --------->9dBm
    **  0xC5 --------->8dBm
    **  0xE7 --------->7dBm
    **  0xE8 --------->6dBm
    **  0xE9 --------->5dBm
    **  0xEA --------->4dBm
    **  0xCB --------->3dBm
    **  0xCC --------->2dBm
    **  0xCD --------->1dBm
    **  0xCE --------->0dBm
    **  0xCF --------->-1dBm
    **  0xAF --------->-2dBm
    **  0x26 --------->-3dBm
    **  0x27 --------->-4dBm
    **  0x28 --------->-5dBm
    **  0x29 --------->-6dBm
    **  0x07 --------->-7dBm
    **  0x08 --------->-8dBm
    **  0x09 --------->-9dBm
    **  0x0A --------->-10dBm
    **  0x0B --------->-11dBm
    **  0x0C --------->-12dBm
    **  0x0D --------->-13dBm
    **  0x0E --------->-14dBm
    **  0x0F --------->-15dBm
    **  0x10 --------->-16dBm
    **  0x11 --------->-17dBm
    **  0x13 --------->-18dBm
    **  0x14 --------->-19dBm
    **  0x15 --------->-20dBm
    **  0x16 --------->-21dBm
    **  0x17 --------->-22dBm
    **  0x18 --------->-23dBm
    **  0x19 --------->-24dBm
    **  0x1A --------->-25dBm
    */
     trx_reg_write( RG_PHY_TX_PWR, 0xc1);
     PRINTF("The RF transmit power is setting with register value %d!\r\n", trx_bit_read( SR_TX_PWR));
   
    /*
    ** Step 11, 通信信道设置，780G的只能去1-4的值; 通信地址
    */
    // 设置信道和地址
    /* Set the channel、panid and mac address. */
//    lowpanif->radio_driver->set_pan_addr(htons(lowpanif->panid), 
//                                         ((u16_t)lowpanif->addr[0] << 8) | lowpanif->addr[1],
//                                          lowpanif->addr); 
//    lowpanif->radio_driver->set_channel(
//                                         lowpanif->channel);

    /*
    ** Step 12, 接受滤波配置
    */
    trx_bit_write(SR_AACK_FVN_MODE, 2);
    trx_bit_write(SR_AACK_PROM_MODE, 1);
    trx_bit_write(SR_AACK_I_AM_COORD, 0);
    trx_bit_write(SR_AACK_UPLD_RES_FT, 0);
    trx_bit_write(SR_AACK_FLTR_RES_FT, 0);
    trx_bit_write(SR_AACK_DIS_ACK, 0);

    /*
    ** Step 13, CLKM配置, 配置关闭CLKM
    */
//    trx_bit_write( SR_CLKM_SHA_SEL, 0);
//    trx_bit_write( SR_CLKM_CTRL, 0);

    /*
    ** Step 14, 电池监控配置, 设置低压报警，低于2.3V报警，具体请参看数据手册，寄存器BATMON，地址0x11
    */
//    trx_bit_write( SR_BATMON_HR, 0);
//    trx_bit_write( SR_BATMON_VTH, 0xc);

    /*
    ** Step 15, Calibration配置
    */
    trx_bit_write( SR_PLL_DCU_START, 1);
    trx_bit_write( SR_PLL_CF_START, 1);
    BUSYWAIT_UNTIL(0, RTIMER_SECOND * 10000 / 1000000);

    /*
    ** Step 16, 　SPI命令模式配置, 可以取下列值
    ** 0: default (empty, all bits 0x00)
    ** 1: monitor RG_TRX_STATUS register (see section State Changes)
    ** 2: monitor RG_PHY_RSSI register (see section RSSI and ED Measurement)
    ** 3: monitor RG_IRQ_STATUS register (see section Interrupts)
    ** 
    */
    trx_bit_write(SR_SPI_CMD_MODE, 2);
    // 设置IRQ引脚指示接收fifo状态, 该状态只有在访问SPI时才有效，故这里不用了
   // trx_bit_write(SR_RX_BL_CTRL, 1);
    // 开启动态帧缓冲区保护功能
    trx_bit_write(SR_RX_SAFE_MODE, 1);
    
    /*
    ** Step 18, 通信信道设置，信道在contiki-net-conf.h中配置，2.4G的只能去11-26的值
    */
    trx_bit_write(SR_CHANNEL, IEEE802154_RF_CONF_CHANNEL);
    PRINTF("The RF commucation channel is %d!\r\n", trx_bit_read(SR_CHANNEL));
    
    // 启动线程
    process_start(&at86rf231_process, NULL);
    

    return 0;
}

/*********************************************************************************************************
** Function name:       prepare
** Descriptions:        准备数据发送
** Input parameters:    payload：       待准备的数据
**                      payload_len：   准备数据的长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int prepare(const void *payload, unsigned short payload_len)
{ 
  if(payload_len > 125) {
    return -1;
  }
  
//  rf_wait_idle();
//  
//   /* Prepare to transmit */
//  trx_state_set(STATE_TX_ARET_ON);
//  PRINTF("The prepare state is 0x%02X\r\n", trx_state_get());
//  trx_frame_write(payload_len, (uint8 *)payload);
  u8SndBuffer = (uint8 *)payload;
  return 0;
}

/*********************************************************************************************************
** Function name:       transmit
** Descriptions:        启动发送数据
** Input parameters:    transmit_len： 发送数据长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int transmit(unsigned short transmit_len)
{   
  uint8 enSendState;
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
   
   
  /* If radio is off (slptr high), turn it on */
  if(ptRFInfo->dev_slp_tr_state()) {
     ptRFInfo->dev_slp_tr_low();
     BUSYWAIT_UNTIL(0, RTIMER_SECOND * 2 * TIME_SLEEP_TO_TRX_OFF / 1000000);
  }
  
  rf_wait_idle();
  
  /* Prepare to transmit */
  trx_state_set(STATE_TX_ARET_ON);
  
  PRINTF("The prepare state is 0x%02X\r\n", trx_state_get());
  
  /* Toggle the SLP_TR pin to initiate the frame transmission, then transfer
   * the frame. We have about 16 us + the on-air transmission time of 40 bits
   * (for the synchronization header) before the transceiver sends the PHR. */
  ptRFInfo->dev_slp_tr_high();
  ptRFInfo->dev_slp_tr_low();
  trx_frame_write(transmit_len, (uint8 *)u8SndBuffer);
//  trx_state_set( STATE_BUSY_TX);
  
  PRINTF("The send state is 0x%02X\r\n", trx_state_get());
  rf_wait_idle();
  PRINTF("The send over state is 0x%02X\r\n", trx_state_get());
  
   // 处理发送中断
    enSendState = trx_bit_read(SR_TRAC_STATUS);
     if(radio_receive_on ==  1) {
        // make to rx on state.
        trx_state_set(STATE_RX_AACK_ON);
     } else {
        rf_reset_state_machine();
     }
     PRINTF("The sent over state is 0x%02X\r\n", trx_state_get());
  
     // 检查发送结果
     if (enSendState == 1) {                //success, data pending from addressee
          enSendState = RADIO_TX_OK;           //handle as ordinary success
      }

      if (enSendState == 3) {        //CSMA channel access failure
          enSendState = RADIO_TX_COLLISION;
      } else if (enSendState == 5) {        //Expected ACK, none received
          enSendState = RADIO_TX_NOACK;
      } else if (enSendState == 7) {        //Invalid (Can't happen since waited for idle above?)
          enSendState = RADIO_TX_ERR;
      } 
   return enSendState;
}

/*********************************************************************************************************
** Function name:       send
** Descriptions:        发送一帧数据
** Input parameters:    payload：       待发送的数据
**                      payload_len：   发送数据的长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int send(const void *payload, unsigned short payload_len)
{
  uint8 enSendState;
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
   
   
  /* If radio is off (slptr high), turn it on */
  if(ptRFInfo->dev_slp_tr_state()) {
     ptRFInfo->dev_slp_tr_low();
     BUSYWAIT_UNTIL(0, RTIMER_SECOND * 2 * TIME_SLEEP_TO_TRX_OFF / 1000000);
  }
  
  rf_wait_idle();
  
  /* Prepare to transmit */
  trx_state_set(STATE_TX_ARET_ON);
  
  PRINTF("The prepare state is 0x%02X\r\n", trx_state_get());
  
  /* Toggle the SLP_TR pin to initiate the frame transmission, then transfer
   * the frame. We have about 16 us + the on-air transmission time of 40 bits
   * (for the synchronization header) before the transceiver sends the PHR. */
  ptRFInfo->dev_slp_tr_high();
  ptRFInfo->dev_slp_tr_low();
  trx_frame_write(payload_len, (uint8 *)payload);
//  trx_state_set( STATE_BUSY_TX);
  
  PRINTF("The send state is 0x%02X\r\n", trx_state_get());
  rf_wait_idle();
  PRINTF("The send over state is 0x%02X\r\n", trx_state_get());
  
   // 处理发送中断
    enSendState = trx_bit_read(SR_TRAC_STATUS);
     if(radio_receive_on ==  1) {
        // make to rx on state.
        trx_state_set(STATE_RX_AACK_ON);
     } else {
        rf_reset_state_machine();
     }
     PRINTF("The sent over state is 0x%02X\r\n", trx_state_get());
  
     // 检查发送结果
     if (enSendState == 1) {                //success, data pending from addressee
          enSendState = RADIO_TX_OK;           //handle as ordinary success
      }

      if (enSendState == 3) {        //CSMA channel access failure
          enSendState = RADIO_TX_COLLISION;
      } else if (enSendState == 5) {        //Expected ACK, none received
          enSendState = RADIO_TX_NOACK;
      } else if (enSendState == 7) {        //Invalid (Can't happen since waited for idle above?)
          enSendState = RADIO_TX_ERR;
      } 
   return enSendState;
}

/*********************************************************************************************************
** Function name:       read
** Descriptions:        读取一帧数据
** Input parameters:    buf：       读取数据存放缓冲区
**                      bufsize：   读取数据的长度
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int read(void *buf, unsigned short bufsize)
{
    uint8_t temp;
    uint8_t lqi;

    temp = trx_bit_read(SR_RX_CRC_VALID);
    if(!temp) {
        RIMESTATS_ADD(badcrc);
        return 0;
    }

    temp = (trx_frame_read(buf, &lqi));
    if(temp > 0) {
        //trx_bit_read(SR_RSSI);
        packetbuf_set_attr(PACKETBUF_ATTR_RSSI, trx_reg_read(RG_PHY_ED_LEVEL));
        packetbuf_set_attr(PACKETBUF_ATTR_LINK_QUALITY, lqi);
    }
    
    return temp;
}

/*********************************************************************************************************
** Function name:       channel_clear
** Descriptions:        CCA的状态检测，清除CCA, 即冲突检测
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int channel_clear(void)
{
  uint8 cca = 0;
  uint8 radio_was_off = 0;
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;

  /* Turn radio on if necessary. If radio is currently busy return busy channel */
  /* This may happen when testing radio duty cycling with RADIOALWAYSON,
   * or because a packet just started. */
  if(radio_receive_on) {
      //should not be sleeping!
      if(ptRFInfo->dev_slp_tr_state()) {
	  goto busyexit;
	} else {
          if (!rf_is_idle()) {
            goto busyexit;
          }
	}
  } else {
    radio_was_off = 1;
    on();
  }
  
  rf_wait_idle();
  trx_bit_write(SR_CCA_REQUEST, 1);
 // delay_us(TIME_CCA);
//  while(trx_bit_read(SR_CCA_DONE) == 0);
  while((cca & 0x80) == 0) {
    if(ptRFInfo->dev_slp_tr_state()) {
        break;
    }
    cca = trx_reg_read(RG_TRX_STATUS);
  }
  //cca = trx_reg_read(RG_TRX_STATUS);
  if(radio_was_off) {
    off();
  }
  if (cca & 0x40) {
    return 1;
  }
  
busyexit:
  return 0;
  
//     trx_bit_write( SR_CCA_REQUEST, 1);
//     while(trx_bit_read( SR_CCA_DONE) == 0);
//     return ((trx_bit_read( SR_CCA_STATUS)));
}

/*********************************************************************************************************
** Function name:       receiving_packet
** Descriptions:        RF是否正在接收数据
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int receiving_packet(void)
{
  uint8 radio_state;
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
  
  if(ptRFInfo->dev_slp_tr_state()) {
    return 0;
  } else {  
    radio_state = trx_state_get();
    if ((radio_state==STATE_BUSY_RX) || (radio_state==STATE_BUSY_RX_AACK)) {
      return 1;
    }
  }
  return 0;
  
    //   return (RF_FRAME_START_STATE());
}

/*********************************************************************************************************
** Function name:       pending_packet
** Descriptions:        RF是否已经接收到了数据
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int pending_packet(void)
{
   return (bIsReceive == 1);
}

/*********************************************************************************************************
** Function name:       on
** Descriptions:        打开RF
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int on(void)
{
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
  
  if(radio_receive_on) {
    return 0;
  }
  
  radio_receive_on =  1;
  
  /* If radio is off (slptr high), turn it on */
  if(ptRFInfo->dev_slp_tr_state()) {
     ptRFInfo->dev_slp_tr_low();
     BUSYWAIT_UNTIL(0, RTIMER_SECOND * 2 * TIME_SLEEP_TO_TRX_OFF / 1000000);
  }
  
  trx_state_set(STATE_RX_AACK_ON);

  return 0;
}
/*********************************************************************************************************
** Function name:       on
** Descriptions:        关闭RF
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static int off(void)
{
  tAt86RFInfo *ptRFInfo = (tAt86RFInfo *)&__GtAt86RF231_Drv;
  
  radio_receive_on = 0;
  if(ptRFInfo->dev_slp_tr_state()) {
    return 0;
  }

  /* Wait for any transmission to end */
  rf_wait_idle(); 
  
  if(ptRFInfo->dev_slp_tr_state()) {
    return 0;
  }
  
  rf_reset_state_machine();
  /* Sleep Radio */
  ptRFInfo->dev_slp_tr_high();

  return 0;
}

/*********************************************************************************************************
** Function name:       at86rf231_isr
** Descriptions:        中断处理程序
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void at86rf231_isr(void)
{
   RF_IRQ_CLEAR();
   process_poll(&at86rf231_process);
}

/*********************************************************************************************************
** Function name:       at86rf231_process
** Descriptions:        RF处理线程
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
PROCESS_THREAD(at86rf231_process, ev, data)
{
  //uint8 enSendState;
  uint8 u8IntStatus;
  uint8 u8Status;
  int len;
  PROCESS_BEGIN();

  PRINTF("at86rf231_process: started\n");

  while(1) {
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL);

    PRINTF("at86rf231_process: calling receiver callback\n");
    
    //do {
        u8IntStatus = trx_reg_read(RG_IRQ_STATUS);
        if(u8IntStatus & IRQ_TRX_END) { 
              u8Status = trx_state_get();
              if((u8Status == STATE_BUSY_RX_AACK) || (u8Status == STATE_RX_ON) || 
               (u8Status == STATE_BUSY_RX) || (u8Status == STATE_RX_AACK_ON)){
                 // 处理接收中断
                  bIsReceive = 1;
                  packetbuf_clear();
                  len = read(packetbuf_dataptr(), PACKETBUF_SIZE);
                  if(len > 0) {
                      packetbuf_set_datalen(len);
                      NETSTACK_RDC.input();
                  }
                   bIsReceive = 0;
               }  else {
                 // 处理发送中断
    //             enSendState = trx_bit_read(SR_TRAC_STATUS);
    //             if(radio_receive_on ==  1) {
    //              // make to rx on state.
    //                trx_state_set(STATE_RX_AACK_ON);
    //                rf_wait_idle();
    //             } else {
    //                rf_reset_state_machine();
    //             }
    //  
    //            // 检查发送结果
    //              if (enSendState == 1) {                //success, data pending from addressee
    //                enSendState = RADIO_TX_OK;           //handle as ordinary success
    //             }
    //
    //             if (enSendState == 3) {        //CSMA channel access failure
    //               enSendState = RADIO_TX_COLLISION;
    //             } else if (enSendState == 5) {        //Expected ACK, none received
    //               enSendState = RADIO_TX_NOACK;
    //             } else if (enSendState == 7) {        //Invalid (Can't happen since waited for idle above?)
    //               enSendState = RADIO_TX_ERR;
    //             } 
               }
        }
   // } while(RF_IRQ_STATE());
  }

  PROCESS_END();
}

/*********************************************************************************************************
** Function name:       ieee_set_pan_addr
** Descriptions:        设置IEEE地址信息
** Input parameters:    无
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ieee_set_pan_addr(uint16_t pan, uint16_t addr,const uint8_t *ieee_addr)
{
    trx_reg_write(RG_PAN_ID_0, (IEEE802154_CONF_PANID & 0x00FF));
    trx_reg_write(RG_PAN_ID_1, ((IEEE802154_CONF_PANID >> 8) & 0x00FF));
    trx_reg_write(RG_SHORT_ADDR_0, addr & 0x00ff);
    trx_reg_write(RG_SHORT_ADDR_1, (addr >> 8) & 0x00ff);
    trx_reg_write(RG_IEEE_ADDR_0, ieee_addr[7]);
    trx_reg_write(RG_IEEE_ADDR_1, ieee_addr[6]);
    trx_reg_write(RG_IEEE_ADDR_2, ieee_addr[5]);
    trx_reg_write(RG_IEEE_ADDR_3, ieee_addr[4]);
    trx_reg_write(RG_IEEE_ADDR_4, ieee_addr[3]);
    trx_reg_write(RG_IEEE_ADDR_5, ieee_addr[2]);
    trx_reg_write(RG_IEEE_ADDR_6, ieee_addr[1]);
    trx_reg_write(RG_IEEE_ADDR_7, ieee_addr[0]);
}

/*********************************************************************************************************
** Function name:       ieee_set_channel
** Descriptions:        设置IEEE通信信道
** Input parameters:    channel: 设置的通信信道
** Output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ieee_set_channel(int channel)
{
  u8_t temp;
  
  if((channel > 3)) {
     return;
  }
  
  /* CC_BNAD is 4 */
  /* Fc [MHz] = 769.0[MHz] + 1[MHz] x CC_NUMBER */
  temp = (channel * 2) + 11;
  trx_bit_write( SR_CC_NUMBER, temp);
  
   // 等待PLL锁定
   BUSYWAIT_UNTIL(0, RTIMER_SECOND / 100);
}

/*********************************************************************************************************
**  定义RF驱动结构体
*********************************************************************************************************/
const struct radio_driver at86rf21x_rf_driver =
{
    init,
    prepare,
    transmit,
    send,
    read,
    channel_clear,
    receiving_packet,
    pending_packet,
    on,
    off,
};

/*********************************************************************************************************
** End of File
*********************************************************************************************************/
