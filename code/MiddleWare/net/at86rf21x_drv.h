/****************************************Copyright (c)****************************************************
**                                蓝精灵6lowpan无线开发平台
**                                  一切为开源应用而作
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           at86rf21x_drv.h
** Last modified Date:  2013-08-30
** Last Version:        V1.0
** Description:         AT86RF231的射频驱动程序
**
**--------------------------------------------------------------------------------------------------------
** Created By:          任海波
** Created date:        2013-08-30
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
#ifndef  __AT86RF21X_DRV_H__
#define  __AT86RF21X_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "infra/rhb_type.h"
#include "devs/spi/rhb_spi_bus_adapter.h"
#include "hal/arch/inc/arch_io_memmap.h"
#include "hal/arch/inc/arch_io_reg.h"
#include "hal/arch/inc/arch_io_rcc.h"
#include "hal/arch/inc/arch_io_ints.h"
#include "hal/arch/inc/arch_io_gpio.h"
#include "hal/arch/inc/arch_io_exti.h"
  
#include "contiki.h"

/*********************************************************************************************************
  硬件连接定义
*********************************************************************************************************/
// 用到IO外设选择，用于使能GPIO外设时钟
#define RF_CTRL_PERIPHERAL1             RCC_PERIPHERAL_AFIO
#define RF_CTRL_PERIPHERAL2             RCC_PERIPHERAL_IOPA
#define RF_CTRL_PERIPHERAL3             RCC_PERIPHERAL_IOPB
#define RF_CTRL_PERIPHERAL4             RCC_PERIPHERAL_IOPC

// RF_RSTN引脚连接
#define RF_RSTN_BASE                    GPIOC_BASE
#define RF_RSTN_PIN                     GPIO_PIN_3
// RF_SPI SEL硬件连接
#define RF_SPI_SEL_BASE                 GPIOC_BASE
#define RF_SPI_SEL_PIN                  GPIO_PIN_5
// RF SLP_TR硬件连接
#define RF_SLP_TR_BASE                  GPIOC_BASE
#define RF_SLP_TR_PIN                   GPIO_PIN_2
// RF IRQ硬件连接
#define RF_IRQ_BASE                     GPIOB_BASE
#define RF_IRQ_PIN                      GPIO_PIN_0
#define RF_IRQ_LINE_PORT                EXTI_GPIO_B
#define RF_IRQ_LINE_NO                  0
#define RF_IRQ_INT_NUM                  ARCH_INT_EXTI0
// RF DIG2硬件连接
#define RF_DIG2_BASE                    GPIOA_BASE
#define RF_DIG2_PIN                     GPIO_PIN_4

/*********************************************************************************************************
  硬件基本宏函数
*********************************************************************************************************/
// 复位引脚操作
#define RF_RSTN_H()                     HWREG32(RF_RSTN_BASE + GPIO_O_BSRR) = RF_RSTN_PIN
#define RF_RSTN_L()                     HWREG32(RF_RSTN_BASE + GPIO_O_BRR) = RF_RSTN_PIN
// SPI片选引脚操作
#define RF_SPI_SEL_H()                  HWREG32(RF_SPI_SEL_BASE + GPIO_O_BSRR) = RF_SPI_SEL_PIN
#define RF_SPI_SEL_L()                  HWREG32(RF_SPI_SEL_BASE + GPIO_O_BRR) = RF_SPI_SEL_PIN
// SLP_TR引脚操作
#define RF_SLP_TR_H()                   HWREG32(RF_SLP_TR_BASE + GPIO_O_BSRR) = RF_SLP_TR_PIN
#define RF_SLP_TR_L()                   HWREG32(RF_SLP_TR_BASE + GPIO_O_BRR) = RF_SLP_TR_PIN
  
// 中断引脚读取
#define RF_IRQ_STATE()                  (!!(HWREG32(RF_IRQ_BASE + GPIO_O_IDR) & (RF_IRQ_PIN))) 
#define RF_IRQ_CLEAR()                  ExtiIntEventClear(RF_IRQ_LINE_NO)
  
// 帧开始标志状态
#define RF_FRAME_START_STATE()          (!!(HWREG32(RF_DIG2_BASE + GPIO_O_IDR) & (RF_DIG2_PIN))) 
// SLP_TR引脚当前状态
#define RF_SLP_TR_STATE()               (!!(HWREG32(RF_SLP_TR_BASE + GPIO_O_IDR) & (RF_SLP_TR_PIN)))

/*********************************************************************************************************
** AR86RF231寄存器定义
*********************************************************************************************************/
#define RG_TRX_STATUS                   (0x01)
#define SR_TRX_STATUS                   0x01, 0x1f, 0
#define SR_RESERVED_01_3                0x01, 0x20, 5
#define SR_CCA_STATUS                   0x01, 0x40, 6
#define SR_CCA_DONE                     0x01, 0x80, 7
#define RG_TRX_STATE                    (0x02)
#define SR_TRX_CMD                      0x02, 0x1f, 0
#define SR_TRAC_STATUS                  0x02, 0xe0, 5
#define RG_TRX_CTRL_0                   (0x03)
#define SR_CLKM_CTRL                    0x03, 0x07, 0
#define SR_CLKM_SHA_SEL                 0x03, 0x08, 3
#define SR_PAD_IO_CLKM                  0x03, 0x30, 4
#define SR_PAD_IO                       0x03, 0xc0, 6
#define RG_TRX_CTRL_1                   (0x04)
#define SR_IRQ_POLARITY                 0x04, 0x01, 0
#define SR_IRQ_MASK_MODE                0x04, 0x02, 1
#define SR_SPI_CMD_MODE                 0x04, 0x0c, 2
#define SR_RX_BL_CTRL                   0x04, 0x10, 4
#define SR_TX_AUTO_CRC_ON               0x04, 0x20, 5
#define SR_IRQ_2_EXT_EN                 0x04, 0x40, 6
#define SR_PA_EXT_EN                    0x04, 0x80, 7
#define RG_PHY_TX_PWR                   (0x05)
#define SR_TX_PWR                       0x05, 0x1f, 0     // AT86RF21x专用
#define SR_GC_PA                        0x05, 0x60, 5     // AT86RF21x专用
#define SR_PA_BOOST                     0x05, 0x80, 7     // AT86RF21x专用
//#define SR_TX_PWR                       0x05, 0x0f, 0   // AT86RF23x专用
//#define SR_PA_LT                        0x05, 0x30, 4   // AT86RF23x专用
//#define SR_PA_BUF_LT                    0x05, 0xc0, 6   // AT86RF23x专用
#define RG_PHY_RSSI                     (0x06)
#define SR_RSSI                         0x06, 0x1f, 0
#define SR_RND_VALUE                    0x06, 0x60, 5
#define SR_RX_CRC_VALID                 0x06, 0x80, 7
#define RG_PHY_ED_LEVEL                 (0x07)
#define SR_ED_LEVEL                     0x07, 0xff, 0
#define RG_PHY_CC_CCA                   (0x08)
#define SR_CHANNEL                      0x08, 0x1f, 0
#define SR_CCA_MODE                     0x08, 0x60, 5
#define SR_CCA_REQUEST                  0x08, 0x80, 7
#define RG_CCA_THRES                    (0x09)
#define SR_CCA_ED_THRES                 0x09, 0x0f, 0
#define SR_RESERVED_09_1                0x09, 0xf0, 4
#define RG_RX_CTRL                      (0x0a)
#define SR_PDT_THRES                    0x0a, 0x0f, 0
#define SR_RESERVED_0a_1                0x0a, 0xf0, 4
#define RG_SFD_VALUE                    (0x0b)
#define SR_SFD_VALUE                    0x0b, 0xff, 0
#define RG_TRX_CTRL_2                   (0x0c)
//#define SR_OQPSK_DATA_RATE              0x0c, 0x03, 0  // AT86RF23x专用
//#define SR_RESERVED_0c_2                0x0c, 0x7c, 2  // AT86RF23x专用
#define SR_RX_SAFE_MODE                 0x0c, 0x80, 7
#define SR_PHY_MODE                     0x0C, 0x3f, 0  // AT86RF21x专用
#define RG_ANT_DIV                      (0x0d)
#define SR_ANT_CTRL                     0x0d, 0x03, 0
#define SR_ANT_EXT_SW_EN                0x0d, 0x04, 2
#define SR_ANT_DIV_EN                   0x0d, 0x08, 3
#define SR_RESERVED_0d_2                0x0d, 0x70, 4
#define SR_ANT_SEL                      0x0d, 0x80, 7
#define RG_IRQ_MASK                     (0x0e)
#define SR_IRQ_MASK                     0x0e, 0xff, 0
#define RG_IRQ_STATUS                   (0x0f)
#define SR_IRQ_0_PLL_LOCK               0x0f, 0x01, 0
#define SR_IRQ_1_PLL_UNLOCK             0x0f, 0x02, 1
#define SR_IRQ_2_RX_START               0x0f, 0x04, 2
#define SR_IRQ_3_TRX_END                0x0f, 0x08, 3
#define SR_IRQ_4_CCA_ED_DONE            0x0f, 0x10, 4
#define SR_IRQ_5_AMI                    0x0f, 0x20, 5
#define SR_IRQ_6_TRX_UR                 0x0f, 0x40, 6
#define SR_IRQ_7_BAT_LOW                0x0f, 0x80, 7
#define RG_VREG_CTRL                    (0x10)
#define SR_RESERVED_10_6                0x10, 0x03, 0
#define SR_DVDD_OK                      0x10, 0x04, 2
#define SR_DVREG_EXT                    0x10, 0x08, 3
#define SR_RESERVED_10_3                0x10, 0x30, 4
#define SR_AVDD_OK                      0x10, 0x40, 6
#define SR_AVREG_EXT                    0x10, 0x80, 7
#define RG_BATMON                       (0x11)
#define SR_BATMON_VTH                   0x11, 0x0f, 0
#define SR_BATMON_HR                    0x11, 0x10, 4
#define SR_BATMON_OK                    0x11, 0x20, 5
#define SR_RESERVED_11_1                0x11, 0xc0, 6
#define RG_XOSC_CTRL                    (0x12)
#define SR_XTAL_TRIM                    0x12, 0x0f, 0
#define SR_XTAL_MODE                    0x12, 0xf0, 4
#define RG_CC_CTRL_0                    (0x13)          // AT86RF21x专用
#define SR_CC_NUMBER                    0x13, 0xff, 0  // AT86RF21x专用 
#define RG_CC_CTRL_1                    (0x14)          // AT86RF21x专用
#define SR_CC_BAND                      0x16, 0x07, 0   // AT86RF21x专用
#define RG_RX_SYN                       (0x15)
#define SR_RX_PDT_LEVEL                 0x15, 0x0f, 0
#define SR_RESERVED_15_2                0x15, 0x70, 4
#define SR_RX_PDT_DIS                   0x15, 0x80, 7
#define RG_RF_CTRL_0                    (0x16)          // AT86RF21x专用
#define SR_GC_TX_OFFS                   0x16, 0x03, 0   // AT86RF21x专用
#define SR_PA_LT                        0x16, 0xC0, 6   // AT86RF21x专用
#define RG_XAH_CTRL_1                   (0x17)
#define SR_RESERVED_17_8                0x17, 0x01, 0
#define SR_AACK_PROM_MODE               0x17, 0x02, 1
#define SR_AACK_ACK_TIME                0x17, 0x04, 2
#define SR_RESERVED_17_5                0x17, 0x08, 3
#define SR_AACK_UPLD_RES_FT             0x17, 0x10, 4
#define SR_AACK_FLTR_RES_FT             0x17, 0x20, 5
#define SR_RESERVED_17_2                0x17, 0x40, 6
#define SR_RESERVED_17_1                0x17, 0x80, 7
#define RG_FTN_CTRL                     (0x18)
#define SR_RESERVED_18_2                0x18, 0x7f, 0
#define SR_FTN_START                    0x18, 0x80, 7
#define RG_PLL_CF                       (0x1a)
#define SR_RESERVED_1a_2                0x1a, 0x7f, 0
#define SR_PLL_CF_START                 0x1a, 0x80, 7
#define RG_PLL_DCU                      (0x1b)
#define SR_RESERVED_1b_3                0x1b, 0x3f, 0
#define SR_RESERVED_1b_2                0x1b, 0x40, 6
#define SR_PLL_DCU_START                0x1b, 0x80, 7
#define RG_PART_NUM                     (0x1c)
#define SR_PART_NUM                     0x1c, 0xff, 0
#define RG_VERSION_NUM                  (0x1d)
#define SR_VERSION_NUM                  0x1d, 0xff, 0
#define RG_MAN_ID_0                     (0x1e)
#define SR_MAN_ID_0                     0x1e, 0xff, 0
#define RG_MAN_ID_1                     (0x1f)
#define SR_MAN_ID_1                     0x1f, 0xff, 0
#define RG_SHORT_ADDR_0                 (0x20)
#define SR_SHORT_ADDR_0                 0x20, 0xff, 0
#define RG_SHORT_ADDR_1                 (0x21)
#define SR_SHORT_ADDR_1                 0x21, 0xff, 0
#define RG_PAN_ID_0                     (0x22)
#define SR_PAN_ID_0                     0x22, 0xff, 0
#define RG_PAN_ID_1                     (0x23)
#define SR_PAN_ID_1                     0x23, 0xff, 0
#define RG_IEEE_ADDR_0                  (0x24)
#define SR_IEEE_ADDR_0                  0x24, 0xff, 0
#define RG_IEEE_ADDR_1                  (0x25)
#define SR_IEEE_ADDR_1                  0x25, 0xff, 0
#define RG_IEEE_ADDR_2                  (0x26)
#define SR_IEEE_ADDR_2                  0x26, 0xff, 0
#define RG_IEEE_ADDR_3                  (0x27)
#define SR_IEEE_ADDR_3                  0x27, 0xff, 0
#define RG_IEEE_ADDR_4                  (0x28)
#define SR_IEEE_ADDR_4                  0x28, 0xff, 0
#define RG_IEEE_ADDR_5                  (0x29)
#define SR_IEEE_ADDR_5                  0x29, 0xff, 0
#define RG_IEEE_ADDR_6                  (0x2a)
#define SR_IEEE_ADDR_6                  0x2a, 0xff, 0
#define RG_IEEE_ADDR_7                  (0x2b)
#define SR_IEEE_ADDR_7                  0x2b, 0xff, 0
#define RG_XAH_CTRL_0                   (0x2c)
#define SR_SLOTTED_OPERATION            0x2c, 0x01, 0
#define SR_MAX_CSMA_RETRIES             0x2c, 0x0e, 1
#define SR_MAX_FRAME_RETRIES            0x2c, 0xf0, 4
#define RG_CSMA_SEED_0                  (0x2d)
#define SR_CSMA_SEED_0                  0x2d, 0xff, 0
#define RG_CSMA_SEED_1                  (0x2e)
#define SR_CSMA_SEED_1                  0x2e, 0x07, 0
#define SR_AACK_I_AM_COORD              0x2e, 0x08, 3
#define SR_AACK_DIS_ACK                 0x2e, 0x10, 4
#define SR_AACK_SET_PD                  0x2e, 0x20, 5
#define SR_AACK_FVN_MODE                0x2e, 0xc0, 6
#define RG_CSMA_BE                      (0x2f)
#define SR_MIN_BE                       0x2f, 0x0f, 0
#define SR_MAX_BE                       0x2f, 0xf0, 4

#define CMD_REG                         0x80
#define CMD_REG_MASK                    0x3f
#define CMD_WRITE                       0x40
#define CMD_FB                          0x20

#define IRQ_BAT_LOW                     (1 << 7)
#define IRQ_TRX_UR                      (1 << 6)
#define IRQ_AMI                         (1 << 5)
#define IRQ_CCA_ED                      (1 << 4)
#define IRQ_TRX_END                     (1 << 3)
#define IRQ_RX_START                    (1 << 2)
#define IRQ_PLL_UNL                     (1 << 1)
#define IRQ_PLL_LOCK                    (1 << 0)

#define IRQ_ACTIVE_HIGH                 0
#define IRQ_ACTIVE_LOW                  1

#define STATE_P_ON                      0x00 /* BUSY */
#define STATE_BUSY_RX                   0x01
#define STATE_BUSY_TX                   0x02
#define STATE_FORCE_TRX_OFF             0x03
#define STATE_FORCE_TX_ON               0x04 /* IDLE */
/* 0x05 */    /* INVALID_PARAMETER */
#define STATE_RX_ON                     0x06
/* 0x07 */    /* SUCCESS */
#define STATE_TRX_OFF                   0x08
#define STATE_TX_ON                     0x09
/* 0x0a - 0x0e */   /* 0x0a - UNSUPPORTED_ATTRIBUTE */
#define STATE_SLEEP                     0x0F
#define STATE_BUSY_RX_AACK              0x11
#define STATE_BUSY_TX_ARET              0x12
#define STATE_RX_AACK_ON                0x16
#define STATE_TX_ARET_ON                0x19
#define STATE_RX_ON_NOCLK               0x1C
#define STATE_RX_AACK_ON_NOCLK          0x1D
#define STATE_BUSY_RX_AACK_NOCLK        0x1E
#define STATE_TRANSITION_IN_PROGRESS    0x1F

/*********************************************************************************************************
  硬件延时参数定义，需要参看详细的硬件手册，这里是AT86RF212B的延时参数
*********************************************************************************************************/
#define TIME_TO_ENTER_P_ON              1000 /**<  Transition time from VCC is applied to P_ON - most favorable case! */
#define TIME_SLEEP_TO_TRX_OFF           420 /**<  Transition time from SLEEP to TRX_OFF. */
#define TIME_TRX_OFF_TO_SLEEP           1   /**<  35*1/fCLKM. This need fCLKM > 250 kHz. */
#define TIME_TRX_OFF_TO_PLL_ON          170 /**<  Transition time from TRX_OFF to PLL ON. */
#define TIME_PLL_ON_TO_TRX_OFF          1   /**<  Transition time from PLL_ON to TRX_OFF. */
#define TIME_TRX_OFF_TO_RX_ON           170 /**<  Transition time from TRX_OFF to RX_ON. */
#define TIME_RX_ON_TO_TRX_OFF           1   /**<  Transition time from RX_ON to TRX_OFF. */
#define TIME_PLL_ON_TO_RX_ON            1   /**<  Transition time from PLL_ON to RX_ON. */
#define TIME_RX_ON_TO_PLL_ON            1   /**<  Transition time from RX_ON to PLL_ON. */
#define TIME_PLL_ON_TO_BUSY_TX          1   /**<  Transition time from PLL_ON to BUSY_TX. */
#define TIME_BUSY_TX_TO_PLL_ON          32  /**<  Transition time from BUSY_TX to PLL_ON. */
#define TIME_ALL_TO_TRX_OFF             1   /**<  Transition time from ALL state to TRX_OFF. */
#define TIME_RESET_TO_TRX_OFF           26  /**<  Transition time from RESET to TRX_OFF. */
#define TIME_VARIOUS_TO_PLL_ON          1   /**<  Transition time from Various states to PLL_ON. */

/*********************************************************************************************************
  定义驱动特征结构
*********************************************************************************************************/
typedef struct {
    rhb_spi_bus *spi_bus;                    // 定义SPI总线适配器选择
    void (*spi_sel_enable)(void);           // spi设备的片选使能
    void (*spi_sel_disable)(void);          // spi设备的片选禁止
    void (*dev_slp_tr_high)(void);          // 引脚SLP_TR设置为高
    void (*dev_slp_tr_low)(void);           // 引脚SLP_TR设置为低
    int  (*dev_slp_tr_state)(void);         // SLP TR引脚当前电平状态
}tAt86RFInfo;

/*********************************************************************************************************
  实现外部函数声明
*********************************************************************************************************/
extern const struct radio_driver at86rf231_rf_driver;
extern void ieee_set_pan_addr(uint16_t pan, uint16_t addr,const uint8_t *ieee_addr);
extern void ieee_set_channel(int channel);


#ifdef __cplusplus
    }
#endif      //  END OF __cplusplus

#endif      // END OF __AT86RF231_DRV_H__

/*********************************************************************************************************
** End of File
*********************************************************************************************************/